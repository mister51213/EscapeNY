#pragma once

#include "IOverlay.h"

class Overlay_Base:
	public IOverlay
{
public:
	Overlay_Base( comptr<IDWriteTextFormat> Font );
	virtual ~Overlay_Base();

	virtual bool Initialize( const Graphics &Gfx ) = 0;
	virtual void Render( const Graphics &Gfx ) = 0;
protected:
	comptr<IDWriteTextFormat> m_Font;

};

