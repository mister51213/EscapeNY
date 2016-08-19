////////////////////////////////////////////////////////////////////////////////
// File name: Shader.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "Includes.h"
#include "Utilities.h"
using namespace DirectX;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Forward declaration(s)
////////////////////////////////////////////////////////////////////////////////
class Model;

class Shader
{
public:
    Shader();
    Shader(const std::wstring &vs, const std::wstring &ps);

public:
    bool Initialize(ID3D11Device* pDevice);

    bool Render(
        ID3D11DeviceContext* deviceContext,
        const XMMATRIX & worldMatrix,
        const XMMATRIX & viewMatrix,
        const XMMATRIX & projectionMatrix,
        ID3D11ShaderResourceView* texture = nullptr,
        FX* effect = nullptr)const;

    virtual void RenderChild(ID3D11DeviceContext*)const = 0;

    bool SetShaderParameters(
        ID3D11DeviceContext* deviceContext,
        const XMMATRIX & worldMatrix,
        const XMMATRIX & viewMatrix,
        const XMMATRIX & projectionMatrix,
        ID3D11ShaderResourceView* texture,
        FX* effect)const;

    virtual bool SetShaderParameters_CHILD(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture) const = 0;

//public:
    //virtual bool InitializeShader(
    //    ID3D11Device*,
    //    const std::wstring &,
    //    const std::wstring &) = 0;
    //bool Shader::InitializeShaderCOMMON(
    //    ID3D11Device* pDevice,
    //    const std::wstring & vsFilename,
    //    const std::wstring & psFilename);

    virtual bool Shader::InitChild(ID3D11Device* pDevice) = 0;

protected:
	comptr<ID3D11VertexShader> m_vertexShader;
	comptr<ID3D11PixelShader> m_pixelShader;
	comptr<ID3D11InputLayout> m_layout;
	comptr<ID3D11Buffer> m_matrixBuffer;
    comptr<ID3D11Buffer> m_lightBuffer;

	std::wstring m_vsFilename;
	std::wstring m_psFilename;
};