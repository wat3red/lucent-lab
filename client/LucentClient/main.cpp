#define IMGUI_DEFINE_MATH_OPERATORS

#include <Windows.h>
#include <iostream>
#include <vector>
#include <winsock2.h>
#include <thread>
#include <mutex>
#include <ws2tcpip.h>

#include <oxorany/oxorany_include.h>

#include "auth/ui/login_window.h"

#include "MinHook/include/MinHook.h"

#include "glad/include/glad/glad.h"


#include "imgui/imgui_notify.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"

#include "resources/Jacquard12_Regular.h"
#include "resources/Montserrat_Regular.h"
#include "resources/Jost_Regular.h"
#include "resources/Jersey15_Regular.h"
#include "resources/Jersey25_Regular.h"
#include "resources/Podkova_Regular.h"
#include "resources/so2_weapon_font_left.h"
#include "resources/so2_weapon_font_right.h"
#include "resources/glyphs.h"
#include "resources/themes.h"

#include "functions_manager.h"
#include "network_manager/network_manager.h"

#include "lab_settings_ui.h"
#include "demo_ping.h"
#include "demo_config_echo.h"
#include "demo_frame_stats.h"

#pragma comment(lib, "ws2_32.lib")

typedef __int64(__fastcall* WglSwapBuffers_t)(HDC);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImFont* mainFont = nullptr;
ImFont* headerFont = nullptr;
ImFont* labelFont = nullptr;
ImFont* weaponFontLeft = nullptr;
ImFont* weaponFontRight = nullptr;
ImFont* glyphsFont = nullptr;
ImFont* cyrillicFont = nullptr;

LPVOID oWglSwapBuffers = NULL;
WNDPROC oWndProc = nullptr;
bool imGuiInitialized = false;
HWND currentHWND = nullptr;
volatile bool bShowMenu = true;
std::mutex g_socketMutex;

NetworkManager g_NetworkManager;
FunctionsManager g_FunctionsManager;

float menuAnimationProgress = 0.0f;
constexpr float MENU_ANIM_SPEED = 3.0f;
bool isMenuOpening = true;
bool isMenuClosing = false;
bool isFirstOpen = true;

bool g_RunTCPThread = true;


#include "blur_effect.h"


void MaxMousePosition(unsigned int position[2])
{
	RECT rect = {};
	GetClientRect(static_cast<HWND>(currentHWND), &rect);
	position[0] = rect.right;
	position[1] = rect.bottom;
}

HWND GetParent(HWND hWnd)
{
	HWND hParent;

	hParent = GetAncestor(hWnd, GA_PARENT);
	if (!hParent || hParent == GetDesktopWindow())
		return NULL;

	return hParent;
}

void InitImGui() {
	std::cout << oxorany("Initializing ImGui...") << std::endl;

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(currentHWND);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();

	LoadImGuiStyle(allThemes[settings.settingsUI.theme]);

	ImGuiIO& io = ImGui::GetIO();

	ImFontConfig font_cfg{};
	font_cfg.MergeMode = false; // основной
	font_cfg.PixelSnapH = true;
	//headerFont = io.Fonts->AddFontFromMemoryTTF(&Jersey15_Regular, sizeof(Jersey15_Regular), 27.0f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic());
	headerFont = io.Fonts->AddFontFromMemoryTTF(&Jost_Regular, sizeof(Jost_Regular), 32.f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic());

	font_cfg.MergeMode = true;
	font_cfg.PixelSnapH = true;
	static const ImWchar icon_ranges[] = { 0xe800, 0xe813, 0 };
	io.Fonts->AddFontFromMemoryTTF(&glyphs, sizeof(glyphs), 22.0f, &font_cfg, icon_ranges);

	labelFont = io.Fonts->AddFontFromMemoryTTF(&Montserrat_Regular, sizeof(Montserrat_Regular), 41.f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	//labelFont = io.Fonts->AddFontFromMemoryTTF(&Jacquard12_Regular, sizeof(Jacquard12_Regular), 41.f, nullptr, io.Fonts->GetGlyphRangesCyrillic());

	weaponFontLeft = io.Fonts->AddFontFromMemoryTTF(&so2_weapon_font_left, sizeof(so2_weapon_font_left), 20.f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	weaponFontRight = io.Fonts->AddFontFromMemoryTTF(&so2_weapon_font_right, sizeof(so2_weapon_font_right), 20.f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	//mainFont = io.Fonts->AddFontFromMemoryTTF(&Jersey25_Regular, sizeof(Jersey25_Regular), 21, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	mainFont = io.Fonts->AddFontFromMemoryTTF(&Jost_Regular, sizeof(Jost_Regular), 26.f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	//cyrillicFont = io.Fonts->AddFontFromMemoryTTF(&Podkova_Regular, sizeof(Podkova_Regular), 21, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	cyrillicFont = mainFont;

	imGuiInitialized = true;

	std::cout << oxorany("ImGui initialized") << std::endl;
}

void UpdateMousePos() {
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	unsigned int max_position[2];
	POINT point;

	MaxMousePosition(max_position);
	GetCursorPos(&point);
	ScreenToClient(static_cast<HWND>(currentHWND), &point);

	io.MousePos.x = point.x * (io.DisplaySize.x / max_position[0]);
	io.MousePos.y = point.y * (io.DisplaySize.y / max_position[1]);
}

__int64 __fastcall hWglSwapBuffers(HDC hdc) {

	if (!imGuiInitialized) {
		//std::cout << "[Hook] First SwapBuffers call, initializing ImGui..." << std::endl;
		currentHWND = WindowFromDC(hdc);
		InitImGui();
	}

	float deltaTime = ImGui::GetIO().DeltaTime;
	if (isMenuOpening)
	{
		menuAnimationProgress += deltaTime * MENU_ANIM_SPEED;
		if (menuAnimationProgress >= 1.0f)
		{
			menuAnimationProgress = 1.0f;
			isMenuOpening = false;
			bShowMenu = true;
		}
	}
	else if (isMenuClosing)
	{
		menuAnimationProgress -= deltaTime * MENU_ANIM_SPEED;
		if (menuAnimationProgress <= 0.0f)
		{
			menuAnimationProgress = 0.0f;
			isMenuClosing = false;
			bShowMenu = false;
		}
	}

	menuAnimationProgress = std::clamp(menuAnimationProgress, 0.0f, 1.0f);

	float easedProgress = easeOutQuint(menuAnimationProgress);

	// Инициализируем систему размытия, если это еще не сделано
	if (bShowMenu && !BlurEffect::isInitialized) {
		BlurEffect::Initialize();
	}

	static bool showAuthMenu = true;

	if (menuAnimationProgress > 0.0f && BlurEffect::isInitialized && settings.settingsUI.enableBlur && !showAuthMenu /*&& !isLegacyMode*/)
	{
		BlurEffect::Apply(2.5f * easedProgress);
	}

	// Теперь отрисовываем ImGui поверх возможно размытого фона
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::GetIO().FontGlobalScale = settings.settingsUI.menuScale;

	UpdateMousePos();

	if (showAuthMenu && LoginWindow::Draw())
		showAuthMenu = false;
	else if (!showAuthMenu) {
		g_FunctionsManager.RenderAllBackgroundUI();

		ImGui::RenderNotifications();
	
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, easedProgress);
		ImGui::PushFont(mainFont);

		ImGui::SetNextWindowBgAlpha(easedProgress);

		//ImGui::ShowStyleEditor();
		//ImGui::ShowMetricsWindow();
		if (menuAnimationProgress > 0.0f || bShowMenu) {
			ImGui::SetNextWindowSize({ oxorany(1224.f), oxorany(768.f) });
			ImGui::Begin(oxorany("main"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
			g_FunctionsManager.RenderAllUI();
			ImGui::End();
		}

		ImGui::PopFont();
		ImGui::PopStyleVar();

		g_FunctionsManager.SendAllSettings();
	}

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return reinterpret_cast<WglSwapBuffers_t>(oWglSwapBuffers)(hdc);
}

LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
	case WM_WINDOWPOSCHANGED:
	case WM_WINDOWPOSCHANGING:
	case WM_NCCALCSIZE:
	case WM_CHILDACTIVATE:
	case WM_NCPAINT:
	case WM_PAINT:
	case WM_ERASEBKGND:
		return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);

	case WM_KEYDOWN:
		// Lab build: competitive hotkey processor is not registered.
		if (wParam == settings.settingsUI.hotkeys.openMenu)
		{
			if (!bShowMenu && !isMenuOpening && !isMenuClosing)
			{
				isMenuOpening = true;
			}
			else if (bShowMenu && !isMenuOpening && !isMenuClosing)
			{
				isMenuClosing = true;
			}
			return 0; // Processed the key, no further handling needed
		}
		break; // Let other keys proceed to ImGui or original proc

	case WM_MOUSEMOVE:
		return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	}

	// Let ImGui handle the message if the menu is visible
	if (bShowMenu && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
		std::cout << "Let ImGui handle the message, the menu is visible" << std::endl;
		return true;
	}

	// All other cases: forward to the original window procedure
	if (!bShowMenu) {
		std::cout << "Forward to the original window procedure" << std::endl;
		return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	}
}

void InitFunctions() {
	// Lab/portfolio build: protocol demos only.
	g_FunctionsManager.AddFunction(new LabPing());
	g_FunctionsManager.AddFunction(new LabConfigEcho());
	g_FunctionsManager.AddFunction(new LabFrameStats());
	g_FunctionsManager.AddFunction(new LabSettingsUI());
}

void MainTcpThread() {
	while (g_RunTCPThread) {
		std::cout << oxorany("Trying to connect to server...") << std::endl;

		if (!g_NetworkManager.Connect(oxorany("127.0.0.1"), 64108, false)) {
			std::cout << oxorany("Failed to connect. Retrying in 1 second...") << std::endl;
			Sleep(1000);
			continue;
		}

		std::cout << oxorany("Connected to the server, starting main loop") << std::endl;

		g_FunctionsManager.InitAllFunctions();

		g_NetworkManager.SetRecieveDataCallback([](const std::vector<uint8_t>& data) {
			g_FunctionsManager.HandleIncomingData(data.data(), data.size());
			Sleep(10);
			});

		while (true) {
			//g_FunctionsManager.SendAllSettings();
			//Sleep(5);
			Sleep(100000);
		}
	}

	g_NetworkManager.Disconnect();
}

void CleanupAndExit(HMODULE hModule)
{
	std::cout << oxorany("[Cleanup] Start CleanupAndExit") << std::endl;

	// Вернуть оригинальный wndproc
	if (oWndProc && currentHWND)
	{
		std::cout << oxorany("[Cleanup] Restoring original WndProc...") << std::endl;
		SetWindowLongPtr(GetParent(currentHWND), GWLP_WNDPROC, (LONG_PTR)oWndProc);
		oWndProc = nullptr;
	}

	// Убираем хуки
	std::cout << oxorany("[Cleanup] Removing MinHook hooks...") << std::endl;
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();

	// Чистим ImGui
	if (imGuiInitialized)
	{
		if (ImGui::GetCurrentContext()) {
			if (ImGui::GetIO().BackendRendererUserData)
				ImGui_ImplOpenGL3_Shutdown();

			if (ImGui::GetIO().BackendPlatformUserData)
				ImGui_ImplWin32_Shutdown();

			ImGui::DestroyContext();
		}

		//std::cout << "[Cleanup] Shutting down ImGui..." << std::endl;
		//ImGui_ImplOpenGL3_Shutdown();
		//std::cout << "[Cleanup] ImplOpenGL3_Shutdown" << std::endl;

		//ImGui_ImplWin32_Shutdown();
		//std::cout << "[Cleanup] ImGui_ImplWin32_Shutdown" << std::endl;

		//ImGui::DestroyContext();
		//std::cout << "[Cleanup] DestroyContext" << std::endl;

		imGuiInitialized = false;
	}
	else {
		std::cout << oxorany("[Cleanup] ImGui not initialized, skipping...") << std::endl;
	}

	// Отключаем сеть
	std::cout << oxorany("[Cleanup] Disconnecting network...") << std::endl;
	g_NetworkManager.Disconnect();

	std::cout << oxorany("[Cleanup] Exiting thread...") << std::endl;
	FreeLibraryAndExitThread(hModule, 0);
}

DWORD WINAPI MainThread(HMODULE hModule)
{
	std::cout << oxorany("[Main] Started") << std::endl;

	// Anti-debug intentionally not initialized in the lab/portfolio build.
	LoginWindow::Init();

	HMODULE handle = GetModuleHandleA(oxorany("opengl32.dll"));
	if (!handle) {
		std::cout << oxorany("[Main] Failed to get opengl32.dll handle") << std::endl;
		CleanupAndExit(hModule);
	}

	LPVOID wglSwapBuffersAddr = GetProcAddress(handle, oxorany("wglSwapBuffers"));
	std::cout << oxorany("[Main] Hooking wglSwapBuffers at ") << wglSwapBuffersAddr << std::endl;

	MH_Initialize();
	MH_CreateHook(wglSwapBuffersAddr, hWglSwapBuffers, &oWglSwapBuffers);
	MH_EnableHook(MH_ALL_HOOKS);

	while (!imGuiInitialized)
		Sleep(50);

	std::cout << "[Main] ImGui initialized, installing WndProc..." << std::endl;

	if (currentHWND != NULL) {
		// Determine the required buffer size
		int length = GetWindowTextLength(currentHWND);
		if (length > 0) {
			// Allocate a buffer for the window title
			std::vector<WCHAR> buffer(length + 1); // +1 for null terminator
			GetWindowText(currentHWND, buffer.data(), length + 1);

			std::wcout << L"Window Title: " << buffer.data() << std::endl;
		}
		else {
			std::wcout << L"Window has no title." << std::endl;
		}
	}
	else {
		std::wcout << L"Window not found." << std::endl;
	}

	if (GetParent(currentHWND) != NULL) {
		// Determine the required buffer size
		int length = GetWindowTextLength(GetParent(currentHWND));
		if (length > 0) {
			// Allocate a buffer for the window title
			std::vector<WCHAR> buffer(length + 1); // +1 for null terminator
			GetWindowText(GetParent(currentHWND), buffer.data(), length + 1);

			std::wcout << L"GetParent(currentHWND) Title: " << buffer.data() << std::endl;
		}
		else {
			std::wcout << L"GetParent(currentHWND) has no title." << std::endl;
		}
	}
	else {
		std::wcout << L"GetParent(currentHWND) not found." << std::endl;
	}

	oWndProc = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(GetParent(currentHWND), GWLP_WNDPROC, (LONG_PTR)WndProc)
		);
	
	InitFunctions();
	std::cout << oxorany("[Main] Functions initialized") << std::endl;

	std::thread(MainTcpThread).detach();

	MSG msg;
	while (!GetAsyncKeyState(VK_END))
	{
		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(50);
	}

	std::cout << oxorany("[Main] VK_END pressed, calling Cleanup...") << std::endl;
	CleanupAndExit(hModule);
	return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
#ifdef _DEBUG
		AllocConsole();
		freopen_s((FILE**)stdout, oxorany("CONOUT$"), oxorany("w"), stdout);
#endif

		DisableThreadLibraryCalls(hModule);

		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		std::cout << oxorany("DLL_PROCESS_DETACH") << std::endl;
		break;
	}
	return true;
}