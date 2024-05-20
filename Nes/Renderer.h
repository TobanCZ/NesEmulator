#pragma once
#include <cstdint>
namespace renderer
{
	class Pixel
	{
	public:
		Pixel(uint8_t r, uint8_t g, uint8_t b);
		Pixel(uint32_t hex);
		~Pixel();

		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint32_t hex;
	};

	class Sprite
	{
	public:
		Sprite();
		~Sprite();
	};

	class Renderer
	{
	public:
		Renderer();
		~Renderer();
	};
}

