cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(SaturnEngine)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)

set(SATURN_SOURCES "")
set(SATURN_GENERATED_SOURCES "")
set(CODEGEN_SOURCES "")
set(SATURN_LINK_LIBRARIES "")
set(SATURN_INCLUDE_DIRECTORIES "include" "${CMAKE_BINARY_DIR}/generated/include") 

add_subdirectory("src")
add_subdirectory("external")

add_executable(CodeGen ${CODEGEN_SOURCES})

add_custom_target(RunCodeGen
    COMMAND CodeGen
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_executable(SaturnEngine ${SATURN_SOURCES} ${SATURN_GENERATED_SOURCES})
target_link_libraries(SaturnEngine PUBLIC ${SATURN_LINK_LIBRARIES})
target_include_directories(SaturnEngine PUBLIC ${SATURN_INCLUDE_DIRECTORIES})

add_dependencies(SaturnEngine RunCodeGen)
