#include "Rover.hpp"
#include "Model.hpp"

int Rover::totalNbRovers = 0;
int Rover::textureId = 0;
int Rover::vertexOffset = 0;
int Rover::indexOffset = 0;
int Rover::indicesCount = 0;

Rover::Rover(glm::vec3 scale, bool matrixOffset, glm::vec3 position,
             bool createRigidBody, rp3d::BodyType bodyType,
             rp3d::PhysicsWorld* world, rp3d::PhysicsCommon* physicsCommon)
    : Model(totalNbRovers, scale, matrixOffset, position, createRigidBody,
            bodyType, world, physicsCommon) {
    MODEL_PATH = "models/rover_mesh.obj";
    TEXTURE_PATH = "textures/rover_diffuse.png";

    totalNbRovers++;
}
