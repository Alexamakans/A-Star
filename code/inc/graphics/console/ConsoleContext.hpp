#pragma once

#include <graphics/core/IContext.hpp>
#include <graphics/console/ConsoleSurface.hpp>

namespace SG {
	class ISurface;

	class ConsoleContext : public IContext
	{
	public:
		ConsoleContext();
		virtual ~ConsoleContext();

		virtual void Init(ISurface* pSurface) override;
		virtual void DrawChar(char c, float x, float y) override;

		// Inherited via IContext
		virtual void FillRectangle(float x, float y, float w, float h) override;
		virtual void DrawRectangle(float x, float y, float w, float h) override;

	private:
		ConsoleSurface* m_pSurface;
	};
}