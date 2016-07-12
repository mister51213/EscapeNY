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

#include <string>
#include <wincodec.h>
#include <wrl.h>

#pragma comment(lib, "windowscodecs.lib")

using WicBitmapResult = std::pair<HRESULT, Microsoft::WRL::ComPtr<IWICBitmap>>;

class ImageLoader
{
public:
	ImageLoader();
	~ImageLoader();

	bool Initialize();
	WicBitmapResult  CreateBitmap( const std::wstring &Filename )const;
	WicBitmapResult  CreateBitmap( const UINT Width, const UINT Height )const;
private:
	Microsoft::WRL::ComPtr<IWICImagingFactory> m_pFactory;
};

