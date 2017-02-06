// Fabianlib.h : main header file for the Fabianlib DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"

// CFabianlibApp
// See Fabianlib.cpp for the implementation of this class
//

class CFabianlibApp : public CWinApp
{
public:
	CFabianlibApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

