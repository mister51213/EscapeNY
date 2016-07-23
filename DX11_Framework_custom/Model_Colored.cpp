#include "Model_Colored.h"


Model_Colored::Model_Colored()
{
}

Model_Colored::~Model_Colored()
{}

bool Model_Colored::Initialize( const PrimitiveMaker & PrimMaker, const Graphics & Gfx )
{
	// Set the stride for this model type
	m_Stride = sizeof( VertexPositionColorType );

	// Load the vertex array with data.
	auto verts = PrimMaker.GetVertices();

	// Set the number of vertices indices in the vertex array.
	m_vertexCount = verts.size();
	m_indexCount = verts.size();

	// Get the color
	auto color = PrimMaker.GetColor();

	// Create the vertex buffer array
	UINT idx = 0;
	for( auto &v : verts )
	{
		vertices[ idx ] = {v, color};
		++idx;
	}

	// Load the index array with data.
	auto indices = PrimMaker.GetIndices();

	// Set the number of indices in the index array.
	m_indexCount = indices.size();

	// use vertex and index arrays to create the vertex and index buffers.
	bool result = initializeBuffers( vertices.data(), indices.data(), Gfx );
	RETURN_IF_FALSE( result );

	return true;
}

std::vector<D3D11_INPUT_ELEMENT_DESC> Model_Colored::GetInputElementDescriptions() const
{
	return VertexPositionColorType::CreateLayoutDescriptions();
}