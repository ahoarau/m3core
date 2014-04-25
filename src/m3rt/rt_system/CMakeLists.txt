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


# Swig
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})
FIND_PACKAGE(PythonLibs REQUIRED)
find_package ( PythonInterp REQUIRED ) 

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
SET(CMAKE_SWIG_FLAGS "")
SET_SOURCE_FILES_PROPERTIES(m3rt_system.i PROPERTIES CPLUSPLUS ON)
#(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../python/m3/) # HACK for the path
####install(FILES ${CMAKE_BINARY_DIR}/src/target.py DESTINATION ${PYTHON_DIST_PACKAGES}/m3) #### TO TRY

SWIG_ADD_MODULE(m3rt_system python m3rt_system.i rt_system.cpp)

SWIG_LINK_LIBRARIES(m3rt_system ${PYTHON_LIBRARIES} ${LIBS} ${LIBNAME})
set_target_properties(${SWIG_MODULE_m3rt_system_REAL_NAME} PROPERTIES LINKER_LANGUAGE CXX)
# End Swig
# Start Protobuf stuff
#include_directories(${PROTOBUF_INCLUDE_DIR} ${M3RT_SOURCE_DIR})
#file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.proto" )
#if(ProtoFiles)	     
#	PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${CMAKE_CURRENT_SOURCE_DIR} ${ProtoFiles})
#endif(Protofiles)
# End Protobuf stuff

## Get all the files sources and headers
file(GLOB ALL_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} [^.]*.cpp)
file(GLOB ALL_HDRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} [^.]*.h)

if(NOT ROS)
message("Removing ROS stuff")
FILE(GLOB ROS_STUFF RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*_ros_*")
message("ros stuff to remove : ${ROS_STUFF}")
list(REMOVE_ITEM ALL_SRCS ${ROS_STUFF})
list(REMOVE_ITEM ALL_HDRS ${ROS_STUFF})
endif(NOT ROS)

list(APPEND ALL_SRCS ${ProtoSources})


file(GLOB ProtoFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.proto )
list(APPEND ALL_HDRS
${ProtoHeaders}
${ProtoFiles}
)

message("ALL SRC in ${PROJECT_NAME}:${ALL_SRCS}")


## Building
add_library(${LIBNAME} SHARED ${ALL_SRCS})
set_target_properties(${LIBNAME} PROPERTIES PREFIX "")
target_link_libraries(${LIBNAME} ${LIBS})
set_target_properties(${LIBNAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${LIBNAME} PROPERTIES COMPILE_FLAGS "-I${CMAKE_BINARY_DIR}/src")

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
message("Swig file out : ${SWIG_MODULE_m3rt_system_REAL_NAME}")
## Installation
install ( TARGETS ${SWIG_MODULE_m3rt_system_REAL_NAME} ${LIBNAME}
   LIBRARY 
     DESTINATION ${PYTHON_SITE_DIR}/m3/ 
     COMPONENT library 
) 

install ( FILES ${CMAKE_CURRENT_BINARY_DIR}/m3rt_system.py 
   DESTINATION ${PYTHON_SITE_DIR}/m3/
   COMPONENT library 
) 



#install(TARGETS ${LIBNAME} DESTINATION lib)
install(FILES ${ALL_HDRS} DESTINATION include/m3rt/${PROJECT_NAME})

