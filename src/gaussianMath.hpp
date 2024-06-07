#ifndef _LENIAGAUSSIANMATH_
#define _LENIAGAUSSIANMATH_

#include <opencv2/opencv.hpp>

float gaussian(float inputValue, float peakValue = 1.0, float mean = 0.0, float standardDeviation = 1.0);

void gaussian(const cv::Mat& inputMatrix, cv::Mat& outputMatrix, float peakValue = 1.0, float mean = 0.0, float standardDeviation = 1.0);

float growth(float inputValue, float magnitude, float mean, float standardDeviation);

void growth(const cv::Mat& inputMatrix, cv::Mat& outputMatrix, float magnitude, float mean, float standardDeviation);

#endif
