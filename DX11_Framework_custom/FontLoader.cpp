#include "FontLoader.h"



FontLoader::FontLoader()
{}


FontLoader::~FontLoader()
{}

bool FontLoader::Initialize()
{
	HRESULT hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof( m_pFactory.Get() ),
		reinterpret_cast<IUnknown**>( m_pFactory.GetAddressOf() )
	);

	if( FAILED( hr ) )
	{
		return false;
	}

	hr = m_pFactory->CreateTextFormat( L"Arial", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		16.f, L"en-us", m_pTextFormat.GetAddressOf() );

	if( FAILED( hr ) )
	{
		return false;
	}
	return true;
}

IDWriteTextFormat * FontLoader::GetTextFormat() const
{
	return m_pTextFormat.Get();
}
