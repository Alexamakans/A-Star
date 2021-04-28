#include <apps/IApp.hpp>

IApp::IApp()
	: m_pSurface(nullptr)
	, m_pContext(nullptr)
	, m_pWorld(nullptr)
	, m_pPathfinder(nullptr)

	, m_pStart(nullptr)
	, m_pGoal(nullptr)
	, m_bGenWorld(true)
	, m_bCalcPath(true)
	, m_bWholePathDrawn(false)
	, m_bExit(false)

	, m_CurrentTile(0)
	, m_Path()

	, m_Mutex()
{}

IApp::~IApp()
{
	Stop();
}

void IApp::Start()
{
	InternalStart();
}

void IApp::Stop()
{
	{
		std::scoped_lock lock(m_Mutex);
		m_bExit = true;
	}
}

void IApp::CopyState(IApp& into) const
{
	into.m_pWorld = m_pWorld;
	into.m_pPathfinder = m_pPathfinder;
	into.m_pStart = m_pStart;
	into.m_pGoal = m_pGoal;
	into.m_bGenWorld = m_bGenWorld;
	into.m_bCalcPath = m_bCalcPath;
	into.m_bWholePathDrawn = m_bWholePathDrawn;
	into.m_CurrentTile = m_CurrentTile;
	into.m_Path = m_Path;
}

void IApp::InternalStart()
{
	{
		std::scoped_lock lock(m_Mutex);
		m_bExit = false;
	}

	bool bExit = false;
	while (!bExit)
	{
		HandleInput();
		Update();
		Render();

		{
			std::scoped_lock lock2(m_Mutex);
			bExit = m_bExit;
		}

		Idle();
	}

	Release();
}

void IApp::Update()
{
	if (m_bGenWorld)
	{
		m_pWorld->Init(WORLD_TILE_WIDTH, WORLD_TILE_HEIGHT);
		m_CurrentTile = 0;
		m_bCalcPath = true;
		m_bGenWorld = false;
	}

	if (m_bCalcPath)
	{
		m_pStart = m_pWorld->GetTile(0, 0);
		m_pWorld->SetTile(0, 0, TILE_TYPE_WALKABLE);
		m_pStart->graphic = 'S';

		m_pGoal = m_pWorld->GetTile(WORLD_TILE_WIDTH - 1, WORLD_TILE_HEIGHT - 1);
		m_pWorld->SetTile(WORLD_TILE_WIDTH - 1, WORLD_TILE_HEIGHT - 1, TILE_TYPE_WALKABLE);
		m_pGoal->graphic = 'G';

		m_bCalcPath = false;

		if (!m_pPathfinder->CalculatePath(*m_pStart, *m_pGoal, &m_Path))
		{
			m_bGenWorld = true;
			m_bCalcPath = true;

			return;
		}
	}

	if (m_CurrentTile < m_Path.size())
	{
		const Tile* pTile = m_Path.at(m_CurrentTile);
		m_CurrentTile++;
		if (pTile == m_pStart || pTile == m_pGoal)
			return;

		m_pWorld->GetTile(pTile->GetX(), pTile->GetY())->graphic = '*';
	}

	if (m_CurrentTile == m_Path.size())
	{
		m_bWholePathDrawn = true;
	}
}

void IApp::Idle() {
	std::this_thread::sleep_for(std::chrono::milliseconds(16));
}
