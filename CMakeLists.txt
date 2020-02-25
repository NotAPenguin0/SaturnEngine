cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(SaturnEngine)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)

set(SATURN_SOURCES "")
set(SATURN_LINK_LIBRARIES "")
set(SATURN_INCLUDE_DIRECTORIES "include")

add_subdirectory("src")
add_subdirectory("external")

add_executable(SaturnEngine ${SATURN_SOURCES})
target_link_libraries(SaturnEngine PUBLIC ${SATURN_LINK_LIBRARIES})
target_include_directories(SaturnEngine PUBLIC ${SATURN_INCLUDE_DIRECTORIES})