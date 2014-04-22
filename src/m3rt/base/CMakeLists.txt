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

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
if(RTAI)
add_definitions(-D__RTAI__)
find_package(RTAI REQUIRED)
endif(RTAI)
#find_package(M3 COMPONENTS HARDWARE ROBOTS CHAINS TOOLBOX SHARED_MEM REQUIRED)
#find_package(M3rt REQUIRED)
find_package(Boost  REQUIRED)
#find_package(Eigen3 REQUIRED)
find_package(Protobuf REQUIRED)

# Start Protobuf stuff
include_directories(${PROTOBUF_INCLUDE_DIR} ${M3RT_SOURCE_DIR})
file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.proto" )
#file(GLOB ProtoFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} [^.]*.proto)
message("ProtoFiles:${ProtoFiles} to ${CMAKE_CURRENT_SOURCE_DIR} ")
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${CMAKE_CURRENT_SOURCE_DIR} ${ProtoFiles})


## Generate and copy the python protobuf
PROTOBUF_GENERATE_PYTHON(PySources ${CMAKE_CURRENT_SOURCE_DIR}/../../../python/m3rt/ ${ProtoFiles}) # HACK on the path
add_custom_target(python_protobuf_output ALL DEPENDS ${PySources}) # Hack to force the generation


# End Protobuf stuff

SET(LIBS ${RTAI_LIBRARIES} ${M3RT_LIBRARIES} pthread ${Boost_LIBRARIES} ${EIGEN3_LIBRARIES})
include_directories(${M3RT_INCLUDE_DIR} ${RTAI_INCLUDE_DIR} ${THREADS_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR})

## Get all the files sources and headers
#file(GLOB ALL_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} [^.]*.cpp)
#file(GLOB ALL_HDRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} [^.]*.h)
file(GLOB all_srcs [^.]*.cpp)
file(GLOB all_hdrs [^.]*.h)
## Remove all generated protobufs froms above lists
#list(REMOVE_ITEM ALL_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.pb.cpp)
#list(REMOVE_ITEM ALL_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.pb.cc)
#list(REMOVE_ITEM ALL_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/*.pb.h)

list(APPEND 
all_srcs
${ProtoSources}
)

#file(GLOB ProtoFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.proto )
list(APPEND all_hdrs
${ProtoHeaders}
${ProtoFiles}
)

add_library(${LIBNAME} SHARED ${all_srcs} ) # big HACK : Adding the py to source so it gets generated
target_link_libraries(${LIBNAME} ${LIBS})
#set_target_properties(${LIBNAME} PROPERTIES LINKER_LANGUAGE CXX)
message("ALL SRCS in ${PROJECT_NAME}:${all_srcs}")

install(TARGETS ${LIBNAME} DESTINATION lib)
install(FILES ${all_hdrs} DESTINATION include/${SUBPROJECT_INSTALL_DIR_NAME}/${SUBPROJECT_INSTALL_NAME})




