#include "Renderer.h"
#include <GLFW/glfw3.h>

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

renderer::Renderer::Renderer()
{

}
