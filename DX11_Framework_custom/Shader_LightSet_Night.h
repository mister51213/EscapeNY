#pragma once
#include "Shader_StateMachine.h"
#include "ILightSet.h"

class Shader_LightSet_Night:
	public Shader_StateMachine
{
public:
	Shader_LightSet_Night();
	~Shader_LightSet_Night();
	
	void UpdateState( ID3D11DeviceContext *pContext,
		const MatrixBufferType &, ILightSet*pLightSet );

private:
	comptr<ID3D11Buffer> m_pMatrixBuffer, m_pVSLightBuffer, m_pPSLightBuffer;
	comptr<ID3D11VertexShader> m_pVertexShader;
	comptr<ID3D11PixelShader> m_pPixelShader;

};

