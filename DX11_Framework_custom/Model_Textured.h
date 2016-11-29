#pragma once

#include "Model.h"

class Model_Textured :public Model
{
public:
	bool Initialize( const PrimitiveFactory &PrimMaker, const Graphics &Gfx )override;

private:
	std::vector<VertexBufferTypeAllInOne> m_Vertices;
};