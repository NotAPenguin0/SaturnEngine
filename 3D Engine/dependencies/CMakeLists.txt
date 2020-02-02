add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(libfmt)
add_subdirectory(audeo)
add_subdirectory(efsw)
add_subdirectory(FreeType)
add_subdirectory(assimp)

set_target_properties(glad glfw audeo freetype assimp efsw
    PROPERTIES
    FOLDER "${ENGINE_DEPENDENCIES_FOLDER_NAME}"
)

if(MSVC)
    set_target_properties(glfw glad efsw freetype assimp
        PROPERTIES
        COMPILE_OPTIONS "/W0"
    )
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GCC" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set_target_properties(glfw glad efsw freetype assimp
        PROPERTIES
        COMPILE_OPTIONS "-Wno-everything"
    )
endif()