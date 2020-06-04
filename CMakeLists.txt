cmake_minimum_required(VERSION 3.14)
project(BBP)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(TroubleBrewing)

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/sleepy-discord)
target_link_libraries(BBP sleepy-discord)
