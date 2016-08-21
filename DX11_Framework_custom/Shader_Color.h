// 
// TODO: Change shader functionality to using built-in shader header files instead of .cso
// files that need to be packaged with the final .exe file (before we actually ship the game)
//

////////////////////////////////////////////////////////////////////////////////
// Filename: Shader_Color.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "Shader.h"

using namespace DirectX;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Forward declaration(s)
////////////////////////////////////////////////////////////////////////////////
//class Model;
////////////////////////////////////////////////////////////////////////////////
// Class name: Shader_Color
////////////////////////////////////////////////////////////////////////////////
class Shader_Color:public Shader
{

public:
	Shader_Color();
	~Shader_Color();

    // no need for sampler description; only lighting.
    virtual vector<D3D11_INPUT_ELEMENT_DESC> InitChild(ID3D11Device* pDevice) override;

    virtual bool SetShaderParameters_CHILD(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture = 0, ILight* pLight = 0)const {
        return true; }
	
    virtual void RenderChild( ID3D11DeviceContext* )const override;
};