/**********************************************************************************************//**
 * @file	MnuWrapperVentState.cpp.
 *
 * Implements the mnu wrapper vent state class.
 **************************************************************************************************/

#include "MnuWrapperVentState.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CMnuWrapperVentState class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CMnuWrapperVentState::CMnuWrapperVentState(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CMnuWrapperVentState class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CMnuWrapperVentState::~CMnuWrapperVentState(void)
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

BOOL CMnuWrapperVentState::createMenu(CWnd* pParentWnd, CMVView *parentView)
{
	BOOL bRes=false;

	RECT rcLd={0,500,610,600};

	bRes=m_menuVentState.Create(pParentWnd,rcLd,IDC_VIEW_VENTMENU);
	if(bRes)
	{
		pMenu=&m_menuVentState;

		m_menuVentState.Init();
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

BOOL CMnuWrapperVentState::destroyView()
{
	BOOL bRes=FALSE;
	if(m_menuVentState)
		bRes=m_menuVentState.DestroyWindow();
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

int CMnuWrapperVentState::GetDlgCtrlID()
{
	return m_menuVentState.GetDlgCtrlID();
}

/**********************************************************************************************//**
 * Shows.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CMnuWrapperVentState::Show(BOOL bShow)
{
	m_menuVentState.Show(bShow);
}

/**********************************************************************************************//**
 * Resets the button presetstate.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

void CMnuWrapperVentState::ResetButtonPresetstate()
{
	m_menuVentState.ResetButtonPresetstate();
}
