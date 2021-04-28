#pragma once

#include <graphics/core/ISurface.hpp>

namespace SG {
	struct ConsoleBackBuffer
	{
		int32 width, height;
		char** ppData;

		ConsoleBackBuffer() : width(), height(), ppData(nullptr) {}
		~ConsoleBackBuffer() { Release(); }
		void Release()
		{
			if (ppData != nullptr)
			{
				for (int x = width - 1; x >= 0; --x)
				{
					delete[] ppData[x];
					ppData[x] = nullptr;
				}

				delete[] ppData;
				ppData = nullptr;
			}
		}
		void Resize(int32 width, int32 height)
		{
			Release();

			this->width = width;
			this->height = height;

			ppData = new char* [width];
			for (int32 x = 0; x < width; ++x)
			{
				ppData[x] = new char[height];
				for (int32 y = 0; y < height; ++y)
				{
					ppData[x][y] = ' ';
				}
			}
		}
	};

	class ConsoleSurface : public ISurface
	{
	public:
		ConsoleSurface();
		virtual ~ConsoleSurface();

		void Release();

		/**
		* Sets the size of the underlying back buffer.
		* In doing this, the back buffer is also cleared.
		*/
		virtual void Resize(int32 width, int32 height) override;

		/**
		* Clears the back buffer.
		*/
		virtual void Clear() override;

		/**
		* Presents the back buffer to the screen.
		*/
		virtual void Present() override;

		/**
		* Returns the back buffer.
		*/
		const ConsoleBackBuffer& GetBackBuffer();

	private:
		ConsoleBackBuffer m_BackBuffer;
	};
}