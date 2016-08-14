// 
// TODO: Change shader functionality to using built-in shader header files instead of .cso
// files that need to be packaged with the final .exe file (before we actually ship the game)
//

////////////////////////////////////////////////////////////////////////////////
// Filename: Shader_Texture.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Shader_Texture.h"
#include "Model.h"

Shader_Texture::Shader_Texture()
	:
	Shader(L"Shaders/texture_vs.cso", L"Shaders/texture_ps.cso")
{}

Shader_Texture::~Shader_Texture()
{}

bool Shader_Texture::InitializeShader( 
    ID3D11Device* pDevice, 
    const std::wstring & vsFilename,
    const std::wstring & psFilename)
{	
	// Initialize the pointers this function will use to null.
	comptr<ID3D10Blob> pVertexShaderBuffer, pPixelShaderBuffer;

	// Compile the vertex shader code.
	HRESULT hr = D3DReadFileToBlob( 
        vsFilename.c_str(), 
        pVertexShaderBuffer.GetAddressOf() );
	RETURN_IF_FAILED( hr );

	// Compile the pixel shader code.
	hr = D3DReadFileToBlob( 
		psFilename.c_str(), 
		pPixelShaderBuffer.GetAddressOf() );

	// Create the vertex shader from the buffer.
	hr = pDevice->CreateVertexShader(
		pVertexShaderBuffer->GetBufferPointer(),
		pVertexShaderBuffer->GetBufferSize(),
		NULL,
		m_vertexShader.GetAddressOf() );
	RETURN_IF_FAILED( hr );

	// Create the pixel shader from the buffer.
	hr = pDevice->CreatePixelShader(
		pPixelShaderBuffer->GetBufferPointer(),
		pPixelShaderBuffer->GetBufferSize(),
		NULL,
		m_pixelShader.GetAddressOf() );
	RETURN_IF_FAILED( hr );
	
	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
  	auto polygonLayout = VertexPositionUVType::CreateLayoutDescriptions();

	// Create the vertex input layout.
	hr = pDevice->CreateInputLayout( polygonLayout.data(), polygonLayout.size(),
		pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(),
		m_layout.GetAddressOf() );
	RETURN_IF_FAILED( hr );

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc{};
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof( MatrixBufferType );
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hr = pDevice->CreateBuffer( &matrixBufferDesc, NULL, &m_matrixBuffer );
	RETURN_IF_FAILED( hr );

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
	hr = pDevice->CreateSamplerState( &samplerDesc, &m_sampleState );
	RETURN_IF_FAILED( hr );

	return true;
}

// CODE_CHANGE: made function const
bool Shader_Texture::SetShaderParameters(
    ID3D11DeviceContext* deviceContext, 
	const XMMATRIX &worldMatrix,
	const XMMATRIX &viewMatrix,
	const XMMATRIX &projectionMatrix,
    ID3D11ShaderResourceView* texture )const
{
	// Lock the constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	HRESULT result = deviceContext->Map( m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0,
		&mappedResource );
	RETURN_IF_FAILED( result );

    // TODO: Consolidate this into a common parent function
	MatrixBufferType data;

    // transposed elsewhere.
    data.world = worldMatrix;
	data.view = XMMatrixTranspose( viewMatrix );
    // TODO: Implement global transpose and then change to this:
   	//data.view = viewMatrix;
	data.projection = XMMatrixTranspose( projectionMatrix );
    // TODO: Implement global transpose and then change to this:
  	//data.projection = projectionMatrix;

	// Copy the matrices into the constant buffer.
	std::memmove( mappedResource.pData, &data, sizeof( MatrixBufferType ) );

	// Unlock the constant buffer.
	deviceContext->Unmap( m_matrixBuffer.Get(), 0 );

	// Set the position of the constant buffer in the vertex shader.
	unsigned int bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers( bufferNumber, 1, m_matrixBuffer.GetAddressOf() );

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources( 0, 1, &texture );

	return true;
}

// CODE_CHANGE: made function const
void Shader_Texture::RenderShader( ID3D11DeviceContext* deviceContext )const
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout( m_layout.Get() );

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader( m_vertexShader.Get(), NULL, 0 );
	deviceContext->PSSetShader( m_pixelShader.Get(), NULL, 0 );

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers( 0, 1, m_sampleState.GetAddressOf() );
}