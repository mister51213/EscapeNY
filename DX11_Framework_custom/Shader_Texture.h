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
	~Shader_Texture();

	virtual bool InitializeShader( 
        ID3D11Device*, 
        const std::wstring &,
        const std::wstring &) override;

	virtual bool SetShaderParameters( 
        ID3D11DeviceContext*, 
        XMMATRIX &, 
        XMMATRIX &, 
        XMMATRIX &, 
        ID3D11ShaderResourceView* ) const override;

	virtual void RenderShader( ID3D11DeviceContext* ) const;

private:
	comptr<ID3D11SamplerState> m_sampleState;
};
