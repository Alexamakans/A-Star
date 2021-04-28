#include <graphics/console/ConsoleSurface.hpp>

namespace SG {
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

    void ConsoleSurface::Resize(int32 width, int32 height)
    {
        m_BackBuffer.Resize(width, height);
    }

    void ConsoleSurface::Clear()
    {
        throw "Not Implemented - ConsoleSurface::Clear";
    }

    void ConsoleSurface::Present()
    {
        system("cls");

        static std::string output;
        output = "";

        for (int32 y = 0; y < m_BackBuffer.height; ++y)
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
}