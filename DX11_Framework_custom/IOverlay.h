#pragma once

#include "Graphics.h"


class MessageBoxBase
{
public:
	MessageBoxBase() = default;
	MessageBoxBase( const std::wstring &Message, const D2D1_RECT_F &Position )
		:
		message( Message ),
		position( Position )
	{
	}

	virtual std::wstring GetString()const = 0;	
	const D2D1_RECT_F &GetRect()const
	{
		return position;
	}
protected:
	std::wstring message;
	D2D1_RECT_F position;
};

class IOverlay
{
public:
	virtual bool Initialize( const Graphics &Gfx ) = 0;
	virtual void Render( const Graphics &Gfx ) = 0;
};

