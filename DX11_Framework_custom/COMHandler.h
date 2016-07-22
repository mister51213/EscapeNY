/************************************************************************
Added a COMHandler class, all it does is initialize the COM interface
when the System class is created and UnInitializes it when the System 
class is deleted.
*************************************************************************/

#pragma once

class COMHandler
{
public:
	COMHandler();
	~COMHandler();
};
