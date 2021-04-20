#pragma once

#include <graphics/core/IGraphics.hpp>
#include <graphics/CharSurface.hpp>

class ISurface;

class CharGraphics : public IGraphics
{
public:
	CharGraphics();
	virtual ~CharGraphics();

	virtual void Init(ISurface* pSurface) override;
	virtual void Draw(char c, int32 x, int32 y) override;

private:
	CharSurface* m_pSurface;
};