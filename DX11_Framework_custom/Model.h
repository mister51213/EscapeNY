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
#include "PrimitiveMaker.h"
using namespace DirectX;

////////////////////////
// Class name: Model
////////////////////////
class Model
{
    
public:
	Model();
	Model(const Model&);
	~Model();

    // This pure virtual initializer takes into account different types of models
    // (textured and primitive, for example)
	virtual bool Initialize( const PrimitiveMaker &PrimMaker, const Graphics &Gfx ) = 0;
	virtual std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputElementDescriptions() const = 0;

	XMMATRIX GetWorldMatrix()const;
	DWORD GetVertexCount()const;
	DWORD GetIndexCount()const;
	ID3D11Buffer *GetVertexBuffer()const;
	ID3D11Buffer *GetIndexBuffer()const;
	UINT GetStride()const;

protected:
	template<class VertexType>
    bool initializeBuffers(VertexType *pVertexData, 
        DWORD *pIndexData, 
        const Graphics &Gfx)
    {
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
        vertexData.pSysMem = pVertexData;
        vertexData.SysMemPitch = 0;
        vertexData.SysMemSlicePitch = 0;

        // Now create the vertex buffer.
        auto pDevice = Gfx.GetDirect3D()->GetDevice();
        HRESULT result = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.GetAddressOf());
        RETURN_IF_FAILED(result);

        // Set up the description of the static index buffer.
        D3D11_BUFFER_DESC indexBufferDesc{};
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(DWORD) * m_indexCount;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;
        indexBufferDesc.StructureByteStride = 0;

        // Give the subresource structure a pointer to the index data.
        D3D11_SUBRESOURCE_DATA indexData{};
        indexData.pSysMem = pIndexData;
        indexData.SysMemPitch = 0;
        indexData.SysMemSlicePitch = 0;

        // Create the index buffer.
        result = pDevice->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.GetAddressOf());
        RETURN_IF_FAILED(result);

        return true;
    }
    
protected:
	XMMATRIX m_worldMatrix;
	comptr<ID3D11Buffer> m_vertexBuffer, m_indexBuffer;
	int m_vertexCount, m_indexCount; // keep track of the size of each buffer
	UINT m_Stride;
};