#include <apps/WinApp.hpp>

#include <graphics/win32/WinSurface.hpp>
#include <graphics/win32/WinContext.hpp>

#include <Windows.h>

WinApp::WinApp()
{
}

WinApp::~WinApp()
{
}

void WinApp::Init(World* pWorld, Pathfinder* pPathfinder, int32 width, int32 height)
{
	if (m_pSurface != nullptr)
		throw "Already initialized";

	pWorld->SetTileSize(4);

	m_pWorld = pWorld;
	m_pPathfinder = pPathfinder;

	HINSTANCE hInstance = GetModuleHandle(NULL);

	m_pSurface = new SG::WinSurface();

	m_pContext = new SG::WinContext();
	m_pContext->Init(m_pSurface);

	SG::WinSurface* pWinSurface = reinterpret_cast<SG::WinSurface*>(m_pSurface);
	SG::WND_PROC_FUNC pWndProc = [=](HWND hWnd, UINT uMsg, WPARAM w, LPARAM l) -> LRESULT {
		switch (uMsg)
		{
		case WM_CLOSE:
		{
			exit(EXIT_SUCCESS);
			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			pWinSurface->SetContext(hdc);

			// OnRender Start
			pWorld->Draw(m_pContext);
			// OnRender End

			BOOL bRes = EndPaint(hWnd, &ps);
			return 0;
		}
		}

		return DefWindowProc(hWnd, uMsg, w, l);
	};

	pWinSurface->SetWndProc(pWndProc);
	pWinSurface->Init(hInstance, 0, 0, pWorld->GetTileSize() * width, pWorld->GetTileSize() * height);
}

void WinApp::HandleInput()
{
	static MSG msg;
	while (PeekMessage(&msg, ((SG::WinSurface*)m_pSurface)->GetHWND(), 0, 0, PM_REMOVE) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void WinApp::Render()
{
	if (m_CurrentTile <= 1)
	{
		InvalidateRect(((SG::WinSurface*)m_pSurface)->GetHWND(), NULL, TRUE);
	}
	else if (m_CurrentTile != m_Path.size())
	{
		const Tile* pTile = m_Path.at(m_CurrentTile);
		int x = pTile->GetX() * m_pWorld->GetTileSize();
		int y = pTile->GetY() * m_pWorld->GetTileSize();
		RECT rc;
		SetRect(&rc, x - 8, y - 8, x + 8, y + 8);
		InvalidateRect(((SG::WinSurface*)m_pSurface)->GetHWND(), &rc, FALSE);
	} else {
		m_bWholePathDrawn = false;
		m_bGenWorld = true;
		m_bCalcPath = true;
		m_CurrentTile = 0;

		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
}

void WinApp::Release()
{
	SAFE_DELETE(m_pContext);
	SAFE_DELETE(m_pSurface);
}