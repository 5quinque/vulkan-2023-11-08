#pragma once

#include <array>       // std::array
#include <glm/glm.hpp> // glm::vec3
#include <vector>      // std::vector

#include <vulkan/vulkan.h> // VkVertexInputBindingDescription, VkVertexInputAttributeDescription

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2>
    getAttributeDescriptions();
};

const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}}, // top left
    {{0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}},  // top right
    {{0.5f, 0.5f}, {0.0f, 0.8f, 0.0f}},   // bottom right
    {{-0.5f, 0.5f}, {0.0f, 0.8f, 0.0f}}   // bottom left
};

// const std::vector<Vertex> vertices = {
//     {{-1.0f, -1.0f}, {1.0f, 0.0f, 1.0f}}, // top left
//     {{0.0f, -1.0f}, {1.0f, 0.0f, 1.0f}},  // top right
//     {{0.0f, 0.0f}, {0.0f, 0.8f, 0.0f}},   // bottom right
//     {{-1.0f, 0.0f}, {0.0f, 0.8f, 0.0f}}   // bottom left
// };

const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};