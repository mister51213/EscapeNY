#pragma once

#include "Shader_Texture.h"

class Shader_Lighting:public Shader_Texture
{
	Shader_Lighting() = default;

	bool Initialize( ID3D11Device* pDevice ) override;
	void Render( ID3D11DeviceContext* pContext )const override;

private:
	void RenderShader( ID3D11DeviceContext* pContext )const;

private:

};