#include "MessageBox_Title.h"



MessageBox_Title::MessageBox_Title()
{
}

MessageBox_Title::MessageBox_Title( const std::wstring & Message, const D2D1_RECT_F & Position )
	:
	MessageBoxBase( Message, Position )
{
}


MessageBox_Title::~MessageBox_Title()
{
}

std::wstring MessageBox_Title::GetString() const
{
	return message;
}
