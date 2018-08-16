/**********************************************************************************************//**
 * @file	MnuWrapperTrendtype.cpp.
 *
 * Implements the mnu wrapper trendtype class.
 **************************************************************************************************/

#include "MnuWrapperTrendtype.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CMnuWrapperTrendtype class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CMnuWrapperTrendtype::CMnuWrapperTrendtype(void)
{
	m_iCurBtnSelTrend=IDC_BTN_SELTREND_1;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CMnuWrapperTrendtype class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CMnuWrapperTrendtype::~CMnuWrapperTrendtype(void)
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

BOOL CMnuWrapperTrendtype::createMenu(CWnd* pParentWnd, CMVView *parentView)
{
	BOOL bRes=false;

	RECT rcLd={38,49,501,490};

	bRes=m_menuTrendtype.Create(pParentWnd,rcLd,IDC_POPUP_TRENDTYPE,m_iCurBtnSelTrend);
	if(bRes)
	{
		pMenu=&m_menuTrendtype;

		m_menuTrendtype.Init(parentView);
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

BOOL CMnuWrapperTrendtype::destroyView()
{
	BOOL bRes=FALSE;
	if(m_menuTrendtype)
		bRes=m_menuTrendtype.DestroyWindow();
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

int CMnuWrapperTrendtype::GetDlgCtrlID()
{
	return m_menuTrendtype.GetDlgCtrlID();
}

/**********************************************************************************************//**
 * Shows.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CMnuWrapperTrendtype::Show(BOOL bShow)
{
	//m_menuTrendtype.Show(bShow);
}

/**********************************************************************************************//**
 * Pre create initialize.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	bSelTrend	The selected trend.
 **************************************************************************************************/

void CMnuWrapperTrendtype::preCreateInit(UINT bSelTrend)
{
	m_iCurBtnSelTrend=bSelTrend;
}

/**********************************************************************************************//**
 * Shows the trend type.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	bShow	  	True to show, false to hide.
 * @param	iTrendType	Type of the trend.
 * @param	bSelTrend 	The selected trend.
 **************************************************************************************************/

void CMnuWrapperTrendtype::ShowTrendType(bool bShow, UINT iTrendType, UINT bSelTrend)
{
	m_menuTrendtype.Show(bShow,iTrendType,bSelTrend);
}