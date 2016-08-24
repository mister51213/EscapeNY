#include "Shader_Lighting.h"

Shader_Lighting::Shader_Lighting()
	:
	Shader( L"Shaders/DiffuseLight_vs.cso", L"Shaders/DiffuseLight_ps.cso" )
{
}

Shader_Lighting::Shader_Lighting( const Shader_Lighting & )
{}

Shader_Lighting::~Shader_Lighting()
{}

vector<D3D11_INPUT_ELEMENT_DESC> Shader_Lighting::InitChild( ID3D11Device * pDevice )
{
	/* If the constant buffer sizes are not multiples of 16, must pad them w extra space or CreateBuffer()
	will fail. In this case the constant buffer is 28 bytes with 4 bytes padding to make it 32. */
	// LIGHT BUFFER DESCRIPTION
	D3D11_BUFFER_DESC lightBufferDesc = LightBufferType::CreateLightDescription();

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	pDevice->CreateBuffer( &lightBufferDesc, NULL, &m_lightBuffer );

	// SAMPLER DESCRIPTION
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc = CreateSamplerDescription();
	pDevice->CreateSamplerState( &samplerDesc, &m_sampleState );

	return VertexPositionUVNormalType::CreateLayoutDescriptions();
}

bool Shader_Lighting::SetShaderParameters_CHILD( ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView * texture, ILight* pLight ) const
{
	////////////////////////////////////////////////////////////////////////
	// LIGHTING RELATED
	////////////////////////////////////////////////////////////////////////
	// Lock the light constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	bool result = deviceContext->Map( m_lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	RETURN_IF_FALSE( result );

	ILightDirection* pLightDiffuse = dynamic_cast<ILightDirection*>(pLight);

	LightBufferType LightBuff{
		pLightDiffuse->GetAmbient(),
		pLightDiffuse->GetColor(),
		pLightDiffuse->GetDirection(),
		0.f };

	// Copy the LIGHTING variables into the constant buffer.
	auto bufferLength = sizeof( LightBufferType );
	CopyMemory( mappedResource.pData, &LightBuff, bufferLength );

	// Unlock the constant buffer.
	deviceContext->Unmap( m_lightBuffer.Get(), 0 );

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers( 0, 1, m_lightBuffer.GetAddressOf() );

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources( 0, 1, &texture );
	return true;
}

void Shader_Lighting::RenderChild( ID3D11DeviceContext * deviceContext/*, int indexCount*/ ) const
{
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers( 0, 1, m_sampleState.GetAddressOf() );

	return;
}
