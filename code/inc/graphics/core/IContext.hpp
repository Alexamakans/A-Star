#pragma once

#include <graphics/core/ISurface.hpp>

namespace SG {
	class IContext
	{
	public:
		virtual ~IContext() {}

		// [[ INTERFACE METHODS ]]
		virtual void Init(ISurface* pSurface) = 0;
		virtual void DrawChar(char c, float x, float y) = 0;

		virtual void FillRectangle(float x, float y, float w, float h) = 0;
		virtual void DrawRectangle(float x, float y, float w, float h) = 0;
	};
}