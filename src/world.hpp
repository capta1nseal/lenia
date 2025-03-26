#ifndef _LENIAWORLD_
#define _LENIAWORLD_

#include <mutex>
#include <vector>
#include <array>
#include <condition_variable>

#include "opencvWrapper.hpp"

#include "kernel.hpp"


/*
class storing all simulation data and computation methods
*/
class LeniaWorld
{
public:
    LeniaWorld(unsigned int width, unsigned int height, int radius, float frequency);

    // values will be linearly distributed between min and max
    void randomizeWorld(float min = 0.0, float max = 1.0);

    // adds noise to the world with a normal distribution, then clamps values between min and max
    void noisifyWorld(float standardDeviation = 0.1, float min = 0.0, float max = 1.0);
    
    // currently populates the kernels vector with the default pattern's hardcoded kernels
    void defineKernels();

    // calculate kernel convolutions, apply growth function, and clamp values between 0.0 and 1.0
    void progressState();

    // getter method for 3-channel world state
    const std::array<cv::Mat, 3>& state();

private:
    unsigned int worldWidth, worldHeight;
    unsigned int dftWidth, dftHeight;

    int kernelRadius;
    float timeFrequency;

    std::array<cv::Mat, 3> worldState;
    std::array<cv::Mat, 3> fourierWorldState;

    std::vector<Kernel> kernels;

    std::mutex worldMutex;
    std::condition_variable convolutionDoneYet;

    // internal multithreading method for convolution by one kernel/thread, which informs the main thread that it can begin processing results when it is ready
    static cv::Mat convolution(const Kernel& kernel, const cv::Mat& fourierChannel, const int& worldWidth, const int& worldHeight, const int& kernelRadius, const float& timeFrequency, std::condition_variable* doneYet);
};

#endif
