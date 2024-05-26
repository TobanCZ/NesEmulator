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
void Reset();

std::unique_ptr<Bus> nes;
std::unique_ptr<Gui> gui;

std::shared_ptr<Cartrige> cartige;
int main()
{
	nes = std::make_unique<Bus>();
	cartige = std::make_shared<Cartrige>("C:/Users/tobia/Desktop/Klauzury/NesEmulator/Roms/Tests/nestest.nes");

	if (!cartige->bImageValid)
		return 0;

	nes->insertCartrige(cartige);
	nes->reset();

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

void Update()
{
	if (!gui->singleStep)
	{
		do { 
			if (nes->cpu.pc == 0xFFFF)
			{
				gui->singleStep = true;
				break;
			}
			else
			{
				nes->clock();
			}
		} while (!nes->ppu.frame_complete);
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
				while (nes->cpu.complete()){ nes->clock(); }
				do { nes->clock(); } while (!nes->cpu.complete());
				
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
					do { nes->clock(); } while (!nes->cpu.complete());
					
				}
			}

			nes->controller[0] |= event->key.keysym.sym == SDLK_DOWN ? 0x04 : 0x00;
			nes->controller[0] |= event->key.keysym.sym == SDLK_UP ? 0x08 : 0x00;
			nes->controller[0] |= event->key.keysym.sym == SDLK_RIGHT ? 0x01 : 0x00;
			nes->controller[0] |= event->key.keysym.sym == SDLK_LEFT ? 0x02 : 0x00;
			nes->controller[0] |= event->key.keysym.sym == SDLK_s ? 0x20 : 0x00;
			nes->controller[0] |= event->key.keysym.sym == SDLK_d ? 0x10 : 0x00;
			nes->controller[0] |= event->key.keysym.sym == SDLK_KP_ENTER ? 0x40 : 0x00;
			nes->controller[0] |= event->key.keysym.sym == SDLK_RSHIFT ? 0x80 : 0x00;
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