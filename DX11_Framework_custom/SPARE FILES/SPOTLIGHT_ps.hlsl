Texture2D shaderTexture;
SamplerState SampleType;

cbuffer SpotLightBuffer
{
	float3 g_lightColor : packoffset(c0);
	float g_lightIntensity : packoffset(c0.a);
	float3 g_lightPosition : packoffset(c1);
	float g_innerCone : packoffset(c1.w);		// Value must be between 0 and 1 and less than g_outerCone
	float3 g_lightDrection : packoffset(c2);
	float g_outerCone : packoffset(c2.w);		// Value must be between 0 and 1 and greater than g_innerCone
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 world : TEXCOORD1;
};

float4 main(PixelInputType Input) : SV_Target
{
	// Get the color of the pixel at the texture coordinates
	float4 textureColor = shaderTexture.Sample(SampleType, Input.tex);
	
	// For now define the ambient color in shader
	float4 ambientColor = float4(.5f, .5f, .1f, 1.f);

	// Calculate distance to light
	float3 distanceToLight = Input.world.xyz - g_lightPosition;
	float recipLen = 1.0f / length(distanceToLight); // get the length (magnitude) of that vector

	// Calculate the direction of the light to the point on the surface
	float3 lightToPixelDir = distanceToLight * recipLen; // normalize this vector by dividing by its magnitude

	// Determine if the direction of the light lines up with the direction
	// of the light to surface vector
	float dp = dot(lightToPixelDir, g_lightDrection);
    // dot product value tells you HOW ON TARGET is the spotlight (target is the pixel)

    // WAY w/ FEWEST POSSIBLE STEPS:
    dpSaturated = saturate(dp); // clamps a value between 0.0 and 1.0
    return textureColor*dpSaturated;

	// Since a dot-product result of 1.f would mean the light is facing the 
	// pixel directly, we invert the values to correspond with the intent of 
	// the variables in the main program.
	float stopRange = 1.f - g_innerCone; // <- makes value bigger
	float startRange = 1.f - g_outerCone; // <- makes value smaller
	
	// Interpolate 
	float range = stopRange - startRange;
	float distance = dp - startRange;
	float scale = (distance / range); //* (10.f * recipLen);
	
	// Calculate the amount the surface is facing the light and 
	// scale the result by the interpolated value
	scale = dot(Input.normal, -g_lightDrection) * scale;

	// Calculate the corrected intensity of the light
	float3 intensity = saturate(g_lightIntensity * scale);

	// Modify the light color by the intensity
	float4 lightColor = float4(g_lightColor * intensity, 1.f);
	
	// Calculate the final color of the pixel by multiplying the light color
	// with the texture color and adding ambient color.
	float4 finalColor = (lightColor * textureColor) + ambientColor;
		
	// Saturate result to be between 0.f and 1.f
	finalColor = saturate(finalColor);

	return finalColor;
}
