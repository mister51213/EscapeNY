#include "Overlay.h"
#include "MessageBox_Goal.h"

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
	
	float left = 100.f;
	float top = 100.f;
	float right = left + 450.f;
	float bottom = top + 40.f;
	D2D1_RECT_F rect( D2D1::RectF( left, top, right, bottom ) );
	// Multi-line strings have to use the \ at the end, and the spacing
	// from the first column of the file it is declared in, is the offset
	// it will have on screen.
	std::wstring mesg( 
		L"Congratulations!!!\n\
You've reached the end of the maze.\n\
Would you like to load another one? \n\
Press Y to regenerate the maze, ESC to close program.\n" );

	msg.reset( new MessageBox_Goal( mesg, rect ) );
	
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
			msg->GetString(),
			m_Font.Get(),
			msg->GetRect()
		);
	}
	
	// Signal direct2d that we are done drawing
	Gfx.EndDraw2D();
}

// CODE_CHANGE: Made function parameter const
void Overlay::Update( const Input & User )
{
	if( m_reachedGoal )
	{
		if( User.IsKeyDown( 'Y' ) )
		{
			m_wantsReset = true;
			ResetGoalFlag();
		}
	}
}

void Overlay::ResetGoalFlag()
{
	m_reachedGoal = false;
}

void Overlay::PlayerReachGoal()
{
	m_reachedGoal = true;
}

bool Overlay::WantsReset()
{
	bool reset = m_wantsReset;
	m_wantsReset = false;
	return reset;
}
