cmake_minimum_required(VERSION 3.26)
project(STUDYMAP C)

set(CMAKE_C_STANDARD 11)

# Set the output directory for the executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Build)


add_subdirectory(src)