#pragma once

#include <World.hpp>

#include <vector>

typedef std::vector<const Tile*> Path;

class Pathfinder
{
public:
	Pathfinder();
	virtual ~Pathfinder();

	void SetWorld(World* pWorld);

	bool CalculatePath(const Tile& start, const Tile& goal, _Inout_ Path* pOutPath);

private:
	// World to pathfind in.
	World* m_pWorld;

	// Currently known cheapest path.
	std::unordered_map<const Tile*, float> gScore;

	// Current best guess.
	std::unordered_map<const Tile*, float> fScore;
};