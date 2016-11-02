#pragma once

class GameEvent
{
	enum class eGameState
	{
		PAUSED,
		MENU,
		CUTSCENE,
		ACTIVEPLAY,
		GAMEOVER
	};

public:
	GameEvent();
	~GameEvent();

	eGameState ChangeState( eGameState newState );
	void NewLevel();
	void StartGame();
	void EndGame();
	void MonstersAppear();
	void SpawnPlayer();
	void PathClear(); // when path to exit clear, exit flashes + sound
	void ThreatLevelUp(); // change music
	void ThreatLevelDown(); // change music

private:
	eGameState m_currentState = eGameState::ACTIVEPLAY;
};

