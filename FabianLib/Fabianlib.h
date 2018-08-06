// FabianLib.h : main header file for the FabianLib DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"

// CFabianLibApp
// See FabianLib.cpp for the implementation of this class
//

class CFabianLibApp : public CWinApp
{
public:
	CFabianLibApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

