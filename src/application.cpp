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
    
    world.printToTerminal();

    running = true;

    while (running)
    {
        world.progressState();

        world.printToTerminal();

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
