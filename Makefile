CFLAGS = -std=c++17 -O2

LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

ODIR = build/objs
_OBJS = Vertex.o Window.o VulkanSetup.o application.o main.o
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