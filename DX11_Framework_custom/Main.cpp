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

#include "System.h"

//DEFAULT MAIN FUNCTION
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PSTR lpCmdLine,
    int nCmdShow)
{
	// TODO: Test comment
    // Create System object
    System* g_pSystem; // efficient to instantiate on heap?
    bool g_result;

    g_pSystem = new System;
    if (!g_pSystem)
    {
        return 0;
    }

    // Initialize and run g_pSystem object
    g_result = g_pSystem->Initialize();
    if (g_result)
    {
        g_pSystem->Run();
    }

    // Shutdown and release g_pSystem object
    g_pSystem->Shutdown();
    delete g_pSystem;
    g_pSystem = 0;

    return 0; // return 0 if successful   
}