#pragma once

#include "Includes.h"
#include "Utilities.h"

class Shader_StateMachine
{
public:
	Shader_StateMachine();
	~Shader_StateMachine();


protected:
	void SetVSConstantBuffer(
		int Index,
		UINT BufferSize,
		void *pBufferData,
		ID3D11Buffer *pConstantBuffer,
		ID3D11DeviceContext *pContext )const;
	bool SetPSConstantBuffer(
		int Index,
		UINT BufferSize,
		void *pBufferData,
		ID3D11Buffer *pConstantBuffer,
		ID3D11DeviceContext *pContext )const;

};

