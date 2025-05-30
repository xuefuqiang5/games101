cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 查找 OpenCV
find_package(OpenCV REQUIRED)

# 包含目录
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(/opt/homebrew/include)
set(CMAKE_BUILD_TYPE Debug)

# 添加可执行文件
add_executable(Rasterizer
    main.cpp
    rasterizer.cpp
    rasterizer.hpp
    Triangle.cpp
    Triangle.hpp
    global.hpp
)

# 链接 OpenCV 库
target_link_libraries(Rasterizer ${OpenCV_LIBRARIES})
