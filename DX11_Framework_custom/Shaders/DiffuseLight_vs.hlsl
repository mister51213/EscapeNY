////////////////////////////////////////////////////////////////////////////////
// Filename: DiffuseLight.vs
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////

// This will be INITIALIZED differently for EACH OBJECT
cbuffer WMatBuffer
{
    matrix objectWorldMatrix; // for objects - CHANGEs FOR EACH OBJECT / FRAME
};

// this is only INITIALIZED once per frame (for the camera)
cbuffer MatrixBuffer
{
    matrix worldMatrix; // for objects - CHANGEs FOR EACH OBJECT / FRAME
    matrix viewMatrix; // for camera - ONLY CHANGES ONCE PER FRAME
    matrix projectionMatrix; // for screen size - ONLY CHANGES ONCE
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
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
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
    // VERTICES FOR INDIVIDUAL OBJECTS
    output.position = mul(output.position, worldMatrix);

    // REPOSITIONING RELATIVE TO CAMERA
    output.position = mul(output.position, viewMatrix);
    // SCALING TO SCREEN RATIO (not size) - Vanishing point, view clipping NOT YET APPLIED
    output.position = mul(output.position, projectionMatrix);

    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    return output;
}

