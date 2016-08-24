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
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
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

    // Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));

    // Check if dot is greater than zero. If so, add diffuse color to ambient color. 
    if(lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (lightColor * lightIntensity);
    }

    // Saturate final light color
    color = saturate(color);

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    color = color * textureColor;

    return color;
}