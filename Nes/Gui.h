#pragma once
#include <SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <memory>

class Gui
{
public:
	Gui(SDL_Window* window, SDL_Renderer* renderer, int width, int height);
	~Gui();

public:
	void Render(std::shared_ptr<bool> isRunning);
	void Clean();
	int width; 
	int height;
private:
	void(*renderCallback)();
	void Init();
	SDL_Window* window;
	SDL_Renderer* renderer;
	ImGuiIO* io;
private:
	void showCPU(bool* p_open);
};

