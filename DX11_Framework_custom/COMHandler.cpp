/************************************************************************
Added a COMHandler class, all it does is initialize the COM interface
when the System class is created and UnInitializes it when the System 
class is deleted.
*************************************************************************/

#include "COMHandler.h"
#include <Windows.h>


COMHandler::COMHandler()
{
	CoInitialize( nullptr );
}


COMHandler::~COMHandler()
{
	CoUninitialize();
}