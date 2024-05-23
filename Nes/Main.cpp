#include <iostream>
#include <sstream>
#include "Bus.h"
#include "Renderer.h"
#include "Cartrige.h"
#include <SDL.h>
#include <string>
#include <memory>
#include "Gui.h"
#undef main


void Update();
void Render(rndr::Renderer* rnd);
void clean();
void guiUpdate(std::shared_ptr<bool> isRunning);
void Event(SDL_Event* event);

std::unique_ptr<Bus> nes;
std::unique_ptr<Gui> gui;

int main()
{
	nes = std::make_unique<Bus>();
	std::shared_ptr<Cartrige> cartige = std::make_shared<Cartrige>("C:/Users/tobia/Desktop/Klauzury/NesEmulator/Roms/Super Mario Bros.nes");

	if (!cartige->bImageValid)
		return 0;

	nes->insertCartrige(cartige);
	nes->reset();

	rndr::Renderer renderer("NES",1000,800, 256, 240, Update, Render, clean, guiUpdate);
	gui = std::make_unique<Gui>(renderer.window,renderer.renderer,1000,800, &nes->cpu, &nes->ppu,Event);
	
	renderer.Start();
	return 0;
}

void Update()
{
	if (!gui->singleStep)
	{
		do { nes->clock(); } while (!nes->ppu.frame_complete);
		nes->ppu.frame_complete = false;
	}
}

void Event(SDL_Event* event)
{
	if (gui->singleStep)
	{
		if (event->type == SDL_KEYDOWN)
		{
			if (event->key.keysym.sym == SDLK_c)
			{
				do { nes->clock(); } while (!nes->cpu.complete());
				do { nes->clock(); } while (nes->cpu.complete());
			}
			if (event->key.keysym.sym == SDLK_v)
			{
				do { nes->clock(); } while (!nes->ppu.frame_complete);
				do { nes->clock(); } while (!nes->cpu.complete());
				nes->ppu.frame_complete = false;
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