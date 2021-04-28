#pragma once

#include <apps/IApp.hpp>

class ConsoleApp : public IApp
{
public:
	ConsoleApp();
	virtual ~ConsoleApp();

	virtual void Init(_In_ World* pWorld, _In_ Pathfinder* pPathfinder, int32 width, int32 height) override;

	virtual void HandleInput() override;
	virtual void Render() override;

	virtual void Release() override;

};