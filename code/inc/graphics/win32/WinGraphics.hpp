#pragma once

#include <Windows.h>

#include <graphics/core/IGraphics.hpp>
#include <graphics/win32/WinSurface.hpp>

class WinGraphics : public IGraphics
{
public:
	WinGraphics();
	virtual ~WinGraphics();

	virtual void Init(ISurface* pSurface) override;
	virtual void DrawChar(char c, float x, float y) override;

	// Inherited via IGraphics
	virtual void FillRectangle(float x, float y, float w, float h) override;
	virtual void DrawRectangle(float x, float y, float w, float h) override;

	void SetColor(COLORREF color);

private:
	WinSurface* m_pSurface;
	HBRUSH m_CurBrush;
};