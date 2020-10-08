cmake_minimum_required(VERSION 2.8) 

project(server)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -std=c++11 -Wall -g")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

find_package(OpenCV)

set(LIBS ${LIBS}
	pthread
)

set(SOURCES main.cpp
    server.cpp
    recognitionVehicle.cpp
    Trafficlight.cpp
)

include_directories(
	${OpenCV_INCLUDES}
)

add_executable(server ${SOURCES})

target_link_libraries(server
	${LIBS}
	${OpenCV_LIBS} 
)
