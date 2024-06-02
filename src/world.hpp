#ifndef _LENIAWORLD_
#define _LENIAWORLD_

#include <opencv2/opencv.hpp>
#include <mutex>
#include <vector>

#include "kernel.hpp"

class LeniaWorld
{
public:
    LeniaWorld(unsigned int width = 1920, unsigned int height = 1080);

    void randomizeWorld(double min = 0.0, double max = 1.0);
    
    void defineKernels();

    void progressState();

    void printToTerminal();

    const cv::Mat& state();

private:
    unsigned int worldWidth, worldHeight;
    unsigned int dftWidth, dftHeight;

    int kernelRadius;
    double timeFrequency;

    double growthMean;
    double growthStandardDeviation;

    cv::Mat worldState;
    cv::Mat fourierWorldState;
    cv::Mat wrappedWorldState;
    cv::Mat calculationMatrix;

    // the convolution kernel used to calculate the equivalent of GOL's "live neighbours"
    std::vector<Kernel> kernels;

    std::mutex worldMutex;

    // the growth function determining what happens to a cell based on its surroundings
    double growthFunction(double factor);
};

#endif
