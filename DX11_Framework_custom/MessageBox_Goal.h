#pragma once

// Needed for wstring, also used for D2D1_RECT_F so no point forward declaring it
#include "Includes.h"
// Needed for MessageBoxBase
#include "IOverlay.h"



class MessageBox_Goal:
	public MessageBoxBase
{
public:
	MessageBox_Goal();
	MessageBox_Goal( const std::wstring &Message, const D2D1_RECT_F &Position );
	~MessageBox_Goal();

	std::wstring GetString()const;
};

