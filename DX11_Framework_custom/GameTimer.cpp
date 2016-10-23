#include "GameTimer.h"

GameTimer::GameTimer()
{
	Start();
}

GameTimer::~GameTimer()
{}

void GameTimer::Start()
{
	// set current time
	lastTime = time( 0 );
}

bool GameTimer::EnoughTimePassed()
{
	// see if more than .250 seconds passed since last time
	if( difftime( time( 0 ), lastTime ) > 250.0f )
	{
		Start(); // reset timer
		return true;
	}
}