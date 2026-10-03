#include "Windows.h"

#include <iostream>
#include <memory>

// OpenGL
#include <glad/glad.h>
#include <SDL3/SDL.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ImGui
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include "ImGuizmo.h"

//Additional C++ Headers
#include <vector>
#include <dxgi1_4.h> //<----- needed to see VRAM values
#include <cpuinfo_x86.h> //<----- needed to check the caps of the cpu

Windows::Windows() : Module(){
	name = "windows";
}

Windows::~Windows() {

}

bool Windows::Awake() {
	// Init SDL
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		return false;
	}
	else {
		//Below theres an example code of xml on how to configure the window using xml it prob won't work with nlohmann
		/*Uint32 flags = 0;
		bool fullscreen = configParameters.child("fullscreen").attribute("value").as_bool();
		bool borderless = configParameters.child("borderless").attribute("value").as_bool();
		bool resizable = configParameters.child("resizable").attribute("value").as_bool();
		bool fullscreen_window = configParameters.child("fullscreen_window").attribute("value").as_bool();

		width = configParameters.child("resolution").attribute("width").as_int();
		height = configParameters.child("resolution").attribute("height").as_int();
		scale = configParameters.child("resolution").attribute("scale").as_int();

		if (fullscreen == true)        flags |= SDL_WINDOW_FULLSCREEN;
		if (borderless == true)        flags |= SDL_WINDOW_BORDERLESS;
		if (resizable == true)         flags |= SDL_WINDOW_RESIZABLE;*/
		// Setup Min/Major version for using OpenGL 4.6
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		// Set Core Profile Mode
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// Create OpenGL window using SDL
		window = SDL_CreateWindow("EnginishGL",
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);
		if (window == nullptr)
		{
			SDL_Quit();
			return false;
		}
	}
    flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	return true;
}

bool Windows::PreUpdate() {
	//FPS Update
	fps = 1.0f / dt;
	if (ms_log.size() > 59) {
		ms_log.erase(ms_log.begin());
	}
	ms_log.emplace_back(dt * 1000);

	if (fps_log.size() > 59) {
		fps_log.erase(fps_log.begin());
	}
	fps_log.emplace_back(fps);
}

bool Windows::Update() {

    ImGui::ShowDemoWindow();
    ImGui::ShowDebugLogWindow();

    // Header Menu Bar
    static bool showAbout = false;

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                exit(0);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("Github Documentation")) {
                SDL_OpenURL("https://github.com/ErikArgemi/JLE-Engine#gameenginetemplate");
            }
            if (ImGui::MenuItem("Report a Bug")) {
                SDL_OpenURL("https://github.com/ErikArgemi/JLE-Engine/issues");
            }
            if (ImGui::MenuItem("Download Latest:")) {
                SDL_OpenURL("https://github.com/ErikArgemi/JLE-Engine");
            }
            if (ImGui::MenuItem("About")) {
                showAbout = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (showAbout)
    {
        ImGui::Begin("About", &showAbout);
        AddWindow("About");

        ImGui::Text("JLE Engine v0.1");
        ImGui::Text("Welcome to the glorious JLE Engine, JLE stands for Jia, Luying and Erik.");
        ImGui::Text("By Group 1: Jia Hao Zhao Deng, Luying Bao Cheng and Erik Argemí Chinchilla");
        ImGui::TextUnformatted(R"(3rd Party Libraries Used:
            - SDL
            - glm
            - sdl3
            - opengl
            - glad
            - imgui
            - imguizmo
            )");
        ImGui::TextUnformatted(R"(MIT License

            Copyright (c) 2026 Jia Hao Zhao Deng & Luying Bao Cheng & Erik Argemí Chinchilla

            Permission is hereby granted, free of charge, to any person obtaining a copy
            of this software and associated documentation files (the "Software"), to deal
            in the Software without restriction, including without limitation the rights
            to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
            copies of the Software, and to permit persons to whom the Software is
            furnished to do so, subject to the following conditions:

            The above copyright notice and this permission notice shall be included in all
            copies or substantial portions of the Software.

            THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
            IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
            FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
            AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
            LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
            OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
            SOFTWARE.)");
        ImGui::End();
    }

    //Configuration window
    ImGui::Begin("Configuration window", nullptr, flags);
    AddWindow("Configuration window");
    if (ImGui::CollapsingHeader("Application"))
    {
        ImGui::Text("JLE-Engine");
        ImGui::Text("UPC CITM");
        ImGui::SliderInt("Max FPS", &max_fps, 0, 120, ImGuiSliderFlags(ImGuiSliderFlags_None));

        char title[25];
        sprintf_s(title, 25, "Framerate %.f", fps_log[fps_log.size() - 1]);
        ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 120.0f, ImVec2(310, 100));
        sprintf_s(title, 25, "Milliseconds %.f", ms_log[ms_log.size() - 1]);
        ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
    }
    if (ImGui::CollapsingHeader("Window"))
    {

        //ImGui::SliderFloat("Brightness", ); TODO: Search how
        ImGui::SliderFloat("Brightness", &brightness, 0.1, 1.0, ImGuiSliderFlags(ImGuiSliderFlags_None));

        //Windows size TODO: Keep proportions
        static int currentSize = 1;
        if (ImGui::Combo("Window sizes", &currentSize, windowSizes, GLM_COUNTOF(windowSizes))) {
            SDL_SetWindowSize(window, windowSizesValues[currentSize].x, windowSizesValues[currentSize].y);
            glViewport(0, 0, windowSizesValues[currentSize].x, windowSizesValues[currentSize].y);
        }

        ImGui::Text("Refresh rate: %i", (int)fps);

        if (ImGui::Checkbox("Fullscreen", &fullscreen)) {
            SDL_SetWindowFullscreen(window, fullscreen);
        }

        if (ImGui::Checkbox("Resizable", &resizable)) {
            SDL_SetWindowResizable(window, resizable);
        }

        if (ImGui::Checkbox("Borderless", &borderless)) {
            SDL_SetWindowBordered(window, !borderless);
        }
    }
    if (ImGui::CollapsingHeader("Hardware")) {
        //get info from the cpu
        const cpu_features::X86Features features = cpu_features::GetX86Info().features;
        std::string capsCPU = GetCapsFromCpu(features);
        pFactory.reset();
        adapter.reset();

        HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&pFactory);
        pFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(&adapter));
        pFactory->Release();
        DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
        adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);
        adapter->Release();
        size_t usedVRAM = videoMemoryInfo.CurrentUsage / 1024 / 1024;
        //end of stack overflow copy-paste
        size_t availableVRAM = videoMemoryInfo.AvailableForReservation / 1024 / 1024;
        size_t budgetVRAM = videoMemoryInfo.Budget / 1024 / 1024;
        size_t reservedVRAM = videoMemoryInfo.CurrentReservation / 1024 / 1024;

        ImGui::Separator();
        ImGui::Text("CPUs: %i (Cache: %ikb)", SDL_GetNumLogicalCPUCores(), SDL_GetCPUCacheLineSize());
        int systemRAM = SDL_GetSystemRAM();
        ImGui::Text("System RAM: %.2f %s", (systemRAM > 1024) ? (float)systemRAM / 1024 : (float)systemRAM, (systemRAM > 1024) ? "Gb" : "Mb");
        ImGui::Text("Caps: %s", capsCPU.c_str());
        ImGui::Separator();
        ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
        ImGui::Text("Brand: %s", glGetString(GL_RENDERER));
        ImGui::Text("VRAM budget: %.2f %s", (budgetVRAM > 1024) ? (float)budgetVRAM / 1024 : (float)budgetVRAM, (budgetVRAM > 1024) ? "Gb" : "Mb");
        ImGui::Text("VRAM usage: %.2f %s", (usedVRAM > 1024) ? (float)usedVRAM / 1024 : (float)usedVRAM, (usedVRAM > 1024) ? "Gb" : "Mb");
        ImGui::Text("VRAM available: %.2f %s", (availableVRAM > 1024) ? (float)availableVRAM / 1024 : (float)availableVRAM, (availableVRAM > 1024) ? "Gb" : "Mb");
        ImGui::Text("VRAM reserved: %.2f %s", (reservedVRAM > 1024) ? (float)reservedVRAM / 1024 : (float)reservedVRAM, (reservedVRAM > 1024) ? "Gb" : "Mb");

    }
    ImGui::End();

    // Dark block for brightness
    float darckBlock = 1.0f - brightness;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::GetForegroundDrawList(viewport)->AddRectFilled(viewport->Pos, ImVec2(viewport->Pos.x + viewport->Size.x, viewport->Pos.y + viewport->Size.y), IM_COL32(0, 0, 0, (int)(darckBlock * 255.0f)));

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap window
    SDL_GL_SwapWindow(window);
}

bool Windows::PostUpdate() {
    // FRAME CONTROL
    SDL_GetCurrentTime(&currentTime);
    float current_dt = (currentTime - prevTime) / 1000000000.0f;
    prevTime = currentTime;
    float max_dt;
    if (max_fps != 0) {
        max_dt = 1000 / max_fps;
    }
    else max_dt = 0;

    if (current_dt < max_dt && max_dt != 0.0f) {
        SDL_Delay(max_dt - current_dt);
    }
}

bool Windows::CleanUp() {
	SDL_DestroyWindow(window);
}

void Windows::AddWindow(std::string newWindow) {
	for (const auto& window : windows) {
		if (window == newWindow) return;
	}
	windows.emplace_back(newWindow);
}

void Windows::ResizeWindows(ImVec2 windowSize) {
	currentWindowSize = windowSize;
	for (auto& window : windows) {
		RescaleWindow(window);
	}
	prevWindowsSize = currentWindowSize;
}

//privates
void Windows::RescaleWindow(std::string window) {
	ImGuiWindow* win = ImGui::FindWindowByName(window.c_str());
	if (!win) return;

	float scaleX = (float)currentWindowSize.x / (float)prevWindowsSize.x;
	float scaleY = (float)currentWindowSize.y / (float)prevWindowsSize.y;

	ImVec2 newPos(win->Pos.x * scaleX, win->Pos.y * scaleY);
	ImVec2 newSize(win->Size.x * scaleX, win->Size.y * scaleY);

	ImGui::SetWindowPos(window.c_str(), newPos, ImGuiCond_Always);
	ImGui::SetWindowSize(window.c_str(), newSize, ImGuiCond_Always);
}

std::string Windows::GetCapsFromCpu(const cpu_features::X86Features& features) {
	std::string listOfFeatures = "";
	if (features.sse) { listOfFeatures += "SSE, "; }
	if (features.sse2) { listOfFeatures += "SSE2, "; }
	if (features.sse3) { listOfFeatures += "SSE3, "; }
	if (features.ssse3) { listOfFeatures += "SSSE3, "; }
	if (features.sse4_1) { listOfFeatures += "SSE4.1, "; }
	if (features.sse4_2) { listOfFeatures += "SSE4.2, "; }
	if (features.avx) { listOfFeatures += "AVX, "; }
	if (features.avx2) { listOfFeatures += "AVX2, "; }
	if (features.avx512f) { listOfFeatures += "AVX-512F, "; }
	if (features.avx512cd) { listOfFeatures += "AVX-512CD, "; }
	if (features.avx512er) { listOfFeatures += "AVX-512ER, "; }
	if (features.avx512pf) { listOfFeatures += "AVX-512PF, "; }
	if (features.avx512vl) { listOfFeatures += "AVX-512VL, "; }
	if (features.avx512dq) { listOfFeatures += "AVX-512DQ, "; }
	if (features.avx512bw) { listOfFeatures += "AVX-512BW, "; }
	if (features.avx512ifma) { listOfFeatures += "AVX-512IFMA, "; }
	if (features.avx512vbmi) { listOfFeatures += "AVX-512VBMI, "; }
	if (features.avx512_4vnniw) { listOfFeatures += "AVX-512 4VNNIW, "; }
	if (features.avx512_4fmaps) { listOfFeatures += "AVX-512 4FMAPS, "; }
	if (features.avx512vpopcntdq) { listOfFeatures += "AVX-512VPOPCNTDQ, "; }
	if (features.avx512vnni) { listOfFeatures += "AVX-512VNNI, "; }
	if (features.avx512vbmi2) { listOfFeatures += "AVX-512VBMI2, "; }
	if (features.avx512bitalg) { listOfFeatures += "AVX-512BITALG, "; }
	if (features.avx512_vp2intersect) { listOfFeatures += "AVX-512 VP2INTERSECT, "; }
	listOfFeatures.pop_back();
	listOfFeatures.pop_back();
	return listOfFeatures;
}