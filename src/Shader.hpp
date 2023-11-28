#pragma once

#include <iostream> // std::cout

#include "Vertex.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp> // glm::mat4

// I'm still undecided if this is the right place for this struct.
struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

class Shader {
    VkDevice* devicePtr;
    VkRenderPass* renderPassPtr;

    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;

  public:
    VkPipeline graphicsPipeline;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;

    std::vector<Vertex> t_vertices;
    std::vector<uint16_t> t_indices;

    Shader(VkDevice* devicePtr, VkRenderPass* renderPassPtr)
        : devicePtr(devicePtr), renderPassPtr(renderPassPtr) {
        // print the render pass pointer
        std::cout << "Shader, devicePtr: " << devicePtr << std::endl;
        std::cout << "Shader, renderPassPtr: " << renderPassPtr << std::endl;

        this->t_vertices = vertices;
        this->t_indices = indices;

        // cout the vertices pointer
        std::cout << "Shader, vertices: " << &vertices << std::endl;
    }

    void loadShaders();
    void createGraphicsPipeline();
    void destroyGraphicsPipeline();
    void destroyPipelineLayout();
    void createDescriptorSetLayout();
    void cleanup();

  private:
    static std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code);
};