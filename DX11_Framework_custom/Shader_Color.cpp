////////////////////////////////////////////////////////////////////////////////
// Filename: Shader_Color.cpp
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "Shader_Color.h"
#include "Model.h"

// class constructor initializes all the private pointers in the class to null.

Shader_Color::Shader_Color():Shader(L"Shaders/color_vs.cso",L"Shaders/color_ps.cso")
{}

Shader_Color::Shader_Color( const Shader_Color& other ):Shader(L"Shaders/color_vs.cso",L"Shaders/color_ps.cso")
{}

Shader_Color::~Shader_Color()
{}

// Pass in the names of the HLSL shader files, color.vs and color.ps
//bool Shader_Color::Initialize( ID3D11Device* device, HWND hwnd, const Model &crModel )
//{
//	bool result;
//
//	// Initialize the vertex and pixel shaders.
//	result = InitializeShader(
//		device,
//		hwnd,
//        m_vsFilename,
//        m_psFilename,
//		crModel );
//	RETURN_IF_FALSE( result );
//
//	return true;
//}

// First set the parameters inside the shader using the SetShaderParameters 
// function. Once the parameters are set then call RenderShader to draw 
// the green triangle using the HLSL shader.

// TODO: Why does color shader pass this as ref, but texture shader doesnt?
//bool Shader_Color::Render( 
//    ID3D11DeviceContext* deviceContext,
//	XMMATRIX & worldMatrix,
//	XMMATRIX & viewMatrix,
//	XMMATRIX & projectionMatrix )
//{
//	// Set the shader parameters to use for rendering.
//	bool result = SetShaderParameters( 
//        deviceContext, 
//        worldMatrix, 
//        viewMatrix, 
//        projectionMatrix );
//	RETURN_IF_FALSE( result );
//
//	// Now render the prepared buffers with the shader.
//	RenderShader( deviceContext );
//
//	return true;
//}

// This function is what actually loads the shader files and 
// makes it usable to DirectX and the GPU. 

bool Shader_Color::InitializeShader( 
    ID3D11Device* device, 
    HWND hwnd, 
	WCHAR* vsFilename, // these aren't getting initialized for some reason
    WCHAR* psFilename, // these aren't getting initialized for some reason
    Model &crModel )
{
	//  Compile the shader programs into buffers. We pass the name of the 
	// shader file, the name of the shader, the shader version (5.0 in DirectX 11), 
	// and the buffer to compile the shader into.

	// Initialize the pointers this function will use to null.
	comptr<ID3D10Blob> errorMessage, vertexShaderBuffer, pixelShaderBuffer;

	// Compile the vertex shader code.
	HRESULT result = D3DCompileFromFile(
		vsFilename, 								// Shader filename
		nullptr,									// Pointer to D3D_SHADER_MACRO
		nullptr,									// Pointer to ID3DInclude interface
		"ColorVertexShader",						// Shader entry function name
		"vs_5_0",									// Shader type and version
		D3DCOMPILE_ENABLE_STRICTNESS,				// Shader flags 1
		NULL,										// Shader flags 2
		vertexShaderBuffer.GetAddressOf(),			// Address of shader blob
		errorMessage.GetAddressOf() );				// Address of shader error blob
	if( FAILED( result ) )
	{
		// If the shader failed to compile it should have writen something to the error message.
		if( errorMessage )
		{
			OutputShaderErrorMessage( errorMessage.Get(), hwnd, vsFilename );
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox( hwnd, vsFilename, L"Missing Shader File", MB_OK );
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile( 
		psFilename,										// Shader filename
		nullptr,										// Pointer to D3D_SHADER_MACRO
		nullptr,										// Pointer to ID3DInclude interface
		"ColorPixelShader",								// Shader entry function name
		"ps_5_0",										// Shader type and version
		D3DCOMPILE_ENABLE_STRICTNESS,					// Shader flags 1
		NULL,											// Shader flags 2
		pixelShaderBuffer.GetAddressOf(),				// Address of shader blob
		errorMessage.GetAddressOf() );					// Address of shader error blob
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

	// Now we will use the buffers of compiled shader code to create 
	// the shader objects themselves.

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader( 
		vertexShaderBuffer->GetBufferPointer(),					// Shader byte-code
		vertexShaderBuffer->GetBufferSize(),					// Shader size
		nullptr,												// ID3D11ClassLinkage interface pointer
		m_vertexShader.GetAddressOf() );						// Address of the vertex shader
	RETURN_IF_FAILED( result );

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader( 
		pixelShaderBuffer->GetBufferPointer(),					// Shader byte-code
		pixelShaderBuffer->GetBufferSize(),						// Shader size
		nullptr,												// ID3D11ClassLinkage interface pointer
		m_pixelShader.GetAddressOf() );							// Address of the vertex shader
	RETURN_IF_FAILED( result );

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the Model class and in the shader.
	auto polygonLayout = crModel.GetInputElementDescriptions();

	// Create the vertex input layout.
	result = device->CreateInputLayout( 
		polygonLayout.data(),								// Input layout description array
		polygonLayout.size(),								// Number of descrptions in array
		vertexShaderBuffer->GetBufferPointer(),				// Vertex buffer byte-code 
		vertexShaderBuffer->GetBufferSize(),				// Length of vertex buffer byte-code
		m_layout.GetAddressOf() );							// Address of ID3D11InputLayout interface
	RETURN_IF_FAILED( result );

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
	result = device->CreateBuffer( &matrixBufferDesc, NULL, m_matrixBuffer.GetAddressOf() );
	RETURN_IF_FAILED( result );

	return true;
}

void Shader_Color::OutputShaderErrorMessage( ID3D10Blob* errorMessage,
	HWND hwnd,
	WCHAR* shaderFilename )
{
	// Get a pointer to the error message text buffer.
	auto compileErrors = (char*)( errorMessage->GetBufferPointer() );

	// Get the length of the message.
	auto bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	ofstream fout( "shader-error.txt" );

	// Write out the error message.
	for( SIZE_T i = 0; i < bufferSize; ++i )
	{
		fout << compileErrors[ i ];
	}

	// Close the file.
	fout.close();

	// Pop a message up on the screen to notify the user to 
	// check the text file for compile errors.
	MessageBox( hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK );
}

//////////////////////////////////////////////////////////////////////////////////
// This function makes setting the global variables in the shader easier. 
// The matrices used in this function are created inside the Graphics class, 
// after which this function is called to send them from there into the vertex 
// shader during the Render function call.
//////////////////////////////////////////////////////////////////////////////////

bool Shader_Color::SetShaderParameters( 
    ID3D11DeviceContext* deviceContext,
	XMMATRIX & worldMatrix, 
    XMMATRIX & viewMatrix, 
    XMMATRIX & projectionMatrix )
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

	// DirectX 11 requires transposing matrices before sending them into the shader
	MatrixBufferType matrixBuffer{
		XMMatrixTranspose( worldMatrix ),
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