cmake_minimum_required(VERSION 2.8)
project(rt_system)
set(LIBNAME "rt_system")

OPTION(RTAI "Enable/Disable RTAI (used for python proxy users only)" ON)
OPTION(ROS "Enable/Disable ROS" OFF)

message("RTAI:${RTAI}")
message("ROS:${ROS}")
INCLUDE_DIRECTORIES(${M3RT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}../base)

#set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
if(RTAI)
add_definitions(-D__RTAI__)
find_package(RTAI REQUIRED)
endif(RTAI)

if(ROS)
add_definitions(-D_ROS_)
###find_package(RTAI REQUIRED)
endif(ROS)



find_package(Threads)
find_package(YamlCpp 0.3 REQUIRED)
#find_package(M3 COMPONENTS HARDWARE ROBOTS CHAINS TOOLBOX SHARED_MEM REQUIRED)
#find_package(M3rt REQUIRED )
#find_package(Boost COMPONENTS filesystem system REQUIRED)
#find_package(Eigen3 REQUIRED)
find_package(Protobuf REQUIRED)

SET(LIBS ${YAMLCPP_LIBRARY} ${PROTOBUF_LIBRARIES} ${RTAI_LIBRARIES} ${M3RT_LIBRARIES} pthread ${Boost_LIBRARIES} ${EIGEN3_LIBRARIES} m3base)
include_directories(${YAMLCPP_INCLUDE_DIR} ${M3RT_INCLUDE_DIR} ${RTAI_INCLUDE_DIR} ${THREADS_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR} ${PROTOBUF_INCLUDE_DIRS})



set(ALL_SRCS
rt_data_service.cpp
rt_log_service.cpp
rt_service.cpp
rt_system.cpp
)
set(ALL_HDRS
rt_data_service.h
rt_log_service.h
rt_service.h
rt_system.h
)

if(ROS)
list(REMOVE_ITEM ALL_SRCS rt_ros_service.cpp)
list(REMOVE_ITEM ALL_HDRS rt_ros_service.h)
endif(ROS)



# Swig
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})
FIND_PACKAGE(PythonLibs REQUIRED)
find_package ( PythonInterp REQUIRED ) 

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
SET(CMAKE_SWIG_FLAGS "")
SET_SOURCE_FILES_PROPERTIES(m3rt_system.i PROPERTIES CPLUSPLUS ON)

SWIG_ADD_MODULE(m3rt_system python m3rt_system.i rt_system.cpp ${ALL_SRCS})

SWIG_LINK_LIBRARIES(m3rt_system ${PYTHON_LIBRARIES} ${LIBS} )
set_target_properties(${SWIG_MODULE_m3rt_system_REAL_NAME} PROPERTIES LINKER_LANGUAGE CXX)
# End swig


#SET_TARGET_PROPERTIES( ${LIBNAME} PROPERTIES COMPILE_FLAGS -fPIC)
execute_process ( 
   COMMAND ${PYTHON_EXECUTABLE} -c 
   	"import site, sys; sys.stdout.write(site.PREFIXES[-1])" 
   OUTPUT_VARIABLE PYTHON_PREFIX 
) 
file ( TO_CMAKE_PATH "${PYTHON_PREFIX}" PYTHON_PREFIX ) 
execute_process ( 
   COMMAND ${PYTHON_EXECUTABLE} -c 
   	"import site, sys; sys.stdout.write(site.getsitepackages()[-1])" 
   OUTPUT_VARIABLE PYTHON_SITE_DIR 
) 

file ( TO_CMAKE_PATH "${PYTHON_SITE_DIR}" PYTHON_SITE_DIR ) 
string ( REGEX REPLACE "^${PYTHON_PREFIX}/" "" 
   PYTHON_SITE_DIR "${PYTHON_SITE_DIR}" 
) 


## Installation
install ( TARGETS ${SWIG_MODULE_m3rt_system_REAL_NAME}
   LIBRARY 
     DESTINATION ${PYTHON_SITE_DIR}/m3/ 
     COMPONENT library 
) 

install ( FILES ${CMAKE_CURRENT_BINARY_DIR}/m3rt_system.py 
   DESTINATION ${PYTHON_SITE_DIR}/m3/
   COMPONENT library 
) 



