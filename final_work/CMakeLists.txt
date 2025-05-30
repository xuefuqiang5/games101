cmake_minimum_required(VERSION 3.10)
project(MyVulkanApp)

set(CMAKE_CXX_STANDARD 17)

# Vulkan SDK path
set(VULKAN_SDK "/Users/xuewenqi/VulkanSDK/1.4.313.1/macOS")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include Vulkan headers
include_directories("${VULKAN_SDK}/include")

# Link Vulkan library
link_directories("${VULKAN_SDK}/lib")

# Define your source files
add_executable(MyVulkanApp main.cpp)

# Link against the Vulkan library
target_link_libraries(MyVulkanApp vulkan.1)
