#pragma once

#include <graphics/core/ISurface.hpp>

class IGraphics
{
public:

	// [[ INTERFACE METHODS ]]
	virtual void Init(ISurface* pSurface) = 0;
	virtual void Draw(char c, uint16 x, uint16 y) = 0;
};