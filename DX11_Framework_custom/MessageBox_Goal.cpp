#include "MessageBox_Goal.h"



MessageBox_Goal::MessageBox_Goal()
{
}

MessageBox_Goal::MessageBox_Goal( const std::wstring & Message, const D2D1_RECT_F & Position )
	:
	MessageBoxBase( Message, Position )
{
}


MessageBox_Goal::~MessageBox_Goal()
{
}

std::wstring MessageBox_Goal::GetString() const
{
	return message;
}
