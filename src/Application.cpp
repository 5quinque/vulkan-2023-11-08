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

    // Add the rigid body to the array of bodies
    bodies.push_back(floorBody);
    // Add the rigid body and the corresponding model to the map
    bodyMap[floorBody] = render.objects[0];

    // int i = 4;
    for (int i = 1; i < render.objects.size(); i++) {
        // Create a rigid body in the world
        rp3d::Vector3 position(render.objects[i]->position.x,
                               render.objects[i]->position.y,
                               render.objects[i]->position.z);
        rp3d::Quaternion orientation = rp3d::Quaternion::identity();
        rp3d::Transform transform(position, orientation);
        rp3d::RigidBody* body = render.world->createRigidBody(transform);
        rp3d::Vector3 halfExtents(0.3, 0.3, 0.3);
        rp3d::BoxShape* boxShape =
            render.physicsCommon.createBoxShape(halfExtents);
        rp3d::Collider* boxCollider =
            body->addCollider(boxShape, rp3d::Transform::identity());
        body->updateMassPropertiesFromColliders();

        // Add the rigid body to the array of bodies
        bodies.push_back(body);

        // Add the rigid body and the corresponding model to the map
        bodyMap[body] = render.objects[i];
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

                // Get rotation
                const rp3d::Quaternion r = transform.getOrientation();
                glm::quat quaternion(r.w, r.x, r.y, r.z);
                glm::vec3 axis = glm::axis(quaternion);
                float angle = glm::angle(quaternion);

                // Convert the angle from radians to degrees
                angle = glm::degrees(angle);

                // Update rotation
                currentModelMatrix =
                    glm::rotate(currentModelMatrix, angle, axis);

                // Set the updated model matrix
                bodyMap[body]->setModelMatrix(currentModelMatrix);
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
