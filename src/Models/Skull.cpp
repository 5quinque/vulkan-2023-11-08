#include "Skull.hpp"
#include "Model.hpp"

int Skull::totalNbSkulls = 0;
bool Skull::loadedModel = false;
int Skull::textureId = 0;
int Skull::vertexOffset = 0;
int Skull::indexOffset = 0;

Skull::Skull(glm::vec3 scale, bool createRigidBody, bool matrixOffset,
             glm::vec3 position)
    : Model(totalNbSkulls, scale, createRigidBody, matrixOffset, position) {
    MODEL_PATH = "models/skull_monster.obj";
    TEXTURE_PATH = "textures/Skull_Monster_512.png";

    totalNbSkulls++;
}

void Skull::loadModel() {
    if (loadedModel) {
        return;
    }
    loadedModel = true;

    Model::loadModel(MODEL_PATH);
}