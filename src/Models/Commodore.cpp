#include "Commodore.hpp"
#include "Model.hpp"

int Commodore::totalNbCommodores = 0;
bool Commodore::loadedModel = false;
int Commodore::textureId = 0;
int Commodore::vertexOffset = 0;
int Commodore::indexOffset = 0;

Commodore::Commodore(glm::vec3 scale, bool createRigidBody, bool matrixOffset,
                     glm::vec3 position)
    : Model(totalNbCommodores, scale, createRigidBody, matrixOffset, position) {
    MODEL_PATH = "models/low_spec_commodore_pet.obj";
    TEXTURE_PATH = "textures/c_pet_tex.png";

    totalNbCommodores++;
}

void Commodore::loadModel() {
    if (loadedModel) {
        return;
    }
    loadedModel = true;

    Model::loadModel(MODEL_PATH);
}