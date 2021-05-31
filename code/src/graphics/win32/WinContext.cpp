#include <graphics/win32/WinContext.hpp>

namespace SG {
	WinContext::WinContext()
		: m_CurBrush(NULL)
		, m_pSurface(nullptr)
	{
	}

	WinContext::~WinContext()
	{
		if (m_CurBrush)
		{
			DeleteObject(m_CurBrush);
			m_CurBrush = NULL;
		}
	}

	void WinContext::Init(ISurface* pSurface)
	{
		if (pSurface != nullptr)
		{
			m_pSurface = dynamic_cast<WinSurface*>(pSurface);
			if (m_pSurface == nullptr)
			{
				throw "Unsupported ISurface passed to WinContext::Init. Use WinSurface.";
			}
		}
		else
		{
			m_pSurface = nullptr;
		}
	}

	void WinContext::DrawChar(char c, float x, float y)
	{
		if (c == ' ')
			return;

		if (c == 'G')
		{
			SetColor(RGB(0, 255, 0));
		}
		else if (c == 'S')
		{
			SetColor(RGB(0, 0, 255));
		}
		else if (c == '*')
		{
			SetColor(RGB(0, 100, 100));
		}
		else
		{
			SetColor(RGB(0, 0, 0));
		}

		FillRectangle(x, y, TILE_SIZE, TILE_SIZE);
	}

	void WinContext::FillRectangle(float x, float y, float w, float h)
	{
		RECT rect;
		rect.left = static_cast<LONG>(x);
		rect.top = static_cast<LONG>(y);
		rect.right = static_cast<LONG>(x + w);
		rect.bottom = static_cast<LONG>(y + h);

		FillRect(m_pSurface->GetContext(), &rect, m_CurBrush);
	}

	void WinContext::DrawRectangle(float x, float y, float w, float h)
	{
		Rectangle(
			m_pSurface->GetContext(),
			static_cast<int32>(x),
			static_cast<int32>(y),
			static_cast<int32>(x + w),
			static_cast<int32>(y + h));
	}

	void WinContext::SetColor(COLORREF color)
	{
		if (m_CurBrush)
		{
			DeleteObject(m_CurBrush);
		}
		m_CurBrush = CreateSolidBrush(color);
	}
}