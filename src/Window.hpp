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
    static void framebufferResizeCallback(GLFWwindow* window, int width,
                                          int height);
};
