#pragma once

#include "PrimitiveFactory.h"
#include "Texture.h"
#include "Includes.h"
#include "Utilities.h"
#include "FontLoader.h"
#include "Graphics.h"
#include "Model_Textured.h"
#include "Camera.h"

class Overlay
{
public:
	Overlay();
	~Overlay();

	bool Initialize( const Graphics &Gfx );
	void Render(const Graphics &Gfx);
	
	void ResetGoalFlag();
	void PlayerReachGoal();

private:
	D2D1_RECT_F m_SurfaceRect;
	Model_Textured m_Model_Textured;
	comptr<IDWriteTextFormat> m_Font;
	std::wstring m_goalMessage;
	bool m_reachedGoal = false;
};