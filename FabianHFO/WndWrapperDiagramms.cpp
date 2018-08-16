/**********************************************************************************************//**
 * @file	WndWrapperDiagramms.cpp.
 *
 * Implements the window wrapper diagramms class.
 **************************************************************************************************/

#include "WndWrapperDiagramms.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CWndWrapperDiagramms class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperDiagramms::CWndWrapperDiagramms(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndWrapperDiagramms class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperDiagramms::~CWndWrapperDiagramms(void)
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

bool CWndWrapperDiagramms::showView(bool bRedraw)
{
	bool bRes=false;

	if(m_vDiagramm)
	{
		m_vDiagramm.Show();
	}
	else
	{
		bRes=m_vDiagramm.CreateView();
		if(bRes)
		{
			pView=&m_vDiagramm;
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

BOOL CWndWrapperDiagramms::destroyView()
{
	BOOL bRes=FALSE;
	if(m_vDiagramm)
		bRes=m_vDiagramm.DestroyWindow();
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

int CWndWrapperDiagramms::GetDlgCtrlID()
{
	return m_vDiagramm.GetDlgCtrlID();
}

/**********************************************************************************************//**
 * Stops a diagramm.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	bStopFreeze	True to stop freeze.
 **************************************************************************************************/

void CWndWrapperDiagramms::StopDiagramm(bool bStopFreeze)
{
	m_vDiagramm.StopDiagramm(bStopFreeze);
}