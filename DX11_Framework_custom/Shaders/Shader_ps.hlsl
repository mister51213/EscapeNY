////////////////////////////////////////////////////////////////////////////////
// Filename: Shader_ps
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
// TODO: use second texture as normal map
Texture2D shaderTextures[2];

SamplerState SampleType;

// This will be INITIALIZED differently for EACH OBJECT
cbuffer WMatBuffer
{
    matrix objectWorldMatrix;
};

cbuffer LightBufferType
{
	float4 lightColor;
	float3 lightPosition;
	float coneAngle;
	float3 lightDirection;
	float padding;
}

//struct LightTemplate
//{
//    float4 ambientColor;
//    float4 diffuseColor;
//    float3 lightDirection;
//    float padding;
//};
//StructuredBuffer<LightTemplate> lightList;

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float4 world : TEXCOORD1; // NEW
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET
{
	// TODO:CREATE a BufferedStructure in Shader class; for example:
	// color = lightList[0].lightDirection;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    //float4 textureColor = shaderTexture.Sample(SampleType, input.tex);
    float4 textureColor = shaderTextures[0].Sample(SampleType, input.tex);
    float4 normalMap = shaderTextures[1].Sample(SampleType, input.tex);
	normalMap.rgb = normalMap.rgb - float3(.5f, .5f, .5f);
	float3 nMap = normalize(normalMap.rgb);
	float4 ambientColor = textureColor*float4( 0.1f, 0.1f, 0.1f, 1.0f );
	
    // Invert the light direction for calculations.
    float3 lightDir = -(lightDirection);

	float nLightIntensity = dot(nMap, lightDir);
	float lightIntensity = dot(input.normal, lightDir) * nLightIntensity;

	float4 finalColor = { 0.f, 0.f, 0.f, 0.f };
	if(lightIntensity > 0.f)
	{
		finalColor = (textureColor * lightColor) * lightIntensity;
	}
    // Calculate the amount of light on this pixel.
    finalColor = saturate(finalColor + ambientColor);
    
    return finalColor;
}