/**********************************************************************************************//**
 * @file	MnuWrapperTrend.cpp.
 *
 * Implements the mnu wrapper trend class.
 **************************************************************************************************/

#include "MnuWrapperTrend.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CMnuWrapperTrend class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CMnuWrapperTrend::CMnuWrapperTrend(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CMnuWrapperTrend class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CMnuWrapperTrend::~CMnuWrapperTrend(void)
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

BOOL CMnuWrapperTrend::createMenu(CWnd* pParentWnd, CMVView *parentView)
{
	BOOL bRes=false;

	RECT rcLd={0,550,600,600};

	bRes=m_menuTrend.Create(pParentWnd,rcLd,IDC_VIEW_TRENDMENU);
	if(bRes)
	{
		pMenu=&m_menuTrend;

		m_menuTrend.Init(parentView);
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

BOOL CMnuWrapperTrend::destroyView()
{
	BOOL bRes=FALSE;
	if(m_menuTrend)
		bRes=m_menuTrend.DestroyWindow();
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

int CMnuWrapperTrend::GetDlgCtrlID()
{
	return m_menuTrend.GetDlgCtrlID();
}

/**********************************************************************************************//**
 * Shows.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CMnuWrapperTrend::Show(BOOL bShow)
{
	m_menuTrend.Show(bShow);
}