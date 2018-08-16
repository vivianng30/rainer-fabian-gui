/**********************************************************************************************//**
 * @file	WndWrapperAlarmLog.cpp.
 *
 * Implements the window wrapper alarm log class.
 **************************************************************************************************/

#include "WndWrapperAlarmLog.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CWndWrapperAlarmLog class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperAlarmLog::CWndWrapperAlarmLog(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndWrapperAlarmLog class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperAlarmLog::~CWndWrapperAlarmLog(void)
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

bool CWndWrapperAlarmLog::showView(bool bRedraw)
{
	bool bRes=false;

	if(m_vLog)
	{
		m_vLog.Show();
	}
	else
	{
		bRes=m_vLog.CreateView();
		if(bRes)
		{
			pView=&m_vLog;
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

BOOL CWndWrapperAlarmLog::destroyView()
{
	BOOL bRes=FALSE;
	if(m_vLog)
		bRes=m_vLog.DestroyWindow();
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

int CWndWrapperAlarmLog::GetDlgCtrlID()
{
	return m_vLog.GetDlgCtrlID();
}