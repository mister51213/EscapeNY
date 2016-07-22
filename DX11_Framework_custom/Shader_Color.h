////////////////////////////////////////////////////////////////////////////////
// Filename: Shader_Color.h
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
////////////////////////////////////////////////////////////////////////////////
// Class name: Shader_Color
////////////////////////////////////////////////////////////////////////////////
class Shader_Color
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	Shader_Color();
	Shader_Color( const Shader_Color& );
	~Shader_Color();

	bool Initialize( ID3D11Device*, HWND, const Model &crModel );
	bool Render( ID3D11DeviceContext*, XMMATRIX &, XMMATRIX &, XMMATRIX & );

private:
	bool InitializeShader( ID3D11Device*, HWND, WCHAR*, WCHAR*, const Model &crModel );
	void OutputShaderErrorMessage( ID3D10Blob*, HWND, WCHAR* );

	bool SetShaderParameters( ID3D11DeviceContext*, XMMATRIX &, XMMATRIX &, XMMATRIX & );
	void RenderShader( ID3D11DeviceContext* );

private:
	comptr<ID3D11VertexShader> m_vertexShader;
	comptr<ID3D11PixelShader> m_pixelShader;
	comptr<ID3D11InputLayout> m_layout;
	comptr<ID3D11Buffer> m_matrixBuffer;
};