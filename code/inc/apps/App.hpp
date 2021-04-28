#pragma once

class ISurface;
namespace SG { class IContext; }

class App {
public:
	virtual ~App() {};

	virtual void Init(int width, int height) = 0;
	
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void Release() = 0;

protected:
	ISurface* m_pSurface;
	SG::IContext* m_pContext;
};