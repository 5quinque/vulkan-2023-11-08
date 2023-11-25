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

        // update vertices colour every 0.1 seconds
        if (glfwGetTime() - lastUpdate > 0.01f) {
            lastUpdate = glfwGetTime();

            // std::cout << "lastFrame: " << lastUpdate << std::endl;

            // sine wave
            // y(t) = A * sin(2 * PI * f * t + shift)
            // A = the amplitude, the peak deviation of the function from zero.
            // f = the ordinary frequency, the number of oscillations (cycles)
            // t = time
            // shift = phase shift
            float sin_value = 0.015 * sin(2 * M_PI * 0.8 * glfwGetTime() + 0);
            float sin_value_2 = 0.01 * sin(2 * M_PI * 1.4 * glfwGetTime() + 0);
            float sin_value_3 = 0.003 * sin(2 * M_PI * 0.2 * glfwGetTime() + 0);
            std::cout << "sin_value: " << sin_value << std::endl;

            // update the vertices
            for (Vertex& vertex : render.shader.t_vertices) {
                // std::cout << "r: " << vertex.color.r << ", g: " <<
                // vertex.color.g << ", b: " << vertex.color.b << std::endl;

                vertex.color.r = vertex.color.r + sin_value;
                vertex.color.g = vertex.color.g + sin_value_2 + sin_value_3;
                vertex.color.b = vertex.color.b + sin_value_2;

                std::cout << "x: " << vertex.pos.x << ", y: " << vertex.pos.y
                          << std::endl;

                vertex.pos.x = vertex.pos.x + sin_value_2 + sin_value;
                vertex.pos.y = vertex.pos.y + sin_value_2 + sin_value_3;
            }

            // update the vertex buffer
            render.vulkanSetup.setVertexBuffer(render.shader.t_vertices,
                                               &render.commandPool);
        }
    }

    vkDeviceWaitIdle(render.vulkanSetup.device);
}

void Application::cleanup() {
    render.cleanup();
    render.vulkanSetup.cleanup();
    window.cleanup();
}
