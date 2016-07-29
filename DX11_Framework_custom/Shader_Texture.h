#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: Shader_Texture.h
////////////////////////////////////////////////////////////////////////////////
//////////////
// INCLUDES //
//////////////
#include "Shader.h"
using namespace DirectX;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Forward declaration(s)
////////////////////////////////////////////////////////////////////////////////
//class Model;
////////////////////////////////////////////////////////////////////////////////
// Class name: Shader_Texture
////////////////////////////////////////////////////////////////////////////////
class Shader_Texture:public Shader
{

public:
	Shader_Texture();
	Shader_Texture( const Shader_Texture& );
	~Shader_Texture();

    // TODO: Why does color shader pass this as ref, but texture shader doesnt?
	//bool Render

	virtual bool InitializeShader( 
        ID3D11Device*, 
        HWND, 
		const std::wstring &VertexShaderFilename, 
        const std::wstring &PixelShaderFilename, 
		const Model &crModel );

	bool compileShader( 
        HWND WinHandle, 
        const std::wstring &ShaderFilename, 
        const std::string &ShaderEntryFunction,
		const std::string &ShaderModelVersion, 
        ID3DBlob **ppShaderBuffer, 
        ID3DBlob **ppErrorMessage );

	void outputShaderErrorMessage( 
        ID3D10Blob* pErrorMessage, 
        HWND WinHandle, 
        const std::wstring &ShaderFilename );

	virtual bool SetShaderParameters( 
        ID3D11DeviceContext*, 
        XMMATRIX &, 
        XMMATRIX &, 
        XMMATRIX &, 
        ID3D11ShaderResourceView* );
	virtual void renderShader( ID3D11DeviceContext* );

private:
	comptr<ID3D11SamplerState> m_sampleState;
};
