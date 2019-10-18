#ifndef SATURN_UI_ANCHOR_HPP_
#define SATURN_UI_ANCHOR_HPP_

#include <glm/glm.hpp>
#include <unordered_map>

namespace Saturn {

namespace ui_anchors {

struct anchor_t {
    size_t id = 0;
};

inline bool operator==(anchor_t const& a, anchor_t const& b) {
	return a.id == b.id;
}

inline bool operator!=(anchor_t const& a, anchor_t const& b) {
	return a.id != b.id;
}

constexpr anchor_t top_left = {0};
constexpr anchor_t top_center = {1};
constexpr anchor_t top_right = {2};
constexpr anchor_t center_left = {3};
constexpr anchor_t center = {4};
constexpr anchor_t center_right = {5};
constexpr anchor_t bottom_left = {6};
constexpr anchor_t bottom_center = {7};
constexpr anchor_t bottom_right = {8};

} // namespace ui_anchors
} // namespace Saturn

namespace std {

template<>
struct hash<Saturn::ui_anchors::anchor_t> {
    size_t operator()(Saturn::ui_anchors::anchor_t const& anch) const {
        return hash<size_t>()(anch.id);
    }
};

} // namespace std

namespace Saturn {
namespace ui_anchors {

// clang-format off
inline std::unordered_map<anchor_t, const char*> anchor_names = {
	{top_left, "Top left"},
	{top_center, "Top center"},
	{top_right, "Top right"},
	{center_left, "Center left"},
	{center, "Center"},
	{center_right, "Center right"},
	{bottom_left, "Bottom left"},
	{bottom_center, "Bottom center"},
	{bottom_right, "Bottom right"}
};

inline std::unordered_map<anchor_t, glm::vec2> anchor_positions = {
	{top_left, glm::vec2(0, 0)},
	{top_center, glm::vec2(0.5, 0)},
	{top_right, glm::vec2(1, 0)},
	{center_left, glm::vec2(0, 0.5)},
	{center, glm::vec2(0.5, 0.5)},
	{center_right, glm::vec2(1, 0.5)},
	{bottom_left, glm::vec2(0, 1)},
	{bottom_center, glm::vec2(0.5, 1)},
	{bottom_right, glm::vec2(1, 1)}
};

// clang-format on

} // namespace ui_anchors

} // namespace Saturn

#endif // ifndef SATURN_UI_ANCHOR_HPP_
