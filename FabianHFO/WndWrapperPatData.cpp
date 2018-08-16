/**********************************************************************************************//**
 * @file	WndWrapperPatData.cpp.
 *
 * Implements the window wrapper pattern data class.
 **************************************************************************************************/

#include "WndWrapperPatData.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CWndWrapperPatData class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperPatData::CWndWrapperPatData(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndWrapperPatData class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperPatData::~CWndWrapperPatData(void)
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

bool CWndWrapperPatData::showView(bool bRedraw)
{
	bool bRes=false;

	if(m_vPatData)
	{
		m_vPatData.Show();
	}
	else
	{
		bRes=m_vPatData.CreateView();
		if(bRes)
		{
			pView=&m_vPatData;
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

BOOL CWndWrapperPatData::destroyView()
{
	BOOL bRes=FALSE;
	if(m_vPatData)
		bRes=m_vPatData.DestroyWindow();
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

int CWndWrapperPatData::GetDlgCtrlID()
{
	return m_vPatData.GetDlgCtrlID();
}
