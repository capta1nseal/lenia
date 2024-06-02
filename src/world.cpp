#include "world.hpp"

#include <opencv2/opencv.hpp>
#include <mutex>

#include <random>

#include "gaussianMath.hpp"

LeniaWorld::LeniaWorld(unsigned int width, unsigned int height)
    : worldWidth(width), worldHeight(height)
{
    kernelRadius = 50;
    timeFrequency = 5.0;

    worldState = cv::Mat::zeros(worldWidth, worldHeight, CV_64F);
    // randomizeWorld(0.0, 1.0);

    cv::Mat pattern = (cv::Mat_<double>(22, 22) << 0,0,0,0,0,0,0,0,0,0,0,0.06,0.1,0.04,0.02,0.01,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.15,0.37,0.5,0.44,0.19,0.23,0.3,0.23,0.15,0.01,0,0,0,0,0,0,0,0,0,0,0.32,0.78,0.26,0,0.11,0.11,0.1,0.08,0.18,0.16,0.17,0.24,0.09,0,0,0,0,0,0,0,0.45,0.16,0,0,0,0,0,0.15,0.15,0.16,0.15,0.1,0.09,0.21,0.24,0.12,0,0,0,0,0,0.1,0,0,0,0,0,0,0,0.17,0.39,0.43,0.34,0.25,0.15,0.16,0.15,0.25,0.03,0,0,0.15,0.06,0,0,0,0,0,0,0,0.24,0.72,0.92,0.85,0.61,0.47,0.39,0.27,0.12,0.18,0.17,0,0,0.08,0,0,0,0,0,0,0,0,1.0,1.0,1.0,1.0,0.73,0.6,0.56,0.31,0.12,0.15,0.24,0.01,0,0.16,0,0,0,0,0,0,0,0.76,1.0,1.0,1.0,1.0,0.76,0.72,0.65,0.39,0.1,0.17,0.24,0.05,0,0.05,0,0,0,0,0,0,0.21,0.83,1.0,1.0,1.0,1.0,0.86,0.85,0.76,0.36,0.17,0.13,0.21,0.07,0,0.05,0,0,0.02,0,0,0,0.4,0.91,1.0,1.0,1.0,1.0,1.0,0.95,0.79,0.36,0.21,0.09,0.18,0.04,0.06,0.08,0,0.18,0.21,0.1,0.03,0.38,0.92,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.64,0.31,0.12,0.07,0.25,0,0.05,0.12,0.27,0.4,0.34,0.42,0.93,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.97,0.33,0.16,0.05,0.1,0.26,0,0,0.25,0.21,0.39,0.99,1.0,1.0,1.0,1.0,1.0,1.0,0.86,0.89,0.94,0.83,0.13,0,0,0.04,0.21,0.18,0,0,0.06,0.29,0.63,0.84,0.97,1.0,1.0,1.0,0.96,0.46,0.33,0.36,0,0,0,0,0,0.03,0.35,0,0,0,0,0.13,0.22,0.59,0.85,0.99,1.0,0.98,0.25,0,0,0,0,0,0,0,0,0.34,0.14,0,0,0,0,0,0,0.33,0.7,0.95,0.8,0.33,0.11,0,0,0,0,0,0,0,0.11,0.26,0,0,0,0,0,0,0,0.16,0.56,0.52,0.51,0.4,0.18,0.01,0,0,0,0,0,0,0.42,0,0,0,0,0,0,0,0,0.01,0,0.33,0.47,0.33,0.05,0,0,0,0,0,0,0.35,0,0,0,0,0,0,0,0,0,0,0.26,0.32,0.13,0,0,0,0,0,0,0,0.34,0,0,0,0,0,0,0,0,0,0,0,0.22,0.25,0.03,0,0,0,0,0,0,0.46,0,0,0,0,0,0,0,0,0,0,0,0,0,0.09,0.2,0.22,0.23,0.23,0.22,0.3,0.3,0,0,0,0,0,0,0,0);

    double scaleFactor = 5.0;

    cv::resize(pattern, pattern, cv::Size(), scaleFactor, scaleFactor, cv::INTER_LINEAR);

    int padding = 35;

    cv::copyMakeBorder(pattern, pattern, padding, padding, padding, padding, cv::BORDER_CONSTANT, cv::Scalar::all(0.0));

    cv::copyMakeBorder(pattern, worldState, 0, worldWidth - pattern.rows, 0, worldHeight - pattern.cols, cv::BORDER_WRAP);

    dftWidth = cv::getOptimalDFTSize(worldState.rows + kernelRadius * 2);
    dftHeight = cv::getOptimalDFTSize(worldState.cols + kernelRadius * 2);
    cv::copyMakeBorder(worldState, fourierWorldState, kernelRadius, kernelRadius, kernelRadius, kernelRadius, cv::BORDER_WRAP);
    cv::copyMakeBorder(fourierWorldState, fourierWorldState, 0, dftHeight - (worldHeight + 2 * kernelRadius), 0, dftWidth - (worldWidth + 2 * kernelRadius), cv::BORDER_CONSTANT, cv::Scalar::all(0.0));

    defineKernels();
}

void LeniaWorld::randomizeWorld(double min, double max)
{
    std::default_random_engine randomEngine{std::random_device{}()};
    std::uniform_real_distribution<double> randomDistribution(min, max);

    worldState.forEach<double>(
        [&randomDistribution, &randomEngine]
        (double& value, const int* position)
        {
            value = randomDistribution(randomEngine);
        }
    );
}

void LeniaWorld::defineKernels()
{
    kernels.clear();
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {1.0, 5.0 / 12.0, 2.0 / 3.0}, 0.156, 0.0118));
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {1.0 / 12.0, 1.0}, 0.193, 0.049));
    kernels.push_back(Kernel(kernelRadius, worldWidth, worldHeight, dftWidth, dftHeight, {1.0}, 0.342, 0.0891));
}

void LeniaWorld::progressState()
{
    cv::copyMakeBorder(worldState, fourierWorldState, kernelRadius, kernelRadius, kernelRadius, kernelRadius, cv::BORDER_WRAP);
    cv::copyMakeBorder(fourierWorldState, fourierWorldState, 0, dftWidth - fourierWorldState.rows, 0, dftHeight - fourierWorldState.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0.0));
    
    cv::dft(fourierWorldState, fourierWorldState, 0, worldState.rows + 2 * kernelRadius);

    spectralFourierState.create(dftWidth, dftHeight, CV_64F);

    for (auto kernel : kernels)
    {
        cv::mulSpectrums(fourierWorldState, kernel.getFourierKernel(), spectralFourierState, 0);

        cv::dft(spectralFourierState, spectralFourierState, cv::DFT_INVERSE + cv::DFT_SCALE, worldState.rows + kernelRadius * 2);

        calculationMatrix.create(worldWidth, worldHeight, CV_64F);
        calculationMatrix = spectralFourierState(cv::Rect(kernelRadius * 2, kernelRadius * 2, worldHeight, worldWidth));

        gaussian(calculationMatrix, calculationMatrix, 2.0, kernel.mean, kernel.deviation);
        calculationMatrix.forEach<double>(
            [timeFrequency = timeFrequency, growthHeight = kernel.growthHeight]
            (double& value, const int* position)
            {
                value = ((value - 1.0) / timeFrequency) * growthHeight;
            }
        );

        worldState = worldState + (calculationMatrix / static_cast<double>(kernels.size()));
    }
    worldState.forEach<double>(
        []
        (double& value, const int* position)
        {
            value = std::clamp(value, 0.0, 1.0);
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
