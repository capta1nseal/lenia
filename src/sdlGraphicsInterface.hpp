#ifndef _LENIASDLGRAPHICSINTERFACE_
#define _LENIASDLGRAPHICSINTERFACE_

#include <SDL2/SDL.h>
#include <opencv2/opencv.hpp>

class LeniaSdlGraphicsInterface
{
public:
    LeniaSdlGraphicsInterface();
    ~LeniaSdlGraphicsInterface();

    void initializeRenderTexture();

    void initializeScaling();
    // returns true only if SDL should quit, therefore the program should stop
    bool handleEvents();
    // by default will not draw more frequently than current display's refresh rate
    void draw(const cv::Mat& worldState);

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
    double cellSize;

    SDL_Event event;

    SDL_Point mousePosition;

};

#endif
