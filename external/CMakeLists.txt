add_subdirectory("phobos")
add_subdirectory("cppast")
add_subdirectory("stl")

set(SATURN_LINK_LIBRARIES
    ${SATURN_LINK_LIBRARIES}
    Phobos
    stl
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
    PARENT_SCOPE
)

set(CODEGEN_INCLUDE_DIRECTORIES
    ${CODEGEN_INCLUDE_DIRECTORIES}
    "${CMAKE_CURRENT_SOURCE_DIR}/cppast/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/stl/include"
    PARENT_SCOPE
)