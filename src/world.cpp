#include "world.hpp"

#include <opencv2/opencv.hpp>
#include <mutex>

#include <random>

LeniaWorld::LeniaWorld(unsigned int width, unsigned int height)
    : worldWidth(width), worldHeight(height)
{
    worldState = cv::Mat(worldWidth, worldHeight, CV_64F, 0.0);
    randomizeWorld();

    defineKernel();
}

void LeniaWorld::randomizeWorld(double min, double max)
{
    std::default_random_engine randomEngine{std::random_device{}()};
    std::uniform_real_distribution<double> randomDistribution(min, max);

    for (int x = 0; x < worldWidth; x++)
    {
        for (int y = 0; y < worldHeight; y++)
        {
            worldState.at<double>(x, y) = round(randomDistribution(randomEngine));
        }
    }
}

void LeniaWorld::defineKernel()
{
    kernel = (cv::Mat_<double>(3, 3) << 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0);
}

void LeniaWorld::progressState()
{
    int kernelRadius = kernel.cols / 2;

    cv::copyMakeBorder(worldState, wrappedWorldState, kernelRadius, kernelRadius, kernelRadius, kernelRadius, cv::BORDER_WRAP);

    cv::filter2D(wrappedWorldState, calculationMatrix, -1, kernel, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);

    worldState.forEach<double>(
        [this, kernelRadius](double& value, const int* position)
        {
            int x = position[0];
            int y = position[1];

            value = std::clamp(value + this->growthFunction(calculationMatrix.at<double>(x + kernelRadius, y + kernelRadius)), 0.0, 1.0);
        }
    );
}

void LeniaWorld::printToTerminal()
{
    for (unsigned int x = 0; x < worldWidth; x++) std::cout << "__";
    std::cout << "\n";
    for (unsigned int y = 0; y < worldHeight; y++)
    {
        for (unsigned int x = 0; x < worldWidth; x++)
        {
            if (round(worldState.at<double>(x, y)) == 0)
            {
                std::cout << "  ";
            }
            else
            {
                std::cout << "# ";
            }
        }
        std::cout << "\n";
    }
}


double LeniaWorld::growthFunction(double factor)
{
    // currently the default growth function for conway's game of life

    if (factor <= 1 or factor > 3) return -1.0;

    if (factor <= 2) return 0.0;

    return 1.0;
}
