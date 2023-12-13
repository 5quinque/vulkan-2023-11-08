#include <iostream>

#include "Window.hpp"

void Window::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::updateTitle(std::string title) {
    double currentTime = glfwGetTime();

    if (currentTime - lastUpdate >= 1.0f) {
        title += " | FPS: " + std::to_string(frameCount);
        frameCount = 0;
        lastUpdate = currentTime;

        if (fullscreen) {
            title += " | Fullscreen";
        } else {
            title += " | Windowed";
        }

        glfwSetWindowTitle(window, title.c_str());
    }

    frameCount++;
}

void Window::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    // [TODO] alt+tab'ing fucks this up

    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

    if (app->camera.mouse.firstMouse) {
        // when the window is initialised set the lastX and lastY to the current
        if (app->camera.mouse.lastX != 0.0f &&
            app->camera.mouse.lastY != 0.0f) {
            app->camera.mouse.firstMouse = false;
        }
        app->camera.mouse.lastX = xpos;
        app->camera.mouse.lastY = ypos;
    }

    int32_t dx = xpos - app->camera.mouse.lastX;
    int32_t dy = ypos - app->camera.mouse.lastY;

    app->camera.rotate(glm::vec3(-dy * app->camera.rotationSpeed,
                                 dx * app->camera.rotationSpeed, 0.0f));

    app->camera.mouse.lastX = xpos;
    app->camera.mouse.lastY = ypos;
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        std::cout << "F11 pressed" << std::endl;
        if (app->fullscreen) {
            std::cout << "F11 pressed and fullscreen" << std::endl;
            app->fullscreen = false;
            glfwSetWindowMonitor(window, nullptr, 0, 0, WIDTH, HEIGHT,
                                 GLFW_DONT_CARE);
        } else {
            std::cout << "F11 pressed and not fullscreen" << std::endl;
            app->fullscreen = true;
            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, WIDTH,
                                 HEIGHT, GLFW_DONT_CARE);
        }
    }

    // [TODO] cleanup key handling
    // w, a, s, d
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        app->camera.keys.up = true;
    }
    if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
        app->camera.keys.up = false;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        app->camera.keys.left = true;
    }
    if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
        app->camera.keys.left = false;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        app->camera.keys.down = true;
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
        app->camera.keys.down = false;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        app->camera.keys.right = true;
    }
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
        app->camera.keys.right = false;
    }

    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
        app->camera.keys.shift = true;
    }
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
        app->camera.keys.shift = false;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        app->camera.keys.space = true;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
        app->camera.keys.space = false;
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
