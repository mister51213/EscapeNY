#pragma once

#include "Includes.h"
#include "Utilities.h"
#include "FontLoader.h"
#include "Graphics.h"
#include "Input.h"

class Overlay
{
public:
	Overlay();
	~Overlay();

	bool Initialize( const Graphics &Gfx );
	void Render(const Graphics &Gfx);
	
	void Update(Input & User);
	void ResetGoalFlag();
	void PlayerReachGoal();
	bool WantsReset();
private:
	D2D1_RECT_F m_goalMessageRect;
	comptr<IDWriteTextFormat> m_Font;
	std::wstring m_goalMessage;
	bool m_reachedGoal = false;
	bool m_wantsReset = false;
};