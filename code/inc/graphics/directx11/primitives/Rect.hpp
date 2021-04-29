#pragma once

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <DirectXMath.h>

#include <vector>

class Rect
{
public:
	Rect();
	virtual ~Rect();

	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	//void AddInstance();
	void AddInstance(float x, float y, float w, float h, float r, float g, float b);
	void DrawInstances();

	void SetPosition(float x, float y);
	void SetScale(float x, float y);
	void SetColor(float r, float g, float b, float a = 1.0f);

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pInstanceBuffer;

	DirectX::XMFLOAT3 m_vPosition;
	DirectX::XMFLOAT3 m_vScale;

	DirectX::XMFLOAT4 m_vColor;

	DirectX::XMMATRIX m_mWorld;

	struct InstanceData {
		DirectX::XMFLOAT4 vColor;
		DirectX::XMMATRIX mWorld;
	};

	static const int32 s_MaxInstances = WORLD_TILE_HEIGHT * WORLD_TILE_WIDTH;
	int32 m_NumInstances;
	InstanceData m_InstanceDatas[s_MaxInstances];
};