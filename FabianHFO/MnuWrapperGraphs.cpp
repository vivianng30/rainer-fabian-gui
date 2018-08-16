/**********************************************************************************************//**
 * @file	MnuWrapperGraphs.cpp.
 *
 * Implements the mnu wrapper graphs class.
 **************************************************************************************************/

#include "MnuWrapperGraphs.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CMnuWrapperGraphs class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CMnuWrapperGraphs::CMnuWrapperGraphs(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CMnuWrapperGraphs class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CMnuWrapperGraphs::~CMnuWrapperGraphs(void)
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

BOOL CMnuWrapperGraphs::createMenu(CWnd* pParentWnd, CMVView *parentView)
{
	BOOL bRes=false;

	RECT rcLd={0,551,610,600};

	bRes=m_menuGraphs.Create(pParentWnd,rcLd,IDC_VIEW_GRAPHMENU);
	if(bRes)
	{
		pMenu=&m_menuGraphs;

		m_menuGraphs.Init(parentView);
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

BOOL CMnuWrapperGraphs::destroyView()
{
	BOOL bRes=FALSE;
	if(m_menuGraphs)
		bRes=m_menuGraphs.DestroyWindow();
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

int CMnuWrapperGraphs::GetDlgCtrlID()
{
	return m_menuGraphs.GetDlgCtrlID();
}

/**********************************************************************************************//**
 * Shows.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CMnuWrapperGraphs::Show(BOOL bShow)
{
	m_menuGraphs.Show(bShow);
}

/**********************************************************************************************//**
 * Flow sensor state changed.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

void CMnuWrapperGraphs::FlowSensorStateChanged()
{
	m_menuGraphs.FlowSensorStateChanged();
}