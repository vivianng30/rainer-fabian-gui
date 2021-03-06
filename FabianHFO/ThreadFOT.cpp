// ThreadFOT.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "FabianLib.h"
#include "ThreadFOT.h"
#include "MVModel.h"
#include "MVViewHandler.h"

#include <cmath>

extern CEvent g_eventFOT;

/**********************************************************************************************//**
 * A macro that defines minimum fotmeasurements
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

#define MINIMUM_FOTMEASUREMENTS 250

PBUFFOTvent* CThreadFOT::m_pbufFOTventilation=NULL;
PBUFFOTcalc* CThreadFOT::m_pbufFOTdisplay=NULL;
CThreadFOT* CThreadFOT::theThreadFOT=0;

/**********************************************************************************************//**
 * Initializes a new instance of the CThreadFOT class
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

CThreadFOT::CThreadFOT()
{
	InitializeCriticalSection(&csFOTthread);
	InitializeCriticalSection(&csFOTvalidData);
	InitializeCriticalSection(&csFOTsequence);
	InitializeCriticalSection(&csFOTstate);
	InitializeCriticalSection(&csFOTcalcBuffer);
	InitializeCriticalSection(&csFOTventBuffer);
	InitializeCriticalSection(&csFOTdate);

	m_eRetryError=RETRY_NONE;
	//m_bTestRetry=true;

	m_pModel = NULL;

	m_bDecreasing=false;

	m_bDoFOTThread=false;
	m_pcwtFOTThread=NULL;
	m_hThreadFOT=INVALID_HANDLE_VALUE;

	m_pbufFOTdisplay = new PBUFFOTcalc[MAXSIZE_FOT_STEPS];
	m_iCountFOTdisplay=0;
	m_pbufFOTventilation = new PBUFFOTvent[MAXSIZE_FOT_BUFFER];
	m_ibufCountFOTventilation=0;
	m_bFOTvalidData=false;
	m_bFOTconvRunning=false;
	m_bFOThfoRunning=false;
	
	m_iFOToriginPEEPPara_IPPV=0;
	m_iFOToriginPMAXVGPara_IPPV=0;
	m_iFOToriginPINSPPara_IPPV=0;
	m_iFOToriginPpsvPara=0;
	m_iFOToriginDiffPEEP_PINSP=0;
	m_iFOToriginDiffPEEP_Ppsv=0;
	m_iFOToriginDiffPEEP_PMAXVG=0;
	m_iFOToriginPIPlowPEEPAlimitDiff=0;

	m_iFOToriginVGstate=false;

	/*m_eFOToriginHFIERatioPara=RIE_1_1;
	m_iFOToriginHFFreqPara=0;
	m_iFOToriginHFAMPLPara=0;*/

	m_eFOToriginHFIERatioPara=getModel()->getDATAHANDLER()->PARADATA()->GetIERatioParaHFO();
	m_iFOToriginHFFreqPara=getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara();
	m_iFOToriginHFAMPLPara=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();

	m_iFOTPEEPStep=0;
	m_icurFOTPIPMAXVG=0;
	m_icurFOTPIP=0;
	m_icurFOTPSV=0;
	m_icurFOTPEEP=0;
	m_iFOTHFPmeanStep=0;
	m_icurFOTHFPmean=0;
	m_iFOTsequence=0;
	m_iFOTdisplaySequence=0;

	m_iCountFOTimer=0;

	feFOTstate=FOT_OFF;

	m_szLastCalculation=_T(" -- ");
}

/**********************************************************************************************//**
 * Finalizes an instance of the CThreadFOT class
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

CThreadFOT::~CThreadFOT()
{
	stopFOTThread();

	if(m_pcwtFOTThread!=NULL)
	{
		delete m_pcwtFOTThread;
		m_pcwtFOTThread=NULL;
		if(m_hThreadFOT!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadFOT);
			m_hThreadFOT=INVALID_HANDLE_VALUE;
		}
	}

	EnterCriticalSection(&csFOTcalcBuffer);
	m_iCountFOTdisplay=0;
	if (m_pbufFOTdisplay != NULL) {
		delete [] m_pbufFOTdisplay;
	}
	m_pbufFOTdisplay=NULL;
	LeaveCriticalSection(&csFOTcalcBuffer);
	

	EnterCriticalSection(&csFOTventBuffer);
	m_ibufCountFOTventilation=0;
	if (m_pbufFOTventilation != NULL) {
		delete [] m_pbufFOTventilation;
	}
	m_pbufFOTventilation=NULL;
	LeaveCriticalSection(&csFOTventBuffer);

	DeleteCriticalSection(&csFOTstate);
	DeleteCriticalSection(&csFOTsequence);
	DeleteCriticalSection(&csFOTvalidData);
	DeleteCriticalSection(&csFOTthread);
	DeleteCriticalSection(&csFOTcalcBuffer);
	DeleteCriticalSection(&csFOTventBuffer);
	DeleteCriticalSection(&csFOTdate);
}

/**********************************************************************************************//**
 * Gets the instance
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	Null if it fails, else the instance.
 **************************************************************************************************/

CThreadFOT* CThreadFOT::getInstance()
{
	if(theThreadFOT == 0)
	{
		theThreadFOT = new CThreadFOT;
		theApp.WriteLog(_T("#########CThreadFOT::getInstance()"));
	}
	return theThreadFOT;
}

/**********************************************************************************************//**
 * Destroys the instance
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::destroyInstance()
{
	if(theThreadFOT != NULL)
	{
		delete theThreadFOT;
		theThreadFOT = NULL;
	}
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CThreadFOT::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

//bool CThreadFOT::getFOTstate()
//{
//	return m_bFOTrunning;
//}

/**********************************************************************************************//**
 * Loads hfo tvalues
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::loadHFO_FOTvalues()
{
	DEBUGMSG(TRUE, (TEXT("loadHFO_FOTvalues\r\n")));
	m_eFOToriginHFIERatioPara=getModel()->getDATAHANDLER()->PARADATA()->GetIERatioParaHFO();
	m_iFOToriginHFFreqPara=getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara();
	m_iFOToriginHFAMPLPara=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
}

/**********************************************************************************************//**
 * Gets date last sequence
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	The date last sequence.
 **************************************************************************************************/

CString CThreadFOT::getDateLastSequence()
{
	CString szDate=_T("");

	EnterCriticalSection(&csFOTdate);
	szDate=m_szLastCalculation;
	LeaveCriticalSection(&csFOTdate);	

	return szDate;
}

/**********************************************************************************************//**
 * Sets date last sequence
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::setDateLastSequence()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	COleDateTime dtTimeLast(st);

	CStringW szTimeLastFOT = dtTimeLast.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);

	EnterCriticalSection(&csFOTdate);
	if(false==dtTimeLast.ParseDateTime(szTimeLastFOT,LOCALE_NOUSEROVERRIDE))
	{
		//m_szLast21Cal=_T("- error -");
		m_szLastCalculation=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);
		theApp.WriteLog(_T("***last FOT calc: unknown***"));
	}
	else
	{
		m_szLastCalculation.Format(_T("%02d.%02d.%04d %02d:%02d:%02d"),
			dtTimeLast.GetDay(),
			dtTimeLast.GetMonth(),
			dtTimeLast.GetYear(),
			dtTimeLast.GetHour(),
			dtTimeLast.GetMinute(),
			dtTimeLast.GetSecond());
		CStringW szLastFOTCalc=_T("***last FOTcalc: ");
		szLastFOTCalc+=m_szLastCalculation;
		szLastFOTCalc+=_T("***");
		theApp.WriteLog(szLastFOTCalc);
	}
	LeaveCriticalSection(&csFOTdate);
}

/**********************************************************************************************//**
 * Resets the date last sequence
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::resetDateLastSequence()
{
	EnterCriticalSection(&csFOTdate);
	m_szLastCalculation=_T("--");
	LeaveCriticalSection(&csFOTdate);
}

/**********************************************************************************************//**
 * Starts a fot
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::startFOT()
{
	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		startFOThfo();
	}
	else
	{
		startFOTconventional();
	}
}

/**********************************************************************************************//**
 * Stops a fot
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::stopFOT()
{
	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		stopFOThfo();
	}
	else
	{
		stopFOTconventional();
	}
	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_TXT_STOP_FOT);
}

/**********************************************************************************************//**
 * Starts fo toscillation
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::startFOToscillation()
{
	DEBUGMSG(TRUE, (TEXT("startFOToscillation\r\n")));
	getModel()->getDATAHANDLER()->setFOToscillationState(true);

	//DEBUGMSG(TRUE, (TEXT("CThreadFOT::startFOToscillation()\r\n")));
	if(m_bFOTconvRunning)
	{
		int iRes=getModel()->getDATAHANDLER()->PARADATA()->getByteFOTconv_FREQPara()+getModel()->getDATAHANDLER()->PARADATA()->getByteFOTconv_AMPLITUDEPara();
		getModel()->getSERIAL()->Send_FOTconv_AMP_FREQ(iRes);
	}
	else
	{
		getModel()->getDATAHANDLER()->SetIERatioParadataHFO(RIE_1_1, true);
		getModel()->getDATAHANDLER()->SetHFFreqParadata(getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_FREQPara(), true);
		getModel()->getDATAHANDLER()->SetHFAMPLParadata(getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_AMPLITUDEPara(), true);
	}
}

/**********************************************************************************************//**
 * Change to fot convert vent mode
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::changeToFOTConvVentMode()
{
	DEBUGMSG(TRUE, (TEXT("changeToFOTConvVentMode\r\n")));
	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_CONTROL_START_FOT_CONVENTIONAL);
}

/**********************************************************************************************//**
 * Restore fot convert vent mode
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::restoreFOTConvVentMode()
{
	DEBUGMSG(TRUE, (TEXT("restoreFOTConvVentMode\r\n")));
	//stop FOT oscillation
	getModel()->getDATAHANDLER()->setFOToscillationState(false);
		
	getModel()->getSERIAL()->Send_FOTconv_AMP_FREQ(0);

	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_CONTROL_STOP_FOT_CONVENTIONAL);
}

/**********************************************************************************************//**
 * Restore fothfo vent mode
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::restoreFOTHFOVentMode()
{
	DEBUGMSG(TRUE, (TEXT("restoreFOTHFOVentMode\r\n")));
	getModel()->getDATAHANDLER()->setFOToscillationState(false);

	bool bSend=getModel()->getVMODEHANDLER()->activeModeIsHFO();
	getModel()->getDATAHANDLER()->SetHFFreqParadata(m_iFOToriginHFFreqPara, bSend);
	getModel()->getDATAHANDLER()->SetHFAMPLParadata(m_iFOToriginHFAMPLPara, bSend);
	getModel()->getDATAHANDLER()->SetIERatioParadataHFO(m_eFOToriginHFIERatioPara, bSend);

	if(		getModel()->getALARMHANDLER()->isVgarantAutoTurnedOff_FOT() 
		&&	getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
	{
		if(getModel()->getDATAHANDLER()->PARADATA()->IsVLimitParamOn_IPPV())
		{
			getModel()->getDATAHANDLER()->SetVLimitParamdata_IPPV(getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_IPPV(),false,true);
		}
		if(getModel()->getDATAHANDLER()->PARADATA()->IsVLimitParamOn_TRIGGER())
		{
			getModel()->getDATAHANDLER()->SetVLimitParamdata_TRIGGER(getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_TRIGGER(),false,true);
		}
		getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PARA_AUTOENABLED);

		getModel()->getALARMHANDLER()->resetVgAutoTurnedOff_FOT();
	}
}

/**********************************************************************************************//**
 * Starts fo tconventional
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::startFOTconventional()
{
	DEBUGMSG(TRUE, (TEXT("startFOTconventional\r\n")));
	m_bFOTconvRunning=true;
	CString szLog=_T("");
	theApp.WriteLog(_T("#FOT: start CONVENTIONAL"));
	szLog.Format(_T("FOTsteps %d\r\n"),getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara());
	theApp.WriteLog(szLog);

	szLog.Format(_T("PEEPlow %d\r\n"),getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPSTARTPara());
	theApp.WriteLog(szLog);

	if(getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara()>1)
	{
		szLog.Format(_T("PEEPhigh %d\r\n"),getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPENDPara());
		theApp.WriteLog(szLog);
	}
	szLog.Format(_T("FOTamp %d\r\n"),getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_AMPLITUDEPara());
	theApp.WriteLog(szLog);

	szLog.Format(_T("FOTfreq %d\r\n"),getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_FREQPara());
	theApp.WriteLog(szLog);
	
	m_iFOToriginPEEPPara_IPPV=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_TRIGGER();
	m_iFOToriginPMAXVGPara_IPPV=getModel()->getDATAHANDLER()->PARADATA()->GetPmaxVolGPara_IPPV();
	m_iFOToriginPINSPPara_IPPV=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPPara_IPPV();
	m_iFOToriginPpsvPara=getModel()->getDATAHANDLER()->PARADATA()->GetPpsvPara();

	m_iFOToriginVGstate=getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn();

	m_iFOToriginPIPlowPEEPAlimitDiff=getModel()->getALARMHANDLER()->getAlimitPIPmin()-getModel()->getALARMHANDLER()->getAlimitPEEPmin();

	//DEBUGMSG(TRUE, (TEXT("startFOTconventional getAlimitPIPmin %d\r\n"),getModel()->getALARMHANDLER()->getAlimitPIPmin()));
	//DEBUGMSG(TRUE, (TEXT("startFOTconventional m_iFOToriginPIPlowPEEPAlimitDiff %d\r\n"),m_iFOToriginPIPlowPEEPAlimitDiff));
	m_iFOToriginDiffPEEP_PINSP=m_iFOToriginPINSPPara_IPPV-m_iFOToriginPEEPPara_IPPV;
	m_iFOToriginDiffPEEP_Ppsv=m_iFOToriginPpsvPara-m_iFOToriginPEEPPara_IPPV;
	m_iFOToriginDiffPEEP_PMAXVG=m_iFOToriginPMAXVGPara_IPPV-m_iFOToriginPEEPPara_IPPV;
	
	//reset all data
	m_iCountFOTimer=0;
	resetFOTdisplayBuffer();

	setParaFOTCONV(false);

	startFOTThread();
}

/**********************************************************************************************//**
 * Stops fo tconventional
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::stopFOTconventional()
{
	theApp.WriteLog(_T("#FOT: stop CONVENTIONAL"));

	restoreFOTConvVentMode();

	stopFOTThread();

	m_bFOTconvRunning=false;
}

/**********************************************************************************************//**
 * Starts fo thfo
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::startFOThfo()
{
	DEBUGMSG(TRUE, (TEXT("startFOThfo\r\n")));
	m_bFOThfoRunning=true;
	CString szLog=_T("");
	theApp.WriteLog(_T("#FOT: start HFO"));
	szLog.Format(_T("FOTsteps %d\r\n"),getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara());
	theApp.WriteLog(szLog);

	szLog.Format(_T("PMEANlow %d\r\n"),getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANSTARTPara());
	theApp.WriteLog(szLog);

	if(getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara()>1)
	{
		szLog.Format(_T("PMEANhigh %d\r\n"),getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANENDPara());
		theApp.WriteLog(szLog);
	}

	szLog.Format(_T("FOTamp %d\r\n"),getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_AMPLITUDEPara());
	theApp.WriteLog(szLog);

	szLog.Format(_T("FOTfreq %d\r\n"),getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_FREQPara());
	theApp.WriteLog(szLog);
	 

	m_eFOToriginHFIERatioPara=getModel()->getDATAHANDLER()->PARADATA()->GetIERatioParaHFO();
	m_iFOToriginHFFreqPara=getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara();
	m_iFOToriginHFAMPLPara=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();

	//reset all data
	m_iCountFOTimer=0;
	resetFOTdisplayBuffer();

	setParaFOTHFO(false);
	
	startFOTThread();
}

/**********************************************************************************************//**
 * Gets origin hfampl para
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	The origin hfampl para.
 **************************************************************************************************/

WORD CThreadFOT::getOriginHFAMPLPara()
{
	return m_iFOToriginHFAMPLPara;
}

/**********************************************************************************************//**
 * Stops fo thfo
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::stopFOThfo()
{
	theApp.WriteLog(_T("#FOT: stop HFO"));

	restoreFOTHFOVentMode();

	m_bFOThfoRunning=false;

	stopFOTThread();
}

/**********************************************************************************************//**
 * Query if this instance is fo trunning
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	True if fo trunning, false if not.
 **************************************************************************************************/

bool CThreadFOT::isFOTrunning()
{
	if(m_bFOTconvRunning)
		return true;
	else if(m_bFOThfoRunning)
		return true;
	else
		return false;
}

/**********************************************************************************************//**
 * Calculates the para fotconv
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::calcParaFOTCONV()
{
	DEBUGMSG(TRUE, (TEXT("calcParaFOTCONV\r\n")));
	WORD iPEEPEND=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPENDPara();
	WORD iPEEPSTART=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPSTARTPara();
	WORD iSteps=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara();

	if(iSteps==1)
	{
		m_icurFOTPEEP=iPEEPSTART;
		//DEBUGMSG(TRUE, (TEXT("setFOTCONVpara() m_icurFOTPEEP1 %d\r\n"),m_icurFOTPEEP));
	}
	else
	{
		if((iSteps-1)/2>0)
			m_iFOTPEEPStep=(iPEEPEND-iPEEPSTART)/((iSteps-1)/2);
		else
		{
			//ERROR
			m_iFOTPEEPStep=0;
			theApp.WriteLog(_T("#HFO:0300"));
		}
		m_iFOTPEEPStep=(m_iFOTPEEPStep/5)*5;

		BYTE iFOTsequence=getCurFOTsequence();
		if(iFOTsequence==1)
		{
			m_icurFOTPEEP=iPEEPSTART;
			//DEBUGMSG(TRUE, (TEXT("setFOTCONVpara() m_icurFOTPEEP2 %d\r\n"),m_icurFOTPEEP));
		}
		else if(iFOTsequence==iSteps)
		{
			m_icurFOTPEEP=iPEEPSTART;
			//DEBUGMSG(TRUE, (TEXT("setFOTCONVpara() m_icurFOTPEEP3 %d\r\n"),m_icurFOTPEEP));
		}
		else if(iFOTsequence<=(iSteps+1)/2)
		{
			m_icurFOTPEEP+=m_iFOTPEEPStep;
			//DEBUGMSG(TRUE, (TEXT("setFOTCONVpara() m_icurFOTPEEP4 %d\r\n"),m_icurFOTPEEP));
		}
		else
		{
			m_icurFOTPEEP-=m_iFOTPEEPStep;
			//DEBUGMSG(TRUE, (TEXT("setFOTCONVpara() m_icurFOTPEEP5 %d\r\n"),m_icurFOTPEEP));
		}
	}

	m_icurFOTPSV=m_icurFOTPEEP+m_iFOToriginDiffPEEP_Ppsv;
	//DEBUGMSG(TRUE, (TEXT("setFOTCONVpara m_icurFOTPSV %d \r\n"),m_icurFOTPSV));
	if(m_icurFOTPEEP%10!=0 && m_iFOToriginDiffPEEP_PINSP%10!=0)
	{
		m_icurFOTPIPMAXVG=m_icurFOTPEEP+m_iFOToriginDiffPEEP_PMAXVG;
		m_icurFOTPIP=m_icurFOTPEEP+m_iFOToriginDiffPEEP_PINSP;
		//DEBUGMSG(TRUE, (TEXT("setFOTCONVpara m_icurFOTPIP2 %d \r\n"),m_icurFOTPIP));
	}
	else if(m_iFOToriginDiffPEEP_PINSP%10!=0)
	{
		m_icurFOTPIPMAXVG=m_icurFOTPEEP+5+m_iFOToriginDiffPEEP_PMAXVG;
		m_icurFOTPIP=m_icurFOTPEEP+5+m_iFOToriginDiffPEEP_PINSP;
		//DEBUGMSG(TRUE, (TEXT("setFOTCONVpara m_icurFOTPIP3 %d \r\n"),m_icurFOTPIP));
	}
	else if(m_icurFOTPEEP%10!=0)
	{
		m_icurFOTPIPMAXVG=m_icurFOTPEEP+5+m_iFOToriginDiffPEEP_PMAXVG;
		m_icurFOTPIP=m_icurFOTPEEP+5+m_iFOToriginDiffPEEP_PINSP;
		//DEBUGMSG(TRUE, (TEXT("setFOTCONVpara m_icurFOTPIP4 %d \r\n"),m_icurFOTPIP));
	}
	else
	{
		m_icurFOTPIPMAXVG=m_icurFOTPEEP+m_iFOToriginDiffPEEP_PMAXVG;
		m_icurFOTPIP=m_icurFOTPEEP+m_iFOToriginDiffPEEP_PINSP;
		//DEBUGMSG(TRUE, (TEXT("setFOTCONVpara m_icurFOTPIP5 %d \r\n"),m_icurFOTPIP));
	}
	//limit PIPmax to 80 mbar
	if(m_icurFOTPIPMAXVG>800)
		m_icurFOTPIPMAXVG=800;
	if(m_icurFOTPIP>800)
		m_icurFOTPIP=800;
	DEBUGMSG(TRUE, (TEXT("calcParaFOTCONV() m_iFOTsequence%d, m_iFOTdisplaySequence%d, m_icurFOTPEEP%d, m_iFOTPEEPStep%d, m_icurFOTPSV%d, m_icurFOTPIPMAXVG%d, m_icurFOTPIP%d\r\n"),m_iFOTsequence,m_iFOTdisplaySequence,m_icurFOTPEEP,m_iFOTPEEPStep,m_icurFOTPSV,m_icurFOTPIPMAXVG,m_icurFOTPIP));

}

/**********************************************************************************************//**
 * Sets decrease para fotconv
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param	bIncreaseSeq	True to increase sequence.
 **************************************************************************************************/

void CThreadFOT::setDecreaseParaFOTCONV(bool bIncreaseSeq)
{
	DEBUGMSG(TRUE, (TEXT("CThreadFOT::setDecreaseParaFOTCONV()\r\n")));

	//resetFOTventdataBuffer();

	if(bIncreaseSeq)
		increaseFOTsequence();
	calcDecreaseParaFOTCONV();

	getModel()->getDATAHANDLER()->SetPmaxVolGParadata_TRIGGER(m_icurFOTPIPMAXVG, true);
	getModel()->getDATAHANDLER()->SetPINSPParadata_IPPV(m_icurFOTPIP, true);
	getModel()->getDATAHANDLER()->SetPpsvParadata(m_icurFOTPSV, true);
	getModel()->getDATAHANDLER()->SetPEEPParadata_IPPV(m_icurFOTPEEP, true);

	if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
	{
		getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();

		fALVALUE fv;
		fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangePIPmin();
		fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmin();
		//DEBUGMSG(TRUE, (TEXT("getAlimitPEEPmin %d\r\n"),getModel()->getALARMHANDLER()->getAlimitPEEPmin()));
		fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitPEEPmin()+m_iFOToriginPIPlowPEEPAlimitDiff;

		if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
			getModel()->getALARMHANDLER()->setAlimitPIPmin(fv.iCurrentLimit);
		}
		else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
			getModel()->getALARMHANDLER()->setAlimitPIPmin(fv.iCurrentLimit);
		}
		getModel()->getALARMHANDLER()->setAlimitPIPmin(fv.iCurrentLimit);
		//DEBUGMSG(TRUE, (TEXT("setAlimitPIPmin %d\r\n"),fv.iCurrentLimit));
	}

	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_FOT);
}

/**********************************************************************************************//**
 * Calculates the decrease para fotconv
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::calcDecreaseParaFOTCONV()
{
	DEBUGMSG(TRUE, (TEXT("calcDecreaseParaFOTCONV()\r\n")));
	WORD iPEEPEND=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPENDPara();
	WORD iPEEPSTART=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPSTARTPara();
	WORD iSteps=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara();

	if(iSteps<=3)//should not occur
	{
		m_bDecreasing=true;
		m_icurFOTPEEP=iPEEPSTART;
		//DEBUGMSG(TRUE, (TEXT("setFOTCONVpara() m_icurFOTPEEP1 %d\r\n"),m_icurFOTPEEP));
	}
	else
	{
		if((iSteps-1)/2>0)
			m_iFOTPEEPStep=(iPEEPEND-iPEEPSTART)/((iSteps-1)/2);
		else
		{
			//ERROR
			m_iFOTPEEPStep=0;
			theApp.WriteLog(_T("#HFO:0300"));
		}
		m_iFOTPEEPStep=(m_iFOTPEEPStep/5)*5;

		BYTE iprevFOTsequence=getCurFOTsequence();

		
		EnterCriticalSection(&csFOTsequence);
		if(m_iFOTsequence<3)
		{
			m_iFOTsequence=iSteps;
		}
		else
		{
			m_iFOTsequence=iSteps-m_iFOTsequence+3;
		}
		
		if(m_iFOTsequence==iSteps)
		{
			m_icurFOTPEEP=iPEEPSTART;
		}
		else
		{
			//m_icurFOTPEEP=iPEEPSTART+(iprevFOTsequence*m_iFOTPEEPStep);
			m_icurFOTPEEP=m_icurFOTPEEP-m_iFOTPEEPStep;
		}
		LeaveCriticalSection(&csFOTsequence);
		
		//DEBUGMSG(TRUE, (TEXT("calcDecreaseParaFOTCONV() m_icurFOTPEEP%d iFOTsequence%d prevSequ %d\r\n"),m_icurFOTPEEP, m_iFOTsequence, iprevFOTsequence));
		
		m_bDecreasing=true;
	}

	m_icurFOTPSV=m_icurFOTPEEP+m_iFOToriginDiffPEEP_Ppsv;
	//DEBUGMSG(TRUE, (TEXT("setFOTCONVpara m_icurFOTPSV %d \r\n"),m_icurFOTPSV));
	if(m_icurFOTPEEP%10!=0 && m_iFOToriginDiffPEEP_PINSP%10!=0)
	{
		m_icurFOTPIPMAXVG=m_icurFOTPEEP+m_iFOToriginDiffPEEP_PMAXVG;
		m_icurFOTPIP=m_icurFOTPEEP+m_iFOToriginDiffPEEP_PINSP;
		//DEBUGMSG(TRUE, (TEXT("setFOTCONVpara m_icurFOTPIP2 %d \r\n"),m_icurFOTPIP));
	}
	else if(m_iFOToriginDiffPEEP_PINSP%10!=0)
	{
		m_icurFOTPIPMAXVG=m_icurFOTPEEP+5+m_iFOToriginDiffPEEP_PMAXVG;
		m_icurFOTPIP=m_icurFOTPEEP+5+m_iFOToriginDiffPEEP_PINSP;
		//DEBUGMSG(TRUE, (TEXT("setFOTCONVpara m_icurFOTPIP3 %d \r\n"),m_icurFOTPIP));
	}
	else if(m_icurFOTPEEP%10!=0)
	{
		m_icurFOTPIPMAXVG=m_icurFOTPEEP+5+m_iFOToriginDiffPEEP_PMAXVG;
		m_icurFOTPIP=m_icurFOTPEEP+5+m_iFOToriginDiffPEEP_PINSP;
		//DEBUGMSG(TRUE, (TEXT("setFOTCONVpara m_icurFOTPIP4 %d \r\n"),m_icurFOTPIP));
	}
	else
	{
		m_icurFOTPIPMAXVG=m_icurFOTPEEP+m_iFOToriginDiffPEEP_PMAXVG;
		m_icurFOTPIP=m_icurFOTPEEP+m_iFOToriginDiffPEEP_PINSP;
		//DEBUGMSG(TRUE, (TEXT("setFOTCONVpara m_icurFOTPIP5 %d \r\n"),m_icurFOTPIP));
	}
	//DEBUGMSG(TRUE, (TEXT("calcDecreaseParaFOTCONV() m_iFOTsequence%d, m_iFOTdisplaySequence%d, m_icurFOTPEEP%d, m_iFOTPEEPStep%d, m_icurFOTPSV%d, m_icurFOTPIPMAXVG%d, m_icurFOTPIP%d\r\n"),m_iFOTsequence,m_iFOTdisplaySequence,m_icurFOTPEEP,m_iFOTPEEPStep,m_icurFOTPSV,m_icurFOTPIPMAXVG,m_icurFOTPIP));

}

/**********************************************************************************************//**
 * Sets para fotconv
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param	bRetry	True to retry.
 **************************************************************************************************/

void CThreadFOT::setParaFOTCONV(bool bRetry)
{
	DEBUGMSG(TRUE, (TEXT("setParaFOTCONV()\r\n")));
	resetFOTventdataBuffer();

	if(false==bRetry)
	{
		increaseFOTsequence();

		calcParaFOTCONV();

		getModel()->getDATAHANDLER()->SetPmaxVolGParadata_TRIGGER(m_icurFOTPIPMAXVG, true);
		getModel()->getDATAHANDLER()->SetPINSPParadata_IPPV(m_icurFOTPIP, true);
		getModel()->getDATAHANDLER()->SetPpsvParadata(m_icurFOTPSV, true);
		getModel()->getDATAHANDLER()->SetPEEPParadata_IPPV(m_icurFOTPEEP, true);
		
		if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
		{
			getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();

			fALVALUE fv;
			fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangePIPmin();
			fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmin();
			//DEBUGMSG(TRUE, (TEXT("getAlimitPEEPmin %d\r\n"),getModel()->getALARMHANDLER()->getAlimitPEEPmin()));
			fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitPEEPmin()+m_iFOToriginPIPlowPEEPAlimitDiff;

			if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
			{
				fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
				getModel()->getALARMHANDLER()->setAlimitPIPmin(fv.iCurrentLimit);
			}
			else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
			{
				fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
				getModel()->getALARMHANDLER()->setAlimitPIPmin(fv.iCurrentLimit);
			}
			getModel()->getALARMHANDLER()->setAlimitPIPmin(fv.iCurrentLimit);
			//DEBUGMSG(TRUE, (TEXT("setAlimitPIPmin %d\r\n"),fv.iCurrentLimit));
		}
		
	}
	else
	{
		//DEBUGMSG(TRUE, (TEXT("CThreadFOT::setParaFOTCONV() RETRY\r\n")));
	}

	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_FOT);
}

/**********************************************************************************************//**
 * Calculates the decrease para fothfo
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::calcDecreaseParaFOTHFO()
{
	DEBUGMSG(TRUE, (TEXT("calcDecreaseParaFOTHFO\r\n")));
	
	WORD iPmeanEND=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANENDPara();
	WORD iPmeanSTART=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANSTARTPara();
	WORD iSteps=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara();

	if(iSteps<=3)//should not occur
	{
		m_bDecreasing=true;
		m_icurFOTHFPmean=iPmeanSTART;
	}
	else
	{
		if((iSteps-1)/2>0)
			m_iFOTHFPmeanStep=(iPmeanEND-iPmeanSTART)/((iSteps-1)/2);
		else
		{
			//ERROR
			m_iFOTHFPmeanStep=0;
			theApp.WriteLog(_T("#HFO:0301"));
		}
		m_iFOTHFPmeanStep=(m_iFOTHFPmeanStep/5)*5;

		BYTE iprevFOTsequence=getCurFOTsequence();

		EnterCriticalSection(&csFOTsequence);
		if(m_iFOTsequence<3)
		{
			m_iFOTsequence=iSteps;
		}
		else
		{
			m_iFOTsequence=iSteps-m_iFOTsequence+3;
		}

		if(m_iFOTsequence==iSteps)
		{
			m_icurFOTHFPmean=iPmeanSTART;
		}
		else
		{
			m_icurFOTHFPmean=m_icurFOTHFPmean-m_iFOTHFPmeanStep;
		}
		LeaveCriticalSection(&csFOTsequence);


		m_bDecreasing=true;

	}
}

/**********************************************************************************************//**
 * Calculates the para fothfo
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::calcParaFOTHFO()
{
	DEBUGMSG(TRUE, (TEXT("calcParaFOTHFO\r\n")));
	WORD iPmeanEND=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANENDPara();
	WORD iPmeanSTART=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANSTARTPara();
	WORD iSteps=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara();

	if(iSteps==1)
	{
		m_icurFOTHFPmean=iPmeanSTART;
	}
	else
	{
		if((iSteps-1)/2>0)
			m_iFOTHFPmeanStep=(iPmeanEND-iPmeanSTART)/((iSteps-1)/2);
		else
		{
			//ERROR
			m_iFOTHFPmeanStep=0;
			theApp.WriteLog(_T("#HFO:0301"));
		}
		m_iFOTHFPmeanStep=(m_iFOTHFPmeanStep/5)*5;

		BYTE iFOTsequence=getCurFOTsequence();
		if(iFOTsequence==1)
		{
			m_icurFOTHFPmean=iPmeanSTART;
		}
		else if(iFOTsequence==iSteps)
		{
			m_icurFOTHFPmean=iPmeanSTART;
		}
		else if(iFOTsequence<=(iSteps+1)/2)
		{
			m_icurFOTHFPmean+=m_iFOTHFPmeanStep;
		}
		else
		{
			m_icurFOTHFPmean-=m_iFOTHFPmeanStep;
		}
	}
}

/**********************************************************************************************//**
 * Sets decrease para fothfo
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param	bIncreaseSeq	True to increase sequence.
 **************************************************************************************************/

void CThreadFOT::setDecreaseParaFOTHFO(bool bIncreaseSeq)
{
	DEBUGMSG(TRUE, (TEXT("setDecreaseParaFOTHFO\r\n")));
	resetFOTventdataBuffer();

	if(bIncreaseSeq)
		increaseFOTsequence();
	calcDecreaseParaFOTHFO();

	getModel()->getDATAHANDLER()->SetHFPMeanParadata(m_icurFOTHFPmean, true);


	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_FOT);
}

/**********************************************************************************************//**
 * Sets para fothfo
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param	bRetry	True to retry.
 **************************************************************************************************/

void CThreadFOT::setParaFOTHFO(bool bRetry)
{
	DEBUGMSG(TRUE, (TEXT("setParaFOTHFO\r\n")));
	resetFOTventdataBuffer();

	if(false==bRetry)
	{
		increaseFOTsequence();

		calcParaFOTHFO();
		
		getModel()->getDATAHANDLER()->SetHFPMeanParadata(m_icurFOTHFPmean, true);
	}
	/*else
	{
		DEBUGMSG(TRUE, (TEXT("CThreadFOT::setParaFOTHFO() RETRY\r\n")));
	}*/


	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_FOT);
}

/**********************************************************************************************//**
 * Gets current fotpeep
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	The current fotpeep.
 **************************************************************************************************/

WORD CThreadFOT::getCurrentFOTPEEP()
{
	return m_icurFOTPEEP;
}

/**********************************************************************************************//**
 * Gets current fothf pmean
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	The current fothf pmean.
 **************************************************************************************************/

WORD CThreadFOT::getCurrentFOTHFPmean()
{
	return m_icurFOTHFPmean;
}

/**********************************************************************************************//**
 * Gets fothf pmean step
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	The fothf pmean step.
 **************************************************************************************************/

WORD CThreadFOT::getFOTHFPmeanStep()
{
	return m_iFOTHFPmeanStep;
}

/**********************************************************************************************//**
 * Gets fotpeep step
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	The fotpeep step.
 **************************************************************************************************/

WORD CThreadFOT::getFOTPEEPStep()
{
	return m_iFOTPEEPStep;
}

/**********************************************************************************************//**
 * Gets fo torigin pinsp para ippv
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	The fo torigin pinsp para ippv.
 **************************************************************************************************/

SHORT CThreadFOT::getFOToriginPINSPPara_IPPV()
{
	return m_iFOToriginPINSPPara_IPPV;
}

/**********************************************************************************************//**
 * Gets fo torigin pmaxvg para ippv
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	The fo torigin pmaxvg para ippv.
 **************************************************************************************************/

SHORT CThreadFOT::getFOToriginPMAXVGPara_IPPV()
{
	return m_iFOToriginPMAXVGPara_IPPV;
}

/**********************************************************************************************//**
 * Query if this instance is fo torigin v gstate on
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	True if fo torigin v gstate on, false if not.
 **************************************************************************************************/

bool CThreadFOT::isFOToriginVGstateOn()
{
	return m_iFOToriginVGstate;
}

/**********************************************************************************************//**
 * Gets fo torigin difference peep pinsp
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	The fo torigin difference peep pinsp.
 **************************************************************************************************/

SHORT CThreadFOT::getFOToriginDiffPEEP_PINSP()
{
	return m_iFOToriginDiffPEEP_PINSP;
}

/**********************************************************************************************//**
 * Gets fo torigin difference peep pmaxvg
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	The fo torigin difference peep pmaxvg.
 **************************************************************************************************/

SHORT CThreadFOT::getFOToriginDiffPEEP_PMAXVG()
{
	return m_iFOToriginDiffPEEP_PMAXVG;
}

/**********************************************************************************************//**
 * Starts a calculation
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::startCalculation()
{
	DEBUGMSG(TRUE, (TEXT("CThreadFOT::startCalculation()\r\n")));
	if(m_bFOTconvRunning)
	{
		restoreFOTConvVentMode();
		calculateFOTdata(getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_FREQPara(),getCurrentFOTPEEP());
	}
	else
	{
		restoreFOTHFOVentMode();
		calculateFOTdata(getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_FREQPara(),getCurrentFOTHFPmean());
	}
}

/**********************************************************************************************//**
 * Executes the step operation
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::runStep()
{
	setFOTstate(FOT_STARTSTEP);
	//if(getFOTstate()==FOT_VENTDELAY)
	//{
	//	if(m_bFOTconvRunning)
	//	{
	//		//wait for CPAP
	//		setFOTstate(FOT_WAITCPAP);
	//		changeToFOTConvVentMode();
	//	}
	//	else
	//	{
	//		setFOTstate(FOT_WAITVALIDDATA);
	//		startFOToscillation();
	//	}
	//	m_iCountFOTimer=0;
	//}
	
}

/**********************************************************************************************//**
 * Continue sequence
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::continueSequence()	
{
	DEBUGMSG(TRUE, (TEXT("continueSequence\r\n")));
	if(getFOTstate()==FOT_WAITCONTINUE)
	{
		//DEBUGMSG(TRUE, (TEXT("CThreadFOT::continueWithSequence()\r\n")));
		if(m_bFOTconvRunning)
		{
			setParaFOTCONV(false);
		}
		else
		{
			setParaFOTHFO(false);
		}
		setFOTstate(FOT_VENTDELAY);
		m_iCountFOTimer=getModel()->getCONFIG()->getFOTventDelaytime();

		//DEBUGMSG(TRUE, (TEXT("CThreadFOT g_eventFOT.SetEvent()\r\n")));
		g_eventFOT.SetEvent();
	}
	//else//ERROR
	//{
	//	DEBUGMSG(TRUE, (TEXT("ERROR CThreadFOT::continueWithSequence() %d\r\n"),(int)getFOTstate()));
	//}
}

/**********************************************************************************************//**
 * Decrease sequence
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::decreaseSequence()
{
	if(getFOTstate()==FOT_WAITCONTINUE)
	{
		//DEBUGMSG(TRUE, (TEXT("CThreadFOT::continueWithSequence()\r\n")));
		if(m_bFOTconvRunning)
		{
			setDecreaseParaFOTCONV(true);
		}
		else
		{
			setDecreaseParaFOTHFO(true);
		}
		setFOTstate(FOT_VENTDELAY);
		m_iCountFOTimer=getModel()->getCONFIG()->getFOTventDelaytime();

		//DEBUGMSG(TRUE, (TEXT("CThreadFOT g_eventFOT.SetEvent()\r\n")));
		g_eventFOT.SetEvent();
	}
	else if(getFOTstate()==FOT_VENTDELAY)
	{
		//DEBUGMSG(TRUE, (TEXT("CThreadFOT::continueWithSequence()\r\n")));
		if(m_bFOTconvRunning)
		{
			setDecreaseParaFOTCONV(false);
		}
		else
		{
			setDecreaseParaFOTHFO(false);
		}
		//m_iCountFOTimer=1;
		m_iCountFOTimer=getModel()->getCONFIG()->getFOTventDelaytime();

		//DEBUGMSG(TRUE, (TEXT("CThreadFOT g_eventFOT.SetEvent()\r\n")));
		g_eventFOT.SetEvent();
	}

}

/**********************************************************************************************//**
 * Repeat sequence
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::repeatSequence()
{
	resetRetryERROR();

	if(getFOTstate()==FOT_RETRY)
	{
		//DEBUGMSG(TRUE, (TEXT("CThreadFOT::continueWithSequence() RETRY\r\n")));
		if(m_bFOTconvRunning)
		{
			setParaFOTCONV(true);
		}
		else
		{
			setParaFOTHFO(true);
		}
		setFOTstate(FOT_STARTSTEP);
		//setFOTstate(FOT_VENTDELAY);
		m_iCountFOTimer=getModel()->getCONFIG()->getFOTventDelaytime();

		//DEBUGMSG(TRUE, (TEXT("CThreadFOT g_eventFOT.SetEvent()\r\n")));
		g_eventFOT.SetEvent();
	}
	else//ERROR
	{
		
		//DEBUGMSG(TRUE, (TEXT("CThreadFOT::repeatSequence() %d\r\n"),(int)getFOTstate()));
		setFOTstate(FOT_RETRY);
		
		if(m_bFOTconvRunning)
		{
			restoreFOTConvVentMode();
		}
		else
		{
			restoreFOTHFOVentMode();
		}

		resetLastDisplayBuf();

		setFOTstate(FOT_STARTSTEP);
		//setFOTstate(FOT_VENTDELAY);
		m_iCountFOTimer=getModel()->getCONFIG()->getFOTventDelaytime();

		//DEBUGMSG(TRUE, (TEXT("CThreadFOT g_eventFOT.SetEvent()\r\n")));
		g_eventFOT.SetEvent();

		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_GRAPH_REDRAW);
	}
	
}

/**********************************************************************************************//**
 * Resets the last display buffer
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::resetLastDisplayBuf()
{
	EnterCriticalSection(&csFOTcalcBuffer);
	if(m_iCountFOTdisplay>0)
		m_iCountFOTdisplay--;
	LeaveCriticalSection(&csFOTcalcBuffer);
}

/**********************************************************************************************//**
 * Query if this instance is decreasing sequence
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	True if decreasing sequence, false if not.
 **************************************************************************************************/

bool CThreadFOT::isDecreasingSequence()
{
	return m_bDecreasing;
}

/**********************************************************************************************//**
 * Query if this instance is next sequence
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	True if next sequence, false if not.
 **************************************************************************************************/

bool CThreadFOT::isNextSequence()
{
	bool bResult=true;

	if(m_bFOTconvRunning)
	{
		if(getCurFOTsequence()>=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara())
		{
			bResult=false;
		}
	}
	else
	{
		if(getCurFOTsequence()>=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara())
		{
			bResult=false;
		}
	}
	return bResult;
}

/**********************************************************************************************//**
 * Gets current fo tsequence
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	The current fo tsequence.
 **************************************************************************************************/

BYTE CThreadFOT::getCurFOTsequence()
{
	BYTE seq=0;
	EnterCriticalSection(&csFOTsequence);
	seq=m_iFOTsequence;
	LeaveCriticalSection(&csFOTsequence);
	return seq;
}

/**********************************************************************************************//**
 * Increase fo tsequence
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::increaseFOTsequence()
{
	DEBUGMSG(TRUE, (TEXT("increaseFOTsequence %d\r\n"),m_iFOTsequence));

	EnterCriticalSection(&csFOTsequence);
	m_iFOTsequence++;
	LeaveCriticalSection(&csFOTsequence);

	m_iFOTdisplaySequence++;
	}

/**********************************************************************************************//**
 * Resets the fo tsequence
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::resetFOTsequence()
{
	EnterCriticalSection(&csFOTsequence);
	m_iFOTsequence=0;
	LeaveCriticalSection(&csFOTsequence);
}

/**********************************************************************************************//**
 * Query if this instance is acquire fot data
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	True if acquire fot data, false if not.
 **************************************************************************************************/

bool CThreadFOT::isAcquireFOTData()
{
	if(getFOTstate()==FOT_ACQUIREDATA)
		return true;
	else
		return false;
}

//void CThreadFOT::setFOTsequenceRunning(bool state)
//{
//	EnterCriticalSection(&csFOTsequence);
//	m_bFOTsequenceRunning=state;
//	LeaveCriticalSection(&csFOTsequence);
//}
//bool CThreadFOT::isFOTsequenceRunning()
//{
//	bool bTemp=false;
//	EnterCriticalSection(&csFOTsequence);
//	bTemp=m_bFOTsequenceRunning;
//	LeaveCriticalSection(&csFOTsequence);
//	return bTemp;
//}

/**********************************************************************************************//**
 * Sets a retry
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param	error	The error.
 **************************************************************************************************/

void CThreadFOT::setRetry(eRetryError error)
{
	EnterCriticalSection(&csFOTvalidData);
	m_eRetryError=error;
	LeaveCriticalSection(&csFOTvalidData);
	
	setFOTstate(FOT_RETRY);
}

/**********************************************************************************************//**
 * Gets retry error
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	The retry error.
 **************************************************************************************************/

eRetryError CThreadFOT::getRetryERROR()
{
	eRetryError error=RETRY_NONE;
	EnterCriticalSection(&csFOTvalidData);
	error=m_eRetryError;
	LeaveCriticalSection(&csFOTvalidData);
	return error;
}

/**********************************************************************************************//**
 * Resets the retry error
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::resetRetryERROR()
{
	EnterCriticalSection(&csFOTvalidData);
	m_eRetryError=RETRY_NONE;
	LeaveCriticalSection(&csFOTvalidData);
}

/**********************************************************************************************//**
 * Sets fo tstate
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param	feState	State of the fe.
 **************************************************************************************************/

void CThreadFOT::setFOTstate(SequenceStatesFOT feState)
{
	DEBUGMSG(TRUE, (TEXT("CThreadFOT::setFOTstate() %d\r\n"),(int)feState));
	
	EnterCriticalSection(&csFOTstate);
	feFOTstate=feState;
	LeaveCriticalSection(&csFOTstate);

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_REDRAW_FOT_STATE);
}

/**********************************************************************************************//**
 * Gets fo tstate
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	The fo tstate.
 **************************************************************************************************/

SequenceStatesFOT CThreadFOT::getFOTstate()
{
	SequenceStatesFOT feState=FOT_OFF;
	EnterCriticalSection(&csFOTstate);
	feState=feFOTstate;
	LeaveCriticalSection(&csFOTstate);
	return feState;
}

/**********************************************************************************************//**
 * Query if this instance is fot data valid
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	True if the fot data is valid, false if not.
 **************************************************************************************************/

bool CThreadFOT::isFOTDataValid()
{
	bool bTemp=false;
	EnterCriticalSection(&csFOTvalidData);
	bTemp=m_bFOTvalidData;
	LeaveCriticalSection(&csFOTvalidData);
	return bTemp;
}

/**********************************************************************************************//**
 * Sets fot data valid
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param	validData	True to valid data.
 * \param	error	 	The error.
 **************************************************************************************************/

void CThreadFOT::setFOTDataValid(bool validData, eRetryError error)
{
	EnterCriticalSection(&csFOTvalidData);
	m_bFOTvalidData=validData;
	m_eRetryError=error;
	LeaveCriticalSection(&csFOTvalidData);
}

/**********************************************************************************************//**
 * Check fo tvalid measurement data
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::checkFOTvalidMeasurementData()
{
	//DEBUGMSG(TRUE, (TEXT("checkFOTvalidMeasurementData\r\n")));

	if(m_bFOThfoRunning)
	{
		SHORT iPMEANmeasurement=getModel()->getDATAHANDLER()->getAVGMessureDataPmitt();
		
		if(m_icurFOTHFPmean-10<=iPMEANmeasurement && iPMEANmeasurement<=m_icurFOTHFPmean+10)
		{
			setFOTDataValid(true);
		}
		else
		{
			setFOTDataValid(false,RETRY_PRESSURE);
		}

		//DEBUGMSG(TRUE, (TEXT("checkFOTvalidMeasurementData cur %d meas %d\r\n"),m_icurFOTHFPmean,iPMEANmeasurement));
	}
	else
	{
		SHORT iPEEPmeasurement=getModel()->getDATAHANDLER()->getAVGMessureDataPmitt();
		
		if(m_icurFOTPEEP-10<=iPEEPmeasurement && iPEEPmeasurement<=m_icurFOTPEEP+10)
		{
			setFOTDataValid(true);
		}
		else
		{
			setFOTDataValid(false,RETRY_PRESSURE);
		}

		//DEBUGMSG(TRUE, (TEXT("checkFOTvalidMeasurementData cur %d meas %d\r\n"),m_icurFOTPEEP,iPEEPmeasurement));
	}

	if(isFOTDataValid())//check leak
	{
		BYTE iLEAKmeasurement=getModel()->getDATAHANDLER()->getAVGMessureDataLeak();
		if(iLEAKmeasurement<20)
		{
			setFOTDataValid(true);
		}
		else
		{
			setFOTDataValid(false,RETRY_LEAK);
		}
	}
}

/**********************************************************************************************//**
 * Writes a fo tventdata buffer
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param	iPRESSURE	Zero-based index of the pressure.
 * \param	iFlow	 	Zero-based index of the flow.
 **************************************************************************************************/

void CThreadFOT::writeFOTventdataBuffer(int iPRESSURE, double iFlow)
{
	EnterCriticalSection(&csFOTventBuffer);
	if(m_ibufCountFOTventilation<MAXSIZE_FOT_BUFFER)
	{
		//DEBUGMSG(TRUE, (TEXT("writeFOTventdataBuffer m_ibufCountFOTventilation %d\r\n"),m_ibufCountFOTventilation));
		m_pbufFOTventilation[m_ibufCountFOTventilation].iValPressure=iPRESSURE;
		m_pbufFOTventilation[m_ibufCountFOTventilation].iValFlow=iFlow;
		m_ibufCountFOTventilation++;

		/*CStringW sz=_T("");
		sz.Format(_T("flow %.2f, "), iFlow);
		DEBUGMSG(TRUE, (sz));
		DEBUGMSG(TRUE, (TEXT("m_ibufCountFOTventilation %d\r\n"),m_ibufCountFOTventilation));*/
	}
	//DEBUGMSG(TRUE, (TEXT("m_ibufCountFOTventilation %d\r\n"),m_ibufCountFOTventilation));
	LeaveCriticalSection(&csFOTventBuffer);
}

/**********************************************************************************************//**
 * Resets the fo tventdata buffer
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::resetFOTventdataBuffer()
{
	//DEBUGMSG(TRUE, (TEXT("CThreadFOT::resetFOTventdataBuffer()\r\n")));
	EnterCriticalSection(&csFOTventBuffer);
	if(m_ibufCountFOTventilation>0)
	{
		m_ibufCountFOTventilation=0;

		for(int i=0;i<MAXSIZE_FOT_BUFFER;i++)
		{
			m_pbufFOTventilation[i].iValPressure=0;
			m_pbufFOTventilation[i].iValFlow=0;
		}
	}
	LeaveCriticalSection(&csFOTventBuffer);
}

/**********************************************************************************************//**
 * Resets the fo tdisplay buffer
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::resetFOTdisplayBuffer()
{
	DEBUGMSG(TRUE, (TEXT("CThreadFOT::resetFOTdisplayBuffer() m_iCountFOTdisplay\r\n")));
	EnterCriticalSection(&csFOTcalcBuffer);
	m_iCountFOTdisplay=0;

	for(int i=0;i<MAXSIZE_FOT_STEPS;i++)
	{
		m_pbufFOTdisplay[i].iXValPmean=0;
		m_pbufFOTdisplay[i].iYValXRS=0;
	}
	LeaveCriticalSection(&csFOTcalcBuffer);

	m_iFOTdisplaySequence=0;
	resetDateLastSequence();
}

/**********************************************************************************************//**
 * Gets buffer size fo tdisplay
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	The buffer size fo tdisplay.
 **************************************************************************************************/

BYTE CThreadFOT::getBufSizeFOTdisplay()
{
	BYTE buf=0;
	EnterCriticalSection(&csFOTcalcBuffer);
	buf=m_iCountFOTdisplay;
	LeaveCriticalSection(&csFOTcalcBuffer);
	return buf;
}

/**********************************************************************************************//**
 * Calculates the fo tdata
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param	i_osc_freq 	Zero-based index of the osc frequency.
 * \param	curPressure	The current pressure.
 **************************************************************************************************/

void CThreadFOT::calculateFOTdata(int i_osc_freq,WORD curPressure)
{
	DEBUGMSG(TRUE, (TEXT("calculateFOTdata\r\n")));
	EnterCriticalSection(&csFOTventBuffer);
	if(m_ibufCountFOTventilation==0)
	{
		LeaveCriticalSection(&csFOTventBuffer);
		return;
	}
	LeaveCriticalSection(&csFOTventBuffer);

	//start algorythm
	//const int i_osc_freq = 10; // FOT oscillation frequency -> read in from setting registry
	//const int i_osc_freq = i_oscFreq;
	const int i_sample_freq = 1000/FOT_SAMPLE_DATA; // sample frequency

	//int size = (int) std::round(i_sample_freq/i_osc_freq); // round - from cmath.h
	int size = (int)CTlsFloat::Round((double)(i_sample_freq/i_osc_freq),0);


	// Step 1) Construct Fourier - only once on initialisation (i_osc_freq set,i_sample_freq set)
	//-----------------------------------------------------------------------------
	double** pp_Fourier;
	pp_Fourier = new double*[size];
	for(int i=0;i<size;i++)
		pp_Fourier[i] = new double[3];

	fnConstructFourier(i_osc_freq,i_sample_freq,pp_Fourier);

	double** pp_PseudoInverse;
	pp_PseudoInverse = new double*[3];
	for(int i=0;i<3;i++)
		pp_PseudoInverse[i] = new double[size];

	// Step 2) Construct PseudoInverse  - only once on initialisation (i_osc_freq set,i_sample_freq set)
	//-----------------------------------------------------------------------------
	fnConstructPseudoInverse(i_osc_freq,i_sample_freq,pp_Fourier,pp_PseudoInverse);

	// Step 3) Calculate Resistance, Reactance, FSI, PSI.
	//-----------------------------------------------------------------------------

	double** pp_Flow;
	pp_Flow = new double*[size];
	for(int i=0;i<size;i++)
		pp_Flow[i] = new double[1];

	double** pp_Pressure;
	pp_Pressure = new double*[size];
	for(int i=0;i<size;i++)
		pp_Pressure[i] = new double[1];

	double* p_Out;
	p_Out = new double[4];
	for(int i=0;i<4;i++)
		p_Out[i] = 0;


	double* p_TotalOut;
	p_TotalOut = new double[4];
	for(int i=0;i<4;i++)
		p_TotalOut[i] = 0;

	//DWORD dwStart=GetTickCount();
	
	CStringW szLog=_T("");
	/*bool bRetryFSI=false;
	bool bRetryResistance=false;
	bool bRetryReactance=false;*/
	bool bValueAccpeted=true;

	int iTestCountbufCountFOT=0;
	int iCountAccepted=0;

	CStringW sz=_T("");

	EnterCriticalSection(&csFOTventBuffer);

	for(int k=0;k<(m_ibufCountFOTventilation-size);k++)
	{ 
		for(int i=0;i<size;i++)
		{
			pp_Flow[i][0] = (double) m_pbufFOTventilation[i+k].iValFlow;
			pp_Pressure[i][0] = (double) m_pbufFOTventilation[i+k].iValPressure;
			/*sz.Format(_T("flow %.2f, "), m_pbufFOTventilation[i+k].iValFlow);
			DEBUGMSG(TRUE, (sz));*/
		}

		if(pp_Flow!=0)
		{
			fnCalcZ(pp_Fourier,pp_PseudoInverse,pp_Flow,pp_Pressure,i_osc_freq,i_sample_freq,p_Out);

			if(p_Out[0]<=0.0)//check Resistance >0, else not valid
			{
				bValueAccpeted=false;
			}
			else if(p_Out[1]>100.0 || p_Out[1]<-500.0)//check Reactance/XRS 100>XRS>-500 cmH2O*s/L, else not valid
			{
				bValueAccpeted=false;
			}
			else if(p_Out[2]>0.20)//check FSI <= 20%, else not valid
			{
				bValueAccpeted=false;
			}
		}
		else
			bValueAccpeted=false;

		Sleep(0);

		iTestCountbufCountFOT++;
		if(bValueAccpeted)
		{
			iCountAccepted++;
			for(int n=0;n<4;n++)
			{
				/*sz.Format(_T("Y1 %d %.2f\r\n"),n,p_Out[n]);
				DEBUGMSG(TRUE, (sz));*/
				p_TotalOut[n] += p_Out[n]; 
				//sz.Format(_T("Y2 %d %.2f\r\n"),n,p_TotalOut[n]);
				//DEBUGMSG(TRUE, (sz));
			}
		}
		
		bValueAccpeted=true;
	}

	/*pOut[0] = Resistance;
	pOut[1] = Reactance;
	pOut[2] = FSI;
	pOut[3] = PSI;*/

	for(int i=0;i<4;i++)
	{
		/*sz.Format(_T("X1 %d %.2f\r\n"),i,p_TotalOut[i]);
		DEBUGMSG(TRUE, (sz));*/
		//p_TotalOut[i] = p_TotalOut[i]/(m_ibufCountFOTventilation-size);
		p_TotalOut[i] = p_TotalOut[i]/iCountAccepted;
		/*sz.Format(_T("X2 %d %.2f\r\n"),i,p_TotalOut[i]);
		DEBUGMSG(TRUE, (sz));*/
	}
	LeaveCriticalSection(&csFOTventBuffer);


	EnterCriticalSection(&csFOTcalcBuffer);
	m_pbufFOTdisplay[m_iFOTdisplaySequence-1].iXValPmean=curPressure;
	m_pbufFOTdisplay[m_iFOTdisplaySequence-1].iYValXRS=p_TotalOut[1];
	m_iCountFOTdisplay++;
	LeaveCriticalSection(&csFOTcalcBuffer);

	/*sz.Format(_T("m_pbufFOTdisplay pressure %d iYValXRS %.2f, m_iCountFOTdisplay %d m_iFOTdisplaySequence %d\r\n"),curPressure,m_pbufFOTdisplay[m_iFOTdisplaySequence-1].iYValXRS, m_iCountFOTdisplay, m_iFOTdisplaySequence);
	DEBUGMSG(TRUE, (sz));*/

	szLog.Format(_T("#FOTcalc: pressure %d XRS%d %.2f Resistance %.2f"),curPressure, m_iFOTdisplaySequence,m_pbufFOTdisplay[m_iFOTdisplaySequence-1].iYValXRS,p_TotalOut[0]);
	//DEBUGMSG(TRUE, (szLog));
	theApp.WriteLog(szLog);
	
	//DWORD dwEnd=GetTickCount();
	//DEBUGMSG(TRUE, (TEXT("calculateFOTCONVdata END %d\r\n"),dwEnd-dwStart));

	// clean up
	for(int i=0;i<size;i++)
		delete [] pp_Flow[i];
	delete [] pp_Flow ;

	for(int i=0;i<size;i++)
		delete [] pp_Pressure[i];

	delete [] pp_Pressure;
	delete [] p_Out;
	delete [] p_TotalOut;

	
	//if(bRetryFSI || bRetryResistance || bRetryReactance)
	if(iCountAccepted<MINIMUM_FOTMEASUREMENTS)
	{
		//if(bRetryFSI)
		//	setRetry(RETRY_FSI);
		//else if(bRetryResistance)
		//	setRetry(RETRY_RESISTANCE);
		//else//bRetryReactance
		//	setRetry(RETRY_REACTANCE);

		setRetry(RETRY_QUALITY);

		resetFOTventdataBuffer();
		resetLastDisplayBuf();

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_REDRAW_FOT_STATE);
	}
	else
	{
		resetFOTventdataBuffer();
		getModel()->getVIEWHANDLER()->drawFOTsteps();
	}
}

/**********************************************************************************************//**
 * Starts fot thread
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::startFOTThread( void )
{
	startThread();

	if(m_pcwtFOTThread!=NULL)
	{
		delete m_pcwtFOTThread;
		m_pcwtFOTThread=NULL;

		if(m_hThreadFOT!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadFOT);
			m_hThreadFOT=INVALID_HANDLE_VALUE;
		}

	}

	m_pcwtFOTThread=AfxBeginThread(FOTThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadFOT=m_pcwtFOTThread->m_hThread;
	m_pcwtFOTThread->m_bAutoDelete = FALSE; 
	m_pcwtFOTThread->ResumeThread();
}

/**********************************************************************************************//**
 * Stops fot thread
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::stopFOTThread( void )
{
	if(doThread())
	{
		stopThread();
		g_eventFOT.SetEvent();
		//DEBUGMSG(TRUE, (TEXT("CThreadFOT g_eventFOT.SetEvent()\r\n")));

		if (WaitForSingleObject(m_pcwtFOTThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			if(!TerminateThread(m_pcwtFOTThread,0))
			{
				theApp.WriteLog(_T("#THR:FOT"));
				/*int err = GetLastError();
				CStringW temp;
				temp.Format(L"TerminateThread error ConnectionThread [%d]",err);*/
			}
		}
	}
}

/**********************************************************************************************//**
 * Executes the thread operation
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CThreadFOT::doThread()
{
	bool bRes=false;
	EnterCriticalSection(&csFOTthread);
	bRes=m_bDoFOTThread;
	LeaveCriticalSection(&csFOTthread);
	return bRes;
}

/**********************************************************************************************//**
 * Starts a thread
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::startThread()
{
	EnterCriticalSection(&csFOTthread);
	m_bDoFOTThread=true;
	LeaveCriticalSection(&csFOTthread);
}

/**********************************************************************************************//**
 * Stops a thread
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 **************************************************************************************************/

void CThreadFOT::stopThread()
{
	EnterCriticalSection(&csFOTthread);
	m_bDoFOTThread=false;
	LeaveCriticalSection(&csFOTthread);
}

/**********************************************************************************************//**
 * Fot thread
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

static UINT FOTThread( LPVOID pc )
{
	try
	{
		((CThreadFOT*)pc)->FOTData();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("FOTThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("FOTThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CThreadFOT*)pc)->FOTData();
	return TRUE;
}

/**********************************************************************************************//**
 * Fot data
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CThreadFOT::FOTData(void) 
{
	CeSetThreadPriority(m_pcwtFOTThread->m_hThread,256);

	bool bValidMeasurementData=false;

	getModel()->getVIEWHANDLER()->setFOTrunning(true);
	
	setFOTstate(FOT_VENTDELAY);
	m_iCountFOTimer=getModel()->getCONFIG()->getFOTventDelaytime();
	
	SequenceStatesFOT feTempFOTstate=FOT_VENTDELAY;
	
	//send to view, with text "wait for valid data"-> state chnaged
	getModel()->getVIEWHANDLER()->drawFOTtime(m_iCountFOTimer);

	m_bDecreasing=false;

	do
	{
		feTempFOTstate=getFOTstate();

		if(		feTempFOTstate!=FOT_OFF
			&&	feTempFOTstate!=FOT_RETRY
			&&	feTempFOTstate!=FOT_WAITCONTINUE)//run a FOT sequence
		{
			if(::WaitForSingleObject(g_eventFOT, 1000))
			{
				if(doThread())
				{
					switch (feTempFOTstate)
					{
					case FOT_VENTDELAY://delay 3min
						
						m_iCountFOTimer--;
						getModel()->getVIEWHANDLER()->drawFOTtime(m_iCountFOTimer);

						if(m_iCountFOTimer<=0)
						{
							if(m_bFOTconvRunning)
							{
								//wait for CPAP
								setFOTstate(FOT_WAITCPAP);
								changeToFOTConvVentMode();
							}
							else
							{
								setFOTstate(FOT_WAITVALIDDATA);
								startFOToscillation();
							}
							m_iCountFOTimer=0;
						}
						break;
					case FOT_STARTSTEP:
						if(m_bFOTconvRunning)
						{
							//wait for CPAP
							setFOTstate(FOT_WAITCPAP);
							changeToFOTConvVentMode();
						}
						else
						{
							setFOTstate(FOT_WAITVALIDDATA);
							startFOToscillation();
						}
						m_iCountFOTimer=0;
						break;
					case FOT_WAITCPAP:

						if(m_bFOTconvRunning)
						{
							//wait for CPAP
							if(getModel()->getVMODEHANDLER()->activeModeIsCPAP())
							{
								setFOTstate(FOT_WAITVALIDDATA);
								startFOToscillation();
							}
							m_iCountFOTimer=0;//FOT_TIMEOUT
						}
						break;
					case FOT_WAITVALIDDATA:
						
						checkFOTvalidMeasurementData();

						m_iCountFOTimer++;//FOT_TIMEOUT

						if(true==isFOTDataValid())
						{
							//all ok, start acquire data
							setFOTstate(FOT_ACQUIREDATA);

							m_iCountFOTimer=3;
							getModel()->getVIEWHANDLER()->drawFOTtime(m_iCountFOTimer);
						}
						else if(m_iCountFOTimer>15 && false==isFOTDataValid())
						{
							//setFOTstate(FOT_RETRY);
							setRetry(RETRY_PRESSURE);

							m_iCountFOTimer=0;
							getModel()->getVIEWHANDLER()->drawFOTtime(m_iCountFOTimer);
							
							if(m_bFOTconvRunning)
							{
								restoreFOTConvVentMode();
							}
							else
							{
								restoreFOTHFOVentMode();
							}
						}
						else //if(m_iCountFOTimer<=0)
						{
							getModel()->getVIEWHANDLER()->drawFOTtime(m_iCountFOTimer);
						}
						break;
					case FOT_ACQUIREDATA:

						checkFOTvalidMeasurementData();

						m_iCountFOTimer--;

						if(m_iCountFOTimer>0 && true==isFOTDataValid())
						{
							//all ok, acquire data
							getModel()->getVIEWHANDLER()->drawFOTtime(m_iCountFOTimer);
						}
						else if(m_iCountFOTimer>0 && false==isFOTDataValid())
						{
							//setFOTstate(FOT_RETRY);
							setRetry(RETRY_PRESSURE);
							m_iCountFOTimer=getModel()->getCONFIG()->getFOTventDelaytime();
							getModel()->getVIEWHANDLER()->drawFOTtime(m_iCountFOTimer);

							if(m_bFOTconvRunning)
							{
								restoreFOTConvVentMode();
							}
							else
							{
								restoreFOTHFOVentMode();
							}
						}
						else //if(m_iCountFOTimer<=0)
						{
							setFOTDataValid(false);//stop SPI data for FOT

							if(isNextSequence())
							{
								setFOTstate(FOT_WAITCONTINUE);
								m_iCountFOTimer=getModel()->getCONFIG()->getFOTventDelaytime();
								getModel()->getVIEWHANDLER()->drawFOTtime(m_iCountFOTimer);

								startCalculation();
							}
							else
							{
								startCalculation();

								if(FOT_RETRY!=getFOTstate())
								{
									setFOTstate(FOT_OFF);
									m_iCountFOTimer=getModel()->getCONFIG()->getFOTventDelaytime();
									getModel()->getVIEWHANDLER()->drawFOTtime(m_iCountFOTimer);

									setDateLastSequence();

									if(m_bFOTconvRunning)
									{
										restoreFOTConvVentMode();

										m_bFOTconvRunning=false;
										stopThread();
									}
									else
									{
										restoreFOTHFOVentMode();

										m_bFOThfoRunning=false;
										stopThread();
									}
								}
								/*else
								{
									DEBUGMSG(TRUE, (TEXT("xxxxxxxxxxx FOT_RETRY\r\n")));
								}*/
							}
						}
						break;
					case FOT_RETRY:
						//DEBUGMSG(TRUE, (TEXT("FOT_RETRY\r\n")));

						break;
					case FOT_WAITCONTINUE:
						//DEBUGMSG(TRUE, (TEXT("FOT_WAITCONTINUE\r\n")));

						break;
					default:
						//DEBUGMSG(TRUE, (TEXT("default\r\n")));
						break;
					} // switch (feRecvState)
					//DEBUGMSG(TRUE, (TEXT("END switch (feTempFOTstate)\r\n")));
				}
			}
		}
		else //wait for continue
		{
			//info an view etc...
			//
			if (::WaitForSingleObject(g_eventFOT, INFINITE) == WAIT_OBJECT_0)
			{
				//DEBUGMSG(TRUE, (TEXT("WaitForSingleObject(g_eventFOT, INFINITE)\r\n")));
				if(doThread())
				{

				}	
			}
		}
		
	}while(doThread());

	getModel()->getDATAHANDLER()->setFOToscillationState(false);
	
	setFOTstate(FOT_OFF);
	resetFOTsequence();
	resetFOTventdataBuffer();
	setFOTDataValid(false);

	getModel()->getVIEWHANDLER()->setFOTrunning(false);

	theApp.WriteLog(_T("#THR:FOT end"));
	//DEBUGMSG(TRUE, (TEXT("CThreadFOT::FOTData end\r\n")));
	

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_SETFOCUS_PIMARYVIEW);

	return 0;
}

/**********************************************************************************************//**
 * fnTranspose function: matrix "pp_A" (format is pointer of pointer [pp]) with rows "n_rows_A"
 * and cols "n_cols_A", transposed into matrix "pp_A_Transpose".
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param [in,out]	pp_A		  	If non-null, a.
 * \param 		  	n_rows_A	  	The rows a.
 * \param 		  	n_cols_A	  	The cols a.
 * \param [in,out]	pp_A_Transpose	If non-null, a transpose.
 **************************************************************************************************/

void CThreadFOT::fnTranspose(double **pp_A,int n_rows_A,int n_cols_A,double **pp_A_Transpose)
{
	for (int i = 0; i < n_rows_A; i++)
		for (int j = 0; j < n_cols_A; j++)
			pp_A_Transpose[j][i] = pp_A[i][j];
}

/**********************************************************************************************//**
 * fnMultiplication function: matrix multiplication of matrix "pp_A" with a matrix "pp_B".
 * Result is matrix "pp_C".
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param [in,out]	pp_A		If non-null, a.
 * \param 		  	n_rows_A	The rows a.
 * \param 		  	n_cols_A	The cols a.
 * \param [in,out]	pp_B		If non-null, the b.
 * \param 		  	n_cols_B	The cols b.
 * \param [in,out]	pp_C		If non-null, the c.
 **************************************************************************************************/

void CThreadFOT::fnMultiplication(double **pp_A,int n_rows_A,int n_cols_A, double **pp_B, int n_cols_B, double **pp_C)
{
	/*for (int i = 0; i < n_rows_A; i++)
		for (int j = 0; j < n_cols_B; j++)
			for(int k=0; pp_C[i][j] = 0; k< n_cols_A; k++)
				pp_C[i][j] += pp_A[i][k]*pp_B[k][j];*/

	for (int i = 0; i < n_rows_A; i++)
		for (int j = 0; j < n_cols_B; j++)
		{
			pp_C[i][j] = 0;
				for(int k=0;k< n_cols_A; k++)
					pp_C[i][j] += pp_A[i][k]*pp_B[k][j];
		}
}

/**********************************************************************************************//**
 * fnMinor function: creation of minor matrix "pp_Minor" for square matrix "pp_A" of order
 * "n_order_A", and indices of matrix ppA "row_skip", "col_skip".
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param [in,out]	pp_A	 	If non-null, a.
 * \param 		  	n_order_A	The order a.
 * \param 		  	row_skip 	The row skip.
 * \param 		  	col_skip 	The col skip.
 * \param [in,out]	pp_Minor 	If non-null, the minor.
 **************************************************************************************************/

void CThreadFOT::fnMinor(double** pp_A,int n_order_A,int row_skip,int col_skip,double** pp_Minor)
{
	int row=0; 
	int row_minor=0;
	for(row;row<n_order_A;row++)
	{
		if(row==row_skip)	continue;
		int col=0;int col_minor=0;
		for(col;col<n_order_A;col++)
		{
			if(col==col_skip)	continue;
			pp_Minor[row_minor][col_minor] = pp_A[row][col];
			col_minor++;
		}
		row_minor++;
	}
}

/**********************************************************************************************//**
 * fnDeterminant: calculation of determinant of square matrix "pp_A" of order "n_order_A".
 * Variable "result" is return for the determinant. WARNING - RECURSIVE FUNCTION
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param [in,out]	pp_A	 	If non-null, a.
 * \param 		  	n_order_A	The order a.
 *
 * \return	A double.
 **************************************************************************************************/

double CThreadFOT::fnDeterminant(double **pp_A,int n_order_A)
{
	double result = 0.0;

	if(n_order_A>1)
	{
		// create Minor
		int n_order_Minor = n_order_A-1;
		double  **pp_Minor;
		pp_Minor = new double*[n_order_Minor];
		for(int i=0;i<n_order_Minor;i++)
			pp_Minor[i] = new double[n_order_Minor];

		// number of columns
		for(int col=0;col<n_order_A;col++)
		{
			// FIRST ROW
			fnMinor(pp_A,n_order_A,0,col,pp_Minor);

			// PLEASE CHECK --- RECURSION FUNCTION ===> fnDeterminant
			if(n_order_A == 2)
				result += pp_A[0][col]*std::pow(-1.0,(double)col)*pp_Minor[0][0]; // double check minor dimensions  // pow - from cmath.h
			else
				result += pp_A[0][col]*std::pow(-1.0,(double)col)*fnDeterminant(pp_Minor,n_order_Minor); // recursion // pow - from cmath.h
		}

		// delete Minor
		for(int i=0;i<n_order_Minor;i++)
			delete [] pp_Minor[i];
		delete [] pp_Minor; pp_Minor = 0;
	}

	return result;
}

/**********************************************************************************************//**
 * fnCoFactor: calculation of cofactor matrix "pp_CoFactor" for matrix "pp_A"
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param [in,out]	pp_A	   	If non-null, a.
 * \param 		  	n_order_A  	The order a.
 * \param [in,out]	pp_CoFactor	If non-null, the co factor.
 **************************************************************************************************/

void CThreadFOT::fnCofactor(double **pp_A,int n_order_A,double **pp_CoFactor)
{
	// create Minor
	int n_order_Minor = n_order_A -1;
	double  **pp_Minor;
	pp_Minor = new double*[n_order_Minor];
	for(int i=0;i<n_order_Minor;i++)
		pp_Minor[i] = new double[n_order_Minor];

	//Cofactor matrix is matrix of determinants from all minors
	for(int row=0;row<n_order_A;row++)
		for(int col=0;col<n_order_A;col++)
		{
			fnMinor(pp_A,n_order_A,row,col,pp_Minor);
			pp_CoFactor[row][col] = fnDeterminant(pp_Minor,n_order_Minor);
		}

		// delete Minor
		for(int i=0;i<n_order_Minor;i++)
			delete [] pp_Minor[i];
		delete [] pp_Minor;  pp_Minor = 0;
}

/**********************************************************************************************//**
 * fnMatrixInverse function: calculation of the matrix inverse of matrix "pp_A" of order,
 * "pp_A_inv".
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param [in,out]	pp_A	 	If non-null, a.
 * \param 		  	n_order_A	The order a.
 * \param [in,out]	pp_A_inv 	If non-null, a inverse.
 **************************************************************************************************/

void CThreadFOT::fnInverse(double **pp_A,int n_order_A,double **pp_A_inv)
{
	// inverse algorithm is analytical

	// create Cofactor
	double  **pp_CoFactor;
	pp_CoFactor = new double*[n_order_A];
	for(int i=0;i<n_order_A;i++)
		pp_CoFactor[i] = new double[n_order_A];

	fnCofactor(pp_A,n_order_A,pp_CoFactor);

	// NOTE: CoFactor Indices switched to get Transpose of Cofactor
	for(int row=0;row<n_order_A;row++)
		for(int col=0;col<n_order_A;col++)
			pp_A_inv[row][col] = pow(-1.0,(double)row+col)*pp_CoFactor[col][row]/fnDeterminant(pp_A,n_order_A);

	// delete Cofactor
	for(int i=0;i<n_order_A;i++)
		delete [] pp_CoFactor[i];
	delete [] pp_CoFactor;  pp_CoFactor = 0;
}

/**********************************************************************************************//**
 * Construct fourier
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param 		  	i_osc_freq   	Zero-based index of the osc frequency.
 * \param 		  	i_sample_freq	Zero-based index of the sample frequency.
 * \param [in,out]	pp_Fourier   	If non-null, the fourier.
 **************************************************************************************************/

void CThreadFOT::fnConstructFourier(int i_osc_freq,int i_sample_freq,double** pp_Fourier)
{
	// Construct Fourier matrix
	//----------------------------------------------------------
	//int size = (int) round(i_sample_freq/i_osc_freq); // round - from cmath.h
	int size = (int)CTlsFloat::Round((double)(i_sample_freq/i_osc_freq),0);

	for(int i=0;i<size;i++)
	{
		pp_Fourier[i][0] = 1;
		pp_Fourier[i][1] = std::cos(2*3.14*i_osc_freq*i/i_sample_freq);
		//pp_Fourier[i][2] = std::sin(2*3.14*i_osc_freq*i/i_sample_freq);
		pp_Fourier[i][2] = std::sin(2*3.14*i_osc_freq*i/i_sample_freq)*(-1);
	}
}

/**********************************************************************************************//**
 * Construct pseudo inverse
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param 		  	i_osc_freq			Zero-based index of the osc frequency.
 * \param 		  	i_sample_freq   	Zero-based index of the sample frequency.
 * \param [in,out]	pp_Fourier			If non-null, the fourier.
 * \param [in,out]	pp_PseudoInverse	If non-null, the pseudo inverse.
 **************************************************************************************************/

void CThreadFOT::fnConstructPseudoInverse(int i_osc_freq,int i_sample_freq,double** pp_Fourier,double** pp_PseudoInverse)
{
	// Construct matrix pPseudoInverse = ((A'A)^-1)A' (pseudo-inverse of A)
	//--------------------------------------------------------------------------
	// Step 1) Construct A' (transpose of A) - whta is A?? just the Fourier
	// Step 2) Construct C (A'A)
	// Step 3) Construct C^-1 (inverse of C)
	// Step 4) Construct pPseudoInverse (C^-1)A'
	//--------------------------------------------------------------------------

	//int size = (int) std::round(i_sample_freq/i_osc_freq); // round - from cmath.h
	int size = (int)CTlsFloat::Round((double)(i_sample_freq/i_osc_freq),0);

	// Step 1) A_t is A'
	//----------------------------------------------------------
	double  **pp_Fourier_T;
	pp_Fourier_T = new double*[3];
	for(int i=0;i<3;i++)
		pp_Fourier_T[i] = new double[size];

	fnTranspose(pp_Fourier,size,3,pp_Fourier_T);
	//----------------------------------------------------------


	// Step 2) C is A'A
	//----------------------------------------------------------
	double  **pp_C;
	pp_C = new double*[3];
	for(int i=0;i<3;i++)
		pp_C[i] = new double[3];

	fnMultiplication(pp_Fourier_T,3,size,pp_Fourier,3,pp_C);
	//------------------------------------------------------------


	// Step 3) C^-1
	//------------------------------------------------------------
	double  **pp_C_inv;
	pp_C_inv = new double*[3];
	for(int i=0;i<3;i++)
		pp_C_inv[i] = new double[3];

	fnInverse(pp_C,3,pp_C_inv);

	for(int i=0;i<3;i++)
		delete [] pp_C[i];
	delete [] pp_C;
	//------------------------------------------------------------


	// Step 4) pPseudoInverse
	//------------------------------------------------------------
	fnMultiplication(pp_C_inv,3,3,pp_Fourier_T,size,pp_PseudoInverse);

	for(int i=0;i<3;i++)
		delete [] pp_C_inv;
	delete [] pp_C_inv;

	for(int i=0;i<3;i++)
		delete [] pp_Fourier_T[i];
	delete [] pp_Fourier_T;

	//--------------------------------------------------------------
}

/**********************************************************************************************//**
 * Calculates the z coordinate
 *
 * \author	Rainer K�hner
 * \date	23.02.2018
 *
 * \param [in,out]	pp_Fourier			If non-null, the fourier.
 * \param [in,out]	pp_PseudoInverse	If non-null, the pseudo inverse.
 * \param [in,out]	pp_Flow				If non-null, the flow.
 * \param [in,out]	pp_Pressure			If non-null, the pressure.
 * \param 		  	i_osc_freq			Zero-based index of the osc frequency.
 * \param 		  	i_sample_freq   	Zero-based index of the sample frequency.
 * \param [in,out]	pOut				If non-null, the out.
 **************************************************************************************************/

void CThreadFOT::fnCalcZ(double** pp_Fourier, double** pp_PseudoInverse,double** pp_Flow,double** pp_Pressure,int i_osc_freq,int i_sample_freq,double *pOut)
{
	// 1) Step 1 - calculate resistance and reactance
	//-------------------------------------------------------------------------------------------------------------------------
	int size = (int)CTlsFloat::Round((double)(i_sample_freq/i_osc_freq),0);

	// pFLow & pPressure
	// needs clarification.
	double** pp_CoefP;
	pp_CoefP = new double*[3];
	for(int i=0;i<3;i++)
	{
		pp_CoefP[i] = new double[1];
		pp_CoefP[i][0] = 0;
	}

	double** pp_CoefF;
	pp_CoefF = new double*[3];
	for(int i=0;i<3;i++)
	{
		pp_CoefF[i] = new double[1];
		pp_CoefF[i][0] = 0;
	}

	// ...
	fnMultiplication(pp_PseudoInverse,3,size,pp_Pressure,1,pp_CoefP);
	fnMultiplication(pp_PseudoInverse,3,size,pp_Flow,1,pp_CoefF);

	double Resistance  = ((pp_CoefP[1][0]*pp_CoefF[1][0]) + (pp_CoefP[2][0]*pp_CoefF[2][0]))/(pp_CoefF[1][0]*pp_CoefF[1][0] + pp_CoefF[2][0]*pp_CoefF[2][0]);
	double Reactance   = ((pp_CoefP[2][0]*pp_CoefF[1][0]) - (pp_CoefP[1][0]*pp_CoefF[2][0]))/(pp_CoefF[1][0]*pp_CoefF[1][0] + pp_CoefF[2][0]*pp_CoefF[2][0]);

	//NEW CORRECTION FACTORS
	double factorK=  1;
	double factorJ=  1;
	if(m_bFOThfoRunning)
	{
		//a=1;
		//b=1;
		factorK=getModel()->getCONFIG()->getAmpCorFactorHFO_K();
		factorJ=getModel()->getCONFIG()->getAmpCorFactorHFO_J();
	}
	else
	{
		//a=  1,39;
		//b=  0,1507;
		factorK=getModel()->getCONFIG()->getAmpCorFactorCONV_K();
		factorJ=getModel()->getCONFIG()->getAmpCorFactorCONV_J();
	}

	double Resistance_corr = factorK*Resistance + factorJ*Reactance;    //value of Resistance corrected
	double Reactance_corr = factorK*Reactance - factorJ*Resistance;     //value of Reactance corrected


	//------------------------------------------------------------------------------------------------------------------------

	// 2) Step 2 - Calculate quality measures (FSI & PSI)
	//------------------------------------------------------------------------------------------------------------------------
	double** pp_SF;
	pp_SF = new double*[size];
	for(int i=0;i<size;i++)
		pp_SF[i] = new double[1];

	double** pp_SP;
	pp_SP = new double*[size];
	for(int i=0;i<size;i++)
		pp_SP[i] = new double[1];

	fnMultiplication(pp_Fourier,size,3,pp_CoefF,1,pp_SF);
	fnMultiplication(pp_Fourier,size,3,pp_CoefP,1,pp_SP);


	//FSI & FSP
	double FSI = 0, PSI = 0;
	for(int i = 0; i < size; i++)
	{
		FSI += std::abs(pp_Flow[i][0] - pp_SF[i][0]);           // abs - from cmath.h
		PSI += std::abs(pp_Pressure[i][0] - pp_SP[i][0]);       // abs - from cmath.h
	}

	double powerF = std::sqrt(pp_CoefF[1][0]*pp_CoefF[1][0] + pp_CoefF[2][0]*pp_CoefF[2][0]);   // sqrt - from cmath.h
	double powerP = std::sqrt(pp_CoefP[1][0]*pp_CoefP[1][0] + pp_CoefP[2][0]*pp_CoefP[2][0]);   // sqrt - from cmath.h
	FSI = FSI/(size*powerF);
	PSI = PSI/(size*powerP);

	//------------------------------------------------------------------------------------------------------------------------

	// 3) pOut is results of both computations
	//------------------------------------------------------------------------------------------------------------------------
	//pOut[0] = Resistance;
	//pOut[1] = Reactance;
	pOut[0] = Resistance_corr;
	pOut[1] = Reactance_corr;
	pOut[2] = FSI;
	pOut[3] = PSI;
	//------------------------------------------------------------------------------------------------------------------------
	
	// clear pp_CoefF
	for(int i=0;i<3;i++)
		delete [] pp_CoefF[i];
	delete [] pp_CoefF;
	// clear pp_CoefP
	for(int i=0;i<3;i++)
		delete [] pp_CoefP[i];
	delete [] pp_CoefP;
	// clear pp_SF
	for(int i=0;i<size;i++)
		delete [] pp_SF[i];
	delete [] pp_SF;
	// clear pp_SP
	for(int i=0;i<size;i++)
		delete [] pp_SP[i];
	delete [] pp_SP;
}
