#include "world.hpp"

#include <mutex>
#include <vector>
#include <array>

#include <opencv2/opencv.hpp>

#include "kernel.hpp"


#include <random>
#include <future>
#include <chrono>
#include <thread>
#include <condition_variable>

#include "gaussianMath.hpp"

LeniaWorld::LeniaWorld(unsigned int width, unsigned int height)
    : worldWidth(width), worldHeight(height)
{
    kernelRadius = 12;
    timeFrequency = 3.0;

    for (auto& channel : worldState)
    {
        channel = cv::Mat::zeros(worldWidth, worldHeight, CV_32F);
    }

    std::array<cv::Mat, 3> pattern;

    pattern[0] = (cv::Mat_<float>(18, 24) << 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.04,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.49,1.0,0,0.03,0.49,0.49,0.28,0.16,0.03,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.6,0.47,0.31,0.58,0.51,0.35,0.28,0.22,0,0,0,0,0,0,0,0,0,0,0,0.15,0.32,0.17,0.61,0.97,0.29,0.67,0.59,0.88,1.0,0.92,0.8,0.61,0.42,0.19,0,0,0,0,0,0,0,0,0,0,0.25,0.64,0.26,0.92,0.04,0.24,0.97,1.0,1.0,1.0,1.0,0.97,0.71,0.33,0.12,0,0,0,0,0,0,0,0,0,0.38,0.84,0.99,0.78,0.67,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.95,0.62,0.37,0,0,0,0,0,0,0.04,0.11,0,0.69,0.75,0.75,0.91,1.0,1.0,0.89,1.0,1.0,1.0,1.0,1.0,1.0,0.81,0.42,0.07,0,0,0,0,0,0.44,0.63,0.04,0,0,0,0.11,0.14,0,0.05,0.64,1.0,1.0,1.0,1.0,1.0,0.92,0.56,0.23,0,0,0,0,0,0.11,0.36,0.35,0.2,0,0,0,0,0,0,0.63,1.0,1.0,1.0,1.0,1.0,0.96,0.49,0.26,0,0,0,0,0,0,0.4,0.37,0.18,0,0,0,0,0,0.04,0.41,0.52,0.67,0.82,1.0,1.0,0.91,0.4,0.23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.04,0,0.05,0.45,0.89,1.0,0.66,0.35,0.09,0,0,0,0.22,0,0,0,0.05,0.36,0.6,0.13,0.02,0.04,0.24,0.34,0.1,0,0.04,0.62,1.0,1.0,0.44,0.25,0,0,0,0,0,0.43,0.53,0.58,0.78,0.9,0.96,1.0,1.0,1.0,1.0,0.71,0.46,0.51,0.81,1.0,1.0,0.93,0.19,0.06,0,0,0,0,0,0,0.23,0.26,0.37,0.51,0.71,0.89,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.42,0.06,0,0,0,0,0,0,0,0.03,0,0,0.11,0.35,0.62,0.81,0.93,1.0,1.0,1.0,1.0,1.0,0.64,0.15,0,0,0,0,0,0,0,0,0,0,0,0.06,0.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.05,0.09,0.05,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
    pattern[1] = (cv::Mat_<float>(18, 24) << 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.02,0.28,0.42,0.44,0.34,0.18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.34,1.0,1.0,1.0,1.0,1.0,0.91,0.52,0.14,0,0,0,0,0,0,0,0,0,0,0,0,0,0.01,0.17,0.75,1.0,1.0,1.0,1.0,1.0,1.0,0.93,0.35,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.22,0.92,1.0,1.0,1.0,1.0,1.0,1.0,0.59,0.09,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.75,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.71,0.16,0,0,0,0,0,0,0,0,0,0,0,0,0.01,0.67,0.83,0.85,1.0,1.0,1.0,1.0,1.0,1.0,0.68,0.17,0,0,0,0,0,0,0,0,0,0,0,0,0.21,0.04,0.12,0.58,0.95,1.0,1.0,1.0,1.0,1.0,0.57,0.13,0,0,0,0,0,0,0,0,0,0,0,0.07,0,0,0,0.2,0.64,0.96,1.0,1.0,1.0,0.9,0.24,0.01,0,0,0,0,0,0,0,0,0,0,0.13,0.29,0,0,0,0.25,0.9,1.0,1.0,1.0,1.0,0.45,0.05,0,0,0,0,0,0,0,0,0,0,0,0.13,0.31,0.07,0,0.46,0.96,1.0,1.0,1.0,1.0,0.51,0.12,0,0,0,0,0,0,0,0,0,0,0.26,0.82,1.0,0.95,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.3,0.05,0,0,0,0,0,0,0,0,0,0,0,0.28,0.74,1.0,0.95,0.87,1.0,1.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.07,0.69,1.0,1.0,1.0,1.0,1.0,0.96,0.25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.4,0.72,0.9,0.83,0.7,0.56,0.43,0.14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
    pattern[2] = (cv::Mat_<float>(18, 24) << 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.04,0.25,0.37,0.44,0.37,0.24,0.11,0.04,0,0,0,0,0,0,0,0,0,0,0,0,0,0.19,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.75,0.4,0.15,0,0,0,0,0,0,0,0,0,0,0,0,0.14,0.48,0.83,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.4,0,0,0,0,0,0,0,0,0,0,0,0,0.62,0.78,0.94,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.64,0,0,0,0,0,0,0,0,0,0,0,0.02,0.65,0.98,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.78,0,0,0,0,0,0,0,0,0,0,0,0.15,0.48,0.93,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.79,0.05,0,0,0,0,0,0,0,0,0,0.33,0.56,0.8,1.0,1.0,1.0,0.37,0.6,0.94,1.0,1.0,1.0,1.0,0.68,0.05,0,0,0,0,0,0,0,0.35,0.51,0.76,0.89,1.0,1.0,0.72,0.15,0,0.29,0.57,0.69,0.86,1.0,0.92,0.49,0,0,0,0,0,0,0,0,0,0.38,0.86,1.0,1.0,0.96,0.31,0,0,0,0,0.02,0.2,0.52,0.37,0.11,0,0,0,0,0,0,0.01,0,0,0.07,0.75,1.0,1.0,1.0,0.48,0.03,0,0,0,0,0,0.18,0.07,0,0,0,0,0,0,0.11,0.09,0.22,0.15,0.32,0.71,0.94,1.0,1.0,0.97,0.54,0.12,0.02,0,0,0,0,0,0,0,0,0,0,0.06,0.33,0.47,0.51,0.58,0.77,0.95,1.0,1.0,1.0,1.0,0.62,0.12,0,0,0,0,0,0,0,0,0,0,0,0.04,0.4,0.69,0.88,0.95,1.0,1.0,1.0,1.0,1.0,0.93,0.68,0.22,0.02,0,0,0.01,0,0,0,0,0,0,0,0,0.39,0.69,0.91,1.0,1.0,1.0,1.0,1.0,0.85,0.52,0.35,0.24,0.17,0.07,0,0,0,0,0,0,0,0,0,0,0,0.29,0.82,1.0,1.0,1.0,1.0,1.0,1.0,0.67,0.29,0.02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.2,0.51,0.77,0.96,0.93,0.71,0.4,0.16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.08,0.07,0.03,0,0,0,0,0,0,0,0,0,0,0,0,0);

    float scaleFactor = 5.0;

    kernelRadius *= scaleFactor;
    
    int padding = 35;

    for (auto& channel : pattern)
    {
        cv::resize(channel, channel, cv::Size(), scaleFactor, scaleFactor, cv::INTER_CUBIC);
        cv::copyMakeBorder(channel, channel, padding, padding, padding, padding, cv::BORDER_CONSTANT, cv::Scalar::all(0.0));
    }
    
    for (int i = 0; i < 3; i++)
    {
        cv::copyMakeBorder(pattern[i], worldState[i], 0, worldWidth - pattern[i].rows, 0, worldHeight - pattern[i].cols, cv::BORDER_REFLECT);
    }

    noisifyWorld();

    dftWidth = cv::getOptimalDFTSize(worldWidth + kernelRadius * 2);
    dftHeight = cv::getOptimalDFTSize(worldHeight + kernelRadius * 2);

    for (int i = 0; i < 3; i++)
    {
        auto& channel = worldState[i];
        auto& fourierChannel = fourierWorldState[i];
        cv::copyMakeBorder(channel, fourierChannel, kernelRadius, kernelRadius, kernelRadius, kernelRadius, cv::BORDER_WRAP);
        cv::copyMakeBorder(fourierChannel, fourierChannel, 0, dftHeight - (worldHeight + 2 * kernelRadius), 0, dftWidth - (worldWidth + 2 * kernelRadius), cv::BORDER_CONSTANT, cv::Scalar::all(0.0));
    }

    defineKernels();
}

void LeniaWorld::randomizeWorld(float min, float max)
{
    std::default_random_engine randomEngine{std::random_device{}()};
    std::uniform_real_distribution<float> randomDistribution(min, max);

    for (auto& channel : worldState)
    {
        channel.forEach<float>(
            [&randomDistribution, &randomEngine]
            (float& value, const int* position)
            {
                value = randomDistribution(randomEngine);
            }
        );
    }
}

void LeniaWorld::noisifyWorld(float standardDeviation, float min, float max)
{
    std::default_random_engine randomEngine{std::random_device{}()};
    std::normal_distribution<float> randomDistribution(0.0, standardDeviation);

    std::mutex noiseMutex;

    for (auto& channel : worldState)
    {
        channel.forEach<float>(
            [&randomDistribution, &randomEngine, min, max, &noiseMutex]
            (float& value, const int* position)
            {
                std::unique_lock<std::mutex> lock(noiseMutex);

                value = std::clamp(value + randomDistribution(randomEngine), min, max);
            }
        );
    }
}

void LeniaWorld::defineKernels()
{
    kernels.clear();
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {1.0}, 0.272, 0.0595, 0.91, 0.138, 0, 0));
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {1.0}, 0.349, 0.1585, 0.62, 0.48, 0, 0));
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {1.0, 1.0 / 4.0}, 0.2, 0.0332, 0.5, 0.284, 0, 0));
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {0.0, 1.0}, 0.114, 0.0528, 0.97, 0.256, 1, 1));
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {1.0}, 0.447, 0.0777, 0.72, 0.5, 1, 1));
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {5.0 / 6.0, 1.0}, 0.247, 0.0342, 0.8, 0.662, 1, 1));
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {1.0}, 0.21, 0.0617, 0.96, 0.35, 2, 2));
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {1.0}, 0.462, 0.1192, 0.56, 0.218, 2, 2));
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {1.0}, 0.446, 0.1793, 0.78, 0.556, 2, 2));
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {11.0 / 12.0, 1.0}, 0.327, 0.1408, 0.79, 0.344, 0, 1));
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {3.0 / 4.0, 1.0}, 0.476, 0.0995, 0.5, 0.456, 0, 2));
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {11.0 / 12.0, 1.0}, 0.379, 0.0697, 0.72, 0.67, 1, 0));
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {1.0}, 0.262, 0.0877, 0.68, 0.42, 1, 2));
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {1.0 / 6.0, 1.0}, 0.412, 0.1101, 0.82, 0.43, 2, 0));
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {1.0}, 0.201, 0.0786, 0.82, 0.278, 2, 1));
}

void LeniaWorld::progressState()
{
    for (int i = 0; i < 3; i++)
    {
        auto& channel = worldState[i];
        auto& fourierChannel = fourierWorldState[i];

        cv::copyMakeBorder(channel, fourierChannel, kernelRadius, kernelRadius, kernelRadius, kernelRadius, cv::BORDER_WRAP);
        cv::copyMakeBorder(fourierChannel, fourierChannel, 0, dftWidth - fourierChannel.rows, 0, dftHeight - fourierChannel.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0.0));

        cv::dft(fourierChannel, fourierChannel, 0, channel.rows + 2 * kernelRadius);
    }

    std::vector<std::future<cv::Mat>> futures;
    std::vector<bool> processedFutures;

    for (auto& kernel : kernels)
    {
        auto& fourierChannel = fourierWorldState[kernel.source];

        futures.push_back(
            std::async(
                std::launch::async,
                LeniaWorld::convolution,
                kernel, fourierChannel, worldWidth, worldHeight, kernelRadius, timeFrequency, &convolutionDoneYet
            )
        );

        processedFutures.push_back(false);
    }

    {
        std::unique_lock<std::mutex> lock(worldMutex);
        convolutionDoneYet.wait(lock);
    }


    bool allFuturesProcessed = false;
    while (not allFuturesProcessed)
    {
        for (int i = 0; i < kernels.size(); i++)
        {
            if (processedFutures[i] == false and futures[i].wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
            {
                auto temp = futures[i].get();
                worldState[kernels[i].dest] += temp;
                processedFutures[i] = true;
            }
        }
        allFuturesProcessed = true;
        for (auto isProcessed : processedFutures)
        {
            if (not isProcessed) allFuturesProcessed = false;
        }
        
    }

    for (auto& channel : worldState)
    {
        channel.forEach<float>(
            []
            (float& value, const int* position)
            {
                value = std::clamp(value, 0.0f, 1.0f);
            }
        );
    }
}

const std::array<cv::Mat, 3>& LeniaWorld::state()
{
    return worldState;
}


cv::Mat LeniaWorld::convolution(const Kernel& kernel, const cv::Mat& fourierChannel, const int& worldWidth, const int& worldHeight, const int& kernelRadius, const float& timeFrequency, std::condition_variable* doneYet)
{
    cv::Mat spectralMatrix = cv::Mat::zeros(fourierChannel.rows, fourierChannel.cols, CV_32F);

    cv::mulSpectrums(fourierChannel, kernel.getFourierKernel(), spectralMatrix, 0);

    cv::dft(spectralMatrix, spectralMatrix, cv::DFT_INVERSE | cv::DFT_SCALE, worldWidth + kernelRadius * 2);

    spectralMatrix = spectralMatrix(cv::Rect(kernelRadius * 2, kernelRadius * 2, worldHeight, worldWidth));

    growth(spectralMatrix, spectralMatrix, kernel.growthHeight / timeFrequency, kernel.mean, kernel.deviation);

    doneYet->notify_one();

    return spectralMatrix;
}
