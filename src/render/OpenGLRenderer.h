#pragma once

#include "Renderer.h"

class Window;

class OpenGLRenderer final : public Renderer {
public:
    explicit OpenGLRenderer(const Window& window);
    ~OpenGLRenderer() override;

    void beginFrame() override;
    void render(const World& world) override;
    void endFrame() override;

private:
    const Window& m_window;
};
