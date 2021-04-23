#include <graphics/console/ConsoleGraphics.hpp>

ConsoleGraphics::ConsoleGraphics()
	: m_pSurface(nullptr)
{}

ConsoleGraphics::~ConsoleGraphics()
{
	m_pSurface = nullptr;
}

void ConsoleGraphics::Init(ISurface* pSurface)
{
	if (pSurface != nullptr)
	{
		m_pSurface = dynamic_cast<ConsoleSurface*>(pSurface);
		if (m_pSurface == nullptr)
		{
			throw "Unsupported ISurface passed to CharGraphics::Init. Use CharSurface.";
		}
	}
	else
	{
		m_pSurface = nullptr;
	}
}

void ConsoleGraphics::DrawChar(char c, float x, float y)
{
	// Flip the indices so we can access a row at a time more easily when outputting to the console.
	m_pSurface->GetBackBuffer().ppData[(int) y][(int) x] = c;
}

void ConsoleGraphics::FillRectangle(float x, float y, float w, float h)
{
}

void ConsoleGraphics::DrawRectangle(float x, float y, float w, float h)
{
}
