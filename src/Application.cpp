#include <iostream> // std::cout
#include <map>      // std::map

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
    // znear is 0.1f because I like my near clipping plane to not
    // poke me in the face
    // [TODO] should this just be a default value set in the camera class?
    camera.setPerspective(60.0f, (float)width / (float)height, 0.1f, 512.0f);

    // set camera movement speed
    // camera.setMovementSpeed(2.5f);

    // we can also set the camera position
    // and the camera direction
    // camera.setCameraPos(glm::vec3(0.0f, 0.0f, 3.0f));
    // camera.setCameraDirection(glm::vec3(0.0f, 0.0f, -1.0f));

    // Constant physics time step
    const float timeStep = 1.0f / 60.0f;
    float accumulator = 0.0f;

    while (!glfwWindowShouldClose(window.window)) {
        tick();
        window.updateTitle(applicationName);
        glfwPollEvents();
        camera.updateCameraPos(tickObject.timeDelta);
        render.drawFrame(camera.matrices);

        // Add the time difference in the accumulator
        accumulator += tickObject.timeDelta;

        // While there is enough accumulated time to take
        // one or several physics steps
        while (accumulator >= timeStep) {
            // Update the Dynamics world with a constant time step
            render.world->update(timeStep);

            // For each body in the world
            // Get the updated position of the body
            for (int i = 0; i < render.objects.size(); i++) {
                // [TODO] move all this to a function in the Model class
                glm::mat4 currentModelMatrix =
                    render.objects[i]->getModelMatrix();
                rp3d::RigidBody* body = render.objects[i]->physicsBody;

                const rp3d::Transform& transform = body->getTransform();
                const rp3d::Vector3& position = transform.getPosition();

                // Update position
                currentModelMatrix[3] = glm::vec4(
                    glm::vec3(position.x, position.y, position.z), 1.0f);

                // Get the orientation quaternion.
                const rp3d::Quaternion r = transform.getOrientation();

                // Convert the quaternion to a glm::quat
                const glm::quat glm_quat(r.w, r.x, r.y, r.z);

                // Convert the glm::quat to a rotation matrix
                const glm::mat4 rotationMatrix = glm::mat4_cast(glm_quat);

                // get current object scale from the current model matrix
                glm::vec3 scale = glm::vec3(glm::length(currentModelMatrix[0]),
                                            glm::length(currentModelMatrix[1]),
                                            glm::length(currentModelMatrix[2]));

                // Replace the rotation part of the model matrix with the new
                // rotation matrix and re-apply the scale
                currentModelMatrix = glm::scale(
                    glm::mat4(rotationMatrix[0], rotationMatrix[1],
                              rotationMatrix[2], currentModelMatrix[3]),
                    scale);

                render.objects[i]->setModelMatrix(currentModelMatrix);
            }

            // Decrease the accumulated time
            accumulator -= timeStep;
        }
    }

    vkDeviceWaitIdle(render.vulkanSetup.device);
}

void Application::cleanup() {
    render.cleanup();
    window.cleanup();
}
