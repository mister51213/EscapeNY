#include "Shader_PtLight.h"

Shader_PtLight::Shader_PtLight()
    :
Shader(L"Shaders/PointLight_vs.cso", L"Shaders/PointLight_ps.cso")
{}

Shader_PtLight::~Shader_PtLight()
{}

vector<D3D11_INPUT_ELEMENT_DESC> Shader_PtLight::InitChild(ID3D11Device * pDevice)
{
    // LIGHT DESCRIPTIONS for point lights
   	D3D11_BUFFER_DESC lightColorBufferDesc;
	D3D11_BUFFER_DESC lightPositionBufferDesc;

    // Light Color buffer array
    lightColorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightColorBufferDesc.ByteWidth = sizeof(LightColorBufferType);
	lightColorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightColorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightColorBufferDesc.MiscFlags = 0;
	lightColorBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	pDevice->CreateBuffer(&lightColorBufferDesc, NULL, &m_lightColorBuffer);

	// Setup the description of the dynamic constant buffer that is in the vertex shader.
	lightPositionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightPositionBufferDesc.ByteWidth = sizeof(LightPositionBufferType);
	lightPositionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightPositionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightPositionBufferDesc.MiscFlags = 0;
	lightPositionBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	pDevice->CreateBuffer(&lightPositionBufferDesc, NULL, &m_lightPositionBuffer);
	
    // SAMPLER DESCRIPTION
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc = CreateSamplerDescription();
    pDevice->CreateSamplerState(&samplerDesc, &m_sampleState);

    return VertexPositionUVNormalType::CreateLayoutDescriptions();
}

bool Shader_PtLight::SetShaderParameters_CHILD(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView * texture, ILight* pLight) const
{
    // Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

    // TODO: Cast pLight to PTLight, and get appropriate info from it to fill PTlight buffers
    /* D3DXVECTOR4 diffuseColor[],D3DXVECTOR4 lightPosition[]) */
    ILightPoint* pLightPT = (ILightPoint*)pLight;
    const unsigned int numLights = pLightPT->GetNumLights();
    
/////////////////////////////////////////////
    // UPDATING CONSTANT BUFFERS
    // using info from ILightPoint interface
    /////////////////////////////////////////

    D3D11_MAPPED_SUBRESOURCE mappedResource;

    /////////////////////////////////////////////
    // COLOR BUFFER
    // Lock device
    deviceContext->Map(m_lightColorBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    LightColorBufferType colorsType = pLightPT->GetColors();
    CopyMemory(mappedResource.pData, &colorsType, sizeof(LightColorBufferType));
    // Unlock
    deviceContext->Unmap(m_lightColorBuffer.Get(), 0);

    // Set the position of the constant buffer in the pixel shader.
	unsigned int bufferNumber = 0;
	// Finally set the constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, m_lightColorBuffer.GetAddressOf());

    /////////////////////////////////////////////
    // POSITION BUFFER
    // Lock device
    deviceContext->Map(m_lightPositionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    LightPositionBufferType positionsType = pLightPT->GetPositions();
    CopyMemory(mappedResource.pData, &positionsType, sizeof(LightPositionBufferType));
    // Unlock
    deviceContext->Unmap(m_lightPositionBuffer.Get(), 0);

    // Set the position of the constant buffer in the vertex shader.
	bufferNumber = 1;
	// Finally set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, m_lightPositionBuffer.GetAddressOf());
	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

void Shader_PtLight::RenderChild(ID3D11DeviceContext * deviceContext/*, int indexCount*/) const
{
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers( 0, 1, m_sampleState.GetAddressOf() );

	return;
}



