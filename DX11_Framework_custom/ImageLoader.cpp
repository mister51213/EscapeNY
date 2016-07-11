#include "ImageLoader.h"
/************************************************************************************
*  File name		: ImageLoader.cpp												*
*  Creation Date	: 7/10/2016														*
*  Purpose			: Used to load/create images from files or memory				*
*  Description		: ImageLoader is a wrapper for the Windows Imaging Component	*
*					  API for loading images and creating compatible buffers with	*
*					  Direct2D														*
*  Requirements		:																*
*************************************************************************************/

using namespace Microsoft::WRL;

ImageLoader::ImageLoader()
{}


ImageLoader::~ImageLoader()
{}

bool ImageLoader::Initialize()
{
	// Initialize COM interface
	CoInitialize( nullptr );

	// Create an instance of the imaging factory
	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS( m_pFactory.GetAddressOf() )
	);
	if( FAILED( hr ) )
	{
		return false;
	}
	return true;
}

WicBitmapResult ImageLoader::CreateBitmap( const std::wstring & Filename )const
{
	// Initial variables needed to load and convert image file
	ComPtr<IWICBitmapDecoder> pDecoder;
	ComPtr<IWICBitmapFrameDecode> pFrame;
	ComPtr<IWICFormatConverter> pConverter;
	ComPtr<IWICBitmap> pBitmap;

	// Create the decoder from the file
	HRESULT hr = m_pFactory->CreateDecoderFromFilename( Filename.c_str(), nullptr,
		GENERIC_READ, WICDecodeMetadataCacheOnDemand, pDecoder.GetAddressOf() );

	if( SUCCEEDED( hr ) )
	{
		// Decode the image, which for bitmaps is the 0th frame
		hr = decoder->GetFrame( 0, pFrame.GetAddressOf() );
	}
	if( SUCCEEDED( hr ) )
	{
		// Create the format converter, used to convert to different color
		// palettes and such.
		hr = m_pFactory->CreateFormatConverter( pConverter.GetAddressOf() );
	}
	if( SUCCEEDED( hr ) )
	{
		// Initialize the converter using the frame grabbed.
		// This is just to make sure that the colors are in BGRA order
		hr = pConverter->Initialize( pFrame.Get(), GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone, nullptr, 1.f, WICBitmapPaletteTypeCustom );
	}
	if( SUCCEEDED( hr ) )
	{
		// Create the bitmap from the converter interface
		hr = m_pFactory->CreateBitmapFromSource( pConverter.Get(), WICBitmapCacheOnDemand,
			pBitmap.GetAddressOf() );
	}

	return{hr,pBitmap};
}

WicBitmapResult  ImageLoader::CreateBitmap( const UINT Width, const UINT Height )const
{
	// Create temp bitmap
	Microsoft::WRL::ComPtr<IWICBitmap> pBitmap;
	// Create bitmap from memory using the temp bitmap
	HRESULT hr = m_pFactory->CreateBitmap(
		Width, Height,							// Width and height of bitmap
		GUID_WICPixelFormat32bppPBGRA,			// BGRA pixel format
		WICBitmapCacheOnDemand,					// Cache on demand, not sure what this does
		pBitmap.GetAddressOf()					// Pass address of the bitmap interface
	);
	return{hr, pBitmap};							// if creation fails, the pointer is nullptr
}
