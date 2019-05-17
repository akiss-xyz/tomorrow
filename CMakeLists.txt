cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 17)

project(tomorrow-proj)

include_directories(include)
file(GLOB_RECURSE SOURCES src/*.cpp)

include_directories(vendor/CImg-2.4.3)

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-unused-parameter")
endif()
if ( MSVC )
    # TODO: exclude the header file from this and add back -WX
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()
    
add_executable(tomorrow ${SOURCES})
    
if( CMAKE_HOST_UNIX )
    # X11 libraries for GUI functionality, used by cimg.h
    find_package(X11 REQUIRED)
    # message(STATUS "X11_FOUND = ${X11_FOUND}")
    # message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
    # message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")

    include_directories(${X11_INCLUDE_DIR})
    link_directories(${X11_LIBRARIES})
    target_link_libraries(tomorrow ${X11_LIBRARIES})

    # pthread for cimg.h
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(tomorrow Threads::Threads)
endif()
