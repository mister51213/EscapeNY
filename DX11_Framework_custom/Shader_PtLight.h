//#pragma once
//#include "Shader.h"
//#include "ILightPoint.h"
//
//class Shader_PtLight :
//    public Shader
//{
//public:
//    Shader_PtLight();
//    ~Shader_PtLight();
//    vector<D3D11_INPUT_ELEMENT_DESC> InitChild(ID3D11Device * pDevice);
//    bool SetShaderParameters_CHILD(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ILight* pLight) const;
//    void RenderChild(ID3D11DeviceContext * deviceContext) const;
//
//    private:
//	comptr<ID3D11SamplerState> m_sampleState;
//    // TWO NEW BUFFERS
//	comptr<ID3D11Buffer> m_lightColorBuffer;
//    comptr<ID3D11Buffer> m_lightPositionBuffer;
//};
//
