#include <graphics/ConsoleSurface.hpp>

ConsoleSurface::ConsoleSurface()
{
}

ConsoleSurface::~ConsoleSurface()
{
    Release();
}

void ConsoleSurface::Release()
{
    m_BackBuffer.Release();
}

void ConsoleSurface::SetSize(uint16 width, uint16 height)
{
    m_BackBuffer.SetSize(width, height);
}

void ConsoleSurface::Clear()
{
    // unsure of what this should do tbh
    throw "NYI";
}

void ConsoleSurface::Present()
{
    system("cls");

    static std::string output;
    output = "";

    for (uint16 y = 0; y < m_BackBuffer.height; ++y)
    {
        output.append(m_BackBuffer.ppData[y], m_BackBuffer.width);
        output += "\n";
    }

    std::cout << output << std::endl;
}

const ConsoleBackBuffer& ConsoleSurface::GetBackBuffer()
{
    return m_BackBuffer;
}