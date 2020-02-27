set(SATURN_SOURCES
    ${SATURN_SOURCES}
    # Core
    "${CMAKE_CURRENT_SOURCE_DIR}/core/engine.cpp"

    # Scene
    "${CMAKE_CURRENT_SOURCE_DIR}/scene/scene.cpp"

    # ECS
    "${CMAKE_CURRENT_SOURCE_DIR}/ecs/registry.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ecs/systems.cpp"

    # Systems
    "${CMAKE_CURRENT_SOURCE_DIR}/systems/base_systems.cpp"

    # Other
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    PARENT_SCOPE
)