#ifndef _LENIAGAUSSIANMATH_
#define _LENIAGAUSSIANMATH_

#include <opencv2/opencv.hpp>

double gaussian(double inputValue, double peakValue = 1.0, double mean = 0.0, double standardDeviation = 1.0);

void gaussian(const cv::Mat& inputMatrix, cv::Mat& outputMatrix, double peakValue = 1.0, double mean = 0.0, double standardDeviation = 1.0);

double growth(double inputValue, double magnitude, double mean, double standardDeviation);

void growth(const cv::Mat& inputMatrix, cv::Mat& outputMatrix, double magnitude, double mean, double standardDeviation);

#endif
