#include "Renderer.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <SDL.h>

#define WIN_WIDTH 256 
#define WIN_HEIGHT 240 

renderer::Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b)
{
	this->r = r;
	this->g = g;
	this->b = b;

	this->hex = (r << 16) | (g << 8) | b;
}

renderer::Pixel::Pixel(uint32_t hex)
{
	this->r = (hex >> 16) & 0x00FF;
	this->g = (hex >> 8) & 0x00FF;
	this->b = hex & 0x00FF;

	this->hex = hex & 0x00FFFFFF;
}


renderer::Sprite::Sprite()
{

}

renderer::Sprite::~Sprite()
{
}

renderer::Renderer::Renderer()
{

}

renderer::Renderer::~Renderer()
{
}

void renderer::Renderer::Start()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
   
    }

    // create SDL window
    SDL_Window* window = SDL_CreateWindow("NES",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIN_WIDTH * 4,
        WIN_HEIGHT * 4,
        SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        SDL_Log("Unable to create window: %s", SDL_GetError());

    }

    // create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());

    }

    SDL_RenderSetLogicalSize(renderer, 10, 10);

    // create texture
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        WIN_WIDTH,
        WIN_HEIGHT);
    if (texture == NULL) {
        SDL_Log("Unable to create texture: %s", SDL_GetError());
      
    }

    // array of pixels
    uint8_t pixels[WIN_WIDTH * WIN_HEIGHT * 4] = { 0 };

    for (int i = 0; i < WIN_WIDTH; i++)
    {
        pixels[i*4] = 255;
        pixels[WIN_WIDTH * (WIN_HEIGHT-1) * 4 + i *4] = 255;
    }

    for (int i = 0; i < WIN_HEIGHT; i++)
    {
        pixels[WIN_WIDTH * 4 * i] = 255;
        pixels[WIN_WIDTH * 4 * i + WIN_WIDTH * 4 - 4] = 255;
    }
    


    // update texture with new data
    int texture_pitch = 0;
    void* texture_pixels = NULL;
    if (SDL_LockTexture(texture, NULL, &texture_pixels, &texture_pitch) != 0) {
        SDL_Log("Unable to lock texture: %s", SDL_GetError());
    }
    else {
        memcpy(texture_pixels, pixels, texture_pitch * WIN_HEIGHT);
    }
    SDL_UnlockTexture(texture);


    bool should_quit = false;
    SDL_Event e;
    while (!should_quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                should_quit = true;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

