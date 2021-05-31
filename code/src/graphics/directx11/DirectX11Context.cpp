#include <graphics/directx11/DirectX11Context.hpp>

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

namespace SG
{
	DirectX11Context::DirectX11Context()
		: m_pSurface(nullptr)

		, m_pVertexShader(nullptr)
		, m_pInputLayout(nullptr)
		, m_pPixelShader(nullptr)

		// world matrix cb is slot 0
		, m_pCB_View(nullptr) // slot 1
		, m_pCB_Proj(nullptr) // slot 2

		, m_mView()
		, m_mProj()
	{
	}

	DirectX11Context::~DirectX11Context()
	{
		SAFE_RELEASE(m_pCB_Proj);
		SAFE_RELEASE(m_pCB_View);

		SAFE_RELEASE(m_pPixelShader);
		SAFE_RELEASE(m_pInputLayout);
		SAFE_RELEASE(m_pVertexShader);

		m_pSurface = nullptr;
	}

	void DirectX11Context::Init(ISurface* pSurface)
	{
		if (pSurface != nullptr)
		{
			m_pSurface = dynamic_cast<DirectX11Surface*>(pSurface);
			if (m_pSurface == nullptr)
			{
				throw "Unsupported ISurface passed to DirectX11Context::Init. Use DirectX11Surface.";
			}
		}
		else
		{
			m_pSurface = nullptr;
		}

		m_mView = DirectX::XMMatrixIdentity();

		float w = static_cast<float>(m_pSurface->GetWidth());
		float h = static_cast<float>(m_pSurface->GetHeight());
		m_fHeight = h;
		m_mProj = DirectX::XMMatrixOrthographicOffCenterLH(
			0.0f, w,
			0.0f, h,
			0.0f, 10.0f
		);

		ID3DBlob* pCodeBlob = nullptr;

		D3DReadFileToBlob(L"VertexShader.cso", &pCodeBlob);

		if (FAILED(
			m_pSurface->GetDevice()->CreateVertexShader(
				pCodeBlob->GetBufferPointer(),
				pCodeBlob->GetBufferSize(),
				nullptr,
				&m_pVertexShader
			)
		)
			)
		{
			MessageBox(m_pSurface->GetHWND(), L"Failed creating vertex shader", L"Fatal error", MB_OK);
			return;
		}

		m_pSurface->GetContext()->VSSetShader(m_pVertexShader, nullptr, 0);

		D3D11_INPUT_ELEMENT_DESC elDescs[6];
		elDescs[0].SemanticName = "POSITION";
		elDescs[0].SemanticIndex = 0;
		elDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // float4
		elDescs[0].InputSlot = 0;
		elDescs[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elDescs[0].InstanceDataStepRate = 0;
		elDescs[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

		elDescs[1].SemanticName = "COLOR";
		elDescs[1].SemanticIndex = 0;
		elDescs[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // float4
		elDescs[1].InputSlot = 1;
		elDescs[1].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		elDescs[1].InstanceDataStepRate = 1;
		elDescs[1].AlignedByteOffset = 0;

		elDescs[2].SemanticName = "MATRIX";
		elDescs[2].SemanticIndex = 0;
		elDescs[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // float4
		elDescs[2].InputSlot = 1;
		elDescs[2].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		elDescs[2].InstanceDataStepRate = 1;
		elDescs[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

		elDescs[3].SemanticName = "MATRIX";
		elDescs[3].SemanticIndex = 1;
		elDescs[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // float4
		elDescs[3].InputSlot = 1;
		elDescs[3].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		elDescs[3].InstanceDataStepRate = 1;
		elDescs[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

		elDescs[4].SemanticName = "MATRIX";
		elDescs[4].SemanticIndex = 2;
		elDescs[4].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // float4
		elDescs[4].InputSlot = 1;
		elDescs[4].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		elDescs[4].InstanceDataStepRate = 1;
		elDescs[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

		elDescs[5].SemanticName = "MATRIX";
		elDescs[5].SemanticIndex = 3;
		elDescs[5].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // float4
		elDescs[5].InputSlot = 1;
		elDescs[5].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		elDescs[5].InstanceDataStepRate = 1;
		elDescs[5].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

		if (FAILED(m_pSurface->GetDevice()->CreateInputLayout(elDescs, 6, pCodeBlob->GetBufferPointer(), pCodeBlob->GetBufferSize(), &m_pInputLayout)))
		{
			MessageBox(m_pSurface->GetHWND(), L"Failed creating input layout", L"Fatal error", MB_OK);
			return;
		}

		m_pSurface->GetContext()->IASetInputLayout(m_pInputLayout);
		m_pSurface->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		SAFE_RELEASE(pCodeBlob);

		D3DReadFileToBlob(L"PixelShader.cso", &pCodeBlob);

		if (FAILED(m_pSurface->GetDevice()->CreatePixelShader(pCodeBlob->GetBufferPointer(), pCodeBlob->GetBufferSize(), nullptr, &m_pPixelShader)))
		{
			MessageBox(m_pSurface->GetHWND(), L"Failed creating pixel shader", L"Fatal error", MB_OK);
			return;
		}

		m_pSurface->GetContext()->PSSetShader(m_pPixelShader, nullptr, 0);

		D3D11_BUFFER_DESC bd;
		bd.ByteWidth = sizeof(DirectX::XMMATRIX);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA pData;
		DirectX::XMMATRIX mView = DirectX::XMMatrixTranspose(m_mView);
		pData.pSysMem = &mView;

		if (FAILED(m_pSurface->GetDevice()->CreateBuffer(&bd, &pData, &m_pCB_View)))
		{
			MessageBox(NULL, L"Failed creating view constant buffer", L"Fatal error", MB_OK);
			return;
		}

		DirectX::XMMATRIX mProj = DirectX::XMMatrixTranspose(m_mProj);
		pData.pSysMem = &mProj;
		if (FAILED(m_pSurface->GetDevice()->CreateBuffer(&bd, &pData, &m_pCB_Proj)))
		{
			MessageBox(NULL, L"Failed creating view constant buffer", L"Fatal error", MB_OK);
			return;
		}

		ID3D11Buffer* pBuffers[2] = {
			m_pCB_View,
			m_pCB_Proj,
		};

		m_pSurface->GetContext()->VSSetConstantBuffers(1, 2, pBuffers);

		rect.Init(m_pSurface->GetDevice(), m_pSurface->GetContext());
	}

	void DirectX11Context::DrawChar(char c, float x, float y)
	{
		if (c == ' ')
			return;

		y = m_fHeight - y;

		//DirectX::XMFLOAT4 color{ 0.0f, 0.0f, 0.0f, 1.0f };
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;

		if (c == 'G')
		{
			g = 1.0f;
		}
		else if (c == 'S')
		{
			b = 1.0f;
		}
		else if (c == '*')
		{
			r = 1.0f;
			//color.y = 0.4f;
			//color.z = 0.4f;
		}

		//rect.SetColor(color.x, color.y, color.z, color.w);
		rect.AddInstance(x, y, TILE_SIZE, TILE_SIZE, r, g, b);
		//FillRectangle(x, y, TILE_SIZE, TILE_SIZE);
	}

	void DirectX11Context::FillRectangle(float x, float y, float w, float h)
	{
		rect.SetScale(w, h);
		rect.SetPosition(x, y);
		//rect.AddInstance();
	}

	void DirectX11Context::DrawRectangle(float x, float y, float w, float h)
	{
	}

	Rect& DirectX11Context::GetRect()
	{
		return rect;
	}
}