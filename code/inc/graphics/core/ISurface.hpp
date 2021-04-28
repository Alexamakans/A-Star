#pragma once

namespace SG {
	class ISurface
	{
	public:
		virtual ~ISurface() {}

		virtual void Clear() = 0;
		virtual void Present() = 0;

		virtual void Resize(int32 width, int32 height) = 0;
	};
}