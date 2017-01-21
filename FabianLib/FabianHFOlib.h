// FabianHFOlib.h : main header file for the FabianHFOlib DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"

// CFabianHFOlibApp
// See FabianHFOlib.cpp for the implementation of this class
//

class CFabianHFOlibApp : public CWinApp
{
public:
	CFabianHFOlibApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

