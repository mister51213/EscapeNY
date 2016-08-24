
cbuffer PerFrameBuffer
{
	matrix g_view;					// 64 bytes 0 - 63
	matrix g_projection;			// 64 bytes	64 - 127
	float3 g_pointLightPosition[4];	// 48 bytes	128 - 175
};

cbuffer PerModelBuffer
{
	matrix g_world;
};

struct VertexInput
{
	float3 position : POSITION;
	float2 tex		: TEXCOORD0;
	float3 normal	: NORMAL;
	float4 color	: COLOR;
};

struct VertexOutput
{
	float4 position : SV_Position;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	float4 color : COLOR;
	// These values are the direction vector from vertex to light and 
	// will be interpolated...aka vertex lighting as opposed to pixel 
	// lighting
	float3 pointLightDirection0 : LIGHTDIR0;
	float3 pointLightDirection1 : LIGHTDIR1;
	float3 pointLightDirection2 : LIGHTDIR2;
	float3 pointLightDirection3 : LIGHTDIR3;
};


VertexOutput main(VertexInput input)
{
	float4 position = float4(input.position, 1.f);
	
	position = mul(position, g_world);
	float3 worldPosition = position.xyz;
	position = mul(position, g_view);
	position = mul(position, g_projection);

	
	VertexOutput output;
	output.position = position;
	output.tex = input.tex;
	output.normal = input.normal;
	output.color = input.color;
	output.pointLightDirection0 = normalize(worldPosition - g_pointLightPosition[0]);
	output.pointLightDirection1 = normalize(worldPosition - g_pointLightPosition[1]);
	output.pointLightDirection2 = normalize(worldPosition - g_pointLightPosition[2]);
	output.pointLightDirection3 = normalize(worldPosition - g_pointLightPosition[3]);

	return output;
}