#include "Model_Textured.h"
#include "PrimitiveFactory.h"

Model_Textured::Model_Textured()
{}

//Model_Textured::Model_Textured(XMFLOAT3 pos):Model::Model(pos) // initialize base position
//{}

Model_Textured::~Model_Textured()
{}

bool Model_Textured::Initialize( const PrimitiveFactory &PrimMaker, const Graphics & Gfx)
{
	// Set the stride for this model type
	m_Stride = sizeof( VertexPositionUVType );

	// Create the vertex array.
	auto verts = PrimMaker.GetVertices();

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