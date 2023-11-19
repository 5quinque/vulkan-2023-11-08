#include <cstdint> // Necessary for uint32_t
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "VulkanSetup.hpp"
#include "Window.hpp"

class HelloTriangleApplication {
  public:
    HelloTriangleApplication() : window(), vulkanSetup(window) {}
    void run();

  private:
    Window window;
    VulkanSetup vulkanSetup;

    void mainLoop();
    void cleanup();
};
