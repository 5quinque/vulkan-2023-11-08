#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#ifndef WINDOWH
#define WINDOWH

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;


class Window {
public:
    bool framebufferResized = false;

    GLFWwindow* window;

    void initWindow();
    void cleanup();
private:

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};

#endif
