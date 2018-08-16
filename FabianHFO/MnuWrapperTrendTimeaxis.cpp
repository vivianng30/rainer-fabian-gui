/**********************************************************************************************//**
 * @file	MnuWrapperTrendTimeaxis.cpp.
 *
 * Implements the mnu wrapper trend timeaxis class.
 **************************************************************************************************/

#include "MnuWrapperTrendTimeaxis.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CMnuWrapperTrendTimeaxis class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CMnuWrapperTrendTimeaxis::CMnuWrapperTrendTimeaxis(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CMnuWrapperTrendTimeaxis class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CMnuWrapperTrendTimeaxis::~CMnuWrapperTrendTimeaxis(void)
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

BOOL CMnuWrapperTrendTimeaxis::createMenu(CWnd* pParentWnd, CMVView *parentView)
{
	BOOL bRes=false;

	RECT rcLd={5,450,585,492};

	bRes=m_menuTimeaxis.Create(pParentWnd,rcLd,IDC_WND_TRENDAXIS);
	if(bRes)
	{
		pMenu=&m_menuTimeaxis;

		m_menuTimeaxis.Init(parentView);
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

BOOL CMnuWrapperTrendTimeaxis::destroyView()
{
	BOOL bRes=FALSE;
	if(m_menuTimeaxis)
		bRes=m_menuTimeaxis.DestroyWindow();
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

int CMnuWrapperTrendTimeaxis::GetDlgCtrlID()
{
	return m_menuTimeaxis.GetDlgCtrlID();
}

/**********************************************************************************************//**
 * Shows.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CMnuWrapperTrendTimeaxis::Show(BOOL bShow)
{
	m_menuTimeaxis.Show(bShow);
}

/**********************************************************************************************//**
 * Sets time range.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	dtStartTime	The dt start time.
 * @param	dtEndTime  	The dt end time.
 * @param	iOffset	   	Zero-based index of the offset.
 * @param	iTimeSpan  	Zero-based index of the time span.
 **************************************************************************************************/

void CMnuWrapperTrendTimeaxis::SetTimeRange(COleDateTime dtStartTime, COleDateTime dtEndTime,int iOffset, int iTimeSpan)
{
	m_menuTimeaxis.SetTimeRange(dtStartTime, dtEndTime,iOffset, iTimeSpan);
}