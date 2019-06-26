add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(nlohmann_json)

set_target_properties(glad glfw # nlohmann_json glm
    PROPERTIES
    FOLDER "${ENGINE_DEPENDENCIES_FOLDER_NAME}"
)