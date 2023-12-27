#pragma once

#include <cmath>
#include <iostream>      // std::cout
#include <optional>      // std::optional
#include <unordered_map> // std::unordered_map
#include <vector>        // std::vector

#include <glm/glm.hpp> // glm::mat4

#include "../Vertex.hpp"

class Model {
    int modelId;
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

  protected:
    static bool loadedModel;
    std::string MODEL_PATH;
    std::string TEXTURE_PATH;

    glm::mat4 model = glm::mat4(1.0f);

  public:
    Model(int modelId, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
          bool createRigidBody = true, bool matrixOffset = false);
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    void loadModel();

    int getModelId() { return modelId; }

    void setScale(glm::vec3 scale) {
        this->scale = scale;
        setModelMatrix(glm::scale(model, scale));
    }

    glm::mat4 getModelMatrix() { return model; }
    // setModelMatrix(glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f,
    // 0.0f, 1.0f)));
    void setModelMatrix(glm::mat4 model) { this->model = model; }
};
