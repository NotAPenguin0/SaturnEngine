#ifndef SATURN_TRANSFER_MODULE_HPP_
#define SATURN_TRANSFER_MODULE_HPP_

#include "../UniformBuffer.hpp"
#include "RenderModule.hpp"

namespace Saturn::RenderModules {

class TransferModule : public RenderModule {
public:
    TransferModule();
    void init() override;

    void
    process(Scene& scene, Viewport& viewport, Framebuffer& target) override;

private:
    void transfer_camera_data(Scene& scene, Viewport& vp);
    void transfer_lighting_data(Scene& scene);

    UniformBuffer matrix_buffer;
    UniformBuffer camera_buffer;
    UniformBuffer lights_buffer;
};

} // namespace Saturn::RenderModules

#endif
