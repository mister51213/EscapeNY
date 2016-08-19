#include "Shader_Lighting.h"

Shader_Lighting::Shader_Lighting()
    :
Shader(L"Shaders/DiffuseLight_vs.cso", L"Shaders/DiffuseLight_ps.cso")
{// TODO: copy cso's into Shader Folder
}

Shader_Lighting::Shader_Lighting(const Shader_Lighting &)
{}

Shader_Lighting::~Shader_Lighting()
{}

vector<D3D11_INPUT_ELEMENT_DESC> Shader_Lighting::InitChild(ID3D11Device * pDevice)
{
/* If the constant buffer sizes are not multiples of 16, must pad them w extra space or CreateBuffer()
    will fail. In this case the constant buffer is 28 bytes with 4 bytes padding to make it 32. */

    // SAMPLER DESCRIPTION
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc = CreateSamplerDescription();
    pDevice->CreateSamplerState(&samplerDesc, &m_sampleState);

    // LIGHT BUFFER DESCRIPTION
    //D3D11_BUFFER_DESC lightBufferDesc;
    //lightBufferDesc = LightBufferType::CreateLightDescription();
    //// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    //result = pDevice->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
    //RETURN_IF_FAILED(result);

    return VertexPositionUVNormalType::CreateLayoutDescriptions();
}
//
//bool Shader_Lighting::InitializeShader(
//    ID3D11Device * pDevice, 
//    const std::wstring & vsFilename, 
//    const std::wstring & psFilename) // HWND member is initialized in constructor
//{
//	HRESULT result;
//
//    // Initialize the pointers this function will use to null.
//	comptr<ID3D10Blob> pVertexShaderBuffer, pPixelShaderBuffer;
//
//    HRESULT hr = D3DReadFileToBlob( vsFilename.c_str(), pVertexShaderBuffer.GetAddressOf() );
//	RETURN_IF_FAILED( hr );
//    hr = D3DReadFileToBlob( psFilename.c_str(), pPixelShaderBuffer.GetAddressOf() );
//    // Create the vertex shader from the buffer.
//	hr = pDevice->CreateVertexShader(pVertexShaderBuffer->
//        GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(),
//		NULL, m_vertexShader.GetAddressOf() );
//	RETURN_IF_FAILED( hr );
//
//	// Create the pixel shader from the buffer.
//	hr = pDevice->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(),
//        pPixelShaderBuffer->GetBufferSize(),NULL,m_pixelShader.GetAddressOf() );
//	RETURN_IF_FAILED( hr );
//	
//    D3D11_INPUT_ELEMENT_DESC polygonLayout[3]; // Make vector w/ global utility, copy to array
//    vector<D3D11_INPUT_ELEMENT_DESC> polyV = VertexPositionUVNormalType::CreateLayoutDescriptions();
//    std::copy(polyV.begin(), polyV.end(), polygonLayout);
//
//	// Get a count of the elements in the layout.
//	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
//
//	// Create the vertex input layout.
//	result = pDevice->CreateInputLayout(
//        polygonLayout, 
//        numElements, 
//        pVertexShaderBuffer->GetBufferPointer(), 
//        pVertexShaderBuffer->GetBufferSize(), 
//        &m_layout);
//	RETURN_IF_FAILED( result );
//
//   	// DESCRIPTION for dynamic matrix constant buffer that is in the vertex shader.
//   	D3D11_BUFFER_DESC matrixBufferDesc = MatrixBufferType::CreateMatrixDescription();
//	result = pDevice->CreateBuffer(
//        &matrixBufferDesc, 
//        NULL, 
//        &m_matrixBuffer);
//	RETURN_IF_FAILED( result );
//
//    //TODO: add sampler description, lightbuffer desc to child
//    // SAMPLER DESCRIPTION
//   	D3D11_SAMPLER_DESC samplerDesc = CreateSamplerDescription();
//	result = pDevice->CreateSamplerState(&samplerDesc, &m_sampleState);
//	RETURN_IF_FAILED( result );
//
//    /*  If the constant buffer sizes are not multiples of 16, must pad them w extra space  or CreateBuffer() 
//        will fail. In this case the constant buffer is 28 bytes with 4 bytes padding to make it 32. */
//    
//    // LIGHT BUFFER DESCRIPTION
//    D3D11_BUFFER_DESC lightBufferDesc = LightBufferType::CreateLightDescription();
//	result = pDevice->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
//	RETURN_IF_FAILED( result );
//
//	return true;
//    }

bool Shader_Lighting::SetShaderParameters_CHILD(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView * texture) const
{
    // Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	return true;
}

//// this function must be overloaded from parent
//bool Shader_Lighting::Render(
//    ID3D11DeviceContext *deviceContext, 
//    //int indexCount, 
//    XMMATRIX worldMatrix, 
//    XMMATRIX viewMatrix,
//    XMMATRIX projectionMatrix, 
//    ID3D11ShaderResourceView* texture, 
//    FX* effect
////    XMFLOAT3 lightDirection, 
//    XMFLOAT4 diffuseColor
//) const
//{
//	// Set the shader parameters that it will use for rendering.
//	bool result = SetShaderParameters(
//        deviceContext, 
//        worldMatrix, 
//        viewMatrix, 
//        projectionMatrix, 
//        texture, 
//        effect
//        /*lightDirection, 
//        diffuseColor*/);
//        RETURN_IF_FALSE(result);
//
//	// Now render the prepared buffers with the shader.
//	RenderShader(deviceContext/*, indexCount*/);
//
//	return true;
//}

void Shader_Lighting::RenderChild(ID3D11DeviceContext * deviceContext/*, int indexCount*/) const
{
 //   // Set the vertex input layout.
	//deviceContext->IASetInputLayout( m_layout.Get() );
	//// Set the vertex and pixel shaders that will be used to render this triangle.
	//deviceContext->VSSetShader( m_vertexShader.Get(), NULL, 0 );
	//deviceContext->PSSetShader( m_pixelShader.Get(), NULL, 0 );

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers( 0, 1, m_sampleState.GetAddressOf() );

	return;
}
