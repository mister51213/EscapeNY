#pragma once

#include "Includes.h"
#include "Actor.h"

class Graphics;

enum eTileType
{
	WALL, 
	PATH
};

class TestBoard :public Actor
{
public:
	TestBoard();
	~TestBoard();

	void Initialize( UINT Width, UINT Height, Graphics *const pGraphics );

	// Accessors
	UINT GetWidth()const;
	UINT GetHeight()const;
	DirectX::XMUINT2 GetStartCellCoord()const;
	DirectX::XMUINT2 GetEndCellCoord()const;
	DirectX::XMFLOAT3 GetStartPosition()const;
	DirectX::XMFLOAT3 GetEndPosition()const;
	DirectX::XMFLOAT3 GetCellSize()const;
	AlignedAxisBoundingBox GetBoardBoundingBox( )const;
	AlignedAxisBoundingBox GetCellBoundingBox( const DirectX::XMFLOAT3 &Position )const;
	const std::vector<Actor>& GetTiles()const;
	void SetCells( std::vector<Actor> &&pCells );

	// Game logic accessor
	bool HasReachedEnd( const Actor &crActor )const;

private:
	DirectX::XMFLOAT3 m_cellSize;
	DirectX::XMUINT2 m_startCell, m_endCell;
	std::vector<Actor> m_pCells;
	UINT m_width, m_height;
};

