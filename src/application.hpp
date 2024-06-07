#ifndef _LENIAAPPLICATION_
#define _LENIAAPPLICATION_

#include "world.hpp"
#include "sdlGraphicsInterface.hpp"

/*
wrapper class for the whole application, 
doing nothing but initializing the other components and calling their methods in a loop
*/
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
