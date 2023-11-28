STB_INCLUDE_PATH = ./libs

CFLAGS = -std=c++17 -g -O1 -I$(STB_INCLUDE_PATH)

LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

ODIR = build/objs
_OBJS = Shader.o Render.o Vertex.o Window.o VulkanSetup.o Application.o main.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

$(ODIR)/%.o: src/%.cpp
	g++ -c $(CFLAGS) -o $@ $<

VulkanTest: $(OBJS)
	g++ $(CFLAGS) -o dist/VulkanTest $(OBJS) $(LDFLAGS)

.PHONY: test clean shaders clean_shaders

test: dist/VulkanTest
	./dist/VulkanTest

clean:
	rm -f $(ODIR)/*.o
	rm -f dist/VulkanTest

shaders:
	glslc shaders/shaders.vert -o shaders/vert.spv
	glslc shaders/shaders.frag -o shaders/frag.spv

clean_shaders:
	rm -f shaders/*.spv

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./dist/VulkanTest