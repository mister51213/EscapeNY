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

// initialize the vertex and index buffer pointers to null.
Model::Model()
{
}

Model::Model(const Model& other)
{
}

Model::~Model()
{
}

bool Model::Initialize(ID3D11Device* device)
{
	bool result;

	// Initialize the world matrix to the identity matrix.
	// This matrix is used to convert the vertices of our objects into 
	// vertices in the 3D scene. 
	m_worldMatrix = XMMatrixIdentity();

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////////
// This function will be called from the Graphics::Render() function.
// This function calls RenderBuffers to put the vertex and index buffers 
// on the graphics pipeline so the color shader will be able to render them.
/////////////////////////////////////////////////////////////////////////////////

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline 
    // to prepare them for drawing.
	RenderBuffers(deviceContext);
	return;
}

void Model::GetWorldMatrix( XMMATRIX & WorldMatrix )
{
	WorldMatrix = m_worldMatrix;
}

//  Returns the number of indices in the model. The color shader will need this information to draw this model.
int Model::GetIndexCount()
{
	return m_indexCount;
}

// Create the vertex and index buffers. 
// ************************************************************
// TODO                                                       *
// When we expand this to drawing multiple polygons,          *
// we  will enable it to read in a model and create the       *
// buffers from that data file; FOR NOW, just draw a triangle *
//                                                            *
//*************************************************************
bool Model::InitializeBuffers(ID3D11Device* device)
{
    // First create two temporary arrays to hold the vertex and index data that we will use later to populate the final buffers with.
    
	// Set the number of vertices in the vertex array.
	m_vertexCount = 3;

	// Set the number of indices in the index array.
	m_indexCount = 3;

    // Now fill both the vertex and index array with the three points of the triangle as well as the index to each of the points. 
    // *POINTS ARE CREATED IN CLOCKWISE ORDER*
    // *If you do this counter clockwise it will think the triangle is 
    // facing the opposite direction and not draw it due to back face culling.
    
	// Create the vertex array.
	// Load the vertex array with data.
	std::vector<VertexType> vertices =
	{
		{XMFLOAT3( -1.0f, -1.0f, 0.0f ),	XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f )},	// Bottom left.
		{XMFLOAT3( 0.0f, 1.0f, 0.0f ),		XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f )},	// Top middle.
		{XMFLOAT3( 1.0f, -1.0f, 0.0f ),		XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f )}		// Bottom right.
	};

	// Create the index array.
	// Load the index array with data.
	std::vector<UINT> indices = 
	{
		0,		// Bottom left.
		1,		// Top middle.
		2		// Bottom right.
	};

    // use vertex and index arrays to create the vertex and index buffers.
	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData{};
	vertexData.pSysMem = vertices.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.GetAddressOf() );
	RETURN_IF_FAILED( result );

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC indexBufferDesc{};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData{};
	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.GetAddressOf() );
	RETURN_IF_FAILED( result );

	return true;
}

// Called from the Render function. The purpose of this function is to set the vertex buffer and index buffer as active on the input assembler in the GPU. 
// Then the GPU can use the HLSL SHADER to render the buffer.
void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof( VertexType );
	unsigned int offset = 0;	
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}