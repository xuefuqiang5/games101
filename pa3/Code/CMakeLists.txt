cmake_minimum_required(VERSION 3.10)
project(shading)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

# 显式指定 OpenCV 配置文件位置（必须在 find_package 之前）
set(OpenCV_DIR "/opt/homebrew/share/opencv4")
find_package(OpenCV REQUIRED)

# 包含目录
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(/opt/homebrew/include)

# 添加可执行文件
add_executable(
    shading
    main.cpp
    rasterizer.hpp rasterizer.cpp
    global.hpp
    Triangle.hpp Triangle.cpp
    Texture.hpp Texture.cpp
    Shader.hpp
    OBJ_Loader.h
)

# 链接 OpenCV 库
target_link_libraries(shading ${OpenCV_LIBRARIES})
