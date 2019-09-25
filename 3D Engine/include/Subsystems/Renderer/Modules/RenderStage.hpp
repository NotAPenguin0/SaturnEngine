#ifndef SATURN_RENDER_STAGE_HPP_
#define SATURN_RENDER_STAGE_HPP_

#include <cstddef>

namespace Saturn {

namespace RenderModules {

class RenderStage {
public:
    RenderStage() = default;
    RenderStage(size_t ordering);
    virtual ~RenderStage() = default;

    virtual size_t get_ordering() const final;

    // Used for sorting the render stages in order

    friend bool operator==(RenderStage const& lhs, RenderStage const& rhs) {
        return lhs.ordering == rhs.ordering;
    }

    friend bool operator<(RenderStage const& lhs, RenderStage const& rhs) {
        return lhs.ordering < rhs.ordering;
    }

    friend bool operator>(RenderStage const& lhs, RenderStage const& rhs) {
        return lhs.ordering > rhs.ordering;
    }

protected:
    // Specifies the order in which a module is executed
    size_t ordering;
};

} // namespace RenderModules

} // namespace Saturn

#endif
