#pragma once

#include "Model.h"

class Model_Textured :public Model
{
public:
    Model_Textured();
	//Model_Textured(XMFLOAT3 pos);
	~Model_Textured();

	bool Initialize( const PrimitiveFactory &PrimMaker, const Graphics &Gfx )override;

private:
	std::vector<VertexBufferTypeAllInOne> m_Vertices;
};