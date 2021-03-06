#pragma once

#include "Actor.h"
#include "Algorithm.h"
#include "Includes.h"
#include "TestBoard.h"
#include "Utilities.h"

class ISubGame;

class Algorithm_Maze :public Algorithm
{
public:
	Algorithm_Maze( ISubGame*const pGame );
	std::vector<Actor> MakePattern( int numActors )override;
	void SetData()override{}

private:
	// Creates a list of neighbor cells
	std::vector<DirectX::XMINT2> MakeNeighborList(
		const DirectX::XMUINT2 &CurCell,
		UINT Width, UINT Height,
		std::vector<eTileType> &TileTypes );
	// Returns the index of the cell between the current cell and the 
	// next cell.
	UINT GetWallIndex(
		const DirectX::XMUINT2 &CurCell,
		const DirectX::XMUINT2 &NextCell,
		const UINT Width, const UINT Height,
		const UINT CurIdx );
	// Function to generate the maze
	std::vector<eTileType> Generate(
		const UINT Width, const UINT Height,
		const DirectX::XMUINT2 &StartCell,
		const DirectX::XMUINT2 &EndCell,
		std::stack<DirectX::XMUINT2> &VisitedCells );
	// Function to create the actor list
	std::vector<Actor> CreateActorList(
		const UINT Width, const UINT Height,
		const std::vector<eTileType> &TileTypes );
private:
	std::stack<DirectX::XMUINT2> m_visitedCells;

};