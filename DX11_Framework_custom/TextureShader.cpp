////////////////////////////////////////////////////////////////////////////////
// Filename: textureshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "TextureShader.h"

TextureShader::TextureShader()
{
}

TextureShader::TextureShader( const TextureShader& other )
{}

TextureShader::~TextureShader()
{}

bool TextureShader::Initialize( ID3D11Device* device, HWND hwnd )
{
	// Initialize the vertex and pixel shaders.
	bool result = InitializeShader( device, hwnd, L"../Engine/texture.vs", L"../Engine/texture.ps" );
	RETURN_IF_FALSE( result );

	return true;
}

bool TextureShader::Render( ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture )
{
	// Set the shader parameters that it will use for rendering.
	bool result = SetShaderParameters( deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture );
	RETURN_IF_FALSE( result );

	// Now render the prepared buffers with the shader.
	RenderShader( deviceContext, indexCount );

	return true;
}

bool TextureShader::InitializeShader( ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename )
{	
	// Initialize the pointers this function will use to null.
	comptr<ID3D10Blob> errorMessage, vertexShaderBuffer, pixelShaderBuffer;

	// Compile the vertex shader code.
	HRESULT result = D3DCompileFromFile( vsFilename, NULL, NULL, "TextureVertexShader",
		"vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, vertexShaderBuffer.GetAddressOf(), 
		errorMessage.GetAddressOf() );
	if( FAILED( result ) )
	{
		// If the shader failed to compile it should have writen something to the error message.
		if( errorMessage )
		{
			OutputShaderErrorMessage( errorMessage.Get(), hwnd, vsFilename );
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox( hwnd, vsFilename, L"Missing Shader File", MB_OK );
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile( psFilename, NULL, NULL, "TexturePixelShader", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, pixelShaderBuffer.GetAddressOf(), 
		errorMessage.GetAddressOf() );
	if( FAILED( result ) )
	{
		// If the shader failed to compile it should have writen something to the error message.
		if( errorMessage )
		{
			OutputShaderErrorMessage( errorMessage.Get(), hwnd, psFilename );
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox( hwnd, psFilename, L"Missing Shader File", MB_OK );
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader( vertexShaderBuffer->GetBufferPointer(), 
		vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader );
	RETURN_IF_FAILED( result );

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader( pixelShaderBuffer->GetBufferPointer(), 
		pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader );
	RETURN_IF_FAILED( result );

	vector<D3D11_INPUT_ELEMENT_DESC> polygonLayout( 2 );
	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[ 0 ].SemanticName = "POSITION";
	polygonLayout[ 0 ].SemanticIndex = 0;
	polygonLayout[ 0 ].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[ 0 ].InputSlot = 0;
	polygonLayout[ 0 ].AlignedByteOffset = 0;
	polygonLayout[ 0 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[ 0 ].InstanceDataStepRate = 0;

	polygonLayout[ 1 ].SemanticName = "TEXCOORD";
	polygonLayout[ 1 ].SemanticIndex = 0;
	polygonLayout[ 1 ].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[ 1 ].InputSlot = 0;
	polygonLayout[ 1 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[ 1 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[ 1 ].InstanceDataStepRate = 0;

	// Create the vertex input layout.
	result = device->CreateInputLayout( polygonLayout.data(), polygonLayout.size(), 
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), 
		m_layout.GetAddressOf() );
	RETURN_IF_FAILED( result );

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc{};
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof( MatrixBufferType );
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer( &matrixBufferDesc, NULL, &m_matrixBuffer );
	RETURN_IF_FAILED( result );

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
	result = device->CreateSamplerState( &samplerDesc, &m_sampleState );
	RETURN_IF_FAILED( result );

	return true;
}

void TextureShader::OutputShaderErrorMessage( ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename )
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)( errorMessage->GetBufferPointer() );

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open( "shader-error.txt" );

	// Write out the error message.
	for( i = 0; i<bufferSize; i++ )
	{
		fout << compileErrors[ i ];
	}

	// Close the file.
	fout.close();

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox( hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK );
}

bool TextureShader::SetShaderParameters( ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
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

void TextureShader::RenderShader( ID3D11DeviceContext* deviceContext, int indexCount )
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout( m_layout.Get() );

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader( m_vertexShader.Get(), NULL, 0 );
	deviceContext->PSSetShader( m_pixelShader.Get(), NULL, 0 );

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers( 0, 1, m_sampleState.GetAddressOf() );

	// Render the triangle.
	deviceContext->DrawIndexed( indexCount, 0, 0 );
}
