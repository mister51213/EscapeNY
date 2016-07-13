//////////////////////////////////////////////////////////////////////////
// Filename: graphics.cpp
//////////////////////////////////////////////////////////////////////////
#include "Graphics.h"

Graphics::Graphics()
{
	m_pDirect3D = 0;
    // Initialize the camera, model, and color shader objects to null.
 	m_ColorShader = 0;
}

Graphics::Graphics(const Graphics& other)
{
}

Graphics::~Graphics()
{
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	m_pDirect3D = new D3DGraphics;
	if (!m_pDirect3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_pDirect3D->Initialize(screenWidth, screenHeight, 
		VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		HandleResult( L"Cound not initialize Direct3D." );
		return false;
	}

	// Create the color shader object.
	m_ColorShader = new ColorShader;
	if (!m_ColorShader)
	{
		HandleResult( L"Could not allocate memory for ColorShader." );
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_pDirect3D->GetDevice(), hwnd);
	if (!result)
	{
		HandleResult( L"Could not initialize color shader object." );
		return false;
	}

}

void Graphics::Shutdown()
{   
	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_pDirect3D)
	{
		m_pDirect3D->Shutdown();
		delete m_pDirect3D;
		m_pDirect3D = 0;
	}

	return;
}

bool Graphics::Frame()
{
	bool result;

	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Clear the buffers to begin the scene.
    // Parameters represent render background color.
	m_pDirect3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d 
	// objects.
	m_pDirect3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_pDirect3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to
	// prepare them for drawing.
	m_Model->Render(m_pDirect3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_ColorShader->Render(m_pDirect3D->GetDeviceContext(), 
        m_Model->GetIndexCount(), 
        worldMatrix, 
        viewMatrix, 
        projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_pDirect3D->EndScene();

	return true;
}