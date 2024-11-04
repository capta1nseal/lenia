#ifndef _LENIASDLGRAPHICSINTERFACE_
#define _LENIASDLGRAPHICSINTERFACE_

#include <array>

#include <SDL2/SDL.h>
#include "opencvWrapper.hpp"


/*
basic graphical user interface for the lenia program, 
drawing everything by just copying values in to an SDL2 texture in memory then drawing it on the window.
also handles user input and SDL events.
*/
class LeniaSdlGraphicsInterface
{
public:
    LeniaSdlGraphicsInterface();
    ~LeniaSdlGraphicsInterface();

    // internal method to initialize an SDL texture for drawing the world state to
    void initializeRenderTexture();

    // internal method to ensure the ouputted world is positioned correctly within the window, and should automatically run when necessary
    void initializeScaling();

    // returns true only if SDL should quit, therefore the program should stop
    bool handleEvents();

    // by default will not draw more frequently than current display's refresh rate
    void draw(const std::array<cv::Mat, 3>& worldState);

private:
    unsigned int displayWidth, displayHeight;
    unsigned int gridWidth, gridHeight;
    bool isFullscreen;

    SDL_DisplayMode displayMode;
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Texture* renderTexture;
    unsigned char* texturePixels;
    int texturePitch;

    SDL_FPoint topLeft;
    float cellSize;

    SDL_Event event;

    SDL_Point mousePosition;
};

#endif
