#include "FontLoader.h"
#include "Utilities.h"



bool FontLoader::Initialize()
{
	RETURN_IF_FAILED( DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, 
		__uuidof( m_pFactory.Get() ), &m_pFactory )
	);

	return true;
}

ComPtr<IDWriteTextFormat> FontLoader::MakeFont( const std::wstring &FontName, float FontHeight, 
	DWRITE_FONT_WEIGHT FontWeight,	DWRITE_FONT_STYLE FontStyle ) const
{
	ComPtr<IDWriteTextFormat> pTextFormat;
	HRESULT hr = m_pFactory->CreateTextFormat( FontName.c_str(), nullptr, FontWeight,
		FontStyle, DWRITE_FONT_STRETCH_NORMAL, FontHeight, L"en-us",
		pTextFormat.GetAddressOf() );

	return pTextFormat;
}