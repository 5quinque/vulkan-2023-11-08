DEBUG = 0
DEBUG_FLAGS = -g -fno-inline

LOCAL_INCLUDE_PATH = ./libs

CFLAGS = -std=c++17 -O3 $(DEBUG_FLAGS) -I$(LOCAL_INCLUDE_PATH) -DTRACY_ENABLE

LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

ODIR = build/objs
_OBJS = Models/Box.o Models/Model.o Shader.o Render.o Vertex.o Window.o VulkanSetup.o Application.o main.o
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