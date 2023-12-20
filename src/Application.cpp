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

    // fov, aspect, znear, zfar
    // znear is 0.3f because I like my near clipping plane to not
    // poke me in the face
    camera.setPerspective(60.0f, (float)width / (float)height, 0.3f, 512.0f);

    // set camera movement speed
    // camera.setMovementSpeed(2.5f);

    // we can also set the camera position
    // and the camera direction
    // camera.setCameraPos(glm::vec3(0.0f, 0.0f, 3.0f));
    // camera.setCameraDirection(glm::vec3(0.0f, 0.0f, -1.0f));

    while (!glfwWindowShouldClose(window.window)) {
        tick();
        window.updateTitle(applicationName);
        glfwPollEvents();
        camera.updateCameraPos(tickObject.timeDelta);
        render.drawFrame(camera.matrices);
    }

    vkDeviceWaitIdle(render.vulkanSetup.device);
}

void Application::cleanup() {
    render.cleanup();
    window.cleanup();
}
