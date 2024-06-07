#include "gaussianMath.hpp"

#include <opencv2/opencv.hpp>
#include <cmath>

float gaussian(float inputValue, float peakValue, float mean, float standardDeviation)
{
    float temp = (inputValue - mean) / standardDeviation;
    return peakValue * exp(-(temp * temp / 2.0));
}

void gaussian(const cv::Mat& inputMatrix, cv::Mat& outputMatrix, float peakValue, float mean, float standardDeviation)
{
    outputMatrix = inputMatrix;

    outputMatrix.forEach<float>(
        [peakValue, mean, standardDeviation]
        (float& value, const int* position)
        {
            value = gaussian(value, peakValue, mean, standardDeviation);
        }
    );
}

float growth(float inputValue, float magnitude, float mean, float standardDeviation)
{
    float temp = (inputValue - mean) / standardDeviation;
    return magnitude * (2.0 * exp(-(temp * temp) / 2.0) - 1.0);    
}

void growth(const cv::Mat& inputMatrix, cv::Mat& outputMatrix, float magnitude, float mean, float standardDeviation)
{
    outputMatrix = inputMatrix;

    outputMatrix.forEach<float>(
        [magnitude, mean, standardDeviation]
        (float& value, const int* position)
        {
            value = growth(value, magnitude, mean, standardDeviation);
        }
    );
}
