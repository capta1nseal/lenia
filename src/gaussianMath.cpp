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
        (double& value, const int* position)
        {
            value = gaussian(value, peakValue, mean, standardDeviation);
        }
    );
}

double growth(double inputValue, double magnitude, double mean, double standardDeviation)
{
    double temp = (inputValue - mean) / standardDeviation;
    return magnitude * (2.0 * exp(-(temp * temp) / 2.0) - 1.0);    
}

void growth(const cv::Mat& inputMatrix, cv::Mat& outputMatrix, double magnitude, double mean, double standardDeviation)
{
    outputMatrix = inputMatrix;

    outputMatrix.forEach<double>(
        [magnitude, mean, standardDeviation]
        (double& value, const int* position)
        {
            value = growth(value, magnitude, mean, standardDeviation);
        }
    );
}
