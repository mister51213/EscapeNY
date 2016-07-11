#pragma once
/*
FontLoader.h
7/11/2016

The FontLoader class is use to create dwrite font objects which will be used to 
load fonts and font styles (bold, italic, etc...).  
*/
#include <dwrite.h>
#include <wrl.h>
#pragma comment(lib, "dwrite.lib")

class FontLoader
{
public:
	FontLoader();
	~FontLoader();

	bool Initialize();
	IDWriteTextFormat *GetTextFormat()const;
private:
	Microsoft::WRL::ComPtr<IDWriteFactory> m_pFactory;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTextFormat;
};

