#include <graphics/win32/WinSurface.hpp>

#include <unordered_map>
namespace SG {
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
        int32 x, int32 y,
        int32 w, int32 h,
        const wchar_t* title,
        const wchar_t* className)
    {
        m_hWnd = MakeWindow(hInstance, x, y, w, h, title, className);

        s_WinSurfaces.insert_or_assign(m_hWnd, this);

        ShowWindow(m_hWnd, SW_SHOW);
        UpdateWindow(m_hWnd);
    }

    void WinSurface::Clear()
    {
        throw "Not Implemented - WinSurface::Clear";
    }

    void WinSurface::Present()
    {
        throw "Not Implemented - WinSurface::Present";
    }

    void WinSurface::Resize(int32 width, int32 height)
    {
        throw "Not Implemented - WinSurface::Resize";
    }

    void WinSurface::SetWndProc(WND_PROC_FUNC wndProc)
    {
        m_WndProc = wndProc;
    }

    LRESULT WinSurface::S_WndProc(HWND hWnd, UINT uMsg, WPARAM w, LPARAM l)
    {
        auto it = s_WinSurfaces.find(hWnd);
        if (it != s_WinSurfaces.end())
        {
            WND_PROC_FUNC callback = it->second->m_WndProc;

            if (callback != nullptr)
            {
                return callback(hWnd, uMsg, w, l);
            }
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

    HWND WinSurface::MakeWindow(HINSTANCE hInstance, int32 x, int32 y, int32 w, int32 h, const wchar_t* title, const wchar_t* className)
    {
        WNDCLASS wc = {};
        wc.lpfnWndProc = S_WndProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = className;
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
        RegisterClass(&wc);

        DWORD dwStyle = WS_OVERLAPPEDWINDOW;
        RECT rc;
        SetRect(&rc, 0, 0, w, h);
        AdjustWindowRect(&rc, dwStyle, FALSE);

        HWND hWnd = CreateWindowEx(
            0,
            className,
            title,
            WS_OVERLAPPEDWINDOW,
            x, y,
            rc.right - rc.left, rc.bottom - rc.top,
            NULL,
            NULL,
            hInstance,
            NULL
        );

        if (hWnd == NULL)
        {
            MessageBox(hWnd, L"Failed creating window", L"Fatal Error", MB_OK);
        }

        return hWnd;
    }
}