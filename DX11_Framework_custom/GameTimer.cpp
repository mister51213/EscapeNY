#include "GameTimer.h"



GameTimer::GameTimer()
{}


GameTimer::~GameTimer()
{}

GameTimer::GameTimer()
{
	startedAt = 0;
	pausedAt = 0;
	paused = false;
	started = false;
}

bool GameTimer::IsStarted()
{
	return started;
}

bool GameTimer::IsStopped()
{
	return !started;
}

bool GameTimer::IsPaused()
{
	return paused;
}

bool GameTimer::IsActive()
{
	return !paused & started;
}

void GameTimer::Pause()
{
	if( paused || !started )
		return;

	paused = true;
	pausedAt = clock();
}

void GameTimer::Resume()
{
	if( !paused )
		return;

	paused = false;
	startedAt += clock() - pausedAt;
}

void GameTimer::Stop()
{
	started = false;
}

void GameTimer::Start()
{
	if( started )
		return;

	started = true;
	paused = false;
	startedAt = clock();
}

void GameTimer::Reset()
{
	paused = false;
	startedAt = clock();
}

clock_t GameTimer::GetTicks()
{
	if( !started )
		return 0;

	if( paused )
		return pausedAt - startedAt;

	return clock() - startedAt;
}
