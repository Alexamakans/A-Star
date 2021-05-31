#include <graphics/directx11/primitives/Rect.hpp>

Rect::Rect()
	: m_pDevice(nullptr)
	, m_pContext(nullptr)
	, m_pVertexBuffer(nullptr)
	, m_pInstanceBuffer(nullptr)
	, m_mWorld()

	, m_vPosition( 0.0f, 0.0f, 0.0f )
	, m_vScale(1.0f, 1.0f, 1.0f)
{
}

Rect::~Rect()
{
	SAFE_RELEASE(m_pInstanceBuffer);
	SAFE_RELEASE(m_pVertexBuffer);

	m_pContext = nullptr;
	m_pDevice = nullptr;
}

void Rect::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	m_mWorld = DirectX::XMMatrixIdentity();

	// Create cbuffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(InstanceData) * s_MaxInstances;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	bd.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_pDevice->CreateBuffer(&bd, NULL, &m_pInstanceBuffer)))
	{
		MessageBox(NULL, L"Failed creating constant buffer", L"Fatal error", MB_OK);
		return;
	}

	DirectX::XMFLOAT4 vertices[6] = {
		{  0.0f, -1.0f,  0.0f, 1.0f },
		{  0.0f,  0.0f,  0.0f, 1.0f },
		{  1.0f,  0.0f,  0.0f, 1.0f },
		{  1.0f,  0.0f,  0.0f, 1.0f },
		{  1.0f, -1.0f,  0.0f, 1.0f },
		{  0.0f, -1.0f,  0.0f, 1.0f },
	};

	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA pData;
	pData.pSysMem = &vertices;

	if (FAILED(m_pDevice->CreateBuffer(&bd, &pData, &m_pVertexBuffer)))
	{
		MessageBox(NULL, L"Failed creating vertex buffer", L"Fatal error", MB_OK);
		return;
	}

	return;
}

void Rect::AddInstance(float x, float y, float w, float h, float r, float g, float b)
//void Rect::AddInstance(float x, float y, float w, float h, float r, float g, float b)
{
	/*m_InstanceDatas[m_NumInstances] = {
		m_vColor,
		DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(
			DirectX::XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z),
			DirectX::XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z)
		))
	};*/

	m_InstanceDatas[m_NumInstances] = {
		{ r, g, b, 1.0f },
		DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(
			DirectX::XMMatrixScalingFromVector({ w, h, 1.0f }),
			DirectX::XMMatrixTranslationFromVector({ x, y, 0.0f })
		))
	};

	++m_NumInstances;
}

void Rect::DrawInstances()
{
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		memcpy_s(pData.pData, sizeof(m_InstanceDatas), m_InstanceDatas, sizeof(InstanceData) * m_NumInstances);

		m_pContext->Unmap(m_pInstanceBuffer, 0);
	}

	UINT pStrides[2] = { sizeof(DirectX::XMFLOAT4), sizeof(InstanceData) };
	UINT pOffsets[2] = { 0, 0 };

	ID3D11Buffer* pBuffers[2] = {
		m_pVertexBuffer,
		m_pInstanceBuffer,
	};

	m_pContext->IASetVertexBuffers(0, 2, pBuffers, pStrides, pOffsets);

	m_pContext->DrawInstanced(6, m_NumInstances, 0, 0);

	m_NumInstances = 0;
}

void Rect::SetPosition(float x, float y)
{
	m_vPosition.x = x;
	m_vPosition.y = y;
}

void Rect::SetScale(float x, float y)
{
	m_vScale.x = x;
	m_vScale.y = y;
}

void Rect::SetColor(float r, float g, float b, float a)
{
	m_vColor.x = r;
	m_vColor.y = g;
	m_vColor.z = b;
	m_vColor.w = a;
}
