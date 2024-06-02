#include "world.hpp"

#include <opencv2/opencv.hpp>
#include <mutex>

#include <random>

#include "gaussianMath.hpp"

LeniaWorld::LeniaWorld(unsigned int width, unsigned int height)
    : worldWidth(width), worldHeight(height)
{
    timeFrequency = 10.0;

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
            worldState.at<double>(x, y) = randomDistribution(randomEngine);
        }
    }
}

void LeniaWorld::defineKernel()
{
    kernel = cv::Mat::zeros(21, 21, CV_64F);

    int kernelRadius = kernel.cols / 2;
    int center = kernelRadius;

    for (int x = 0; x < kernel.rows; x++)
    {
        for (int y = 0; y < kernel.cols; y++)
        {
            int dx = center - x;
            int dy = center - y;
            double normalizedDifference = sqrt(dx * dx + dy * dy) / kernelRadius;
            kernel.at<double>(x, y) = (normalizedDifference < 1.0) * gaussian(normalizedDifference, 1.0, 0.5, 0.15);
        }
    }

    kernel = kernel / cv::sum(kernel);
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

            value = std::clamp(value + (1 / this->timeFrequency) * this->growthFunction(calculationMatrix.at<double>(x + kernelRadius, y + kernelRadius)), 0.0, 1.0);
        }
    );
}

void LeniaWorld::printToTerminal()
{
    char asciiGradient[] = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
    std::vector<double> asciiBrightnesses = {0.0, 0.0751, 0.0829, 0.0848, 0.1227, 0.1403, 0.1559, 0.185, 0.2183, 0.2417, 0.2571, 0.2852, 0.2902, 0.2919, 0.3099, 0.3192, 0.3232, 0.3294, 0.3384, 0.3609, 0.3619, 0.3667, 0.3737, 0.3747, 0.3838, 0.3921, 0.396, 0.3984, 0.3993, 0.4075, 0.4091, 0.4101, 0.42, 0.423, 0.4247, 0.4274, 0.4293, 0.4328, 0.4382, 0.4385, 0.442, 0.4473, 0.4477, 0.4503, 0.4562, 0.458, 0.461, 0.4638, 0.4667, 0.4686, 0.4693, 0.4703, 0.4833, 0.4881, 0.4944, 0.4953, 0.4992, 0.5509, 0.5567, 0.5569, 0.5591, 0.5602, 0.5602, 0.565, 0.5776, 0.5777, 0.5818, 0.587, 0.5972, 0.5999, 0.6043, 0.6049, 0.6093, 0.6099, 0.6465, 0.6561, 0.6595, 0.6631, 0.6714, 0.6759, 0.6809, 0.6816, 0.6925, 0.7039, 0.7086, 0.7235, 0.7302, 0.7332, 0.7602, 0.7834, 0.8037, 0.9999};

    for (unsigned int x = 0; x < worldWidth; x++) std::cout << "__";
    std::cout << "\n";
    for (unsigned int y = 0; y < worldHeight; y++)
    {
        for (unsigned int x = 0; x < worldWidth; x++)
        {
            unsigned int closestIndex = 0;
            double closestDifference = 1.0;
            for (unsigned int i = 0; i < asciiBrightnesses.size(); i++)
            {
                double difference = abs(asciiBrightnesses[i] - worldState.at<double>(x, y));
                if (difference < closestDifference)
                {
                    closestIndex = i;
                    closestDifference = difference;
                }
            }

            std::cout << asciiGradient[closestIndex] << " ";
        }
        std::cout << "\n";
    }
}

const cv::Mat& LeniaWorld::state()
{
    return worldState;
}

double LeniaWorld::growthFunction(double inputValue)
{
    // the default growth function for conway's game of life
    //if (inputValue <= 1 or inputValue > 3) return -0.5;
    //
    //if (inputValue <= 2) return 0.0;
    //
    //return 0.5;
    
    return gaussian(inputValue, 2.0, 0.135, 0.015) - 1.0;
}
