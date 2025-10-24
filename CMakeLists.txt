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
        VulkanTest/Models/penis.json
        VulkanTest/Models/Untitled.glb
        VulkanTest/Shaders/shader_build.bat
        VulkanTest/Shaders/simple_shader.frag
        VulkanTest/Shaders/simple_shader.frag.spv
        VulkanTest/Shaders/simple_shader.vert
        VulkanTest/Shaders/simple_shader.vert.spv
        VulkanTest/first_app.cpp
        VulkanTest/first_app.hpp
        VulkanTest/GameEngine_device.cpp
        VulkanTest/GameEngine_device.hpp
        VulkanTest/GameEngine_model.cpp
        VulkanTest/GameEngine_model.hpp
        VulkanTest/GameEngine_pipeline.cpp
        VulkanTest/GameEngine_pipeline.hpp
        VulkanTest/GameEngine_swap_chain.cpp
        VulkanTest/GameEngine_swap_chain.hpp
        VulkanTest/GameEngine_window.cpp
        VulkanTest/GameEngine_window.hpp
        VulkanTest/main.cpp
        VulkanTest/GameEngine_swap_chain.cpp
        VulkanTest/GameEngine_swap_chain.cpp)


target_link_libraries(VulkanTest glfw ${Vulkan_LIBRARIES})
