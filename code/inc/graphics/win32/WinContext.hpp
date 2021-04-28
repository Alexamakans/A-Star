#pragma once

#include <Windows.h>

#include <graphics/core/IContext.hpp>
#include <graphics/win32/WinSurface.hpp>

namespace SG {
	class WinContext : public IContext
	{
	public:
		WinContext();
		virtual ~WinContext();

		virtual void Init(ISurface* pSurface) override;
		virtual void DrawChar(char c, float x, float y) override;

		// Inherited via IContext
		virtual void FillRectangle(float x, float y, float w, float h) override;
		virtual void DrawRectangle(float x, float y, float w, float h) override;

		void SetColor(COLORREF color);

	private:
		WinSurface* m_pSurface;
		HBRUSH m_CurBrush;
	};
}