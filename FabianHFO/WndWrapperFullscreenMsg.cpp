/**********************************************************************************************//**
 * @file	WndWrapperFullscreenMsg.cpp.
 *
 * Implements the window wrapper fullscreen message class.
 **************************************************************************************************/

#include "WndWrapperFullscreenMsg.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CWndWrapperFullscreenMsg class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperFullscreenMsg::CWndWrapperFullscreenMsg(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndWrapperFullscreenMsg class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperFullscreenMsg::~CWndWrapperFullscreenMsg(void)
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

bool CWndWrapperFullscreenMsg::showView(bool bRedraw)
{
	bool bRes=false;

	if(m_vFullscreenMsg)
	{
		m_vFullscreenMsg.Show();
	}
	else
	{
		bRes=m_vFullscreenMsg.CreateView();
		if(bRes)
		{
			pView=&m_vFullscreenMsg;
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

BOOL CWndWrapperFullscreenMsg::destroyView()
{
	BOOL bRes=FALSE;
	if(m_vFullscreenMsg)
		bRes=m_vFullscreenMsg.DestroyWindow();
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

int CWndWrapperFullscreenMsg::GetDlgCtrlID()
{
	return m_vFullscreenMsg.GetDlgCtrlID();
}