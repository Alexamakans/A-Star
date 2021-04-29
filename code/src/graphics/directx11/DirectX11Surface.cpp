#include <graphics/directx11/DirectX11Surface.hpp>

namespace SG
{
    static std::unordered_map<HWND, DirectX11Surface*> s_DX11Surfaces;

    DirectX11Surface::DirectX11Surface()
        : m_pDevice(nullptr)
        , m_pContext(nullptr)
        , m_pSwapChain(nullptr)
        , m_pBackBufferRTV(nullptr)

        , m_pDepthBufferTex(nullptr)
        , m_pDepthStencilView(nullptr)
        , m_pDepthStencilState(nullptr)

        , m_pRasterizerState(nullptr)
	{
	}

	DirectX11Surface::~DirectX11Surface()
	{
        s_DX11Surfaces.erase(m_hWnd);

        SAFE_RELEASE(m_pRasterizerState);
        SAFE_RELEASE(m_pDepthStencilState);
        SAFE_RELEASE(m_pDepthStencilView);
        SAFE_RELEASE(m_pDepthBufferTex);
        SAFE_RELEASE(m_pBackBufferRTV);
        SAFE_RELEASE(m_pSwapChain);
        SAFE_RELEASE(m_pContext);
        SAFE_RELEASE(m_pDevice);

        if (m_hWnd)
        {
            DestroyWindow(m_hWnd);
            m_hWnd = NULL;
        }
	}

	void DirectX11Surface::Init(HINSTANCE hInstance, int32 x, int32 y, int32 w, int32 h, const wchar_t* title, const wchar_t* className)
	{
        m_hWnd = MakeWindow(hInstance, x, y, w, h, title, className);

        s_DX11Surfaces.insert_or_assign(m_hWnd, this);

        ShowWindow(m_hWnd, SW_SHOW);
        UpdateWindow(m_hWnd);

        if (!CreateDeviceAndSwapChain())
        {
            return;
        }

        if (!CreateBackBuffer())
        {
            return;
        }

        if (!CreateDepthBuffer())
        {
            return;
        }

        m_pContext->OMSetRenderTargets(1, &m_pBackBufferRTV, m_pDepthStencilView);

        if (!CreateDepthStencilState())
        {
            return;
        }

        m_pContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

        if (!CreateRasterizerState())
        {
            return;
        }

        m_pContext->RSSetState(m_pRasterizerState);

        D3D11_VIEWPORT viewport;
        viewport.Width = (float)m_Width;
        viewport.Height = (float)m_Height;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;

        // Create the viewport.
        m_pContext->RSSetViewports(1, &viewport);
	}

	void DirectX11Surface::Clear()
	{
        m_pContext->ClearRenderTargetView(m_pBackBufferRTV, ClearColor);
        m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void DirectX11Surface::Present()
	{
        m_pSwapChain->Present(0, 0);
	}

	void DirectX11Surface::Resize(int32 width, int32 height)
	{
	}

    int32 DirectX11Surface::GetWidth() const
    {
        return m_Width;
    }

    int32 DirectX11Surface::GetHeight() const
    {
        return m_Height;
    }

    void DirectX11Surface::SetWndProc(WND_PROC_FUNC wndProc)
    {
        m_WndProc = wndProc;
    }
    
    LRESULT CALLBACK DirectX11Surface::S_WndProc(HWND hWnd, UINT uMsg, WPARAM w, LPARAM l)
    {
        auto it = s_DX11Surfaces.find(hWnd);
        if (it != s_DX11Surfaces.end())
        {
            WND_PROC_FUNC callback = it->second->m_WndProc;

            if (callback != nullptr)
            {
                return callback(hWnd, uMsg, w, l);
            }
        }

        return DefWindowProc(hWnd, uMsg, w, l);
    }

    HWND DirectX11Surface::GetHWND() const
    {
        return m_hWnd;
    }

    ID3D11Device* DirectX11Surface::GetDevice()
    {
        return m_pDevice;
    }

    ID3D11DeviceContext* DirectX11Surface::GetContext()
    {
        return m_pContext;
    }
    
    HWND DirectX11Surface::MakeWindow(HINSTANCE hInstance, int32 x, int32 y, int32 w, int32 h, const wchar_t* title, const wchar_t* className)
    {
        m_Width = w;
        m_Height = h;

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

    bool DirectX11Surface::CreateDeviceAndSwapChain()
    {
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Width = m_Width;
        swapChainDesc.BufferDesc.Width = m_Height;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = m_hWnd;
        swapChainDesc.Windowed = TRUE;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;

        D3D_FEATURE_LEVEL pFeatureLevels[1] = {
            D3D_FEATURE_LEVEL_11_1,
        };

        D3D_FEATURE_LEVEL selectedFeatureLevel;

        if (FAILED(D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            pFeatureLevels, 1,
            D3D11_SDK_VERSION,
            &swapChainDesc,
            &m_pSwapChain,
            &m_pDevice,
            &selectedFeatureLevel,
            &m_pContext)))
        {
            MessageBox(m_hWnd, L"Failed creating device and swapchain", L"Fatal error", MB_OK);
            return false;
        }

        return true;
    }
    
    bool DirectX11Surface::CreateBackBuffer()
    {
        ID3D11Texture2D* pTex = nullptr;

        if (FAILED(m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pTex))))
        {
            MessageBox(m_hWnd, L"Failed getting buffer from swapchain", L"Fatal error", MB_OK);
            return false;
        }

        if (FAILED(m_pDevice->CreateRenderTargetView(pTex, NULL, &m_pBackBufferRTV)))
        {
            pTex->Release();
            MessageBox(m_hWnd, L"Failed creating backbuffer RTV", L"Fatal error", MB_OK);
            return false;
        }

        pTex->Release();

        return true;
    }

    bool DirectX11Surface::CreateDepthBuffer()
    {
        D3D11_TEXTURE2D_DESC td;
        ZeroMemory(&td, sizeof(td));
        td.Width = m_Width;
        td.Height = m_Height;
        td.MipLevels = 1;
        td.ArraySize = 1;
        td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        td.SampleDesc.Count = 1;
        td.SampleDesc.Quality = 0;
        td.Usage = D3D11_USAGE_DEFAULT;
        td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        td.CPUAccessFlags = 0;
        td.MiscFlags = 0;

        if (FAILED(m_pDevice->CreateTexture2D(&td, NULL, &m_pDepthBufferTex)))
        {
            MessageBox(m_hWnd, L"Failed creating depth buffer texture", L"Fatal error", MB_OK);
            return false;
        }

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
        ZeroMemory(&dsvDesc, sizeof(dsvDesc));
        dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        dsvDesc.Texture2D.MipSlice = 0;

        if (FAILED(m_pDevice->CreateDepthStencilView(m_pDepthBufferTex, &dsvDesc, &m_pDepthStencilView)))
        {
            MessageBox(m_hWnd, L"Failed creating depth stencil view", L"Fatal error", MB_OK);
            return false;
        }

        return true;
    }

    bool DirectX11Surface::CreateDepthStencilState()
    {
        D3D11_DEPTH_STENCIL_DESC sd;// Initialize the description of the stencil state.
        ZeroMemory(&sd, sizeof(sd));

        sd.DepthEnable = true;
        sd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        sd.DepthFunc = D3D11_COMPARISON_LESS;

        sd.StencilEnable = true;
        sd.StencilReadMask = 0xFF;
        sd.StencilWriteMask = 0xFF;

        sd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        sd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        sd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        sd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        sd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        sd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        sd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        sd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        if (FAILED(m_pDevice->CreateDepthStencilState(&sd, &m_pDepthStencilState)))
        {
            MessageBox(m_hWnd, L"Failed creating depth stencil state", L"Fatal error", MB_OK);
            return false;
        }

        return true;
    }

    bool DirectX11Surface::CreateRasterizerState()
    {
        D3D11_RASTERIZER_DESC rd;
        ZeroMemory(&rd, sizeof(rd));
        rd.AntialiasedLineEnable = false;
        rd.CullMode = D3D11_CULL_BACK;
        rd.DepthBias = 0;
        rd.DepthBiasClamp = 0.0f;
        rd.DepthClipEnable = true;
        rd.FillMode = D3D11_FILL_SOLID;
        rd.FrontCounterClockwise = false;
        rd.MultisampleEnable = false;
        rd.ScissorEnable = false;
        rd.SlopeScaledDepthBias = 0.0f;

        if (FAILED(m_pDevice->CreateRasterizerState(&rd, &m_pRasterizerState)))
        {
            MessageBox(m_hWnd, L"Failed creating rasterizer state", L"Fatal error", MB_OK);
            return false;
        }

        return true;
    }
}