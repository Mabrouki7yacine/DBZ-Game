cmake_minimum_required(VERSION 3.10)
project(game_v1)

# Add headers
include_directories(${CMAKE_SOURCE_DIR}/Include)
# Add executable
add_executable(${PROJECT_NAME} main.cpp src/Person.cpp)
# Link raylib libraries
target_link_libraries(${PROJECT_NAME} raylib)