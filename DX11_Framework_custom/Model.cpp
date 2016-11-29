#include "Model.h"

DWORD Model::GetVertexCount() const
{
	return m_vertexCount;
}

DWORD Model::GetIndexCount() const
{
	return m_indexCount;
}

ID3D11Buffer * Model::GetVertexBuffer() const
{
	return m_vertexBuffer.Get();
}

ID3D11Buffer * Model::GetIndexBuffer() const
{
	return m_indexBuffer.Get();
}

UINT Model::GetStride() const
{
	return m_Stride;
}