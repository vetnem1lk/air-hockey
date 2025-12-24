#include "OpenGLRenderer.h"

#include "platform/Window.h"
#include "core/Game.h"

#include <GL/gl.h>

OpenGLRenderer::OpenGLRenderer(const Window& window)
    : m_window(window)
{
    glViewport(0, 0, m_window.width(), m_window.height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

OpenGLRenderer::~OpenGLRenderer() = default;

void OpenGLRenderer::beginFrame()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, m_window.width(), m_window.height());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_window.width(), 0, m_window.height(), -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenGLRenderer::render(const World& world)
{
    // Шайба
    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(world.puck.position.x, world.puck.position.y);
    int segments = 32;
    for(int i = 0; i <= segments; ++i) {
        float angle = i / (float)segments * 2.f * 3.1415926f;
        float x = world.puck.position.x + cosf(angle) * world.puck.radius;
        float y = world.puck.position.y + sinf(angle) * world.puck.radius;
        glVertex2f(x, y);
    }
    glEnd();

    // Левая бита
    glColor3f(0.f, 0.f, 1.f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(world.leftPaddle.position.x, world.leftPaddle.position.y);
    for(int i = 0; i <= segments; ++i) {
        float angle = i / (float)segments * 2.f * 3.1415926f;
        float x = world.leftPaddle.position.x + cosf(angle) * world.leftPaddle.radius;
        float y = world.leftPaddle.position.y + sinf(angle) * world.leftPaddle.radius;
        glVertex2f(x, y);
    }
    glEnd();

    // Правая бита
    glColor3f(1.f, 0.f, 0.f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(world.rightPaddle.position.x, world.rightPaddle.position.y);
    for(int i = 0; i <= segments; ++i) {
        float angle = i / (float)segments * 2.f * 3.1415926f;
        float x = world.rightPaddle.position.x + cosf(angle) * world.rightPaddle.radius;
        float y = world.rightPaddle.position.y + sinf(angle) * world.rightPaddle.radius;
        glVertex2f(x, y);
    }
    glEnd();

    // каток
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(world.rink.left,  world.rink.bottom);
    glVertex2f(world.rink.right, world.rink.bottom);
    glVertex2f(world.rink.right, world.rink.top);
    glVertex2f(world.rink.left,  world.rink.top);
    glEnd();
}


void OpenGLRenderer::endFrame()
{
}
