#ifndef SATURN_TRANSFER_MODULE_HPP_
#define SATURN_TRANSFER_MODULE_HPP_

#include "../UniformBuffer.hpp"
#include "RenderModule.hpp"

namespace Saturn::RenderModules {

class TransferModule : public RenderModule {
public:
    TransferModule();

    std::string_view str_id() const override { return "TransferModule"; }
    std::string_view description() const override {
        return "Category: Render module\n\n"
               "This render module is ran first. It transfers rendering data "
               "to the GPU to be used there. This data includes camera data "
               "and lighting info.";
    }

    void init() override;

    void
    process(Scene& scene, Viewport& viewport, Framebuffer& target) override;

    ~TransferModule() = default;

private:
    void transfer_camera_data(Scene& scene, Viewport& vp);
    void transfer_lighting_data(Scene& scene);

    UniformBuffer matrix_buffer;
    UniformBuffer camera_buffer;
    UniformBuffer lights_buffer;
};

} // namespace Saturn::RenderModules

#endif
