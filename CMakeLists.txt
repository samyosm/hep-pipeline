cmake_minimum_required(VERSION 3.15)
project(HEPPipeline VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(event-generation)
add_subdirectory(common)
