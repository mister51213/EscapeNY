Texture2D textures[2];
SamplerState samp;


struct LightTemplate
{
	float4 color;
	float3 position;
	float coneAngle;
	float3 direction;
	int type;
};

cbuffer LightBuffer
{
	float4 g_ambientColor;
	int g_lightCount;
	float3 g_padding;
	LightTemplate g_lights[10];
}

struct PixelBuffer
{
	float4 position : SV_Position;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
	float4 worldPosition : POSITION;
	float3 tangent : TANGENT;
	float3 Binormal : BINORMAL;
	float3 normal : NORMAL;
};


float4 CalculateInfiniteLight(
	float4 LightColor, 
	float3 LightDirection, 
	float3 SurfaceNormal)
{
    // Invert the light direction for calculations.
	float3 lightDir = -LightDirection;

    // Calculate the amount of light on this pixel.
	float intensity = saturate(dot(SurfaceNormal, lightDir));

	return LightColor * intensity;
}

float4 CalculatePointLight(
	float4 LightColor, 
	float3 LightPosition, 
	float3 SurfacePosition, 
	float3 SurfaceNormal)
{
	float3 surfaceToLightDir = normalize(SurfacePosition - LightPosition);

	float intensity = saturate(dot(surfaceToLightDir, SurfaceNormal));

	return LightColor * intensity;
}

float4 CalculateSpotLight(
	float4 LightColor, 
	float3 LightPosition, 
	float3 LightDirection, 
	float3 SurfacePosition, 
	float3 SurfaceNormal,
	float ConeAngle)
{
	float3 surfaceToLightDir = normalize(SurfacePosition - LightPosition);
	float ptLightIntensity = saturate(dot(surfaceToLightDir, LightDirection));

	float dirLightIntensity = saturate(dot(-LightDirection, SurfaceNormal));
	ConeAngle = min(180.f, max(1.f, ConeAngle));
	ConeAngle /= 180.f;

	float4 color = float4(0.f, 0.f, 0.f, 0.f);
	if (ptLightIntensity >= 1.f - ConeAngle)
	{		
		float intensity = ptLightIntensity * dirLightIntensity;
		color = LightColor * intensity;
	}

	return color;
}


float4 main(PixelBuffer input):SV_Target
{
	float4 texColor = textures[0].Sample(samp, input.texcoord);
	float4 nMapColor = textures[1].Sample(samp, input.texcoord);

	float3 nMap = (nMapColor.rgb * 2.f) - float3(1.f, 1.f, 1.f);
	
	float3x3 tanMatrix = float3x3(input.tangent, input.Binormal, input.normal);
	float3 lightTan = mul(nMap, tanMatrix);

	float4 color = g_ambientColor * texColor;
	for (int idx = 0; idx < g_lightCount; ++idx)
	{
		float4 lColor = float4(0.f, 0.f, 0.f, 0.f);
		if(g_lights[idx].type == 0)
		{
			lColor = saturate(CalculateInfiniteLight(g_lights[idx].color, g_lights[idx].direction, lightTan));
		}
		else if(g_lights[idx].type == 1)
		{
			lColor = saturate(CalculatePointLight(g_lights[idx].color, g_lights[idx].position, input.worldPosition.xyz, lightTan));
		}
		else if(g_lights[idx].type == 2)
		{
			lColor = saturate(CalculateSpotLight(g_lights[idx].color, g_lights[idx].position, g_lights[idx].direction, input.worldPosition.xyz, lightTan, g_lights[idx].coneAngle));
		}

		color += lColor;
	}

	return saturate(color);

}