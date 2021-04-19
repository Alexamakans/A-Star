#include <World.hpp>

#include <graphics/CharGraphics.hpp>
#include <graphics/CharSurface.hpp>

#include <chrono>
#include <thread>

int main()
{
	uint16 w = 120;
	uint16 h = w;

	CharSurface* pSurface = new CharSurface();
	pSurface->SetSize(w, h);
	pSurface->Clear();

	CharGraphics* pGraphics = new CharGraphics();
	pGraphics->Init(pSurface);

	World* pWorld = new World();


	bool running = true;
	while (running)
	{
		pWorld->Init(w, h);
		pWorld->Draw(pGraphics);

		pSurface->Clear();
		pSurface->Present();

		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}

	return 0;
}