#pragma once

#include "IOverlay.h"

class MessageBox_Title:
	public MessageBoxBase
{
public:
	MessageBox_Title();
	MessageBox_Title( 
		const std::wstring &Message, const D2D1_RECT_F &Position );
	~MessageBox_Title();

	std::wstring GetString()const override;

};

