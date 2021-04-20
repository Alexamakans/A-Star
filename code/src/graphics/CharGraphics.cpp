#include <graphics/CharGraphics.hpp>

CharGraphics::CharGraphics()
	: m_pSurface(nullptr)
{}

CharGraphics::~CharGraphics()
{
	m_pSurface = nullptr;
}

void CharGraphics::Init(ISurface* pSurface)
{
	if (pSurface != nullptr)
	{
		m_pSurface = dynamic_cast<CharSurface*>(pSurface);
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

void CharGraphics::Draw(char c, int32 x, int32 y)
{
	// Flip the indices so we can access a row at a time more easily when outputting to the console.
	m_pSurface->GetBackBuffer().ppData[y][x] = c;
}