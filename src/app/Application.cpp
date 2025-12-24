#include "Application.h"

#include "platform/IWindow.h"
#include "platform/GlfwWindow.h"

#include "core/Game.h"

#include "render/IRenderer.h"
#include "render/OpenGLRenderer.h"

Application::Application() : m_running(false), m_isPaused(false) {}
Application::~Application() = default;

int Application::run()
{
    init();
    loop();
    shutdown();
    return 0;
}

void Application::init()
{
#ifdef EMSCRIPTEN
    m_window   = std::make_unique<EmsWindow>(1280, 720, "Air Hockey");
    m_renderer = std::make_unique<WebGLRenderer>(*m_window);
#else
    m_window   = std::make_unique<GlfwWindow>(1280, 720, "Air Hockey");
    m_renderer = std::make_unique<OpenGLRenderer>(*m_window);
#endif

    m_game = std::make_unique<Game>();
    m_running = true;
}

void Application::loop()
{
    m_time.update();

    while (m_running && !m_window->shouldClose()) {
        m_time.update();
        const double dt = m_time.delta();

        m_window->pollEvents();

        if (!m_isPaused) {
            m_game->physics().applyPlayerInput(m_game->world(), m_window->input(), dt);
            m_game->update(dt);
        }

        m_renderer->beginFrame();
        m_renderer->render(m_game->world());
        m_renderer->endFrame();

        m_window->swapBuffers();
    }
}

void Application::shutdown()
{

}
