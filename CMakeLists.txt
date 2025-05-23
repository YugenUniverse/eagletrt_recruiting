cmake_minimum_required(VERSION 3.21)
project(Project2 VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread -O0 -g -w")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    # ${CMAKE_CURRENT_SOURCE_DIR}/other/dirs
)

add_library(
    ${PROJECT_NAME}
    SHARED
    
    # Sources
    ${CMAKE_CURRENT_SOURCE_DIR}/fake_receiver.c
)

target_link_libraries(${PROJECT_NAME}
    # Other librs
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(project_2 main.cpp
        Data_Unit.h
        Data_Unit.cpp
        compute.cpp
        compute.h
        Stat_Unit.h
        Stat_Unit.cpp
)
target_link_libraries(project_2 ${PROJECT_NAME})

add_custom_command(TARGET project_2 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/logs
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/stats
)