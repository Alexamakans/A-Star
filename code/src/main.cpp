#pragma region Headers
#include <chrono>
#include <thread>
#include <mutex>

#include <World.hpp>
#include <pathfinding/Pathfinder.hpp>

// Apps
#include <apps/ConsoleApp.hpp>
#include <apps/WinApp.hpp>
#include <apps/DirectXApp.hpp>

#include <Windows.h> // GetAsyncKeyState
#pragma endregion

IApp* g_pApp = nullptr;

#pragma region Multithreading
std::thread inputThread;
std::mutex g_Mutex;
#pragma endregion

#pragma region Constants
constexpr int32 CONSOLE_APP = 0;
constexpr int32 WIN32_APP = 1;
constexpr int32 DX11_APP = 2;
constexpr int32 MAX_APPS = 3;
#pragma endregion

#pragma region Program flow control variables
volatile int32 appType = CONSOLE_APP;
volatile bool bRunning = true;
volatile bool bReceiveInput = true;
#pragma endregion

#pragma region Forward Declarations
void InputTask();

int32 RegisterExitHandlers();
#pragma endregion

int32 main()
{
	if (RegisterExitHandlers() != EXIT_SUCCESS)
	{
		std::cerr << "Registration of exit handlers failed" << std::endl;
		return EXIT_FAILURE;
	}

	inputThread = std::thread(InputTask);

	World* pWorld = new World();
	Pathfinder* pPathfinder = new Pathfinder();
	pPathfinder->SetWorld(pWorld);

	appType = CONSOLE_APP;
	g_pApp = new ConsoleApp();
	g_pApp->Init(pWorld, pPathfinder, WORLD_TILE_WIDTH, WORLD_TILE_HEIGHT);

	while (bRunning)
	{
		g_pApp->Start();

		{
			std::scoped_lock lock(g_Mutex);
			IApp* pNextApp = nullptr;

			// Init and launch the switched to app
			if (appType == CONSOLE_APP)
			{
				pNextApp = new ConsoleApp();
				pNextApp->Init(pWorld, pPathfinder, WORLD_TILE_WIDTH, WORLD_TILE_HEIGHT);
			}
			else if (appType == WIN32_APP)
			{
				pNextApp = new WinApp();
				pNextApp->Init(pWorld, pPathfinder, WORLD_TILE_WIDTH, WORLD_TILE_HEIGHT);
			}
			else if (appType == DX11_APP)
			{
				pNextApp = new DirectXApp();
				pNextApp->Init(pWorld, pPathfinder, WORLD_TILE_WIDTH, WORLD_TILE_HEIGHT);
			}

			g_pApp->CopyState(*pNextApp);
			delete g_pApp;
			g_pApp = pNextApp;
		}
	}

	SAFE_DELETE(g_pApp);
	SAFE_DELETE(pPathfinder);
	SAFE_DELETE(pWorld);

	return EXIT_SUCCESS;
}

void InputTask()
{
	while (bReceiveInput)
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x0001)
		{
			std::scoped_lock lock(g_Mutex);
			--appType;
			appType = appType < 0 ? MAX_APPS - 1 : appType;
			g_pApp->Stop();
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
		{
			std::scoped_lock lock(g_Mutex);
			++appType;
			appType = appType >= MAX_APPS ? 0 : appType;
			g_pApp->Stop();
		}

		if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
		{
			std::scoped_lock lock(g_Mutex);
			bRunning = false;
			g_pApp->Stop();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

#pragma region Exit Handlers
void AtExitRelease()
{
	if (inputThread.joinable())
	{
		bReceiveInput = false;
		inputThread.join();
	}

	SAFE_DELETE(g_pApp);
}

BOOL ConsoleCtrlHandler(DWORD event)
{
	if (event == CTRL_CLOSE_EVENT) {
		exit(EXIT_SUCCESS);
		return TRUE;
	}
	return FALSE;
}

int32 RegisterExitHandlers()
{
	// Console close handler
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)(ConsoleCtrlHandler), TRUE);

	// Window close handler
	const int32 regAtExitReleaseResult = std::atexit(AtExitRelease);
	if (regAtExitReleaseResult != 0)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
#pragma endregion
