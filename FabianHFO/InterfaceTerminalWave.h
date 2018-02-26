//=============================================================================
/** 
* \class CInterfaceTerminalWave
* 		 
* \brief Interface for wave terminal interface.
* 		 
* \author Rainer Kuehner
* \date 2017-10-23 Creation of file
*
**/
//=============================================================================
#pragma once
#include "InterfaceTerminal.h"

// CInterfaceTerminalWave

class CInterfaceTerminalWave : public CInterfaceTerminal
{
	DECLARE_DYNAMIC(CInterfaceTerminalWave)

public:
	CInterfaceTerminalWave();
	virtual ~CInterfaceTerminalWave();

	//virtual abstract
	bool Init();
	bool OpenCOM();
	void OnEvent (EEvent eEvent, EError eError);

protected:
	//DECLARE_MESSAGE_MAP()
};

