cmake_minimum_required(VERSION 2.8)
project(base)
set(SUBPROJECT_INSTALL_DIR_NAME m3rt)
set(SUBPROJECT_INSTALL_NAME ${PROJECT_NAME})
set(LIBNAME "m3base")

OPTION(RTAI "Enable/Disable RTAI (used for python proxy users only)" ON)


message("RTAI:${RTAI}")

#set(CMAKE_CXX_FLAGS "-Wall -std=c++0x")
#set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
if(RTAI)
find_package(RTAI REQUIRED)
add_definitions(-D__RTAI__)
endif(RTAI)
#find_package(M3 COMPONENTS HARDWARE ROBOTS CHAINS TOOLBOX SHARED_MEM REQUIRED)
#find_package(M3rt REQUIRED)
find_package(Boost  REQUIRED)


## YAML CPP 
find_package(PkgConfig REQUIRED)
#find_package(YamlCpp  REQUIRED)
pkg_check_modules(YAML_CPP REQUIRED yaml-cpp<=0.5)
if(NOT ${YAML_CPP_VERSION} VERSION_LESS "0.5")
add_definitions(-DHAVE_NEW_YAMLCPP)
endif(NOT ${YAML_CPP_VERSION} VERSION_LESS "0.5")

#find_package(Eigen3 REQUIRED)
find_package(Protobuf REQUIRED)

# Start Protobuf stuff
include_directories(${PROTOBUF_INCLUDE_DIR} ${M3RT_SOURCE_DIR})
file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.proto" )
#file(GLOB ProtoFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} [^.]*.proto)
message("ProtoFiles:${ProtoFiles} to ${CMAKE_CURRENT_SOURCE_DIR} ")
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${CMAKE_CURRENT_SOURCE_DIR} ${ProtoFiles})
add_custom_target(${LIBNAME}_protobuf_output ALL DEPENDS ${ProtoSources}) # force the generation

find_package(PythonLibs 2.7 REQUIRED)
find_package(PythonInterp REQUIRED) 
## Generate and copy the python protobuf
PROTOBUF_GENERATE_PYTHON(PySources ${CMAKE_CURRENT_BINARY_DIR} ${ProtoFiles}) 
add_custom_target(python_protobuf_output ALL DEPENDS ${PySources}) # force the generation


# End Protobuf stuff

SET(LIBS ${YAMLCPP_LIBRARY} ${PROTOBUF_LIBRARIES} ${RTAI_LIBRARIES}  pthread ${Boost_LIBRARIES} ${EIGEN3_LIBRARIES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ ${M3RT_INCLUDE_DIR} ${YAMLCPP_INCLUDE_DIR} ${RTAI_INCLUDE_DIR} ${THREADS_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR} ${PROTOBUF_INCLUDE_DIRS})

## Get all the files sources and headers

set( all_srcs
component_async.cpp
component.cpp
component_ec.cpp
component_factory.cpp
component_shm.cpp
simple_server.cpp
toolbox.cpp
)
set(all_hdrs
component_async.h
component_ec.h
component_factory.h
component.h
component_shm.h
m3ec_def.h
m3rt_def.h
simple_server.h
toolbox.h
)

list(APPEND 
all_srcs
${ProtoSources}
)

#file(GLOB ProtoFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.proto )
list(APPEND all_hdrs
${ProtoHeaders}
${ProtoFiles}
)

add_library(${LIBNAME} SHARED ${all_srcs} )
target_link_libraries(${LIBNAME} ${LIBS})
#set_target_properties(${LIBNAME} PROPERTIES LINKER_LANGUAGE CXX)


install(TARGETS ${LIBNAME} DESTINATION lib)
install(FILES ${all_hdrs} DESTINATION include/${SUBPROJECT_INSTALL_DIR_NAME}/${SUBPROJECT_INSTALL_NAME})

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


install ( FILES ${PySources} 
   DESTINATION ${PYTHON_SITE_DIR}/m3/
   COMPONENT library 
) 


