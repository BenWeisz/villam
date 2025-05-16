cmake_minimum_required( VERSION 3.20 )

set( EXE_NAME villam )
project( ${EXE_NAME} )

set( CMAKE_C_STANDARD 99 )
set( CMAKE_C_FLAGS "-Wall" )

add_executable( ${EXE_NAME} main.c src/io.c src/kvfile.c )
target_include_directories( ${EXE_NAME} PUBLIC include/ )
