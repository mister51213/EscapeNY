#include "Shader_Lighting.h"


bool Shader_Lighting::Initialize( ID3D11Device * pDevice )
{
	// Initialize the vertex and pixel shaders.
	bool result = InitializeShaderTextureCommon(
		pDevice,
		L"Shaders/DiffuseLight_vs.cso",
		L"Shaders/DiffuseLight_ps.cso" );
	RETURN_IF_FALSE( result );


	return result;
}

void Shader_Lighting::Render( ID3D11DeviceContext *pContext ) const
{
	// Now render the prepared buffers with the shader.
	RenderShader( pContext );
}

void Shader_Lighting::RenderShader( ID3D11DeviceContext * pContext ) const
{
	RenderShaderTextureCommon( pContext );

	int bufferNumber = 0;

	// Set the light constant buffer in the pixel shader with the updated values.
	pContext->PSSetConstantBuffers( bufferNumber, 1, m_lightBuffer.GetAddressOf() );

}
