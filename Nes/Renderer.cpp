#include "Renderer.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#ifdef MAKEFILE
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"

rndr::Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	this->r = r;
	this->g = g;
	this->b = b;
    this->a = a;

	this->hex = (a << 24) | (r << 16) | (g << 8) | b;
}

rndr::Pixel::Pixel(uint32_t hex)
{
    this->a = (hex >> 24) & 0x00FF;
	this->r = (hex >> 16) & 0x00FF;
	this->g = (hex >> 8) & 0x00FF;
	this->b = hex & 0x00FF;

	this->hex = hex;
}

rndr::Pixel::~Pixel()
{
}


rndr::Sprite::Sprite(uint16_t width, uint16_t height, std::vector<Pixel> pixels)
{
    this->width = width;
    this->height = height;
    this->pixels = pixels;
    if(this->pixels.size() != width*height)
        this->pixels.resize(width * height, Pixel(0, 0, 0, 1));
}

rndr::Sprite::Sprite(uint16_t width, uint16_t height)
{
    this->width = width;
    this->height = height;
    this->pixels = std::vector<Pixel>(width*height, Pixel(0,0,0,1));
}

rndr::Sprite::~Sprite()
{
}

void rndr::Sprite::SetPixel(int index, Pixel pixel)
{
    pixels[index] = pixel;
}

void rndr::Sprite::SetPixel(int x, int y, Pixel pixel)
{
    if(x < width && y < height && x >= 0 && y >= 0)
        pixels[y * width + x] = pixel;
}


rndr::Renderer::Renderer(std::string title, int winWidth, int winHeight, int nesWidth, int nesHeight, void (*updateCallback)(Uint32 elapsed), void (*renderCallback)(rndr::Renderer* renderer), void (*cleanCallback)(),  void (*guiCallback)(std::shared_ptr<bool> isRunning))
{
    this->title = title;
    this->winWidth = winWidth;
    this->winHeight = winHeight;
    this->nesWidth = nesWidth;
    this->nesHeight = nesHeight;
    this->updateCallback = updateCallback;
    this->renderCallback = renderCallback;
    this->guiCallback = guiCallback;
    this->cleanCallback = cleanCallback;
    isRunning = std::make_shared<bool>(false);
    Init();
}


rndr::Renderer::~Renderer()
{
}


void rndr::Renderer::Start()
{
    while (*isRunning)
    {
        Update();
        Render(); 
    }
    Clean();
}

void rndr::Renderer::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
   
    // create SDL window
    window = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,winWidth,winHeight,SDL_WINDOW_RESIZABLE);
    if (window == NULL) 
        SDL_Log("Unable to create window: %s", SDL_GetError());
    
    // create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) 
        SDL_Log("Unable to create renderer: %s", SDL_GetError());

    SDL_RenderSetLogicalSize(renderer, winWidth, winHeight);
    
    canvas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, nesWidth, nesHeight);
    if (canvas == NULL)
    {
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        return;
    }

 /*   GUIrenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (GUIrenderer == NULL)
        SDL_Log("Unable to create GUIrenderer: %s", SDL_GetError());*/

    *isRunning = true;
}

void rndr::Renderer::Clean()
{
    //SDL_DestroyTexture(texture);
    cleanCallback();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

uint32_t startTime = SDL_GetTicks();
uint32_t frameCount = 0;

void rndr::Renderer::Update()
{

    if (updateCallback)
    {
        updateCallback(SDL_GetTicks());
    }

    frameCount++;
    if (SDL_GetTicks() - startTime >= 1000)
    {
        updateWindowTitleWithFPS(window, frameCount, startTime);
        frameCount = 0;
        startTime = SDL_GetTicks();
    }
}

void rndr::Renderer::Render()
{
    SDL_RenderClear(renderer);
   
    int texture_pitch = nesWidth * 4;
    if (SDL_LockTexture(canvas, NULL, &canvas_pixels, &texture_pitch) != 0)
    {
        SDL_Log("Unable to lock texture: %s", SDL_GetError());
        return;
    }

    renderCallback(this);
    SDL_UnlockTexture(canvas);
    SDL_Rect destRect = { 0, 0,  winWidth, winHeight};
    SDL_RenderCopy(renderer, canvas, NULL, &destRect);
    guiCallback(isRunning);
    SDL_RenderPresent(renderer);
}


void rndr::Renderer::updateWindowTitleWithFPS(SDL_Window* window, Uint32 frameCount, Uint32 startTime)
{
    Uint32 currentTime = SDL_GetTicks();
    float elapsedSeconds = (currentTime - startTime) / 1000.0f;
    int fps = frameCount / elapsedSeconds;

    std::string title = this->title + " | FPS: " + std::to_string(fps);
    SDL_SetWindowTitle(window, title.c_str());
}


void rndr::Renderer::Draw(const Sprite& sprite,int posX, int posY)
{
     uint8_t* dst = static_cast<uint8_t*>(canvas_pixels);
     for (int y = 0; y < sprite.height; y++)
     {
         for (int x = 0; x < sprite.width; x++)
         {
             const Pixel& pixel = sprite.pixels[y * sprite.width + x];
             dst[(y + posY) * 4 * nesWidth + (x + posX) * 4 + 0] = pixel.r;
             dst[(y + posY) * 4 * nesWidth + (x + posX) * 4 + 1] = pixel.g;
             dst[(y + posY) * 4 * nesWidth + (x + posX) * 4 + 2] = pixel.b;
             dst[(y + posY) * 4 * nesWidth + (x + posX) * 4 + 3] = pixel.a;
         }
     }
}

