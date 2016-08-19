////////////////////////////////////////////////////////////////////////////////
// File name: Shader.cpp
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Shader.h"

Shader::Shader(){}

Shader::Shader( const std::wstring &vs, const std::wstring &ps)
{
	m_vsFilename = vs;
	m_psFilename = ps;
}

bool Shader::Initialize( ID3D11Device* pDevice )
{
	// Initialize the vertex and pixel shaders.
	//bool result = InitializeShader( pDevice, m_vsFilename, m_psFilename );
	//RETURN_IF_FALSE( result );
    //   return true;

    // Initialize the pointers this function will use to null.
	comptr<ID3D10Blob> pVertexShaderBuffer, pPixelShaderBuffer;

    // Read in compiled shader files
    HRESULT hr = D3DReadFileToBlob( m_vsFilename.c_str(), pVertexShaderBuffer.GetAddressOf() );
	RETURN_IF_FAILED( hr );
    hr = D3DReadFileToBlob( m_psFilename.c_str(), pPixelShaderBuffer.GetAddressOf() );
    
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
    unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	hr = pDevice->CreateInputLayout(
        polygonLayout, 
        numElements, 
        pVertexShaderBuffer->GetBufferPointer(), 
        pVertexShaderBuffer->GetBufferSize(), 
        m_layout.GetAddressOf());
		RETURN_IF_FAILED( hr );

	// DESCRIPTION for dynamic matrix constant buffer that is in the vertex shader.
   	D3D11_BUFFER_DESC matrixBufferDesc = MatrixBufferType::CreateMatrixDescription();
	hr = pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
    RETURN_IF_FAILED( hr );

    // This may or may not be implemented depending on child shader
    hr = InitChild(pDevice);

    return true;
}

// TODO: Make this NON virtual; shared by both children
//bool Shader::InitializeShaderCOMMON(
//    ID3D11Device* pDevice,
//    const std::wstring & vsFilename,
//    const std::wstring & psFilename) 
//{
 //   // Initialize the pointers this function will use to null.
	//comptr<ID3D10Blob> pVertexShaderBuffer, pPixelShaderBuffer;
 //   // Read in compiled shader files
 //   HRESULT hr = D3DReadFileToBlob( vsFilename.c_str(), pVertexShaderBuffer.GetAddressOf() );
	//RETURN_IF_FAILED( hr );
 //   hr = D3DReadFileToBlob( psFilename.c_str(), pPixelShaderBuffer.GetAddressOf() );
 //   
 //   // Create the vertex shader from the buffer.
	//hr = pDevice->CreateVertexShader(pVertexShaderBuffer->
 //       GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(),
	//	NULL, m_vertexShader.GetAddressOf() );
	//RETURN_IF_FAILED( hr );
	//// Create the pixel shader from the buffer.
	//hr = pDevice->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(),
 //       pPixelShaderBuffer->GetBufferSize(),NULL,m_pixelShader.GetAddressOf() );
	//RETURN_IF_FAILED( hr );
	//
 //   D3D11_INPUT_ELEMENT_DESC polygonLayout[3]; // Make vector w/ global utility, copy to array
 //   vector<D3D11_INPUT_ELEMENT_DESC> polyV = VertexPositionUVNormalType::CreateLayoutDescriptions();
 //   std::copy(polyV.begin(), polyV.end(), polygonLayout);
	//// Get a count of the elements in the layout.
 //   unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	//// Create the vertex input layout.
	//hr = pDevice->CreateInputLayout(
 //       polygonLayout, 
 //       numElements, 
 //       pVertexShaderBuffer->GetBufferPointer(), 
 //       pVertexShaderBuffer->GetBufferSize(), 
 //       m_layout.GetAddressOf());
	//	RETURN_IF_FAILED( hr );
	//// DESCRIPTION for dynamic matrix constant buffer that is in the vertex shader.
 //  	D3D11_BUFFER_DESC matrixBufferDesc = MatrixBufferType::CreateMatrixDescription();
	//hr = pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
 //   RETURN_IF_FAILED( hr );
 //   return true;
    // TODO: add this into initialize function
    // TODO: add sampler description, lightbuffer desc to child
//}


bool Shader::Render(
    ID3D11DeviceContext* deviceContext,
    const XMMATRIX & worldMatrix,
    const XMMATRIX & viewMatrix,
    const XMMATRIX & projectionMatrix,
    ID3D11ShaderResourceView* texture,
    FX* effect)const
{
    // Set the shader parameters to use for rendering.
    // NOTE: texture is NULL by default and will be set only in CHILD texture class.
    bool result = SetShaderParameters(
        deviceContext,
        worldMatrix,
        viewMatrix,
        projectionMatrix,
        texture,
        effect);
    RETURN_IF_FALSE(result);

    // Now render the prepared buffers with the shader.
        // Set the vertex input layout.
	deviceContext->IASetInputLayout( m_layout.Get() );

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader( m_vertexShader.Get(), NULL, 0 );
	deviceContext->PSSetShader( m_pixelShader.Get(), NULL, 0 );

    RenderChild(deviceContext);

    return true;
}

////////////////////////////////////////////////////////
// TODO: THIS WAS COPIED FROM SHADER_LIGHTING!!!!
// TODO: Compare it to the one in Shader_Color, consolidate
// TODO: in parent.
////////////////////////////////////////////////////////
bool Shader::SetShaderParameters(
    ID3D11DeviceContext * deviceContext, 
    const XMMATRIX & worldMatrix, 
    const XMMATRIX & viewMatrix, 
    const XMMATRIX & projectionMatrix, 
    ID3D11ShaderResourceView * texture,
    FX* effect
    /*XMFLOAT3 lightDirection, 
	XMFLOAT4 diffuseColor*/) const
{
    // TODO: Copied from shader_lighting - compare w shader_color function
    D3D11_MAPPED_SUBRESOURCE mappedResource{};
    /////////////////
    // WRITE to GPU
    /////////////////
	HRESULT result = deviceContext->Map(
        m_matrixBuffer.Get(), 
        0, 
        D3D11_MAP_WRITE_DISCARD, 
        0, 
        &mappedResource);
	RETURN_IF_FAILED( result );

// FASTER IMPLEMENTATION
    // DirectX 11 requires transposing matrices before sending them into the shader
	MatrixBufferType matrixBuffer{
		worldMatrix,
		XMMatrixTranspose( viewMatrix ),
		XMMatrixTranspose( projectionMatrix )
	};

	// Copy the matrices into the constant buffer. // unsigned int = size_t
	size_t bufferLength = sizeof( MatrixBufferType );
	CopyMemory( mappedResource.pData, &matrixBuffer, bufferLength );

#pragma region SLOW IMPLEMENTATION

 //   // TODO: BAD! Performance hit!
 //   /*
 //   https://msdn.microsoft.com/en-us/library/windows/desktop/ff476457(v=vs.85).aspx
 //   */
	//// Cast void pointer to GPU into a MatrixBufferType* pointer
	//MatrixBufferType* pMatrixBuff = (MatrixBufferType*)mappedResource.pData;

 //   /////////////////
 //   // READ from AND WRITE to GPU in same step = BAD!!!!
 //   /////////////////
 // 	pMatrixBuff->world = worldMatrix; // TODO: worldMat is transposed elsewhere. shouldnt be that way.
	//pMatrixBuff->view = XMMatrixTranspose(viewMatrix);
	//pMatrixBuff->projection = XMMatrixTranspose(projectionMatrix);

#pragma endregion

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer.Get(), 0);

	// Set the position of the constant buffer in the vertex shader.
	unsigned int bufferNumber = 0;
	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, m_matrixBuffer.GetAddressOf());

////////////////////////////////////////////////////////////////////////
// TODO: LIGHTING RELATED (new to Shader_Color)
////////////////////////////////////////////////////////////////////////
    // Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result)){return false;}

    LightBufferType LightBuff{
        effect->Color,
        effect->Direction,
        0.f };

	// Copy the LIGHTING variables into the constant buffer.
    bufferLength = sizeof( LightBufferType );
    CopyMemory(mappedResource.pData, &LightBuff, bufferLength);
 //   pLightBuff->diffuseColor = effect->Color;
	//pLightBuff->lightDirection = effect->Direction;
 //   pLightBuff->padding = 0.0f;

 // Unlock the constant buffer.
	deviceContext->Unmap(m_lightBuffer.Get(), 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;
	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, m_lightBuffer.GetAddressOf());

    SetShaderParameters_CHILD(deviceContext, texture);

    return true;
}
