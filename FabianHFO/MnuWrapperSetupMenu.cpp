/**********************************************************************************************//**
 * @file	MnuWrapperSetupMenu.cpp.
 *
 * Implements the mnu wrapper setup menu class.
 **************************************************************************************************/

#include "MnuWrapperSetupMenu.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CMnuWrapperSetupMenu class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CMnuWrapperSetupMenu::CMnuWrapperSetupMenu(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CMnuWrapperSetupMenu class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CMnuWrapperSetupMenu::~CMnuWrapperSetupMenu(void)
{
	destroyView();
}

/**********************************************************************************************//**
 * Creates a menu.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param [in,out]	pParentWnd	If non-null, the parent window.
 * @param [in,out]	parentView	If non-null, the parent view.
 *
 * @return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CMnuWrapperSetupMenu::createMenu(CWnd* pParentWnd, CMVView *parentView)
{
	BOOL bRes=false;

	RECT rcLd={0,551,800,600};

	bRes=m_menuSetupMenu.Create(pParentWnd,rcLd,IDC_VIEW_SETUPMENU);
	if(bRes)
	{
		pMenu=&m_menuSetupMenu;

		m_menuSetupMenu.Init(parentView);
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

BOOL CMnuWrapperSetupMenu::destroyView()
{
	BOOL bRes=FALSE;
	if(m_menuSetupMenu)
		bRes=m_menuSetupMenu.DestroyWindow();
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

int CMnuWrapperSetupMenu::GetDlgCtrlID()
{
	return m_menuSetupMenu.GetDlgCtrlID();
}

/**********************************************************************************************//**
 * Shows.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CMnuWrapperSetupMenu::Show(BOOL bShow)
{
	m_menuSetupMenu.Show(bShow);
}

/**********************************************************************************************//**
 * Sets menu text.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	txt	   	The text.
 * @param	bRedraw	True to redraw.
 **************************************************************************************************/

void CMnuWrapperSetupMenu::SetMenuText(CStringW txt, bool bRedraw)
{
	m_menuSetupMenu.SetMenuText(txt, bRedraw);
}