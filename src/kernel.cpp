#include "kernel.hpp"

#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

#include "gaussianMath.hpp"

Kernel::Kernel(
    unsigned int kernelRadius, unsigned int worldWidth, unsigned int worldHeight, unsigned int fourierWidth, unsigned int fourierHeight,
    const std::vector<float>& kernelRingPeakValues, float growthMean, float growthStandardDeviation,
    float relativeRadius, float growthHeightScale,
    int sourceChannel, int destChannel
)
    : dftWidth(fourierWidth), dftHeight(fourierHeight), peaks(kernelRingPeakValues), mean(growthMean), deviation(growthStandardDeviation), radius(relativeRadius), growthHeight(growthHeightScale), source(sourceChannel), dest(destChannel)
{
    kernel = cv::Mat::zeros(kernelRadius * 2 + 1, kernelRadius * 2 + 1, CV_32F);

    int centerX = kernelRadius;
    int centerY = kernelRadius;

    kernel.forEach<float>(
        [centerX, centerY, kernelRadius, &peaks = peaks, radius = radius]
        (float& value, const int* position)
        {
            int dx = position[0] - centerX;
            int dy = position[1] - centerY;

            float normalizedDifference = sqrt(dx * dx + dy * dy) / kernelRadius * peaks.size() / radius;
            value = (normalizedDifference < peaks.size()) * peaks[std::min(static_cast<int>(normalizedDifference), static_cast<int>(peaks.size() - 1))] * gaussian(normalizedDifference - floor(normalizedDifference), 1.0, 0.5, 0.15);
        }
    );

    kernel = kernel / cv::sum(kernel);

    fourierKernel.create(fourierWidth, fourierHeight, CV_32F);

    cv::copyMakeBorder(kernel, fourierKernel, 0, dftWidth - (kernelRadius * 2 + 1), 0, dftHeight - (kernelRadius * 2 + 1), cv::BORDER_CONSTANT, cv::Scalar::all(0.0));

    cv::dft(fourierKernel, fourierKernel, 0, kernelRadius * 2 + 1);
}

const cv::Mat& Kernel::getFourierKernel() const
{
    return fourierKernel;
}
