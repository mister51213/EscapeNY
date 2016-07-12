////////////////////////////////////////////////////////////////////////////////
// Filename: colorshader.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ColorShader.h"

// class constructor initializes all the private pointers in the class to null.

ColorShader::ColorShader()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}


ColorShader::ColorShader( const ColorShader& other )
{}


ColorShader::~ColorShader()
{}

// Pass in the names of the HLSL shader files, color.vs and color.ps
bool ColorShader::Initialize( ID3D11Device* device, HWND hwnd )
{
	bool result;

	// Initialize the vertex and pixel shaders.
	result = InitializeShader(
		device,
		hwnd,
		L"color.vs", L"color.ps" );
	if( !result )
	{
		return false;
	}

	return true;
}

// call the shutdown of the shader.
void ColorShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}

// First set the parameters inside the shader using the SetShaderParameters 
// function. Once the parameters are set then call RenderShader to draw 
// the green triangle using the HLSL shader.

bool ColorShader::Render( ID3D11DeviceContext* deviceContext,
	int indexCount,
	XMMATRIX & worldMatrix,
	XMMATRIX & viewMatrix,
	XMMATRIX & projectionMatrix )
{
	bool result;

	// Set the shader parameters to use for rendering.
	result = SetShaderParameters( deviceContext, worldMatrix, viewMatrix, projectionMatrix );
	if( !result )
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader( deviceContext, indexCount );

	return true;
}

// This function is what actually loads the shader files and 
// makes it usable to DirectX and the GPU. 

bool ColorShader::InitializeShader( ID3D11Device* device,
	HWND hwnd,
	WCHAR* vsFilename,
	WCHAR* psFilename )
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[ 2 ];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//  Compile the shader programs into buffers. We pass the name of the 
	// shader file, the name of the shader, the shader version (5.0 in DirectX 11), 
	// and the buffer to compile the shader into.

	// Compile the vertex shader code.
	result = D3DCompileFromFile( vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage );
	if( FAILED( result ) )
	{
		// If the shader failed to compile it should have writen something to the error message.
		if( errorMessage )
		{
			OutputShaderErrorMessage( errorMessage, hwnd, vsFilename );
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox( hwnd, vsFilename, L"Missing Shader File", MB_OK );
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile( psFilename,
		NULL,
		NULL,
		"ColorPixelShader",
		"ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&pixelShaderBuffer,
		&errorMessage );
	if( FAILED( result ) )
	{
		// If the shader failed to compile it should have writen something to the error message.
		if( errorMessage )
		{
			OutputShaderErrorMessage( errorMessage, hwnd, psFilename );
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
	result = device->CreateVertexShader( vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		NULL,
		&m_vertexShader );
	if( FAILED( result ) )
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader( pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(),
		NULL,
		&m_pixelShader );
	if( FAILED( result ) )
	{
		return false;
	}

	///////////////////////////////////////////////////////////////////////////////////
	// Create the layout of the VERTEX DATA that will be processed by the shader.    //
	// We indicate the usage of each element in the layout to the shader by labeling //
	// the first one POSITION and the second one COLOR.                              //
	///////////////////////////////////////////////////////////////////////////////////

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the Model class and in the shader.
	polygonLayout[ 0 ].SemanticName = "POSITION";
	polygonLayout[ 0 ].SemanticIndex = 0;
	polygonLayout[ 0 ].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[ 0 ].InputSlot = 0;
	polygonLayout[ 0 ].AlignedByteOffset = 0;
	polygonLayout[ 0 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[ 0 ].InstanceDataStepRate = 0;

	polygonLayout[ 1 ].SemanticName = "COLOR";
	polygonLayout[ 1 ].SemanticIndex = 0;
	polygonLayout[ 1 ].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[ 1 ].InputSlot = 0;
	polygonLayout[ 1 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[ 1 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[ 1 ].InstanceDataStepRate = 0;

	// release the vertex and pixel shader buffers since they are 
	// no longer needed once the layout has been created.

	// Get a count of the elements in the layout.
	numElements = sizeof( polygonLayout ) / sizeof( polygonLayout[ 0 ] );

	// Create the vertex input layout.
	result = device->CreateInputLayout( polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout );
	if( FAILED( result ) )
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// setup constant buffer to interface with the shader.
	// it will be updating every frame, so set buffer usage to dynamic.

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof( MatrixBufferType );
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer( &matrixBufferDesc, NULL, &m_matrixBuffer );
	if( FAILED( result ) )
	{
		return false;
	}

	return true;
}

// Release the four interfaces that were setup in the InitializeShader function.

void ColorShader::ShutdownShader()
{
	// Release the matrix constant buffer.
	if( m_matrixBuffer )
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the layout.
	if( m_layout )
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if( m_pixelShader )
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if( m_vertexShader )
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}

void ColorShader::OutputShaderErrorMessage( ID3D10Blob* errorMessage,
	HWND hwnd,
	WCHAR* shaderFilename )
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

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to 
	// check the text file for compile errors.
	MessageBox( hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK );

	return;
}

//////////////////////////////////////////////////////////////////////////////////
// This function makes setting the global variables in the shader easier. 
// The matrices used in this function are created inside the Graphics class, 
// after which this function is called to send them from there into the vertex 
// shader during the Render function call.
//////////////////////////////////////////////////////////////////////////////////

bool ColorShader::SetShaderParameters( ID3D11DeviceContext* deviceContext,
	XMMATRIX & worldMatrix,
	XMMATRIX & viewMatrix,
	XMMATRIX & projectionMatrix )
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// DirectX 11 requires transposing matrices before sending them into the shader
	worldMatrix = XMMatrixTranspose( worldMatrix );
	viewMatrix = XMMatrixTranspose( viewMatrix );
	projectionMatrix = XMMatrixTranspose( projectionMatrix );

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map( m_matrixBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource );
	if( FAILED( result ) )
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap( m_matrixBuffer, 0 );

	// Set the position of the constant buffer in the HLSL vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers( bufferNumber, 1, &m_matrixBuffer );

	return true;
}

// Second function called in the Render function;
// actually does the RENDERING.
void ColorShader::RenderShader( ID3D11DeviceContext* deviceContext, int indexCount )
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout( m_layout );

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader( m_vertexShader, NULL, 0 );
	deviceContext->PSSetShader( m_pixelShader, NULL, 0 );

	/////////////////////////
	// RENDER THE TRIANGLE //
	/////////////////////////
	deviceContext->DrawIndexed( indexCount, 0, 0 );

	return;
}
