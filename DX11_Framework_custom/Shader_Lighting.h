#pragma once
#include "Shader.h"

class Shader_Lighting : public Shader
{
public:
    Shader_Lighting();
  	Shader_Lighting(const Shader_Lighting&);
    ~Shader_Lighting();

    virtual vector<D3D11_INPUT_ELEMENT_DESC> InitChild(ID3D11Device * pDevice) override;

    // TODO: The override for this is NOT being called!
    //bool InitializeShader(
    //    ID3D11Device* pDevice,
    //    const std::wstring & vsFilename,
    //    const std::wstring & psFilename) override;

    bool SetShaderParameters_CHILD(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ILight* pLight=0) const;
    
    //bool Render(
    //    ID3D11DeviceContext*, 
    //    //int, 
    //    XMMATRIX, 
    //    XMMATRIX, 
    //    XMMATRIX, 
    //    ID3D11ShaderResourceView*, 
    //    FX* effect
    //    /*XMFLOAT3 lightDirection, 
    //    XMFLOAT4 diffuseColor*/) const; 

    void RenderChild(ID3D11DeviceContext* deviceContext/*, int indexCount*/) const override;
  //  void RenderShader(ID3D11DeviceContext*)const override {}
  //  bool SetShaderParameters(
  //      ID3D11DeviceContext*,
		//const XMMATRIX &,
		//const XMMATRIX &,
		//const XMMATRIX &,
  //      ID3D11ShaderResourceView*)const override {
  //      return false;
  //  }

private:
    // child-specific members
	comptr<ID3D11SamplerState> m_sampleState;
    //comptr<ID3D11Buffer> m_lightBuffer;
};

