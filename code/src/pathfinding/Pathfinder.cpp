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

float h(const Tile& a, const Tile& b)
{
	return fabsf(static_cast<float>(a.GetX() - b.GetX())) + fabsf(static_cast<float>(a.GetY() - b.GetY()));
	//return sqrtf(static_cast<float>((a.GetX() - b.GetX()) * (a.GetX() - b.GetX()) + (a.GetY() - b.GetY()) * (a.GetY() - b.GetY())));
}

bool Pathfinder::CalculatePath(const Tile& start, const Tile& goal, _Inout_ Path* pOutPath)
{
	// Currently known cheapest path
	std::unordered_map<const Tile*, float> gScore;
	gScore.insert_or_assign(&start, 0.0f);

	// Current best guess
	std::unordered_map<const Tile*, float> fScore;
	fScore.insert_or_assign(&start, h(start, goal));

	std::set<const Tile*> openSet;
	openSet.insert(&start);

	std::unordered_map<const Tile*, const Tile*> cameFrom;

	while (openSet.size() > 0)
	{
		// Get lowest fScore in openSet
		const Tile* pCurrent = nullptr;
		float lowest = std::numeric_limits<float>::infinity();

		for (const Tile* pTile : openSet)
		{
			if (fScore.find(pTile) != fScore.end())
			{
				float score = fScore.at(pTile);
				if (score < lowest)
				{
					lowest = score;
					pCurrent = pTile;
				}
			}
		}

		if (pCurrent == nullptr)
		{
			return false;
		}

		if (pCurrent == &goal)
		{
			pOutPath->clear();
			pOutPath->push_back(pCurrent);

			auto it = cameFrom.find(pCurrent);
			while (it != cameFrom.end())
			{
				pCurrent = it->second;
				pOutPath->insert(pOutPath->begin(), pCurrent);
				it = cameFrom.find(pCurrent);
			}

			return true;
		}

		openSet.erase(pCurrent);
		int32 offsets[4][2] = {
			{ -1,  0 },
			{  1,  0 },
			{  0, -1 },
			{  0,  1 },
		};

		for (uint16 i = 0; i < 4; ++i)
		{
			const Tile* pNeighbor = m_pWorld->GetTile(
				pCurrent->GetX() + offsets[i][0],
				pCurrent->GetY() + offsets[i][1]
			);

			if (!pNeighbor)
				continue;

			float tmpGscore;
			if (pNeighbor->flags & TILE_FLAG_UNWALKABLE)
			{
				tmpGscore = std::numeric_limits<float>::infinity();
			}
			else {
				tmpGscore = gScore.at(pCurrent) + h(*pCurrent, *pNeighbor);
			}

			float gS;
			if (gScore.find(pNeighbor) != gScore.end())
			{
				gS = gScore.at(pNeighbor);
			}
			else {
				gS = std::numeric_limits<float>::infinity();
			}

			if (tmpGscore <= gS)
			{
				// Found better path, record
				cameFrom.insert_or_assign(pNeighbor, pCurrent);
				gScore.insert_or_assign(pNeighbor, tmpGscore);
				fScore.insert_or_assign(pNeighbor, tmpGscore + h(*pNeighbor, goal));

				openSet.insert(pNeighbor);
			}
		}
	}

	return false;
}