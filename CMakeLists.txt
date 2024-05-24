cmake_minimum_required(VERSION 3.10)
project(proj3)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(../matplotplusplus build/matplotplusplus)

add_executable(proj3 main.cpp)

target_link_libraries(proj3 PRIVATE matplot)



set(AUDIOFILE_SOURCE_DIR "C:/Users/adamj/Desktop/scikit_build_example/AudioFile")
set(AUDIOFILE_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/AudioFile")
add_subdirectory(${AUDIOFILE_SOURCE_DIR} ${AUDIOFILE_BINARY_DIR})

add_definitions(-DPROJECT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}")

file(COPY ${CMAKE_SOURCE_DIR}/test-audio.wav DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(proj3 PRIVATE AudioFile)



add_subdirectory(${CMAKE_SOURCE_DIR}/../pybind11 ${CMAKE_BINARY_DIR}/pybind11)

target_link_libraries(proj3 PRIVATE pybind11::pybind11)