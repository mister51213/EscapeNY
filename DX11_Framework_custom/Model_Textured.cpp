#include "Model_Textured.h"
#include "PrimitiveMaker.h"

Model_Textured::Model_Textured()
{}

Model_Textured::~Model_Textured()
{}

bool Model_Textured::Initialize( const PrimitiveMaker &PrimMaker, const Graphics & Gfx )
{
	// Set the stride for this model type
	m_Stride = sizeof( VertexPositionUVType );

    /////////////////////////////////////////////////////
    // TODO: Add extra function here that ADDS 
    // TODO: position offset to these values after getting vertices
    /////////////////////////////////////////////////////

	// Create the vertex array.
	auto verts = PrimMaker.GetVertices();

    for each (XMFLOAT3 vertex in verts)
    {
        vertex.x += m_position.x;
        vertex.y += m_position.y;
        vertex.z += m_position.z;
    }

	// Set the number of verticex indices in the vertex array.
	m_vertexCount = verts.size();
	m_indexCount = verts.size();
	m_Vertices.resize( m_vertexCount );

	// Get the color
	auto uvs = PrimMaker.GetUVs();

	// Load the vertex buffer array with data.
	UINT idx = 0;
	for( auto &v : verts )
	{
		m_Vertices[ idx ] = {verts[ idx ], uvs[ idx ]};
		++idx;
	}

	// Load the index array with data.
	auto indices = PrimMaker.GetIndices();

	// Set the number of indices in the index array.
	m_indexCount = indices.size();

	// use vertex and index arrays to create the vertex and index buffers.
	bool result = initializeBuffers( m_Vertices.data(), indices.data(), Gfx );
	RETURN_MESSAGE_IF_FALSE( result, L"Could not initialize the model's buffers." );

	return true;
}

std::vector<D3D11_INPUT_ELEMENT_DESC> Model_Textured::GetInputElementDescriptions() const
{
	return VertexPositionUVType::CreateLayoutDescriptions();
}