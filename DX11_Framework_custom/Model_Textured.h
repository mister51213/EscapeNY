#pragma once

#include "Model.h"

class Model_Textured :public Model
{
public:
    Model_Textured();
	//Model_Textured(XMFLOAT3 pos);
	~Model_Textured();

	bool Initialize( const PrimitiveFactory &PrimMaker, const Graphics &Gfx )override;
	std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputElementDescriptions() const override;

private:
	std::vector<VertexPositionUVType> m_Vertices;
};