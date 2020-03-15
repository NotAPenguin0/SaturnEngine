cmake_policy(SET CMP0076 NEW)
target_sources(SaturnEditor PUBLIC 
    "editor_main.cpp"

    "systems/editor_system.cpp"
    "systems/camera.cpp"
    "log_window.cpp"
)