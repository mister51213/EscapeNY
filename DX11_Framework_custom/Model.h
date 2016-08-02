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
	Model(XMFLOAT3 pos);
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

    // TODO: These shouldn't be inline
    void Move(XMFLOAT3 Offset)
    {
        m_offset.x += Offset.x;
        m_offset.y += Offset.y;
        m_offset.z += Offset.z;
        m_worldMatrix = DirectX::XMMatrixTranslation(m_offset.x, m_offset.y, m_offset.z);
    }

    // Simply modulate position member of Model;
    // this will be reflected by global GetWorldMatrix 
    // later in rendering pipeline each frame.
    void MoveAlt(XMFLOAT3 Offset)
    {
        // Vector operator overloading won't work for some reason.
        //m_Position += Offset;
        m_Position.x += Offset.x;
        m_Position.y += Offset.y;
        m_Position.z += Offset.z;
    }

    void Rotate(XMFLOAT3 axis, float angle)
    {
        //m_rotateAxis.x += axis.x;
        //m_rotateAxis.y += axis.y;
        //m_rotateAxis.z += axis.z;
        m_rotateAngle += angle;
        m_worldMatrix = XMMatrixRotationX(m_rotateAngle);
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

public:
        // TODO: use these to get world matrix each step.
    // Model stores variables for position, orientation, and scale; will be passed to GetWorldMatrix()
    XMFLOAT3 m_Position = { 1,0,0 }; // TODO: Set this initially in constructor
    XMFLOAT3 m_Orientation = { 0,0,0 }; // TODO: Set this initially in constructor
    XMFLOAT3 m_Scale = { 1,0,0 }; // TODO: Set this initially in constructor

protected:
    /////////////////////////////////////////////////////////////////////////
    // The world matrix should only be created once per frame per model and 
    // is a combination of the translation, rotation and scale matrices.
    ///////////////////////////////////////////////////////////////////////
    XMFLOAT3 m_offset = { 1,0,0 };
    XMFLOAT3 m_rotateAxis;
    float m_rotateAngle=.1f;

    // TODO: Remove this variable.
	XMMATRIX m_worldMatrix;

	comptr<ID3D11Buffer> m_vertexBuffer, m_indexBuffer;
	int m_vertexCount, m_indexCount; // keep track of the size of each buffer
	UINT m_Stride;
};