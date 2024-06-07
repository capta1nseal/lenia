#ifndef _LENIAKERNELDATA_
#define _LENIAKERNELDATA_

#include <vector>

#include <opencv2/opencv.hpp>

/*
struct holding all parameters of a kernel, 
as well as the kernel matrix and dft of the kernel matrix, 
to save repeatedly calculating these later
*/
struct Kernel
{
    std::vector<float> peaks;
    float mean;
    float deviation;
    float radius;
    float growthHeight;

    int source;
    int dest;

    cv::Mat kernel;
    cv::Mat fourierKernel;

    unsigned int dftWidth, dftHeight;

    // check out openlenia on github if this mess of parameters is unclear to you
    Kernel(
        unsigned int kernelRadius, unsigned int worldWidth, unsigned int worldHeight, unsigned int fourierWidth, unsigned int fourierHeight,
        const std::vector<float>& kernelRingPeakValues = {1.0}, float growthMean = 0.135, float growthStandardDeviation = 0.015,
        float relativeRadius = 1.0, float growthHeightScale = 1.0,
        int sourceChannel = 0, int destChannel = 0
    );

    // returns the dft of the kernel matrix, automatically calculated on initialization
    const cv::Mat& getFourierKernel() const;
};

#endif
