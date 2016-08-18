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

//virtual bool InitializeShader( 
//       ID3D11Device*, 
//       const std::wstring &,
//       const std::wstring &) override;

    // no need for sampler description; only lighting.
    virtual bool InitChild(ID3D11Device* pDevice) override;

    virtual bool SetShaderParameters_CHILD(
        ID3D11DeviceContext* deviceContext,
        const XMMATRIX & worldMatrix,
        const XMMATRIX & viewMatrix,
        const XMMATRIX & projectionMatrix,
        ID3D11ShaderResourceView* texture = 0,
        FX* effect = 0)const;
	
    virtual void RenderChild( ID3D11DeviceContext* )const override;

//private:
//    comptr<ID3D11Buffer> m_lightBuffer;
};