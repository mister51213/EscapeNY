////////////////////////////////////////////////////////////////////////////////
// Filename: Shader_Color.cpp
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "Shader_Color.h"

bool Shader_Color::Initialize( ID3D11Device* pDevice )
{
	bool result = InitializeShaderCommon(
		pDevice,
		L"Shaders/color_vs.cso", 
		L"Shaders/color_ps.cso" );

	return result;
}

void Shader_Color::Render( ID3D11DeviceContext * pContext ) const
{
	RenderShaderCommon( pContext );
}
