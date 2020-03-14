#include <saturn/assets/importers/stb_texture_import.hpp>

#include <stb/stb_image.h>
#include <phobos/renderer/texture.hpp>

namespace saturn::assets::importers {

ph::Texture import_with_stb(Context& ctx, fs::path const& path) {
    int w, h, channels;
    uint8_t* img = stbi_load(path.generic_string().c_str(), &w, &h, &channels, STBI_rgb_alpha);
    ph::Texture::CreateInfo tex_info;
    tex_info.ctx = ctx.vulkan;
    tex_info.channels = channels;
    tex_info.format = vk::Format::eR8G8B8A8Srgb;
    tex_info.width = w;
    tex_info.height = h;
    tex_info.data = img;
    ph::Texture texture(tex_info);
    stbi_image_free(img);

    return texture;
}

}