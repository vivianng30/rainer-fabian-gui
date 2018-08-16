/**********************************************************************************************//**
 * @file	WndWrapperTrends.cpp.
 *
 * Implements the window wrapper trends class.
 **************************************************************************************************/

#include "WndWrapperTrends.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CWndWrapperTrends class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperTrends::CWndWrapperTrends(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndWrapperTrends class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperTrends::~CWndWrapperTrends(void)
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

bool CWndWrapperTrends::showView(bool bRedraw)
{
	bool bRes=false;

	if(m_vTrend)
	{
		m_vTrend.Show(bRedraw);
	}
	else
	{
		bRes=m_vTrend.CreateView();
		if(bRes)
		{
			pView=&m_vTrend;
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

BOOL CWndWrapperTrends::destroyView()
{
	BOOL bRes=FALSE;
	if(m_vTrend)
		bRes=m_vTrend.DestroyWindow();
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

int CWndWrapperTrends::GetDlgCtrlID()
{
	return m_vTrend.GetDlgCtrlID();
}