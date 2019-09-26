#ifndef SATURN_DEPTH_MAP_PASS_HPP_
#define SATURN_DEPTH_MAP_PASS_HPP_

#include "PreRenderStage.hpp"

#include "../DepthMap.hpp"
#include "../Shader.hpp"
#include "Subsystems/AssetManager/Resource.hpp"

namespace Saturn::RenderModules {

class DepthMapPass : public PreRenderStage {
public:
    DepthMapPass();

    std::string_view str_id() const override { return "DepthMapPass"; }
    std::string_view description() const override {
        return "Category: Pre-render stage\n\n"
               "This render stage is run first. It generates shadow maps to be "
               "used when rendering shadows. Disabling this is still a bit "
               "buggy.";
    }

    void init() override final;
    void process(Scene& scene) override final;

    virtual ~DepthMapPass() override;

    static DepthMap* last_depthmap;

private:
    // #TODO: Make this a setting somewhere (pass via constructor, idk)
    static constexpr size_t Precision = 1024;
    DepthMap depth_map;
    Resource<Shader> depth_shader;

    // Initializes OpenGL state for rendering
    void setup_for_render();
    // Cleans up OpenGL state after rendering
    void reset();
};

} // namespace Saturn::RenderModules

#endif
