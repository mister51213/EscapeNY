#pragma once
/************************************************************************************
*  File name		: ImageLoader.h/.cpp											*
*  Creation Date	: 7/10/2016														*
*  Purpose			: Used to load/create images from files or memory				*
*  Description		: ImageLoader is a wrapper for the Windows Imaging Component	*
*					  API for loading images and creating compatible buffers with	*
*					  Direct2D														*
*  Requirements		:																*
*************************************************************************************/
#include "Includes.h"

class Wic;

using Microsoft::WRL::ComPtr;
using WicBitmapResult = std::pair<HRESULT, ComPtr<IWICBitmap>>;

class ImageLoader
{
	struct TargaHeader
	{
		unsigned char data1[ 12 ];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	static WicBitmapResult CreateBitmap( const std::wstring &Filename, const Wic &crWic );
	static WicBitmapResult CreateBitmap( const UINT Width, const UINT Height, const Wic &rcWic );
private:
	static WicBitmapResult loadTarga( const std::wstring &Filename, const Wic &rcWic );

};