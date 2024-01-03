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

    // create the floor
    rp3d::Vector3 floorPosition(render.objects[0]->position.x,
                                render.objects[0]->position.y,
                                render.objects[0]->position.z);
    rp3d::Quaternion floorOrientation = rp3d::Quaternion::identity();
    rp3d::Transform floorTransform(floorPosition, floorOrientation);
    // rp3d::CollisionBody* floorBody =
    // render.world->createCollisionBody(floorTransform);
    rp3d::RigidBody* floorBody =
        render.world->createRigidBody(floorTransform); // Change this line

    // Set the mass of the floor to zero to make it static
    // floorBody->setMass(rp3d::decimal(0.0));
    floorBody->setType(rp3d::BodyType::STATIC);

    const rp3d::Vector3 floorHalfExtents(25, 0.10, 25);
    // rp3d::BoxShape* floorShape =
    // render.physicsCommon.createBoxShape(rp3d::Vector3(25.0, 1.50, 25.0));
    rp3d::BoxShape* floorShape =
        render.physicsCommon.createBoxShape(floorHalfExtents);
    rp3d::Collider* floorCollider =
        floorBody->addCollider(floorShape, rp3d::Transform::identity());

    // floorBody->updateMassPropertiesFromColliders();

    std::vector<rp3d::RigidBody*> bodies;
    std::map<rp3d::RigidBody*, std::shared_ptr<Model>> bodyMap;

    // std::map<rp3d::RigidBody*, glm::vec3> rotationDeltaMap;

    // Add the rigid body to the array of bodies
    bodies.push_back(floorBody);
    // Add the rigid body and the corresponding model to the map
    bodyMap[floorBody] = render.objects[0];
    // rotationDeltaMap[floorBody] = glm::vec3(0.0f, 0.0f, 0.0f);

    // int i = 4;
    for (int i = 1; i < render.objects.size(); i++) {
        // Create a rigid body in the world
        rp3d::Vector3 position(render.objects[i]->position.x,
                               render.objects[i]->position.y,
                               render.objects[i]->position.z);
        rp3d::Quaternion orientation = rp3d::Quaternion::identity();
        rp3d::Transform transform(position, orientation);
        rp3d::RigidBody* body = render.world->createRigidBody(transform);
        rp3d::Vector3 halfExtents(0.5, 0.5, 0.5);
        rp3d::BoxShape* boxShape =
            render.physicsCommon.createBoxShape(halfExtents);
        rp3d::Collider* boxCollider =
            body->addCollider(boxShape, rp3d::Transform::identity());
        body->updateMassPropertiesFromColliders();

        // Add the rigid body to the array of bodies
        bodies.push_back(body);

        // Add the rigid body and the corresponding model to the map
        bodyMap[body] = render.objects[i];
        // rotationDeltaMap[body] = glm::vec3(0.0f, 0.0f, 0.0f);
    }

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
            for (rp3d::RigidBody* body : bodies) {
                glm::mat4 currentModelMatrix = bodyMap[body]->getModelMatrix();

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

                // Replace the rotation part of the model matrix with the new
                // rotation matrix
                currentModelMatrix =
                    glm::mat4(rotationMatrix[0], rotationMatrix[1],
                              rotationMatrix[2], currentModelMatrix[3]);

                // // Multiply the current model matrix with the rotation matrix
                // currentModelMatrix = currentModelMatrix * rotationMatrix;

                // // std::cout << "Quaternion: (" << r.x << ", " << r.y << ", "
                // << r.z << ", " << r.w << ")" << std::endl;

                // const glm::quat glm_quat(r.w, r.x, r.y, r.z);
                // const glm::vec3 euler = glm::eulerAngles(glm_quat);

                // // Update rotation
                // // currentModelMatrix[0] = glm::vec4(
                // //     glm::rotate(glm::mat4(1.0f), euler.x, glm::vec3(1.0f,
                // 0.0f, 0.0f))[0]);
                // // currentModelMatrix[1] = glm::vec4(
                // //     glm::rotate(glm::mat4(1.0f), euler.y,
                // glm::vec3(0.0f, 1.0f, 0.0f))[1]);
                // // currentModelMatrix[2] = glm::vec4(
                // //     glm::rotate(glm::mat4(1.0f), euler.z, glm::vec3(0.0f,
                // 0.0f, 1.0f))[2]);

                // // currentModelMatrix =
                // //     glm::rotate(currentModelMatrix,
                // // return_trasform.set_rotation(Core::SVector3(euler.x,
                // euler.y, euler.z));
                // // bodyMap[body]->setModelMatrix(currentModelMatrix);

                // glm::vec3 rotationDelta = rotationDeltaMap[body] -
                // glm::vec3(euler.x, euler.y, euler.z);

                // std::cout << "Rotation Delta: (" << rotationDelta.x << ", "
                // << rotationDelta.y << ", " << rotationDelta.z << ")" <<
                // std::endl;

                // glm::quat quaternion(r.w, rotationDelta.x, rotationDelta.y,
                // rotationDelta.z); glm::vec3 axis = glm::axis(quaternion);
                // float angle = glm::angle(quaternion);

                // // Convert the angle from radians to degrees
                // // angle = glm::degrees(angle);

                // // Update rotation
                // currentModelMatrix =
                //     glm::rotate(currentModelMatrix, angle, axis);

                // Set the updated model matrix

                if (bodyMap[body]->getModelId() == 0) {
                    currentModelMatrix = glm::scale(
                        currentModelMatrix, glm::vec3(50.0f, 0.1f, 50.0f));
                } else {
                    currentModelMatrix = glm::scale(
                        currentModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
                }

                bodyMap[body]->setModelMatrix(currentModelMatrix);

                // rotationDeltaMap[body] = glm::vec3(r.x, r.y, r.z);
            }

            // for(int i = 1; i < render.objects.size(); i++) {
            //     render.objects[i]->setModelMatrix(glm::translate(glm::mat4(1.0f),
            //     glm::vec3(position.x, position.y, position.z)));
            //     // std::cout << "Model Position: (" <<
            //     render.objects[i]->position.x << ", " <<
            //     // render.objects[i]->position.y << ", " <<
            //     render.objects[i]->position.z << ")" << std::endl;
            // }

            // // Display the position of the body
            // std::cout << "Body Position: (" << position.x << ", " <<
            // position.y << ", " << position.z << ")" << std::endl;

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
