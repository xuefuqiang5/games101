//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
class Texture{
private:
    cv::Mat image_data;

public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        
    if (image_data.empty()) {
        std::cerr << "Failed to load texture image: " << name << std::endl;
        width = 0;
        height = 0;
        return; // 或者抛出异常
    }
            cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);

    }
    Eigen::Vector3f getColorBilinear(float u, float v){
        // 将 u,v 转换为图像坐标系
    float x = u * width;
    float y = (1.0f - v) * height;

    int x0 = std::clamp(int(floor(x)), 0, width - 2);
    int y0 = std::clamp(int(floor(y)), 0, height - 2);

    float dx = x - x0;
    float dy = y - y0;

    auto lerp = [](float t, const Eigen::Vector3f& a, const Eigen::Vector3f& b){
        return a + t * (b - a);
    };

    std::vector<Eigen::Vector3f> colors;

    for(int j = 0; j < 2; j++){
        for(int i = 0; i < 2; i++){
            int xi = x0 + i;
            int yj = y0 + j;

            const auto& color = image_data.at<cv::Vec3b>(yj, xi);
            colors.push_back(Eigen::Vector3f(color[0], color[1], color[2]));
        }
    }

    // colors 顺序为： [c00, c10, c01, c11]
    Eigen::Vector3f c0 = lerp(dx, colors[0], colors[1]);  // top row
    Eigen::Vector3f c1 = lerp(dx, colors[2], colors[3]);  // bottom row

    return lerp(dy, c0, c1);
    }


};
#endif //RASTERIZER_TEXTURE_H
