#include "application.hpp"

#include <string>
#include <exception>
#include <iostream>

int main (int argc, char *argv[])
{
    unsigned int width, height;
    int radius;
    float timeFrequency;

    bool useDefaultParams = true;

    if (useDefaultParams) {
        width = 16u*30u
        height = 9u*30u
        radius = 12
        timeFrequency = 3.0
    }

    std::cout << "Size: (" << width << "," << height << "), Kernel Radius: " << radius << ", Time Frequency: " << timeFrequency << "\n";

    auto application = LeniaApplication(width, height, radius, timeFrequency);

    application.run();

    return 0;
}
