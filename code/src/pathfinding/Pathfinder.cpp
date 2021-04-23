#include <pathfinding/Pathfinder.hpp>

#include <unordered_map>
#include <set>

#include <climits>

Pathfinder::Pathfinder()
	: m_pWorld(nullptr)
{
}

Pathfinder::~Pathfinder()
{
	m_pWorld = nullptr;
}

void Pathfinder::SetWorld(World* pWorld)
{
	m_pWorld = pWorld;
}

// Leads to paths with fewer turns
float ManhattanDistance(const Tile& a, const Tile& b)
{
	return fabsf(static_cast<float>(a.GetX() - b.GetX())) + fabsf(static_cast<float>(a.GetY() - b.GetY()));
}

// Tends to zig-zag a lot when the goal is in a diagonal direction
float EuclideanDistance(const Tile& a, const Tile& b)
{
	// sqrtf not needed here, but without it it wouldn't really be Euclidean Distance, so I'm leaving it in.
	return sqrtf((a.GetX() - b.GetX()) * (a.GetX() - b.GetY()) + (a.GetY() - b.GetY()) * (a.GetY() - b.GetY()));
}

float h(const Tile& a, const Tile& b)
{
	return ManhattanDistance(a, b);
}

bool Pathfinder::CalculatePath(const Tile& start, const Tile& goal, _Inout_ Path* pOutPath)
{
	Node* pStart = new Node();
	pStart->pTile = &start;
	pStart->gScore = 0.0f; // start -> start has no cost
	pStart->fScore = h(start, goal);

	m_TileNodeMappings.insert_or_assign(pStart->pTile, pStart);

	// Contains Nodes of interest, ordered in ascending order by fScore (1st) and gScore (2nd)
	std::set<Node*, NodeCmpAscendingByFScore> openSet;
	openSet.insert(pStart);

	std::unordered_map<const Tile*, const Tile*> cameFrom;

	while (openSet.size() > 0)
	{
		// Get Node with lowest fScore in openSet
		Node* pCurrentNode = *openSet.begin();

		// Path found?
		if (pCurrentNode->pTile == &goal)
		{
			ReconstructPath(pCurrentNode->pTile, cameFrom, pOutPath);

			// deallocate nodes
			for (const std::pair<const Tile*, Node*>& pair : m_TileNodeMappings)
				delete pair.second;

			m_TileNodeMappings.clear();

			return true;
		}

		// processing this node now, can remove it from the open set
		openSet.erase(pCurrentNode);

		int32 offsets[4][2] = {
			{ -1,  0 }, // left
			{  1,  0 }, // right
			{  0, -1 }, // top
			{  0,  1 }, // bottom
		};

		// For each orthogonal neighbor
		for (uint16 i = 0; i < 4; ++i)
		{
			const Tile* pNeighbor = GetUsingOffset(pCurrentNode->pTile, offsets[i][0], offsets[i][1]);

			if (!ValidateTile(pNeighbor))
				continue;

			Node* pNeighborNode = GetOrCreateNode(pNeighbor);

			// cost(start, current) + cost(current, neighbor) = cost(start, neighbor)
			float tmpGscore = pCurrentNode->gScore + h(*pCurrentNode->pTile, *pNeighbor); // could add weight related things here, multiplying the result of h() by the weight

			// Have we found a faster or same cost path to neighbor tile?
			if (tmpGscore <= pNeighborNode->gScore)
			{
				// Found better or equal path, record
				// Store were we came from for reconstructing later.
				cameFrom.insert_or_assign(pNeighbor, pCurrentNode->pTile);

				// start -> neighbor, best known
				pNeighborNode->gScore = tmpGscore;

				// start -> neighbor -> goal, best guess
				pNeighborNode->fScore = tmpGscore + h(*pNeighbor, goal);

				// Found a faster path than before, so we need to update the neighbor's neighbors as well
				// i.e. this node is interesting
				openSet.insert(pNeighborNode);
			}
		}
	}

	// deallocate nodes
	for (const std::pair<const Tile*, Node*>& pair : m_TileNodeMappings)
		delete pair.second;

	m_TileNodeMappings.clear();

	return false;
}

void Pathfinder::ReconstructPath(const Tile* pTile, std::unordered_map<const Tile*, const Tile*> cameFrom, Path* pOutPath)
{
	// Reconstruct path
	pOutPath->clear();
	pOutPath->push_back(pTile);

	auto it = cameFrom.find(pTile);
	while (it != cameFrom.end())
	{
		pTile = it->second;
		pOutPath->insert(pOutPath->begin(), pTile);
		it = cameFrom.find(pTile);
	}
}

Pathfinder::Node* Pathfinder::GetOrCreateNode(const Tile* pTile)
{
	auto itNode = m_TileNodeMappings.find(pTile);
	Node* pNeighborNode;
	if (itNode == m_TileNodeMappings.end())
	{
		// No node exists for neighbor tile, create one

		pNeighborNode = new Node();
		pNeighborNode->pTile = pTile;
		pNeighborNode->gScore = std::numeric_limits<float>::infinity();
		pNeighborNode->fScore = pNeighborNode->gScore;

		m_TileNodeMappings.insert_or_assign(pTile, pNeighborNode);
	}
	else {
		// Node exists for neighbor tile, use it
		pNeighborNode = itNode->second;
	}

	return pNeighborNode;
}

const Tile* Pathfinder::GetUsingOffset(const Tile* pTile, int32 dx, int32 dy)
{
	return m_pWorld->GetTile(
		pTile->GetX() + dx,
		pTile->GetY() + dy
	);
}

bool Pathfinder::ValidateTile(const Tile* pTile)
{
	if (!pTile) // e.g. when out of bounds
		return false;

	if (pTile->flags & TILE_FLAG_UNWALKABLE)
	{
		// Can't walk on this tile, continue
		return false;
	}

	return true;
}
