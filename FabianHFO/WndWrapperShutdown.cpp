/**********************************************************************************************//**
 * @file	WndWrapperShutdown.cpp.
 *
 * Implements the window wrapper shutdown class.
 **************************************************************************************************/

#include "WndWrapperShutdown.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CWndWrapperShutdown class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperShutdown::CWndWrapperShutdown(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndWrapperShutdown class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperShutdown::~CWndWrapperShutdown(void)
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

bool CWndWrapperShutdown::showView(bool bRedraw)
{
	bool bRes=false;

	if(m_vShutdown)
	{
		m_vShutdown.Show();
	}
	else
	{
		bRes=m_vShutdown.CreateView();
		if(bRes)
		{
			pView=&m_vShutdown;
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

BOOL CWndWrapperShutdown::destroyView()
{
	BOOL bRes=FALSE;
	if(m_vShutdown)
		bRes=m_vShutdown.DestroyWindow();
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

int CWndWrapperShutdown::GetDlgCtrlID()
{
	return m_vShutdown.GetDlgCtrlID();
}

