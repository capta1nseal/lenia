#ifndef _LENIAWORLD_
#define _LENIAWORLD_

#include <opencv2/opencv.hpp>
#include <mutex>

class LeniaWorld
{
public:
    LeniaWorld(unsigned int width = 32, unsigned int height = 18);

    void randomizeWorld(double min = 0.0, double max = 1.0);
    
    void defineKernel();

    void progressState();

    void printToTerminal();

private:
    unsigned int worldWidth;
    unsigned int worldHeight;

    cv::Mat worldState;
    cv::Mat wrappedWorldState;
    cv::Mat calculationMatrix;

    // the convolution kernel used to calculate the equivalent of GOL's "live neighbours"
    cv::Mat kernel;

    std::mutex worldMutex;

    // the growth function determining what happens to a cell based on its surroundings
    double growthFunction(double factor);
};

#endif
