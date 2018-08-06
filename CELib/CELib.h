// CELib.h : main header file for the CELib DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CCELibApp
// See CELib.cpp for the implementation of this class
//

class CCELibApp : public CWinApp
{
public:
	CCELibApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

