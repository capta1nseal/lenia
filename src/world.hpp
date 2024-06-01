#ifndef _LENIAWORLD_
#define _LENIAWORLD_

#include <opencv2/opencv.hpp>

class LeniaWorld
{
public:
    LeniaWorld(unsigned int width = 16, unsigned int height = 9);

    void randomizeWorld(double min = 0.0, double max = 1.0);

private:
    unsigned int worldWidth;
    unsigned int worldHeight;

    cv::Mat worldState;

};

#endif
