#pragma once

#include <glm/glm.hpp> // glm::mat4
#include <string>      // std::string

#include "Model.hpp"

class Box : public Model {
  private:
    /// Total number of boxes created
    static int totalNbBoxes;

  public:
    Box(glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
        bool createRigidBody = true, bool matrixOffset = false);

    std::string getTexturePath() { return TEXTURE_PATH; }
};
