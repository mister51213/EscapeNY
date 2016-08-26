// Vertex shader

cbuffer Transforms
{
	matrix g_world;
	matrix g_view;
	matrix g_proj;
};

struct VertexBuffer
{
	float3 position : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float3 conormal : CONORMAL;
};

struct PixelBuffer
{
	float4 position : SV_Position;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float4 worldPosition : POSITION;
	float3x3 tbc : TBC;
};

PixelBuffer main(VertexBuffer input)
{
	PixelBuffer output;

	output.position = float4(input.position, 1.f);
	output.position = mul(output.position, g_world);
	output.worldPosition = output.position;
	output.position = mul(output.position, g_view);
	output.position = mul(output.position, g_proj);

	float3x3 rotation = (float3x3) g_world;
	output.normal = normalize(mul(input.normal, rotation));
	output.tbc[0] = normalize(mul(input.tangent, rotation));
	output.tbc[1] = normalize(mul(input.bitangent, rotation));
	output.tbc[2] = normalize(mul(input.conormal, rotation));

	output.color = input.color;
	output.texcoord = input.texcoord;

	return output;
}