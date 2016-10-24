#pragma once

#include <chrono>
using namespace std::chrono;
// TODO: will this cause a conflict w Includes.h?

class GameTimer
{
public:
	GameTimer();
	~GameTimer() = default;

	void Start();
	void Stop();
	long long Reset();
	// Function returns a 64bit long long int in milliseconds
	long long GetMilli();

	bool EnoughTimePassed();

private:
	high_resolution_clock clock;
	time_point<high_resolution_clock> start, stop;
	bool clockStopped = false;
};

