#include <World.hpp>

#include <graphics/console/ConsoleSurface.hpp>
#include <graphics/console/ConsoleGraphics.hpp>

#include <chrono>
#include <thread>

#include <pathfinding/Pathfinder.hpp>

int main()
{
	uint16 w = 16;
	uint16 h = w;

	ConsoleSurface* pSurface = new ConsoleSurface();
	pSurface->SetSize(w, h);

	ConsoleGraphics* pConsoleGraphics = new ConsoleGraphics();
	pConsoleGraphics->Init(pSurface);

	World* pWorld = new World();

	Pathfinder* pPathfinder = new Pathfinder();
	pPathfinder->SetWorld(pWorld);

	bool running = true;
	while (running)
	{
		pWorld->Init(w, h);
		pWorld->Draw(pConsoleGraphics);

		pWorld->SetTile(0, 0, TILE_TYPE_WALKABLE);
		Tile* pStart = pWorld->GetTile(0, 0);
		pWorld->SetTile(0, 12, TILE_TYPE_WALKABLE);
		Tile* pGoal = pWorld->GetTile(0, 12);

		pConsoleGraphics->Draw('S', pStart->GetX(), pStart->GetY());
		pConsoleGraphics->Draw('G', pGoal->GetX(), pGoal->GetY());

		pSurface->Present();

		Path path;
		pPathfinder->CalculatePath(*pStart, *pGoal, &path);
		for (const Tile* pTile : path)
		{
			if (pTile == pStart || pTile == pGoal)
				continue;

			pConsoleGraphics->Draw('*', pTile->GetX(), pTile->GetY());
			pSurface->Present();

			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}

		pSurface->Present();

		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}

	return 0;
}