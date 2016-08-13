#include "Shader_Lighting.h"

Shader_Lighting::Shader_Lighting()
    :
Shader(L"Shaders/lighting_vs.cso", L"Shaders/lighting_vs.cso")
// TODO: remember to manually copy and paste cso's into Shader Folder
{
    m_vertexShader.Reset();
	m_pixelShader.Reset();
	m_layout.Reset();
	m_sampleState.Reset();
	m_matrixBuffer.Reset();
// Set the light constant buffer to null in the class constructor.
	m_lightBuffer.Reset();
}

Shader_Lighting::Shader_Lighting(const Shader_Lighting &)
{}

Shader_Lighting::~Shader_Lighting()
{
}

//TODO: is this necessary w comptrs?
//void Shader_Lighting::Shutdown()
//{
//	// Shutdown the vertex and pixel shaders as well as the related objects.
//	ShutdownShader();
//
//	return;
//}

bool Shader_Lighting::InitializeShader(ID3D11Device * pDevice, const std::wstring & vsFilename, const std::wstring & psFilename)
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

	return true;}

bool Shader_Lighting::SetShaderParameters(ID3D11DeviceContext * deviceContext, XMMATRIX & worldMatrix, XMMATRIX & viewMatrix, XMMATRIX & projectionMatrix, ID3D11ShaderResourceView * texture) const
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

	return true;}

bool Shader_Lighting::Render(
    ID3D11DeviceContext *deviceContext, 
    int indexCount, 
    XMMATRIX worldMatrix, 
    XMMATRIX viewMatrix,
    XMMATRIX projectionMatrix, 
    ID3D11ShaderResourceView *texture, 
    XMVECTOR lightDirection, 
    XMVECTOR diffuseColor)
{
		// NOTE: texture is NULL by default and will be set only in CHILD texture class.
		bool result = SetShaderParameters(
			deviceContext,
			worldMatrix,
			viewMatrix,
			projectionMatrix,
			texture );
		RETURN_IF_FALSE( result );

		// Now render the prepared buffers with the shader.
		RenderShader( deviceContext );

		return true;}

void Shader_Lighting::RenderShader(ID3D11DeviceContext * deviceContext) const
{
    // Set the vertex input layout.
	deviceContext->IASetInputLayout( m_layout.Get() );

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader( m_vertexShader.Get(), NULL, 0 );
	deviceContext->PSSetShader( m_pixelShader.Get(), NULL, 0 );

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers( 0, 1, m_sampleState.GetAddressOf() );
}
