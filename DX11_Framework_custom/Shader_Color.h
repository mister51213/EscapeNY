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

	virtual bool InitializeShader( 
        ID3D11Device*, 
        const std::wstring &,
        const std::wstring &) override;

	virtual bool SetShaderParameters( 
        ID3D11DeviceContext*, 
        XMMATRIX &, 
        XMMATRIX &, 
        XMMATRIX &, 
        ID3D11ShaderResourceView* =0) override;
	
    virtual void RenderShader( ID3D11DeviceContext* );
};