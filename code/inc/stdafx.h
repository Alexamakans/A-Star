#pragma once

#include <iostream>

#define SAFE_DELETE(x) if (x != nullptr) { delete x; x = nullptr; }
#define SAFE_RELEASE(x) if (x != nullptr) { x->Release(); x = nullptr; }

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;

constexpr int32 WORLD_TILE_WIDTH = 64;
constexpr int32 WORLD_TILE_HEIGHT = 64;
constexpr int32 TILE_SIZE = 16;