#include <cstdint> // Necessary for uint32_t
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "Window.hpp"
#include "VulkanSetup.hpp"


class HelloTriangleApplication {
public:
    // HelloTriangleApplication();
    HelloTriangleApplication() : window(), vulkanSetup(window) {}
    void run();

private:
    Window window;
    VulkanSetup vulkanSetup;

    void mainLoop();
    void cleanup();
};
