/**********************************************************************************************//**
 * @file	WndWrapperAlarmLimits.cpp.
 *
 * Implements the window wrapper alarm limits class.
 **************************************************************************************************/

#include "WndWrapperAlarmLimits.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CWndWrapperAlarmLimits class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperAlarmLimits::CWndWrapperAlarmLimits(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndWrapperAlarmLimits class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperAlarmLimits::~CWndWrapperAlarmLimits(void)
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

bool CWndWrapperAlarmLimits::showView(bool bRedraw)
{
	bool bRes=false;

	if(m_vAlarmLimits)
	{
		m_vAlarmLimits.Show();
	}
	else
	{
		bRes=m_vAlarmLimits.CreateView();
		if(bRes)
		{
			pView=&m_vAlarmLimits;
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

BOOL CWndWrapperAlarmLimits::destroyView()
{
	BOOL bRes=FALSE;
	if(m_vAlarmLimits)
		bRes=m_vAlarmLimits.DestroyWindow();
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

int CWndWrapperAlarmLimits::GetDlgCtrlID()
{
	return m_vAlarmLimits.GetDlgCtrlID();
}

/**********************************************************************************************//**
 * Sets view focus.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

void CWndWrapperAlarmLimits::SetViewFocus()
{
	m_vAlarmLimits.SetViewFocus();
}