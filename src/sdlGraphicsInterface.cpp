#include "sdlGraphicsInterface.hpp"

#include <SDL2/SDL.h>
#include <opencv2/opencv.hpp>

LeniaSdlGraphicsInterface::LeniaSdlGraphicsInterface()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GetCurrentDisplayMode(0, &displayMode);

    displayWidth = displayMode.w / 2;
    displayHeight = displayMode.h / 2;
    gridWidth = 16;
    gridHeight = 9;

    isFullscreen = false;

    uint32_t windowFlags = SDL_WINDOW_RESIZABLE;
    window = SDL_CreateWindow(
        "mandelbrot",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        displayWidth, displayHeight,
        windowFlags);
    
    uint32_t renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    renderer = SDL_CreateRenderer(window, -1, renderFlags);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");

    initializeRenderTexture();
}

LeniaSdlGraphicsInterface::~LeniaSdlGraphicsInterface()
{
    SDL_DestroyTexture(renderTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void LeniaSdlGraphicsInterface::initializeRenderTexture()
{
    renderTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, gridWidth, gridHeight);

    SDL_SetTextureBlendMode(renderTexture, SDL_BLENDMODE_BLEND);

    initializeScaling();
}

void LeniaSdlGraphicsInterface::initializeScaling()
{
    if (displayWidth / gridWidth < displayHeight / gridHeight)
    {
        cellSize = static_cast<double>(displayWidth) / static_cast<double>(gridWidth);
        topLeft.x = 0.0;
        topLeft.y = (displayHeight - gridHeight * cellSize) / 2.0;
    }
    else
    {
        cellSize = static_cast<double>(displayHeight) / static_cast<double>(gridHeight);
        topLeft.x = (displayWidth - gridWidth * cellSize) / 2.0;
        topLeft.y = 0.0;
    }
}

bool LeniaSdlGraphicsInterface::handleEvents()
{
    SDL_PumpEvents();

    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return true;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                displayWidth = event.window.data1;
                displayHeight = event.window.data2;

                initializeScaling();
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                return true;
            case SDL_SCANCODE_F11:
                if (isFullscreen)
                {
                    SDL_SetWindowFullscreen(window, 0);
                }
                else
                {
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
                isFullscreen = !isFullscreen;
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                // handle left click event
            }
            else if (event.button.button == SDL_BUTTON_RIGHT)
            {
                // handle right click event
            }
        default:
            break;
        }
    }

    return false;
}

void LeniaSdlGraphicsInterface::draw(const std::array<cv::Mat, 3>& worldState)
{
    if (worldState[0].rows != gridWidth or worldState[0].cols != gridHeight)
    {
        gridWidth = worldState[0].rows;
        gridHeight = worldState[0].cols;

        initializeRenderTexture();
    }

    for (auto channel : worldState)
    {
        if (channel.dims != 2) throw std::invalid_argument("World must be two-dimensional.");

        if (channel.rows != gridWidth or channel.cols != gridHeight) throw std::invalid_argument("Channels must be the same size");
    }
    
    SDL_SetRenderDrawColor(renderer, 0, 71, 171, 255);
    SDL_RenderClear(renderer);

    SDL_LockTexture(renderTexture, NULL, reinterpret_cast<void**>(&texturePixels), &texturePitch);

    worldState[0].forEach<double>(
        [&texturePixels = texturePixels, &texturePitch = texturePitch, &greenChannel = worldState[1], &blueChannel = worldState[2]]
        (double& value, const int* position)
        {
            int x = position[0];
            int y = position[1];

            int red   = std::clamp(static_cast<int>(floor<int>(256.0 * value)), 0, 255);
            int green = std::clamp(static_cast<int>(floor<int>(256.0 * greenChannel.at<double>(x, y))), 0, 255);
            int blue  = std::clamp(static_cast<int>(floor<int>(256.0 * blueChannel.at<double>(x, y))), 0, 255);

            texturePixels[y * texturePitch + x * 4    ] = static_cast<unsigned char>(blue);
            texturePixels[y * texturePitch + x * 4 + 1] = static_cast<unsigned char>(green);
            texturePixels[y * texturePitch + x * 4 + 2] = static_cast<unsigned char>(red);
            texturePixels[y * texturePitch + x * 4 + 3] = static_cast<unsigned char>(255);
        }
    );

    //for (unsigned int x = 0; x < gridWidth; x++)
    //{
    //    for (unsigned int y = 0; y < gridHeight; y++)
    //    {
    //        int red   = std::clamp(static_cast<int>(floor<int>(256.0 * worldState[0].at<double>(x, y))), 0, 255);
    //        int green = std::clamp(static_cast<int>(floor<int>(256.0 * worldState[1].at<double>(x, y))), 0, 255);
    //        int blue  = std::clamp(static_cast<int>(floor<int>(256.0 * worldState[2].at<double>(x, y))), 0, 255);
    //
    //        texturePixels[y * texturePitch + x * 4    ] = static_cast<unsigned char>(blue);
    //        texturePixels[y * texturePitch + x * 4 + 1] = static_cast<unsigned char>(green);
    //        texturePixels[y * texturePitch + x * 4 + 2] = static_cast<unsigned char>(red);
    //        texturePixels[y * texturePitch + x * 4 + 3] = static_cast<unsigned char>(255);
    //    }
    //}

    SDL_UnlockTexture(renderTexture);

    SDL_FRect destination;
    destination.x = topLeft.x;
    destination.y = topLeft.y;
    destination.w = gridWidth * cellSize;
    destination.h = gridHeight * cellSize;

    SDL_FPoint center;
    center.x = displayWidth / 2.0;
    center.y = displayHeight / 2.0;

    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopyExF(renderer, renderTexture, NULL, &destination, 0, &center, SDL_FLIP_NONE);

    SDL_RenderPresent(renderer);
}
