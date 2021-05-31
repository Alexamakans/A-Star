#pragma once

#include <graphics/core/ISurface.hpp>

#include <functional>

#include <Windows.h>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

namespace SG {
	typedef std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> WND_PROC_FUNC;

	class DirectX11Surface : public SG::ISurface
	{
	public:
		DirectX11Surface();
		virtual ~DirectX11Surface();

		void Init(
			HINSTANCE hInstance,
			int32 x, int32 y,
			int32 w, int32 h,
			const wchar_t* title = L"Default DX11 Window Title",
			const wchar_t* className = L"DefaultDX11WindowClassName");

		virtual void Clear() override;
		virtual void Present() override;
		virtual void Resize(int32 width, int32 height) override;

		int32 GetWidth() const;
		int32 GetHeight() const;

		void SetWndProc(WND_PROC_FUNC wndProc);

		WND_PROC_FUNC m_WndProc;

		static LRESULT CALLBACK S_WndProc(HWND hWnd, UINT uMsg, WPARAM w, LPARAM l);

		HWND GetHWND() const;

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContext();

	private:
		HWND MakeWindow(HINSTANCE hInstance, int32 x, int32 y, int32 w, int32 h, const wchar_t* title, const wchar_t* className);;
		bool CreateDeviceAndSwapChain();
		bool CreateBackBuffer();
		bool CreateDepthBuffer();
		bool CreateDepthStencilState();
		bool CreateRasterizerState();

	private:
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pContext;

		IDXGISwapChain* m_pSwapChain;
		ID3D11RenderTargetView* m_pBackBufferRTV;

		ID3D11Texture2D* m_pDepthBufferTex;
		ID3D11DepthStencilView* m_pDepthStencilView;

		ID3D11DepthStencilState* m_pDepthStencilState;

		ID3D11RasterizerState* m_pRasterizerState;

	private:
		HWND m_hWnd;
		FLOAT ClearColor[4] = { 0.0f, 0.5f, 0.7f, 1.0f };
		int32 m_Width;
		int32 m_Height;
	};
}