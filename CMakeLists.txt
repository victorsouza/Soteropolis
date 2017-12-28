cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(Soteropolis C CXX)

# Add project includes

include_directories(
    "${PROJECT_SOURCE_DIR}/Soteropolis")

# Include threads

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
add_definitions(${CMAKE_THREAD_LIBS_INIT})

# Include CGAL

find_package(CGAL REQUIRED)
add_definitions(-DSEISHPC_ENABLE_CGAL)
add_definitions(-frounding-math)
include_directories(${CGAL_INCLUDES})

# Include Boost GLEW OpenGL GLM and SDL2

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# find_package(glm REQUIRED) # TODO: Find valid CMake FindPackage
# find_package(SDL2 REQUIRED) # TODO: Find valid CMake FindPackage

# Enable C++11 and some other stuff on GNU

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -D_GNU_SOURCE -D_REENTRANT -D_FILE_OFFSET_BITS=64)
endif()

add_subdirectory(Soteropolis)

# vim:sw=4:et:sta
