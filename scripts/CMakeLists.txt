



set(M3RTSCRIPTS
disable_ros
lsec
m3rt_insmods
m3rt_rmmods
m3rt_rotate_logs
m3rt_server_debug
m3rt_server_kill
m3rt_server_run
m3rt_timestats_run
uninstall_m3
)

install (
	FILES ${M3RTSCRIPTS} 
	PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ WORLD_EXECUTE
	DESTINATION bin
)




