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
void guiUpdate();

std::unique_ptr<Bus> nes;
std::unique_ptr<Gui> gui;

int main()
{
	nes = std::make_unique<Bus>();
	std::shared_ptr<Cartrige> cartige = std::make_shared<Cartrige>("C:/Users/tobia/Desktop/Klauzury/NesEmulator/Roms/nestest.nes");

	if (!cartige->bImageValid)
		return 0;

	nes->insertCartrige(cartige);
	nes->reset();

	rndr::Renderer renderer("NES", Update, Render, clean, guiUpdate);
	gui = std::make_unique<Gui>(renderer.window,renderer.renderer);
	
	renderer.Start();
	return 0;
}

void Update()
{
	do { nes->clock(); } while (!nes->ppu.frame_complete);
	nes->ppu.frame_complete = false;
}

void Render(rndr::Renderer* renderer)
{
	renderer->Draw(*nes->ppu.backgroundCanvas,0,0);
}

void guiUpdate()
{
	gui->Render();
}

void clean()
{
	gui->Clean();
}