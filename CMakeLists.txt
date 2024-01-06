cmake_minimum_required(VERSION 3.10)
project(VulkanTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DTRACY_ENABLE")

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

add_executable(VulkanTest ${SOURCES})

target_link_libraries(VulkanTest
  glfw
  ${VULKAN_LIBRARY}
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