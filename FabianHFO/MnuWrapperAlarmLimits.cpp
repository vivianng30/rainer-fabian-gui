/**********************************************************************************************//**
 * @file	MnuWrapperAlarmLimits.cpp.
 *
 * Implements the mnu wrapper alarm limits class.
 **************************************************************************************************/

#include "MnuWrapperAlarmLimits.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CMnuWrapperAlarmLimits class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CMnuWrapperAlarmLimits::CMnuWrapperAlarmLimits(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CMnuWrapperAlarmLimits class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CMnuWrapperAlarmLimits::~CMnuWrapperAlarmLimits(void)
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

BOOL CMnuWrapperAlarmLimits::createMenu(CWnd* pParentWnd, CMVView *parentView)
{
	BOOL bRes=false;

	RECT rcLd={0,551,800,600};

	bRes=m_menuAlarmLimits.Create(pParentWnd,rcLd,IDC_VIEW_ALARMLIMITSMENU);
	if(bRes)
	{
		pMenu=&m_menuAlarmLimits;

		m_menuAlarmLimits.Init(parentView);
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

BOOL CMnuWrapperAlarmLimits::destroyView()
{
	BOOL bRes=FALSE;
	if(m_menuAlarmLimits)
		bRes=m_menuAlarmLimits.DestroyWindow();
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

int CMnuWrapperAlarmLimits::GetDlgCtrlID()
{
	return m_menuAlarmLimits.GetDlgCtrlID();
}

/**********************************************************************************************//**
 * Shows.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CMnuWrapperAlarmLimits::Show(BOOL bShow)
{
	m_menuAlarmLimits.Show(bShow);
}
