#pragma once

#include "ShaderTEMPLATE.h"
#include "DXUtils.h"

class Shader_NMap: public ShaderT<VertexBufferTypeAllInOneNMap, MatrixBufferType, SceneBufferType>
{
public:
	Shader_NMap();
	~Shader_NMap();
	
	bool Initialize( ID3D11Device* pDevice, int MaxLightCount = 1 )override;
	bool UpdateTransformBuffer( ID3D11DeviceContext *pContext, const MatrixBufferType &Transforms )const;
	bool UpdateLightBuffer( ID3D11DeviceContext *pContext, const SceneBufferType& SceneLights )const;
	void Render( ID3D11DeviceContext *pContext, ID3D11ShaderResourceView **ppShaderResources )const;

private:
	comptr<ID3D11VertexShader> m_pVertexShader;
	comptr<ID3D11PixelShader> m_pPixelShader;
	comptr<ID3D11InputLayout> m_pLayout;
	comptr<ID3D11Buffer> m_pMatrixBuffer, m_pLightBuffer;
	comptr<ID3D11SamplerState> m_pSampleState;

};

