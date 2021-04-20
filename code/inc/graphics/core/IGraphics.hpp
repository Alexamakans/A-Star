#pragma once

#include <graphics/core/ISurface.hpp>

class IGraphics
{
public:
	// [[ INTERFACE METHODS ]]
	virtual void Init(ISurface* pSurface) = 0;
	virtual void Draw(char c, int32 x, int32 y) = 0;
};