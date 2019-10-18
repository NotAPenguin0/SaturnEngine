#include "Renderer/Font.hpp"
#include "Renderer/OpenGL.hpp"

#include "Editor/EditorLog.hpp"

// clang-format off
#include <ft2build.h>
#include <freetype/freetype.h>
// clang-format on

namespace Saturn {

namespace impl {
unsigned int create_glyph_texture(FT_Face& face) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                 face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return texture;
}

void load_glyphs(FT_Library& ft,
                 const char* font_file,
                 std::unordered_map<char, Font::glyph_data>& glyphs) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    FT_Face face;
    if (FT_New_Face(ft, font_file, 0, &face)) {
        log::error("Failed to load font {}", font_file);
    }
    // Set font size
    constexpr size_t pixel_size = 48;
    FT_Set_Pixel_Sizes(face, 0, pixel_size);
    for (unsigned char c = 0; c < 128; ++c) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            log::error("Failed to extract character '{}' from font file '{}'",
                       c, font_file);
            continue;
        }
        unsigned int texture = create_glyph_texture(face);
        Font::glyph_data data;
        data.texture = texture;
        data.size =
            glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        data.bearing =
            glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        data.advance = face->glyph->advance.x;
        data.pixel_size = pixel_size;
        glyphs.insert({c, std::move(data)});
    }
    // reset texture alignment to default value
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    FT_Done_Face(face);
}

} // namespace impl

Font::Font(CreateInfo const& info) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        log::error("Failed to initialize freetype library");
        return;
    }
    impl::load_glyphs(ft, info.font_path.data(), glyphs);
    FT_Done_FreeType(ft);
}

Font::~Font() {
    for (auto& [c, data] : glyphs) {
        if (data.texture) { glDeleteTextures(1, &data.texture); }
    }
}

void Font::swap(Font& other) { std::swap(glyphs, other.glyphs); }

} // namespace Saturn
