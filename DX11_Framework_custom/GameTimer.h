#pragma once

#include <ctime>

// TODO: will this cause a conflict w Includes.h?

class GameTimer
{
public:
	GameTimer();
	~GameTimer();

	bool IsStarted();
	bool IsStopped();
	bool IsPaused();
	bool IsActive();

	void Pause();
	void Resume();
	void Stop();
	void Start();
	void Reset();

	clock_t GetTicks();

private:
	clock_t startedAt;
	clock_t pausedAt;
	bool started;
	bool paused;


};

