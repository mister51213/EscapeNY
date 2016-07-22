#pragma once

#include "Model.h"

class Model_Colored :public Model
{
public:
	Model_Colored();
	~Model_Colored();

	bool Initialize( const PrimitiveMaker &PrimMaker, const Graphics &Gfx )override;
	std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputElementDescriptions() const override;

private:
	std::vector<VertexPositionColorType> vertices;
};
