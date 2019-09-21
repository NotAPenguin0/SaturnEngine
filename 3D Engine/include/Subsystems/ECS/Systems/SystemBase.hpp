#ifndef SATURN_SYSTEM_BASE_HPP_
#define SATURN_SYSTEM_BASE_HPP_

#include "Subsystems/Serialization/CodeGenDefinitions.hpp"

namespace Saturn {
class Scene;
}

namespace Saturn {
namespace Systems {

#define SYSTEM_RUN_IN_EDITOR                                                   \
    inline bool run_in_editor() const override { return true; }

#define SYSTEM_EDITOR_ONLY                                                     \
    inline bool run_in_editor_only() const override { return true; }

class SystemBase {
public:
    virtual ~SystemBase() = 0;

    virtual void on_start(Scene& scene);
    virtual void on_update(Scene& scene) = 0;
    virtual void on_late_update(Scene& scene);
	virtual void on_exit(Scene& scene) {}

    virtual inline bool run_in_editor() const { return false; }
    virtual inline bool run_in_editor_only() const { return false; }
};

} // namespace Systems
} // namespace Saturn

#endif
