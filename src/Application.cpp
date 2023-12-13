#include "Application.hpp"

void Application::run() {
    window.initWindow();
    render.initVulkan();
    mainLoop();
    cleanup();
}

void Application::tick() {
    tickObject.time = glfwGetTime();
    tickObject.timeDelta = tickObject.time - timeLast;
    timeLast = tickObject.time;
}

void Application::mainLoop() {
    float width = render.vulkanSetup.swapChainExtent.width;
    float height = render.vulkanSetup.swapChainExtent.height;

    // set camera position
    // x, y, z
    camera.setPosition(glm::vec3(0.0f, 2.0f, -5.0f));
    //
    camera.setRotation(glm::vec3(-25.0f, 0.0f, 0.0f));
    // fov, aspect, znear, zfar
    camera.setPerspective(60.0f, (float)width / (float)height, 0.001f, 512.0f);

    // set camera movement speed
    camera.setMovementSpeed(2.5f);

    while (!glfwWindowShouldClose(window.window)) {
        tick();
        window.updateTitle(applicationName);
        glfwPollEvents();
        camera.update(tickObject.timeDelta);
        render.drawFrame(camera.matrices);
    }

    vkDeviceWaitIdle(render.vulkanSetup.device);
}

void Application::cleanup() {
    render.cleanup();
    window.cleanup();
}
