////////////////////////////////////////////////////////////////////////////////
// Filename: D3DGraphics.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

/////////////
// LINKING //
/////////////
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

//////////////
// INCLUDES //
//////////////
#include "Includes.h"
#include "Utilities.h"
using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: D3DGraphics
////////////////////////////////////////////////////////////////////////////////
class D3DGraphics
{
	friend class Graphics;
public:
	D3DGraphics();
	D3DGraphics(const D3DGraphics&);
	~D3DGraphics();

	bool Initialize( UINT ScreenWidth, UINT ScreenHeight, bool, HWND, bool);
	void Shutdown();
	
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	void GetVideoCardInfo(std::wstring &Description, int& Memory);

private:
	bool initializeAdapterInfo( UINT ScreenWidth, UINT ScreenHeight, DXGI_RATIONAL &Ratio );
	bool initializeDeviceAndSwapchain( UINT ScreenWidth, UINT ScreenHeight, 
		DXGI_RATIONAL RefreshRate, HWND WinHandle, bool FullScreen);
	bool initializeBackBuffer();
	bool initializeDepthBuffer( UINT ScreenWidth, UINT ScreenHeight );
	bool initializeDepthStencilState();
	bool initializeDepthStencilView();
	bool initializeRasterizer();
	bool initializeViewport( UINT ScreenWidth, UINT ScreenHeight );

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;	
	std::wstring m_videoCardDescription;

	comptr<IDXGISwapChain> m_swapChain;
	comptr<ID3D11Device> m_device;
	comptr<ID3D11DeviceContext> m_deviceContext;
	comptr<ID3D11RenderTargetView> m_renderTargetView;
	comptr<ID3D11Texture2D> m_depthStencilBuffer;
	comptr<ID3D11DepthStencilState> m_depthStencilState;
	comptr<ID3D11DepthStencilView> m_depthStencilView;
	comptr<ID3D11RasterizerState> m_rasterState;
};