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
    std::string applicationName = "Vulkan Test";

    void mainLoop();
    void cleanup();
};
