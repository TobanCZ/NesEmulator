#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include "Cartrige.h"
#include "Renderer.h"

class Ppu
{
public:
	Ppu();
	~Ppu();

public:
	std::array<uint8_t, 8192> pattern;
	std::array<uint8_t, 2048> name;
	std::array<uint8_t,32> pallet;
	rndr::Sprite* backgroundCanvas;
	bool nmi = false;
	bool frame_complete = false;

private:
	std::shared_ptr<Cartrige> cartrige;
	std::vector<rndr::Pixel> colorPallet;
	rndr::Sprite GetPatternTable(uint8_t index);
	rndr::Pixel GetColourFromPaletteRam(uint8_t palette, uint8_t pixel);

public:
	void CpuWrite(uint16_t address, uint8_t data);
	uint8_t CpuRead(uint16_t address, bool readOnly = false);

	void PpuWrite(uint16_t address, uint8_t data);
	uint8_t PpuRead(uint16_t address);

	void Reset();

public:
	void connectCartrige(const std::shared_ptr<Cartrige>& cartrige);
	void clock();

private:
	int16_t cycle = 0;
	int16_t scanline = 0;

	union
	{
		struct
		{
			uint8_t unused : 5;
			uint8_t sprite_overflow : 1;
			uint8_t sprite_zero_hit : 1;
			uint8_t vertical_blank : 1;
		};
		uint8_t reg;
	} status;

	union
	{
		struct
		{
			uint8_t grayscale : 1;
			uint8_t render_background_left : 1;
			uint8_t render_sprites_left : 1;
			uint8_t render_background : 1;
			uint8_t render_sprites : 1;
			uint8_t enchance_red : 1;
			uint8_t enchance_green : 1;
			uint8_t enchance_blue : 1;
		};
		uint8_t reg;
	} mask;

	union PPUCTRL
	{
		struct
		{
			uint8_t nametable_x : 1;
			uint8_t nametable_y : 1;
			uint8_t increment_mode : 1;
			uint8_t pattern_sprite : 1;
			uint8_t pattern_background : 1;
			uint8_t sprite_size : 1;
			uint8_t slave_mode : 1;
			uint8_t enable_nmi : 1;
		};
		uint8_t reg;
	} control;

	uint8_t adress_latch = 0x00;
	uint8_t ppu_data_buffer = 0x00;

	union loopy_register
	{
		struct
		{

			uint16_t coarse_x : 5;
			uint16_t coarse_y : 5;
			uint16_t nametable_x : 1;
			uint16_t nametable_y : 1;
			uint16_t fine_y : 3;
			uint16_t unused : 1;
		};

		uint16_t reg = 0x0000;
	};

	loopy_register vram_addr; 
	loopy_register tram_addr;

	uint8_t fine_x = 0x00;

	uint8_t bg_next_tile_id = 0x00;
	uint8_t bg_next_tile_attrib = 0x00;
	uint8_t bg_next_tile_lsb = 0x00;
	uint8_t bg_next_tile_msb = 0x00;
	uint16_t bg_shifter_pattern_lo = 0x0000;
	uint16_t bg_shifter_pattern_hi = 0x0000;
	uint16_t bg_shifter_attrib_lo = 0x0000;
	uint16_t bg_shifter_attrib_hi = 0x0000;

	void IncrementScrollX();
	void IncrementScrollY();
	void TransferAddressX();
	void TransferAddressY();
	void LoadBackgroundShifters();
	void UpdateShifters();
};

