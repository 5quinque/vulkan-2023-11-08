
## compile

```bash
make && make test

make shaders
# make clean_shaders
```

[Vulkan SDK](https://vulkan.lunarg.com/)
```bash
export VULKAN_SDK=/home/ryan/Downloads/vulkansdk/1.3.268.0
source $VULKAN_SDK"/setup-env.sh"
```


# dependancies

```bash
sudo dnf install vulkan-tools \
     glfw-devel \
     glm-devel \
     libXi-devel libXxf86vm-devel \
     vulkan-loader-devel \
     mesa-vulkan-devel vulkan-validation-layers-devel
```


# wip structure

```
main.cpp: This file would contain the main() function and the high-level control flow of your program. It would instantiate objects and call methods from other classes.

VulkanSetup.hpp and VulkanSetup.cpp: These files would contain the setup and cleanup code for Vulkan. The CreateDebugUtilsMessengerEXT and DestroyDebugUtilsMessengerEXT functions could go here. This class would handle the creation and destruction of the Vulkan instance, the physical and logical devices, the swap chain, etc.

Vertex.hpp and Vertex.cpp: These files would contain the Vertex struct and any related functions, such as the bindingDescription and attributeDescriptions functions.

Render.hpp and Render.cpp: These files would handle the rendering loop, including creating command buffers, drawing frames, and handling synchronization.

Shader.hpp and Shader.cpp: These files would handle shader-related tasks, such as loading shaders from files, creating shader modules, and managing the pipeline layout.

Window.hpp and Window.cpp: These files would handle window creation and input events. This would likely use a library like GLFW or SDL.

Each .cpp file would #include its corresponding .hpp file. main.cpp would #include the .hpp files of the other classes it uses. Each class would be responsible for cleaning up its own Vulkan resources in its destructor.
```


# resources

https://vulkan-tutorial.com/

https://github.com/KhronosGroup/Vulkan-Docs

https://github.com/David-DiGioia/vulkan-diagrams

https://github.com/KhronosGroup/Vulkan-Hpp

https://github.com/KhronosGroup/Vulkan-Guide

https://www.vulkan.org/learn#vulkan-tutorials

https://renderdoc.org/