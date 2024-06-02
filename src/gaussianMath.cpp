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
    for (int x = 0; x < inputMatrix.rows; x++)
    {
        for (int y = 0; y < inputMatrix.cols; y++)
        {
            outputMatrix.at<double>(x, y) = gaussian(inputMatrix.at<double>(x, y), peakValue, mean, standardDeviation);
        }
    }
}
