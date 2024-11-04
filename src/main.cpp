#include "application.hpp"

#include <string>
#include <exception>
#include <iostream>

int main (int argc, char *argv[])
{
    uint width, height

    if argc >= 2 {
        try {
            width = std::stoi(argv[0])
        } catch (std::exception const &e) {
            return -1
        }
        try {
            height = std::stoi(argv[1])
        } catch (std::exception const &e) {
            return -1
        }
    }

    std::cout << "Size: (" << width << "," << height << ")"

    auto application = LeniaApplication();

    application.run();

    return 0;
}
