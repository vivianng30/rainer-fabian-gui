/**********************************************************************************************//**
 * @file	WndWrapperDTBField.cpp.
 *
 * Implements the window wrapper dtb field class.
 **************************************************************************************************/

#include "WndWrapperDTBField.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CWndWrapperDTBField class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperDTBField::CWndWrapperDTBField(void)
{
	
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndWrapperDTBField class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperDTBField::~CWndWrapperDTBField(void)
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

bool CWndWrapperDTBField::showView(bool bRedraw)
{
	bool bRes=false;

	if(m_vDTBField)
	{
		m_vDTBField.Show();
	}
	else
	{
		bRes=m_vDTBField.CreateView();
		if(bRes)
		{
			pView=&m_vDTBField;
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

BOOL CWndWrapperDTBField::destroyView()
{
	BOOL bRes=FALSE;
	if(m_vDTBField)
		bRes=m_vDTBField.DestroyWindow();
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

int CWndWrapperDTBField::GetDlgCtrlID()
{
	return m_vDTBField.GetDlgCtrlID();
}

/**********************************************************************************************//**
 * Sets time o 2 flush.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	iO2FlushTime	The o 2 flush time.
 **************************************************************************************************/

void CWndWrapperDTBField::SetTimeO2Flush(int iO2FlushTime)
{
	CStringW sz;
	sz.Format(_T("%s %d %s"),getModel()->GetLanguageString(IDS_TXT_FLUSHTIME), iO2FlushTime, getModel()->GetLanguageString(IDS_UNIT_SECONDS));
	if(m_vDTBField)
		m_vDTBField.SetTimeO2Flush(sz);
}

/**********************************************************************************************//**
 * Sets time until start vent.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	iCountDown	Zero-based index of the count down.
 **************************************************************************************************/

void CWndWrapperDTBField::SetTimeUntilStartVent(int iCountDown)
{
	if(m_vDTBField)
		m_vDTBField.SetTimeUntilStartVent(iCountDown);
}

/**********************************************************************************************//**
 * Sets time alarm silent.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	iCountDown	Zero-based index of the count down.
 **************************************************************************************************/

void CWndWrapperDTBField::SetTimeAlarmSilent(int iCountDown)
{
	if(m_vDTBField)
		m_vDTBField.SetTimeAlarmSilent(iCountDown);
}

/**********************************************************************************************//**
 * Sets time until stop vent.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	iCountDown	Zero-based index of the count down.
 **************************************************************************************************/

void CWndWrapperDTBField::SetTimeUntilStopVent(int iCountDown)
{
	if(m_vDTBField)
		m_vDTBField.SetTimeUntilStopVent(iCountDown);
}

/**********************************************************************************************//**
 * Sets time until turn off.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	iCountDown	Zero-based index of the count down.
 **************************************************************************************************/

void CWndWrapperDTBField::SetTimeUntilTurnOff(int iCountDown)
{
	if(m_vDTBField)
		m_vDTBField.SetTimeUntilTurnOff(iCountDown);
}