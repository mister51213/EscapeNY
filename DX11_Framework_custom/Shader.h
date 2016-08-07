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
	Shader(const std::wstring &vs, const std::wstring &ps );

protected:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	bool Initialize( ID3D11Device* pDevice );

	// CODE_CHANGE: made function const
	bool Render(
		ID3D11DeviceContext* deviceContext,
		XMMATRIX & worldMatrix,
		XMMATRIX & viewMatrix,
		XMMATRIX & projectionMatrix,
		ID3D11ShaderResourceView* texture = nullptr )const
	{
		// Set the shader parameters to use for rendering.

		// NOTE: texture is NULL by default and will be set only in CHILD texture class.
		bool result = SetShaderParameters(
			deviceContext,
			worldMatrix,
			viewMatrix,
			projectionMatrix,
			texture );
		RETURN_IF_FALSE( result );

		// Now render the prepared buffers with the shader.
		RenderShader( deviceContext );

		return true;
	}

	// CODE_CHANGE: made function const
	virtual void RenderShader( ID3D11DeviceContext* )const = 0;

	// CODE_CHANGE: made function const
	virtual bool SetShaderParameters(
		ID3D11DeviceContext*,
		XMMATRIX &,
		XMMATRIX &,
		XMMATRIX &,
		ID3D11ShaderResourceView* )const = 0;

public:
	virtual bool InitializeShader(
		ID3D11Device*,
		const std::wstring &,
		const std::wstring & ) = 0;

public:
	comptr<ID3D11VertexShader> m_vertexShader;
	comptr<ID3D11PixelShader> m_pixelShader;
	comptr<ID3D11InputLayout> m_layout;
	comptr<ID3D11Buffer> m_matrixBuffer;

	std::wstring m_vsFilename;
	std::wstring m_psFilename;
};