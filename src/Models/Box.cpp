#include "Box.hpp"
#include "Model.hpp"

int Box::totalNbBoxes = 0;

Box::Box(glm::vec3 scale, bool createRigidBody, bool matrixOffset)
    : Model(totalNbBoxes, scale, createRigidBody, matrixOffset) {
    MODEL_PATH = "models/cube.obj";
    TEXTURE_PATH = "textures/cube_orientation.png";

    totalNbBoxes++;
}
