#pragma once

#include <chrono>
using namespace std::chrono;
// TODO: will this cause a conflict w Includes.h?

class GameTimer
{
public:
	GameTimer();
	~GameTimer();

	void Start();

bool EnoughTimePassed();

private:
	time_t lastTime;
};

