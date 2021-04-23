#pragma once

#include <World.hpp>

#include <vector>

typedef std::vector<const Tile*> Path;

class Pathfinder
{
private:
	struct Node
	{
		const Tile* pTile;
		// Current best guess for start -> this -> goal
		float fScore;
		// Currently known cheapest path for start -> this
		float gScore;
	};

	// Used for openSet in Pathfinder::CalculatePath
	struct NodeCmpAscendingByFScore {
		bool operator() (const Node* a, const Node* b) const
		{
			if (a->fScore == b->fScore)
			{
				if (a->gScore == b->gScore)
				{
					return a->pTile < b->pTile;
				}

				return a->gScore < b->gScore;
			}

			return a->fScore < b->fScore;
		}
	};
public:
	Pathfinder();
	virtual ~Pathfinder();

	void SetWorld(World* pWorld);

	bool CalculatePath(const Tile& start, const Tile& goal, _Inout_ Path* pOutPath);

private:
	void ReconstructPath(const Tile* pTile, std::unordered_map<const Tile*, const Tile*> cameFrom, _Inout_ Path* pOutPath);
	Node* GetOrCreateNode(const Tile* pTile);
	const Tile* GetUsingOffset(const Tile* pTile, int32 dx, int32 dy);
	bool ValidateTile(const Tile* pTile);

	// World to pathfind in.
	World* m_pWorld;

	// Used to relate Tiles with Node data: fScore, gScore
	std::unordered_map<const Tile*, Node*> m_TileNodeMappings;
};