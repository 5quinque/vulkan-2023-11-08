#pragma once

#include <GLFW/glfw3.h>

#include "Camera.hpp"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class Window {
    Camera& camera;

  public:
    Window(Camera& camera) : camera(camera) {}

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
