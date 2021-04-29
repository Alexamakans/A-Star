/*cbuffer per_object : register(b0)
{
	matrix g_mWorld;
	float4 g_vColor;
};*/

cbuffer ViewMatrix : register(b1)
{
	matrix g_mView;
};

cbuffer ProjMatrix : register(b2)
{
	matrix g_mProj;
};

struct Vert {
	// Vertex Data
	float4 pos : POSITION;
	// Instance Data
	float4 color : COLOR;
	matrix mWorld : MATRIX;
};

struct VS_Out {
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VS_Out main(Vert vert)
{
	VS_Out output = (VS_Out)0;
	output.pos = mul(vert.pos, vert.mWorld);
	output.pos  = mul(output.pos, g_mView);
	output.pos  = mul(output.pos, g_mProj);

	output.color = vert.color;

	return output;
}