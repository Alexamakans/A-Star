#include <chrono>
#include <thread>

#include <World.hpp>
#include <pathfinding/Pathfinder.hpp>

#include <Windows.h> // GetAsyncKeyState

// Console
#include <graphics/console/ConsoleSurface.hpp>
#include <graphics/console/ConsoleGraphics.hpp>

// DirectX11

// Win32
#include <graphics/win32/WinSurface.hpp>
#include <graphics/win32/WinGraphics.hpp>

void ConsoleApp();
void DirectX11App();
void Win32App();

constexpr int CONSOLE_APP = 0;
constexpr int WIN32_APP = 1;
constexpr int MAX_APPS = 2;

void InputTask(volatile int* appType, volatile bool* bRunning)
{
	while (*bRunning)
	{

		if (GetAsyncKeyState(VK_LEFT) & 0x0001)
		{
			--*appType;
			*appType = *appType < 0 ? MAX_APPS - 1 : *appType;
		}
		
		if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
		{
			++*appType;
			*appType = *appType >= MAX_APPS ? 0 : *appType;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(5));

		if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
		{
			*bRunning = false;
		}
	}
}

volatile bool bSwitchApp = false;

int main()
{
	volatile int appType = CONSOLE_APP;
	int prevAppType = -1;
	volatile bool bRunning = true;

	std::thread inputThread(InputTask, &appType, &bRunning);

	std::thread appThread;

	while (bRunning)
	{
		if (prevAppType != appType)
		{
			if (appThread.joinable())
			{
				bSwitchApp = true;
				appThread.join();
				bSwitchApp = false;
			}

			prevAppType = appType;
			if (appType == CONSOLE_APP)
			{
				appThread = std::thread(ConsoleApp);
			}
			else if (appType == WIN32_APP)
			{
				appThread = std::thread(Win32App);
			}
		}

		if (!bRunning && inputThread.joinable())
		{
			inputThread.join();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}

	return 0;
}

void ConsoleApp()
{
	uint16 w = 8;
	uint16 h = w;

	ConsoleSurface* pSurface = new ConsoleSurface();
	pSurface->SetSize(w, h);

	ConsoleGraphics* pGraphics = new ConsoleGraphics();
	pGraphics->Init(pSurface);

	World* pWorld = new World();

	Pathfinder* pPathfinder = new Pathfinder();
	pPathfinder->SetWorld(pWorld);

	while (!bSwitchApp)
	{
		pWorld->Init(w, h);
		pWorld->Draw(pGraphics);

		pWorld->SetTile(1, 1, TILE_TYPE_WALKABLE);
		Tile* pStart = pWorld->GetTile(0, 0);
		pWorld->SetTile(w - 1, h - 1, TILE_TYPE_WALKABLE);
		Tile* pGoal = pWorld->GetTile(w - 1, h - 1);

		pGraphics->DrawChar('S', static_cast<float>(pStart->GetX()), static_cast<float>(pStart->GetY()));
		pGraphics->DrawChar('G', static_cast<float>(pGoal->GetX()), static_cast<float>(pGoal->GetY()));

		pSurface->Present();

		Path path;
		pPathfinder->CalculatePath(*pStart, *pGoal, &path);
		for (const Tile* pTile : path)
		{
			if (pTile == pStart || pTile == pGoal)
				continue;

			pGraphics->DrawChar('*', static_cast<float>(pTile->GetX()), static_cast<float>(pTile->GetY()));
			pSurface->Present();

			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}

		pSurface->Present();

		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}

	delete pPathfinder;
	delete pWorld;
	delete pGraphics;
	delete pSurface;
}

void DirectX11App()
{

}

void Win32App()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	WinSurface* pSurface = new WinSurface();

	WinGraphics* pGraphics = new WinGraphics();
	pGraphics->Init(pSurface);

	World* pWorld = new World();
	pWorld->SetTileSize(16);

	Pathfinder* pPathfinder = new Pathfinder();
	pPathfinder->SetWorld(pWorld);

	WND_PROC_FUNC pWndProc = [&](HWND hWnd, UINT uMsg, WPARAM w, LPARAM l) -> LRESULT {
		switch (uMsg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			pSurface->SetContext(hdc);
			//pSurface->Clear();

			if (pWorld)
				pWorld->Draw(pGraphics);

			BOOL bRes = EndPaint(hWnd, &ps);
			return 0;
		}
		}

		return DefWindowProc(hWnd, uMsg, w, l);
	};

	const int S = 64;

	pSurface->SetWndProc(pWndProc);
	pSurface->Init(hInstance, 0, 0, 900, 900);
	//pSurface->Init(hInstance, 0, 0, S, S);

	int currentTile = 0;
	bool genWorld = true;
	bool calcPath = true;
	Tile* pStart = nullptr;
	Tile* pGoal = nullptr;
	Path path;

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
			if (genWorld)
			{
				pWorld->Init(S, S);
				currentTile = 0;
				calcPath = true;
				genWorld = false;
			}

			if (calcPath)
			{
				pStart = pWorld->GetTile(1, 1);
				pWorld->SetTile(1, 1, TILE_TYPE_WALKABLE);
				pStart->graphic = 'S';

				pGoal = pWorld->GetTile(S - 1, S - 1);
				pWorld->SetTile(S - 1, S - 1, TILE_TYPE_WALKABLE);
				pGoal->graphic = 'G';

				calcPath = false;

				if (!pPathfinder->CalculatePath(*pStart, *pGoal, &path))
				{
					genWorld = true;
					calcPath = true;
					continue;
				}
			}

			if (currentTile < path.size())
			{
				const Tile* pTile = path.at(currentTile);
				currentTile++;
				if (pTile == pStart || pTile == pGoal)
					continue;

				pWorld->GetTile(pTile->GetX(), pTile->GetY())->graphic = '*';
			}

			InvalidateRect(pSurface->GetHWND(), 0, TRUE);
			std::this_thread::sleep_for(std::chrono::milliseconds(50));

			if (currentTile == path.size())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				genWorld = true;
				calcPath = true;
			}
		}

		if (bSwitchApp)
		{
			PostQuitMessage(0);
		}
	}

	delete pPathfinder;
	delete pWorld;
	delete pGraphics;
	delete pSurface;
}