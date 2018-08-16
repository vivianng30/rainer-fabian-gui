/**********************************************************************************************//**
 * @file	WndWrapperStartup.cpp.
 *
 * Implements the window wrapper startup class.
 **************************************************************************************************/

#include "WndWrapperStartup.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CWndWrapperStartup class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperStartup::CWndWrapperStartup(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndWrapperStartup class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperStartup::~CWndWrapperStartup(void)
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

bool CWndWrapperStartup::showView(bool bRedraw)
{
	bool bRes=false;

	if(m_vStartup)
	{
		m_vStartup.Show();
	}
	else
	{
		bRes=m_vStartup.CreateView();
		if(bRes)
		{
			pView=&m_vStartup;
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

BOOL CWndWrapperStartup::destroyView()
{
	BOOL bRes=FALSE;
	if(m_vStartup)
		bRes=m_vStartup.DestroyWindow();
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

int CWndWrapperStartup::GetDlgCtrlID()
{
	return m_vStartup.GetDlgCtrlID();
}
