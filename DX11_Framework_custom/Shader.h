////////////////////////////////////////////////////////////////////////////////
// File name: Shader.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "Includes.h"
#include "Utilities.h"
using namespace DirectX;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Forward declaration(s)
////////////////////////////////////////////////////////////////////////////////
class Model;

class Shader 
{
public:
    Shader(WCHAR* vs, WCHAR* ps):
    m_vsFilename(vs),
    m_psFilename(ps)
    {}
    protected:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
    bool Initialize(
        ID3D11Device* pDevice, 
        HWND WinHandle, 
        Model &crModel) 
    {
	// Initialize the vertex and pixel shaders.
	bool result = InitializeShader( 
		pDevice, 
		WinHandle, 
	    m_vsFilename, 
        m_psFilename, 
		crModel );
	RETURN_IF_FALSE( result );

	return true;
    }

    // TODO: Virtualize this function
// **** Why does color version pass as ref, but texture version doesnt?

    bool Render(
        ID3D11DeviceContext* deviceContext,
        XMMATRIX & worldMatrix,
        XMMATRIX & viewMatrix,
        XMMATRIX & projectionMatrix)
    {
        	// Set the shader parameters to use for rendering.
	bool result = SetShaderParameters( 
        deviceContext, 
        worldMatrix, 
        viewMatrix, 
        projectionMatrix );
	RETURN_IF_FALSE( result );

	// Now render the prepared buffers with the shader.
    RenderShader(deviceContext);

	return true;
    }

    virtual void RenderShader(ID3D11DeviceContext*) = 0;

    virtual bool SetShaderParameters(
        ID3D11DeviceContext*,
        XMMATRIX &,
        XMMATRIX &,
        XMMATRIX &) = 0;

public:
    virtual bool InitializeShader(
        ID3D11Device*,
        HWND, 
        WCHAR*,
        WCHAR*,
        Model &crModel)
    {
        return true;
    }

	//bool Render( ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView* );

public:
    comptr<ID3D11VertexShader> m_vertexShader;
	comptr<ID3D11PixelShader> m_pixelShader;
	comptr<ID3D11InputLayout> m_layout;
	comptr<ID3D11Buffer> m_matrixBuffer;
	
    WCHAR* m_vsFilename;
    WCHAR* m_psFilename;
};