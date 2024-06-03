#include "world.hpp"

#include <opencv2/opencv.hpp>
#include <mutex>

#include <random>

#include "gaussianMath.hpp"

LeniaWorld::LeniaWorld(unsigned int width, unsigned int height)
    : worldWidth(width), worldHeight(height)
{
    kernelRadius = 12;
    timeFrequency = 2.0;

    for (auto &channel : worldState)
    {
        channel = cv::Mat::zeros(worldWidth, worldHeight, CV_64F);
    }
    randomizeWorld(0.0, 1.0);

    std::array<cv::Mat, 3> pattern;

    pattern[0] = (cv::Mat_<double>(18, 24) << 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.04,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.49,1.0,0,0.03,0.49,0.49,0.28,0.16,0.03,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.6,0.47,0.31,0.58,0.51,0.35,0.28,0.22,0,0,0,0,0,0,0,0,0,0,0,0.15,0.32,0.17,0.61,0.97,0.29,0.67,0.59,0.88,1.0,0.92,0.8,0.61,0.42,0.19,0,0,0,0,0,0,0,0,0,0,0.25,0.64,0.26,0.92,0.04,0.24,0.97,1.0,1.0,1.0,1.0,0.97,0.71,0.33,0.12,0,0,0,0,0,0,0,0,0,0.38,0.84,0.99,0.78,0.67,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.95,0.62,0.37,0,0,0,0,0,0,0.04,0.11,0,0.69,0.75,0.75,0.91,1.0,1.0,0.89,1.0,1.0,1.0,1.0,1.0,1.0,0.81,0.42,0.07,0,0,0,0,0,0.44,0.63,0.04,0,0,0,0.11,0.14,0,0.05,0.64,1.0,1.0,1.0,1.0,1.0,0.92,0.56,0.23,0,0,0,0,0,0.11,0.36,0.35,0.2,0,0,0,0,0,0,0.63,1.0,1.0,1.0,1.0,1.0,0.96,0.49,0.26,0,0,0,0,0,0,0.4,0.37,0.18,0,0,0,0,0,0.04,0.41,0.52,0.67,0.82,1.0,1.0,0.91,0.4,0.23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.04,0,0.05,0.45,0.89,1.0,0.66,0.35,0.09,0,0,0,0.22,0,0,0,0.05,0.36,0.6,0.13,0.02,0.04,0.24,0.34,0.1,0,0.04,0.62,1.0,1.0,0.44,0.25,0,0,0,0,0,0.43,0.53,0.58,0.78,0.9,0.96,1.0,1.0,1.0,1.0,0.71,0.46,0.51,0.81,1.0,1.0,0.93,0.19,0.06,0,0,0,0,0,0,0.23,0.26,0.37,0.51,0.71,0.89,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.42,0.06,0,0,0,0,0,0,0,0.03,0,0,0.11,0.35,0.62,0.81,0.93,1.0,1.0,1.0,1.0,1.0,0.64,0.15,0,0,0,0,0,0,0,0,0,0,0,0.06,0.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.05,0.09,0.05,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
    pattern[1] = (cv::Mat_<double>(18, 24) << 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.02,0.28,0.42,0.44,0.34,0.18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.34,1.0,1.0,1.0,1.0,1.0,0.91,0.52,0.14,0,0,0,0,0,0,0,0,0,0,0,0,0,0.01,0.17,0.75,1.0,1.0,1.0,1.0,1.0,1.0,0.93,0.35,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.22,0.92,1.0,1.0,1.0,1.0,1.0,1.0,0.59,0.09,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.75,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.71,0.16,0,0,0,0,0,0,0,0,0,0,0,0,0.01,0.67,0.83,0.85,1.0,1.0,1.0,1.0,1.0,1.0,0.68,0.17,0,0,0,0,0,0,0,0,0,0,0,0,0.21,0.04,0.12,0.58,0.95,1.0,1.0,1.0,1.0,1.0,0.57,0.13,0,0,0,0,0,0,0,0,0,0,0,0.07,0,0,0,0.2,0.64,0.96,1.0,1.0,1.0,0.9,0.24,0.01,0,0,0,0,0,0,0,0,0,0,0.13,0.29,0,0,0,0.25,0.9,1.0,1.0,1.0,1.0,0.45,0.05,0,0,0,0,0,0,0,0,0,0,0,0.13,0.31,0.07,0,0.46,0.96,1.0,1.0,1.0,1.0,0.51,0.12,0,0,0,0,0,0,0,0,0,0,0.26,0.82,1.0,0.95,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.3,0.05,0,0,0,0,0,0,0,0,0,0,0,0.28,0.74,1.0,0.95,0.87,1.0,1.0,1.0,1.0,1.0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.07,0.69,1.0,1.0,1.0,1.0,1.0,0.96,0.25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.4,0.72,0.9,0.83,0.7,0.56,0.43,0.14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
    pattern[2] = (cv::Mat_<double>(18, 24) << 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.04,0.25,0.37,0.44,0.37,0.24,0.11,0.04,0,0,0,0,0,0,0,0,0,0,0,0,0,0.19,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.75,0.4,0.15,0,0,0,0,0,0,0,0,0,0,0,0,0.14,0.48,0.83,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.4,0,0,0,0,0,0,0,0,0,0,0,0,0.62,0.78,0.94,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.64,0,0,0,0,0,0,0,0,0,0,0,0.02,0.65,0.98,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.78,0,0,0,0,0,0,0,0,0,0,0,0.15,0.48,0.93,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.79,0.05,0,0,0,0,0,0,0,0,0,0.33,0.56,0.8,1.0,1.0,1.0,0.37,0.6,0.94,1.0,1.0,1.0,1.0,0.68,0.05,0,0,0,0,0,0,0,0.35,0.51,0.76,0.89,1.0,1.0,0.72,0.15,0,0.29,0.57,0.69,0.86,1.0,0.92,0.49,0,0,0,0,0,0,0,0,0,0.38,0.86,1.0,1.0,0.96,0.31,0,0,0,0,0.02,0.2,0.52,0.37,0.11,0,0,0,0,0,0,0.01,0,0,0.07,0.75,1.0,1.0,1.0,0.48,0.03,0,0,0,0,0,0.18,0.07,0,0,0,0,0,0,0.11,0.09,0.22,0.15,0.32,0.71,0.94,1.0,1.0,0.97,0.54,0.12,0.02,0,0,0,0,0,0,0,0,0,0,0.06,0.33,0.47,0.51,0.58,0.77,0.95,1.0,1.0,1.0,1.0,0.62,0.12,0,0,0,0,0,0,0,0,0,0,0,0.04,0.4,0.69,0.88,0.95,1.0,1.0,1.0,1.0,1.0,0.93,0.68,0.22,0.02,0,0,0.01,0,0,0,0,0,0,0,0,0.39,0.69,0.91,1.0,1.0,1.0,1.0,1.0,0.85,0.52,0.35,0.24,0.17,0.07,0,0,0,0,0,0,0,0,0,0,0,0.29,0.82,1.0,1.0,1.0,1.0,1.0,1.0,0.67,0.29,0.02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.2,0.51,0.77,0.96,0.93,0.71,0.4,0.16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.08,0.07,0.03,0,0,0,0,0,0,0,0,0,0,0,0,0);

    double scaleFactor = 5.0;

    kernelRadius *= scaleFactor;
    
    int padding = 25;

    for (auto &channel : pattern)
    {
        cv::resize(channel, channel, cv::Size(), scaleFactor, scaleFactor, cv::INTER_CUBIC);
        cv::copyMakeBorder(channel, channel, padding, padding, padding, padding, cv::BORDER_CONSTANT, cv::Scalar::all(0.0));
    }
    
    for (int i = 0; i < 3; i++)
    {
        cv::copyMakeBorder(pattern[i], worldState[i], 0, worldWidth - pattern[i].rows, 0, worldHeight - pattern[i].cols, cv::BORDER_REFLECT);
    }

    dftWidth = cv::getOptimalDFTSize(worldWidth + kernelRadius * 2);
    dftHeight = cv::getOptimalDFTSize(worldHeight + kernelRadius * 2);

    for (int i = 0; i < 3; i++)
    {
        auto &channel = worldState[i];
        auto &fourierChannel = fourierWorldState[i];
        cv::copyMakeBorder(channel, fourierChannel, kernelRadius, kernelRadius, kernelRadius, kernelRadius, cv::BORDER_WRAP);
        cv::copyMakeBorder(fourierChannel, fourierChannel, 0, dftHeight - (worldHeight + 2 * kernelRadius), 0, dftWidth - (worldWidth + 2 * kernelRadius), cv::BORDER_CONSTANT, cv::Scalar::all(0.0));
    }

    defineKernels();
}

void LeniaWorld::randomizeWorld(double min, double max)
{
    std::default_random_engine randomEngine{std::random_device{}()};
    std::uniform_real_distribution<double> randomDistribution(min, max);

    for (auto &channel : worldState)
    {
        channel.forEach<double>(
            [&randomDistribution, &randomEngine]
            (double& value, const int* position)
            {
                value = randomDistribution(randomEngine);
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
        auto &channel = worldState[i];
        auto &fourierChannel = fourierWorldState[i];

        cv::copyMakeBorder(channel, fourierChannel, kernelRadius, kernelRadius, kernelRadius, kernelRadius, cv::BORDER_WRAP);
        cv::copyMakeBorder(fourierChannel, fourierChannel, 0, dftWidth - fourierChannel.rows, 0, dftHeight - fourierChannel.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0.0));

        cv::dft(fourierChannel, fourierChannel, 0, channel.rows + 2 * kernelRadius);
    }

    for (int i = 0; i < 3; i++)
    {
        auto &channel = worldState[i];
        auto &fourierChannel = fourierWorldState[i];
        auto &spectralChannel = spectralFourierState[i];
        auto &calculationChannel = calculationMatrix[i];

        spectralChannel.create(dftWidth, dftHeight, CV_64F);

        for (auto &kernel : kernels)
        {
            if (kernel.source != i) continue;
            cv::mulSpectrums(fourierChannel, kernel.getFourierKernel(), spectralChannel, 0);

            cv::dft(spectralChannel, spectralChannel, cv::DFT_INVERSE + cv::DFT_SCALE, channel.rows + kernelRadius * 2);

            calculationChannel.create(worldWidth, worldHeight, CV_64F);
            calculationChannel = spectralChannel(cv::Rect(kernelRadius * 2, kernelRadius * 2, worldHeight, worldWidth));

            gaussian(calculationChannel, calculationChannel, 2.0, kernel.mean, kernel.deviation);
            calculationChannel.forEach<double>(
                [timeFrequency = timeFrequency, growthHeight = kernel.growthHeight]
                (double& value, const int* position)
                {
                    value = ((value - 1.0) / timeFrequency) * growthHeight;
                }
            );

            worldState[kernel.dest] += calculationChannel;
        }
    }
    for (auto &channel : worldState)
    {
        channel.forEach<double>(
            []
            (double& value, const int* position)
            {
                value = std::clamp(value, 0.0, 1.0);
            }
        );
    }
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
                double difference = abs(asciiBrightnesses[i] - worldState[0].at<double>(x, y));
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

const std::array<cv::Mat, 3>& LeniaWorld::state()
{
    return worldState;
}
