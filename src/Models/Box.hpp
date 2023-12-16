#pragma once

#include <glm/glm.hpp> // glm::mat4
#include <string>      // std::string

#include "Model.hpp"

class Box : public Model {
    // glm::mat4 model = glm::mat4(1.0f);

  private:
    /// Total number of boxes created
    static int totalNbBoxes;
    // float mSize[3];
    // glm::mat4 mScalingMatrix;

  public:
    // Box(glm::vec3 size, bool createRigidBody = true);
    Box();

    std::string getTexturePath() { return TEXTURE_PATH; }
    // glm::mat4 getModelMatrix() { return model; }
    // void setModelMatrix(glm::mat4 model);
};
