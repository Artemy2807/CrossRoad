cmake_minimum_required(VERSION 2.8) 

project(client)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -std=c++11 -Wall -g")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

find_package(OpenCV)

set(SOURCES main.cpp
    client.cpp
    system.cpp
)

include_directories(
	${OpenCV_INCLUDES}
)

add_executable(client ${SOURCES})

target_link_libraries(client
	${OpenCV_LIBS} 
) 
