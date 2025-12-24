#pragma once

#include <memory>

class Window;
class Game;
class Renderer;

class Application {
public:
    Application();
    ~Application();

    int run();

private:
    void init();
    void loop();
    void shutdown();

private:
    bool m_running = false;

    std::unique_ptr<Window>   m_window;
    std::unique_ptr<Game>     m_game;
    std::unique_ptr<Renderer> m_renderer;
};
