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

Gui::Gui(SDL_Window* window, SDL_Renderer* renderer, int width, int height, Cpu* cpu, Ppu* ppu, void (*eventCallback)(SDL_Event* event))
{
	this->window = window;
	this->renderer = renderer;
    this->width = width;
    this->height = height;
    this->cpu = cpu;
    this->ppu = ppu;
    this->eventCallback = eventCallback;
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
    disassemlbed = cpu->disassemble(0x0000,0xFFFF);
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
        int start_line = static_cast<int>(cpu->pc - half_lines);
        auto temp = disassemlbed.lower_bound(start_line);

        for (int i = 0; i < lines; i++)
        {
            if (temp != disassemlbed.end())
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
    ImGui::SetNextWindowSize(ImVec2(200, 150), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("PPU", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
    {
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

void Gui::Clean()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
