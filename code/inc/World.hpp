#pragma once

#include <graphics/core/IDrawable.hpp>

#include <unordered_map>

typedef char TileGraphic;
enum TileType : int32
{
    TILE_TYPE_UNWALKABLE = 0,
    TILE_TYPE_WALKABLE,
    TILE_TYPE_MAX,
};

enum TileFlag : int32 {
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
    int32 flags = TILE_FLAG_UNWALKABLE;

private:
    int32 x, y;
    friend class World;
};

class World : public SG::IDrawable {
public:
    World();
    virtual ~World();

    void Release();

    void Init(int32 worldWidth, int32 worldHeight);

    void SetTile(int32 x, int32 y, TileType type);
    Tile* GetTile(int32 x, int32 y) const;

    void SetTileSize(int32 tileSize);

    virtual void Draw(SG::IContext* pContext) override;

    // World map data
    Tile** m_ppTiles;

    // World width in tile units.
    int32 m_WorldWidth;
    // World height in tiles units.
    int32 m_WorldHeight;

    int32 m_TileSize;
};