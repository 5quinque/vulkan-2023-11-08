#pragma once

#include "Render.hpp"
#include "Window.hpp"

class Application {
  public:
    Window window;
    Render render;

    Application() : window(), render(window) {}
    void run();

  private:
    void mainLoop();
    void cleanup();
};
