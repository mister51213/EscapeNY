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

    // child-specific functionality
    auto polygonLayout = InitChild(pDevice);

	// Create the vertex input layout.
	hr = pDevice->CreateInputLayout(
        polygonLayout.data(), 
        polygonLayout.size(), 
        pVertexShaderBuffer->GetBufferPointer(), 
        pVertexShaderBuffer->GetBufferSize(), 
        m_layout.GetAddressOf());
		RETURN_IF_FAILED( hr );

	// DESCRIPTION for dynamic matrix constant buffer that is in the vertex shader.
   	D3D11_BUFFER_DESC matrixBufferDesc = MatrixBufferType::CreateMatrixDescription();
	hr = pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
    RETURN_IF_FAILED( hr );

        // LIGHT BUFFER DESCRIPTION
    D3D11_BUFFER_DESC lightBufferDesc;
    lightBufferDesc = LightBufferType::CreateLightDescription();
    
    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    hr = pDevice->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
    RETURN_IF_FAILED(hr);

    return true;
}

bool Shader::Render(
    ID3D11DeviceContext* deviceContext,
    const XMMATRIX & worldMatrix,
    const XMMATRIX & viewMatrix,
    const XMMATRIX & projectionMatrix,
    ID3D11ShaderResourceView* texture,
    ILight* pLight )const
{
    // Set the shader parameters to use for rendering.
    // NOTE: texture is NULL by default and will be set only in CHILD texture class.
    bool result = SetShaderParameters(
        deviceContext,
        worldMatrix,
        viewMatrix,
        projectionMatrix,
        texture,
        pLight);
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

bool Shader::SetShaderParameters(
    ID3D11DeviceContext * deviceContext, 
    const XMMATRIX & worldMatrix, 
    const XMMATRIX & viewMatrix, 
    const XMMATRIX & projectionMatrix, 
    ID3D11ShaderResourceView * texture,
    ILight* pLight) const
{
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

// DirectX 11 requires transposing matrices before sending them into the shader
	MatrixBufferType matrixBuffer{
		worldMatrix,
		XMMatrixTranspose( viewMatrix ),
		XMMatrixTranspose( projectionMatrix )
	};

	// Copy the matrices into the constant buffer. // unsigned int = size_t
	size_t bufferLength = sizeof( MatrixBufferType );
	CopyMemory( mappedResource.pData, &matrixBuffer, bufferLength );

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer.Get(), 0);

	// Set the position of the constant buffer in the vertex shader.
	unsigned int bufferNumber = 0;
	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, m_matrixBuffer.GetAddressOf());

////////////////////////////////////////////////////////////////////////
// LIGHTING RELATED
////////////////////////////////////////////////////////////////////////
    // Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result)){return false;}

    ILightDirection* pLightDiffuse = (ILightDirection*)pLight;

    LightBufferType LightBuff{
        pLightDiffuse->GetAmbient(),
        pLightDiffuse->GetColor(),
        pLightDiffuse->GetDirection(),
        0.f };

	// Copy the LIGHTING variables into the constant buffer.
    bufferLength = sizeof( LightBufferType );
    CopyMemory(mappedResource.pData, &LightBuff, bufferLength);

    // Unlock the constant buffer.
	deviceContext->Unmap(m_lightBuffer.Get(), 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, m_lightBuffer.GetAddressOf());

    SetShaderParameters_CHILD(deviceContext, texture);

    return true;
}
