cmake_minimum_required(VERSION 2.8)
project(ethercat_fake)


#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# RTAI
find_package(RTAI REQUIRED)
add_definitions(-D__RTAI__)
add_definitions(-D__IN_RTAI__)
include_directories(${RTAI_INCLUDE_DIR})
# end RTAI

add_definitions(-ffast-math)


Set( MODULE_OUTPUT_FILES    m3ec.o
                            m3ec.mod.c
                            m3ec.mod.o
                            m3ec.ko )

Set( MODULE_SOURCE_FILES  Kbuild
			m3ec.c )

Set( DRIVER_FILE        m3ec.ko )
Set( DRIVER_TARGET_NAME m3ec-module )
Set( DRIVER_BIN_FILE    ${CMAKE_CURRENT_BINARY_DIR}/${DRIVER_FILE} )
Set( MODULE_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR} )

Set( KERNEL_DIR "/lib/modules/${CMAKE_SYSTEM_VERSION}/build" )
message("-- Kernel dir : ${KERNEL_DIR} (Should be rtai-patched)")
Set( KBUILD_CMD ${CMAKE_MAKE_PROGRAM}
                -C ${KERNEL_DIR}
                M=${MODULE_SOURCE_DIR}
                modules )

Add_Custom_Command( OUTPUT  ${DRIVER_BIN_FILE}
                            ${MODULE_OUTPUT_FILES}
                    COMMAND ${KBUILD_CMD}
		    COMMAND cp -f ${DRIVER_FILE} ${DRIVER_BIN_FILE}
                    WORKING_DIRECTORY ${MODULE_SOURCE_DIR}
                    DEPENDS ${MODULE_SOURCE_FILES} VERBATIM )

Add_Custom_Target ( ${DRIVER_TARGET_NAME}
                    ALL
                    DEPENDS ${DRIVER_BIN_FILE} )
Install(FILES ${DRIVER_FILE} DESTINATION /lib/modules/${CMAKE_SYSTEM_VERSION}/extra/)
