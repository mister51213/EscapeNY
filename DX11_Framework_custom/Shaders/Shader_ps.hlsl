////////////////////////////////////////////////////////////////////////////////
// Filename: Shader_ps
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

// This will be INITIALIZED differently for EACH OBJECT
cbuffer WMatBuffer
{
    matrix objectWorldMatrix;
};

// TODO: change this to LightBufferType
//cbuffer LightBuffer
//{
//    float4 ambientColor;
//    float4 diffuseColor;
//    float3 lightDirection;
//    float padding;
//};

//cbuffer LightBufferType
//{
//	float4 lightColor;
//	float3 lightPosition;
//	float coneAngle;
//	float3 lightDirection;
//	float padding;
//}

// The packoffset keyword helps pack data tightly to reduce memory waste.
cbuffer SceneAttr
{
	// First 16 byte vector
	float4 ambientColor : packoffset(c0);	
	// First 4 bytes of second 16 byte vector
	float lightCount : packoffset(c1.x);	
	// Last 12 bytes of second 16 byte vector
	float3 g_padding : packoffset(c1.yzw);	
};

// lightTypeEnum types
/*
0 = infinite/direction light
1 = point light
2 = spot light
*/
struct LightTemplate
{
	float4 lightColor;
	float3 lightPosition;
	float coneAngle;
	float3 lightDirection;
	int lightTypeEnum;
};

StructuredBuffer<LightTemplate> lightList;

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float3 surfaceWorldPosition : POSITION;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

float4 CalculateInfiniteLight(float4 LightColor, float3 LightDirection, float3 SurfaceNormal)
{
    // Invert the light direction for calculations.
	float3 lightDir = -LightDirection;

    // Calculate the amount of light on this pixel.
	float Intensity = saturate(dot(SurfaceNormal, lightDir));

	return LightColor * Intensity;
}
float4 CalculatePointLight(float4 LightColor, float3 LightPosition, float3 SurfacePosition, float3 SurfaceNormal)
{

}
float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;

	// Set ambient color here
	float4 ambientColor = { 0.15f, 0.3f, 0.3f, 1.0f };

	// TODO:CREATE a BufferedStructure in Shader class; for example:
	// color = lightList[0].lightDirection;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    // Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

	// Calculate infinite light color adjusted for angle of light to surface 
	// normal
	float4 lightColor = { 0.f, 0.f, 0.f, 0.f };
	for (int index = 0; index < lightCount; ++index)
	{
		if(lightList[i].lightTypeEnum == 0)
		{
			lightColor += CalculateInfiniteLight(lightList[index].lightColor, lightList[0].lightDirection, input.normal);
		}
		else if(lightList[i].lightTypeEnum == 1)
		{
			lightColor += CalculatePointLight(lightList[index].lightColor, lightList[index].lightPosition, input.surfaceWorldPosition, input.normal);
		}
		else
		{

		}
	}

		

	// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
	color += lightColor;

    // Saturate final light color
    color = saturate(color);

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    color = color * textureColor;

    return color;
}