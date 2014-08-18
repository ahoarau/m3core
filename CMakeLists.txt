cmake_minimum_required(VERSION 2.8)
project(m3core)
SET(MAJOR_VERSION "1")
SET(MINOR_VERSION "9")
SET(PATCH_VERSION "3")
set(M3CORE_VERSION
  ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

set(KERNEL_NAME "" CACHE STRING "The rtai patched kernel to use. Ex:-DKERNEL_NAME=\"3.8.13-rtmeka4.0\" (default is uname -a) ")
#OPTION(KERNEL_NAME)

if(KERNEL_NAME)
set(CMAKE_SYSTEM_VERSION ${KERNEL_NAME})
message("-- Custom kernel : ${KERNEL_NAME}")
endif()


OPTION(RTAI "Enable/Disable RTAI (used for python proxy users only)" ON)

OPTION(ETHERCAT "Enable/Disable EtherCAT (sim or not)" OFF)

OPTION(VIRTUAL_MEKA "To be used on simulation or not" ON)
if(NOT VIRTUAL_MEKA)
add_definitions("-D__KERNEL_SYNC__")
endif()

# Tmp solution for the FindXXX.cmake 
set(CMAKE_MODULE_PATH 
    "${CMAKE_CURRENT_SOURCE_DIR}/../meka-cmake-modules/Modules/"
    ${CMAKE_MODULE_PATH})

set(M3RT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Include dirs to look for
include_directories(
    ${M3RT_SRC}
    ${CMAKE_CURRENT_BINARY_DIR}/src # Include this directory for the generated protobufs headers
)

add_subdirectory(scripts)
add_subdirectory(src)
add_subdirectory(python)

configure_file(setup.bash.in ${CMAKE_CURRENT_BINARY_DIR}/setup.bash)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/setup.bash DESTINATION ${CMAKE_INSTALL_PREFIX}/share/m3)
 
IF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
INCLUDE(InstallRequiredSystemLibraries)
 
SET(CPACK_SET_DESTDIR "on")
SET(CPACK_PACKAGING_INSTALL_PREFIX "/tmp")
SET(CPACK_GENERATOR "DEB")
 
SET(CPACK_PACKAGE_DESCRIPTION "m3core : core components to control the meka robots")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Real-time control system for robot control - interfaced with ROS")
SET(CPACK_PACKAGE_VENDOR "Antoine Hoarau")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Antoine Hoarau <hoarau.robotics@gmail.com>")
SET(CPACK_PACKAGE_CONTACT "hoarau.robotics@gmail.com")
SET(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
#demo dependencies
#SET(CPACK_DEBIAN_PACKAGE_DEPENDS " kate , plasma-desktop, libc6 (>= 2.1.3) ")
 
#dependencies for this service menu
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "python2.7,libqt4-dev,moc,g++,libncurses5-dev,kernel-package,gcc-multilib,libc6-dev,libtool,automake,cmake,git,openssh-server,openssh-client,libeigen3-dev,libprotobuf-dev,protobuf-compiler,libboost-dev,protobuf-compiler,python-dev,libprotobuf-dev,python-protobuf,python-matplotlib,python-yaml,python-gnuplot,python-scipy,python-sip-dev,python-sip,sip-dev ")
 
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_SECTION "libraries")
SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
 
#SET(CPACK_COMPONENTS_ALL Libraries ApplicationData)
INCLUDE(CPack)
 
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
