#pragma once

#include "../core/Input.h"

class Window {
public:
    virtual ~Window() = default;

    virtual void pollEvents() = 0;
    virtual void swapBuffers() = 0;

    virtual bool shouldClose() const = 0;

    virtual int width() const = 0;
    virtual int height() const = 0;

    const InputState& input() const { return m_input; }

protected:
    InputState m_input;
};
