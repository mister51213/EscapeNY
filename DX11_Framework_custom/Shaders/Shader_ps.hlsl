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
    float4 textureColor = shaderTexture.Sample(SampleType, input.tex);
	float4 ambientColor = textureColor*float4( 0.5f, 0.1f, 0.0f, 1.0f );
	
    // Invert the light direction for calculations.
    float3 lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    float lightIntensity = saturate(dot(input.normal, lightDir))*lightColor;

    return (textureColor * lightIntensity) + ambientColor;
}