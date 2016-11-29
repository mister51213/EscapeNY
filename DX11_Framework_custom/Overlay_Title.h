#pragma once

#include "Overlay_Base.h"
#include "MessageBox_Title.h"

class Graphics;
class Input;

class Overlay_Title:
	public Overlay_Base
{
public:
	Overlay_Title( Microsoft::WRL::ComPtr<IDWriteTextFormat> Font );
	~Overlay_Title();

	bool Initialize( const Graphics &Gfx, const Microsoft::WRL::ComPtr<IDWriteTextFormat> &Font );
	void Render( const Graphics &Gfx );
	void Update( Input *const pInput );
private:
	std::vector<MessageBox_Title> m_messages;
	int m_selectedMessage = 0;
};

