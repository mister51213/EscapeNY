#pragma once

#include "Includes.h"
#include "Utilities.h"

template<class MatBuffer, class LightBuffer>
class ShaderT
{
public:

	ShaderT() = default;
	~ShaderT() = default;

	bool Initialize( ID3D11Device* pDevice, int lightCount = 1)
	{
		// Initialize the vertex and pixel shaders.
		bool result = InitializeShaderCommon(
			pDevice,
			L"Shaders/Shader_vs.cso",
			L"Shaders/Shader_ps.cso" );
		RETURN_IF_FALSE( result );

		///////////////////////////////////////////////////////////
		// Texture shader setup
		///////////////////////////////////////////////////////////
		// Create a texture sampler state description.
        D3D11_SAMPLER_DESC samplerDesc = CreateSamplerDescription();

		// Create the texture sampler state.
		HRESULT hr = pDevice->CreateSamplerState( &samplerDesc, &m_sampleState );
		RETURN_IF_FAILED( hr );

		///////////////////////////////////////////////////////////
		// Light shader setup
		///////////////////////////////////////////////////////////

        D3D11_BUFFER_DESC lightBufferDesc = LightBufferType::CreateLightDescription(sizeof(LightBuffer));

		// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		hr = pDevice->CreateBuffer( &lightBufferDesc, NULL, &m_lightBuffer );
		RETURN_IF_FAILED( hr );

		return result;
	}

	bool UpdateTransformBuffer(
		ID3D11DeviceContext *pContext,
		const MatBuffer &BufferData )const
	{
		LPCVOID pBuffer = reinterpret_cast<LPCVOID>( &BufferData );
		size_t buffSize = sizeof( BufferData );
		bool result = UpdateConstantBuffer(
			pContext, pBuffer, buffSize, m_matrixBuffer.Get()
		);

		return result;
	}

	bool UpdateLightBuffer(
		ID3D11DeviceContext *pContext,
		const LightBuffer *BufferData )const
	{
		LPCVOID pBuffer = reinterpret_cast<LPCVOID>( BufferData );
		size_t buffSize = sizeof( LightBuffer );
		bool result = UpdateConstantBuffer(
			pContext, pBuffer, buffSize, m_lightBuffer.Get()
		);

		return result;
	}

	/*bool UpdateLightBuffer(
	ID3D11DeviceContext *pContext,
	const LightBufferType &BufferData )const;
	bool UpdateLightBuffer(
	ID3D11DeviceContext *pContext,
	const SpotLightBuffer &BufferData )const;
	bool UpdateLightBuffer(
	ID3D11DeviceContext *pContext,
	const LightSpotBase &BufferData )const;*/

	void Render(
		ID3D11DeviceContext* pContext,
		/*ID3D11ShaderResourceView *pTextureView*/ 
        ID3D11ShaderResourceView *const *ppTextureViews )const
	{
		//////////////////////////////////////////////////////
		//				Set shader resources				//
		//////////////////////////////////////////////////////

		// Set the position of the constant buffer in the vertex shader.
		unsigned int bufferNumber = 0;

		// Now set the constant buffer in the vertex shader with the updated values.
		pContext->VSSetConstantBuffers( bufferNumber, 1, m_matrixBuffer.GetAddressOf() );

		//bufferNumber = 1;
		// Set the light constant buffer in the pixel shader with the updated values.
		pContext->PSSetConstantBuffers( bufferNumber, 1, m_lightBuffer.GetAddressOf() );

		// Set the vertex input layout.
		pContext->IASetInputLayout( m_layout.Get() );

		// Set the sampler state in the pixel shader.
		pContext->PSSetSamplers( 0, 1, m_sampleState.GetAddressOf() );

        // TODO: MAKE THIS TAKE AN ARRAY of shaders for multitexturing and normal mapping
        // TODO: vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

        // Set shader resources
		//pContext->PSSetShaderResources( 0, 1, &pTextureView );

   		pContext->PSSetShaderResources( 0, 2, ppTextureViews );

          // TODO: ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
          // TODO: MAKE THIS TAKE AN ARRAY of shaders for multitexturing and normal mapping

		// Set the vertex and pixel shaders that will be used to render this triangle.
		pContext->VSSetShader( m_vertexShader.Get(), NULL, 0 );
		pContext->PSSetShader( m_pixelShader.Get(), NULL, 0 );
	}

protected:
	bool UpdateConstantBuffer(
		ID3D11DeviceContext *pContext,
		LPCVOID BufferData,
		size_t BufferSize,
		ID3D11Buffer *pConstantBuffer )const
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource{};
		// Lock the constant buffer so it can be written to.
		HRESULT result = pContext->Map(
			pConstantBuffer,
			0, D3D11_MAP_WRITE_DISCARD,
			0, &mappedResource );
		RETURN_IF_FAILED( result );

		CopyMemory( mappedResource.pData, BufferData, BufferSize );

		// Unlock the constant buffer.
		pContext->Unmap( pConstantBuffer, 0 );

		return true;
	}
	bool InitializeShaderCommon(
		ID3D11Device* pDevice,
		const std::wstring & vsFilename,
		const std::wstring & psFilename )
	{
		///////////////////////////////////////////////////////////
		// Common shader setup
		///////////////////////////////////////////////////////////
		// Initialize the variables for this function
		HRESULT result = S_OK;
		comptr<ID3D10Blob> pVertexShaderBuffer, pPixelShaderBuffer;

		// Load the vertex shader code.
		result = D3DReadFileToBlob(
			vsFilename.c_str(),
			pVertexShaderBuffer.GetAddressOf() );
		RETURN_IF_FAILED( result );

		// Load the pixel shader code.
		result = D3DReadFileToBlob(
			psFilename.c_str(),
			pPixelShaderBuffer.GetAddressOf() );
		RETURN_IF_FAILED( result );

		// Create the vertex shader from the buffer.
		result = pDevice->CreateVertexShader(
			pVertexShaderBuffer->GetBufferPointer(),
			pVertexShaderBuffer->GetBufferSize(),
			NULL,
			m_vertexShader.GetAddressOf() );
		RETURN_IF_FAILED( result );

		// Create the pixel shader from the buffer.
		result = pDevice->CreatePixelShader(
			pPixelShaderBuffer->GetBufferPointer(),
			pPixelShaderBuffer->GetBufferSize(),
			NULL,
			m_pixelShader.GetAddressOf() );
		RETURN_IF_FAILED( result );

		// Get appropiate layout description for this shader
		auto layoutDesciptions = VertexBufferTypeAllInOne::CreateLayoutDescriptions();

		// Create the vertex input layout.
		result = pDevice->CreateInputLayout(
			layoutDesciptions.data(),
			layoutDesciptions.size(),
			pVertexShaderBuffer->GetBufferPointer(),
			pVertexShaderBuffer->GetBufferSize(),
			&m_layout );
		RETURN_IF_FAILED( result );

		// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
		D3D11_BUFFER_DESC matrixBufferDesc{};
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof( MatBuffer );
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		result = pDevice->CreateBuffer( &matrixBufferDesc, NULL, &m_matrixBuffer );
		RETURN_IF_FAILED( result );

		return SUCCEEDED( result );
	}


private:
	comptr<ID3D11VertexShader> m_vertexShader;
	comptr<ID3D11PixelShader> m_pixelShader;
	comptr<ID3D11InputLayout> m_layout;
	comptr<ID3D11Buffer> m_matrixBuffer, m_lightBuffer;
	comptr<ID3D11SamplerState> m_sampleState;

};