#pragma once

#include "Includes.h"
#include "IOverlay.h"

class Graphics;

class Overlay_Base:
	public IOverlay
{
public:
	Overlay_Base( Microsoft::WRL::ComPtr<IDWriteTextFormat> Font );
	virtual ~Overlay_Base();

	virtual bool Initialize( const Graphics &Gfx ) = 0;
	virtual void Render( const Graphics &Gfx ) = 0;
protected:
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_Font;

};

