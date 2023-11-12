CFLAGS = -std=c++17 -O2

LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: main.cpp
	g++ $(CFLAGS) -o VulkanTest main.cpp $(LDFLAGS)

.PHONY: test clean shaders clean_shaders

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest

shaders:
	glslc shaders/shaders.vert -o shaders/vert.spv
	glslc shaders/shaders.frag -o shaders/frag.spv

clean_shaders:
	rm -f shaders/*.spv