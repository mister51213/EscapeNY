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
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "texture.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Model
////////////////////////////////////////////////////////////////////////////////
class Model
{
private:
    // Definition of the vertex type that will be used with the vertex buffer in this class.
    // *This typedef must match the layout in the ColorShader class*
    
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture; // now use texture instead of color
	};

public:
	Model();
	Model(const Model&);
	~Model();
    
	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
    
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
    // The Render function puts the model geometry on the video card to prepare it for drawing by the color shader.
	void RenderBuffers(ID3D11DeviceContext*);

    // Load and release the texture that will be used to render this model.
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount; // keep track of the size of each buffer

    // Used for loading, releasing, and accessing the texture resource for this model.
	Texture* m_Texture;
};