#pragma once

#include <memory> // std::shared_ptr
#include <vector> // std::vector

#include <vulkan/vulkan.h>

#include "FPSCamera.hpp"
#include "Models/Model.hpp"
#include "Shader.hpp"
#include "VulkanSetup.hpp"

class Render {
    Window& window;
    VkRenderPass renderPass;

  public:
    VulkanSetup vulkanSetup;
    Shader shader;
    VkCommandPool commandPool;

    Render(Window& window)
        : window(window), vulkanSetup(window, &renderPass),
          shader(&vulkanSetup.device, &renderPass) {}

    void initVulkan();
    void createCommandPool();
    void createRenderPass();
    void createSyncObjects();
    void createCommandBuffers();

    void drawFrame(FPSCamera::Matrices& matrices);
    void updateUniformBuffer(uint32_t currentImage,
                             FPSCamera::Matrices& matrices);
    void createScene(std::vector<Vertex>& vertices,
                     std::vector<uint32_t>& indices);
    void cleanup();

  private:
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkFence> inFlightFences;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;

    std::vector<std::shared_ptr<Model>> objects;

    uint32_t currentFrame = 0;

    void recordCommandBuffer(VkCommandBuffer commandBuffer,
                             uint32_t imageIndex);
};
