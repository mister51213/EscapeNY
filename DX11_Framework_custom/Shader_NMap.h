#pragma once

#include "ShaderTEMPLATE.h"

struct VertexBufferTypeAllInOneNMap;
struct MatrixBufferType;
struct SceneBufferType;

class Shader_NMap: public ShaderT<VertexBufferTypeAllInOneNMap, MatrixBufferType, SceneBufferType>
{
public:
	bool Initialize( ID3D11Device* pDevice, int MaxLightCount = 1 )override;
	bool UpdateTransformBuffer( ID3D11DeviceContext *pContext, const MatrixBufferType &Transforms )const;
	bool UpdateLightBuffer( ID3D11DeviceContext *pContext, const SceneBufferType& SceneLights )const;
	void Render( ID3D11DeviceContext *pContext, ID3D11ShaderResourceView **ppShaderResources )const;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pMatrixBuffer, m_pLightBuffer;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampleState;

};

