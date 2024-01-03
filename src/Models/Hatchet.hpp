#pragma once

#include <glm/glm.hpp> // glm::mat4
#include <string>      // std::string

#include "Model.hpp"

class Hatchet : public Model {
  protected:
    static bool loadedModel;
    static int textureId;
    std::string MODEL_PATH;
    std::string TEXTURE_PATH;

  private:
    static int totalNbHatchets;
    static int vertexOffset;
    static int indexOffset;

  public:
    Hatchet(glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
            bool matrixOffset = false,
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            bool createRigidBody = true,
            rp3d::BodyType bodyType = rp3d::BodyType::DYNAMIC,
            rp3d::PhysicsWorld* world = nullptr,
            rp3d::PhysicsCommon* physicsCommon = nullptr);

    void loadModel();

    std::string getTexturePath() { return TEXTURE_PATH; }

    int getModelCount() { return totalNbHatchets; }
    int getTextureId() {
        // std::cout << "bridge getTextureId: " << textureId << std::endl;
        return textureId;
    }
    void setTextureId(int id) {
        // std::cout << "bridge setTextureId: " << id << std::endl;
        textureId = id;
    }
    int getVertexOffset() { return vertexOffset; }
    void setVertexOffset(int offset) { vertexOffset = offset; }
    int getIndexOffset() { return indexOffset; }
    void setIndexOffset(int offset) { indexOffset = offset; }
};
