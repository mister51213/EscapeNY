#pragma once

#include "Includes.h"
#include "Actor.h"
#include "ResourceManager.h"

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

	void Initialize( UINT Width, UINT Height, Graphics *const pGraphics, ResourceManager *const pResource );
	DirectX::XMUINT2 GetStartCellCoord()const;
	DirectX::XMUINT2 GetEndCellCoord()const;
	DirectX::XMFLOAT3 GetStartPosition()const;
	DirectX::XMFLOAT3 GetEndPosition()const;
	UINT GetWidth()const;
	UINT GetHeight()const;

	bool HasReachedEnd( const Actor &crActor )const;

	void SetCells( std::vector<Actor> &&pCells );
	DirectX::XMFLOAT3 GetBoundingBox(const DirectX::XMFLOAT3 &Position)const;
	const std::vector<Actor>& GetTiles()const;
	
private:
	UINT m_width, m_height;
	DirectX::XMFLOAT3 m_cellSize;
	std::vector<Actor> m_pCells;
	DirectX::XMUINT2 m_startCell, m_endCell;

};

