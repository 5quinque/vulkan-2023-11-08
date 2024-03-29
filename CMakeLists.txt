cmake_minimum_required(VERSION 3.16)
project(VulkanTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 ")


# n.b this will only copy the files when you run the CMake command.
#     If you add new files to these directories, you will need to rerun
#     CMake to copy them to the build directory.
# Copy all shader files
file(GLOB SHADER_FILES ${CMAKE_SOURCE_DIR}/shaders/*)
file(COPY ${SHADER_FILES} DESTINATION ${CMAKE_BINARY_DIR}/shaders)

# Copy all model files
file(GLOB MODEL_FILES ${CMAKE_SOURCE_DIR}/models/*)
file(COPY ${MODEL_FILES} DESTINATION ${CMAKE_BINARY_DIR}/models)

# Copy all texture files
file(GLOB TEXTURE_FILES ${CMAKE_SOURCE_DIR}/textures/*)
file(COPY ${TEXTURE_FILES} DESTINATION ${CMAKE_BINARY_DIR}/textures)



if(DEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fno-inline")
endif()

include_directories(${PROJECT_SOURCE_DIR}/libs)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(X11 REQUIRED)
find_package(Threads REQUIRED)

set(SOURCES
  src/Models/Box.cpp
  src/Models/Bridge.cpp
  src/Models/Commodore.cpp
  src/Models/Hatchet.cpp
  src/Models/House.cpp
  src/Models/Skull.cpp
  src/Models/Model.cpp
  src/Shader.cpp
  src/Render.cpp
  src/Vertex.cpp
  src/Window.cpp
  src/VulkanSetup.cpp
  src/Application.cpp
  src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
  glfw
  Vulkan::Vulkan
  ${CMAKE_DL_LIBS}
  ${CMAKE_THREAD_LIBS_INIT}
  ${X11_LIBRARIES}
  ${X11_Xxf86vm_LIB}
  ${X11_Xrandr_LIB}
  ${X11_Xi_LIB}
  reactphysics3d
)

add_custom_target(test
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/VulkanTest
  DEPENDS VulkanTest
)
