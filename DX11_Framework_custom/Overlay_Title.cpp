#include "Overlay_Title.h"



Overlay_Title::Overlay_Title( comptr<IDWriteTextFormat> Font )
	:
	Overlay_Base( Font )
{
}


Overlay_Title::~Overlay_Title()
{
}

bool Overlay_Title::Initialize( const Graphics &Gfx, const comptr<IDWriteTextFormat> &Font )
{
	/*FontLoader fntLoader;
	bool result = fntLoader.Initialize();
	RETURN_IF_FALSE( result );

	m_Font = fntLoader.MakeFont( L"Arial", 24.f );
	RETURN_IF_FALSE( m_Font != nullptr );*/
	
	// TODO: Make save game system		
	MessageBox_Title newGame(
		L"Start new game",
		D2D1::RectF(
			static_cast<float>( g_screenWidth / 2 - 100 ),
			static_cast<float>( g_screenHeight / 2 - 50 ),
			static_cast<float>( g_screenWidth / 2 + 100 ),
			static_cast<float>( g_screenHeight / 2 + 50 ) ) );
	m_messages.push_back( newGame );
	// TODO: Make a loadGame message box

	return true;
}

void Overlay_Title::Render( const Graphics & Gfx )
{
	Gfx.BeginDraw2D();
	Gfx.RenderString(
		m_messages[ m_selectedMessage ].GetString(),
		m_Font.Get(),
		m_messages[ m_selectedMessage ].GetRect()
	);
}

void Overlay_Title::Update( Input * const pInput )
{
	if( pInput->IsKeyDown( VK_DOWN ) && m_selectedMessage < ( m_messages.size() - 1 ) )
	{
		++m_selectedMessage;
	}
	else if( pInput->IsKeyDown( VK_UP ) && m_selectedMessage > 0 )
	{
		--m_selectedMessage;
	}
}
