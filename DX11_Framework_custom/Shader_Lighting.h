#pragma once
#include "Shader.h"

struct LightBufferType
	{
		XMVECTOR diffuseColor; // should be XMFLOAT3?
		XMVECTOR lightDirection; // should be XMFLOAT4?
		float padding;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
	};

class Shader_Lighting : public Shader
{
public:
    Shader_Lighting();
  	Shader_Lighting(const Shader_Lighting&);
    ~Shader_Lighting();

    bool InitializeShader(
        ID3D11Device* pDevice,
        const std::wstring & vsFilename,
        const std::wstring & psFilename);

    bool SetShaderParameters(
        ID3D11DeviceContext* deviceContext,
        XMMATRIX & worldMatrix,
        XMMATRIX & viewMatrix,
        XMMATRIX & projectionMatrix,
        ID3D11ShaderResourceView* texture) const;
    
    // Need difft parameters so have to overload the parent function.
    bool Render(
        ID3D11DeviceContext*, 
        int, 
        XMMATRIX, 
        XMMATRIX, 
        XMMATRIX, 
        ID3D11ShaderResourceView*, 
        XMVECTOR,  //TODO: should be D3DXVECTOR3
        XMVECTOR); //TODO: should be D3DXVECTOR4

    void RenderShader(ID3D11DeviceContext* deviceContext) const;
private:
	comptr<ID3D11SamplerState> m_sampleState;
    comptr<ID3D11Buffer> m_lightBuffer;
};

