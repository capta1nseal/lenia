#ifndef _LENIAWORLD_
#define _LENIAWORLD_

#include <mutex>
#include <vector>
#include <array>
#include <condition_variable>

#include <opencv2/opencv.hpp>

#include "kernel.hpp"

class LeniaWorld
{
public:
    LeniaWorld(unsigned int width = 16*40, unsigned int height = 9*40);

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

    std::vector<Kernel> kernels;

    std::mutex worldMutex;
    std::condition_variable convolutionDoneYet;

    static cv::Mat convolution(const Kernel& kernel, const cv::Mat& fourierChannel, const int& worldWidth, const int& worldHeight, const int& kernelRadius, const double& timeFrequency, std::condition_variable* doneYet);
};

#endif
