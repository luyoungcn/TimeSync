cmake_minimum_required(VERSION 3.10)
project(TimeSync)

set(CMAKE_CXX_STANDARD 11)

if (WIN32)
find_package(cppzmq CONFIG REQUIRED)
add_executable(TimeSync main.cpp send_time2brd.cpp)
target_link_libraries(TimeSync PRIVATE cppzmq cppzmq-static)
else ()
add_executable(TimeSync main.cpp send_time2brd.cpp)
target_link_libraries(TimeSync zmq)
endif ()