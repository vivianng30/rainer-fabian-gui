/**********************************************************************************************//**
 * @file	WndWrapperSystemAlarm.cpp.
 *
 * Implements the window wrapper system alarm class.
 **************************************************************************************************/

#include "WndWrapperSystemAlarm.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CWndWrapperSystemAlarm class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperSystemAlarm::CWndWrapperSystemAlarm(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndWrapperSystemAlarm class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperSystemAlarm::~CWndWrapperSystemAlarm(void)
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

bool CWndWrapperSystemAlarm::showView(bool bRedraw)
{
	bool bRes=false;

	if(m_vSystemAlarm)
	{
		m_vSystemAlarm.Show();
	}
	else
	{
		bRes=m_vSystemAlarm.CreateView();
		if(bRes)
		{
			pView=&m_vSystemAlarm;
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

BOOL CWndWrapperSystemAlarm::destroyView()
{
	BOOL bRes=FALSE;
	if(m_vSystemAlarm)
		bRes=m_vSystemAlarm.DestroyWindow();
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

int CWndWrapperSystemAlarm::GetDlgCtrlID()
{
	return m_vSystemAlarm.GetDlgCtrlID();
}