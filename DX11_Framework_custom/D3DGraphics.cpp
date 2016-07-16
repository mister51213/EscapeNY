#include "D3DGraphics.h"


D3DGraphics::D3DGraphics()
{
}

D3DGraphics::D3DGraphics(const D3DGraphics& other)
{
}

D3DGraphics::~D3DGraphics()
{
}

bool D3DGraphics::Initialize( UINT ScreenWidth, UINT ScreenHeight, bool VSync, 
	HWND WinHandle, bool Fullscreen)
{
	// Store the vsync setting.
	m_vsync_enabled = VSync;

	// initialize adapter info and get refresh-rate
	DXGI_RATIONAL refreshRate{0,1};

	RETURN_IF_FALSE( initializeAdapterInfo( ScreenWidth, ScreenHeight, refreshRate ) );

	// Create device, device context and swap chain
	RETURN_IF_FALSE( initializeDeviceAndSwapchain( ScreenWidth, ScreenHeight, refreshRate, 
		WinHandle, Fullscreen ) );

	// Create back buffer
	RETURN_IF_FALSE( initializeBackBuffer() );

	// Create depth buffer
	RETURN_IF_FALSE( initializeDepthBuffer( ScreenWidth, ScreenHeight ) );

	// Create and set depth stencil state
	RETURN_IF_FALSE( initializeDepthStencilState() );

	// Create depth stencil view and set render targets
	RETURN_IF_FALSE( initializeDepthStencilView() );

	// Create and set rasterizer state	
	RETURN_IF_FALSE( initializeRasterizer() );

	// Create and set viewport
	RETURN_IF_FALSE( initializeViewport( ScreenWidth, ScreenHeight ) );

	return true;
}

void D3DGraphics::Shutdown()
{
	// Force the swap chain to go into windowed mode first before 
    // releasing any pointers. Otherwise exceptions can be thrown.
	if(m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}
}

//////////////////////
// HELPER FUNCTIONS //
//////////////////////

// Get pointers to the Direct3D device and the Direct3D device context. 
// These helper functions will be called by the framework often.
ID3D11Device* D3DGraphics::GetDevice()
{
	return m_device.Get();
}

ID3D11DeviceContext* D3DGraphics::GetDeviceContext()
{
	return m_deviceContext.Get();
}


void D3DGraphics::GetVideoCardInfo( std::wstring &Description, int& Memory )
{
	Description = m_videoCardDescription;
	Memory = m_videoCardMemory;
	return;
}

bool D3DGraphics::initializeAdapterInfo( UINT ScreenWidth, UINT ScreenHeight, DXGI_RATIONAL &Ratio )
{
	comptr<IDXGIFactory> pFactory;
	comptr<IDXGIAdapter> pAdapter;
	comptr<IDXGIOutput> pAdapterOutput;

	// Create a DirectX graphics interface factory.
	RETURN_IF_FAILED( CreateDXGIFactory( IID_PPV_ARGS( pFactory.GetAddressOf() ) ) );

	// Use the factory to create an adapter for the primary graphics interface (video card).
	RETURN_IF_FAILED( pFactory->EnumAdapters( 0, pAdapter.GetAddressOf() ) );

	// Enumerate the primary adapter output (monitor).
	RETURN_IF_FAILED( pAdapter->EnumOutputs( 0, pAdapterOutput.GetAddressOf() ) );

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM 
	// display format for the adapter output (monitor).
	UINT numModes = 0;
	RETURN_IF_FAILED( pAdapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, NULL ) );

	// Create a list to hold all the possible display modes for this monitor/video 
	// card combination.
	std::vector<DXGI_MODE_DESC> displayModeList( numModes );
	RETURN_IF_FALSE( !displayModeList.empty() );
	
	// Now fill the display mode list structures.
	RETURN_IF_FAILED( pAdapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList.data() ) );

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for( UINT i = 0; i<numModes; i++ )
	{
		if( displayModeList[ i ].Width == ScreenWidth )
		{
			if( displayModeList[ i ].Height == ScreenHeight )
			{
				Ratio.Numerator = displayModeList[ i ].RefreshRate.Numerator;
				Ratio.Denominator = displayModeList[ i ].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	DXGI_ADAPTER_DESC adapterDesc{};
	RETURN_IF_FAILED( pAdapter->GetDesc( &adapterDesc ) );
	
	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)( adapterDesc.DedicatedVideoMemory / 1024 / 1024 );

	// Store the video card description
	m_videoCardDescription = adapterDesc.Description;
	
	return true;
}

bool D3DGraphics::initializeDeviceAndSwapchain( UINT ScreenWidth, UINT ScreenHeight,
	DXGI_RATIONAL RefreshRate, HWND WinHandle, bool FullScreen )
{
	// Set the feature level to DirectX 11.
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Initialize the swap chain description.
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = ScreenWidth;
	swapChainDesc.BufferDesc.Height = ScreenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if( m_vsync_enabled )
	{
		swapChainDesc.BufferDesc.RefreshRate = RefreshRate;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = WinHandle;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	swapChainDesc.Windowed = !FullScreen;

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;


	// Create the swap chain, Direct3D device, and Direct3D device context.
	RETURN_IF_FAILED( D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, m_swapChain.GetAddressOf(), 
		m_device.GetAddressOf(), nullptr, m_deviceContext.GetAddressOf() 
	) );

	return true;
}

bool D3DGraphics::initializeBackBuffer()
{
	comptr<ID3D11Texture2D> pBackBuffer;

	// Get the pointer to the back buffer.
	RETURN_IF_FAILED( m_swapChain->GetBuffer( 0, IID_PPV_ARGS(pBackBuffer.GetAddressOf() ) ) );

	// Create the render target view with the back buffer pointer.
	RETURN_IF_FAILED( m_device->CreateRenderTargetView( pBackBuffer.Get(), NULL,
		m_renderTargetView.GetAddressOf() ) );

	return true;
}

bool D3DGraphics::initializeDepthBuffer( UINT ScreenWidth, UINT ScreenHeight )
{
	// Initialize the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc{};

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = ScreenWidth;
	depthBufferDesc.Height = ScreenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	RETURN_IF_FAILED( m_device->CreateTexture2D( &depthBufferDesc, NULL, m_depthStencilBuffer.GetAddressOf() ) );
	
	return true;
}

bool D3DGraphics::initializeDepthStencilState()
{
	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.	
	RETURN_IF_FAILED( m_device->CreateDepthStencilState( &depthStencilDesc, m_depthStencilState.GetAddressOf() ) );

	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState( m_depthStencilState.Get(), 1 );

	return true;
}

bool D3DGraphics::initializeDepthStencilView()
{
	// Initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	RETURN_IF_FAILED( m_device->CreateDepthStencilView(
		m_depthStencilBuffer.Get(), &depthStencilViewDesc, m_depthStencilView.GetAddressOf() ) );

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets( 1, m_renderTargetView.GetAddressOf(), 
		m_depthStencilView.Get() );

	return true;
}

bool D3DGraphics::initializeRasterizer()
{
	// Setup the raster description which will determine how and what polygons will be drawn.
	D3D11_RASTERIZER_DESC rasterDesc{};
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	RETURN_IF_FAILED( m_device->CreateRasterizerState( &rasterDesc, m_rasterState.GetAddressOf() ) );

	// Now set the rasterizer state.
	m_deviceContext->RSSetState( m_rasterState.Get() );

	return true;
}

bool D3DGraphics::initializeViewport( UINT ScreenWidth, UINT ScreenHeight )
{
	// Setup the viewport for rendering.
	D3D11_VIEWPORT viewport{};
	viewport.Width = static_cast<float>( ScreenWidth );
	viewport.Height = static_cast<float>( ScreenHeight );
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports( 1, &viewport );

	return true;
}

