////////////////////////////////////////////////////////////////////////////////
// File name: Shader.cpp
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Shader.h"

    Shader::Shader(std::wstring vs, std::wstring ps) {
        m_vsFilename = vs;
        m_psFilename = ps;
    }

    bool Shader::Initialize(
        ID3D11Device* pDevice, 
        HWND WinHandle/*, 
        const Model &crModel*/) 
    {
	// Initialize the vertex and pixel shaders.
	bool result = InitializeShader( 
		pDevice, 
		WinHandle, 
	    m_vsFilename, 
        m_psFilename/*, 
		crModel */);
	RETURN_IF_FALSE( result );

	return true;
    }
