cmake_minimum_required(VERSION 2.8)
project(rt_system)
set(LIBNAME "rt_system")

OPTION(RTAI "Enable/Disable RTAI (used for python proxy users only)" ON)
OPTION(ROS "Enable/Disable ROS" OFF)

INCLUDE_DIRECTORIES(${M3RT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}../base)

#set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
if(RTAI)
add_definitions(-D__RTAI__)
find_package(RTAI REQUIRED)
SET(LIBS ${LIBS} ${RTAI_LIBRARIES})
include_directories(${RTAI_INCLUDE_DIR})
endif(RTAI)

if(ROS)
add_definitions(-D_ROS_)
endif(ROS)



find_package(Threads)


#### YAML-CPP <=0.5
find_package(PkgConfig REQUIRED)
#find_package(YamlCpp  REQUIRED)
pkg_check_modules(YAMLCPP REQUIRED yaml-cpp<=0.5)
if(NOT ${YAMLCPP_VERSION} VERSION_LESS "0.5")
add_definitions(-DHAVE_NEW_YAMLCPP)
endif(NOT ${YAMLCPP_VERSION} VERSION_LESS "0.5")
link_directories(${YAMLCPP_LIBRARY_DIRS})



find_package(Protobuf REQUIRED)

SET(LIBS ${LIBS} ${YAMLCPP_LIBRARY_DIRS} ${YAMLCPP_LIBRARIES} ${PROTOBUF_LIBRARIES}   pthread ${Boost_LIBRARIES} ${EIGEN3_LIBRARIES} m3base)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ ${YAMLCPP_INCLUDE_DIRS} ${M3RT_INCLUDE_DIR}  ${THREADS_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR} ${PROTOBUF_INCLUDE_DIRS})




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
list(APPEND ALL_SRCS rt_ros_service.cpp)
list(APPEND ALL_HDRS rt_ros_service.h)
endif(ROS)



# Swig
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})
FIND_PACKAGE(PythonLibs 2.7 REQUIRED)
find_package ( PythonInterp REQUIRED ) 

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
SET(CMAKE_SWIG_FLAGS "")

set(M3_SWIG_MODULE_NAME "m3rt_system")


SET_SOURCE_FILES_PROPERTIES(${M3_SWIG_MODULE_NAME}.i PROPERTIES CPLUSPLUS ON)

SWIG_ADD_MODULE(${M3_SWIG_MODULE_NAME} python ${M3_SWIG_MODULE_NAME}.i ${ALL_SRCS})

SWIG_LINK_LIBRARIES(${M3_SWIG_MODULE_NAME} ${PYTHON_LIBRARIES} ${LIBS} )
set_target_properties(${SWIG_MODULE_${M3_SWIG_MODULE_NAME}_REAL_NAME} PROPERTIES LINKER_LANGUAGE CXX)
add_custom_target(${M3_SWIG_MODULE_NAME} ALL DEPENDS ${ALL_SRCS})

add_custom_target(${LIBNAME} ALL DEPENDS ${M3_SWIG_MODULE_NAME})
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
install ( TARGETS ${SWIG_MODULE_${M3_SWIG_MODULE_NAME}_REAL_NAME}
   LIBRARY 
     DESTINATION ${PYTHON_SITE_DIR}/m3/ 
     COMPONENT library 
) 

install ( FILES ${CMAKE_CURRENT_BINARY_DIR}/${M3_SWIG_MODULE_NAME}.py 
   DESTINATION ${PYTHON_SITE_DIR}/m3/
   COMPONENT library 
) 



