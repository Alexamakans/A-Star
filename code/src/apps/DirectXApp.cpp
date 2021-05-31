#include <apps/DirectXApp.hpp>

#include <graphics/directx11/DirectX11Surface.hpp>
#include <graphics/directx11/DirectX11Context.hpp>

#include <Windows.h>

DirectXApp::DirectXApp()
{
}

DirectXApp::~DirectXApp()
{
	Release();
}

void DirectXApp::Init(World* pWorld, Pathfinder* pPathfinder, int32 width, int32 height)
{
	if (m_pSurface != nullptr)
		throw "Already initialized";

	pWorld->SetTileSize(TILE_SIZE);

	m_pWorld = pWorld;
	m_pPathfinder = pPathfinder;

	m_pSurface = new SG::DirectX11Surface();
	SG::DirectX11Surface* pDXSurface = reinterpret_cast<SG::DirectX11Surface*>(m_pSurface);
	SG::WND_PROC_FUNC pWndProc = [=](HWND hWnd, UINT uMsg, WPARAM w, LPARAM l) -> LRESULT {
		switch (uMsg)
		{
		case WM_CLOSE:
		{
			exit(EXIT_SUCCESS);
			return 0;
		}
		}

		return DefWindowProc(hWnd, uMsg, w, l);
	};
	pDXSurface->Init(GetModuleHandle(NULL), 0, 0,
		pWorld->GetTileSize() * width,
		pWorld->GetTileSize() * height);

	m_pContext = new SG::DirectX11Context();
	m_pContext->Init(m_pSurface);

	pDXSurface->SetWndProc(pWndProc);
}

void DirectXApp::Release()
{
	SAFE_DELETE(m_pContext);
	SAFE_DELETE(m_pSurface);
}

void DirectXApp::HandleInput()
{
	static MSG msg;
	while (PeekMessage(&msg, ((SG::DirectX11Surface*)m_pSurface)->GetHWND(), 0, 0, PM_REMOVE) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void DirectXApp::Render()
{
	m_pWorld->Draw(m_pContext);
	((SG::DirectX11Context*)m_pContext)->GetRect().DrawInstances();

	m_pSurface->Present();
	m_pSurface->Clear();

	if (m_bWholePathDrawn) {
		m_bWholePathDrawn = false;
		m_bGenWorld = true;
		m_bCalcPath = true;
		m_CurrentTile = 0;

		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
}
