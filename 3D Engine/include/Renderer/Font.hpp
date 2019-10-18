#ifndef SATURN_FONT_HPP_
#define SATURN_FONT_HPP_

#include <glm/glm.hpp>
#include <unordered_map>

namespace Saturn {

class Font {
public:
    struct CreateInfo {
        std::string_view font_path;
    };

	Font() = default;
    Font(CreateInfo const& info);
	~Font();

    struct glyph_data {
        unsigned int texture;
        // size of the glyph
        glm::vec2 size;
        // offset from baseline to left/top of the glyph
        glm::vec2 bearing;
        // offset to advance to next glyph
        unsigned int advance;
        // the pixel size this character was rendered with in the font texture
        size_t pixel_size;
    };

    std::unordered_map<char, glyph_data> glyphs;

	void swap(Font& other);
};

} // namespace Saturn

#endif // ifndef SATURN_FONT_HPP_
