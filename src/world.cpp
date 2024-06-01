#include "world.hpp"

#include <opencv2/opencv.hpp>

#include <random>

LeniaWorld::LeniaWorld(unsigned int width, unsigned int height)
    : worldWidth(width), worldHeight(height)
{
    worldState = cv::Mat(worldWidth, worldHeight, CV_64F, 0.0);

    randomizeWorld();

    std::cout << worldState << "\n";
}

void LeniaWorld::randomizeWorld(double min, double max)
{
    std::default_random_engine randomEngine{std::random_device{}()};
    std::uniform_real_distribution<double> randomDistribution(min, max);

    for (int x = 0; x < worldWidth; x++)
    {
        for (int y = 0; y < worldHeight; y++)
        {
            worldState.at<double>(x, y) = randomDistribution(randomEngine);
        }
    }
}
