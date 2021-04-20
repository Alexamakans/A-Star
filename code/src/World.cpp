#include <World.hpp>

#include <time.h>

World::World()
	: m_WorldWidth(0)
	, m_WorldHeight(0)
	, m_ppTiles(nullptr)
{
}

World::~World()
{
	Release();
}

void World::Release()
{
	if (m_ppTiles != nullptr)
	{
		for (int x = m_WorldWidth - 1; x >= 0; --x)
		{
			delete[] m_ppTiles[x];
			m_ppTiles[x] = nullptr;
		}

		delete[] m_ppTiles;
		m_ppTiles = nullptr;
	}

	m_WorldWidth = 0;
	m_WorldHeight = 0;
}

void World::Init(uint16 worldWidth, uint16 worldHeight)
{
	srand(static_cast<uint32>(time(static_cast<time_t*>(NULL))));

	Release();

	m_WorldWidth = worldWidth;
	m_WorldHeight = worldHeight;
	m_ppTiles = new Tile*[worldWidth];
	for (uint16 x = 0; x < worldWidth; ++x)
	{
		m_ppTiles[x] = new Tile[worldHeight];

		for (uint16 y = 0; y < worldHeight; ++y)
		{
			int r = rand() % 30;

			if (r < 5)
				SetTile(x, y, TILE_TYPE_UNWALKABLE);
			else
				SetTile(x, y, TILE_TYPE_WALKABLE);
		}
	}
}


void World::SetTile(uint16 x, uint16 y, TileType type)
{
	m_ppTiles[x][y].type = type;
}

void World::Draw(IGraphics* pGraphics)
{
	for (uint16 x = 0; x < m_WorldWidth; ++x)
	{
		for (uint16 y = 0; y < m_WorldHeight; ++y)
		{
			pGraphics->Draw(sTileGraphics.at(m_ppTiles[x][y].type), x, y);
		}
	}
}