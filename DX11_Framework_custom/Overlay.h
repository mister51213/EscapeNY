#pragma once

#include "Includes.h"
#include "Utilities.h"
#include "FontLoader.h"
#include "IOverlay.h"
#include "Input.h"

class Overlay :public IOverlay
{
public:
	Overlay();
	~Overlay();

	bool Initialize( const Graphics &Gfx )override;
	void Render(const Graphics &Gfx)override;
	
	void Update(const Input & User);
	void ResetGoalFlag();
	void PlayerReachGoal();
	bool WantsReset();
private:
	comptr<IDWriteTextFormat> m_Font;
	std::unique_ptr< MessageBoxBase > msg;
	
	bool m_reachedGoal = false;
	bool m_wantsReset = false;
};