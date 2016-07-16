/************************************************************************************
 * This class is responsible for encapsulating the geometry for 3D models.          *
 * It can manually setup the geometry for a polygon, or (to be added later),        *
 * read from a file. Then it creates a vertex and index buffer for the polygon      *
 * so that it can be rendered.                                                      *
 *                                                                                  *
 ***********************************************************************************/
#pragma once

//////////////
// INCLUDES //
//////////////
#include "Graphics.h"
using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: Model
////////////////////////////////////////////////////////////////////////////////
class Model
{
private:
    // Definition of the vertex type that will be used with the vertex buffer
	// in this class.  
    // *This typedef must match the layout in the ColorShader class*
    
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	Model();
	Model(const Model&);
	~Model();
    
	bool Initialize(ID3D11Device*);
	void Render(ID3D11DeviceContext*);
	void GetWorldMatrix( XMMATRIX &WorldMatrix );
	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
    // The Render function puts the model geometry on the video card to prepare it for drawing by the color shader.
	void RenderBuffers(ID3D11DeviceContext*);
  
private:
	comptr<ID3D11Buffer> m_vertexBuffer, m_indexBuffer;
	int m_vertexCount, m_indexCount; // keep track of the size of each buffer
	XMMATRIX m_worldMatrix;
};