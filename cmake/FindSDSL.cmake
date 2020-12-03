
# - Try to find SDSL
#
# The following variables are optionally searched for defaults
#  SDSL_ROOT_DIR:            Base directory where all GLOG components are found
#
# The following are set after configuration is done:
#  SDSL_FOUND
#  SDSL_INCLUDE_DIRS
#  SDSL_LIBRARIES

include(FindPackageHandleStandardArgs)

set(SDSL_ROOT_DIR "" CACHE PATH "Folder contains sdsl-lite")

find_path(SDSL_INCLUDE_DIR sdsl/config.hpp
        PATHS ${SDSL_ROOT_DIR})

find_library(SDSL_LIBRARY sdsl
        PATHS ${SDSL_ROOT_DIR})

if(SDSL_FOUND)
    set(SDSL_INCLUDE_DIRS ${SDSL_INCLUDE_DIR})
endif()
