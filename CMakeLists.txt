cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

################### Variables. ####################
# Change if you want modify path or other values. #
###################################################

set(PROJECT_NAME My3DEngine)
# Output Variables
set(OUTPUT_DEBUG Debug/bin)
set(OUTPUT_RELEASE Release/bin)
# Folders files
set(CPP_DIR_1 glad/src)
set(CPP_DIR_2 ./)
set(CPP_DIR_3 src/Core)
set(CPP_DIR_4 src/Subsystems/AssetManager)
set(CPP_DIR_5 src/Subsystems/ECS)
set(CPP_DIR_6 src/Subsystems/ECS/Systems)
set(CPP_DIR_7 src/Subsystems/Input)
set(CPP_DIR_8 src/Subsystems/Logging)
set(CPP_DIR_9 src/Subsystems/Math)
set(CPP_DIR_10 src/Subsystems/Renderer)
set(CPP_DIR_11 src/Subsystems/Scene)
set(CPP_DIR_12 src/Subsystems/Time)
set(CPP_DIR_13 src/Utility)
set(HEADER_DIR_1 include/Core)
set(HEADER_DIR_2 include/Subsystems/AssetManager)
set(HEADER_DIR_3 include/Subsystems/ECS)
set(HEADER_DIR_4 include/Subsystems/ECS/Components)
set(HEADER_DIR_5 include/Subsystems/ECS/Systems)
set(HEADER_DIR_6 include/Subsystems/Input)
set(HEADER_DIR_7 include/Subsystems/Logging)
set(HEADER_DIR_8 include/Subsystems/Math)
set(HEADER_DIR_9 include/Subsystems/Renderer)
set(HEADER_DIR_10 include/Subsystems/Scene)
set(HEADER_DIR_11 include/Subsystems/Time)
set(HEADER_DIR_12 include/Utility)

############## CMake Project ################
#        The main options of project        #
#############################################

project(${PROJECT_NAME} C)

# Define Release by default.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
  message(STATUS "Build type not specified: Use Release by default.")
endif(NOT CMAKE_BUILD_TYPE)

# Definition of Macros
add_definitions(
   -DNO_PERFORMANCE_LOG 
   -DENGINE_DEBUG_BUILD 
   -D_MBCS 
   -D__clang__%(PreprocessorDefinitions) 
)

############## Artefacts Output #################
# Defines outputs , depending Debug or Release. #
#################################################

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_DEBUG}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_DEBUG}")
  set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_DEBUG}")
else()
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
  set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
endif()

################# Flags ################
# Defines Flags for Windows and Linux. #
########################################

if(MSVC)
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3 /MD /Od /GR /EHsc")
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W3 /GL /Od /Oi /GR /Gy /EHsc")
endif(MSVC)
if(NOT MSVC)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
   endif()
endif(NOT MSVC)

################ Files ################
#   --   Add files to project.   --   #
#######################################

file(GLOB SRC_FILES
    ${CPP_DIR_1}/*.c
    ${CPP_DIR_1}/*.cpp
    ${CPP_DIR_2}/*.c
    ${CPP_DIR_2}/*.cpp
    ${CPP_DIR_3}/*.c
    ${CPP_DIR_3}/*.cpp
    ${CPP_DIR_4}/*.c
    ${CPP_DIR_4}/*.cpp
    ${CPP_DIR_5}/*.c
    ${CPP_DIR_5}/*.cpp
    ${CPP_DIR_6}/*.c
    ${CPP_DIR_6}/*.cpp
    ${CPP_DIR_7}/*.c
    ${CPP_DIR_7}/*.cpp
    ${CPP_DIR_8}/*.c
    ${CPP_DIR_8}/*.cpp
    ${CPP_DIR_9}/*.c
    ${CPP_DIR_9}/*.cpp
    ${CPP_DIR_10}/*.c
    ${CPP_DIR_10}/*.cpp
    ${CPP_DIR_11}/*.c
    ${CPP_DIR_11}/*.cpp
    ${CPP_DIR_12}/*.c
    ${CPP_DIR_12}/*.cpp
    ${CPP_DIR_13}/*.c
    ${CPP_DIR_13}/*.cpp
    ${HEADER_DIR_1}/*.h
    ${HEADER_DIR_2}/*.h
    ${HEADER_DIR_3}/*.h
    ${HEADER_DIR_4}/*.h
    ${HEADER_DIR_5}/*.h
    ${HEADER_DIR_6}/*.h
    ${HEADER_DIR_7}/*.h
    ${HEADER_DIR_8}/*.h
    ${HEADER_DIR_9}/*.h
    ${HEADER_DIR_10}/*.h
    ${HEADER_DIR_11}/*.h
    ${HEADER_DIR_12}/*.h
)

# Add executable to build.
add_executable(${PROJECT_NAME} 
   ${SRC_FILES}
)

if(MSVC)
   target_link_libraries(${PROJECT_NAME} opengl32.lib C:\assimp-bin\assimp.lib C:\Libs\lib\glfw3.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib )
endif(MSVC)