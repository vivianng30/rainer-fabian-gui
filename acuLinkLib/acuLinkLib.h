// acuLinkLib.h : main header file for the acuLinkLib DLL
//

#pragma once

//#ifdef ACULINKLIB_EXPORTS
//#define ACULINKLIB_API __declspec(dllexport) 
//#else
//#define ACULINKLIB_API __declspec(dllimport) 
//#endif


#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"


#ifndef EXPORT 
#ifdef __cplusplus 
#define EXPORT extern "C" __declspec (dllexport) 
#endif 
#endif 

// CacuLinkLibApp
// See acuLinkLib.cpp for the implementation of this class
//

class CacuLinkLibApp : public CWinApp
{
public:
	CacuLinkLibApp();

	//static ACULINKLIB_API int Add(int a, int b);
	static EXPORT int AddTest(int a, int b);
// Overrides
public:
	virtual BOOL InitInstance();

	
	DECLARE_MESSAGE_MAP()
};

