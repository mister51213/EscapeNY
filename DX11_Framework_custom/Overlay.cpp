#include "Overlay.h"

Overlay::Overlay()
{}

Overlay::~Overlay()
{}

bool Overlay::Initialize( const Graphics &Gfx )
{
	FontLoader fntLoader;
	bool result = fntLoader.Initialize();
	RETURN_IF_FALSE( result );

	m_Font = fntLoader.MakeFont( L"Arial",24.f );
	RETURN_IF_FALSE( m_Font != nullptr );
	
	m_goalMessage = L"Congratulations!!! \nYou've reached the end of the maze.  \nWould you like to load another one? \n";
	return true;
}

// The Render function puts the model geometry on the video card to prepare it for drawing by the color shader.
void Overlay::Render( const Graphics &Gfx )
{
	// Begin and End draw are done here because the overlay is rendering to a texture
	// and not to the back buffer, so Overlay needs to control when to begin and end the
	// drawing.
	
	// All overlay rendering should be done between BeginDraw and EndDraw
	Gfx.BeginDraw2D();

	if( m_reachedGoal )
	{
		Gfx.RenderString(
			m_goalMessage,
			m_Font.Get(),
			{ 40.f, 40.f, 120.f, 40.f } 
		);
	}
	// The rect is where to draw the text and when to either wrap the text or cut it off		
	Gfx.RenderString( L"Hello", m_Font.Get(), D2D1::RectF( 0.f, 0.f, 80.f, 60.f ) );
	
	// Signal direct2d that we are done drawing
	Gfx.EndDraw2D();
}

void Overlay::ResetGoalFlag()
{
	m_reachedGoal = false;
}

void Overlay::PlayerReachGoal()
{
	m_reachedGoal = true;
}
