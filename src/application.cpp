#include "application.hpp"

#include "world.hpp"

#include <chrono>
#include <thread>

LeniaApplication::LeniaApplication(unsigned int width, unsigned int height, int radius, float frequency)
    : world(width, height, radius, frequency)
{
}
LeniaApplication::~LeniaApplication()
{
}

void LeniaApplication::run()
{
    std::chrono::time_point start = std::chrono::steady_clock::now();
    int frameCount = 0;

    std::cin.tie(0);
    
    graphicsInterface.draw(world.state());

    running = true;

    while (running)
    {
        if (graphicsInterface.handleEvents()) running = false;

        world.progressState();

        graphicsInterface.draw(world.state());
        
        frameCount++;
    }

    std::cout << "Average frame time was " << (std::chrono::steady_clock::now() - start).count() / static_cast<float>(frameCount) * 1e-6 << " ms\n";
}
