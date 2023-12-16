#include <iostream> // std::cout

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale

#include <cstring> // memcpy

#include "Camera.hpp"
#include "Render.hpp"

void Render::initVulkan() {
    std::cout << "Render::initVulkan()" << std::endl;
    vulkanSetup.createInstance();
    vulkanSetup.setupDebugMessenger();
    vulkanSetup.createSurface();
    vulkanSetup.pickPhysicalDevice();
    vulkanSetup.createLogicalDevice();
    vulkanSetup.createSwapChain();
    vulkanSetup.createImageViews();
    createRenderPass();
    shader.loadShaders();
    shader.createDescriptorSetLayout();
    shader.createGraphicsPipeline();
    createCommandPool();
    vulkanSetup.createDepthResources(&commandPool);
    vulkanSetup.createFramebuffers();

    // this is not the correct place to load models
    // create 10 models and add them to the vector
    for (int i = 0; i < 10; i++) {
        Model model(i);
        model.loadModel();
        models.push_back(model);
    }

    vulkanSetup.createTextureImage(&commandPool, models[0].TEXTURE_PATH);
    vulkanSetup.createTextureImageView();
    vulkanSetup.createTextureSampler();

    vulkanSetup.createVertexBuffer(&commandPool, models[0].vertices);
    vulkanSetup.createIndexBuffer(&commandPool, models[0].indices,
                                  sizeof(models[0].indices[0]) *
                                      models[0].indices.size());
    vulkanSetup.createUniformBuffers();
    vulkanSetup.createDescriptorPool();
    vulkanSetup.createDescriptorSets(&shader.descriptorSetLayout);
    createCommandBuffers();
    createSyncObjects();
}

void Render::drawFrame(Camera::Matrices& matrices) {
    // https://vulkan-tutorial.com/en/Drawing_a_triangle/Drawing/Rendering_and_presentation#page_Outline-of-a-frame
    // At a high level, rendering a frame in Vulkan consists of a common set of
    // steps:

    //     Wait for the previous frame to finish
    vkWaitForFences(vulkanSetup.device, 1, &inFlightFences[currentFrame],
                    VK_TRUE, UINT64_MAX);

    //     Acquire an image from the swap chain
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(
        vulkanSetup.device, vulkanSetup.swapChain, UINT64_MAX,
        imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        vulkanSetup.recreateSwapChain(&commandPool);
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    updateUniformBuffer(currentFrame, matrices);

    // update position of the models
    // for (Model& model : models) {
    //     model.setModelMatrix(glm::translate(model.getModelMatrix(),
    //     glm::vec3(0.0f, 0.0001f, 0.0001f)));
    // }

    // Only reset the fence if we are submitting work
    vkResetFences(vulkanSetup.device, 1, &inFlightFences[currentFrame]);

    //     Record a command buffer which draws the scene onto that image
    vkResetCommandBuffer(commandBuffers[currentFrame], 0);
    recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

    //     Submit the recorded command buffer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];
    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(vulkanSetup.graphicsQueue, 1, &submitInfo,
                      inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    //     Present the swap chain image
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {vulkanSetup.swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional

    result = vkQueuePresentKHR(vulkanSetup.presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
        window.framebufferResized) {
        window.framebufferResized = false;
        vulkanSetup.recreateSwapChain(&commandPool);
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Render::updateUniformBuffer(uint32_t currentImage,
                                 Camera::Matrices& matrices) {
    UniformBufferObject ubo{};

    // the model is rotated around the z-axis
    ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f),
                            glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = matrices.view;
    ubo.proj = matrices.perspective;

    // Using a UBO this way is not the most efficient way to pass frequently
    // changing values to the shader. A more efficient way to pass a small
    // buffer of data to shaders are push constants.
    // https://vulkan-tutorial.com/Uniform_buffers/Descriptor_layout_and_buffer
    memcpy(vulkanSetup.uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void Render::createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = nullptr;
    semaphoreInfo.flags = 0;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(vulkanSetup.device, &semaphoreInfo, nullptr,
                              &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(vulkanSetup.device, &semaphoreInfo, nullptr,
                              &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(vulkanSetup.device, &fenceInfo, nullptr,
                          &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error(
                "failed to create synchronization objects for a frame!");
        }
    }
}

void Render::createCommandBuffers() {
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (vkAllocateCommandBuffers(vulkanSetup.device, &allocInfo,
                                 commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void Render::recordCommandBuffer(VkCommandBuffer commandBuffer,
                                 uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;                  // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.4f, 0.5f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = vulkanSetup.swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = vulkanSetup.swapChainExtent;

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
                         VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      shader.graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(vulkanSetup.swapChainExtent.width);
    viewport.height = static_cast<float>(vulkanSetup.swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = vulkanSetup.swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    VkBuffer vertexBuffers[] = {vulkanSetup.vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer, vulkanSetup.indexBuffer, 0,
                         VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(
        commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader.pipelineLayout,
        0, 1, &vulkanSetup.descriptorSets[currentFrame], 0, nullptr);

    std::vector<glm::mat4> modelMatrices;
    for (Model& model : models) {
        modelMatrices.push_back(model.getModelMatrix());
    }

    uint32_t indexCount = static_cast<uint32_t>(models[0].indices.size());
    uint32_t instanceCount = static_cast<uint32_t>(modelMatrices.size());

    for (const glm::mat4& modelMatrix : modelMatrices) {
        vkCmdPushConstants(commandBuffer, shader.pipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4),
                           &modelMatrix);
        vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, 0, 0, 0);
    }

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void Render::createCommandPool() {
    // std::cout << "Render::createCommandPool()" << std::endl;
    QueueFamilyIndices queueFamilyIndices =
        vulkanSetup.findQueueFamilies(vulkanSetup.physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(vulkanSetup.device, &poolInfo, nullptr,
                            &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

void Render::createRenderPass() {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = vulkanSetup.swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    // index of the attachment description array
    colorAttachmentRef.attachment = 0;
    // layout to transition to before subpass begins
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = vulkanSetup.findDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // subpass
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    // index of the attachment reference array
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                               VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment,
                                                          depthAttachment};
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(vulkanSetup.device, &renderPassInfo, nullptr,
                           &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

void Render::cleanup() {
    shader.destroyGraphicsPipeline();
    shader.destroyPipelineLayout();

    vkDestroyRenderPass(vulkanSetup.device, renderPass, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(vulkanSetup.device, imageAvailableSemaphores[i],
                           nullptr);
        vkDestroySemaphore(vulkanSetup.device, renderFinishedSemaphores[i],
                           nullptr);
        vkDestroyFence(vulkanSetup.device, inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(vulkanSetup.device, commandPool, nullptr);

    shader.cleanup();
    vulkanSetup.cleanup();
}
