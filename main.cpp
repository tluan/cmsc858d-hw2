#include <sdsl/bit_vectors.hpp>
#include <iostream>
#include "xxhash.h"
#include "khash.h"
#include <vector>
#include <map>
#include <iostream>
#include <ctime>
#include <unordered_map>
#include <unistd.h>
#include <sys/time.h>
//XXH64_hash_t XXH64(const char input[14], size_t i, int i1);

using namespace std;
using namespace sdsl;



long int now_ms() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
                "0123456789"
                "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}



std::string random_string2( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] ="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}
    class BBhash{
    vector<int> seeds;
    vector<bit_vector>A;
//    vector<bit_vector>C;
    float gamma = 3;
    vector<string> inKeys;
    map<string,  uint64_t> trad;
public:
    BBhash(vector<string> keys,vector<int> seeds);
    void setGamma(int inGamma);
    vector<string> level(int seed,vector<string> F) ;
    uint64_t query(string key);
    void run();

};

BBhash::BBhash(vector<string> keys,vector<int> seeds) {
    this->inKeys = keys;
    this->seeds = seeds;


}

void BBhash::setGamma(int inGamma) {
    this->gamma = inGamma;
}

vector<string> BBhash::level(int seed,vector<string> F) {
    if(F.size()==0){
        return {};
    }
    int size_p =this->gamma*F.size();
    bit_vector b(size_p, 0);

    bit_vector C(size_p , 0);
    vector<string> result = {};
    vector<string> v(size_p);
    for(string k:F){

        int n = k.length();
        char char_array[n + 1];
        strcpy(char_array, k.c_str());
        XXH64_hash_t hash = XXH64(char_array, k.length(), seed);

        int hash_index = hash%size_p;
//        cout<<"here"<< k<<seed<<" "<<hash<<endl;
//        cout<<hash_index<<endl;
        if(C[hash_index]==1){
            b[hash_index] =0;
             result.push_back(k);
        }
        if(b[hash_index]==0 &&C[hash_index]==0){
            b[hash_index]=1;
            v[hash_index] = k;
        }else if(b[hash_index]==1 &&C[hash_index]==0){
            b[hash_index]=0;
            C[hash_index]=1;
            result.push_back(v[hash_index]);
            result.push_back(k);

        }

    }
    A.push_back(b);
    for(int i = 0; i < result.size();i ++){
//        cout<<result[i]<<endl;
    }
//    cout<<"b"<<b<<endl;
    return result;
}
void BBhash::run(){
    vector<string> level1 = level(seeds[0],this->inKeys);
    vector<string> level2 = level(seeds[1],level1);
    vector<string> level3 = level(seeds[2],level2);
    if(level3.size()!=0){
        int remaine = this->inKeys.size();
        cout<<"remain"<<level3.size()<<endl;
        for(int i = 0; i < level3.size();i++){
            trad[level3[i]]=remaine;
            remaine = remaine -1;

        }
    }
    cout<<"size"<<level1.size()<<" "<<level2.size()<<" "<< level3.size()<<endl;


}
uint64_t BBhash::query(string key){
    int n = key.length();
    char char_array[n + 1];
    strcpy(char_array, key.c_str());

    int sum= 0;
    for(int i = 0;i<this->seeds.size();i++) {
        XXH64_hash_t hash = XXH64(char_array, key.length(), seeds[i]);
//        cout<<"test3"<<i<<endl;

        bit_vector curr = A[i];
//        cout<<"test5"<<endl;
        uint64_t index= (uint64_t)(hash%curr.size());
//        cout<<"index"<<index<<endl;
//        cout<<"test4"<<endl;
        rank_support_v<> rc(&curr);
        if(curr[index] == 1 ){

//            cout<<"A"<<curr<<endl;
//            int sum= 0;
//            for(int j = 0;j<i;j++){
//                bit_vector temp = A[j];
//                rank_support_v<> rb(&temp);
//                sum = sum + rb(temp.size())
//            }

            sum = sum + rc(index+1);
//            cout<<"here"<<key<<seeds[i]<<" "<< hash<<endl;
            return  sum;
        }
        sum = sum + rc(curr.size());

//        cout<<"test1"<<endl;

    }
//    cout<<"test2"<<endl;
    if (trad.count(key) > 0){
        return sum + trad[key];
    }else{
        return (numeric_limits<uint64_t>::max());
    }
    return (numeric_limits<uint64_t>::max());

}


int main()
{   //cout<<gen_random(4)<<endl;
    vector<string> gl;
    std::unordered_map<std::string, std::string> u = {
    };

    for(int i = 0;i < 200;i ++){
        string f = random_string(4);
       gl.push_back(f);
       u[f] = i;
    }

    //vector<string> gl = {"test","test2","test3","test4","rujX"};
    vector<int>seeds ={1,2,3};
    BBhash tes1 = BBhash(gl,seeds);
    tes1.run();
    long int run_time = 0;
    long int start_ms = now_ms();
    for(int i = 0; i < 100000;i ++){

        int randm =  rand() % (gl.size());
        tes1.query(gl[randm]);

    }

    for(int i = 0; i < 1000; i ++){
        tes1.query(random_string2(4));
    }

    long int end_ms = now_ms();
    run_time += (end_ms - start_ms);
    cout<<"runtime"<<run_time<<endl;

//    long int run_time2 = 0;
//    long int start_ms2 = now_ms();
//    for(int i = 0; i < 10;i ++){
//        int randm =  rand() % (gl.size());
//        string f = gl[randm];
//        u[f];
//    }
//
//    long int end_ms2 = now_ms();
//    run_time2 += (end_ms2 - start_ms2);
//    cout<<"runtime"<<run_time2<<endl;






}
