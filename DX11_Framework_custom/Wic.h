#pragma once

#pragma comment(lib, "windowscodecs.lib")

#include "Includes.h"

using Microsoft::WRL::ComPtr;
class Wic
{
public:
	bool Initialize();
	IWICImagingFactory *GetFactory()const;

private:
	ComPtr<IWICImagingFactory> m_pFactory;
};