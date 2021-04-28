#pragma once

#include <graphics/core/ISurface.hpp>
#include <graphics/core/IContext.hpp>

#include <World.hpp>
#include <pathfinding/Pathfinder.hpp>

#include <mutex>

class IApp {
public:
	IApp();

	virtual ~IApp();

	virtual void Init(_In_ World* pWorld, _In_ Pathfinder* pPathfinder, int32 width, int32 height) = 0;
	virtual void Release() = 0;

	virtual void Start();
	virtual void Stop();

	void CopyState(IApp& into) const;

public:
	std::mutex m_Mutex;

protected:
	virtual void InternalStart();

	virtual void HandleInput() = 0;
	virtual void Update();
	virtual void Render() = 0;
	virtual void Idle();

protected:
	SG::ISurface* m_pSurface;
	SG::IContext* m_pContext;

	World* m_pWorld;
	Pathfinder* m_pPathfinder;

	Tile* m_pStart;
	Tile* m_pGoal;
	bool m_bGenWorld;
	bool m_bCalcPath;
	bool m_bWholePathDrawn;
	bool m_bExit;
	int32 m_CurrentTile;
	Path m_Path;
};