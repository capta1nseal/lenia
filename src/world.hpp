#ifndef _LENIAWORLD_
#define _LENIAWORLD_

#include <opencv2/opencv.hpp>
#include <mutex>
#include <vector>
#include <array>

#include "kernel.hpp"

class LeniaWorld
{
public:
    LeniaWorld(unsigned int width = 16*50, unsigned int height = 9*50);

    void randomizeWorld(double min = 0.0, double max = 1.0);
    
    void defineKernels();

    void progressState();

    void printToTerminal();

    const std::array<cv::Mat, 3>& state();

private:
    unsigned int worldWidth, worldHeight;
    unsigned int dftWidth, dftHeight;

    int kernelRadius;
    double timeFrequency;

    std::array<cv::Mat, 3> worldState;
    std::array<cv::Mat, 3> fourierWorldState;
    std::array<cv::Mat, 3> spectralFourierState;
    std::array<cv::Mat, 3> calculationMatrix;

    // the convolution kernel used to calculate the equivalent of GOL's "live neighbours"
    std::vector<Kernel> kernels;

    std::mutex worldMutex;
};

#endif
