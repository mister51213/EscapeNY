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
#include "PrimitiveFactory.h"
#include "DXUtils.h"

////////////////////////
// Class name: Model
////////////////////////
class Model
{
public:
    Model()
    {}
    Model(const Model& other)
    {}
    ~Model()
    {}

    // This pure virtual initializer takes into account different types of models
    // (textured and primitive, for example)
    virtual bool Initialize(const PrimitiveFactory &PrimMaker, const Graphics &Gfx) = 0;

    // Returns the number of vertices, sometimes may be different than 
// the index count
    DWORD GetVertexCount()const
    {
        return m_vertexCount;

    }
    // Returns the number of indices in the model. The color shader will 
// need this information to draw this model.
    DWORD GetIndexCount()const
    {
        return m_indexCount;
    }
    ID3D11Buffer *GetVertexBuffer()const
    {
        return m_vertexBuffer.Get();
    }
    ID3D11Buffer *GetIndexBuffer()const
    {
        return m_indexBuffer.Get();
    }
    ///////////////////////////////////////////////////////////////////////
// Stride is the size of one unit of info in an array
// for example one row in a pixel array representing the screen (pitch)
// or one Vertex containing 3D COORDINATE and UV COORDINATE info
///////////////////////////////////////////////////////////////////////
    UINT GetStride()const
    {
        return m_Stride;
    }

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
    comptr<ID3D11Buffer> m_vertexBuffer, m_indexBuffer;
    int m_vertexCount, m_indexCount; // keep track of the size of each buffer
    UINT m_Stride;
};