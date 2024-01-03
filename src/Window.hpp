#pragma once

#include <GLFW/glfw3.h>

#include "FPSCamera.hpp"

const uint32_t WIDTH = 1920;
const uint32_t HEIGHT = 1080;

class Window {
    FPSCamera& camera;
    float& timeDelta;

  public:
    Window(FPSCamera& camera, float& timeDelta)
        : camera(camera), timeDelta(timeDelta) {}

    bool framebufferResized = false;

    GLFWwindow* window;

    void initWindow();
    void cleanup();

    void updateTitle(std::string title);

  private:
    bool fullscreen = false;

    // variable to hold the time of the last frame
    float lastUpdate = 0.0f;
    int frameCount = 0;

    static void framebufferResizeCallback(GLFWwindow* window, int width,
                                          int height);

    static void keyCallback(GLFWwindow* window, int key, int scancode,
                            int action, int mods);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
};
