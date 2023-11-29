#include "Application.hpp"

void Application::run() {
    window.initWindow();
    render.initVulkan();
    mainLoop();
    cleanup();
}

void Application::mainLoop() {
    while (!glfwWindowShouldClose(window.window)) {
        window.updateTitle(applicationName);

        glfwPollEvents();
        render.drawFrame();
    }

    vkDeviceWaitIdle(render.vulkanSetup.device);
}

void Application::cleanup() {
    render.cleanup();
    window.cleanup();
}
