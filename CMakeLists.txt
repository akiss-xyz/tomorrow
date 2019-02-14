cmake_minimum_required(VERSION 3.1) # Just so cmake knows what standard's going on
set (CMAKE_CXX_STANDARD 17)


project(hello) # project name, for now



include_directories(include) # where our header files go - you can read up on these or ask me about them. probs best to do both.
file(GLOB_RECURSE SOURCES src/*.cpp) # recursively search our src folder for c++ files

#cimg library
include_directories(vendor/CImg-2.4.3) # literally never used this before

# Compilation
if ( CMAKE_COMPILER_IS_GNUCC )
    # Just for testing, so we know what compiler we're using
    add_compile_definitions(GCC)
    # Adding some compiler flags - we want as many errors and suggestions and information from the compiler as possible
    # Added nounusused parameter because honestly wth
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-unused-parameter")
endif()
if ( MSVC )
    # Once again, just so we know what's good
    add_compile_definitions(VCPP)
    # We can't treat warnings as errors due to warnings in the cimg library.
    # TODO: exclude the header file from this and add back -WX
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

# Tell cmake what we're actually trying to make e.g. if this was a static lib this'd be add_library()
add_executable(hello ${SOURCES})