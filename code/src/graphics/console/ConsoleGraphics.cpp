#include <graphics/console/ConsoleContext.hpp>

namespace SG {
	ConsoleContext::ConsoleContext()
		: m_pSurface(nullptr)
	{}

	ConsoleContext::~ConsoleContext()
	{
		m_pSurface = nullptr;
	}

	void ConsoleContext::Init(ISurface* pSurface)
	{
		if (pSurface != nullptr)
		{
			m_pSurface = dynamic_cast<ConsoleSurface*>(pSurface);
			if (m_pSurface == nullptr)
			{
				throw "Unsupported ISurface passed to CharContext::Init. Use CharSurface.";
			}
		}
		else
		{
			m_pSurface = nullptr;
		}
	}

	void ConsoleContext::DrawChar(char c, float x, float y)
	{
		// Flip the indices so we can access a row at a time more easily when outputting to the console.
		m_pSurface->GetBackBuffer().ppData[(int)y][(int)x] = c;
	}

	void ConsoleContext::FillRectangle(float x, float y, float w, float h)
	{
		throw "Not Implemented - ConsoleContext::FillRectangle";
	}

	void ConsoleContext::DrawRectangle(float x, float y, float w, float h)
	{
		throw "Not Implemented - ConsoleContext::DrawRectangle";
	}
}