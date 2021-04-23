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
typedef std::unordered_map<TileType, TileFlag> TileFlagStore;
static TileGraphicStore sTileGraphics{
    { TILE_TYPE_UNWALKABLE,  'X' },
    { TILE_TYPE_WALKABLE,    ' ' },
};

static TileFlagStore sTileFlags{
    { TILE_TYPE_UNWALKABLE,     TILE_FLAG_UNWALKABLE    },
    { TILE_TYPE_WALKABLE,       TILE_FLAG_NO_FLAGS      },
};

struct Tile
{
public:
    int32 GetX() const { return x; }
    int32 GetY() const { return y; }

    TileType type = TILE_TYPE_UNWALKABLE;
    TileGraphic graphic = sTileGraphics.at(type);
    uint32 flags = TILE_FLAG_UNWALKABLE;

private:
    uint16 x, y;
    friend class World;
};

class World : public IDrawable {
public:
    World();
    virtual ~World();

    void Release();

    void Init(uint16 worldWidth, uint16 worldHeight);

    void SetTile(uint16 x, uint16 y, TileType type);
    Tile* GetTile(int32 x, int32 y) const;

    void SetTileSize(uint16 tileSize);

    virtual void Draw(IGraphics* pGraphics) override;

    // World map data
    Tile** m_ppTiles;

    // World width in tile units.
    uint16 m_WorldWidth;
    // World height in tiles units.
    uint16 m_WorldHeight;

    uint16 m_TileSize;
};