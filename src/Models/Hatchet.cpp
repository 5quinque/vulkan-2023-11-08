#include "Hatchet.hpp"
#include "Model.hpp"

int Hatchet::totalNbHatchets = 0;
bool Hatchet::loadedModel = false;
int Hatchet::textureId = 0;
int Hatchet::vertexOffset = 0;
int Hatchet::indexOffset = 0;

Hatchet::Hatchet(glm::vec3 scale, bool createRigidBody, bool matrixOffset,
                 glm::vec3 position)
    : Model(totalNbHatchets, scale, createRigidBody, matrixOffset, position) {
    MODEL_PATH = "models/hatchet.obj";
    TEXTURE_PATH = "textures/hatchet_diffuse.png";

    totalNbHatchets++;
}

void Hatchet::loadModel() {
    if (loadedModel) {
        return;
    }
    loadedModel = true;

    Model::loadModel(MODEL_PATH);
}