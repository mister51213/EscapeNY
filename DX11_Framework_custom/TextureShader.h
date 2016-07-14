#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: textureshaderclass.h
////////////////////////////////////////////////////////////////////////////////
//////////////
// INCLUDES //
//////////////
#include "Includes.h"
#include "Utilities.h"
using namespace DirectX;
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureShaderClass
////////////////////////////////////////////////////////////////////////////////
class TextureShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	TextureShader();
	TextureShader( const TextureShader& );
	~TextureShader();

	bool Initialize( ID3D11Device*, HWND );
	bool Render( ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView* );

private:
	bool InitializeShader( ID3D11Device*, HWND, WCHAR*, WCHAR* );
	void OutputShaderErrorMessage( ID3D10Blob*, HWND, WCHAR* );

	bool SetShaderParameters( ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView* );
	void RenderShader( ID3D11DeviceContext*, int );

private:
	comptr<ID3D11VertexShader> m_vertexShader;
	comptr<ID3D11PixelShader> m_pixelShader;
	comptr<ID3D11InputLayout> m_layout;
	comptr<ID3D11Buffer> m_matrixBuffer;
	comptr<ID3D11SamplerState> m_sampleState;
};

