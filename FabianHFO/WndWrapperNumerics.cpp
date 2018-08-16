/**********************************************************************************************//**
 * @file	WndWrapperNumerics.cpp.
 *
 * Implements the window wrapper numerics class.
 **************************************************************************************************/

#include "WndWrapperNumerics.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CWndWrapperNumerics class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperNumerics::CWndWrapperNumerics(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndWrapperNumerics class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperNumerics::~CWndWrapperNumerics(void)
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

bool CWndWrapperNumerics::showView(bool bRedraw)
{
	bool bRes=false;

	if(m_vNumericWnd)
	{
		m_vNumericWnd.Show();
	}
	else
	{
		bRes=m_vNumericWnd.CreateView();
		if(bRes)
		{
			pView=&m_vNumericWnd;
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

BOOL CWndWrapperNumerics::destroyView()
{
	BOOL bRes=FALSE;
	if(m_vNumericWnd)
		bRes=m_vNumericWnd.DestroyWindow();
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

int CWndWrapperNumerics::GetDlgCtrlID()
{
	return m_vNumericWnd.GetDlgCtrlID();
}

