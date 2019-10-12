cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(ENGINE_FOLDER_NAME "Engine")
set(ENGINE_DEPENDENCIES_FOLDER_NAME "Dependencies")
set(DEBUG_OUTPUT_DIRECTORY "bin/debug")
set(RELEASE_OUTPUT_DIRECTORY "bin/release")
set(EDITOR_OUTPUT_DIRECTORY "bin/editor")

project(SaturnEngine)
enable_language(CXX)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

set(ENGINE_COMPILE_FLAGS "-Wno-unused-variable -Wno-unused-function -Wno-unused-parameter")

# Define Release by default.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Editor")
  message(STATUS "Build type not specified: Use Editor by default.")
endif(NOT CMAKE_BUILD_TYPE)


set(CMAKE_CXX_FLAGS_EDITOR "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS_EDITOR} -DWITH_EDITOR")


add_definitions(
   -DNO_PERFORMANCE_LOG 
   -DENGINE_DEBUG_BUILD 
   -D_MBCS 
   # -D__clang__%(PreprocessorDefinitions) # doesn't compile in msvc
    ${ENGINE_COMPILE_FLAGS}  # Dirty, but easy and convenient, see 			     	     
		             # https://stackoverflow.com/questions/11783932/how-do-i-add-a-linker-or-compile-flag-in-a-cmake-file
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
    "${CMAKE_CURRENT_SOURCE_DIR}/3D Engine/dependencies/libfmt/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/3D Engine/dependencies/efsw/include"
)

get_filename_component(ABSOLUTE_PROJECT_DIR "3D Engine" ABSOLUTE)
get_filename_component(ABSOLUTE_CODEGEN_PATH 
"builds/CodeGen/Serialization/${CMAKE_BUILD_TYPE}/SerializationCodeGen.exe" ABSOLUTE)

add_custom_command(TARGET ${PROJECT_NAME}
	PRE_BUILD
	COMMAND ${ABSOLUTE_CODEGEN_PATH} ${ABSOLUTE_PROJECT_DIR}
)

set(SATURN_LINK_LIBRARIES
	${OPENGL_LIBRARIES} glad glfw nlohmann_json glm fmt audeo efsw	
)

if (WIN32) 
	set(SATURN_LINK_LIBRARIES ${SATURN_LINK_LIBRARIES} shlwapi.lib propsys.lib)
endif(WIN32)

target_link_libraries(${PROJECT_NAME} ${SATURN_LINK_LIBRARIES})

# Code Generation projects

# Add subdirectories
add_subdirectory("CodeGen/Serialization")
