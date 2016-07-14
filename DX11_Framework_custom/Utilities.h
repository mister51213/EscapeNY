#pragma once

// Permit me to be a little lazy :)
// This is just an alias to shorten the amount of typing
template<class T>
using comptr = Microsoft::WRL::ComPtr<T>;

// Helper macros to decrease the amount of typing and space
#define RETURN_IF_FALSE(Result)\
if( !( Result ) ) \
{ \
	return false; \
}\
else int a = 0
#define RETURN_MESSAGE_IF_FALSE(Result, Message)\
if(!(Result))\
{\
	ShowCursor(TRUE);\
	MessageBox(nullptr, Message, L"Error!", MB_OK);\
	return false;\
}\
else int a = 0
#define RETURN_IF_FAILED(Result) RETURN_IF_FALSE( SUCCEEDED( ( Result ) ) )
