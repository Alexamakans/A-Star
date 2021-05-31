#pragma once

#include <Windows.h>

#include <graphics/core/ISurface.hpp>

#include <functional>
namespace SG {
	typedef std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> WND_PROC_FUNC;

	class WinSurface : public ISurface
	{
	public:
		WinSurface();
		virtual ~WinSurface();

		void Init(
			HINSTANCE hInstance,
			int32 x, int32 y,
			int32 w, int32 h,
			const wchar_t* title = L"Default Window Title",
			const wchar_t* className = L"DefaultWindowClassName");

		// Inherited via ISurface
		virtual void Clear() override;

		virtual void Present() override;

		virtual void Resize(int32 width, int32 height) override;

		void SetWndProc(WND_PROC_FUNC wndProc);

		WND_PROC_FUNC m_WndProc;

		static LRESULT CALLBACK S_WndProc(HWND hWnd, UINT uMsg, WPARAM w, LPARAM l);

		HDC GetContext() const;
		void SetContext(HDC context);

		void SetClearColor(COLORREF clearColor);

		HWND GetHWND() const;

	private:
		HWND MakeWindow(
			HINSTANCE hInstance,
			int32 x, int32 y,
			int32 w, int32 h,
			const wchar_t* title,
			const wchar_t* className);

	private:
		HDC m_Context;
		HWND m_hWnd;

		COLORREF m_ClearColor;
	};
}