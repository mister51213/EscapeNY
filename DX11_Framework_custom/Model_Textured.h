#pragma once

#include "Model.h"

class Model_Textured :public Model
{
public:
	Model_Textured() = default;
	~Model_Textured() = default;

	bool Initialize( const PrimitiveFactory &PrimMaker, const Graphics &Gfx )/*override*/;
private:
	std::vector<VertexBufferTypeAllInOne> m_vertices;
};