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

protected:
    bool Initialize(
        ID3D11Device* pDevice, 
        HWND WinHandle, 
        const Model &crModel) 
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

    virtual bool InitializeShader(
        ID3D11Device*,
        HWND, 
        WCHAR*,
        WCHAR*,
        const Model &crModel) = 0;

	//bool Render( ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView* );

protected:
    comptr<ID3D11VertexShader> m_vertexShader;
	comptr<ID3D11PixelShader> m_pixelShader;
	comptr<ID3D11InputLayout> m_layout;
	comptr<ID3D11Buffer> m_matrixBuffer;
	
    WCHAR* m_vsFilename;
    WCHAR* m_psFilename;
};