/*******************************************************************************
*  MAIN ARCHITECTURE:                                                          *
*  1) The WinMain function required by Win32 g_pSystem is contained in Main.cpp   *
*  2) The guts of the standard Windows instantiation code (MSG proc, etc),     *
*  are contained in separate "MainWindow.h/.cpp" files.                        *
*  3) The Game logic is contained in Game.h/cpp                                *
*  4) DirectX graphics functionality is contained in Graphics.h/cpp            *
*  5) Main.cpp includes both MainWindow.h and Game.h;                          *
*  Game takes a MainWindow instance in its constructor; this MainWindow        *
*  object will contain the handle to the main window and everything else       *
*  the game will need to initialize DirectX Device and access the main window  *
*  through the graphics class.                                                 *
*                                                                              *
*  This way, the GAME LOGIC, DX GRAPHICS LOGIC, and Win32 WINDOWS LOGIC,       *
*  are all encapsulated.                                                       *
*                                                                              *
********************************************************************************/
#pragma once

#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

///////////////////////////
// Custom class includes //
///////////////////////////
#include "Input.h"
#include "Graphics.h"

class System
{
public:
    System();
    System(const System&);
    ~System();

    bool Initialize();
    void Shutdown();
    void Run();

    // forward declaration
    LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hwnd;

	Input* m_Input;
	Graphics* m_Graphics;
};

///////////////////////////////////////////////////////////////////////
// The WndProc function and ApplicationHandle pointer are also included 
// in this class file so we can re-direct the windows g_pSystem messaging 
// into our MessageHandler function inside the g_pSystem class.
///////////////////////////////////////////////////////////////////////

// FUNCTION PROTOTYPES //
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// GLOBALS //
static System* ApplicationHandle = 0;

#endif