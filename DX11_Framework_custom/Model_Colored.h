#pragma once

#include "Model.h"

class Model_Colored :public Model
{
public:
    Model_Colored();
    //Model_Colored(XMFLOAT3 pos); // initialize base position
	~Model_Colored();

	bool Initialize( const PrimitiveFactory &PrimMaker, const Graphics &Gfx )override;

private:
	std::vector<VertexBufferTypeAllInOne> m_vertices;
};
