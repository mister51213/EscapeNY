
////////////////////////////////////////////////////////////////////////////////
// Filename: color.ps
////////////////////////////////////////////////////////////////////////////////

// Constant buffer for updating light information for shader
cbuffer LightBuffer
{
	float4 g_diffuseColor;
	float3 g_lightDirection;
	float padding;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
	float3 normal : NORMAL;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET
{
	// Get the color of the surface
	float4 color = input.color;

    // Invert the light direction for calculations.
	float3 lightDir = -g_lightDirection;

    // Calculate the amount of light on this pixel.
	float lightIntensity = saturate(dot(input.normal, lightDir));

    // Determine the final amount of diffuse color based on the diffuse color 
	// combined with the light intensity.
	float4 finalColor = saturate(g_diffuseColor * lightIntensity);

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	finalColor = finalColor * color;

	return finalColor;
}
