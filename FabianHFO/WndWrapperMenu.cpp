/**********************************************************************************************//**
 * @file	WndWrapperMenu.cpp.
 *
 * Implements the window wrapper menu class.
 **************************************************************************************************/

#include "WndWrapperMenu.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CWndWrapperMenu class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperMenu::CWndWrapperMenu(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndWrapperMenu class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperMenu::~CWndWrapperMenu(void)
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

bool CWndWrapperMenu::showView(bool bRedraw)
{
	bool bRes=false;

	if(m_vMenu)
	{
		m_vMenu.Show(bRedraw);
	}
	else
	{
		bRes=m_vMenu.CreateView();
		if(bRes)
		{
			pView=&m_vMenu;
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

BOOL CWndWrapperMenu::destroyView()
{
	BOOL bRes=FALSE;
	if(m_vMenu)
		bRes=m_vMenu.DestroyWindow();
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

int CWndWrapperMenu::GetDlgCtrlID()
{
	return m_vMenu.GetDlgCtrlID();
}

/**********************************************************************************************//**
 * Sets view focus.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

void CWndWrapperMenu::SetViewFocus()
{
	m_vMenu.SetViewFocus();
}