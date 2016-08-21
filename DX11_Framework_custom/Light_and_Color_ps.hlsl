Texture2D shaderTexture;
SamplerState SampleType;

cbuffer DirectionLightBuffer
{
	float4 g_directionLightDiffuseColor;
	float3 g_directionLightDirection;
	float padding;
};

cbuffer SpotLightBuffer
{
	float3 g_spotLightColor : packoffset(c0);
	float g_spotLightIntensity : packoffset(c0.a);
	float3 g_spotLightPosition : packoffset(c1);
	float g_spotLightInnerCone : packoffset(c1.w); // Value must be between 0 and 1 and less than g_outerCone
	float3 g_spotLightDrection : packoffset(c2);
	float g_spotLightOuterCone : packoffset(c2.w); // Value must be between 0 and 1 and greater than g_innerCone
};

struct PixelInput
{
	float4 position : SV_Position;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float4 worldPosition : POSITION;
};

// Function calulates light color based on surface normal direction
float4 CalculateDirectionalLightColor(PixelInput psin)
{
    // Invert the light direction for calculations.
	float3 lightDir = -g_directionLightDirection;

    // Calculate the amount of light on this pixel.
	float lightIntensity = saturate(dot(psin.normal, lightDir));

    // Determine the final amount of diffuse color based on the diffuse color
	// combined with the light intensity.
	return saturate(g_directionLightDiffuseColor * lightIntensity);
}

float4 CalculateSpotLightColor(PixelInput psin)
{
	// Calculate distance to light
	float3 distanceToLight = psin.worldPosition.xyz - g_spotLightPosition;
	float recipLen = 1.0f / length(distanceToLight);

	// Calculate the direction of the light to the point on the surface
	float3 lightToPixelDir = distanceToLight * recipLen;

	// Determine if the direction of the light lines up with the direction
	// of the light to surface vector
	float dp = dot(lightToPixelDir, g_spotLightDrection);
	
	// Since a dot-product result of 1.f would mean the light is facing the 
	// pixel directly, we invert the values to correspond with the intent of 
	// the variables in the main program.
	float stopRange = 1.f - g_spotLightInnerCone; // <- makes value bigger
	float startRange = 1.f - g_spotLightOuterCone; // <- makes value smaller
	
	// Interpolate 
	float range = stopRange - startRange;
	float distance = dp - startRange;
	float scale = (distance / range); //* (10.f * recipLen);
	
	// Calculate the amount the surface is facing the light and 
	// scale the result by the interpolated value
	scale = dot(psin.normal, -g_spotLightDrection) * scale;

	// Calculate the corrected intensity of the light
	float3 intensity = saturate(g_spotLightIntensity * scale);

	// Modify the light color by the intensity
	float4 lightColor = float4(g_spotLightColor * intensity, 1.f);
	
	return lightColor;
}

float4 main(PixelInput psin) : SV_Target
{
    // Sample the pixel color from the texture using the sampler at this texture 
	// coordinate location.
	float4 textureColor = shaderTexture.Sample(SampleType, psin.texcoord);

	// For now define the ambient color in shader
	float4 ambientColor = float4(.1f, .1f, .1f, 1.f);
	
	float directionLightColor = CalculateDirectionalLightColor(psin);
	float spotLightColor = CalculateSpotLightColor(psin);
	float totalLightColor = saturate(directionLightColor + spotLightColor + ambientColor);

    // Multiply the texture pixel and the final diffuse color to get the final
	// pixel color result.
	float4 color = saturate(totalLightColor * psin.color * textureColor);

}
