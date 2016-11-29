#pragma once
/*
FontLoader.h
7/11/2016

The FontLoader class is use to create dwrite font objects which will be used to 
load fonts and font styles (bold, italic, etc...).  
*/
#pragma comment(lib, "dwrite.lib")
#include "Includes.h"

using Microsoft::WRL::ComPtr;

class FontLoader
{
public:
	bool Initialize();
	ComPtr<IDWriteTextFormat>MakeFont(
		const std::wstring &FontName,								// Name of font, like L"Arial"
		float FontHeight = 16.f,									// Size of font
		DWRITE_FONT_WEIGHT FontWeight = DWRITE_FONT_WEIGHT_NORMAL,	// Bold, Normal, Light, etc
		DWRITE_FONT_STYLE FontStyle = DWRITE_FONT_STYLE_NORMAL		// Italic, Normal, Oblique
	)const;

private:
	ComPtr<IDWriteFactory> m_pFactory;
};
