////////////////////////////////////////////////////////////////////////////////
// File name: Shader.cpp
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Shader.h"

Shader::Shader( const std::wstring &vs, const std::wstring &ps)
{
	m_vsFilename = vs;
	m_psFilename = ps;
}

bool Shader::Initialize( ID3D11Device* pDevice )
{
	// Initialize the vertex and pixel shaders.
	bool result = InitializeShader( pDevice, m_vsFilename, m_psFilename );
	RETURN_IF_FALSE( result );

	return true;
}
