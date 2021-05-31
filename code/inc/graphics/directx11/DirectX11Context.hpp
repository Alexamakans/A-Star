#pragma once

#include <graphics/core/IContext.hpp>
#include <graphics/directx11/DirectX11Surface.hpp>

#include <DirectXMath.h>

#include <graphics/directx11/primitives/Rect.hpp>

namespace SG {
	class DirectX11Context : public IContext
	{
	public:
		DirectX11Context();
		virtual ~DirectX11Context();

		virtual void Init(ISurface* pSurface) override;
		virtual void DrawChar(char c, float x, float y) override;
		virtual void FillRectangle(float x, float y, float w, float h) override;
		virtual void DrawRectangle(float x, float y, float w, float h) override;

		Rect& GetRect();

	private:
		DirectX11Surface* m_pSurface;

		ID3D11VertexShader* m_pVertexShader;
		ID3D11PixelShader* m_pPixelShader;

		ID3D11InputLayout* m_pInputLayout;

		// world matrix cb is slot 0
		ID3D11Buffer* m_pCB_View; // slot 1
		ID3D11Buffer* m_pCB_Proj; // slot 2

		DirectX::XMMATRIX m_mView;
		DirectX::XMMATRIX m_mProj;

		Rect rect;

		float m_fHeight;
	};
}