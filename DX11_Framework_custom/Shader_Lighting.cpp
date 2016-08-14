#include "Shader_Lighting.h"

Shader_Lighting::Shader_Lighting()
:
Shader(L"Shaders/DiffuseLight_vs.cso", L"Shaders/DiffuseLight_ps.cso"),
m_hWnd(NULL) // need this bc not in Initialize()
{}

Shader_Lighting::Shader_Lighting(HWND& hWnd)
    :
Shader(L"Shaders/DiffuseLight_vs.cso", L"Shaders/DiffuseLight_ps.cso"),
m_hWnd(NULL) // need this bc not in Initialize()
// TODO: remember to manually copy and paste cso's into Shader Folder
{}

Shader_Lighting::Shader_Lighting(const Shader_Lighting &)
{}

Shader_Lighting::~Shader_Lighting()
{}

// TODO: this isn't being called!!!!
bool Shader_Lighting::InitializeShader(
    ID3D11Device * pDevice, 
    const std::wstring & vsFilename, 
    const std::wstring & psFilename) // HWND member is initialized in constructor
{
	HRESULT result;
	ID3DBlob* errorMessage;
	//ID3DBlob* vertexShaderBuffer;
	//ID3DBlob* pixelShaderBuffer;

    D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;

    // Light constant buffer description
    D3D11_BUFFER_DESC lightBufferDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	//vertexShaderBuffer = 0;
	//pixelShaderBuffer = 0;

    //Load in the new light vertex shader.
	// Compile the vertex shader code.
	//result = D3DCompileFromFile(
 //       L"DiffuseLight_vs.hlsl",
 //       NULL, 
 //       NULL, 
 //       "main", 
 //       "vs_5_0", 
 //       D3D10_SHADER_ENABLE_STRICTNESS, 
 //       NULL, 
 //       &vertexShaderBuffer, 
 //       &errorMessage);
	////if(FAILED(result))
	////{
	////	// If the shader failed to compile it should have writen something to the error message.
	////	if(errorMessage)
	////	{
	////		OutputShaderErrorMessage(errorMessage, m_hWnd, vsFilename);
	////	}
	////	// If there was nothing in the error message then it simply could not find the shader file itself.
	////	else
	////	{
	////		MessageBox(m_hWnd, vsFilename, L"Missing Shader File", MB_OK);
	////	}
	////	return false;
	////}
 //   //Load in the new light pixel shader, Compile the pixel shader code.
	//result = D3DCompileFromFile(
 //       L"DiffuseLight_ps.hlsl",
 //       NULL, 
 //       NULL, 
 //       "main", 
 //       "ps_5_0", 
 //       D3D10_SHADER_ENABLE_STRICTNESS, 
 //       NULL,
 //       &pixelShaderBuffer, 
 //       &errorMessage);
	////if(FAILED(result))
	////{
	////	// If the shader failed to compile it should have writen something to the error message.
	////	if(errorMessage)
	////	{
	////		OutputShaderErrorMessage(errorMessage, m_hWnd, psFilename);
	////	}
	////	// If there was nothing in the error message then it simply could not find the file itself.
	////	else
	////	{
	////		MessageBox(m_hWnd, psFilename, L"Missing Shader File", MB_OK);
	////	}
	////	return false;
	////}
 //   	// Create the vertex shader from the buffer.
	//result = pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	//if(FAILED(result))
	//{
	//	return false;
	//}
	//// Create the pixel shader from the buffer.
	//result = pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	//if(FAILED(result))
	//{
	//	return false;
	//}

    // TODO:*********************************************
    // TODO: Try using D3DReadFileToBlob instead here:
    // TODO:*********************************************
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
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT; // TODO: should be DXGI_FORMAT_R32G32B32A32_FLOAT to match shader?
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

// add a third element for the normal vector for lighting
	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = pDevice->CreateInputLayout(polygonLayout, numElements, pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), 
					   &m_layout);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	//vertexShaderBuffer->Release();
	//vertexShaderBuffer = 0;
	//pixelShaderBuffer->Release();
	//pixelShaderBuffer = 0;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = pDevice->CreateSamplerState(&samplerDesc, &m_sampleState);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if(FAILED(result))
	{
		return false;
	}
// Here we setup the light constant buffer description which will handle the diffuse light color and light direction. Pay attention to the size of the constant buffers, if they are not multiples of 16 you need to pad extra space on to the end of them or the CreateBuffer function will fail. In this case the constant buffer is 28 bytes with 4 bytes padding to make it 32.

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = pDevice->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
    }

void Shader_Lighting::OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND& hwnd, LPCWSTR& shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

bool Shader_Lighting::SetShaderParameters(
    ID3D11DeviceContext * deviceContext, 
    XMMATRIX & worldMatrix, 
    XMMATRIX & viewMatrix, 
    XMMATRIX & projectionMatrix, 
    ID3D11ShaderResourceView * texture,
    XMFLOAT3 lightDirection, // should be 3 and 4?
	XMFLOAT4 diffuseColor) const
{

    HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;

	// Transpose the matrices to prepare them for the shader.
	XMMATRIX worldMatrixT = worldMatrix;
	XMMATRIX viewMatrixT = XMMatrixTranspose(viewMatrix);
	XMMATRIX projectionMatrixT = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	//dataPtr->world = worldMatrix;
	//dataPtr->view = viewMatrix;
	//dataPtr->projection = projectionMatrix;
  	dataPtr->world = worldMatrixT;
	dataPtr->view = viewMatrixT;
	dataPtr->projection = projectionMatrixT;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer.Get(), 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, m_matrixBuffer.GetAddressOf());

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightDirection = lightDirection;
	dataPtr2->padding = 0.0f;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_lightBuffer.Get(), 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, m_lightBuffer.GetAddressOf());

	return true;
}

// this function must be overloaded from parent
bool Shader_Lighting::Render(
    ID3D11DeviceContext *deviceContext, 
    int indexCount, 
    XMMATRIX worldMatrix, 
    XMMATRIX viewMatrix,
    XMMATRIX projectionMatrix, 
    ID3D11ShaderResourceView* texture, 
    XMFLOAT3 lightDirection, 
    XMFLOAT4 diffuseColor) const
{
    	bool result;
	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(
        deviceContext, 
        worldMatrix, 
        viewMatrix, 
        projectionMatrix, 
        texture, 
        lightDirection, 
        diffuseColor);
	if(!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

void Shader_Lighting::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount) const
{
    // Set the vertex input layout.
	deviceContext->IASetInputLayout( m_layout.Get() );

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader( m_vertexShader.Get(), NULL, 0 );
	deviceContext->PSSetShader( m_pixelShader.Get(), NULL, 0 );

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers( 0, 1, m_sampleState.GetAddressOf() );

  	// Render the triangle.
	//deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}
