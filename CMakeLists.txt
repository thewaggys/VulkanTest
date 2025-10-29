cmake_minimum_required(VERSION 4.0)
project(VulkanTest)

set(CMAKE_CXX_STANDARD 14)

include_directories(VulkanTest)

include(FetchContent)
FETCHCONTENT_DECLARE(GLFW GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.4)
FETCHCONTENT_MAKEAVAILABLE(GLFW)

find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    include_directories(${Vulkan_INCLUDE_DIRS})
    link_directories(${Vulkan_LIBRARY_DIRS})
endif ()

add_executable(VulkanTest
VulkanTest/first_app.cpp
VulkanTest/first_app.hpp
VulkanTest/Rendering/device/window/GameEngine_window.cpp
VulkanTest/Rendering/device/window/GameEngine_window.hpp
VulkanTest/Rendering/device/GameEngine_device.cpp
VulkanTest/Rendering/device/GameEngine_device.hpp
VulkanTest/Rendering/gameObject/ge_gameObject.hpp
VulkanTest/Rendering/pipeline/GameEngine_pipeline.cpp
VulkanTest/Rendering/pipeline/GameEngine_pipeline.hpp
VulkanTest/Rendering/Shaders/simple_shader.frag.spv
VulkanTest/Rendering/Shaders/simple_shader.vert.spv
VulkanTest/Rendering/swapchain/GameEngine_swap_chain.cpp
VulkanTest/Rendering/swapchain/GameEngine_swap_chain.hpp
VulkanTest/Rendering/pipeline/model/GameEngine_model.cpp
VulkanTest/Rendering/pipeline/model/GameEngine_model.hpp
VulkanTest/main.cpp
VulkanTest/Rendering/ge_renderer.cpp
VulkanTest/Rendering/ge_renderer.hpp
VulkanTest/Rendering/Shaders/simple_render_system.cpp
VulkanTest/Rendering/Shaders/simple_render_system.hpp
)


target_link_libraries(VulkanTest glfw ${Vulkan_LIBRARIES})
