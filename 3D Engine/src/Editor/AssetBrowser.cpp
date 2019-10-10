#include "Editor/AssetBrowser.hpp"

#include <audeo/SoundSource.hpp>
#include <imgui/imgui.h>

#include "Editor/EditorLog.hpp"
#include "Editor/PreviewRendering.hpp"
#include "Editor/SelectFileDialog.hpp"
#include "Subsystems/AssetManager/AssetManager.hpp"
#include "Subsystems/Renderer/Mesh.hpp"
#include "Subsystems/Renderer/Shader.hpp"
#include "Subsystems/Renderer/Texture.hpp"
#include "Utility/Utility.hpp"

#include <tuple>

namespace Saturn::Editor {

// utilities for the asset browser
namespace impl {

namespace {

#define ASSET_LIST Shader, Texture, Mesh, audeo::SoundSource
template<size_t I, typename F, typename... Args>
void do_for_each_asset(F&& f, Args&&... args) {
    using assets = std::tuple<ASSET_LIST>;
    constexpr size_t assets_size = std::tuple_size_v<assets>;
    // Call the function
    f(std::forward<Args>(args)...);
    if constexpr (I < assets_size) {
        do_for_each_asset<I + 1, F, Args...>(std::forward<F>(f),
                                             std::forward<Args>(args)...);
    }
}

template<typename F, typename... Args>
void for_each_asset(F&& f, Args&&... args) {
    do_for_each_asset<0, F, Args...>(std::forward<F>(f),
                                     std::forward<Args>(args)...);
}

// Special overload for functors that are templated on the asset type
template<size_t I, template<typename> typename F, typename... Args>
void do_for_each_asset(Args&&... args) {
    using assets = std::tuple<ASSET_LIST>;
    constexpr size_t asset_size = std::tuple_size_v<assets>;
    using asset_t = std::tuple_element_t<I, assets>;

    F<asset_t>{}(std::forward<Args>(args)...);
    if constexpr (I < asset_size - 1) {
        do_for_each_asset<I + 1, F, Args...>(std::forward<Args>(args)...);
    }
}

template<template<typename> typename F, typename... Args>
void for_each_asset(Args&&... args) {
    do_for_each_asset<0, F, Args...>(std::forward<Args>(args)...);
}

template<typename A>
std::string_view asset_type_string() {
    if constexpr (std::is_same_v<A, Shader>) { return "shader"; }
    if constexpr (std::is_same_v<A, Texture>) { return "texture"; }
    if constexpr (std::is_same_v<A, Mesh>) { return "mesh"; }
    if constexpr (std::is_same_v<A, audeo::SoundSource>) { return "sound"; }
}

template<typename A>
constexpr size_t asset_index() {
    return index_of<A, ASSET_LIST>::value;
}

#undef ASSET_LIST

} // namespace
} // namespace impl

template<typename A>
struct fill_asset_types {
    void operator()(std::vector<std::string_view>& types) {
        types.push_back(impl::asset_type_string<A>());
    }
};

AssetBrowser::AssetBrowser() {

    // Show the asset browser by default
    shown = true;

    // Fill the list with asset types.
    impl::for_each_asset<fill_asset_types>(asset_types);
}

template<typename AssetT>
void display_preview(AssetT& asset, ImVec2 size) {
    // Default preview: an image saying no preview is available
    static Resource<Texture> no_preview_tex =
        AssetManager<Texture>::get_resource(
            "config/resources/textures/no_preview.tex", true);
    ImGui::Image(reinterpret_cast<ImTextureID>(no_preview_tex->handle()), size);
}

template<>
void display_preview(AssetManager<audeo::SoundSource>::Asset& asset,
                     ImVec2 size) {

    static Resource<Texture> speaker_tex = AssetManager<Texture>::get_resource(
        "config/resources/textures/speaker.tex", true);
    ImGui::Image(reinterpret_cast<ImTextureID>(speaker_tex->handle()), size);
}

template<>
void display_preview(AssetManager<Texture>::Asset& asset, ImVec2 size) {
    ImGui::Image(reinterpret_cast<ImTextureID>(asset.ptr->handle()), size);
}

template<>
void display_preview(AssetManager<Mesh>::Asset& asset, ImVec2 size) {
    auto texture_id = previews::render_mesh_preview(asset);
    ImGui::Image(reinterpret_cast<ImTextureID>(texture_id), size);
}

template<>
void display_preview(AssetManager<Shader>::Asset& asset, ImVec2 size) {
    static Resource<Texture> shader_icon = AssetManager<Texture>::get_resource(
        "config/resources/textures/shader_icon.tex", true);
    ImGui::Image(reinterpret_cast<ImTextureID>(shader_icon->handle()), size);
}

template<typename A>
struct show_asset_tab {
    static constexpr size_t index = impl::asset_index<A>();

    static std::string get_asset_name(fs::path const& path) {
        return path.stem().generic_string();
    }

    ImVec2 get_grid_size() {
        const float pad_x = ImGui::GetStyle().WindowPadding.x;
        const float pad_y = ImGui::GetStyle().WindowPadding.y;
        const ImVec2 full_window_size = ImGui::GetWindowSize();
        return ImVec2(full_window_size.x - 2 * pad_x,
                      full_window_size.y - 2 * pad_y);
    }

    // Calculates how many columns will fit
    int get_column_count(const ImVec2 grid_size,
                         const ImVec2 preview_size,
                         const ImVec2 pad) {
        //		width = cols * (pad_x + frame_w) + pad_x
        // <=>	cols = (width - pad_x) / (pad_x + frame_w)

        return static_cast<int>((grid_size.x - pad.x) /
                                (pad.x + preview_size.x));
    }

    void show_asset_grid(int sz, bool show_editor_assets) {
        ImVec2 grid_size = get_grid_size();
        // #TODO: Add slider to adjust this setting

        const ImVec2 pad = ImGui::GetStyle().WindowPadding;
        const float text_h = ImGui::GetFrameHeightWithSpacing();
        const ImVec2 preview_size = ImVec2(sz, sz + text_h + pad.y);

        const int max_cols = get_column_count(grid_size, preview_size, pad);

        if (max_cols <= 0) {
            log::warn("Can no longer fit asset columns on screen!");
            return;
        }

        auto& assets = AssetManager<A>::resource_list();
        size_t idx = 0;
        typename AssetManager<A>::Asset* asset_to_erase = nullptr;
        for (auto& [id, asset] : assets) {
            // Only display imported assets
            if (!show_editor_assets && !asset.imported) { continue; }
            auto asset_name = get_asset_name(asset.path);
            if (ImGui::BeginChild(
                    fmt::format("AssetPreviewFrame##{}", asset_name).c_str(),
                    preview_size, true,
                    ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoScrollWithMouse)) {

                bool open_details_dialog = false;

                if (ImGui::IsWindowHovered()) {
                    if (ImGui::IsMouseClicked(1)) {
                        open_details_dialog = true;
                    }
                }

                float edge_sz = preview_size.x - 2 * pad.x;
                display_preview(asset, ImVec2(edge_sz, edge_sz));

                ImGui::TextWrapped("%s", asset_name.c_str());

                if (open_details_dialog) {
                    ImGui::OpenPopup("##AssetContextMenu");
                }
                if (ImGui::BeginPopup("##AssetContextMenu")) {
                    if (ImGui::MenuItem("Remove asset")) {
                        asset_to_erase = &asset;
                    }
                    ImGui::EndPopup();
                }
            }
            ImGui::EndChild();

            // Put the next widget on the same line if we're not at the end of a
            // column
            if ((idx + 1) % max_cols != 0) { ImGui::SameLine(); }
            ++idx;
        }
        // do erasing of id's that have to BEGONE
        if (asset_to_erase) { AssetManager<A>::remove_asset(*asset_to_erase); }
    }

    void operator()(std::vector<std::string_view> const& asset_types,
                    int preview_size,
                    bool show_editor_assets) {
        std::string_view type_name = asset_types[index];
        if (ImGui::BeginTabItem(
                fmt::format("{}##BrowserTabBar", type_name).c_str())) {

            show_asset_grid(preview_size, show_editor_assets);
            ImGui::Separator();
            if (ImGui::Button("Import##ImportAssetBtn")) {
                SelectFileDialog dialog;
                dialog.show(SelectFileDialog::PickFiles,
                            fs::absolute(fs::path(last_open_path<A>())),
                            FileTypes<A>::types);
                fs::path result = dialog.get_result();
                if (result != "") {
                    auto discarded = AssetManager<A>::get_resource(
                        fs::relative(result, ProjectFile::root_path())
                            .generic_string(),
                        false, true);
                    last_open_path<A>() = result.remove_filename().string();
                }
            }

            ImGui::EndTabItem();
        }
    }
};

void AssetBrowser::show() {

    if (ImGui::Begin("Asset browser", get_shown_pointer())) {

        // Menu buttons
        if (ImGui::Button("Clear cache")) { previews::clear_cache(); }
        ImGui::SameLine();
        ImGui::Checkbox("Show editor assets", &show_editor_assets);
        ImGui::SameLine();
        ImGui::SliderInt("Preview size", &preview_size, 1,
                         previews::max_preview_size, "%d px");

        ImGui::Separator();

        if (ImGui::BeginTabBar("Asset types##AssetBrowserTabBar",
                               ImGuiTabBarFlags_NoCloseWithMiddleMouseButton |
                                   ImGuiTabBarFlags_Reorderable)) {

            impl::for_each_asset<show_asset_tab>(asset_types, preview_size,
                                                 show_editor_assets);

            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

} // namespace Saturn::Editor
