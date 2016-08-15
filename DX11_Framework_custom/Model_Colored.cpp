#include "Model_Colored.h"

bool Model_Colored::Initialize( 
	const PrimitiveFactory & PrimMaker, const Graphics & Gfx )
{
	m_Stride = sizeof( m_vertices[ 0 ] );
	// Load the vertex array with data.
	auto verts = PrimMaker.GetVertices();
	// Get the color
	auto color = PrimMaker.GetColor();
	// Get normals
	auto normals = PrimMaker.GetNormals();
	// Load the index array with data.
	auto indices = PrimMaker.GetIndices();

	// Set the number of vertices and indices in the vertex array.
	m_vertexCount = verts.size();
	m_indexCount = indices.size();

	// Create the vertex buffer array
	
    m_vertices.resize(m_vertexCount);
	for( UINT idx = 0; idx < m_vertexCount; ++idx )
	{
   		m_vertices[ idx ] = {verts[ idx ], color, normals[idx]};
		++idx;
	}

	// use vertex and index arrays to create the vertex and index buffers.
	bool result = initializeBuffers( m_vertices.data(), indices.data(), Gfx );
	RETURN_IF_FALSE( result );

	return true;
}