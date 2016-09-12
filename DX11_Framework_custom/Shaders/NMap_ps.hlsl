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
	float4 g_ambientColor;
	int g_lightCount;
	float3 g_padding;
	LightTemplate g_lights;
}

struct PixelBuffer
{
	float4 position : SV_Position;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
	float4 worldPixelPosition : POSITION;
	float3 tangent : TANGENT;
	float3 Binormal : BINORMAL;
	float3 normal : NORMAL;
};

float4 main(PixelBuffer input) : SV_Target
{
	float4 texColor = textures[0].Sample(samp, input.texcoord);
	float4 nMapColor = textures[1].Sample(samp, input.texcoord);

	float3 nMap = (nMapColor.rgb * 2.f) - float3(1.f, 1.f, 1.f);
	
	float3x3 tanMatrix = float3x3(input.tangent, input.Binormal, input.normal);
	float3 lightTan = mul(nMap, tanMatrix);

	float3 lightDirection = -g_lights.direction;

        /******************* SPOTLIGHT **************************/
	float4 ambientColor = texColor * float4(0.06f, 0.06f, 0.06f, 1.0f);

    // TODO: transform light direction first

    // Calculate the vector of pixel to light
	float3 surfToLight = g_lights.position - input.worldPixelPosition.xyz;
	float3 surfToLightN = normalize(surfToLight);

	float4 finalColor = ambientColor;

	// This calculates the angle between the surface normal and the light direction
    	float coneAngle = 0.9f;

	float DP1 = saturate(dot(surfToLightN, lightDirection));
    float DP2 = saturate(dot(surfToLightN, input.normal));

    float DPdiff = abs(DP1 - DP2);

	// This clips all points outside of range
	if (DP1 > coneAngle && DPdiff > 0.7) // CUTOFF RANGE
	{
		// Range of light from center to outer
		float range = 1.f - coneAngle;
		// Calculate the intensity at distance from outer edge
		float gradientIntensity = (DP1 - coneAngle) / range;
		// Calculate the amount of light on this pixel.
		float intensity = 10.0f* dot(lightDirection, lightTan) * gradientIntensity;
		intensity *= 1.f / length(surfToLight);
		finalColor = saturate(g_lights.color * intensity + ambientColor);
	}

	return finalColor;
}