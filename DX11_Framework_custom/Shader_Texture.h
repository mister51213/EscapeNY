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
// Class name: Shader_Texture
////////////////////////////////////////////////////////////////////////////////
class Shader_Texture:public Shader
{

public:
	Shader_Texture() = default;
	~Shader_Texture() = default;

	virtual bool Initialize( ID3D11Device* pDevice) override;

	// CODE_CHANGE made function const and override
	virtual void Render( ID3D11DeviceContext*pContext )const override;

protected:
	bool InitializeShaderTextureCommon( 
		ID3D11Device *pDevice, 
		const std::wstring &vsFilename,
		const std::wstring &psFilename);

	void RenderShaderTextureCommon( ID3D11DeviceContext *pContext )const;
protected:
};
