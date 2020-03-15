set(SATURN_SOURCES
    ${SATURN_SOURCES}
    # Assets
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/assets.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/importers/simple_mesh.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/importers/stb_texture_import.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/importers/obj.cpp"

    # Core
    "${CMAKE_CURRENT_SOURCE_DIR}/core/engine.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/input.cpp"

    # Scene
    "${CMAKE_CURRENT_SOURCE_DIR}/scene/scene.cpp"

    # ECS
    "${CMAKE_CURRENT_SOURCE_DIR}/ecs/registry.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ecs/system_manager.cpp"

    # Systems
    

    # Serialization
    "${CMAKE_CURRENT_SOURCE_DIR}/serialization/default_serializers.cpp"

    # Other
    "${CMAKE_CURRENT_SOURCE_DIR}/../external/phobos/imgui_style.cpp"


    PARENT_SCOPE
)

set(SATURN_GENERATED_SOURCES
    ${SATURN_GENERATED_SOURCES}

    "${CMAKE_CURRENT_SOURCE_DIR}/../include/saturn/meta/type_category.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/saturn/serialization/component_serializers.hpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/serialization/component_serializers.generated.cpp"

    PARENT_SCOPE
)

set(CODEGEN_SOURCES
    ${CODEGEN_SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/codegen/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/codegen/argument_parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/codegen/file_parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/codegen/ast_visitor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/codegen/category_generator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/codegen/utility.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/codegen/serialization_generator.cpp"

    PARENT_SCOPE
)