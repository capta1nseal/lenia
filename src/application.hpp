#ifndef _LENIAAPPLICATION_
#define _LENIAAPPLICATION_

#include "world.hpp"
#include "sdlGraphicsInterface.hpp"

class LeniaApplication
{
public:
    LeniaApplication();
    ~LeniaApplication();

    void run();

private:
    bool running;

    LeniaWorld world;
    LeniaSdlGraphicsInterface graphicsInterface;
};

#endif
