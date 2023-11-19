#include "application.hpp"


void HelloTriangleApplication::run() {
    window.initWindow();
    vulkanSetup.initVulkan();
    mainLoop();
    cleanup();
}

void HelloTriangleApplication::mainLoop() {
    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();
        vulkanSetup.drawFrame();
    }

    vkDeviceWaitIdle(vulkanSetup.device);
}

void HelloTriangleApplication::cleanup() {
    vulkanSetup.cleanup();
    window.cleanup();
}
