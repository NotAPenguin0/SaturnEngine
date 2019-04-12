#ifndef MVG_VIEWPORT_HPP_
#define MVG_VIEWPORT_HPP_

#include "Utility.hpp"

namespace Saturn {

class Viewport {
public:
    Viewport() = default;
    Viewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h);

    WindowDim dimensions() const;
    WindowDim position() const;

	void resize(unsigned int new_w, unsigned int new_h);
    void move(unsigned int new_x, unsigned int new_y);

	static void set_active(Viewport const& viewport);
    static Viewport current();

private: 
	unsigned int x, y;
    unsigned int w, h;
};

} // namespace Saturn

#endif