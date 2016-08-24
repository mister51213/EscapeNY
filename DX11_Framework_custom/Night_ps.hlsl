Texture2D g_Texture;
SamplerState g_sampler;

cbuffer LightBuffer
{
	float4 g_infiniteLightColor;
	float4 g_pointLightColor[4];
	float3 g_infiniteLightDirection;
	int g_numLights;
};

float4 main() :SV_Target
{
	float3 lightPos[g_numLights];

}