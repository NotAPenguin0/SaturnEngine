cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(SaturnEngine)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)

set(SATURN_SOURCES "")
set(SATURN_GENERATED_SOURCES "")
set(SATURN_LINK_LIBRARIES "")
set(SATURN_INCLUDE_DIRECTORIES "include") 

set(CODEGEN_SOURCES "")
set(CODEGEN_LINK_LIBRARIES "")
set(CODEGEN_INCLUDE_DIRECTORIES "include" "external")

option(VS_BUILD CACHE OFF)

add_subdirectory("src")
add_subdirectory("external")

add_executable(CodeGen ${CODEGEN_SOURCES})
target_include_directories(CodeGen PUBLIC ${CODEGEN_INCLUDE_DIRECTORIES})
target_link_libraries(CodeGen PUBLIC ${CODEGEN_LINK_LIBRARIES})

get_directory_property(PHOBOS_INCLUDE_DIRS 
    DIRECTORY "external/phobos" 
    DEFINITION PHOBOS_INCLUDE_DIRECTORIES
)

set(RUN_CODEGEN_INCLUDE_DIRS
    ${SATURN_INCLUDE_DIRECTORIES}
    ${PHOBOS_INCLUDE_DIRS}
)

if (NOT VS_BUILD)
    add_custom_target(RunCodeGen
        COMMAND CodeGen "-i \"${RUN_CODEGEN_INCLUDE_DIRS}\""
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        BYPRODUCTS "${SATURN_GENERATED_SOURCES}"
        COMMENT "Running codegen."
    ) 
else()
    add_custom_target(RunCodeGen
        COMMAND CodeGen "-i \"${RUN_CODEGEN_INCLUDE_DIRS}\""
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
# BYPRODUCTS doesn't work properly in the current VS version
#        BYPRODUCTS "${SATURN_GENERATED_SOURCES}"
        COMMENT "Running codegen."
    ) 
endif()

add_library(SaturnEngine STATIC ${SATURN_SOURCES} ${SATURN_GENERATED_SOURCES})
target_link_libraries(SaturnEngine PUBLIC ${SATURN_LINK_LIBRARIES})
target_include_directories(SaturnEngine PUBLIC ${SATURN_INCLUDE_DIRECTORIES})

target_compile_options(SaturnEngine PRIVATE "-Wall" "-Werror")

add_dependencies(SaturnEngine RunCodeGen)

#target_compile_options(SaturnEngine PUBLIC "-ftime-trace")

option(BUILD_EDITOR CACHE ON)
if (BUILD_EDITOR)
    add_executable(SaturnEditor)
    target_include_directories(SaturnEditor PUBLIC ${SATURN_INCLUDE_DIRECTORIES})
    target_link_libraries(SaturnEditor PUBLIC SaturnEngine)
    add_subdirectory("src/editor")

    target_compile_options(SaturnEditor PRIVATE "-Wall" "-Werror")

    add_dependencies(SaturnEditor SaturnEngine)
endif(BUILD_EDITOR)

