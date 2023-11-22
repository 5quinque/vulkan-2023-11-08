#include "Application.hpp"

void Application::run() {
    window.initWindow();
    render.initVulkan();
    mainLoop();
    cleanup();
}

void Application::mainLoop() {
    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();
        render.drawFrame();
    }

    vkDeviceWaitIdle(render.vulkanSetup.device);
}

void Application::cleanup() {
    render.cleanup();
    render.vulkanSetup.cleanup();
    window.cleanup();
}
