Texture2D textures[2];
SamplerState samp;


struct LightTemplate
{
	float4 color;
	float3 position;
	float coneAngle;
	float3 direction;
	float padding;
};

cbuffer LightBuffer
{
	LightTemplate g_lights;
	float4 g_ambientColor;
	int g_lightCount;
	float3 g_padding;
}

struct PixelBuffer
{
	float4 position : SV_Position;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float4 worldPosition : POSITION;
	float3x3 tbc : TBC;
};

float4 main(PixelBuffer input):SV_Target
{
	float4 texColor = textures[0].Sample(samp, input.texcoord);
	float4 nMapColor = textures[1].Sample(samp, input.texcoord);

	float3 nMap = nMapColor.rgb - float3(0.5f, 0.5f, 0.5f);
	float3 lightTan = mul(g_lights.direction, input.tbc);

	float intensity = dot(nMap, lightTan);
	float4 color = g_ambientColor;
	if(color.r > .5f)
	{
		color = float4(.1f, .1f, .1f, .1f);
	}
	if(intensity > 0.f)
	{
		color = saturate(color + (texColor * (g_lights.color * intensity)));
	}

	return color;

}