#pragma once
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

