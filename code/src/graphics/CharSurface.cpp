#include <graphics/CharSurface.hpp>

CharSurface::CharSurface()
{
}

CharSurface::~CharSurface()
{
    Release();
}

void CharSurface::Release()
{
    m_BackBuffer.Release();
}

void CharSurface::SetSize(uint16 width, uint16 height)
{
    m_BackBuffer.SetSize(width, height);
}

void CharSurface::Clear()
{
    system("cls");
}

void CharSurface::Present()
{
    static std::string output;
    output = "";

    for (uint16 y = 0; y < m_BackBuffer.height; ++y)
    {
        output.append(m_BackBuffer.ppData[y], m_BackBuffer.width);
        output += "\n";
    }

    std::cout << output << std::endl;
}

const CharBackBuffer& CharSurface::GetBackBuffer()
{
    return m_BackBuffer;
}
