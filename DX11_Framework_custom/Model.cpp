/************************************************************************************
 * This class is responsible for encapsulating the geometry for 3D models.          *
 * It can manually setup the geometry for a polygon, or (to be added later),        *
 * read from a file. Then it creates a vertex and index buffer for the polygon      *
 * so that it can be rendered.                                                      *
 *                                                                                  *
 ***********************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Filename: model.cpp
////////////////////////////////////////////////////////////////////////////////
#include "model.h"
#include "PrimitiveMaker.h"

// initialize the vertex and index buffer pointers to null.
Model::Model()
{
	// This matrix transforms vertices from object space coordinates to
	// world space coordinates
	m_worldMatrix = XMMatrixIdentity();
}

Model::Model(const Model& other)
{
}

Model::~Model()
{
}

// Accessor functions
ID3D11Buffer * Model::GetVertexBuffer() const
{
	return m_vertexBuffer.Get();
}

ID3D11Buffer * Model::GetIndexBuffer() const
{
	return m_indexBuffer.Get();
}

///////////////////////////////////////////////////////////////////////
// Stride is the size of one unit of info in an array
// for example one row in a pixel array representing the screen (pitch)
// or one Vertex containing 3D COORDINATE and UV COORDINATE info
///////////////////////////////////////////////////////////////////////
UINT Model::GetStride() const
{
	return m_Stride;
}

// ************************************************************
// TODO: Create mesh loader to load mesh files.               *
//*************************************************************

XMMATRIX Model::GetWorldMatrix()const
{
	return m_worldMatrix;
}

// Returns the number of indices in the model. The color shader will 
// need this information to draw this model.
DWORD Model::GetIndexCount() const
{
	return m_indexCount;
}

// Returns the number of vertices, sometimes may be different than 
// the index count
DWORD Model::GetVertexCount()const
{
	return m_vertexCount;
}