#include <graphics/win32/WinSurface.hpp>

#include <unordered_map>

static std::unordered_map<HWND, WinSurface*> s_WinSurfaces;

WinSurface::WinSurface()
    : m_ClearColor(0x00404040)
    , m_Context()
    , m_hWnd()
    , m_WndProc()
{}

WinSurface::~WinSurface()
{
    s_WinSurfaces.erase(m_hWnd);

    if (m_Context)
    {
        // ReleaseDC(m_hWnd, m_Context);
        m_Context = NULL;
    }

    if (m_hWnd)
    {
        DestroyWindow(m_hWnd);
        m_hWnd = NULL;
    }
}

void WinSurface::Init(
    HINSTANCE hInstance,
    int x, int y,
    int w, int h,
    const wchar_t* title,
    const wchar_t* className)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = S_WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    RegisterClass(&wc);

    m_hWnd = CreateWindowEx(
        0,
        className,
        title,
        WS_OVERLAPPEDWINDOW,
        x, y,
        w, h,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (m_hWnd == NULL)
    {
        MessageBox(m_hWnd, L"Failed creating window", L"Fatal Error", MB_OK);
        return;
    }

    s_WinSurfaces.insert_or_assign(m_hWnd, this);

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
}

void WinSurface::Clear()
{
}

void WinSurface::Present()
{
    // Not gonna implement until I make use of a back buffer
}

void WinSurface::SetWndProc(WND_PROC_FUNC wndProc)
{
    m_WndProc = wndProc;
}

LRESULT WinSurface::S_WndProc(HWND hWnd, UINT uMsg, WPARAM w, LPARAM l)
{
    auto it = s_WinSurfaces.find(hWnd);

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    if (it != s_WinSurfaces.end())
    {
        WND_PROC_FUNC callback = it->second->m_WndProc;
        return callback(hWnd, uMsg, w, l);
    }

    return DefWindowProc(hWnd, uMsg, w, l);
}

HDC WinSurface::GetContext() const
{
    return m_Context;
}

void WinSurface::SetContext(HDC context)
{
    m_Context = context;
}

void WinSurface::SetClearColor(COLORREF clearColor)
{
    m_ClearColor = clearColor;
}

HWND WinSurface::GetHWND() const
{
    return m_hWnd;
}
