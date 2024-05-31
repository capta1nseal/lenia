#include "world.hpp"

#include <opencv2/opencv.hpp>

LeniaWorld::LeniaWorld(unsigned int width, unsigned int height)
    : worldWidth(width), worldHeight(height)
{
    worldState = cv::Mat(9, 16, CV_32F);

    std::cout << worldState << "\n";
}
