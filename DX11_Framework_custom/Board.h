#pragma once

#include "Actor.h"
#include "Model_Colored.h"

class Board :public Actor
{
public:
	static constexpr uint8_t NORTHWALL = 0b0001;
	static constexpr uint8_t EASTWALL = 0b0010;
	static constexpr uint8_t SOUTHWALL = 0b0100;
	static constexpr uint8_t WESTWALL = 0b1000;

	class Cell :public Actor
	{
	public:
		Cell(const ModelSpecs_W & Specs);

		void Initialize( std::shared_ptr<Model> pModel );
		const std::shared_ptr<Model> &GetModel()const;
		void Visited();
		void RemoveWall( const uint8_t Direction );
		bool WasVisited()const;
		uint8_t GetWall()const;
	private:
		uint8_t m_wall;
		bool m_visited;
		std::shared_ptr<Model> m_pModel;
	};

	Board();
	~Board();
	// Passing shared_ptr by value so the ref count DOES increment since Board
	// does share ownership of the model.
	bool Initialize( unsigned Width, unsigned Height );
	std::vector<Cell> &GetCellArray();
	unsigned GetWidth()const;
	unsigned GetHeight()const;
	const std::shared_ptr<Model> &GetModel()const;
	void SetStartAndEnd( unsigned StartCellIndex, unsigned EndCellIndex );
private:
	std::vector<Cell> m_cells;
	std::shared_ptr<Model> m_pModel;
	unsigned m_width, m_height;
	unsigned m_start, m_end;
};

