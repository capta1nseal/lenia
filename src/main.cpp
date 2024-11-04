#include "application.hpp"

#include <string>
#include <iostream>

int main (int argc, char *argv[])
{
    unsigned int width, height;
    int radius;
    float timeFrequency;

    bool useDefaultParams;

    bool validInputEntered = false;

    while (validInputEntered == false) {
        std::cout << "Use default paramaters, or enter manually? (1/2)\n";
        int option;
        std::cin >> option;

        validInputEntered = true;

        switch (option) {
        case 1:
            useDefaultParams = true;
            break;
        case 2:
            useDefaultParams = false;
            break;
        default:
            validInputEntered = false;
            std::cout << "Invalid input. Please try again.\n";
        }
    }

    if (useDefaultParams) {
        width = 16u*30u;
        height = 9u*30u;
        radius = 12;
        timeFrequency = 3.0;
    } else {
        validInputEntered = false;
        while (validInputEntered == false) {
            std::cout << "Enter width:\n";
            int option;
            std::cin >> option;

            validInputEntered = true;

            if (option > 0) {
                width = static_cast<unsigned int>(option);
            } else {
                validInputEntered = false;
                std::cout << "Width must be positive and non-zero. Try again.\n";
            }
        }
    }

    std::cout << "Size: (" << width << "," << height << "), Kernel Radius: " << radius << ", Time Frequency: " << timeFrequency << "\n";

    auto application = LeniaApplication(width, height, radius, timeFrequency);

    application.run();

    return 0;
}
