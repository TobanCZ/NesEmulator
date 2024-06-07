#include <iostream>
#include <sstream>
#include "Bus.h"
#include "Renderer.h"
#include "Cartrige.h"
#ifdef MAKEFILE
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <string>
#include <memory>
#include "Gui.h"
#define main SDL_main

void Update(Uint32 elapsed);
void Render(rndr::Renderer* rnd);
void clean();
void guiUpdate(std::shared_ptr<bool> isRunning);
void Event(SDL_Event* event);
void Reset();

std::unique_ptr<Bus> nes;
std::unique_ptr<Gui> gui;

std::shared_ptr<Cartrige> cartige;
float fResidualTime = 0.0f;

bool isKeyPressed(SDL_Keycode key) {
	const Uint8* state = SDL_GetKeyboardState(NULL);
	return state[SDL_GetScancodeFromKey(key)] != 0;
}

int main(int argc, char* argv[])
{
	nes = std::make_unique<Bus>();
	cartige = std::make_shared<Cartrige>("");

	if (!cartige->bImageValid)
		return 0;

	/*nes->insertCartrige(cartige);
	nes->reset();*/

	rndr::Renderer renderer("NES",1000,800, 256, 240, Update, Render, clean, guiUpdate);
	gui = std::make_unique<Gui>(renderer.window,renderer.renderer,1000,800, nes.get(), Event, Reset);
	
	renderer.Start();
	return 0;
}

void Reset()
{
	nes->insertCartrige(cartige);
	nes->reset();
}

Uint32 lastTime = 0;
const Uint32 frameTime = 1000 / 60;

void Update(Uint32 elapsed) //main loop
{
	nes->controller[0] = 0x00;
	nes->controller[0] |= isKeyPressed(SDLK_d) ? 0x80 : 0x00;
	nes->controller[0] |= isKeyPressed(SDLK_s) ? 0x40 : 0x00;
	nes->controller[0] |= isKeyPressed(SDLK_z) ? 0x10 : 0x00;
	nes->controller[0] |= isKeyPressed(SDLK_x) ? 0x20 : 0x00;
	nes->controller[0] |= isKeyPressed(SDLK_DOWN) ? 0x04 : 0x00;
	nes->controller[0] |= isKeyPressed(SDLK_UP) ? 0x08 : 0x00;
	nes->controller[0] |= isKeyPressed(SDLK_RIGHT) ? 0x01 : 0x00;
	nes->controller[0] |= isKeyPressed(SDLK_LEFT) ? 0x02 : 0x00;

	Uint32 currentTime = SDL_GetTicks();
	Uint32 deltaTime = currentTime - lastTime;

	if (deltaTime < frameTime) {
		SDL_Delay(frameTime - deltaTime);
		return;
	}

	if (nes->cartrige == nullptr)
		return;

	if (!gui->singleStep)
	{
		if (fResidualTime > 0.0f)
			fResidualTime -= elapsed;
		else
		{

			fResidualTime = (60.0 * 1000) + elapsed;
			do { 
				nes->clock(); 

				/*if (nes->cpu.pc == 0xDBB5)
				{
					gui->singleStep = true;
					break;
				}*/

			} while (!nes->ppu.frame_complete);
			nes->ppu.frame_complete = false;
		}
		
	}
	
}

void Event(SDL_Event* event)
{
	
	if (event->type == SDL_KEYDOWN)
	{
		if (gui->singleStep)
		{

			if (event->key.keysym.sym == SDLK_c)
			{
				while (nes->cpu.complete()) { nes->clock(gui.get()); }
				do { nes->clock(gui.get()); } while (!nes->cpu.complete());

			}
			if (event->key.keysym.sym == SDLK_v)
			{
				do { nes->clock(); } while (!nes->ppu.frame_complete);
				do { nes->clock(); } while (!nes->cpu.complete());
				nes->ppu.frame_complete = false;
			}
			if (event->key.keysym.sym == SDLK_b)
			{
				for (int i = 0; i < 100; i++)
				{
					do { nes->clock(gui.get()); } while (!nes->cpu.complete());

				}
			}
		}

		
	}

}

void Render(rndr::Renderer* renderer)
{
	renderer->Draw(*nes->ppu.backgroundCanvas,0,0);
}

void guiUpdate(std::shared_ptr<bool> isRunning)
{
	gui->Render(isRunning);
}

void clean()
{
	gui->Clean();
}

#undef main

int main(int argc, char** argv) {
	return SDL_main(argc, argv);
}