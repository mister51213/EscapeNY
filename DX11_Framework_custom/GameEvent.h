#pragma once

class GameEvent
{
	enum class GameState
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

	GameState ChangeState( GameState newState );
	void NewLevel();
	void StartGame();
	void EndGame();
	void MonstersAppear();
	void SpawnPlayer();
	void PathClear(); // when path to exit clear, exit flashes + sound
	void ThreatLevelUp(); // change music
	void ThreatLevelDown(); // change music

private:
	GameState m_currentState = GameState::ACTIVEPLAY;
};

