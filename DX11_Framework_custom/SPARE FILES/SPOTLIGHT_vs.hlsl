////////////////////////////////////////////////////////////////////////////////
// Filename: DiffuseLight.vs
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////

// This will be INITIALIZED differently for EACH OBJECT
cbuffer WMatBuffer
{
    matrix objectWorldMatrix;
};

// this is only INITIALIZED once per frame (for the camera)
cbuffer MatrixBuffer
{
    matrix worldMatrix; // TODO: remove this after implementing above cbuffer
    matrix viewMatrix;
    matrix projectionMatrix;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION; // do the math to make it relative to screen coordinates
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 world : TEXCOORD1; // tells us the world position of the point on the surface
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    output.position = float4(input.position, 1.0f);
    //input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    //output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, worldMatrix);
    // tells us the world position of the point on the surface
	output.world = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    return output;
}

