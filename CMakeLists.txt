cmake_minimum_required(VERSION 2.8)
project(fdt_client)

set(CMAKE_CXX_FLAGS "-g -Wall -pedantic -std=c++0x")
#set(CMAKE_VERBOSE_MAKEFILE on)
set(EXECUTABLE_OUTPUT_PATH bin)

include_directories(src)

set(OBJS
    src/types.cpp
    src/http.cpp
    src/parsers/panel.cpp
)

add_executable(main ${OBJS} src/main.cpp)

target_link_libraries(main curl re2)
