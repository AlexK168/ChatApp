cmake_minimum_required(VERSION 3.16)
project(ConsoleChat)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread ")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})


add_executable(ConsoleChat main.cpp MsgQueue.cpp MsgQueue.h Session.cpp Session.h)
target_link_libraries(ConsoleChat ${CURSES_LIBRARIES})
