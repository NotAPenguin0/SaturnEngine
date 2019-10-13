#ifndef SATURN_RENDER_STAGE_HPP_
#define SATURN_RENDER_STAGE_HPP_

#include <cstddef>
#include <string_view>

namespace Saturn {

namespace RenderModules {

class RenderStage {
public:
    RenderStage() = default;
    RenderStage(size_t ordering);
    virtual ~RenderStage() = default;

    virtual size_t get_ordering() const final;

    virtual std::string_view str_id() const = 0;
	virtual std::string_view description() const = 0;

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
