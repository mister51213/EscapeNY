#include "Model_TexturedNM.h"
#include "Graphics.h"
#include "Utilities.h"

bool Model_TexturedNM::Initialize( const PrimitiveFactory & PrimMaker, const Graphics & Gfx )
{
	// Set the stride for this model type
	m_Stride = sizeof( VertexBufferTypeAllInOneNMap );

	// Create the vertex array.
	auto vertPosition = PrimMaker.GetVertices();

	// Set the number of verticex indices in the vertex array.
	m_vertexCount = vertPosition.size();
	m_indexCount = vertPosition.size();

	// Get the color
	auto color = PrimMaker.GetColor();
	// Get the texture coordinates
	auto uvs = PrimMaker.GetUVs();
	// Get normals
	auto normals = PrimMaker.GetNormals();
	// Get precalculated Tangent, biTangent and CoNormal
	auto tangent = PrimMaker.GetTangent();
	auto binormal = PrimMaker.GetBiTangent();
	// TODO: later add functionality to deal w files w NO normals
	// Load the vertex buffer array with data.
	std::vector<VertexBufferTypeAllInOneNMap> pVertices( m_vertexCount );
	for( UINT idx = 0; idx < m_vertexCount; ++idx )
	{
		pVertices[ idx ].color = color;
		pVertices[ idx ].position = vertPosition[ idx ];
		pVertices[ idx ].uv = uvs[ idx ];
		pVertices[ idx ].tangent = tangent[ idx ];
		pVertices[ idx ].binormal = binormal[ idx ];
		pVertices[ idx ].normal = normals[ idx ];
	}

	// Load the index array with data.
	auto indices = PrimMaker.GetIndices();

	// Set the number of indices in the index array.
	m_indexCount = indices.size();

	// use vertex and index arrays to create the vertex and index buffers.
	bool result = initializeBuffers( pVertices.data(), indices.data(), Gfx );
	RETURN_MESSAGE_IF_FALSE( result, L"Could not initialize the model's buffers." );

	return true;
}
