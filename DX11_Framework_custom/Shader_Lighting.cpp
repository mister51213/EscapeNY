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
{// TODO: remember to manually copy and paste cso's into Shader Folder
}

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
    unsigned int numElements;

    // Initialize the pointers this function will use to null.
	comptr<ID3D10Blob> pVertexShaderBuffer, pPixelShaderBuffer;

    HRESULT hr = D3DReadFileToBlob( vsFilename.c_str(), pVertexShaderBuffer.GetAddressOf() );
	RETURN_IF_FAILED( hr );
    hr = D3DReadFileToBlob( psFilename.c_str(), pPixelShaderBuffer.GetAddressOf() );
    // Create the vertex shader from the buffer.
	hr = pDevice->CreateVertexShader(pVertexShaderBuffer->
        GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(),
		NULL, m_vertexShader.GetAddressOf() );
	RETURN_IF_FAILED( hr );
	// Create the pixel shader from the buffer.
	hr = pDevice->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(),
        pPixelShaderBuffer->GetBufferSize(),NULL,m_pixelShader.GetAddressOf() );
	RETURN_IF_FAILED( hr );
	
    D3D11_INPUT_ELEMENT_DESC polygonLayout[3]; // Make vector w/ global utility, copy to array
    vector<D3D11_INPUT_ELEMENT_DESC> polyV = VertexPositionUVNormalType::CreateLayoutDescriptions();
    std::copy(polyV.begin(), polyV.end(), polygonLayout);

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = pDevice->CreateInputLayout(polygonLayout, numElements, pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), &m_layout);
	if(FAILED(result))
	{
		return false;
	}

    // SAMPLER DESCRIPTION
   	D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc = CreateSamplerDescription();
	result = pDevice->CreateSamplerState(&samplerDesc, &m_sampleState);
	if(FAILED(result))
	{
		return false;
	}

	// DESCRIPTION for dynamic matrix constant buffer that is in the vertex shader.
   	D3D11_BUFFER_DESC matrixBufferDesc;
    matrixBufferDesc = MatrixBufferType::CreateMatrixDescription();
	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if(FAILED(result))
	{
		return false;
	}

    /*  If the constant buffer sizes are not multiples of 16, must pad them w extra space  or CreateBuffer() 
        will fail. In this case the constant buffer is 28 bytes with 4 bytes padding to make it 32. */

    // LIGHT BUFFER DESCRIPTION
    D3D11_BUFFER_DESC lightBufferDesc;
    lightBufferDesc = LightBufferType::CreateLightDescription();
	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = pDevice->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
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
    XMFLOAT3 lightDirection, // should be 3 and 4?
	XMFLOAT4 diffuseColor) const
{
    HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result)){return false;}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
  	dataPtr->world = worldMatrix; // TODO: worldMat is transposed elsewhere. shouldnt be that way.
	dataPtr->view = XMMatrixTranspose(viewMatrix);
	dataPtr->projection = XMMatrixTranspose(projectionMatrix);

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
	if(FAILED(result)){return false;}

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
