#pragma once
#include "Shader.h"

struct LightBufferType
	{
		XMFLOAT4 diffuseColor; // should be XMFLOAT3?
		XMFLOAT3 lightDirection; // should be XMFLOAT4?
		float padding;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
	};

class Shader_Lighting : public Shader
{
public:
    Shader_Lighting();
    Shader_Lighting(HWND& hWnd);
  	Shader_Lighting(const Shader_Lighting&);
    ~Shader_Lighting();

    bool InitializeShader(
        ID3D11Device* pDevice,
        const std::wstring & vsFilename,
        const std::wstring & psFilename/*,
        HWND& hwnd*/);

    void OutputShaderErrorMessage(ID3DBlob * errorMessage, HWND & hwnd, LPCWSTR & shaderFilename);

    bool SetShaderParameters(
        ID3D11DeviceContext* deviceContext,
        XMMATRIX & worldMatrix,
        XMMATRIX & viewMatrix,
        XMMATRIX & projectionMatrix,
        ID3D11ShaderResourceView* texture,
        XMFLOAT3 lightDirection,
        XMFLOAT4 diffuseColor) const;
    
    // Need difft parameters so have to overload the parent function.
    bool Render(
        ID3D11DeviceContext*, 
        int, 
        XMMATRIX, 
        XMMATRIX, 
        XMMATRIX, 
        ID3D11ShaderResourceView*, 
        XMFLOAT3,  //TODO: should be D3DXVECTOR3
        XMFLOAT4) const; //TODO: should be D3DXVECTOR4

    void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount) const;
    void RenderShader(ID3D11DeviceContext*)const override {}
    bool SetShaderParameters(
        ID3D11DeviceContext*,
		const XMMATRIX &,
		const XMMATRIX &,
		const XMMATRIX &,
        ID3D11ShaderResourceView*)const override {
        return false;
    }

    // DUMMY DECLARATION
  //  bool InitializeShader(
		//ID3D11Device*,
		//const std::wstring &,
  //      const std::wstring &) override {
  //      return false;
  //  }
private:
    // child-specfic members
	comptr<ID3D11SamplerState> m_sampleState;
    comptr<ID3D11Buffer> m_lightBuffer;
    HWND m_hWnd; // can't pass in initializer
    // passed in constructor
    /*ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;*/
};

