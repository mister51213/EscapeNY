#include "GameEvent.h"



GameEvent::GameEvent()
{}


GameEvent::~GameEvent()
{}

GameEvent::eGameState GameEvent::ChangeState( eGameState newState )
{
	return eGameState();
}

void GameEvent::NewLevel()
{}
