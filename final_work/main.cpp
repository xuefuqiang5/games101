#include "vulkan/vulkan_core.h"
#include <vulkan/vulkan.h>
#include <iostream>

int main() {
    uint32_t version = 0;
    vkEnumerateInstanceVersion(&version);
    std::cout << "Vulkan version: " 
              << VK_VERSION_MAJOR(version) << "."
              << VK_VERSION_MINOR(version) << "."
              << VK_VERSION_PATCH(version) << std::endl;
    return 0;
}
