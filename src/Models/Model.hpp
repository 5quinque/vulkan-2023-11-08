#pragma once

#include <iostream>      // std::cout
#include <optional>      // std::optional
#include <unordered_map> // std::unordered_map
#include <vector>        // std::vector

#include <glm/glm.hpp> // glm::mat4

#include "../Vertex.hpp"

class Model {
    int modelId;

  protected:
    std::string MODEL_PATH;
    std::string TEXTURE_PATH;

    glm::mat4 model = glm::mat4(1.0f);

    float mSize[3];
    glm::mat4 mScalingMatrix;

  public:
    // Model(int modelId, glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f), bool
    // createRigidBody = true) : modelId(modelId) {
    Model(int modelId) : modelId(modelId) {
        std::cout << "Model constructor" << std::endl;

        // // Initialize the size of the box
        // mSize[0] = size.x * 0.5f;
        // mSize[1] = size.y * 0.5f;
        // mSize[2] = size.z * 0.5f;

        // // Compute the scaling matrix
        // mScalingMatrix = glm::mat4(mSize[0], 0, 0, 0,
        //                             0, mSize[1], 0, 0,
        //                             0, 0, mSize[2], 0,
        //                             0, 0, 0, 1);

        setModelMatrix(glm::translate(
            model, glm::vec3(0.0f, static_cast<float>(modelId) * 2, 0.0f)));
    }

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    void loadModel();
    glm::mat4 getModelMatrix();
    void setModelMatrix(glm::mat4 model);

    int getModelId() { return modelId; }
};
