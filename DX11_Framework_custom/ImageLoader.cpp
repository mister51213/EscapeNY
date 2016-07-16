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
	
	return true;
}

// Normally supports bmp, gif, jpg, ico, png, tiff
WicBitmapResult ImageLoader::CreateBitmap( const std::wstring & Filename )const // can't change any class members
{
	std::wstring extension( Filename.substr( Filename.size() - 3, 3 ) );
    // check to see if the image file is targa; in which case it will have to 
    // use our custome image loading function
	int cmpResult = extension.compare( L"tga" );

	UINT width = 0, height = 0;

	if( cmpResult == 0 )
	{
		return loadTarga( Filename );
	}

    // Functions for loading bitmap into memory
    // Supported formats: bmp, gif, jpg, ico, png, tiff
    else
	{
		WicBitmapResult imgResult;

		// Create the decoder from the file
		ComPtr<IWICBitmapDecoder> pDecoder;
		imgResult.first = m_pFactory->CreateDecoderFromFilename( Filename.c_str(), nullptr,
			GENERIC_READ, WICDecodeMetadataCacheOnDemand, pDecoder.GetAddressOf() );
		if( FAILED( imgResult.first ) )
		{
			return imgResult;
		}
		
		// Decode the image, which for bitmaps is the 0th frame
		ComPtr<IWICBitmapFrameDecode> pFrame;
		imgResult.first = pDecoder->GetFrame( 0, pFrame.GetAddressOf() );
		if( FAILED( imgResult.first ) )
		{
			return imgResult;
		}

		// Create the format converter, used to convert to different color
		// palettes and such.
		ComPtr<IWICFormatConverter> pConverter;
		imgResult.first = m_pFactory->CreateFormatConverter( pConverter.GetAddressOf() );
		if( FAILED( imgResult.first ) )
		{
			return imgResult;
		}

		// Initialize the converter using the frame grabbed.
		// This is just to make sure that the colors are in BGRA order
        // TODO: Change pixel format from BGRA to RGBA to complement DirectX colors
		imgResult.first = pConverter->Initialize( pFrame.Get(), GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone, nullptr, 1.f, WICBitmapPaletteTypeCustom );
		if( FAILED( imgResult.first ) )
		{
			return imgResult;
		}

        // ComPtrs are like shared pointers; variables on the stack that function as 
        // WRAPPERS around a pointer; there can multipile comptprs representing one
        // pointer (they're reference counted); you can delete one of them and the others
        // will keep track of when to delete themselves, and ultimately destroy the pointed to 
        // object and release the memory.

		// Create the bitmap from the converter interface
		imgResult.first = m_pFactory->CreateBitmapFromSource( pConverter.Get(), WICBitmapCacheOnDemand,
			imgResult.second.GetAddressOf() );
		if( FAILED( imgResult.first ) )
		{
			return imgResult;
		}

		return imgResult;
	}
}

///////////////
// FOR FONTS //
///////////////
//TODO: May use this otherwise; add a comment if you do
// TODO: Change pixel format from BGRA to RGBA to complement DirectX colors
// Create an empty bitmap using width and height that you give it
// Will be used for font rendering
WicBitmapResult ImageLoader::CreateBitmap( const UINT Width, const UINT Height )const
{
	// Create temp bitmap
	Microsoft::WRL::ComPtr<IWICBitmap> pBitmap;

	// Create bitmap from memory using the temp bitmap
	HRESULT hr = m_pFactory->CreateBitmap(
		Width, Height,								// Width and height of bitmap
		GUID_WICPixelFormat32bppPBGRA,				// BGRA pixel format
		WICBitmapCacheOnDemand,						// Cache on demand, not sure what this does
		&pBitmap									// Pass address of the bitmap interface
	);

	return{hr, pBitmap};							// if creation fails, the pointer is nullptr
}

// Custom Targa loading function just in case 
// (Default supported formats of WIC API are bmp, gif, jpg, ico, png, tiff)
WicBitmapResult ImageLoader::loadTarga( const std::wstring &Filename)const
{
	// Found out that this procedure doesn't support compressed or palette based images
	// The tga file must be an RGB uncompressed image

	WicBitmapResult bitmapResult;

	// Open the targa file for reading in binary.
	std::ifstream file( Filename, std::ios::binary );
	bitmapResult.first = file.is_open() ? S_OK : STG_E_FILENOTFOUND;
	if( FAILED( bitmapResult.first ) )
	{
		return std::move(bitmapResult);
	}

	// Read in the file header.
	TargaHeader targaFileHeader{};
	file.read( reinterpret_cast<char*>(&targaFileHeader), sizeof( TargaHeader ) );

	// Check that width and height are greater than 0 and that it is 32 bit and not 24 bit.
	bitmapResult.first =
		( targaFileHeader.width > 0 ) && 
		( targaFileHeader.height > 0 ) && 
		( targaFileHeader.bpp == 32 ) ? S_OK : STG_E_INVALIDHEADER;
	if( FAILED( bitmapResult.first ) )
	{
		file.close();
		return std::move( bitmapResult );
	}

	// Create an empty WIC bitmap to copy the image data into
	bitmapResult = CreateBitmap( targaFileHeader.width, targaFileHeader.height );
	if( FAILED( bitmapResult.first ) )
	{
		file.close();
		return std::move( bitmapResult );
	}

	// Calculate the size of the 32 bit image data.	
	UINT imageSize = targaFileHeader.width * targaFileHeader.height * ( targaFileHeader.bpp / 8 );

	// Acquire the bitmap lock
	comptr<IWICBitmapLock> pLock;
	WICRect rect{0, 0, targaFileHeader.width, targaFileHeader.height};
	bitmapResult.first = bitmapResult.second->Lock( &rect, WICBitmapLockWrite, pLock.GetAddressOf() );
	if( FAILED( bitmapResult.first ) )
	{
		file.close();
		return std::move( bitmapResult );
	}

	// Read in the targa image data.
	char *pData = nullptr;
	bitmapResult.first = pLock->GetDataPointer( &imageSize, reinterpret_cast<BYTE**>( &pData ) );
	if( FAILED( bitmapResult.first ) )
	{
		file.close();
		return std::move( bitmapResult );
	}

	file.read( pData, imageSize );
	bitmapResult.first = pData != nullptr ? S_OK : STG_E_READFAULT;
	if( FAILED( bitmapResult.first ) )
	{
		file.close();
		return std::move( bitmapResult );
	}
	
	file.close();

	return bitmapResult;
}
