////////////////////////////////////////////////////////////////////////////////
// Filename: PointLight_ps
////////////////////////////////////////////////////////////////////////////////

/////////////
// DEFINES //
/////////////
#define NUM_LIGHTS 4

/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

// This will be INITIALIZED differently for EACH OBJECT
//cbuffer WMatBuffer
//{
//    matrix objectWorldMatrix;
//};

cbuffer LightColorBuffer
{
    float4 diffuseColor[NUM_LIGHTS];
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 lightPos1 : TEXCOORD1;
    float3 lightPos2 : TEXCOORD2;
    float3 lightPos3 : TEXCOORD3;
    float3 lightPos4 : TEXCOORD4;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float lightIntensity1, lightIntensity2, lightIntensity3, lightIntensity4;
    float4 color, color1, color2, color3, color4;

    // Calculate the different amounts of light on this pixel based on the positions of the lights.
    lightIntensity1 = saturate(dot(input.normal, input.lightPos1));
    lightIntensity2 = saturate(dot(input.normal, input.lightPos2));
    lightIntensity3 = saturate(dot(input.normal, input.lightPos3));
    lightIntensity4 = saturate(dot(input.normal, input.lightPos4));

    // Determine the diffuse color amount of each of the four lights.
    color1 = diffuseColor[0] * lightIntensity1;
    color2 = diffuseColor[1] * lightIntensity2;
    color3 = diffuseColor[2] * lightIntensity3;
    color4 = diffuseColor[3] * lightIntensity4;

    // Sample the texture pixel at this location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    // Multiply the texture pixel by the combination of all four light colors to get the final result.
    color = saturate(color1 + color2 + color3 + color4) * textureColor;
	
    return color;
}