#include "Application.hpp"

void Application::run() {
    window.initWindow();
    render.initVulkan();
    mainLoop();
    cleanup();
}

void Application::mainLoop() {
    // variable to hold the time of the last frame
    float lastUpdate = 0.0f;

    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();
        render.drawFrame();
    }

    vkDeviceWaitIdle(render.vulkanSetup.device);
}

void Application::cleanup() {
    render.cleanup();
    window.cleanup();
}
