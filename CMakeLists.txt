cmake_minimum_required(VERSION 3.20)
project(grid_code_manager)

set(CMAKE_CXX_STANDARD 17)

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else ()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif ()

include_directories(
        lib
        api
)

add_library(log INTERFACE lib/log.h)

add_executable(main main.cpp)
target_link_libraries(main ${CONAN_LIBS})
