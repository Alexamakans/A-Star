#pragma region Defines
#define SAFE_DELETE(x) if (x != nullptr) { delete x; x = nullptr; }
#pragma endregion

#pragma region Headers
#include <chrono>
#include <thread>
#include <mutex>

#include <World.hpp>
#include <pathfinding/Pathfinder.hpp>

#include <Windows.h> // GetAsyncKeyState

// Console
#include <graphics/console/ConsoleSurface.hpp>
#include <graphics/console/ConsoleContext.hpp>

// DirectX11

// Win32
#include <graphics/win32/WinSurface.hpp>
#include <graphics/win32/WinContext.hpp>
#pragma endregion

#pragma region Constants
constexpr int CONSOLE_APP = 0;
constexpr int WIN32_APP = 1;
constexpr int MAX_APPS = 2;
#pragma endregion

#pragma region Multithreading
std::thread inputThread;
std::thread appThread;

std::mutex appTypeChangeMutex;
#pragma endregion

#pragma region World Settings
constexpr int WORLD_TILE_WIDTH = 32;
constexpr int WORLD_TILE_HEIGHT = 32;
#pragma endregion

#pragma region App variables
#pragma region Shared between apps
World* pWorld = nullptr;
Pathfinder* pPathfinder = nullptr;
Path path;
Tile* pStart = nullptr;
Tile* pGoal = nullptr;

// Flow control
bool genWorld = true;
bool calcPath = true;
bool wholePathDrawn = false;
int currentTile = 0;
#pragma endregion

// These are the only two that differ between apps
SG::ISurface* pSurface = nullptr;
SG::IContext* pContext = nullptr;
#pragma endregion

#pragma region Forward Declarations
void ConsoleApp();
void DirectX11App();
void Win32App();

void Update();
void Render();
void InputTask();

int RegisterExitHandlers();
#pragma endregion

#pragma region Program flow control variables
volatile int appType = CONSOLE_APP;
volatile int prevAppType = -1;
volatile bool bRunning = true;
volatile bool bReceiveInput = true;
volatile bool bSwitchApp = false;
#pragma endregion

int main()
{
	if (RegisterExitHandlers() != EXIT_SUCCESS)
	{
		std::cerr << "Registration of exit handlers failed" << std::endl;
		return EXIT_FAILURE;
	}

	inputThread = std::thread(InputTask);

	pWorld = new World();
	pPathfinder = new Pathfinder();
	pPathfinder->SetWorld(pWorld);

	// This loop waits for and handles app switching
	while (bRunning)
	{
		if (prevAppType != appType)
		{
			prevAppType = appType;

			// Exit current app
			if (appThread.joinable())
			{
				bSwitchApp = true;
				appThread.join();
				bSwitchApp = false;
			}

			// Init and launch the switched to app
			if (appType == CONSOLE_APP)
			{
				appThread = std::thread(ConsoleApp);
			}
			else if (appType == WIN32_APP)
			{
				appThread = std::thread(Win32App);
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	return EXIT_SUCCESS;
}

#pragma region General Flow Functions
void Update()
{
	if (genWorld)
	{
		pWorld->Init(WORLD_TILE_WIDTH, WORLD_TILE_HEIGHT);
		currentTile = 0;
		calcPath = true;
		genWorld = false;
	}

	if (calcPath)
	{
		pStart = pWorld->GetTile(0, 0);
		pWorld->SetTile(0, 0, TILE_TYPE_WALKABLE);
		pStart->graphic = 'S';

		pGoal = pWorld->GetTile(WORLD_TILE_WIDTH - 1, WORLD_TILE_HEIGHT - 1);
		pWorld->SetTile(WORLD_TILE_WIDTH - 1, WORLD_TILE_HEIGHT - 1, TILE_TYPE_WALKABLE);
		pGoal->graphic = 'G';

		calcPath = false;

		if (!pPathfinder->CalculatePath(*pStart, *pGoal, &path))
		{
			genWorld = true;
			calcPath = true;

			return;
		}
	}

	if (currentTile < path.size())
	{
		const Tile* pTile = path.at(currentTile);
		currentTile++;
		if (pTile == pStart || pTile == pGoal)
			return;

		pWorld->GetTile(pTile->GetX(), pTile->GetY())->graphic = '*';
	}

	if (currentTile == path.size())
	{
		wholePathDrawn = true;
	}
}

void Render()
{
	{ // Thread-safe
		std::scoped_lock lock(appTypeChangeMutex);

		if (prevAppType != appType || !bRunning || bSwitchApp)
		{
			return;
		}

		switch (appType)
		{
		case CONSOLE_APP:
		{
			pWorld->Draw(pContext);
			pSurface->Present();
			break;
		}
		case WIN32_APP:
		{
			InvalidateRect(((SG::WinSurface*)pSurface)->GetHWND(), NULL, TRUE);
			break;
		}
		}
	}

	if (wholePathDrawn)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		genWorld = true;
		calcPath = true;
		wholePathDrawn = false;
	}
}

void InputTask()
{
	while (bReceiveInput)
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x0001)
		{
			std::scoped_lock lock(appTypeChangeMutex);
			--appType;
			appType = appType < 0 ? MAX_APPS - 1 : appType;
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
		{
			std::scoped_lock lock(appTypeChangeMutex);
			++appType;
			appType = appType >= MAX_APPS ? 0 : appType;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(5));

		if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
		{
			bRunning = false;
		}
	}
}
#pragma endregion

#pragma region Console App
void InitConsoleApp();
void ReleaseConsoleApp();
void ConsoleApp()
{
	InitConsoleApp();

	while (!bSwitchApp)
	{
		Update();
		Render();

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	ReleaseConsoleApp();
}

void InitConsoleApp()
{
	pWorld->SetTileSize(1);

	pSurface = new SG::ConsoleSurface();
	pSurface->Resize(WORLD_TILE_WIDTH, WORLD_TILE_HEIGHT);

	pContext = new SG::ConsoleContext();
	pContext->Init(pSurface);
}

void ReleaseConsoleApp()
{
	SAFE_DELETE(pContext);
	SAFE_DELETE(pSurface);
}
#pragma endregion

#pragma region DirectX11 App
void DirectX11App()
{
	throw "Not Implemented - DirectX11App";
}
#pragma endregion

#pragma region Win32 App
void InitWin32App();
void ReleaseWin32App();
void Win32App()
{
	InitWin32App();

	// Game loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Update();
			Render();

			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}

		if (bSwitchApp)
		{
			PostQuitMessage(0);
		}
	}

	ReleaseWin32App();
}

void InitWin32App()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	pSurface = new SG::WinSurface();
	SG::WinSurface* pWinSurface = reinterpret_cast<SG::WinSurface*>(pSurface);

	pContext = new SG::WinContext();
	pContext->Init(pSurface);

	pWorld->SetTileSize(16);

	SG::WND_PROC_FUNC pWndProc = [=](HWND hWnd, UINT uMsg, WPARAM w, LPARAM l) -> LRESULT {
		switch (uMsg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			pWinSurface->SetContext(hdc);

			pWorld->Draw(pContext);

			BOOL bRes = EndPaint(hWnd, &ps);
			return 0;
		}
		}

		return DefWindowProc(hWnd, uMsg, w, l);
	};

	pWinSurface->SetWndProc(pWndProc);
	pWinSurface->Init(hInstance, 0, 0, 720, 720);
}

void ReleaseWin32App()
{
	// Did we exit because we X'd the window?
	if (!bSwitchApp)
	{
		// Yes
		exit(EXIT_SUCCESS);
	}

	SAFE_DELETE(pContext);
	SAFE_DELETE(pSurface);
}
#pragma endregion

#pragma region Exit Handling Functions
void AtExitRelease()
{
	if (appThread.joinable())
	{
		bSwitchApp = true;
		appThread.join();
		bSwitchApp = false;
	}

	if (inputThread.joinable())
	{
		bReceiveInput = false;
		inputThread.join();
	}

	SAFE_DELETE(pContext);
	SAFE_DELETE(pSurface);
	SAFE_DELETE(pPathfinder);
	SAFE_DELETE(pWorld);
}

BOOL ConsoleCtrlHandler(DWORD event)
{
	if (event == CTRL_CLOSE_EVENT) {
		exit(EXIT_SUCCESS);
		return TRUE;
	}
	return FALSE;
}

int RegisterExitHandlers()
{
	// Console close handler
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)(ConsoleCtrlHandler), TRUE);

	// Window close handler
	const int regAtExitReleaseResult = std::atexit(AtExitRelease);
	if (regAtExitReleaseResult != 0)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
#pragma endregion