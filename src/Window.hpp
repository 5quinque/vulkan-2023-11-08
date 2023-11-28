#pragma once

#include <GLFW/glfw3.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class Window {
  public:
    bool framebufferResized = false;

    GLFWwindow* window;

    void initWindow();
    void cleanup();

  private:
    bool fullscreen = false;

    static void framebufferResizeCallback(GLFWwindow* window, int width,
                                          int height);

    static void keyCallback(GLFWwindow* window, int key, int scancode,
                            int action, int mods);
};
