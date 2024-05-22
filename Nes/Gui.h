#pragma once
#include <SDL.h>
class Gui
{
public:
	Gui(SDL_Window* window, SDL_Renderer* renderer);
	~Gui();

public:
	void Render();
	void Clean();
private:
	void(*renderCallback)();
	void Init();
	SDL_Window* window;
	SDL_Renderer* renderer;
};

