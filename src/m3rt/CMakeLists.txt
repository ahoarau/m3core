


add_subdirectory(base)
add_subdirectory(rt_system)

if(NOT ETHERCAT)
    add_subdirectory(ethercat_fake)
    message("************************************")
    message("-- Ethercat is OFF : Virtual Meka")
    message("************************************")
else()
add_definitions(__ETHERCAT__)
    add_subdirectory(ethercat)
    message("************************************")
    message("-- Ethercat is ON : Real Meka Hardware")
    message("************************************")
endif()
 
