#pragma once

#include <graphics/core/ISurface.hpp>

struct CharBackBuffer
{
	uint16 width, height;
	char** ppData = nullptr;

	~CharBackBuffer() { Release(); }
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
	void SetSize(uint16 width, uint16 height)
	{
		Release();

		this->width = width;
		this->height = height;

		ppData = new char* [width];
		for (uint16 x = 0; x < width; ++x)
		{
			ppData[x] = new char[height];
			for (uint16 y = 0; y < height; ++y)
			{
				ppData[x][y] = ' ';
			}
		}
	}
};

class CharSurface : public ISurface
{
public:
	CharSurface();
	virtual ~CharSurface();

	void Release();

	/**
	* Sets the size of the underlying back buffer.
	* In doing this, the back buffer is also cleared.
	*/
	virtual void SetSize(uint16 width, uint16 height);

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
	const CharBackBuffer& GetBackBuffer();

private:
	CharBackBuffer m_BackBuffer;
};