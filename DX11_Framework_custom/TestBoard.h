#pragma once

#include "Actor.h"
#include "Includes.h"

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

	void Initialize( UINT Width, UINT Height );
	DirectX::XMFLOAT3 GetStartPosition()const;
	DirectX::XMFLOAT3 GetEndPosition()const;
	UINT GetWidth()const;
	UINT GetHeight()const;
	void SetCells( std::vector<Actor> &&pCells );

	const std::vector<Actor>& GetTiles()const;
	
private:
	UINT m_width, m_height;
	std::vector<Actor> m_pCells;
	DirectX::XMUINT2 m_startCell, m_endCell;
};

