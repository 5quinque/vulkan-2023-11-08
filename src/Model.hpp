#pragma once

#include <iostream>      // std::cout
#include <optional>      // std::optional
#include <unordered_map> // std::unordered_map
#include <vector>        // std::vector

#include <glm/glm.hpp> // glm::mat4

#include "Vertex.hpp"

class Model {
    int modelId;
    glm::mat4 model = glm::mat4(1.0f);

  public:
    Model(int modelId) : modelId(modelId) {
        std::cout << "Model constructor" << std::endl;

        setModelMatrix(glm::translate(
            model, glm::vec3(0.0f, static_cast<float>(modelId) * 2, 0.0f)));
    }

    const std::string MODEL_PATH = "models/cube.obj";
    const std::string TEXTURE_PATH = "textures/the_metamorphosis.jpg";
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    void loadModel();
    glm::mat4 getModelMatrix();
    void setModelMatrix(glm::mat4 model);

    int getModelId() { return modelId; }
};
