#include "GameTimer.h"

GameTimer::GameTimer()
{
	Start();
}

void GameTimer::Start()
{
	// set current time
	start = clock.now();
}

void GameTimer::Stop()
{
	stop = clock.now();
}

long long GameTimer::Reset()
{
	Stop();
	const long long elapsed = GetMilli();
	Start();

	return elapsed;
}

long long GameTimer::GetMilli()
{
	return clockStopped ?
		duration_cast< milliseconds >( stop - start ).count() :
		duration_cast< milliseconds >( clock.now() - start ).count();	
}

bool GameTimer::EnoughTimePassed()
{
	// see if more than .250 seconds passed since last time
	if ( GetMilli() > 250 )
	{
		Reset();
		return true;
	}
	return false;
}