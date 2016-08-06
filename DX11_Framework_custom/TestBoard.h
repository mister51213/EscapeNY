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
	DirectX::XMUINT2 GetStartPosition()const;
	DirectX::XMUINT2 GetEndPosition()const;
	UINT GetWidth()const;
	UINT GetHeight()const;
	std::vector<eTileType> &GetTiles();
private:
	UINT m_width, m_height;
	/*std::vector<eTileType> m_tileTypes;*/
	DirectX::XMUINT2 m_startCell, m_endCell;
};

