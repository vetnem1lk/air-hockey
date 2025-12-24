#pragma once

class World;

class Renderer {
public:
    virtual ~Renderer() = default;

    virtual void beginFrame() = 0;
    virtual void render(const World& world) = 0;
    virtual void endFrame() = 0;
};
