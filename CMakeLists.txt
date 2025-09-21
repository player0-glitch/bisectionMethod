cmake_minimum_required(VERSION 3.16)
project(bisection_method)

#Clangd compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Setting the source files for the project 
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

#Check and ask user to install external packages/executables
#figlet
find_program(FIGLET_EXECUTABLE figlet)
message(STATUS "Found figlet: ${FIGLET_EXECUTABLE}")
if(NOT FIGLET_EXECUTABLE)
    message(WARNING "figlet not found. Please install figlet.")
endif()

#pdflatex (this one is very important for the program)
find_program(PDFLATEX_EXECUTABLE pdflatex)
message(STATUS "Found pdflatex: ${PDFLATEX_EXECUTABLE}")

if(NOT PDFLATEX_EXECUTABLE)
    message(FATAL_ERROR "pdflatex not found. Please install pdflatex.")
endif()

#look for ccache on the user's system if available to help with fast compilation 
find_program(CCACHE_PROGRAM ccache)
message(STATUS "Found ccache to try and help with compile times: ${CCACHE_PROGRAM}")

if(CCACHE_PROGRAM)
    message(STATUS "Found ccache: ${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
endif()

#adding executable
add_executable(${PROJECT_NAME} ${SOURCES})

#include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

#for header files that do not change often 
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/exprtk.hpp)


target_compile_options(${PROJECT_NAME} PRIVATE -std=c++23 -Wall -Wextra -Wpedantic -g -funroll-all-loops -O0)
