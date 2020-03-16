cmake_policy(SET CMP0076 NEW)
target_sources(SaturnEditor PUBLIC 
    "editor_main.cpp"

    "systems/editor_system.cpp"
    "systems/camera.cpp"

    "widgets/log_window.cpp"
    "widgets/entity_tree.cpp"
    "widgets/inspector.cpp"
)