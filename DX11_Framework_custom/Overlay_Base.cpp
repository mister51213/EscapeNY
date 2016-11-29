#include "Overlay_Base.h"



Overlay_Base::Overlay_Base( Microsoft::WRL::ComPtr<IDWriteTextFormat> Font )
	:
	m_Font(Font)
{
}


Overlay_Base::~Overlay_Base()
{
}
