#include "FontLoader.h"



FontLoader::FontLoader()
{}


FontLoader::~FontLoader()
{}

bool FontLoader::Initialize()
{
	RETURN_IF_FAILED( DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, 
		__uuidof( m_pFactory.Get() ), &m_pFactory )
	);
	throw( true );
	RETURN_IF_FAILED( m_pFactory->CreateTextFormat( L"Arial", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 16.f, L"en-us", &m_pTextFormat ) );

	return true;
}

IDWriteTextFormat * FontLoader::GetTextFormat() const
{
	return m_pTextFormat.Get();
}
