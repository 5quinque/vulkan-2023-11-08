#include "Window.hpp"

void Window::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    glfwSetKeyCallback(window, keyCallback);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        if (app->fullscreen) {
            app->fullscreen = false;
            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, WIDTH,
                                 HEIGHT, GLFW_DONT_CARE);
        } else {
            app->fullscreen = true;
            glfwSetWindowMonitor(window, nullptr, 0, 0, WIDTH, HEIGHT,
                                 GLFW_DONT_CARE);
        }
    }
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width,
                                       int height) {
    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

void Window::cleanup() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
