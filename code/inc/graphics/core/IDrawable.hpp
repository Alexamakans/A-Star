#pragma once

#include <graphics/core/IGraphics.hpp>

class IDrawable
{
public:
	virtual void Draw(IGraphics* pGraphics) = 0;
};