#include <apps/ConsoleApp.hpp>

#include <graphics/console/ConsoleSurface.hpp>
#include <graphics/console/ConsoleContext.hpp>

ConsoleApp::ConsoleApp()
{
}

ConsoleApp::~ConsoleApp()
{
}

void ConsoleApp::Init(_In_ World* pWorld, _In_ Pathfinder* pPathfinder, int32 width, int32 height)
{
	if (m_pSurface != nullptr)
		throw "Already initialized";

	pWorld->SetTileSize(1);

	m_pWorld = pWorld;
	m_pPathfinder = pPathfinder;

	m_pSurface = new SG::ConsoleSurface();
	m_pSurface->Resize(pWorld->GetTileSize() * width, pWorld->GetTileSize() * height);

	m_pContext = new SG::ConsoleContext();
	m_pContext->Init(m_pSurface);

}

void ConsoleApp::HandleInput()
{}

void ConsoleApp::Render()
{
	m_pWorld->Draw(m_pContext);
	m_pSurface->Present();

	if (m_bWholePathDrawn)
	{
		m_bGenWorld = true;
		m_bCalcPath = true;
		m_bWholePathDrawn = false;

		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
}

void ConsoleApp::Release()
{
	SAFE_DELETE(m_pContext);
	SAFE_DELETE(m_pSurface);
}