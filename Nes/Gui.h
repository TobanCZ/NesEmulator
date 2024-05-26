#pragma once
#include <SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <memory>
#include "Bus.h"
#include "Cpu.h"
#include "Ppu.h"
#include <wtypes.h>

class Gui
{
public:
	Gui(SDL_Window* window, SDL_Renderer* renderer, int width, int height, Bus* bus, void (*eventCallback)(SDL_Event* event), void (*resetCallback)());
	~Gui();

public:
	void Render(std::shared_ptr<bool> isRunning);
	void Clean();
	int width; 
	int height;
	bool singleStep = false;
	std::map<uint16_t, std::string> disassembler;
private:
	void (*eventCallback)(SDL_Event* event);
	void (*resetCallback)();
	void Init();
	SDL_Window* window;
	SDL_Renderer* renderer;
	ImGuiIO* io;
	Bus* bus = nullptr;
	std::array<SDL_Texture*, 32> palletTexture;
	SDL_Texture* patternTable0Texture;
	SDL_Texture* patternTable1Texture;
	void UpdatePalletTexture();
	void UpdatePatternTexture();
private:
	bool OpenFileDialog(wchar_t* filePath, DWORD filePathSize);
	void showCPU(bool* p_open);
	void showRAM(bool* p_open);
	void showAssembly(bool* p_open, int lines);
	void showPPU(bool* p_open);
	void showController(bool* p_open);
};

