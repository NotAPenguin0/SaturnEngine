#ifndef MVG_VIEWPORT_HPP_
#define MVG_VIEWPORT_HPP_

#include "Utility.hpp"

#include "Camera.hpp"

namespace Saturn {

class Viewport {
public:
    Viewport() = default;
    Viewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
    Viewport(std::size_t cam, unsigned int x, unsigned int y, unsigned int w, unsigned int h);

    WindowDim dimensions() const;
    WindowDim position() const;

    void resize(unsigned int new_w, unsigned int new_h);
    void move(unsigned int new_x, unsigned int new_y);

    static void set_active(Viewport const& viewport);
   
	std::size_t get_camera() const;
	void set_camera(std::size_t cam_id);

	bool has_camera() const;

private:
    unsigned int x, y;
    unsigned int w, h;
    std::size_t camera = static_cast<std::size_t>(-1);
};

} // namespace Saturn

#endif