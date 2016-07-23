////////////////////////////////////////////////////////////////////////////////
// Filename: Shader_Texture.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Shader_Texture.h"
#include "Model.h"

Shader_Texture::Shader_Texture()
{
}

Shader_Texture::Shader_Texture( const Shader_Texture& other )
{}

Shader_Texture::~Shader_Texture()
{}

bool Shader_Texture::Initialize( ID3D11Device* pDevice, HWND WinHandle, const Model &crModel )
{
	// Initialize the vertex and pixel shaders.
	bool result = initializeShader( 
		pDevice, 
		WinHandle, 
		L"Shaders/texture_vs.cso", L"Shaders/texture_ps.cso", 
		crModel );
	RETURN_IF_FALSE( result );

	return true;
}

bool Shader_Texture::Render( ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture )
{
	// Set the shader parameters that it will use for rendering.
	bool result = setShaderParameters( deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture );
	RETURN_IF_FALSE( result );

	// Now render the prepared buffers with the shader.
	renderShader( deviceContext );

	return true;
}

bool Shader_Texture::initializeShader( ID3D11Device* pDevice, HWND WinHandle,
	const std::wstring &VertexShaderFilename, const std::wstring &PixelShaderFilename,
	const Model &crModel )
{	
	// Initialize the pointers this function will use to null.
	comptr<ID3D10Blob> pVertexShaderBuffer, pPixelShaderBuffer, pErrorMessage;

	// Compile the vertex shader code.
	HRESULT hr = D3DReadFileToBlob( VertexShaderFilename.c_str(), pVertexShaderBuffer.GetAddressOf() );
	RETURN_IF_FAILED( hr );
	/*bool result = compileShader( WinHandle, VertexShaderFilename, "TextureVertexShader", 
		"vs_5_0", pVertexShaderBuffer.GetAddressOf(), pErrorMessage.GetAddressOf() );*/

	// Create the vertex shader from the buffer.
	std::string vsCode( reinterpret_cast<char*>( pVertexShaderBuffer->GetBufferPointer() ) );
	auto vsSize = pVertexShaderBuffer->GetBufferSize();
	hr = pDevice->CreateVertexShader( pVertexShaderBuffer->GetBufferPointer(),
		pVertexShaderBuffer->GetBufferSize(), NULL, m_vertexShader.GetAddressOf() );
	RETURN_IF_FAILED( hr );

	// Compile the pixel shader code.
	hr = D3DReadFileToBlob( PixelShaderFilename.c_str(), pPixelShaderBuffer.GetAddressOf() );
	/*result = compileShader( WinHandle, PixelShaderFilename, "TexturePixelShader",
		"ps_5_0", pPixelShaderBuffer.GetAddressOf(), pErrorMessage.GetAddressOf() );*/

	// Create the pixel shader from the buffer.
	hr = pDevice->CreatePixelShader( pPixelShaderBuffer->GetBufferPointer(),
		pPixelShaderBuffer->GetBufferSize(), NULL, m_pixelShader.GetAddressOf() );
	RETURN_IF_FAILED( hr );

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	auto polygonLayout = crModel.GetInputElementDescriptions();

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

bool Shader_Texture::compileShader( HWND WinHandle, const std::wstring & ShaderFilename,
	const std::string & ShaderEntryFunction, const std::string & ShaderModelVersion, 
	ID3DBlob **ppShaderBuffer, ID3DBlob **ppErrorMessage )
{
	HRESULT result = D3DCompileFromFile( ShaderFilename.c_str(), NULL, NULL,
		ShaderEntryFunction.c_str(), ShaderModelVersion.c_str(), D3D10_SHADER_ENABLE_STRICTNESS, 0,
		ppShaderBuffer, ppErrorMessage );
	if( FAILED( result ) )
	{
		// If the shader failed to compile it should have writen something to the error message.
		if( *ppErrorMessage )
		{
			outputShaderErrorMessage( *ppErrorMessage, WinHandle, ShaderFilename );
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox( WinHandle, ShaderFilename.c_str(), L"Missing Shader File", MB_OK );
		}

		return false;
	}

	return true;
}

void Shader_Texture::outputShaderErrorMessage( ID3D10Blob* pErrorMessage, HWND WinHandle, const std::wstring &ShaderFilename )
{
	// Open a file to write the error message to.
	ofstream fout( "shader-error.txt" );

	// Write out the error message.
	fout << std::string( reinterpret_cast<char*>( pErrorMessage->GetBufferPointer() ) );

	// Close the file.
	fout.close();

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox( WinHandle, L"Error compiling shader.  Check shader-error.txt for message.", ShaderFilename.c_str(), MB_OK );
}

bool Shader_Texture::setShaderParameters( ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture )
{
	// Lock the constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	HRESULT result = deviceContext->Map( m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0,
		&mappedResource );
	RETURN_IF_FAILED( result );

	// Transpose the matrices to prepare them for the shader.
	MatrixBufferType data;
	data.world = XMMatrixTranspose( worldMatrix );
	data.view = XMMatrixTranspose( viewMatrix );
	data.projection = XMMatrixTranspose( projectionMatrix );

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

void Shader_Texture::renderShader( ID3D11DeviceContext* deviceContext )
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout( m_layout.Get() );

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader( m_vertexShader.Get(), NULL, 0 );
	deviceContext->PSSetShader( m_pixelShader.Get(), NULL, 0 );

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers( 0, 1, m_sampleState.GetAddressOf() );
}