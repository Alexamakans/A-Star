#include <World.hpp>

#include <graphics/CharGraphics.hpp>
#include <graphics/CharSurface.hpp>

#include <chrono>
#include <thread>

#include <pathfinding/Pathfinder.hpp>

int main()
{
	uint16 w = 16;
	uint16 h = w;

	CharSurface* pSurface = new CharSurface();
	pSurface->SetSize(w, h);

	CharGraphics* pGraphics = new CharGraphics();
	pGraphics->Init(pSurface);

	World* pWorld = new World();

	Pathfinder* pPathfinder = new Pathfinder();
	pPathfinder->SetWorld(pWorld);

	bool running = true;
	while (running)
	{
		pWorld->Init(w, h);
		pWorld->Draw(pGraphics);

		pWorld->SetTile(0, 0, TILE_TYPE_WALKABLE);
		Tile* pStart = pWorld->GetTile(0, 0);
		pWorld->SetTile(0, 12, TILE_TYPE_WALKABLE);
		Tile* pGoal = pWorld->GetTile(0, 12);

		pGraphics->Draw('S', pStart->GetX(), pStart->GetY());
		pGraphics->Draw('G', pGoal->GetX(), pGoal->GetY());
		pSurface->Present();

		Path path;
		pPathfinder->CalculatePath(*pStart, *pGoal, &path);
		for (const Tile* pTile : path)
		{
			if (pTile == pStart || pTile == pGoal)
				continue;

			pGraphics->Draw('*', pTile->GetX(), pTile->GetY());
			pSurface->Present();

			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}

		pSurface->Present();

		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}

	return 0;
}