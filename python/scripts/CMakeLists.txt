

cmake_minimum_required(VERSION 2.8)
project(m3core-python-scripts)

file(GLOB ${PROJECT_NAME}_files
    "m3rt/*.py"
    "dev/*.py"
)
message("Scripts:${${PROJECT_NAME}_files}")

install (
	FILES ${${PROJECT_NAME}_files} 
	PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ WORLD_EXECUTE
	DESTINATION bin
)




