#include "application.hpp"

#include "world.hpp"

#include <chrono>
#include <thread>

LeniaApplication::LeniaApplication()
{
}
LeniaApplication::~LeniaApplication()
{
}

void LeniaApplication::run()
{
    std::cin.tie(0);
    
    graphicsInterface.draw(world.state());

    running = true;

    while (running)
    {
        if (graphicsInterface.handleEvents()) running = false;

        world.progressState();

        graphicsInterface.draw(world.state());
    }
}
