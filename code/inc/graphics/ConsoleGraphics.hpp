#pragma once

#include <graphics/core/IGraphics.hpp>
#include <graphics/ConsoleSurface.hpp>

class ISurface;

class ConsoleGraphics : public IGraphics
{
public:
	ConsoleGraphics();
	virtual ~ConsoleGraphics();

	virtual void Init(ISurface* pSurface) override;
	virtual void Draw(char c, int32 x, int32 y) override;

private:
	ConsoleSurface* m_pSurface;
};