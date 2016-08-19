Texture2D shaderTexture;
SamplerState SampleType;

cbuffer DirectionLightBuffer
{
	float4 g_diffuseColor;
	float3 g_lightDirection;
	float padding;
};

cbuffer SpotLightBuffer
{
	
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
	float3 lightDir = -g_lightDirection;

    // Calculate the amount of light on this pixel.
	float lightIntensity = saturate(dot(psin.normal, lightDir));

    // Determine the final amount of diffuse color based on the diffuse color
	// combined with the light intensity.
	return saturate(g_diffuseColor * lightIntensity);
}

float4 CalculateSpotLightColor(PixelInput psin)
{
	
}

float4 main(PixelInput psin) : SV_Target
{
    // Sample the pixel color from the texture using the sampler at this texture 
	// coordinate location.
	float4 textureColor = shaderTexture.Sample(SampleType, psin.texcoord);

	
	float directionLightColor = CalculateDirectionalLightColor(psin);
	float spotLightColor = CalculateSpotLightColor(psin);
	float totalLightColor = saturate(directionLightColor + spotLightColor);

    // Multiply the texture pixel and the final diffuse color to get the final
	// pixel color result.
	float4 color = totalLightColor * psin.color * textureColor;

}
