cbuffer PerFrameMatrixBuffer
{
	matrix g_view;
	matrix g_projection;
};

cbuffer PerModelMatrixBuffer
{
	matrix g_world;
};

struct VertexInput
{
	float3 position : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

struct PixelOutput
{
	float4 position : SV_Position;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float4 worldPosition : POSITION;
};

PixelOutput main(VertexInput vsin)
{
	PixelOutput vsout;

	vsout.worldPosition = mul(float4(vsin.position, 1.f), g_world);
	vsout.position = vsout.worldPosition;
	vsout.position = mul(vsout.position, g_view);
	vsout.position = mul(vsout.position, g_projection);
	
	vsout.texcoord = vsin.texcoord;
	vsout.normal = mul(vsin.normal, float3x3(g_world[0].xyz, g_world[1].xyz, g_world[2].xyz));
	vsout.color = vsin.color;
	
	return vsout;
}