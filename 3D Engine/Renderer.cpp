#include "Renderer.hpp"

#include "Application.hpp"
#include "Exceptions.hpp"

#include <sstream>

namespace Saturn {

static constexpr float screen_vertices[] = {
    // Vertices	        Texture coords
    -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // TL
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // BL
    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // BR
    -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // Second triangle TL
    1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // Second triangle TR
    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // Second triangle BR
};

Renderer::Renderer(CreateInfo create_info) :
    app(create_info.app), screen_size(create_info.screen_size) {

	// Setup the framebuffer
    Framebuffer::CreateInfo framebuffer_create_info;
    framebuffer_create_info.size = screen_size;
    framebuf = Framebuffer(framebuffer_create_info);

	// Create default viewport
	add_viewport(Viewport(0, 0, screen_size.x, screen_size.y));
	// Set it as the active viewport
	Viewport::set_active(get_viewport(0));

    // Create buffers
    glGenVertexArrays(1, &screen_vao);
    glGenBuffers(1, &screen_vbo);
    glBindVertexArray(screen_vao);
    glBindBuffer(GL_ARRAY_BUFFER, screen_vbo);
    // Fill the buffer. GL_STATIC_DRAW because this data will never change
    glBufferData(GL_ARRAY_BUFFER, sizeof(screen_vertices), screen_vertices,
                 GL_STATIC_DRAW);
    // Setup vertex attributes

    // Attribute 0: Position.		Type: vec3<float>	Offset: 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)0);
    // Attribute 1: Texture coords. Type: vec2<float>	Offset: 3 floats
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), //FIRST PARAM HAS TO BE 1, 0 FOR TESTING DEBUG OUTPUT
                          (void*)(3 * sizeof(float)));
    // Unbind vertex array
    glBindVertexArray(0);

	// Setup shaders
    Shader::CreateInfo default_shader_create_info;
    default_shader_create_info.vtx_path =
        "resources/shaders/postprocessing/default_v.glsl";
    default_shader_create_info.frag_path =
        "resources/shaders/postprocessing/default_f.glsl";
    default_shader = Shader::create(default_shader_create_info);
    LogSystem::write(LogSystem::Severity::Info, "Renderer created.");
    LogSystem::write(LogSystem::Severity::Warning,
                     "No postprocessing shader system in place. Default shader "
                     "will be used");
}

Renderer::Renderer(Renderer&& other) :
    app(std::move(other.app)), screen_size(std::move(other.screen_size)),
    framebuf(std::move(other.framebuf)), screen_vao(other.screen_vao),
    screen_vbo(other.screen_vbo) {

    other.screen_vao = 0;
    other.screen_vbo = 0;
}

Renderer& Renderer::operator=(Renderer&& other) {
    app = std::move(other.app);
    screen_size = std::move(other.screen_size);
    framebuf = std::move(other.framebuf);
    screen_vao = other.screen_vao;
    screen_vbo = other.screen_vbo;

    other.screen_vao = 0;
    other.screen_vbo = 0;

    return *this;
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &screen_vao);
    glDeleteBuffers(1, &screen_vbo);
}

void Renderer::clear(
    Color clear_color,
    GLenum flags /*= GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT*/) {
    Framebuffer::bind(framebuf);
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(flags);
    Framebuffer::unbind(framebuf);
}

void Renderer::update_screen() {
    // Bind screen. This is equivalent to unbinding the current framebuffer
    Framebuffer::unbind();
    glBindVertexArray(screen_vao);
    // Temporarily disable some GL functionality
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Set (postprocessing) shader
    Shader::use(default_shader);

    // Render framebuffer texture to the screen
    glBindTexture(GL_TEXTURE_2D, framebuf.texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Re enable functionality
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void Renderer::debug_log_viewport() const {
	Viewport vp = Viewport::current();
    LogSystem::write(LogSystem::Severity::Info, "Viewport info: ");
    std::ostringstream oss;
    oss << "x: " << vp.position().x << " y: " << vp.position().y << "\n";
    oss << "w: " << vp.dimensions().x << " h: " << vp.dimensions().y << "\n";
    LogSystem::write(LogSystem::Severity::Info, oss.str());
}

Viewport& Renderer::get_viewport(std::size_t index) {
	if (index >= viewports.size()) {
        throw InvalidViewportException("Invalid viewport with index " +
                                       std::to_string(index) + " requested.");
	}
    return viewports[index];
}

std::size_t Renderer::add_viewport(Viewport vp) { 
	viewports.push_back(vp);
    return viewports.size() - 1;
}

} // namespace Saturn
