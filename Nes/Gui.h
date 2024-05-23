#pragma once
#include <SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <memory>
#include "Cpu.h"
#include "Ppu.h"

class Gui
{
public:
	Gui(SDL_Window* window, SDL_Renderer* renderer, int width, int height, Cpu* cpu, Ppu* ppu, void (*eventCallback)(SDL_Event* event));
	~Gui();

public:
	void Render(std::shared_ptr<bool> isRunning);
	void Clean();
	int width; 
	int height;
	bool singleStep = false;
	std::map<uint16_t, std::string> disassemlbed;
private:
	void (*eventCallback)(SDL_Event* event);
	void Init();
	SDL_Window* window;
	SDL_Renderer* renderer;
	ImGuiIO* io;
	Cpu* cpu;
	Ppu* ppu;
private:
	void showCPU(bool* p_open);
	void showRAM(bool* p_open);
	void showAssembly(bool* p_open, int lines);
	void showPPU(bool* p_open);
};

