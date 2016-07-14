////////////////////////////////////////////////////////////////////////////////
// Filename: colorshader.h
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
// Class name: ColorShader
////////////////////////////////////////////////////////////////////////////////
class ColorShader
{
private:
    struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	ColorShader();
	ColorShader(const ColorShader&);
	~ColorShader();
    
	bool Initialize(ID3D11Device*, HWND);
	bool Render(ID3D11DeviceContext*, int, XMMATRIX &, XMMATRIX &, XMMATRIX & );

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX &, XMMATRIX &, XMMATRIX & );
	void RenderShader(ID3D11DeviceContext*, int);

private:
	comptr<ID3D11VertexShader> m_vertexShader;
	comptr<ID3D11PixelShader> m_pixelShader;
	comptr<ID3D11InputLayout> m_layout;
	comptr<ID3D11Buffer> m_matrixBuffer;
};