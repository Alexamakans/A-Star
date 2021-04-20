#pragma once

#include <graphics/core/IDrawable.hpp>

#include <unordered_map>

typedef char TileGraphic;
enum TileType : uint16
{
    TILE_TYPE_UNWALKABLE = 0,
    TILE_TYPE_WALKABLE,
    TILE_TYPE_MAX,
};

enum TileFlag : uint32 {
    TILE_FLAG_NO_FLAGS =        0,
    TILE_FLAG_UNWALKABLE = 1 << 0,
};

typedef std::unordered_map<TileType, TileGraphic> TileGraphicStore;

struct Tile
{
    TileType type = UNWALKABLE_TILE;
};

static TileGraphicStore sTileGraphics {
    { UNWALKABLE_TILE,  'X' },
    { WALKABLE_TILE,    ' ' },
    { GRASS_TILE,       'G' }
};


class World : public IDrawable {
public:
    World();
    virtual ~World();

    void Release();

    void Init(uint16 worldWidth, uint16 worldHeight);

    void SetTile(uint16 x, uint16 y, TileType type);

    virtual void Draw(IGraphics* pGraphics) override;

    // World map data
    Tile** m_ppTiles;

    // World width in tile units.
    uint16 m_WorldWidth;
    // World height in tiles units.
    uint16 m_WorldHeight;
};