#ifndef _LENIAGAUSSIANMATH_
#define _LENIAGAUSSIANMATH_

#include <opencv2/opencv.hpp>

// calculate gaussian of one float
float gaussian(float inputValue, float peakValue = 1.0, float mean = 0.0, float standardDeviation = 1.0);

// populate outputMatrix with the gaussians of inputMatrix's values
void gaussian(const cv::Mat& inputMatrix, cv::Mat& outputMatrix, float peakValue = 1.0, float mean = 0.0, float standardDeviation = 1.0);

// calculate the lenia gaussian-based growth function of one float
float growth(float inputValue, float magnitude, float mean, float standardDeviation);

// populate outputMatrix with inputMatrix's values passed through lenia's gaussian-based growth function
void growth(const cv::Mat& inputMatrix, cv::Mat& outputMatrix, float magnitude, float mean, float standardDeviation);

#endif
