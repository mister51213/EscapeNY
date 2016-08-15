////////////////////////////////////////////////////////////////////////////////
// Filename: Shader_Texture.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Shader_Texture.h"

bool Shader_Texture::Initialize( ID3D11Device * pDevice )
{
	return InitializeShaderTextureCommon( pDevice,
		L"Shaders/texture_vs.cso",
		L"Shaders/texture_ps.cso");
}

void Shader_Texture::Render( ID3D11DeviceContext * pContext ) const
{
	RenderShaderTextureCommon( pContext );
}

bool Shader_Texture::InitializeShaderTextureCommon( 
    ID3D11Device* pDevice, 
    const std::wstring & vsFilename,
    const std::wstring & psFilename)
{	
	bool result = InitializeShaderCommon( pDevice,
		vsFilename,
		psFilename );
	RETURN_IF_FALSE( result );

	// Create a texture sampler state description.
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[ 0 ] = 0;
	samplerDesc.BorderColor[ 1 ] = 0;
	samplerDesc.BorderColor[ 2 ] = 0;
	samplerDesc.BorderColor[ 3 ] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	HRESULT hr = pDevice->CreateSamplerState( &samplerDesc, &m_sampleState );
	RETURN_IF_FAILED( hr );

	return true;
}

// CODE_CHANGE: made function const
void Shader_Texture::RenderShaderTextureCommon(
	ID3D11DeviceContext *pContext )const
{
	RenderShaderCommon( pContext );

}