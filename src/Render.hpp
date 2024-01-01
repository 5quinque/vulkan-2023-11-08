#pragma once

#include <memory> // std::shared_ptr
#include <vector> // std::vector

#include <reactphysics3d/reactphysics3d.h>
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

    std::vector<std::shared_ptr<Model>> objects;

    rp3d::PhysicsCommon physicsCommon;
    rp3d::PhysicsWorld* world = physicsCommon.createPhysicsWorld();

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

    uint32_t currentFrame = 0;

    void recordCommandBuffer(VkCommandBuffer commandBuffer,
                             uint32_t imageIndex);
};
