#pragma once
#include "Renderer.h"
#include <SDL.h>
#include <stdint.h>
#include <cstdint>
#include <functional>
#include <vector>
#include <string>
#include <memory>

namespace rndr
{
	class Pixel
	{
	public:
		Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		Pixel(uint32_t hex);
		~Pixel();

		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
		uint32_t hex;
	};

	class Sprite
	{
	public:
		Sprite(uint16_t width, uint16_t height, std::vector<Pixel> pixels);
		Sprite(uint16_t width, uint16_t height);
		~Sprite();
	public:
		uint16_t width;
		uint16_t height;
		std::vector<Pixel> pixels;
		void SetPixel(int index, Pixel pixel);
		void SetPixel(int x, int y, Pixel pixel);
	};

	class Renderer
	{
	public:
		Renderer(std::string title, int winWidth,int winHeight, int nesWidth, int nesHeight ,void (*updateCallback)(), void (*renderCallback)(rndr::Renderer* renderer), void (*cleanCallback)(), void (*guiCallback)(std::shared_ptr<bool> isRunning));
		~Renderer();
		

	public:
		std::string title;
		int winWidth;
		int winHeight;
		int nesWidth;
		int nesHeight;
		void Start();
		void Draw(const Sprite& sprite, int posX, int posY);
		std::shared_ptr<bool> isRunning;
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Renderer* GUIrenderer;

	private:
		SDL_Texture* canvas;
		void* canvas_pixels = NULL;
		void Init();
		void Clean();
		void Update();
		void Render();
		void updateWindowTitleWithFPS(SDL_Window* window, Uint32 frameCount, Uint32 startTime);
		void (*updateCallback)();
		void (*renderCallback)(rndr::Renderer* renderer);
		void (*guiCallback)(std::shared_ptr<bool> isRunning);
		void (*cleanCallback)();
	};
}

