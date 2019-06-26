cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(ENGINE_FOLDER_NAME "Engine")
set(ENGINE_DEPENDENCIES_FOLDER_NAME "Dependencies")
set(DEBUG_OUTPUT_DIRECTORY "bin/debug")
set(RELEASE_OUTPUT_DIRECTORY "bin/release")

project(SaturnEngine)
enable_language(CXX)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

# Define Release by default.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
  message(STATUS "Build type not specified: Use Release by default.")
endif(NOT CMAKE_BUILD_TYPE)

add_definitions(
   -DNO_PERFORMANCE_LOG 
   -DENGINE_DEBUG_BUILD 
   -D_MBCS 
   # -D__clang__%(PreprocessorDefinitions) # doesn't compile in msvc
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${DEBUG_OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${DEBUG_OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${DEBUG_OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${RELEASE_OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${RELEASE_OUTPUT_DIRECTORY}")
set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${RELEASE_OUTPUT_DIRECTORY}")

if(MSVC)
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3 /MD /Od /GR /EHsc")
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W3 /GL /Od /Oi /GR /Gy /EHsc")
else()
   if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
   endif()
endif()

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
add_subdirectory("3D Engine/dependencies")
add_subdirectory("3D Engine/include")
add_subdirectory("3D Engine/src")

add_executable(${PROJECT_NAME} 
    ${ENGINE_HEADER_FILES}
    ${ENGINE_SOURCE_FILES}
)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Engine")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/3D Engine" FILES ${ENGINE_HEADER_FILES} ${ENGINE_SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${ENGINE_PUBLIC_INCLUDE_DIRECTORIES}
)

target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    glad
    glfw
    nlohmann_json
    glm
)