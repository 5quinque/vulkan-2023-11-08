#include <array>  // Necessary for std::array
#include <glm/glm.hpp>  // Necessary for glm::vec3
#include <vector>

#include <vulkan/vulkan.h>  // Necessary for VkVertexInputBindingDescription, VkVertexInputAttributeDescription

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};

const std::vector<Vertex> vertices = {
    {{0.0f, -0.8f}, {0.0f, 0.8f, 0.25f}},
    {{0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
    {{0.5f, -1.0f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    {{-0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}}
};
