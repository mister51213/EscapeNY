#pragma once

#include "Includes.h"
#include <DirectXCollision.h>

class DXBoundingMesh
{
public:
	DXBoundingMesh();
	~DXBoundingMesh();

	DirectX::BoundingBox m_box;
};

