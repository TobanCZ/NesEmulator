#include "Gui.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <memory>

Gui::Gui(SDL_Window* window, SDL_Renderer* renderer, int width, int height)
{
	this->window = window;
	this->renderer = renderer;
    this->width = width;
    this->height = height;
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
    }

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();


    static bool show_CPU = false;
    if (show_CPU) showCPU(&show_CPU);

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
            if (ImGui::MenuItem("PPU")) {}
            if (ImGui::MenuItem("Assembly")) {}
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
    ImGui::SetNextWindowSize(ImVec2(200, 150), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("CPU", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("Flags: ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "C");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Z");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "I");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "D");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "B");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "U");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "V");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "N");

        ImGui::Text("srogram sounter: ");
        ImGui::SameLine();
        ImGui::Text("0x00");

        ImGui::Text("rA: ");
        ImGui::SameLine();
        ImGui::Text("0x00");

        ImGui::Text("rX: ");
        ImGui::SameLine();
        ImGui::Text("0x00");

        ImGui::Text("rY: ");
        ImGui::SameLine();
        ImGui::Text("0x00");

        ImGui::Text("stack pointer: ");
        ImGui::SameLine();
        ImGui::Text("0x00");
    }
    ImGui::End();
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
