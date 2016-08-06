////////////////////////////////////////////////////////////////////////////////
// Filename: Shader_Color.cpp
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "Shader_Color.h"

Shader_Color::Shader_Color()
	:
    Shader(L"Shaders/color_vs.cso",L"Shaders/color_ps.cso")
{}

Shader_Color::~Shader_Color()
{}

bool Shader_Color::InitializeShader( 
    ID3D11Device* pDevice,
    const std::wstring & vsFilename,
    const std::wstring & psFilename )
{
	// Initialize the pointers this function will use to null.
	comptr<ID3D10Blob> pVertexShaderBuffer, pPixelShaderBuffer;

	// Load the vertex shader code.
	HRESULT hr = D3DReadFileToBlob(
		vsFilename.c_str(),
		pVertexShaderBuffer.GetAddressOf() );
	RETURN_IF_FAILED( hr );

	// Load the pixel shader code.
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
	// This setup needs to match the VertexType stucture in the Model class and in the shader.
    auto polygonLayout = VertexPositionColorType::CreateLayoutDescriptions();

	// Create the vertex input layout.
	hr = pDevice->CreateInputLayout(
		polygonLayout.data(),								// Input layout description array
		polygonLayout.size(),								// Number of descrptions in array
		pVertexShaderBuffer->GetBufferPointer(),				// Vertex buffer byte-code 
		pVertexShaderBuffer->GetBufferSize(),				// Length of vertex buffer byte-code
		m_layout.GetAddressOf() );							// Address of ID3D11InputLayout interface
	RETURN_IF_FAILED( hr );

	// setup constant buffer to interface with the shader.
	// it will be updating every frame, so set buffer usage to dynamic.

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc{};
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof( MatrixBufferType );
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hr = pDevice->CreateBuffer( 
		&matrixBufferDesc, 
		NULL, 
		m_matrixBuffer.GetAddressOf() );
	RETURN_IF_FAILED( hr );

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// This function makes setting the global variables in the shader easier. 
// The matrices used in this function are created inside the Graphics class, 
// after which this function is called to send them from there into the vertex 
// shader during the Render function call.
//////////////////////////////////////////////////////////////////////////////////

// Passing by reference in case we compile in 32 bit mode; if passed 
// by value in 32 bit mode, it will copy the value into 32 bit format, 
// and it will no longer be 16 byte aligned with simd register.

bool Shader_Color::SetShaderParameters( 
    ID3D11DeviceContext* deviceContext,
	XMMATRIX & worldMatrix, 
    XMMATRIX & viewMatrix, 
    XMMATRIX & projectionMatrix,
    ID3D11ShaderResourceView*)
{
	// Lock the constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	HRESULT result = deviceContext->Map(
		m_matrixBuffer.Get(),									// Subresource to be updated
		0,														// Subresource level
		D3D11_MAP_WRITE_DISCARD,								// Map type
		0,														// Map flag
		&mappedResource );										// Address of the mapped resource
	RETURN_IF_FAILED( result );

    // TODO: Consolidate this into a common parent function
    // DirectX 11 requires transposing matrices before sending them into the shader
	MatrixBufferType matrixBuffer{
		worldMatrix,
		XMMatrixTranspose( viewMatrix ),
		XMMatrixTranspose( projectionMatrix )
	};

	// Copy the matrices into the constant buffer.
	rsize_t bufferLength = sizeof( MatrixBufferType );
	memmove_s( mappedResource.pData, bufferLength, &matrixBuffer, bufferLength );

	// Unlock the constant buffer.
	deviceContext->Unmap( m_matrixBuffer.Get(), 0 );

	// Set the position of the constant buffer in the HLSL vertex shader.
	UINT bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers( bufferNumber, 1, m_matrixBuffer.GetAddressOf() );

	return true;
}

// Second function called in the Render function;
// actually does the RENDERING.
void Shader_Color::RenderShader( ID3D11DeviceContext* deviceContext )
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout( m_layout.Get() );

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader( m_vertexShader.Get(), NULL, 0 );
	deviceContext->PSSetShader( m_pixelShader.Get(), NULL, 0 );
}