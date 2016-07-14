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
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
    
	m_Texture = 0;
}

Model::Model(const Model& other)
{
}

Model::~Model()
{
}

bool Model::Initialize(
    ID3D11Device* device, 
    ID3D11DeviceContext* deviceContext, 
    char* textureFilename)
{
	bool result;

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}
    
    // Load the texture for this model.
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}
	
    return true;
}

void Model::Shutdown()
{
    // Release the model texture object that was loaded during initialization.
    ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}

/////////////////////////////////////////////////////////////////////////////////
// This function will be called from the Graphics::Render() function.
// This function calls RenderBuffers to put the vertex and index buffers 
// on the graphics pipeline so the color shader will be able to render them.
/////////////////////////////////////////////////////////////////////////////////

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);
	return;
}

//  Returns the number of indices in the model. The color shader will need this information to draw this model.
int Model::GetIndexCount()
{
	return m_indexCount;
}

// Returns the model texture resource. The texture shader will need access to this texture to render the model.
ID3D11ShaderResourceView* Model::GetTexture()
{
	return m_Texture->GetTexture();
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
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

    // First create two temporary arrays to hold the vertex and index data that we will use later to populate the final buffers with.
    
	// Set the number of vertices in the vertex array.
	m_vertexCount = 3;

	// Set the number of indices in the index array.
	m_indexCount = 3;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

    // Now fill both the vertex and index array with the three points of the triangle as well as the index to each of the points. 
    // *POINTS ARE CREATED IN CLOCKWISE ORDER*
    // *If you do this counter clockwise it will think the triangle is 
    // facing the opposite direction and not draw it due to back face culling.
    
	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
    vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].texture = XMFLOAT2(0.5f, 0.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

    // use vertex and index arrays to create the vertex and index buffers.
    
	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

    // After the vertex buffer and index buffer have been created,
    // you can delete the vertex and index arrays as they are no 
    // longer needed since the data was copied into the buffers.

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

// Release the vertex buffer and index buffer that were created in the InitializeBuffers function.

void Model::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

// Called from the Render function. The purpose of this function is to set the vertex buffer and index buffer as active on the input assembler in the GPU. 
// Then the GPU can use the HLSL SHADER to render the buffer.

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

// Create the texture object and then initialize it with the input 
// file name provided. This function is called during initialization.

bool Model::LoadTexture(
    ID3D11Device* device, 
    ID3D11DeviceContext* deviceContext, 
    char* filename)
{
	bool result;

	// Create the texture object.
	m_Texture = new Texture;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

// Release the texture object that was created and 
// loaded during the LoadTexture function.
void Model::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}
