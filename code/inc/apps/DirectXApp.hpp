#pragma once

#include <apps/IApp.hpp>

class DirectXApp : public IApp
{
public:
	DirectXApp();
	virtual ~DirectXApp();

	virtual void Init(World* pWorld, Pathfinder* pPathfinder, int32 width, int32 height) override;
	virtual void Release() override;
	virtual void HandleInput() override;
	virtual void Render() override;

};