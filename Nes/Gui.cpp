#include "Gui.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <memory>
#include "Cpu.h"
#include "Ppu.h"
#include <string>
#include <iostream>
#include <cstring> 
#include <cstdlib>
#include <stdio.h>
#include <string.h>

char* hex(uint32_t n, uint8_t d);
char* combineChar(char* first, const char* second);
SDL_Texture* CreateTextureFromSprite(SDL_Renderer* renderer, const rndr::Sprite& sprite);
void DrawColoredSquare(ImDrawList* draw_list, const ImVec2& pos, const ImVec2& size, ImU32 color) {
    draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), color);
}


Gui::Gui(SDL_Window* window, SDL_Renderer* renderer, int width, int height, Cpu* cpu, Ppu* ppu, void (*eventCallback)(SDL_Event* event), void (*resetCallback)())
{
	this->window = window;
	this->renderer = renderer;
    this->width = width;
    this->height = height;
    this->cpu = cpu;
    this->ppu = ppu;
    this->eventCallback = eventCallback;
    this->resetCallback = resetCallback;
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
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
    disassembler = cpu->disassemble(0x0000,0xFFFF);
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

    if (show_CPU) showCPU(&show_CPU);
    if (show_RAM) showRAM(&show_RAM);
    if (show_Assembly) showAssembly(&show_Assembly,33);
    if (show_PPU) showPPU(&show_PPU);

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Reset")) { resetCallback(); };
            ImGui::MenuItem("CPU", NULL, &show_CPU);
            ImGui::MenuItem("RAM", NULL, &show_RAM);
            ImGui::MenuItem("PPU", NULL, &show_PPU);
            ImGui::MenuItem("Assembly", NULL, &show_Assembly);
            ImGui::Checkbox("SingleStep", &singleStep);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }


    ImGui::ShowDemoWindow();
    ImGui::Render();
    SDL_RenderSetScale(renderer, io->DisplayFramebufferScale.x, io->DisplayFramebufferScale.y);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
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
        ImGui::TextColored(cpu->getFlag(Cpu::C) ? green : red, "C");
        ImGui::SameLine();
        ImGui::TextColored(cpu->getFlag(Cpu::Z) ? green : red, "Z");
        ImGui::SameLine();
        ImGui::TextColored(cpu->getFlag(Cpu::I) ? green : red, "I");
        ImGui::SameLine();
        ImGui::TextColored(cpu->getFlag(Cpu::D) ? green : red, "D");
        ImGui::SameLine();
        ImGui::TextColored(cpu->getFlag(Cpu::B) ? green : red, "B");
        ImGui::SameLine();
        ImGui::TextColored(cpu->getFlag(Cpu::U) ? green : red, "U");
        ImGui::SameLine();
        ImGui::TextColored(cpu->getFlag(Cpu::V) ? green : red, "V");
        ImGui::SameLine();
        ImGui::TextColored(cpu->getFlag(Cpu::N) ? green : red, "N");

        ImGui::Text("srogram sounter:");
        ImGui::SameLine();
        ImGui::Text(hex(cpu->pc,4));

        ImGui::Text("rA:");
        ImGui::SameLine();
        ImGui::Text(hex(cpu->rA,2));

        ImGui::Text("rX:");
        ImGui::SameLine();
        ImGui::Text(hex(cpu->rX,2));

        ImGui::Text("rY:");
        ImGui::SameLine();
        ImGui::Text(hex(cpu->rY,2));

        ImGui::Text("stack pointer:");
        ImGui::SameLine();
        ImGui::Text(hex(cpu->sp,2));
    }
    ImGui::End();
}

void Gui::showRAM(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(200, 150), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("RAM", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
    {
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
        auto temp = disassembler.lower_bound(cpu->pc);

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

    ImGui::SetNextWindowSize(ImVec2(750, 750), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("PPU", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("Control: ");
        ImGui::SameLine();
        ImGui::TextColored(ppu->control.enable_nmi? green : red, "NMI");
        ImGui::SameLine();
        ImGui::TextColored(ppu->control.increment_mode ? green : red, "increment");
        ImGui::SameLine();
        ImGui::TextColored(ppu->control.nametable_x ? green : red, "nameX");
        ImGui::SameLine();
        ImGui::TextColored(ppu->control.nametable_y ? green : red, "nameY");
        ImGui::SameLine();
        ImGui::TextColored(ppu->control.pattern_background? green : red, "patternBG");
        ImGui::SameLine();
        ImGui::TextColored(ppu->control.pattern_sprite ? green : red, "patternSP");
        ImGui::SameLine();
        ImGui::TextColored(ppu->control.slave_mode ? green : red, "slave");
        ImGui::SameLine();
        ImGui::TextColored(ppu->control.sprite_size ? green : red, "SPsize");

        ImGui::Text("Mask: ");
        ImGui::SameLine();
        ImGui::TextColored(ppu->mask.render_sprites ? green : red, "renderSP");
        ImGui::SameLine();
        ImGui::TextColored(ppu->mask.render_background ? green : red, "renderBG");
        ImGui::SameLine();
        ImGui::TextColored(ppu->mask.render_sprites_left ? green : red, "renderSPL");
        ImGui::SameLine();
        ImGui::TextColored(ppu->mask.render_background_left ? green : red, "renderBGL");
        ImGui::SameLine();
        ImGui::TextColored(ppu->mask.grayscale ? green : red, "grayscale");
        ImGui::SameLine();
        ImGui::TextColored(ppu->mask.enchance_red ? green : red, "eR");
        ImGui::SameLine();
        ImGui::TextColored(ppu->mask.enchance_green ? green : red, "eG");
        ImGui::SameLine();
        ImGui::TextColored(ppu->mask.enchance_blue ? green : red, "eB");

        ImGui::Text("Status: ");
        ImGui::SameLine();
        ImGui::TextColored(ppu->status.sprite_overflow ? green : red, "SPoverflow");
        ImGui::SameLine();
        ImGui::TextColored(ppu->status.sprite_zero_hit ? green : red, "SPzeroHit");
        ImGui::SameLine();
        ImGui::TextColored(ppu->status.vertical_blank ? green : red, "VB");
        ImGui::SameLine();
        ImGui::TextColored(ppu->status.unused ? green : red, "U");


        rndr::Sprite idk(100,100,std::vector<rndr::Pixel>(100*100,rndr::Pixel(255,0,0,1)));


        ImGui::Columns(2, nullptr, false);

        ImGui::Spacing();
        ImGui::Text("Pattern table 0:");
        ImGui::Image(CreateTextureFromSprite(renderer, idk), ImVec2(100, 100));

        ImGui::NextColumn();

        ImGui::Spacing();
        ImGui::Text("Pattern table 1:");

        ImGui::Image(CreateTextureFromSprite(renderer, idk), ImVec2(100, 100));

        ImGui::Columns(1);

        ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
        ImVec2 squareSize(20, 20);
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 pos(cursor_pos.x + (squareSize.x + ImGui::GetStyle().ItemSpacing.x), cursor_pos.y);
        DrawColoredSquare(draw_list, pos, squareSize, rndr::Pixel(255, 0, 0, 1).hex);
        ImGui::NextColumn();
       

        //ImGui::Colo
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

SDL_Texture* CreateTextureFromSprite(SDL_Renderer* renderer, const rndr::Sprite& sprite)
{
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, sprite.width, sprite.height);
    if (!texture) {
        return nullptr;
    }

    std::vector<uint32_t> pixelData(sprite.width * sprite.height);
    for (size_t i = 0; i < sprite.pixels.size(); ++i) {
        const rndr::Pixel& pixel = sprite.pixels[i];
        pixelData[i] = (pixel.r << 24) | (pixel.g << 16) | (pixel.b << 8) | pixel.a;
    }

    SDL_UpdateTexture(texture, nullptr, pixelData.data(), sprite.width * sizeof(uint32_t));
    return texture;
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
