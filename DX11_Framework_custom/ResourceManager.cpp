#include "ResourceManager.h"



ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}

bool ResourceManager::Initialize( Graphics *const pGraphics )
{
	const int numTextures = 5;
	m_pTextureList.resize( numTextures );
	m_pTextureList[ AsphaltFresh ].Initialize( *pGraphics, L"Textures\\fresh-black-asphalt-texture.jpg" );
	m_pTextureList[ AsphaltTGA ].Initialize( *pGraphics, L"Textures\\asphalt.tga" );
	m_pTextureList[ AsphaltOld ].Initialize( *pGraphics, L"Textures\\old-asphalt-texture.jpg" );
	m_pTextureList[ Water ].Initialize( *pGraphics, L"Textures\\water3.jpg" );
	m_pTextureList[ SharkSkin ].Initialize( *pGraphics, L"Textures\\sharkskin1.jpg" );

	return true;
}

const Texture * const ResourceManager::GetTexture( const eTexture TextureType )const
{
	auto *const pTexture = &m_pTextureList[ TextureType ];
	return pTexture;
}
