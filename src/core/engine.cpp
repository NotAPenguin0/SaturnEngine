#include <saturn/core/engine.hpp>

#include <phobos/core/window_context.hpp>
#include <phobos/core/vulkan_context.hpp>

#include <phobos/renderer/render_graph.hpp>
#include <phobos/renderer/renderer.hpp>
#include <phobos/renderer/imgui_renderer.hpp>

#include <phobos/../../imgui_style.hpp>

#include <phobos/present/present_manager.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_mimas.h>
#include <mimas/mimas.h>

#include <saturn/assets/assets.hpp>

#include <saturn/scene/scene.hpp>
#include <saturn/ecs/system_manager.hpp>

#include <saturn/meta/reflect.hpp>
#include <saturn/components/transform.hpp>

#include <saturn/serialization/component_serializers.hpp>
#include <saturn/serialization/default_serializers.hpp>

#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>

namespace saturn {

Engine::Engine(ph::log::LogInterface* logger) {
    window_context = ph::create_window_context("SaturnEngine - Vulkan", 1280, 720);
    ph::AppSettings settings;
    settings.enable_validation_layers = true;
    settings.version = ph::Version{0, 0, 1};
    vulkan_context = ph::create_vulkan_context(*window_context, logger, settings);

     // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingWithShift = false;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    style_theme_grey();
}

Engine::~Engine() {
    delete window_context;
    delete vulkan_context;
}

void Engine::run() {
    // Create Phobos rendering devices
    ph::PresentManager present_manager(*vulkan_context);
    ph::Renderer renderer(*vulkan_context);

    ph::ImGuiRenderer imgui_renderer(*window_context, *vulkan_context);

    Scene demo_scene;
    demo_scene.init_demo_scene(vulkan_context);

    present_manager.add_color_attachment("color1");
    present_manager.add_depth_attachment("depth1");

    systems.startup(*vulkan_context);

    while(window_context->is_open()) { 
        window_context->poll_events();
    
        present_manager.wait_for_available_frame();

        imgui_renderer.begin_frame();

        ph::FrameInfo& frame = present_manager.get_frame_info();

        FrameContext frame_ctx { demo_scene, demo_scene.ecs, frame };
        systems.update_all(frame_ctx);

        auto& color_attachment = present_manager.get_attachment("color1");
        auto& depth_attachment = present_manager.get_attachment("depth1");

        frame.offscreen_target = 
            ph::RenderTarget(vulkan_context, vulkan_context->default_render_pass, {color_attachment, depth_attachment});

        ph::RenderGraph render_graph;
        render_graph.clear_color = vk::ClearColorValue(std::array<float, 4>{{0, 0, 0, 1}});

        demo_scene.build_render_graph(frame, render_graph);

        // Render the frame
        renderer.render_frame(frame, render_graph);
        imgui_renderer.render_frame(frame);

        // Present to swapchain
        present_manager.present_frame(frame);
    }

    // Wait until the VkDevice is idle before we can start shutting down
    vulkan_context->device.waitIdle();

    // Serialize to file on exit
//    demo_scene.save_to_file("data/ecs.bin");

    assets::destroy_all_assets();
    present_manager.destroy();
    imgui_renderer.destroy();
    renderer.destroy();
}

} // namespace saturn