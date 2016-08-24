#pragma once
#include "Shader.h"

class Shader_Lighting : public Shader
{
public:
    Shader_Lighting();
  	Shader_Lighting(const Shader_Lighting&);
    ~Shader_Lighting();

    virtual vector<D3D11_INPUT_ELEMENT_DESC> InitChild(ID3D11Device * pDevice) override;
    bool SetShaderParameters_CHILD(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ILight* pLight=0) const;
    void RenderChild(ID3D11DeviceContext* deviceContext/*, int indexCount*/) const override;

private:
    // child-specific members
	comptr<ID3D11SamplerState> m_sampleState;
    comptr<ID3D11Buffer> m_lightBuffer;
};

