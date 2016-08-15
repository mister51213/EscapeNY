#include "Model_Textured.h"

bool Model_Textured::Initialize(
	const PrimitiveFactory &PrimMaker, const Graphics & Gfx )
{
	// TODO: later add functionality to deal w files w NO normals

	// Set the stride for this model type
	m_Stride = sizeof( m_vertices[ 0 ] );

	// Create the vertex array.
	auto verts = PrimMaker.GetVertices();
	// Get the color
	auto uvs = PrimMaker.GetUVs();
	// Get normals
	auto normals = PrimMaker.GetNormals();
	// Get color
	auto color = PrimMaker.GetColor();
	// Load the index array with data.
	auto indices = PrimMaker.GetIndices();

	// Set the number of verticex indices in the vertex array.
	m_vertexCount = verts.size();
	m_indexCount = verts.size();
	m_vertices.resize( m_vertexCount );

	// Load the vertex buffer array with data.

	for( UINT idx = 0; idx < verts.size(); ++idx )
	{
		m_vertices[ idx ] = { verts[ idx ], uvs[ idx ], normals[ idx ], color };
	}

	// use vertex and index arrays to create the vertex and index buffers.
	bool result = initializeBuffers( m_vertices.data(), indices.data(), Gfx );
	RETURN_MESSAGE_IF_FALSE( result, L"Could not initialize the model's buffers." );

	return true;
}
