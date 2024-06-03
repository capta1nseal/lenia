#include "gaussianMath.hpp"

#include <opencv2/opencv.hpp>
#include <cmath>

double gaussian(double inputValue, double peakValue, double mean, double standardDeviation)
{
    double temp = (inputValue - mean) / standardDeviation;
    return peakValue * exp(-(temp * temp / 2.0));
}

void gaussian(const cv::Mat& inputMatrix, cv::Mat& outputMatrix, double peakValue, double mean, double standardDeviation)
{
    outputMatrix = inputMatrix;

    outputMatrix.forEach<double>(
        [peakValue, mean, standardDeviation]
        (double &value, const int* position)
        {
            value = gaussian(value, peakValue, mean, standardDeviation);
        }
    );
}
