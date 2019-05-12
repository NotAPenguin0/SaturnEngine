#ifndef MVG_SYSTEM_BASE_HPP_
#define MVG_SYSTEM_BASE_HPP_

namespace Saturn {
class Scene;
}

namespace Saturn {
namespace Systems {

class SystemBase {
public:
    virtual ~SystemBase() = 0;

	virtual void on_start(Scene& scene);
    virtual void on_update(Scene& scene) = 0;
};


} // namespace Systems
} // namespace Saturn

#endif
