#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "application.hpp"


int main() {
    HelloTriangleApplication app;

    std::cout << "Hello, Triangle!" << std::endl;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
