#include "Bridge.hpp"
#include "Model.hpp"

int Bridge::totalNbBridges = 0;
bool Bridge::loadedModel = false;
int Bridge::textureId = 0;
int Bridge::vertexOffset = 0;
int Bridge::indexOffset = 0;

Bridge::Bridge(glm::vec3 scale, bool createRigidBody, bool matrixOffset,
               glm::vec3 position)
    : Model(totalNbBridges, scale, createRigidBody, matrixOffset, position) {
    MODEL_PATH = "models/bridge.obj";
    TEXTURE_PATH = "textures/bridge_diffuse.png";

    totalNbBridges++;
}

void Bridge::loadModel() {
    if (loadedModel) {
        return;
    }
    loadedModel = true;

    Model::loadModel(MODEL_PATH);
}