#pragma once

#include <graphics/core/IContext.hpp>

namespace SG {
	class IDrawable
	{
	public:
		virtual ~IDrawable() {}

		virtual void Draw(IContext* pContext) = 0;
	};
}