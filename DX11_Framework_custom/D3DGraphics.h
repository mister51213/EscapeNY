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
class Texture;

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
	
	comptr<ID3D11Texture2D> CreateTexture2D( UINT Width, UINT Height, 
		DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM,
		BOOL IsWritable = FALSE,		// Used to set the CPU access flag
		UINT MipLevels = 1,				// 0 for mip-maps, 1 for multi-sample texture
		UINT ArraySize = 1,				// Number of textures in the texture array 
		UINT SampleCount = 1,			
		UINT SampleQuality = 0,
		D3D11_USAGE Usage = D3D11_USAGE_DEFAULT, 
		D3D11_BIND_FLAG BindFlag = D3D11_BIND_SHADER_RESOURCE, 
		UINT MiscFlag = 0,				// D3D11_RESOURCE_MISC_FLAG
		LPVOID pImageData = nullptr );	// Initialize with pixels, nullptr for empty texture
	comptr<ID3D11ShaderResourceView> CreateShaderResourceView( const Texture &Tex );
	
	ID3D11Device* GetDevice()const;
	ID3D11DeviceContext* GetDeviceContext()const;
	ID3D11Texture2D *GetRenderTarget()const;
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

	comptr<IDXGISwapChain> m_pSwapChain;
	comptr<ID3D11Device> m_pDevice;
	comptr<ID3D11DeviceContext> m_pDeviceContext;
	comptr<ID3D11RenderTargetView> m_pRenderTargetView;
	comptr<ID3D11Texture2D> m_pDepthStencilBuffer;
	comptr<ID3D11DepthStencilState> m_pDepthStencilState;
	comptr<ID3D11DepthStencilView> m_pDepthStencilView;
	comptr<ID3D11RasterizerState> m_pRasterState;
};