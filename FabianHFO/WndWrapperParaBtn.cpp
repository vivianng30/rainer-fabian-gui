/**********************************************************************************************//**
 * @file	WndWrapperParaBtn.cpp.
 *
 * Implements the window wrapper para button class.
 **************************************************************************************************/

#include "WndWrapperParaBtn.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CWndWrapperParaBtn class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperParaBtn::CWndWrapperParaBtn(void)
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndWrapperParaBtn class.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 **************************************************************************************************/

CWndWrapperParaBtn::~CWndWrapperParaBtn(void)
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

bool CWndWrapperParaBtn::showView(bool bRedraw)
{
	bool bRes=false;

	if(m_vParaBtn)
	{
		m_vParaBtn.Show();
	}
	else
	{
		bRes=m_vParaBtn.CreateView();
		if(bRes)
		{
			pView=&m_vParaBtn;
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

BOOL CWndWrapperParaBtn::destroyView()
{
	BOOL bRes=FALSE;
	if(m_vParaBtn)
		bRes=m_vParaBtn.DestroyWindow();
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

int CWndWrapperParaBtn::GetDlgCtrlID()
{
	return m_vParaBtn.GetDlgCtrlID();
}

/**********************************************************************************************//**
 * Sets pmean difference.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	iValPmean	Zero-based index of the value pmean.
 * @param	bSend	 	True to send.
 **************************************************************************************************/

void CWndWrapperParaBtn::setPmeanDiff(int iValPmean, bool bSend)
{
	if(m_vParaBtn)
		m_vParaBtn.setPmeanDiff(iValPmean,bSend);
}

/**********************************************************************************************//**
 * Sets pmean record difference.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	iValPmeanRec	Zero-based index of the value pmean record.
 * @param	bSend			True to send.
 **************************************************************************************************/

void CWndWrapperParaBtn::setPmeanRecDiff(int iValPmeanRec, bool bSend)
{
	if(m_vParaBtn)
		m_vParaBtn.setPmeanRecDiff(iValPmeanRec,bSend);
}

/**********************************************************************************************//**
 * Sets cpa pvalue.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	iValCPAP	Zero-based index of the value cpap.
 * @param	bSend   	True to send.
 **************************************************************************************************/

void CWndWrapperParaBtn::setCPAPvalue(int iValCPAP, bool bSend)
{
	/*if(m_vParaBtn)
		m_vParaBtn.setCPAPvalue(iValCPAP,bSend);*/
}

/**********************************************************************************************//**
 * Sets pee pvalue.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	iValPEEP	Zero-based index of the value peep.
 * @param	bSend   	True to send.
 **************************************************************************************************/

void CWndWrapperParaBtn::setPEEPvalue(int iValPEEP, bool bSend)
{
	if(m_vParaBtn)
		m_vParaBtn.setPEEPvalue(iValPEEP,bSend);
}

/**********************************************************************************************//**
 * Sets pman value.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	iValPMAN	Zero-based index of the value pman.
 **************************************************************************************************/

void CWndWrapperParaBtn::setPmanValue(int iValPMAN)
{
	/*if(m_vParaBtn)
		m_vParaBtn.setPmanValue(iValPMAN);*/
}

/**********************************************************************************************//**
 * Sets ppsv value.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	iValPPSV	Zero-based index of the value ppsv.
 * @param	bSend   	True to send.
 **************************************************************************************************/

void CWndWrapperParaBtn::setPpsvValue(int iValPPSV,bool bSend)
{
	if(m_vParaBtn)
		m_vParaBtn.setPpsvValue(iValPPSV, bSend);
}

/**********************************************************************************************//**
 * Sets pinsp value.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	iValPINSP	Zero-based index of the value pinsp.
 **************************************************************************************************/

void CWndWrapperParaBtn::setPinspValue(int iValPINSP)
{
	/*if(m_vParaBtn)
		m_vParaBtn.setPinspValue(iValPINSP);*/
}

/**********************************************************************************************//**
 * Sets pmax value.
 *
 * @author	Rainer Kühner
 * @date	16.08.2018
 *
 * @param	iValPMAX	Zero-based index of the value pmax.
 **************************************************************************************************/

void CWndWrapperParaBtn::setPmaxValue(int iValPMAX)
{
	/*if(m_vParaBtn)
		m_vParaBtn.setPmaxValue(iValPMAX);*/
}