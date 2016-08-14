#pragma once

#include "Texture.h"
#include "Model.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	bool Initialize( Graphics *const pGraphics );
	const Texture *const GetTexture( const eTexture TextureType )const;
private:
	std::vector<Texture> m_pTextureList;
};

