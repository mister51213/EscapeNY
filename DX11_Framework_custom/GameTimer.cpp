#include "GameTimer.h"

GameTimer::GameTimer()
{
	Start();
}

void GameTimer::Start()
{
	// Number of seconds since 00:00 hours, Jan 1, 1970 UTC 
	// set current time
	start = clock.now();
	clockStopped = false;
}

void GameTimer::Stop()
{
	// TODO: CHECK FOR COUNTING OVERFLOW POSSIBILITY
	stop = clock.now();
	clockStopped = true;
}

long long GameTimer::Reset()
{
	// TODO: CHECK FOR COUNTING OVERFLOW POSSIBILITY
	Stop();
	const long long elapsed = MilliSPassed();
	Start();

	return elapsed;
}

long long GameTimer::MilliSPassed()
{
	return clockStopped ?
		duration_cast< milliseconds >( stop - start ).count() :
		duration_cast< milliseconds >( clock.now() - start ).count();	
}

float GameTimer::SecondsPassed()
{
	int mPassed = MilliSPassed();
	float seconds = (float)mPassed * 0.001f;
	return seconds;
}

bool GameTimer::EnoughTimePassed()
{
	// see if more than .250 seconds passed since last time
	if ( MilliSPassed() > 250 ) // milliseconds
	{
		Reset();
		return true;
	}
	return false;
}

