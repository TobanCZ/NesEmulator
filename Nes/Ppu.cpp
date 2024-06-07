#include "Ppu.h"
#include <cstdint>
#include "Cartrige.h"
#include <memory>
#include "Renderer.h"


Ppu::Ppu()
{
    this->backgroundCanvas = new rndr::Sprite(256, 240);
    this->colorPallet = {
        rndr::Pixel(0xFF59595F), rndr::Pixel(0xFF00008F), rndr::Pixel(0xFF18018E), rndr::Pixel(0xFF3F0176) ,rndr::Pixel(0xFF550055), rndr::Pixel(0xFF550110), rndr::Pixel(0xFF550000), rndr::Pixel(0xFF442200), rndr::Pixel(0xFF323200), rndr::Pixel(0xFF103301), rndr::Pixel(0xFF013310), rndr::Pixel(0xFF004545), rndr::Pixel(0xFF014467), rndr::Pixel(0xFF000000), rndr::Pixel(0xFF000000), rndr::Pixel(0xFF000000),
        rndr::Pixel(0xFFAAAAAA), rndr::Pixel(0xFF0145DD), rndr::Pixel(0xFF5510EF), rndr::Pixel(0xFF7600EF) ,rndr::Pixel(0xFF9801BB), rndr::Pixel(0xFFAB0155), rndr::Pixel(0xFF983301), rndr::Pixel(0xFF894401), rndr::Pixel(0xFF676700), rndr::Pixel(0xFF326700), rndr::Pixel(0xFF016700), rndr::Pixel(0xFF016654), rndr::Pixel(0xFF015488), rndr::Pixel(0xFF080808), rndr::Pixel(0xFF000000), rndr::Pixel(0xFF000000),
        rndr::Pixel(0xFFEFEEEF), rndr::Pixel(0xFF4588FE), rndr::Pixel(0xFF7776FE), rndr::Pixel(0xFF9944FF) ,rndr::Pixel(0xFFBB44EE), rndr::Pixel(0xFFCD5598), rndr::Pixel(0xFFDD6644), rndr::Pixel(0xFFCC8901), rndr::Pixel(0xFFBAAA01), rndr::Pixel(0xFF77BA01), rndr::Pixel(0xFF22BB22), rndr::Pixel(0xFF23BB76), rndr::Pixel(0xFF22BBCC), rndr::Pixel(0xFF454544), rndr::Pixel(0xFF000000), rndr::Pixel(0xFF000000),
        rndr::Pixel(0xFFEFEEEF), rndr::Pixel(0xFF99CCFF), rndr::Pixel(0xFFAAABFE), rndr::Pixel(0xFFBA98FF) ,rndr::Pixel(0xFFDD98FE), rndr::Pixel(0xFFEF99DC), rndr::Pixel(0xFFEFABAA), rndr::Pixel(0xFFEEBA98), rndr::Pixel(0xFFEFDD89), rndr::Pixel(0xFFBBDC89), rndr::Pixel(0xFF99DC98), rndr::Pixel(0xFF99DDBB), rndr::Pixel(0xFF98DDEF), rndr::Pixel(0xFFAAAAAA), rndr::Pixel(0xFF000000), rndr::Pixel(0xFF000000),
    };
}

Ppu::~Ppu()
{
}

void Ppu::clock()
{
    if (scanline >= -1 && scanline < 240)
    {
        if (scanline == -1 && cycle == 1)
        {
            status.vertical_blank = 0;
            status.sprite_overflow = 0;
            status.sprite_zero_hit = 0;

            for (int i = 0; i < 8; i++)
            {
                spriteShifterLow[i] = 0;
                spriteShifterHigh[i] = 0;
            }
        }

        if (scanline == 0 && cycle == 0)
            cycle = 1;

        if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338))
        {
            UpdateShifters();

            switch ((cycle - 1) % 8)
            {
            case 0:
                LoadBackgroundShifters();
                bg_next_tile_id = PpuRead(0x2000 | (vram_addr.reg & 0x0FFF));
                break;
            case 2:
                bg_next_tile_attrib = PpuRead(0x23C0 | (vram_addr.nametable_y << 11)
                    | (vram_addr.nametable_x << 10)
                    | ((vram_addr.coarse_y >> 2) << 3)
                    | (vram_addr.coarse_x >> 2));
                if (vram_addr.coarse_y & 0x02) bg_next_tile_attrib >>= 4;
                if (vram_addr.coarse_x & 0x02) bg_next_tile_attrib >>= 2;
                bg_next_tile_attrib &= 0x03;
                break;
            case 4:
                bg_next_tile_lsb = PpuRead((control.pattern_background << 12)
                    + ((uint16_t)bg_next_tile_id << 4)
                    + (vram_addr.fine_y) + 0);
                break;
            case 6:
                bg_next_tile_msb = PpuRead((control.pattern_background << 12)
                    + ((uint16_t)bg_next_tile_id << 4)
                    + (vram_addr.fine_y) + 8);
                break;
            case 7:
                IncrementScrollX();
                break;
            }
        }

        if (cycle == 256)
            IncrementScrollY();

        if (cycle == 257)
        {
            LoadBackgroundShifters();
            TransferAddressX();
        }

        if (cycle == 338 || cycle == 340)
            bg_next_tile_id = PpuRead(0x2000 | (vram_addr.reg & 0x0FFF));
        


        if (scanline == -1 && cycle >= 280 && cycle < 305)
            TransferAddressY();
        



        if (cycle == 257 && scanline >= 0)
        {
            std::memset(spritesInScanline, 0xFF, 8 * sizeof(OAMstruct));
            spritesCount = 0;

            uint8_t nOAMEntry = 0;
            spriteZHPossible = false;
            while (nOAMEntry < 64 && spritesCount < 9)
            {
                int16_t diff = (int16_t)scanline - (int16_t)OAM[nOAMEntry].y;
                if (diff >= 0 && diff < (control.sprite_size ? 16 : 8))
                {
                    if (spritesCount < 8)
                    {
                        if (nOAMEntry == 0)
                            spriteZHPossible = true;

                        memcpy(&spritesInScanline[spritesCount], &OAM[nOAMEntry], sizeof(OAMstruct));
                        spritesCount++;
                    }
                }
                nOAMEntry++;
            }
            status.sprite_overflow = (spritesCount > 8);
        }


        if (cycle == 340)
        {
            for (uint8_t i = 0; i < spritesCount; i++)
            {
                uint8_t spritePatternBitsLow;
                uint8_t spritePatternBitsHigh;
                uint16_t spritePatternAddressLow;
                uint16_t spritePatternAddressHigh;

                if (!control.sprite_size) //8x8
                {
                    if (!(spritesInScanline[i].attribute & 0x80)) //flip
                        spritePatternAddressLow = (control.pattern_sprite << 12) | (spritesInScanline[i].id << 4) | (scanline - spritesInScanline[i].y);
                    else
                        spritePatternAddressLow = (control.pattern_sprite << 12) | (spritesInScanline[i].id << 4) | (7 - (scanline - spritesInScanline[i].y));
                    
                }
                else //16x8
                {
                    if (!(spritesInScanline[i].attribute & 0x80)) //flip
                    {
                        if (scanline - spritesInScanline[i].y < 8) //top
                            spritePatternAddressLow = ((spritesInScanline[i].id & 0x01) << 12) | ((spritesInScanline[i].id & 0xFE) << 4) | ((scanline - spritesInScanline[i].y) & 0x07);
                        else //bottom
                            spritePatternAddressLow = ((spritesInScanline[i].id & 0x01) << 12) | (((spritesInScanline[i].id & 0xFE) + 1) << 4) | ((scanline - spritesInScanline[i].y) & 0x07);
                    }
                    else
                    {
                        if (scanline - spritesInScanline[i].y < 8) //top
                            spritePatternAddressLow = ((spritesInScanline[i].id & 0x01) << 12) | (((spritesInScanline[i].id & 0xFE) + 1) << 4) | (7 - (scanline - spritesInScanline[i].y) & 0x07);
                        else //bottom
                            spritePatternAddressLow = ((spritesInScanline[i].id & 0x01) << 12) | ((spritesInScanline[i].id & 0xFE) << 4) | (7 - (scanline - spritesInScanline[i].y) & 0x07);
                    }
                }

                spritePatternAddressHigh = spritePatternAddressLow + 8;
                spritePatternBitsLow = PpuRead(spritePatternAddressLow);
                spritePatternBitsHigh = PpuRead(spritePatternAddressHigh);

                if (spritesInScanline[i].attribute & 0x40)
                {
                    auto flipbyte = [](uint8_t b)
                    {
                        b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
                        b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
                        b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
                        return b;
                    };

                    spritePatternBitsLow = flipbyte(spritePatternBitsLow);
                    spritePatternBitsHigh = flipbyte(spritePatternBitsHigh);
                }

                spriteShifterLow[i] = spritePatternBitsLow;
                spriteShifterHigh[i] = spritePatternBitsHigh;
            }
        }
    }


    if (scanline == 241 && cycle == 1)
    {
        status.vertical_blank = 1;
        if (control.enable_nmi)
            nmi = true;
    }

    uint8_t bg_pixel = 0x00;   
    uint8_t bg_palette = 0x00; 

    if (mask.render_background)
    {
        uint16_t bit_mux = 0x8000 >> fine_x;

        uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
        uint8_t p1_pixel = (bg_shifter_pattern_hi & bit_mux) > 0;

        bg_pixel = (p1_pixel << 1) | p0_pixel;

        uint8_t bg_pal0 = (bg_shifter_attrib_lo & bit_mux) > 0;
        uint8_t bg_pal1 = (bg_shifter_attrib_hi & bit_mux) > 0;
        bg_palette = (bg_pal1 << 1) | bg_pal0;
    }

    uint8_t fg_pixel = 0x00;
    uint8_t fg_palette = 0x00;
    uint8_t fg_priority = 0x00;

    if (mask.render_sprites)
    {
        spriteZHBeingRendered = false;
        for (uint8_t i = 0; i < spritesCount; i++)
        {
            if (spritesInScanline[i].x == 0)
            {
                uint8_t p0_pixel = (spriteShifterLow[i] & 0x80) > 0;
                uint8_t p1_pixel = (spriteShifterHigh[i] & 0x80) > 0;

                fg_pixel = (p1_pixel << 1) | p0_pixel;
                fg_palette = (spritesInScanline[i].attribute & 0x03) + 0x04;
                fg_priority = (spritesInScanline[i].attribute & 0x20) == 0; 

                if (fg_pixel != 0)
                {
                    if(i == 0)
                        spriteZHBeingRendered = true;
                    break;
                }
            }
        }
    }
    
    uint8_t pixel = 0x00;
    uint8_t palette = 0x00;

    if (bg_pixel == 0 && fg_pixel == 0)
    {
        pixel = 0x00;
        palette = 0x00;
    }
    else if (bg_pixel == 0 && fg_pixel > 0)
    {
        pixel = fg_pixel;
        palette = fg_palette;
    }
    else if (bg_pixel > 0 && fg_pixel == 0)
    {
        pixel = bg_pixel;
        palette = bg_palette;
    }
    else if (bg_pixel > 0 && fg_pixel > 0)
    {
        if (fg_priority)
        {
            pixel = fg_pixel;
            palette = fg_palette;
        }
        else
        {
            pixel = bg_pixel;
            palette = bg_palette;
        }

        if (spriteZHPossible && spriteZHBeingRendered)
        {
            if (mask.render_background && mask.render_sprites)
            {
                if (~(mask.render_background_left | mask.render_sprites_left))
                {
                    if (cycle >= 9 && cycle < 258)
                        status.sprite_zero_hit = 1;
                }
                else
                {
                    if (cycle >= 1 && cycle < 258)
                        status.sprite_zero_hit = 1;
                }
            }
        }
    }


    backgroundCanvas->SetPixel(cycle - 1, scanline, GetColourFromPaletteRam(palette,pixel));

    cycle++;
    if (cycle >= 341)
    {
        cycle = 0;
        scanline++;
        if (scanline >= 261)
        {
            scanline = -1;
            frame_complete = true;
        }
    }
}

void Ppu::Reset()
{
    fine_x = 0x00;
    adress_latch = 0x00;
    ppu_data_buffer = 0x00;
    scanline = 0;
    cycle = 0;
    bg_next_tile_id = 0x00;
    bg_next_tile_attrib = 0x00;
    bg_next_tile_lsb = 0x00;
    bg_next_tile_msb = 0x00;
    bg_shifter_pattern_lo = 0x0000;
    bg_shifter_pattern_hi = 0x0000;
    bg_shifter_attrib_lo = 0x0000;
    bg_shifter_attrib_hi = 0x0000;
    status.reg = 0x00;
    mask.reg = 0x00;
    control.reg = 0x00;
    vram_addr.reg = 0x0000;
    tram_addr.reg = 0x0000;
}


void Ppu::IncrementScrollX()
{
    if (mask.render_background || mask.render_sprites)
    {
        if (vram_addr.coarse_x == 31)
        {
            vram_addr.coarse_x = 0;
            vram_addr.nametable_x = ~vram_addr.nametable_x;
        }
        else
        {
            vram_addr.coarse_x++;
        }
    }
}

void Ppu::IncrementScrollY()
{
    if (mask.render_background || mask.render_sprites)
    {
        if (vram_addr.fine_y < 7)
        {
            vram_addr.fine_y++;
        }
        else
        {
            vram_addr.fine_y = 0;

            if (vram_addr.coarse_y == 29)
            {
                vram_addr.coarse_y = 0;
                vram_addr.nametable_y = ~vram_addr.nametable_y;
            }
            else if (vram_addr.coarse_y == 31)
            {
                vram_addr.coarse_y = 0;
            }
            else
            {
                vram_addr.coarse_y++;
            }
        }
    }
}

void Ppu::TransferAddressX()
{
    if (mask.render_background || mask.render_sprites)
    {
        vram_addr.nametable_x = tram_addr.nametable_x;
        vram_addr.coarse_x = tram_addr.coarse_x;
    }
}

void Ppu::TransferAddressY()
{
    if (mask.render_background || mask.render_sprites)
    {
        vram_addr.fine_y = tram_addr.fine_y;
        vram_addr.nametable_y = tram_addr.nametable_y;
        vram_addr.coarse_y = tram_addr.coarse_y;
    }
}

void Ppu::LoadBackgroundShifters()
{
    bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
    bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;
    bg_shifter_attrib_lo = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
    bg_shifter_attrib_hi = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);

}

void Ppu::UpdateShifters()
{
    if (mask.render_background)
    {
        bg_shifter_pattern_lo <<= 1;
        bg_shifter_pattern_hi <<= 1;
        bg_shifter_attrib_lo <<= 1;
        bg_shifter_attrib_hi <<= 1;
    }

    if (mask.render_sprites && cycle >= 1 && cycle < 258)
    {
        for (uint8_t i = 0; i < spritesCount; i++)
        {
            if (spritesInScanline[i].x > 0)
            {
                spritesInScanline[i].x--;
            }
            else
            {
                spriteShifterLow[i] <<= 1;
                spriteShifterHigh[i] <<= 1;
            }
        }
    }
}

rndr::Sprite Ppu::GetPatternTable(uint8_t index)
{
    rndr::Sprite table(128,128);
    for (uint16_t yTile = 0; yTile < 16; yTile++)
    {
        for (uint16_t xTile = 0; xTile < 16; xTile++)
        {
            uint16_t offset = yTile * 256 + xTile * 16;

            for (uint16_t yPixel = 0; yPixel < 8; yPixel++)
            {
                uint8_t tile_lsb = PpuRead(index * 0x1000 + offset + yPixel);
                uint8_t tile_msb = PpuRead(index * 0x1000 + offset + yPixel + 8);

                for (uint16_t xPixel = 0; xPixel < 8; xPixel++)
                {
                    uint8_t pixel = ((tile_lsb & 0x01) << 1) | (tile_msb & 0x01);
                    tile_lsb >>= 1; 
                    tile_msb >>= 1;
                    table.SetPixel(xTile * 8 + (7 - xPixel), yTile * 8 + yPixel, GetColourFromPaletteRam(0,pixel));
                }
            }
        }
    }
    return table;
}

rndr::Pixel Ppu::GetColourFromPaletteRam(uint8_t palette, uint8_t pixel)
{
    if (pixel == 0) palette = 0;
    return colorPallet[PpuRead(0x3F00 + (palette << 2) +pixel) & 0x3F];
}

void Ppu::CpuWrite(uint16_t address, uint8_t data)
{
    switch (address)
    {
    case 0x0000:
        control.reg = data;
        tram_addr.nametable_x = control.nametable_x;
        tram_addr.nametable_y = control.nametable_y;
        break;
    case 0x0001:
        mask.reg = data;
        break;
    case 0x0002:
        break;
    case 0x0003:
        adress_OAM = data;
        break;
    case 0x0004:
        pOAM[adress_OAM] = data;
        break;
    case 0x0005:
        if (adress_latch == 0)
        {
            fine_x = data & 0x07;
            tram_addr.coarse_x = data >> 3;
            adress_latch = 1;
        }
        else
        {
            tram_addr.fine_y = data & 0x07;
            tram_addr.coarse_y = data >> 3;
            adress_latch = 0;
        }
        break;
    case 0x0006:
        if (adress_latch == 0)
        {
            tram_addr.reg = ((data & 0x3F) << 8) | (tram_addr.reg & 0x00FF);
            adress_latch = 1;
        }
        else
        {
            tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
            vram_addr = tram_addr;
            adress_latch = 0;
        }
        break;
    case 0x0007:
        PpuWrite(vram_addr.reg,data);
        vram_addr.reg += (control.increment_mode ? 32: 1);
        break;
    }
}

uint8_t Ppu::CpuRead(uint16_t address, bool readOnly)
{
    uint8_t data = 0x00;
    if (readOnly)
    {
        switch (address)
        {
        case 0x0000: 
            data = control.reg;
            break;
        case 0x0001: 
            data = mask.reg;
            break;
        case 0x0002: 
            data = status.reg;
            break;
        case 0x0003: 
            break;
        case 0x0004:
            break;
        case 0x0005: 
            break;
        case 0x0006: 
            break;
        case 0x0007: 
            break;
        }
    }
    else
    {
        switch (address)
        {
        case 0x0000:
            break;
        case 0x0001:
            break;
        case 0x0002:
            data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
            status.vertical_blank = 0;
            adress_latch = 0;
            break;
        case 0x0003:
            break;
        case 0x0004:
            data = pOAM[adress_OAM];
            break;
        case 0x0005:
            break;
        case 0x0006:
            break;
        case 0x0007:
            data = ppu_data_buffer;
            ppu_data_buffer = PpuRead(vram_addr.reg);

            if (vram_addr.reg >= 0x3F00)
                data = ppu_data_buffer;
            vram_addr.reg += (control.increment_mode ? 32 : 1);
            break;
        }
    }
    return data;
}

void Ppu::PpuWrite(uint16_t address, uint8_t data)
{
    address &= 0x3FFF;

    if (cartrige->PpuWrite(address, data))
    {}
    else if (address >= 0x0000 && address <= 0x1FFF)
    {
        pattern[(((address & 0x1000) >> 12) * 4048) + address & 0x0FFF] = data;
    }
    else if (address >= 0x2000 && address <= 0x3EFF)
    {
        address &= 0x0FFF;
        if (cartrige->Mirror() == MIRROR::VERTICAL)
        {
            if (address >= 0x0000 && address <= 0x03FF)
                name[address & 0x03FF] = data;
            if (address >= 0x0400 && address <= 0x07FF)
                name[1024 + (address & 0x03FF)] = data;
            if (address >= 0x0800 && address <= 0x0BFF)
                name[address & 0x03FF] = data;
            if (address >= 0x0C00 && address <= 0x0FFF)
                name[1024 + (address & 0x03FF)] = data;
        }
        else if (cartrige->Mirror() == MIRROR::HORIZONTAL)
        {
            if (address >= 0x0000 && address <= 0x03FF)
                name[address & 0x03FF] = data;
            if (address >= 0x0400 && address <= 0x07FF)
                name[address & 0x03FF] = data;
            if (address >= 0x0800 && address <= 0x0BFF)
                name[1024 + (address & 0x03FF)] = data;
            if (address >= 0x0C00 && address <= 0x0FFF)
                name[1024 + (address & 0x03FF)] = data;
        }
    }
    else if (address >= 0x3F00 && address <= 0x3FFF)
    {
        address &= 0x001F;
        if (address == 0x0010) address = 0x0000;
        if (address == 0x0014) address = 0x0004;
        if (address == 0x0018) address = 0x0008;
        if (address == 0x001C) address = 0x000C;
        pallet[address] = data;
    }
   
}

uint8_t Ppu::PpuRead(uint16_t address)
{
    uint8_t data = 0x00;
    address &= 0x3FFF;

    if (cartrige->PpuRead(address,data))
    {}
    else if (address >= 0x0000 && address <= 0x1FFF)
    {
        data = pattern[(((address & 0x1000) >> 12) * 4048) + address & 0x0FFF];
    }
    else if (address >= 0x2000 && address <= 0x3EFF)
    {
        address &= 0x0FFF;
        if (cartrige->Mirror() == MIRROR::VERTICAL)
        {
            if (address >= 0x0000 && address <= 0x03FF)
                data = name[address & 0x03FF];
            if (address >= 0x0400 && address <= 0x07FF)
                data = name[1024 + (address & 0x03FF)];
            if (address >= 0x0800 && address <= 0x0BFF)
                data = name[address & 0x03FF];
            if (address >= 0x0C00 && address <= 0x0FFF)
                data = name[1024 + (address & 0x03FF)];
        }
        else if (cartrige->Mirror() == MIRROR::HORIZONTAL)
        {
            if (address >= 0x0000 && address <= 0x03FF)
                data = name[address & 0x03FF];
            if (address >= 0x0400 && address <= 0x07FF)
                data = name[address & 0x03FF];
            if (address >= 0x0800 && address <= 0x0BFF)
                data = name[1024 + (address & 0x03FF)];
            if (address >= 0x0C00 && address <= 0x0FFF)
                data = name[1024 + (address & 0x03FF)];
        }
    }
    else if (address >= 0x3F00 && address <= 0x3FFF)
    {
        address &= 0x001F;
        if (address == 0x0010) address = 0x0000;
        if (address == 0x0014) address = 0x0004;
        if (address == 0x0018) address = 0x0008;
        if (address == 0x001C) address = 0x000C;
        data = pallet[address] & (mask.grayscale ? 0x30 : 0x3F);
    }

    return data;
}

void Ppu::connectCartrige(const std::shared_ptr<Cartrige>& cartrige)
{
    this->cartrige = cartrige;
}


