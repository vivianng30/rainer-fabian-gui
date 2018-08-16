/**********************************************************************************************//**
 * @file	WndWrapperService.cpp.
 *
 * Implements the window wrapper service class.
 **************************************************************************************************/

#include "WndWrapperService.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CWndWrapperService class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperService::CWndWrapperService(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndWrapperService class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperService::~CWndWrapperService(void)
{
	destroyView();
}

/**********************************************************************************************//**
 * Shows the view.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	bRedraw	True to redraw.
 *
 * @return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CWndWrapperService::showView(bool bRedraw)
{
	bool bRes=false;

	if(m_vService)
	{
		m_vService.Show(bRedraw);
	}
	else
	{
		bRes=m_vService.CreateView();
		if(bRes)
		{
			pView=&m_vService;
		}
	}

	return bRes;
}

/**********************************************************************************************//**
 * Destroys the view.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CWndWrapperService::destroyView()
{
	BOOL bRes=FALSE;
	if(m_vService)
		bRes=m_vService.DestroyWindow();
	return bRes;
}

/**********************************************************************************************//**
 * Gets dialog control identifier.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @return	The dialog control identifier.
 **************************************************************************************************/

int CWndWrapperService::GetDlgCtrlID()
{
	return m_vService.GetDlgCtrlID();
}
