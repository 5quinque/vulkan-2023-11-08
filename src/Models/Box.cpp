#include "Box.hpp"
#include "Model.hpp"

int Box::totalNbBoxes = 0;

Box::Box(glm::vec3 scale, bool createRigidBody, bool matrixOffset,
         glm::vec3 position)
    : Model(totalNbBoxes, scale, createRigidBody, matrixOffset, position) {
    MODEL_PATH = "models/cube.obj";
    TEXTURE_PATH = "textures/cube_orientation.png";

    totalNbBoxes++;
}
