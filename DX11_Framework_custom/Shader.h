////////////////////////////////////////////////////////////////////////////////
// File name: Shader.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "Includes.h"
#include "Utilities.h"
#include "ILightDirection.h"

using namespace DirectX;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Forward declaration(s)
////////////////////////////////////////////////////////////////////////////////

// To determine how the shaders should be laid out in a hierarchy or not, 
// need to determine what the shaders are doing.  Let's start with Initialization.

/*
Don't worry about what the functions are, worry about what the functions are doing.
///////////////////////////////
// Init:					 //
///////////////////////////////
Shader::Initialize is only calling ShaderChild::InitializeShader

// Common initialization tasks between all shaders
ShaderChild::InitializeShader is:
	Loading cso files for vertex and pixel shaders
	Creating the vertex and pixel shaders
	Using one of the VertexType structs' static CreateLayoutDescriptions
		functions to create a vector of D3D11_INPUT_ELEMENT_DESCs.
	Creating an ID3D11InputLayout using the vector just created
	Creating a constant buffer to hold the transformation matrices that will be
		used in the vertex shader

// Common initialization tasks for Shader_Texture and Shader_Lighting
	Create an ID3D11SamplerState using a D3D11_SAMPLER_DESC

// Specific initialization tasks for Shader_Lighting
	Create a constant buffer to hold the lighting information that will be used
	in the the pixel shader

///////////////////////////////
// Setting shader parameters //
///////////////////////////////
Shader::SetShaderParameters is pure virtual

// Common shader parameters to be set
ShaderChild::SetShaderParameters is:
	Transposing the world, view and projection matrices
	Mapping the matrices to the vertex shader constant buffer
	Setting the updated constant buffer as the current active constant buffer
		for the vertex shader

// Common shader parameters for Shader_Texture and Shader_lighting
	Set the pixel shader resource using a ID3D11ShaderResourceView 
		which represents a texture 

// Specific shader parameters for Shader_Lighting
	Map the lighting buffer to the pixel shader constant buffer
	Setting the updated constant buffer as the current active constant buffer
		for the pixel shader

///////////////////////////////
// Rendering				 //
///////////////////////////////
Shader::Render is just calling ShaderChild::SetShaderParameters and
	ShaderChild::RenderShader

// Common rendering tasks
ShaderChild::RenderShader is:
	Setting the input layout associtated with the type of shader, based on
		the VertexInputType struct in the shader
	Setting the ID3D11VertexShader and ID3D11PixelShader inherited 
		members as the active shaders
		
// Common rendering tasks for Shader_Texture and Shader_Lighting
	Setting the member ID3D11SamplerState as the current sampler state
*/

//Possible shader hierarchy for most code reuse
/*
// Parent
Shader
// Children of Shader
Shader_Texture, Shader_Color
// Child(ren) of Shader_Texture
Shader_Lighting
*/

// Possible shader hierarchy for consolidation
/*
// Requirements for actors, models and shaders (hlsl)
All actors have a color, default is white { 1.f, 1.f, 1.f, 1.f }
All actors have a texture index, default is eTexture::NONE ( NONE would be 0 )
All models use the VertexPositionUVNormalType ( for now )
All models will acquire the vertex position, texture coordinates and normals
	upon initialization

// All-in-one shader
Shader's VertexInputType will be the following:
struct VertexInputType
{
	float3 position : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal	: NORMAL;
	float4 color	: COLOR;
};

Shader's PixelInputType will be the following
struct PixelInputType
{
	float3 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal	: NORMAL;
	float4 color	: COLOR;
};

After transforming the vertices in the vertex shader and passing the results
	and forwarding the color and normals to the pixel shader

	Do light shader as before, the only change being multiplying the light 
		color with the color in the PixelInputType structure before multiplying
		the intensity.
*/

class Shader
{
public:
    Shader();
    Shader(const std::wstring &vs, const std::wstring &ps);

public:
    bool Initialize(ID3D11Device* pDevice);

    bool Render(
        ID3D11DeviceContext* deviceContext,
        const XMMATRIX & worldMatrix,
        const XMMATRIX & viewMatrix,
        const XMMATRIX & projectionMatrix,
        ID3D11ShaderResourceView* texture,
        ILight* pLight)const;

    virtual void RenderChild(ID3D11DeviceContext*)const = 0;

    bool SetShaderParameters(
        ID3D11DeviceContext* deviceContext,
        const XMMATRIX & worldMatrix,
        const XMMATRIX & viewMatrix,
        const XMMATRIX & projectionMatrix,
        ID3D11ShaderResourceView* texture,
        ILight* pLight)const;

    virtual bool SetShaderParameters_CHILD(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture) const = 0;

//public:
    //virtual bool InitializeShader(
    //    ID3D11Device*,
    //    const std::wstring &,
    //    const std::wstring &) = 0;
    //bool Shader::InitializeShaderCOMMON(
    //    ID3D11Device* pDevice,
    //    const std::wstring & vsFilename,
    //    const std::wstring & psFilename);

    virtual vector<D3D11_INPUT_ELEMENT_DESC> Shader::InitChild(ID3D11Device* pDevice) = 0;

protected:
	comptr<ID3D11VertexShader> m_vertexShader;
	comptr<ID3D11PixelShader> m_pixelShader;
	comptr<ID3D11InputLayout> m_layout;
	comptr<ID3D11Buffer> m_matrixBuffer;
    comptr<ID3D11Buffer> m_lightBuffer;

	std::wstring m_vsFilename;
	std::wstring m_psFilename;
};