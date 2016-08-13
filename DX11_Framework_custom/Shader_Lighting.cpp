#include "Shader_Lighting.h"

Shader_Lighting::Shader_Lighting(HWND hWnd)
    :
Shader(L"Shaders/lighting_vs.cso", L"Shaders/lighting_vs.cso")
// TODO: remember to manually copy and paste cso's into Shader Folder
{
    // set member pointers to null.
    m_vertexShader.Reset();
	m_pixelShader.Reset();
	m_layout.Reset();
	m_sampleState.Reset();
	m_matrixBuffer.Reset();
// Set the light constant buffer to null in the class constructor.
	m_lightBuffer.Reset();
    m_hWnd = hWnd;
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

// TODO: Add HWND in above function too. Here HWND has been added.
bool Shader_Lighting::InitializeShader(
    ID3D11Device * pDevice, 
    HWND hwnd, 
    LPCWSTR& vsFilename, 
    LPCWSTR& psFilename)
{
	HRESULT result;
	ID3DBlob* errorMessage;
	ID3DBlob* vertexShaderBuffer;
	ID3DBlob* pixelShaderBuffer;

    // TODO: Change element description here
    D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;

    // Light constant buffer description
    D3D11_BUFFER_DESC lightBufferDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

    //Load in the new light vertex shader.

	// Compile the vertex shader code.
	result = D3DCompileFromFile(
        vsFilename, 
        NULL, 
        NULL, 
        "LightVertexShader", 
        "vs_5_0", 
        D3D10_SHADER_ENABLE_STRICTNESS, 
        0, 
        NULL, 
        &vertexShaderBuffer, 
        &errorMessage, 
        NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}
//Load in the new light pixel shader.

	// Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
				       &pixelShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
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
// One of the major changes to the shader initialization is here in the polygonLayout. 
// We add a third element for the normal vector that will be used for lighting. 
// The semantic name is NORMAL and the format is the regular DXGI_FORMAT_R32G32B32_FLOAT which handles 3 floats for the x, y, and z of the normal vector. The layout will now match the expected input to the HLSL vertex shader.

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
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), 
					   &m_layout);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

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
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
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
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
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
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
    }

bool Shader_Lighting::SetShaderParameters(
    ID3D11DeviceContext * deviceContext, 
    XMMATRIX & worldMatrix, 
    XMMATRIX & viewMatrix, 
    XMMATRIX & projectionMatrix, 
    ID3D11ShaderResourceView * texture,
    XMVECTOR lightDirection, // should be 3 and 4?
	XMVECTOR diffuseColor) const
{

    HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;


	// Transpose the matrices to prepare them for the shader.
	XMMatrixTranspose(worldMatrix);
	XMMatrixTranspose(viewMatrix);
	XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
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
    XMVECTOR lightDirection, 
    XMVECTOR diffuseColor)
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
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}
