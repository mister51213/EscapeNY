#include "Shader_LightSet_Night.h"



Shader_LightSet_Night::Shader_LightSet_Night()
{
}


Shader_LightSet_Night::~Shader_LightSet_Night()
{
}

void Shader_LightSet_Night::UpdateState( ID3D11DeviceContext *pContext,
	const MatrixBufferType &, ILightSet*pLightSet )
{
	this->SetVSConstantBuffer(
		0, sizeof(MatrixBufferType), 
	)
}
