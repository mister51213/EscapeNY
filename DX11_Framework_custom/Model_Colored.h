#pragma once

#include "Model.h"

class Model_Colored :public Model
{
public:
	Model_Colored() = default;
	~Model_Colored() = default;

	bool Initialize( const PrimitiveFactory &PrimMaker, const Graphics &Gfx )/*override*/;

	std::vector<VertexPositionColorNormalType> m_vertices;
};
