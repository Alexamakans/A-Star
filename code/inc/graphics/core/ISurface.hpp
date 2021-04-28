#pragma once

namespace SG {
	class ISurface
	{
	public:
		virtual ~ISurface() {}

		virtual void Clear() = 0;
		virtual void Present() = 0;

		virtual void Resize(int width, int height) = 0;
	};
}