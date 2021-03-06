#include <World.hpp>

#include <time.h>

World::World()
	: m_WorldWidth(0)
	, m_WorldHeight(0)
	, m_ppTiles(nullptr)
	, m_TileSize(1)
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
		for (int32 x = m_WorldWidth - 1; x >= 0; --x)
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

void World::Init(int32 worldWidth, int32 worldHeight)
{
	srand(static_cast<uint32>(time(static_cast<time_t*>(NULL))));

	Release();

	m_WorldWidth = worldWidth;
	m_WorldHeight = worldHeight;
	m_ppTiles = new Tile*[worldWidth];
	for (int32 x = 0; x < worldWidth; ++x)
	{
		m_ppTiles[x] = new Tile[worldHeight];

		for (int32 y = 0; y < worldHeight; ++y)
		{
			int32 r = rand() % 30;

			if (r < 9)
				SetTile(x, y, TILE_TYPE_UNWALKABLE);
			else
				SetTile(x, y, TILE_TYPE_WALKABLE);
		}
	}
}

void World::SetTile(int32 x, int32 y, TileType type)
{
	m_ppTiles[x][y].x = x;
	m_ppTiles[x][y].y = y;

	m_ppTiles[x][y].type = type;
	m_ppTiles[x][y].flags = sTileFlags.at(type);
	m_ppTiles[x][y].graphic = sTileGraphics.at(type);
}

Tile* World::GetTile(int32 x, int32 y) const
{
	if (x < 0 || y < 0 || x >= m_WorldWidth || y >= m_WorldHeight)
		return nullptr;
	return &m_ppTiles[x][y];
}

void World::SetTileSize(int32 tileSize)
{
	m_TileSize = tileSize;
}

int32 World::GetTileSize() const
{
	return m_TileSize;
}

void World::Draw(SG::IContext* pContext)
{
	for (int32 x = 0; x < m_WorldWidth; ++x)
	{
		for (int32 y = 0; y < m_WorldHeight; ++y)
		{
			pContext->DrawChar(m_ppTiles[x][y].graphic,
				static_cast<float>(x * m_TileSize),
				static_cast<float>(y * m_TileSize)
			);
		}
	}
}