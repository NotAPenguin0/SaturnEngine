add_subdirectory("phobos")
add_subdirectory("cppast")

set(LINK_LIBRARIES "")

set(LINK_LIBRARIES
    ${LINK_LIBRARIES}
    Phobos
    stl
)

if (NOT VS_BUILD)
    add_subdirectory("assimp")
    target_compile_options(assimp PUBLIC "-w")
    set(LINK_LIBRARIES 
        ${LINK_LIBRARIES} 
        assimp 
    )
else()
    set(SATURN_LINK_LIBRARIES 
        ${LINK_LIBRARIES} 
        "assimpd.lib" "IrrXMLd.lib" "zlibstaticd.lib" 
    )
endif()

set(SATURN_LINK_LIBRARIES 
    ${SATURN_LINK_LIBRARIES}
    ${LINK_LIBRARIES}
    PARENT_SCOPE    
)

set(CODEGEN_LINK_LIBRARIES
    ${CODEGEN_LINK_LIBRARIES}
    cppast
    stl
    PARENT_SCOPE
)

set(SATURN_INCLUDE_DIRECTORIES
    ${SATURN_INCLUDE_DIRECTORIES}
    "${CMAKE_CURRENT_SOURCE_DIR}/phobos/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/nlohmann_json/single_include"
    "${CMAKE_CURRENT_SOURCE_DIR}/stl/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/assimp/include"
    "${CMAKE_BINARY_DIR}/external/assimp/include"
    PARENT_SCOPE
)

set(CODEGEN_INCLUDE_DIRECTORIES
    ${CODEGEN_INCLUDE_DIRECTORIES}
    "${CMAKE_CURRENT_SOURCE_DIR}/cppast/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/stl/include"
    PARENT_SCOPE
)