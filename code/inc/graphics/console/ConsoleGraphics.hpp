#pragma once

#include <graphics/core/IGraphics.hpp>
#include <graphics/console/ConsoleSurface.hpp>

class ISurface;

class ConsoleGraphics : public IGraphics
{
public:
	ConsoleGraphics();
	virtual ~ConsoleGraphics();

	virtual void Init(ISurface* pSurface) override;
	virtual void DrawChar(char c, float x, float y) override;

	// Inherited via IGraphics
	virtual void FillRectangle(float x, float y, float w, float h) override;
	virtual void DrawRectangle(float x, float y, float w, float h) override;

private:
	ConsoleSurface* m_pSurface;
};