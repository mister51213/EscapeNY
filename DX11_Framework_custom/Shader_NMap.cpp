#include "Shader_NMap.h"
#include "Includes.h"


Shader_NMap::Shader_NMap()
{}


Shader_NMap::~Shader_NMap()
{}

bool Shader_NMap::Initialize( ID3D11Device * pDevice, int MaxLightCount )
{
	// Load/Create vertex shader and input layout
	{
		comptr<ID3DBlob> pBuffer;
		HRESULT hr = D3DReadFileToBlob(
			L"Shaders/NMap_vs.cso",
			pBuffer.GetAddressOf()
		);
		assert( SUCCEEDED( hr ) );

		hr = pDevice->CreateVertexShader(
			pBuffer->GetBufferPointer(),
			pBuffer->GetBufferSize(),
			nullptr,
			m_pVertexShader.GetAddressOf() );
		assert( SUCCEEDED( hr ) );

		auto layoutDescriptions =
			VertexBufferTypeAllInOneNMap::CreateLayoutDescriptions();
		hr = pDevice->CreateInputLayout(
			layoutDescriptions.data(),
			layoutDescriptions.size(),
			pBuffer->GetBufferPointer(),
			pBuffer->GetBufferSize(),
			m_pLayout.GetAddressOf()
		);
		assert( SUCCEEDED( hr ) );
	}

	// Load/Create pixel shader 
	{
		comptr<ID3DBlob> pBuffer;
		HRESULT hr = D3DReadFileToBlob(
			L"Shaders/NMap_ps.cso",
			pBuffer.GetAddressOf()
		);
		assert( SUCCEEDED( hr ) );

		hr = pDevice->CreatePixelShader(
			pBuffer->GetBufferPointer(),
			pBuffer->GetBufferSize(),
			nullptr,
			m_pPixelShader.GetAddressOf()
		);
		assert( SUCCEEDED( hr ) );
	}

	// Load sampler state
	{
		auto samplerDesc = CreateSamplerDescription();
		HRESULT hr = pDevice->CreateSamplerState(
			&samplerDesc, m_pSampleState.GetAddressOf()
		);
		assert( SUCCEEDED( hr ) );
	}

	// Load matrix constant buffer
	{
		auto matBufferDesc = MatrixBufferType::CreateMatrixDescription();
		HRESULT hr = pDevice->CreateBuffer(
			&matBufferDesc, 
			nullptr, 
			m_pMatrixBuffer.GetAddressOf() );
		assert( SUCCEEDED( hr ) );
	}

	// Load light constant buffer
	{
		// Function takes in max light count as param, default is 1
		auto lightBufferDesc = SceneBufferType::CreateLightDescription();
		HRESULT hr = pDevice->CreateBuffer(
			&lightBufferDesc,
			nullptr,
			m_pLightBuffer.GetAddressOf()
		);
		assert( SUCCEEDED( hr ) );
	}

	return true;
}

bool Shader_NMap::UpdateTransformBuffer( ID3D11DeviceContext * pContext, const MatrixBufferType & Transforms )const
{
	D3D11_MAPPED_SUBRESOURCE ms{};
	HRESULT hr = pContext->Map(
		m_pMatrixBuffer.Get(),
		0, D3D11_MAP_WRITE_DISCARD,
		0, &ms );
	assert( SUCCEEDED( hr ) );

	CopyMemory( ms.pData, &Transforms, sizeof( MatrixBufferType ) );

	pContext->Unmap( m_pMatrixBuffer.Get(), 0 );

	return true;
}

bool Shader_NMap::UpdateLightBuffer( ID3D11DeviceContext * pContext, const SceneBufferType& SceneLights )const
{
	D3D11_MAPPED_SUBRESOURCE ms{};
	HRESULT hr = pContext->Map(
		m_pLightBuffer.Get(),
		0, D3D11_MAP_WRITE_DISCARD,
		0, &ms );
	assert( SUCCEEDED( hr ) );
		
	CopyMemory( ms.pData, &SceneLights, sizeof( SceneBufferType ) );

	pContext->Unmap( m_pLightBuffer.Get(), 0 );

	return true;
}

void Shader_NMap::Render( ID3D11DeviceContext * pContext, ID3D11ShaderResourceView ** ppShaderResources )const
{
	// Set the position of the constant buffer in the vertex shader.
	unsigned int bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	pContext->VSSetConstantBuffers( bufferNumber, 1, m_pMatrixBuffer.GetAddressOf() );

	//bufferNumber = 1;
	// Set the light constant buffer in the pixel shader with the updated values.
	pContext->PSSetConstantBuffers( bufferNumber, 1, m_pLightBuffer.GetAddressOf() );

	// Set the vertex input layout.
	pContext->IASetInputLayout( m_pLayout.Get() );

	// Set the sampler state in the pixel shader.
	pContext->PSSetSamplers( 0, 1, m_pSampleState.GetAddressOf() );

	pContext->PSSetShaderResources( 0, 2, ppShaderResources );

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pContext->VSSetShader( m_pVertexShader.Get(), NULL, 0 );
	pContext->PSSetShader( m_pPixelShader.Get(), NULL, 0 );

}
