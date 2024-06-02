#ifndef _LENIAKERNELDATA_
#define _LENIAKERNELDATA_

#include <vector>

#include <opencv2/opencv.hpp>

struct Kernel
{
    std::vector<double> peaks;
    double mean;
    double deviation;
    double radius;
    double growthHeight;

    int source;
    int dest;

    cv::Mat kernel;
    cv::Mat fourierKernel;

    unsigned int dftWidth, dftHeight;

    Kernel(
        unsigned int kernelRadius, unsigned int worldWidth, unsigned int worldHeight, unsigned int fourierWidth, unsigned int fourierHeight,
        const std::vector<double>& kernelRingPeakValues = {1.0}, double growthMean = 0.135, double growthStandardDeviation = 0.015,
        double relativeRadius = 1.0, double growthHeightScale = 1.0,
        int sourceChannel = 0, int destChannel = 0
    );

    const cv::Mat& getFourierKernel();
};

#endif
