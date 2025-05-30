#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
        -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model;
    float theta = rotation_angle * MY_PI / 180.0f;
    model <<    cos(theta),    -sin(theta),     0,  0,
                sin(theta),     cos(theta),     0,  0, 
                0,              0,              1,  0, 
                0,              0,              0,  1;

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    
    return model;
}
Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    float theta = eye_fov * MY_PI / 180.0f; 
    
    float t = tan(theta) * zNear;
    float b = -t;
    float r = aspect_ratio * t;
    float l = -r; float n = zNear; float f = -std::abs(zFar); // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.

    Eigen::Matrix4f ortho1, ortho2; 
    Eigen::Matrix4f pers;
    pers << n,  0,  0,  0,
            0,  n,  0,  0,
            0,  0,  n+f,-n*f,
            0,  0,  1,  0;
    ortho1 <<2/(r-l), 0, 0, 0,
            0, 2/(t-b), 0, 0, 
            0, 0, 2/(n-f), 0,
            0, 0, 0, 1;
    ortho2 <<1, 0, 0, -(r+l)/2,
             0, 1, 0, -(t+b)/2, 
             0, 0, 1, -(n+f)/2, 
             0, 0, 0, 1;
    projection = ortho1 * ortho2 * pers; 
    return projection;
}
Eigen::Matrix4f get_rotation(Eigen::Vector3f axis, float angle){
    Eigen::Matrix4f res;
    float theta = angle * MY_PI / 180;
    if(axis == Eigen::Vector3f(1,0, 0)){
        res << 1, 0, 0, 0,
               0, cos(theta), -sin(theta), 0,
               0, sin(theta), cos(theta),  0,
               0, 0, 0, 1; 
    }
    else if(axis == Eigen::Vector3f(0,1,0)){
        res << cos(theta), 0, sin(theta), 0,
                0, 1, 0, 0,
                -sin(theta), 0, cos(theta),0,
                0, 0, 0, 1;
    }
    else {
        res << cos(theta), -sin(theta), 0, 0,
                sin(theta), cos(theta), 0, 0, 
                0, 0, 1, 0,
                0, 0, 0, 1;
    }
    

    return res;
}
Vector3f set_axis(){
    std::cout << "请选择旋转轴 (x / y / z): ";
    char c;
    while (true) {
        std::cin >> c;
        switch (c) {
            case 'x':
            case 'X':
                std::cout << "选择绕 X 轴旋转\n";
                return Eigen::Vector3f(1, 0, 0);
            case 'y':
            case 'Y':
                std::cout << "选择绕 Y 轴旋转\n";
                return Eigen::Vector3f(0, 1, 0);
            case 'z':
            case 'Z':
                std::cout << "选择绕 Z 轴旋转\n";
                return Eigen::Vector3f(0, 0, 1);
            default:
                return Eigen::Vector3f(0,0,1);
        }
    }
}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";
    Eigen::Vector3f axis = Eigen::Vector3f(0,0,1); 
    axis = set_axis();

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_rotation(axis, angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_rotation(axis, angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        }
        else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
