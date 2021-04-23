#include <graphics/win32/WinGraphics.hpp>

WinGraphics::WinGraphics()
	: m_CurBrush()
	, m_pSurface(nullptr)
{
}

WinGraphics::~WinGraphics()
{
}

void WinGraphics::Init(ISurface* pSurface)
{
	if (pSurface != nullptr)
	{
		m_pSurface = dynamic_cast<WinSurface*>(pSurface);
		if (m_pSurface == nullptr)
		{
			throw "Unsupported ISurface passed to WinGraphics::Init. Use WinSurface.";
		}
	}
	else
	{
		m_pSurface = nullptr;
	}
}

void WinGraphics::DrawChar(char c, float x, float y)
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

	FillRectangle(x, y, 16, 16);
}

void WinGraphics::FillRectangle(float x, float y, float w, float h)
{
	RECT rect;
	rect.left = static_cast<LONG>(x);
	rect.top = static_cast<LONG>(y);
	rect.right = static_cast<LONG>(x + w);
	rect.bottom = static_cast<LONG>(y + h);

	FillRect(m_pSurface->GetContext(), &rect, m_CurBrush);
}

void WinGraphics::DrawRectangle(float x, float y, float w, float h)
{
	Rectangle(
		m_pSurface->GetContext(),
		static_cast<int>(x),
		static_cast<int>(y),
		static_cast<int>(x + w),
		static_cast<int>(y + h));
}

void WinGraphics::SetColor(COLORREF color)
{
	if (m_CurBrush)
	{
		DeleteObject(m_CurBrush);
	}
	m_CurBrush = CreateSolidBrush(color);
}