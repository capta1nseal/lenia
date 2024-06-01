#ifndef _LENIAAPPLICATION_
#define _LENIAAPPLICATION_

#include "world.hpp"

class LeniaApplication
{
public:
    LeniaApplication();
    ~LeniaApplication();

    void run();

private:
    bool running;

    LeniaWorld world;

};

#endif
