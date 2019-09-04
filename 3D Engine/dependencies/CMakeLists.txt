add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(nlohmann_json)
add_subdirectory(libfmt)

set_target_properties(glad glfw # nlohmann_json glm
    PROPERTIES
    FOLDER "${ENGINE_DEPENDENCIES_FOLDER_NAME}"
)

if(MSVC)
    set_target_properties(glfw glad
        PROPERTIES
        COMPILE_OPTIONS "/W0"
    )
elseif("${CMAKE_CXX_COMPILER_ID}" matches "GCC" or "${CMAKE_CXX_COMPILER_ID}" matches "Clang")
    set_target_properties(glfw glad
        PROPERTIES
        COMPILE_OPTIONS "-Wno-everything"
    )
endif()