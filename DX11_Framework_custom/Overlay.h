#pragma once

#include "Includes.h"
#include "FontLoader.h"
#include "IOverlay.h"

class Input;
class Graphics;

using Microsoft::WRL::ComPtr;

class Overlay :public IOverlay
{
public:
	bool Initialize( const Graphics &Gfx )override;
	void Render(const Graphics &Gfx)override;
	
	void Update(const Input & User);
	void ResetGoalFlag();
	void PlayerReachGoal();
	bool WantsReset();
private:
	ComPtr<IDWriteTextFormat> m_Font;
	std::unique_ptr< MessageBoxBase > msg;
	
	bool m_reachedGoal = false;
	bool m_wantsReset = false;
};