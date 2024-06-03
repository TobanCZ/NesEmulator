#include "Gui.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <memory>
#include "Bus.h"
#include "Cpu.h"
#include "Ppu.h"
#include <string>
#include <iostream>
#include <cstring> 
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <commdlg.h>
#include <locale>
#include <codecvt>

char* hex(uint32_t n, uint8_t d);
char* combineChar(char* first, const char* second);

void DrawColoredSquare(ImDrawList* draw_list, const ImVec2& pos, const ImVec2& size, ImU32 color) {
    draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), color);
}

std::string WCharToString(const wchar_t* wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(wstr);
}


Gui::Gui(SDL_Window* window, SDL_Renderer* renderer, int width, int height, Bus* bus, void (*eventCallback)(SDL_Event* event), void (*resetCallback)())
    : window(window), renderer(renderer), width(width), height(height), bus(bus), eventCallback(eventCallback), resetCallback(resetCallback), singleStep(false)
{
    Init();
}

Gui::~Gui()
{
}

void Gui::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        
        return;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)io;
    io->IniFilename = NULL;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
    

    for (int i = 0; i < palletTexture.size(); i++)
    {
        palletTexture[i] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 1, 1);
        if (palletTexture[i] == NULL)
        {
            SDL_Log("Unable to create pallet texture: %s", SDL_GetError());
            return;
        }
    }

    patternTable0Texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 128, 128);
    if (patternTable0Texture == NULL)
    {
        SDL_Log("Unable to create pattern 0 texture: %s", SDL_GetError());
        return;
    }

    patternTable1Texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 128, 128);
    if (patternTable1Texture == NULL)
    {
        SDL_Log("Unable to create pattern 1 texture: %s", SDL_GetError());
        return;
    }
}

void Gui::Render(std::shared_ptr<bool> isRunning)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            *isRunning = false;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            *isRunning = false;

        eventCallback(&event);
    }

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();


    static bool show_CPU = false;
    static bool show_RAM = false;
    static bool show_Assembly = false;
    static bool show_PPU = false;
    static bool show_Controller = false;

    if (disassembler.empty())
        show_Assembly = false;

    if (show_CPU) showCPU(&show_CPU);
    if (show_RAM) showRAM(&show_RAM);
    if (show_Assembly) showAssembly(&show_Assembly, 33);
    if (show_PPU) showPPU(&show_PPU);
    if (show_Controller) showController(&show_Controller);

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load")) 
            {
                wchar_t filePath[260];

                if (OpenFileDialog(filePath, sizeof(filePath) / sizeof(filePath[0]))) {
                    std::string filePathStr = WCharToString(filePath);
                    std::shared_ptr<Cartrige> karta = std::make_shared<Cartrige>(filePathStr);
                    bus->insertCartrige(karta);
                    disassembler = bus->cpu.disassemble(0x0000, 0xFFFF);
                    bus->reset();
                }
                else {
                    std::cout << "Zadny soubor nebyl vybran nebo doslo k chybe." << std::endl;
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Reset")) { bus->reset(); };
            ImGui::MenuItem("CPU", NULL, &show_CPU);
            ImGui::MenuItem("RAM", NULL, &show_RAM);
            ImGui::MenuItem("PPU", NULL, &show_PPU);
            ImGui::MenuItem("Assembly", NULL, &show_Assembly);
            ImGui::MenuItem("Controller", NULL, &show_Controller);
            ImGui::Checkbox("SingleStep", &singleStep);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }


    ImGui::Render();
    SDL_RenderSetScale(renderer, io->DisplayFramebufferScale.x, io->DisplayFramebufferScale.y);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
}

bool Gui::OpenFileDialog(wchar_t* filePath, DWORD filePathSize) {
    OPENFILENAMEW ofn;
    wchar_t szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
    ofn.lpstrFilter = L"NES Files\0*.nes\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn) == TRUE) {
        wcsncpy_s(filePath, filePathSize, ofn.lpstrFile, _TRUNCATE);
        return true;
    }
    return false;
}

void Gui::showController(bool* p_open)
{
    ImVec4 red(1, 0, 0, 1);
    ImVec4 green(0, 1, 0, 1);

    ImGui::SetNextWindowSize(ImVec2(150, 150), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Controller", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("Arrows:");
        ImGui::TextColored(bus->controller[0] & 0x02 ? green : red, "LEFT");
        ImGui::SameLine();
        ImGui::TextColored(bus->controller[0] & 0x01 ? green : red, "RIGHT");
        ImGui::SameLine();
        ImGui::TextColored(bus->controller[0] & 0x08 ? green : red, "UP");
        ImGui::SameLine();
        ImGui::TextColored(bus->controller[0] & 0x04 ? green : red, "DOWN");
 
        ImGui::TextColored(bus->controller[0] & 0x10 ? green : red, "A (s)");
        ImGui::TextColored(bus->controller[0] & 0x20 ? green : red, "B (d)");
        ImGui::TextColored(bus->controller[0] & 0x40 ? green : red, "START (z)");
        ImGui::TextColored(bus->controller[0] & 0x80 ? green : red, "SELECT (x)");
    }
    ImGui::End();
}

void Gui::showCPU(bool* p_open)
{
    ImVec4 red(1, 0, 0, 1);
    ImVec4 green(0, 1, 0, 1);

    ImGui::SetNextWindowSize(ImVec2(200, 150), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("CPU", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("Flags: ");
        ImGui::SameLine();
        ImGui::TextColored(bus->cpu.getFlag(Cpu::C) ? green : red, "C");
        ImGui::SameLine();
        ImGui::TextColored(bus->cpu.getFlag(Cpu::Z) ? green : red, "Z");
        ImGui::SameLine();
        ImGui::TextColored(bus->cpu.getFlag(Cpu::I) ? green : red, "I");
        ImGui::SameLine();
        ImGui::TextColored(bus->cpu.getFlag(Cpu::D) ? green : red, "D");
        ImGui::SameLine();
        ImGui::TextColored(bus->cpu.getFlag(Cpu::B) ? green : red, "B");
        ImGui::SameLine();
        ImGui::TextColored(bus->cpu.getFlag(Cpu::U) ? green : red, "U");
        ImGui::SameLine();
        ImGui::TextColored(bus->cpu.getFlag(Cpu::V) ? green : red, "V");
        ImGui::SameLine();
        ImGui::TextColored(bus->cpu.getFlag(Cpu::N) ? green : red, "N");

        ImGui::Text("program counter:");
        ImGui::SameLine();
        ImGui::Text(hex(bus->cpu.pc,4));

        ImGui::Text("rA:");
        ImGui::SameLine();
        ImGui::Text(hex(bus->cpu.rA,2));

        ImGui::Text("rX:");
        ImGui::SameLine();
        ImGui::Text(hex(bus->cpu.rX,2));

        ImGui::Text("rY:");
        ImGui::SameLine();
        ImGui::Text(hex(bus->cpu.rY,2));

        ImGui::Text("stack pointer:");
        ImGui::SameLine();
        ImGui::Text(hex(bus->cpu.sp,2));
    }
    ImGui::End();
}

void Gui::showRAM(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(700, 700), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("RAM", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        static int bytes_per_row = 16;
        if (ImGui::BeginTable("hexTable", bytes_per_row + 1, ImGuiTableFlags_Borders))
        {
            // Header
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Offset");
            for (int i = 0; i < bytes_per_row; ++i)
            {
                ImGui::TableNextColumn();
                ImGui::Text(hex(i,2));
            }

            // Data
            for (size_t row = 0; row < bus->ram.size() / bytes_per_row; ++row)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text(hex(row * bytes_per_row,6));

                for (int col = 0; col < bytes_per_row; ++col)
                {
                    ImGui::TableNextColumn();
                    ImGui::Text(hex(bus->ram[row * bytes_per_row + col],2));
                }
            }

            ImGui::EndTable();
        }
    }
    ImGui::End();
}

void Gui::showAssembly(bool* p_open, int lines)
{
    ImVec4 red(1, 0, 0, 1);
    ImGui::SetNextWindowSize(ImVec2(250, 600), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Assembly", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        int half_lines = lines / 2;
        auto temp = disassembler.lower_bound(bus->cpu.pc);

        for (int i = 0; i < half_lines; i++)
            --temp;

        for (int i = 0; i < lines; i++)
        {
            if (temp != disassembler.end())
            {
                if (i == half_lines) // Highlight the middle line
                {
                    ImGui::TextColored(red, "%04X: %s", temp->first, temp->second.c_str());
                }
                else
                {
                    ImGui::Text("%04X: %s", temp->first, temp->second.c_str());
                }
                ++temp;
            }
        }
    }
    ImGui::End();
}

void Gui::showPPU(bool* p_open)
{
    ImVec4 red(1, 0, 0, 1);
    ImVec4 green(0, 1, 0, 1);

    ImGui::SetNextWindowSize(ImVec2(750, 550), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("PPU", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("Control: ");
        ImGui::SameLine();
        ImGui::Text(hex(bus->ppu.control.reg,2));
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.control.enable_nmi? green : red, "NMI");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.control.increment_mode ? green : red, "increment");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.control.nametable_x ? green : red, "nameX");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.control.nametable_y ? green : red, "nameY");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.control.pattern_background? green : red, "patternBG");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.control.pattern_sprite ? green : red, "patternSP");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.control.slave_mode ? green : red, "slave");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.control.sprite_size ? green : red, "SPsize");

        ImGui::Text("Mask: ");
        ImGui::SameLine();
        ImGui::Text(hex(bus->ppu.mask.reg, 2));
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.mask.render_sprites ? green : red, "renderSP");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.mask.render_background ? green : red, "renderBG");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.mask.render_sprites_left ? green : red, "renderSPL");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.mask.render_background_left ? green : red, "renderBGL");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.mask.grayscale ? green : red, "grayscale");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.mask.enchance_red ? green : red, "eR");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.mask.enchance_green ? green : red, "eG");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.mask.enchance_blue ? green : red, "eB");

        ImGui::Text("Status: ");
        ImGui::SameLine();
        ImGui::Text(hex(bus->ppu.status.reg, 2));
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.status.sprite_overflow ? green : red, "SPoverflow");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.status.sprite_zero_hit ? green : red, "SPzeroHit");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.status.vertical_blank ? green : red, "VB");
        ImGui::SameLine();
        ImGui::TextColored(bus->ppu.status.unused ? green : red, "U");

        ImGui::Text("Addr: ");
        ImGui::SameLine();
        ImGui::Text(hex(bus->ppu.vram_addr.reg, 4));

        ImGui::Text("Scanline: ");
        ImGui::SameLine();
        ImGui::Text(std::to_string(bus->ppu.scanline).c_str());

        ImGui::Text("Cycle: ");
        ImGui::SameLine();
        ImGui::Text(std::to_string(bus->ppu.cycle).c_str());

        ImGui::Spacing();

        UpdatePalletTexture();
        UpdatePatternTexture();

        ImGui::Columns(2, nullptr, false);

        ImGui::Text("Pattern table 0:");
        ImGui::Image(patternTable0Texture, ImVec2(300, 300));

        ImGui::NextColumn();
        
        ImGui::Text("Pattern table 1:");
        ImGui::Image(patternTable1Texture, ImVec2(300, 300));
        ImGui::Columns(1);
        ImGui::Spacing();
        ImGui::Text("Pallets:");
        ImGui::Columns(4,nullptr, false);
        for (int i = 0; i < palletTexture.size(); i+=4)
        {
            for (int j = 0; j < 4; j++)
            {
                ImGui::Image(palletTexture[i+j], ImVec2(20, 20));
                if(j != 3)
                    ImGui::SameLine();
            }
            ImGui::NextColumn();
        }
    }
    ImGui::End();
}

char* hex(uint32_t n, uint8_t d)
{
    std::string s(d, '0');
    for (int i = d - 1; i >= 0; i--, n >>= 4)
        s[i] = "0123456789ABCDEF"[n & 0xF];
    char* result = new char[d + 1]; // Allocate memory for the result
    strcpy_s(result, d + 1, s.c_str()); // Copy the string content to the allocated memory
    return result;
};

char* combineChar(char* first, const char* second)
{
    int len = strlen(first) + strlen(second);
    std::string jedna = first;
    std::string dva = second;
    std::string s = jedna + dva;
    char* result = new char[len];
    strcpy_s(result, len+1, s.c_str());
    return result;
}

void Gui::UpdatePalletTexture()
{
    for (int i = 0; i < palletTexture.size(); i++)
    {
        rndr::Pixel pixel = bus->ppu.GetColourFromPaletteRam(i);
        void* pixels = nullptr;
        int pitch = 0;
        SDL_LockTexture(palletTexture[i], NULL, &pixels, &pitch);
        uint8_t* dst = static_cast<uint8_t*>(pixels);
        dst[0] = pixel.r;
        dst[1] = pixel.g;
        dst[2] = pixel.b;
        dst[3] = pixel.a;
        SDL_UnlockTexture(palletTexture[i]);
    }
}

void Gui::UpdatePatternTexture()
{
    rndr::Sprite table0 = bus->ppu.GetPatternTable(0);
    rndr::Sprite table1 = bus->ppu.GetPatternTable(1);

    void* pixels = nullptr;
    int pitch = table0.width*4;

    SDL_LockTexture(patternTable0Texture, NULL, &pixels, &pitch);

    uint8_t* dst = static_cast<uint8_t*>(pixels);
    for (int y = 0; y < table0.height; y++)
    {
        for (int x = 0; x < table0.width; x++)
        {
            const rndr::Pixel& pixel = table0.pixels[y * table0.width + x];
            dst[y * 4 * 128 + x * 4 + 0] = pixel.r;
            dst[y * 4 * 128 + x * 4 + 1] = pixel.g;
            dst[y * 4 * 128 + x * 4 + 2] = pixel.b;
            dst[y * 4 * 128 + x * 4 + 3] = pixel.a;
        }
    }

    SDL_UnlockTexture(patternTable0Texture);

    pixels = nullptr;
    pitch = table1.width * 4;

    SDL_LockTexture(patternTable1Texture, NULL, &pixels, &pitch);

    dst = static_cast<uint8_t*>(pixels);
    for (int y = 0; y < table1.height; y++)
    {
        for (int x = 0; x < table1.width; x++)
        {
            const rndr::Pixel& pixel = table1.pixels[y * table0.width + x];
            dst[y * 4 * 128 + x * 4 + 0] = pixel.r;
            dst[y * 4 * 128 + x * 4 + 1] = pixel.g;
            dst[y * 4 * 128 + x * 4 + 2] = pixel.b;
            dst[y * 4 * 128 + x * 4 + 3] = pixel.a;
        }
    }

    SDL_UnlockTexture(patternTable1Texture);
}

void Gui::Clean()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
