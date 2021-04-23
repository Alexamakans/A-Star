#pragma once

#include <Windows.h>

#include <graphics/core/ISurface.hpp>

#include <functional>

typedef std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> WND_PROC_FUNC;

class WinSurface : public ISurface
{
public:
	WinSurface();
	virtual ~WinSurface();

	void Init(
		HINSTANCE hInstance,
		int x, int y,
		int w, int h,
		const wchar_t* title = L"Default Window Title",
		const wchar_t* className = L"DefaultWindowClassName");

	// Inherited via ISurface
	virtual void Clear() override;

	virtual void Present() override;

	void SetWndProc(WND_PROC_FUNC wndProc);

	WND_PROC_FUNC m_WndProc;

	static LRESULT CALLBACK S_WndProc(HWND hWnd, UINT msg, WPARAM w, LPARAM l);

	HDC GetContext() const;
	void SetContext(HDC context);

	void SetClearColor(COLORREF clearColor);

	HWND GetHWND() const;

private:
	HDC m_Context;
	HWND m_hWnd;

	COLORREF m_ClearColor;
};