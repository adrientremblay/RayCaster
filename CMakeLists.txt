cmake_minimum_required(VERSION 3.0)

project(RayCaster)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

# Set the source files
set(SOURCES
        src/main.cpp
        )

# Add Eigen library
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# Set the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio)