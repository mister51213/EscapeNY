////////////////////////////////////////////////////////////////////////////////
// File name: Shader.cpp
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Shader.h"

bool Shader::Initialize( ID3D11Device * pDevice )
{
	// Initialize the vertex and pixel shaders.
	bool result = InitializeShaderCommon(
		pDevice,
		L"Shaders/DiffuseLight_vs.cso",
		L"Shaders/DiffuseLight_ps.cso" );
	RETURN_IF_FALSE( result );

	///////////////////////////////////////////////////////////
	// Texture shader setup
	///////////////////////////////////////////////////////////
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

	///////////////////////////////////////////////////////////
	// Light shader setup
	///////////////////////////////////////////////////////////
	// Light constant buffer description
	D3D11_BUFFER_DESC lightBufferDesc{};
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//lightBufferDesc.ByteWidth = sizeof( LightBufferType );
	lightBufferDesc.ByteWidth = sizeof( LightSpotBase );
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hr = pDevice->CreateBuffer( &lightBufferDesc, NULL, &m_lightBuffer );
	RETURN_IF_FAILED( hr );

	return result;
}

void Shader::Render( ID3D11DeviceContext * pContext, ID3D11ShaderResourceView * pTextureView ) const
{
	//////////////////////////////////////////////////////
	//				Set shader resources				//
	//////////////////////////////////////////////////////

	// Set the position of the constant buffer in the vertex shader.
	unsigned int bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	pContext->VSSetConstantBuffers( bufferNumber, 1, m_matrixBuffer.GetAddressOf() );

	//bufferNumber = 1;
	// Set the light constant buffer in the pixel shader with the updated values.
	pContext->PSSetConstantBuffers( bufferNumber, 1, m_lightBuffer.GetAddressOf() );

	// Set the vertex input layout.
	pContext->IASetInputLayout( m_layout.Get() );

	// Set the sampler state in the pixel shader.
	pContext->PSSetSamplers( 0, 1, m_sampleState.GetAddressOf() );

	// Set shader resources
	pContext->PSSetShaderResources( 0, 1, &pTextureView );

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pContext->VSSetShader( m_vertexShader.Get(), NULL, 0 );
	pContext->PSSetShader( m_pixelShader.Get(), NULL, 0 );
}

bool Shader::UpdateTransformBuffer(
	ID3D11DeviceContext * pContext, 
	const MatrixBufferType & BufferData) const
{
	bool result = UpdateConstantBuffer(
		pContext,
		reinterpret_cast<LPCVOID>( &BufferData ),
		sizeof( MatrixBufferType ),
		m_matrixBuffer.Get()
	);

	return result;
}

//bool Shader::UpdateLightBuffer( ID3D11DeviceContext * pContext, const LightBufferType & BufferData ) const
//{
//	bool result = UpdateConstantBuffer(
//		pContext,
//		reinterpret_cast<LPCVOID>( &BufferData ),
//		sizeof( LightBufferType ),
//		m_lightBuffer.Get()
//	);
//
//	return result;
//}
//
//bool Shader::UpdateLightBuffer( 
//	ID3D11DeviceContext * pContext, 
//	const SpotLightBuffer & BufferData ) const
//{
//	auto bufferSize = sizeof( BufferData );
//	bool result = UpdateConstantBuffer(
//		pContext,
//		reinterpret_cast<LPCVOID>( &BufferData ),
//		bufferSize,
//		m_lightBuffer.Get()
//	);
//
//	return result;
//}
//
//bool Shader::UpdateLightBuffer( ID3D11DeviceContext * pContext, const LightSpotBase & BufferData ) const
//{
//	auto bufferSize = sizeof( BufferData );
//	bool result = UpdateConstantBuffer(
//		pContext,
//		reinterpret_cast<LPCVOID>( &BufferData ),
//		bufferSize,
//		m_lightBuffer.Get()
//	);
//
//	return result;
//}

bool Shader::UpdateConstantBuffer(
	ID3D11DeviceContext * pContext, 
	LPCVOID BufferData,
	size_t BufferSize,
	ID3D11Buffer *pConstantBuffer )const
{
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	// Lock the constant buffer so it can be written to.
	HRESULT result = pContext->Map(
		pConstantBuffer,
		0, D3D11_MAP_WRITE_DISCARD,
		0, &mappedResource );
	RETURN_IF_FAILED( result );

	CopyMemory( mappedResource.pData, BufferData, BufferSize );

	// Unlock the constant buffer.
	pContext->Unmap( pConstantBuffer, 0 );

	return true;
}

bool Shader::InitializeShaderCommon(
	ID3D11Device * pDevice, 
	const std::wstring & vsFilename, 
	const std::wstring & psFilename )
{
	///////////////////////////////////////////////////////////
	// Common shader setup
	///////////////////////////////////////////////////////////
	// Initialize the variables for this function
	HRESULT result = S_OK;
	comptr<ID3D10Blob> pVertexShaderBuffer, pPixelShaderBuffer;

	// Load the vertex shader code.
	result = D3DReadFileToBlob(
		vsFilename.c_str(),
		pVertexShaderBuffer.GetAddressOf() );
	RETURN_IF_FAILED( result );

	// Load the pixel shader code.
	result = D3DReadFileToBlob(
		psFilename.c_str(),
		pPixelShaderBuffer.GetAddressOf() );
	RETURN_IF_FAILED( result );

	// Create the vertex shader from the buffer.
	result = pDevice->CreateVertexShader(
		pVertexShaderBuffer->GetBufferPointer(),
		pVertexShaderBuffer->GetBufferSize(),
		NULL,
		m_vertexShader.GetAddressOf() );
	RETURN_IF_FAILED( result );

	// Create the pixel shader from the buffer.
	result = pDevice->CreatePixelShader(
		pPixelShaderBuffer->GetBufferPointer(),
		pPixelShaderBuffer->GetBufferSize(),
		NULL,
		m_pixelShader.GetAddressOf() );
	RETURN_IF_FAILED( result );

	// Get appropiate layout description for this shader
	auto layoutDesciptions = VertexBufferTypeAllInOne::CreateLayoutDescriptions();
	
	// Create the vertex input layout.
	result = pDevice->CreateInputLayout( 
		layoutDesciptions.data(), 
		layoutDesciptions.size(), 
		pVertexShaderBuffer->GetBufferPointer(), 
		pVertexShaderBuffer->GetBufferSize(),
		&m_layout );
	RETURN_IF_FAILED( result );

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc{};
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof( MatrixBufferType );
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = pDevice->CreateBuffer( &matrixBufferDesc, NULL, &m_matrixBuffer );
	RETURN_IF_FAILED( result );

	return SUCCEEDED(result);
}
