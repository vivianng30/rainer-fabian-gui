//=============================================================================
/** 
* \file DataHandler.cpp
* \brief Functions for Data-Controller class of Model-View-Controller software architecture.
* \author Rainer Kuehner
* \date 2013-01-21 Creation of file
*
*       Copyright (c) 2013 Ing.Buero Rainer Kuehner
*                      Illerkirchberg
*
**/
//=============================================================================

//=============================================================================
/** 
* \brief description
* \class is used to indicate that the comment block contains documentation for the class Test
* \struct to document a C-struct.
* \enum to document an enumeration type. 
* \fn to document a function. 
* \var to document a variable or typedef or enum value
* \def to document a #define. 
* \typedef to document a type definition. 
* \file to document a file. 
* \author Rainer Kuehner
* \date 2013-01-21 Creation of file
*
*
**/
//=============================================================================



#include "StdAfx.h"
#include "DataHandler.h"
#include "TlsBlob.h"
#include "MVModel.h"
#include "pwingdi.h"
#include "regkey.h"
#include "TlsFile.h"
#include "TlsRegistry.h"
#include "TlsIniFile.h"
#include "TlsXml.h"
#include "Rijndael.h"
#include "MVViewHandler.h"

#define MAXTICKS	4294080000
#define NURSEPINBIT 5

#define FLOWSENSORRANGE_BIT		0
#define DEVICETYPE_EVOSERIE_BIT	1
#define DEVICETYPE_HFO_BIT		2
#define PATIENTGASSUPPLY1_BIT	3
#define PATIENTGASSUPPLY2_BIT	4
#define HFOMODULE_BIT			5

#define FABIANINI       _T("\\FFSDISK\\fabianhfo.ini")
#define NUMERICCONFINI       _T("\\FFSDISK\\numericconfig.ini")


#define	MIN_ETIME_RECRUITMENT		2000	//!< minimum for E-TIME lung recruitment

 /**
 * @brief Max Timespan [seconds] to delete all Trendfiles when devices was turned off.
 **/
enum eTrendTimespanToDelete {
	TTTD_NEVER			= 0, 
	TTTD_1HR			= 3600,		//!< 1 hours
	TTTD_2HRS			= 7200,		//!< 2 hours
	TTTD_5HRS			= 18000,	//!< 5 hours
	TTTD_10HRS			= 36000,	//!< 10 hours
	TTTD_1DAY			= 86400,	//!< 1 day
	TTTD_2DAYS			= 172800,	//!< 2 day
	TTTD_3DAYS			= 259200,	//!< 3 day
	TTTD_4DAYS			= 345600,	//!< 4 day
	TTTD_5DAYS			= 432000	//!< 5 day
};



//***************************************************************************
//     Berechnungen: 
//
//		MV (Atemminutenvolumen):
//
//			MV = ?????Atemfrequenz * Atemzugvolumen????
//
//		Inspirationszeit & Exspirationszeit:
//
//			Exspirationszeit = (60/BPM) - Inspirationszeit
//			Verhälntnis I : E = 1 : x, wobei x = Exspirationszeit / Inspirationszeit
//
//		
//
//
//**************************************************************************

CDataHandler* CDataHandler::theDataHandler=0;

CircularBuffer<PBUFSPO2> CDataHandler::m_rbufSPO2(MAXSIZE_SPIREADIN_BUFFER); 
CircularBuffer<SHORT> CDataHandler::m_rbufCO2(MAXSIZE_SPIREADIN_BUFFER); 
CircularBuffer<PBUFSPI> CDataHandler::m_rbufSPI(MAXSIZE_SPIREADIN_BUFFER); 
CircularBuffer<PBUFSPI> CDataHandler::m_rbufVent(MAXSIZE_RING_BUFFER); 
CircularBuffer<PBUFSPI> CDataHandler::m_rbufCopy(MAXSIZE_RING_BUFFER); 

PBUFSPI* CDataHandler::m_pbufSavedBreath=NULL;

CParaDataPRESET* CDataHandler::m_PRESET=NULL;
CParaData* CDataHandler::m_PARADATA=NULL;

LPNUMERICINI CDataHandler::m_pbufNumericIPPV=NULL;
LPNUMERICINI CDataHandler::m_pbufNumericSIPPV=NULL;
LPNUMERICINI CDataHandler::m_pbufNumericSIMV=NULL;
LPNUMERICINI CDataHandler::m_pbufNumericSIMVPSV=NULL;
LPNUMERICINI CDataHandler::m_pbufNumericPSV=NULL;
LPNUMERICINI CDataHandler::m_pbufNumericCPAP=NULL;
LPNUMERICINI CDataHandler::m_pbufNumericHFO=NULL;
LPNUMERICINI CDataHandler::m_pbufNumericNCPAP=NULL;
LPNUMERICINI CDataHandler::m_pbufNumericDUOPAP=NULL;
LPNUMERICINI CDataHandler::m_pbufNumericTHERAPY=NULL;
LPNUMERICINI CDataHandler::m_pbufNumericFLOWOFFCONV=NULL;
LPNUMERICINI CDataHandler::m_pbufNumericFLOWOFFCPAP=NULL;
LPNUMERICINI CDataHandler::m_pbufNumericFLOWOFFHFO=NULL;

INT* CDataHandler::m_pbufMessureAVG=NULL;
INT* CDataHandler::m_pbufMessureBTB=NULL;


/**********************************************************************************************//**
 * @fn	CDataHandler::CDataHandler(void)
 *
 * @brief	Default constructor.
 * 			- initializes all critical sections
 *			- initializes all member variables
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
CDataHandler::CDataHandler(void)
{
	m_bExit=false;
	m_pModel = NULL;
	m_pbufSavedBreath=NULL;

	InitializeCriticalSection(&csOpTime);
	InitializeCriticalSection(&csSPIDataBuffer);
	InitializeCriticalSection(&csCopyDataBuffer);
	InitializeCriticalSection(&csVentDataBuffer);
	InitializeCriticalSection(&csCO2DataBuffer);
	InitializeCriticalSection(&csSPO2DataBuffer);
	InitializeCriticalSection(&csMainboardData);
	InitializeCriticalSection(&csMessureDataAVG);
	InitializeCriticalSection(&csMessureDataBTB);
	InitializeCriticalSection(&csMessureData);
	InitializeCriticalSection(&csAlarmStateBytes);
	InitializeCriticalSection(&csFlowsensorStateBytes);
	InitializeCriticalSection(&csFlowsensorState);
	InitializeCriticalSection(&csOxyCal);
	InitializeCriticalSection(&csOxyState);
	InitializeCriticalSection(&csTrendFileData);
	InitializeCriticalSection(&csTrend);
	InitializeCriticalSection(&csTrendUpdate);
	InitializeCriticalSection(&csDelTrendThread);
	InitializeCriticalSection(&csSavedBreath);
	InitializeCriticalSection(&csFOTosciState);

	//m_ePrevConvTrigger=TRIGGER_NONE;
	m_ePrevTrigger_CONV=TRIGGER_NONE;
	//m_ePrevTrigger_CPAP=TRIGGER_NONE;

	m_bLEDdisplay=true;

	m_bPRICOrunning=false;
	
	m_iNumericIPPVcount=0;
	m_iNumericSIPPVcount=0;
	m_iNumericSIMVcount=0;
	m_iNumericSIMVPSVcount=0;
	m_iNumericPSVcount=0;
	m_iNumericCPAPcount=0;
	m_iNumericHFOcount=0;
	m_iNumericNCPAPcount=0;
	m_iNumericDUOPAPcount=0;
	m_iNumericTHERAPYcount=0;
	m_iNumericFLOWOFFCONVcount=0;
	m_iNumericFLOWOFFCPAPcount=0;
	m_iNumericFLOWOFFHFOcount=0;

	m_iSPO2waveData=0;

	m_nCountDelTrends=0;
	m_bDoDelTrendThread=false;
	
	m_wHardwareConfig=0;
	m_byI2CWatchdogState=0;

	m_bVGUARANTLicenseAvailable=false;
	m_bVLIMITLicenseAvailable=false;
	m_bHFOLicenseAvailable=false;
	m_bNMODELicenseAvailable=false;
	m_bLUNGRECLicenseAvailable=false;
	m_bTRENDLicenseAvailable=false;
	m_bTHERAPYLicenseAvailable=false;
	m_bPRICOLicenseAvailable=false;
	m_bFOTLicenseAvailable=false;
	m_bNIVTRIGGERLicenseAvailable=false;

	m_bVGUARANTDemoLicAvailable=false;
	m_bVLIMITDemoLicAvailable=false;
	m_bHFODemoLicAvailable=false;
	m_bNMODEDemoLicAvailable=false;
	m_bLUNGRECDemoLicAvailable=false;
	m_bTRENDDemoLicAvailable=false;
	m_bTHERAPYDemoLicAvailable=false;
	m_bPRICODemoLicAvailable=false;
	m_bFOTDemoLicAvailable=false;
	m_bNIVTRIGGERDemoLicAvailable=false;

	m_iBodyweightGramm=0;

	m_bFOToscillationState=false;

	m_pbufSavedBreath = new PBUFSPI[G_MAXPOINTS];
	m_iSizeSavedBreath=0;

	m_pbufMessureAVG=new INT[MAX_NUMERICVALUES];
	m_pbufMessureBTB=new INT[MAX_NUMERICVALUES];

	m_pbufNumericIPPV = new NUMERICINI[MAX_NUMERICMODEBLOCK];
	m_pbufNumericSIPPV = new NUMERICINI[MAX_NUMERICMODEBLOCK];
	m_pbufNumericSIMV = new NUMERICINI[MAX_NUMERICMODEBLOCK];
	m_pbufNumericSIMVPSV = new NUMERICINI[MAX_NUMERICMODEBLOCK];
	m_pbufNumericPSV = new NUMERICINI[MAX_NUMERICMODEBLOCK];
	m_pbufNumericCPAP = new NUMERICINI[MAX_NUMERICMODEBLOCK];
	m_pbufNumericHFO = new NUMERICINI[MAX_NUMERICMODEBLOCK];
	m_pbufNumericNCPAP = new NUMERICINI[MAX_NUMERICMODEBLOCK];
	m_pbufNumericDUOPAP = new NUMERICINI[MAX_NUMERICMODEBLOCK];
	m_pbufNumericTHERAPY = new NUMERICINI[MAX_NUMERICMODEBLOCK];
	m_pbufNumericFLOWOFFCONV = new NUMERICINI[MAX_NUMERICMODEBLOCK];
	m_pbufNumericFLOWOFFCPAP = new NUMERICINI[MAX_NUMERICMODEBLOCK];
	m_pbufNumericFLOWOFFHFO = new NUMERICINI[MAX_NUMERICMODEBLOCK];

	for(int i=0;i<3;i++)
	{
		m_pbufNumericIPPV[i].eNumMode=NUMMODE_IPPV;
		m_pbufNumericIPPV[i].SHOW=FALSE;
		m_pbufNumericIPPV[i].VAL1=NUMT_PPEAK;
		m_pbufNumericIPPV[i].VAL2=NUMT_PPEAK;
		m_pbufNumericIPPV[i].VAL3=NUMT_PPEAK;
		m_pbufNumericIPPV[i].VAL4=NUMT_PPEAK;
		m_pbufNumericIPPV[i].VAL5=NUMT_PPEAK;
		m_pbufNumericIPPV[i].VAL6=NUMT_PPEAK;
		m_pbufNumericIPPV[i].VAL7=NUMT_PPEAK;
		m_pbufNumericIPPV[i].VAL8=NUMT_PPEAK;
		m_pbufNumericIPPV[i].SIZE1=NUMERICSIZE_0;
		m_pbufNumericIPPV[i].SIZE2=NUMERICSIZE_0;
		m_pbufNumericIPPV[i].SIZE3=NUMERICSIZE_0;
		m_pbufNumericIPPV[i].SIZE4=NUMERICSIZE_0;
		m_pbufNumericIPPV[i].SIZE5=NUMERICSIZE_0;
		m_pbufNumericIPPV[i].SIZE6=NUMERICSIZE_0;
		m_pbufNumericIPPV[i].SIZE7=NUMERICSIZE_0;
		m_pbufNumericIPPV[i].SIZE8=NUMERICSIZE_0;

		m_pbufNumericSIPPV[i].eNumMode=NUMMODE_SIPPV;
		m_pbufNumericSIPPV[i].SHOW=FALSE;
		m_pbufNumericSIPPV[i].VAL1=NUMT_PPEAK;
		m_pbufNumericSIPPV[i].VAL2=NUMT_PPEAK;
		m_pbufNumericSIPPV[i].VAL3=NUMT_PPEAK;
		m_pbufNumericSIPPV[i].VAL4=NUMT_PPEAK;
		m_pbufNumericSIPPV[i].VAL5=NUMT_PPEAK;
		m_pbufNumericSIPPV[i].VAL6=NUMT_PPEAK;
		m_pbufNumericSIPPV[i].VAL7=NUMT_PPEAK;
		m_pbufNumericSIPPV[i].VAL8=NUMT_PPEAK;
		m_pbufNumericSIPPV[i].SIZE1=NUMERICSIZE_0;
		m_pbufNumericSIPPV[i].SIZE2=NUMERICSIZE_0;
		m_pbufNumericSIPPV[i].SIZE3=NUMERICSIZE_0;
		m_pbufNumericSIPPV[i].SIZE4=NUMERICSIZE_0;
		m_pbufNumericSIPPV[i].SIZE5=NUMERICSIZE_0;
		m_pbufNumericSIPPV[i].SIZE6=NUMERICSIZE_0;
		m_pbufNumericSIPPV[i].SIZE7=NUMERICSIZE_0;
		m_pbufNumericSIPPV[i].SIZE8=NUMERICSIZE_0;

		m_pbufNumericSIMV[i].eNumMode=NUMMODE_SIMV;
		m_pbufNumericSIMV[i].SHOW=FALSE;
		m_pbufNumericSIMV[i].VAL1=NUMT_PPEAK;
		m_pbufNumericSIMV[i].VAL2=NUMT_PPEAK;
		m_pbufNumericSIMV[i].VAL3=NUMT_PPEAK;
		m_pbufNumericSIMV[i].VAL4=NUMT_PPEAK;
		m_pbufNumericSIMV[i].VAL5=NUMT_PPEAK;
		m_pbufNumericSIMV[i].VAL6=NUMT_PPEAK;
		m_pbufNumericSIMV[i].VAL7=NUMT_PPEAK;
		m_pbufNumericSIMV[i].VAL8=NUMT_PPEAK;
		m_pbufNumericSIMV[i].SIZE1=NUMERICSIZE_0;
		m_pbufNumericSIMV[i].SIZE2=NUMERICSIZE_0;
		m_pbufNumericSIMV[i].SIZE3=NUMERICSIZE_0;
		m_pbufNumericSIMV[i].SIZE4=NUMERICSIZE_0;
		m_pbufNumericSIMV[i].SIZE5=NUMERICSIZE_0;
		m_pbufNumericSIMV[i].SIZE6=NUMERICSIZE_0;
		m_pbufNumericSIMV[i].SIZE7=NUMERICSIZE_0;
		m_pbufNumericSIMV[i].SIZE8=NUMERICSIZE_0;

		m_pbufNumericSIMVPSV[i].eNumMode=NUMMODE_SIMVPSV;
		m_pbufNumericSIMVPSV[i].SHOW=FALSE;
		m_pbufNumericSIMVPSV[i].VAL1=NUMT_PPEAK;
		m_pbufNumericSIMVPSV[i].VAL2=NUMT_PPEAK;
		m_pbufNumericSIMVPSV[i].VAL3=NUMT_PPEAK;
		m_pbufNumericSIMVPSV[i].VAL4=NUMT_PPEAK;
		m_pbufNumericSIMVPSV[i].VAL5=NUMT_PPEAK;
		m_pbufNumericSIMVPSV[i].VAL6=NUMT_PPEAK;
		m_pbufNumericSIMVPSV[i].VAL7=NUMT_PPEAK;
		m_pbufNumericSIMVPSV[i].VAL8=NUMT_PPEAK;
		m_pbufNumericSIMVPSV[i].SIZE1=NUMERICSIZE_0;
		m_pbufNumericSIMVPSV[i].SIZE2=NUMERICSIZE_0;
		m_pbufNumericSIMVPSV[i].SIZE3=NUMERICSIZE_0;
		m_pbufNumericSIMVPSV[i].SIZE4=NUMERICSIZE_0;
		m_pbufNumericSIMVPSV[i].SIZE5=NUMERICSIZE_0;
		m_pbufNumericSIMVPSV[i].SIZE6=NUMERICSIZE_0;
		m_pbufNumericSIMVPSV[i].SIZE7=NUMERICSIZE_0;
		m_pbufNumericSIMVPSV[i].SIZE8=NUMERICSIZE_0;

		m_pbufNumericPSV[i].eNumMode=NUMMODE_PSV;
		m_pbufNumericPSV[i].SHOW=FALSE;
		m_pbufNumericPSV[i].VAL1=NUMT_PPEAK;
		m_pbufNumericPSV[i].VAL2=NUMT_PPEAK;
		m_pbufNumericPSV[i].VAL3=NUMT_PPEAK;
		m_pbufNumericPSV[i].VAL4=NUMT_PPEAK;
		m_pbufNumericPSV[i].VAL5=NUMT_PPEAK;
		m_pbufNumericPSV[i].VAL6=NUMT_PPEAK;
		m_pbufNumericPSV[i].VAL7=NUMT_PPEAK;
		m_pbufNumericPSV[i].VAL8=NUMT_PPEAK;
		m_pbufNumericPSV[i].SIZE1=NUMERICSIZE_0;
		m_pbufNumericPSV[i].SIZE2=NUMERICSIZE_0;
		m_pbufNumericPSV[i].SIZE3=NUMERICSIZE_0;
		m_pbufNumericPSV[i].SIZE4=NUMERICSIZE_0;
		m_pbufNumericPSV[i].SIZE5=NUMERICSIZE_0;
		m_pbufNumericPSV[i].SIZE6=NUMERICSIZE_0;
		m_pbufNumericPSV[i].SIZE7=NUMERICSIZE_0;
		m_pbufNumericPSV[i].SIZE8=NUMERICSIZE_0;

		m_pbufNumericCPAP[i].eNumMode=NUMMODE_CPAP;
		m_pbufNumericCPAP[i].SHOW=FALSE;
		m_pbufNumericCPAP[i].VAL1=NUMT_PPEAK;
		m_pbufNumericCPAP[i].VAL2=NUMT_PPEAK;
		m_pbufNumericCPAP[i].VAL3=NUMT_PPEAK;
		m_pbufNumericCPAP[i].VAL4=NUMT_PPEAK;
		m_pbufNumericCPAP[i].VAL5=NUMT_PPEAK;
		m_pbufNumericCPAP[i].VAL6=NUMT_PPEAK;
		m_pbufNumericCPAP[i].VAL7=NUMT_PPEAK;
		m_pbufNumericCPAP[i].VAL8=NUMT_PPEAK;
		m_pbufNumericCPAP[i].SIZE1=NUMERICSIZE_0;
		m_pbufNumericCPAP[i].SIZE2=NUMERICSIZE_0;
		m_pbufNumericCPAP[i].SIZE3=NUMERICSIZE_0;
		m_pbufNumericCPAP[i].SIZE4=NUMERICSIZE_0;
		m_pbufNumericCPAP[i].SIZE5=NUMERICSIZE_0;
		m_pbufNumericCPAP[i].SIZE6=NUMERICSIZE_0;
		m_pbufNumericCPAP[i].SIZE7=NUMERICSIZE_0;
		m_pbufNumericCPAP[i].SIZE8=NUMERICSIZE_0;

		m_pbufNumericHFO[i].eNumMode=NUMMODE_HFO;
		m_pbufNumericHFO[i].SHOW=FALSE;
		m_pbufNumericHFO[i].VAL1=NUMT_PPEAK;
		m_pbufNumericHFO[i].VAL2=NUMT_PPEAK;
		m_pbufNumericHFO[i].VAL3=NUMT_PPEAK;
		m_pbufNumericHFO[i].VAL4=NUMT_PPEAK;
		m_pbufNumericHFO[i].VAL5=NUMT_PPEAK;
		m_pbufNumericHFO[i].VAL6=NUMT_PPEAK;
		m_pbufNumericHFO[i].VAL7=NUMT_PPEAK;
		m_pbufNumericHFO[i].VAL8=NUMT_PPEAK;
		m_pbufNumericHFO[i].SIZE1=NUMERICSIZE_0;
		m_pbufNumericHFO[i].SIZE2=NUMERICSIZE_0;
		m_pbufNumericHFO[i].SIZE3=NUMERICSIZE_0;
		m_pbufNumericHFO[i].SIZE4=NUMERICSIZE_0;
		m_pbufNumericHFO[i].SIZE5=NUMERICSIZE_0;
		m_pbufNumericHFO[i].SIZE6=NUMERICSIZE_0;
		m_pbufNumericHFO[i].SIZE7=NUMERICSIZE_0;
		m_pbufNumericHFO[i].SIZE8=NUMERICSIZE_0;

		m_pbufNumericNCPAP[i].eNumMode=NUMMODE_NCPAP;
		m_pbufNumericNCPAP[i].SHOW=FALSE;
		m_pbufNumericNCPAP[i].VAL1=NUMT_PPEAK;
		m_pbufNumericNCPAP[i].VAL2=NUMT_PPEAK;
		m_pbufNumericNCPAP[i].VAL3=NUMT_PPEAK;
		m_pbufNumericNCPAP[i].VAL4=NUMT_PPEAK;
		m_pbufNumericNCPAP[i].VAL5=NUMT_PPEAK;
		m_pbufNumericNCPAP[i].VAL6=NUMT_PPEAK;
		m_pbufNumericNCPAP[i].VAL7=NUMT_PPEAK;
		m_pbufNumericNCPAP[i].VAL8=NUMT_PPEAK;
		m_pbufNumericNCPAP[i].SIZE1=NUMERICSIZE_0;
		m_pbufNumericNCPAP[i].SIZE2=NUMERICSIZE_0;
		m_pbufNumericNCPAP[i].SIZE3=NUMERICSIZE_0;
		m_pbufNumericNCPAP[i].SIZE4=NUMERICSIZE_0;
		m_pbufNumericNCPAP[i].SIZE5=NUMERICSIZE_0;
		m_pbufNumericNCPAP[i].SIZE6=NUMERICSIZE_0;
		m_pbufNumericNCPAP[i].SIZE7=NUMERICSIZE_0;
		m_pbufNumericNCPAP[i].SIZE8=NUMERICSIZE_0;

		m_pbufNumericDUOPAP[i].eNumMode=NUMMODE_DUOPAP;
		m_pbufNumericDUOPAP[i].SHOW=FALSE;
		m_pbufNumericDUOPAP[i].VAL1=NUMT_PPEAK;
		m_pbufNumericDUOPAP[i].VAL2=NUMT_PPEAK;
		m_pbufNumericDUOPAP[i].VAL3=NUMT_PPEAK;
		m_pbufNumericDUOPAP[i].VAL4=NUMT_PPEAK;
		m_pbufNumericDUOPAP[i].VAL5=NUMT_PPEAK;
		m_pbufNumericDUOPAP[i].VAL6=NUMT_PPEAK;
		m_pbufNumericDUOPAP[i].VAL7=NUMT_PPEAK;
		m_pbufNumericDUOPAP[i].VAL8=NUMT_PPEAK;
		m_pbufNumericDUOPAP[i].SIZE1=NUMERICSIZE_0;
		m_pbufNumericDUOPAP[i].SIZE2=NUMERICSIZE_0;
		m_pbufNumericDUOPAP[i].SIZE3=NUMERICSIZE_0;
		m_pbufNumericDUOPAP[i].SIZE4=NUMERICSIZE_0;
		m_pbufNumericDUOPAP[i].SIZE5=NUMERICSIZE_0;
		m_pbufNumericDUOPAP[i].SIZE6=NUMERICSIZE_0;
		m_pbufNumericDUOPAP[i].SIZE7=NUMERICSIZE_0;
		m_pbufNumericDUOPAP[i].SIZE8=NUMERICSIZE_0;

		m_pbufNumericTHERAPY[i].eNumMode=NUMMODE_THERAPY;
		m_pbufNumericTHERAPY[i].SHOW=FALSE;
		m_pbufNumericTHERAPY[i].VAL1=NUMT_PPEAK;
		m_pbufNumericTHERAPY[i].VAL2=NUMT_PPEAK;
		m_pbufNumericTHERAPY[i].VAL3=NUMT_PPEAK;
		m_pbufNumericTHERAPY[i].VAL4=NUMT_PPEAK;
		m_pbufNumericTHERAPY[i].VAL5=NUMT_PPEAK;
		m_pbufNumericTHERAPY[i].VAL6=NUMT_PPEAK;
		m_pbufNumericTHERAPY[i].VAL7=NUMT_PPEAK;
		m_pbufNumericTHERAPY[i].VAL8=NUMT_PPEAK;
		m_pbufNumericTHERAPY[i].SIZE1=NUMERICSIZE_0;
		m_pbufNumericTHERAPY[i].SIZE2=NUMERICSIZE_0;
		m_pbufNumericTHERAPY[i].SIZE3=NUMERICSIZE_0;
		m_pbufNumericTHERAPY[i].SIZE4=NUMERICSIZE_0;
		m_pbufNumericTHERAPY[i].SIZE5=NUMERICSIZE_0;
		m_pbufNumericTHERAPY[i].SIZE6=NUMERICSIZE_0;
		m_pbufNumericTHERAPY[i].SIZE7=NUMERICSIZE_0;
		m_pbufNumericTHERAPY[i].SIZE8=NUMERICSIZE_0;

		m_pbufNumericFLOWOFFCONV[i].eNumMode=NUMMODE_FLOWOFFCONV;
		m_pbufNumericFLOWOFFCONV[i].SHOW=FALSE;
		m_pbufNumericFLOWOFFCONV[i].VAL1=NUMT_PPEAK;
		m_pbufNumericFLOWOFFCONV[i].VAL2=NUMT_PPEAK;
		m_pbufNumericFLOWOFFCONV[i].VAL3=NUMT_PPEAK;
		m_pbufNumericFLOWOFFCONV[i].VAL4=NUMT_PPEAK;
		m_pbufNumericFLOWOFFCONV[i].VAL5=NUMT_PPEAK;
		m_pbufNumericFLOWOFFCONV[i].VAL6=NUMT_PPEAK;
		m_pbufNumericFLOWOFFCONV[i].VAL7=NUMT_PPEAK;
		m_pbufNumericFLOWOFFCONV[i].VAL8=NUMT_PPEAK;
		m_pbufNumericFLOWOFFCONV[i].SIZE1=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFCONV[i].SIZE2=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFCONV[i].SIZE3=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFCONV[i].SIZE4=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFCONV[i].SIZE5=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFCONV[i].SIZE6=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFCONV[i].SIZE7=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFCONV[i].SIZE8=NUMERICSIZE_0;

		m_pbufNumericFLOWOFFCPAP[i].eNumMode=NUMMODE_FLOWOFFCPAP;
		m_pbufNumericFLOWOFFCPAP[i].SHOW=FALSE;
		m_pbufNumericFLOWOFFCPAP[i].VAL1=NUMT_PPEAK;
		m_pbufNumericFLOWOFFCPAP[i].VAL2=NUMT_PPEAK;
		m_pbufNumericFLOWOFFCPAP[i].VAL3=NUMT_PPEAK;
		m_pbufNumericFLOWOFFCPAP[i].VAL4=NUMT_PPEAK;
		m_pbufNumericFLOWOFFCPAP[i].VAL5=NUMT_PPEAK;
		m_pbufNumericFLOWOFFCPAP[i].VAL6=NUMT_PPEAK;
		m_pbufNumericFLOWOFFCPAP[i].VAL7=NUMT_PPEAK;
		m_pbufNumericFLOWOFFCPAP[i].VAL8=NUMT_PPEAK;
		m_pbufNumericFLOWOFFCPAP[i].SIZE1=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFCPAP[i].SIZE2=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFCPAP[i].SIZE3=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFCPAP[i].SIZE4=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFCPAP[i].SIZE5=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFCPAP[i].SIZE6=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFCPAP[i].SIZE7=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFCPAP[i].SIZE8=NUMERICSIZE_0;

		m_pbufNumericFLOWOFFHFO[i].eNumMode=NUMMODE_FLOWOFFHFO;
		m_pbufNumericFLOWOFFHFO[i].SHOW=FALSE;
		m_pbufNumericFLOWOFFHFO[i].VAL1=NUMT_PPEAK;
		m_pbufNumericFLOWOFFHFO[i].VAL2=NUMT_PPEAK;
		m_pbufNumericFLOWOFFHFO[i].VAL3=NUMT_PPEAK;
		m_pbufNumericFLOWOFFHFO[i].VAL4=NUMT_PPEAK;
		m_pbufNumericFLOWOFFHFO[i].VAL5=NUMT_PPEAK;
		m_pbufNumericFLOWOFFHFO[i].VAL6=NUMT_PPEAK;
		m_pbufNumericFLOWOFFHFO[i].VAL7=NUMT_PPEAK;
		m_pbufNumericFLOWOFFHFO[i].VAL8=NUMT_PPEAK;
		m_pbufNumericFLOWOFFHFO[i].SIZE1=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFHFO[i].SIZE2=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFHFO[i].SIZE3=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFHFO[i].SIZE4=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFHFO[i].SIZE5=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFHFO[i].SIZE6=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFHFO[i].SIZE7=NUMERICSIZE_0;
		m_pbufNumericFLOWOFFHFO[i].SIZE8=NUMERICSIZE_0;
	}

	m_bO21SensorCalState=false;
	m_bO100SensorCalState=false;
	m_bFlowSensorCalState=false;

	m_bOxyCalRunning=false;
	m_bOxyHourglassRunning=false;

	m_bPatientDataAvailable=false;

	m_iCountDIOERROR=0;

	m_bFreezedGraphs=false;
	
	m_bSavingTrendToUSB=false;
	m_bTrendsLoading=false;

	m_iCO2ErrorCodeBits=0;
	m_iCOMErrorCodeBits=0;
	m_iSPIErrorCodeBits=0;

	m_szCOMlastSendError=_T("");

	m_bShowFlowAC=FALSE;

	m_bPSVapnoe=false;
	m_bNurscallAlarm=false;

	m_bVentilBurnRunning=false;

	m_iO2Difference=0;
	m_iPmeanDifference=0;
	m_iPEEP_PPSVdifference=0;

	m_bHFOrunning=false;
	
	m_wACCU_DAT_STATE=0;
	m_bAccuSupply=false;
	m_wBattStatMinutes=0;
	m_wBattStatPercents=0;
	m_bShowBatteryEmpty=true;
	m_bShowBattery60=false;//es wird erst ab 30 Minuten alarmiert!
	m_bShowBattery30=true;
	m_bShowBatteryState15=true;

	m_bI2CError=false;

	m_bVolGarantyRunning=false;

	m_eOxySensorCalState=OS_UNKNOWN;
	m_eFlowSensorCalState=FS_UNKNOWN;
	m_eO2SensorState=OXYSENS_ON;
	m_eFlowSensorState=FLOWSENSOR_ON;
	m_ePrevFlowSensorState=FLOWSENSOR_ON;
	
	m_dwOpTimeDevTickCountStarted=0;
	m_dwOpTimeBattTickCountStarted=0;
	m_dwOpTimeHFOTickCountStarted=0;

	m_iAlarmState=0;
	m_iFlowsensorState=0;
	m_bConPICChecksumError=false;
	m_iMSTATUS_ConPIC=0;
	m_bExhalValvCalMode=false;
	m_bPinspNotReached=false;

	m_iMSTATUS_BlenderPIC=0;

	m_dataMessure.m_iPRESSURE		= 0;
	m_dataMessure.m_iFLOW			= 0;
	m_dataMessure.m_iVOLUME			= 0;
	m_dataMessure.m_iVolumeCorr		= 0;
	

	for(BYTE i=0;i<MAX_NUMERICVALUES;i++)
	{
		m_pbufMessureAVG[i]=0;
		m_pbufMessureBTB[i]=0;
	}
	
	m_iTempITime= 0;
	m_iTempETime= 0;

	m_iIERatioIpart= 0;
	m_iIERatioEpart= 0;
	
	m_iSaveIERelationValue_ITIME=100;
	m_iSaveIERelationValue_ETIME=200;

	m_dataMainboard.m_iMonitorChecksum=0;
	m_dataMainboard.m_iMonitorVersion_x=0;
	m_dataMainboard.m_iMonitorVersion_y=0;
	m_dataMainboard.m_iMonitorVersion_z=0;
	m_dataMainboard.m_iConductorChecksum=0;
	m_dataMainboard.m_szConductorVersion=_T("0.0.0");
	m_dataMainboard.m_iHfoChecksum=0;
	m_dataMainboard.m_szHfoVersion=_T("0.0.0");
	m_dataMainboard.m_szBlenderVersion=_T("0.0");

	m_iConductorCALPRESSSCALE=0;
	m_iConductorCALPRESSOFFSET=0;

	m_eCurVgarantState=VOLUMEGAR_NONE;

	m_Status2=0;

	//******************TrendData*******************//
	m_iTrendCnt=0;
	m_fTrendData_Pmean=0;
	m_fTrendData_FiO2=0;
	m_fTrendData_Pinsp=0;
	m_fTrendData_Vte=0;
	m_fTrendData_Compliance=0;
	m_fTrendData_CO2HFO=0;
	m_fTrendData_MV=0;
	m_fTrendData_HFAMP=0;
	m_fTrendData_RSBI=0;
	m_fTrendData_ShareMVmand=0;
	m_fTrendData_Resistance=0;
	m_fTrendData_Leak=0;
	m_fTrendData_SpO2=0;
	m_fTrendData_PI=0;
	m_fTrendData_etCO2=0;
	m_fTrendData_SpO2PR=0;
	m_fTrendData_Frequency=0;


	m_chFilenumPINSP=1;
	m_chFilenumPMEAN=1;
	m_chFilenumFIO2=1;
	m_chFilenumVTE=1;
	m_chFilenumCOMPLIANCE=1;
	m_chFilenumCO2HFO=1;
	m_chFilenumMV=1;
	m_chFilenumHFAMP=1;
	m_chFilenumRSBI=1;
	m_chFilenumShareMVmand=1;
	m_chFilenumResistance=1;
	m_chFilenumLeak=1;
	m_chFilenumSpO2=1;
	m_chFilenumSPO2PI=1;
	m_chFilenumEtCO2=1;
	m_chFilenumSpO2PR=1;
	m_chFilenumFrequency=1;
	
	m_iContrastParm=0;

	m_iLastVGarantParam=0;
	m_iLastHFVGarantParam=0;
	m_iLastVLimitParam=0;

	m_crGraphColor_PRESSURE=0x8c8c8c;
	m_crGraphColor_FLOW=0x8c8c8c;
	m_crGraphColor_VOLUME=0x8c8c8c;
	m_crGraphColor_ETCO2=0x8c8c8c;
	m_crGraphColor_SPO2=0x8c8c8c;
	m_crGraphColor_LOOP=0x0000ff;
	m_crGraphColor_SAVEDLOOP=0x000000;
	m_crGraphColor_TRIGGER=0x00dc00;

	m_bProcPressureCal60=false;

	m_dwRemainCO2PumpTime=0;

	m_bDUOPAPtriggerAutoEnable=false;
	m_iPrevTRIGGERDUOPAPPara=0;
	m_bTriggerDUOPAPenabled=true;

	m_bInitialSaveTrend=false;
	m_bTrendUpdateRunning=false;
	m_sTempTrendData_Pmean=0;
	m_wTempTrendData_Pinsp=0;
	m_wTempTrendData_FiO2=0;
	m_wTempTrendData_Vte=0;
	m_wTempTrendData_Compliance=0;
	m_wTempTrendData_CO2HFO=0;
	m_wTempTrendData_MV=0;
	m_wTempTrendData_HFAMP=0;
	m_wTempTrendData_RSBI=0;
	m_wTempTrendData_ShareMVmand=0;
	m_wTempTrendData_Resistance=0;
	m_wTempTrendData_Leak=0;
	m_wTempTrendData_SpO2=0;
	m_wTempTrendData_PI=0;
	m_wTempTrendData_etCO2=0;
	m_wTempTrendData_SpO2PR=0;
	m_wTempTrendData_Frequency=0;
	//m_dtTempTimestamp=COleDateTime::null;
}

/**********************************************************************************************//**
 * @fn	CDataHandler::~CDataHandler(void)
 *
 * @brief	Destructor.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/

CDataHandler::~CDataHandler(void)
{
}


/**********************************************************************************************//**
 * @fn	CParaData* CDataHandler::PARADATA()
 *
 * @brief	Get the instance of para data structure ventilation mode (singleton).
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @return	null if it fails, else a CParaData*.
 **************************************************************************************************/
CParaData* CDataHandler::PARADATA()
{ 
	if(m_PARADATA==NULL)
	{
		m_PARADATA=CParaData::GetInstance();
	}
	return m_PARADATA;
}
/**********************************************************************************************//**
 * @fn	CParaDataPRESET* CDataHandler::PRESET()
 *
 * @brief	Get the instance of para data structure preset mode (singleton).
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @return	null if it fails, else a CParaDataPRESET*.
 **************************************************************************************************/
CParaDataPRESET* CDataHandler::PRESET()
{ 
	if(m_PRESET==NULL)
	{
		m_PRESET=CParaDataPRESET::GetInstance();
	}
	return m_PRESET;
}


/**********************************************************************************************//**
 * @fn	CDataHandler* CDataHandler::getInstance()
 *
 * @brief	Get the instance of data handler (singleton).
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @return	null if it fails, else the instance.
 **************************************************************************************************/

CDataHandler* CDataHandler::getInstance()
{
	if(theDataHandler == 0)
	{
		theDataHandler = new CDataHandler;
	}
	return theDataHandler;
}

/**********************************************************************************************//**
 * @fn	void CDataHandler::destroyInstance()
 *
 * @brief	Destroys the instance of data handler (singleton).
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
void CDataHandler::destroyInstance()
{
	if(theDataHandler != NULL)
	{
		delete theDataHandler;
		theDataHandler = NULL;
	}
}

/**********************************************************************************************//**
 * @fn	void CDataHandler::setExit()
 *
 * @brief	Sets the exit, destruct and clean.
 * 			- deinitializes all critical sections
 *			- deletes all memory allocations
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @exception	std	Thrown when a standard error condition occurs.
 **************************************************************************************************/

void CDataHandler::setExit()
{
	m_bExit=true;

	saveOpTime();


	m_PARADATA->DestroyInstance();
	m_PRESET->DestroyInstance();


	EnterCriticalSection(&csSavedBreath);
	if (m_pbufSavedBreath != NULL) {
		delete [] m_pbufSavedBreath;
	}
	m_pbufSavedBreath=NULL;
	LeaveCriticalSection(&csSavedBreath);

	EnterCriticalSection(&csVentDataBuffer);
	m_rbufVent.~CircularBuffer();
	LeaveCriticalSection(&csVentDataBuffer);

	EnterCriticalSection(&csCopyDataBuffer);
	m_rbufCopy.~CircularBuffer();
	LeaveCriticalSection(&csCopyDataBuffer);

	EnterCriticalSection(&csSPIDataBuffer);
	m_rbufSPI.~CircularBuffer();
	LeaveCriticalSection(&csSPIDataBuffer);

	EnterCriticalSection(&csSPO2DataBuffer);
	m_rbufSPO2.~CircularBuffer();
	LeaveCriticalSection(&csSPO2DataBuffer);

	EnterCriticalSection(&csCO2DataBuffer);
	m_rbufCO2.~CircularBuffer();
	LeaveCriticalSection(&csCO2DataBuffer);

	delete [] m_pbufNumericIPPV;
	m_pbufNumericIPPV=NULL;
	delete [] m_pbufNumericSIPPV;
	m_pbufNumericSIPPV=NULL;
	delete [] m_pbufNumericSIMV;
	m_pbufNumericSIMV=NULL;
	delete [] m_pbufNumericSIMVPSV;
	m_pbufNumericSIMVPSV=NULL;
	delete [] m_pbufNumericPSV;
	m_pbufNumericPSV=NULL;
	delete [] m_pbufNumericCPAP;
	m_pbufNumericCPAP=NULL;
	delete [] m_pbufNumericHFO;
	m_pbufNumericHFO=NULL;
	delete [] m_pbufNumericNCPAP;
	m_pbufNumericNCPAP=NULL;
	delete [] m_pbufNumericDUOPAP;
	m_pbufNumericDUOPAP=NULL;
	delete [] m_pbufNumericTHERAPY;
	m_pbufNumericTHERAPY=NULL;
	delete [] m_pbufNumericFLOWOFFCONV;
	m_pbufNumericFLOWOFFCONV=NULL;
	delete [] m_pbufNumericFLOWOFFCPAP;
	m_pbufNumericFLOWOFFCPAP=NULL;
	delete [] m_pbufNumericFLOWOFFHFO;
	m_pbufNumericFLOWOFFHFO=NULL;

	delete [] m_pbufMessureAVG;
	m_pbufMessureAVG=NULL;
	delete [] m_pbufMessureBTB;
	m_pbufMessureBTB=NULL;

	DeleteCriticalSection(&csOpTime);
	DeleteCriticalSection(&csSPIDataBuffer);
	DeleteCriticalSection(&csCopyDataBuffer);
	DeleteCriticalSection(&csVentDataBuffer);
	DeleteCriticalSection(&csCO2DataBuffer);
	DeleteCriticalSection(&csSPO2DataBuffer);
	DeleteCriticalSection(&csMainboardData);
	DeleteCriticalSection(&csMessureDataAVG);
	DeleteCriticalSection(&csMessureDataBTB);
	DeleteCriticalSection(&csMessureData);
	DeleteCriticalSection(&csAlarmStateBytes);
	DeleteCriticalSection(&csFlowsensorStateBytes);
	DeleteCriticalSection(&csFlowsensorState);
	DeleteCriticalSection(&csOxyCal);
	DeleteCriticalSection(&csOxyState);
	DeleteCriticalSection(&csTrendFileData);
	DeleteCriticalSection(&csTrend);
	DeleteCriticalSection(&csTrendUpdate);
	DeleteCriticalSection(&csDelTrendThread);
	DeleteCriticalSection(&csSavedBreath);
	DeleteCriticalSection(&csFOTosciState);
}


/**********************************************************************************************//**
 * @fn	CMVModel *CDataHandler::getModel()
 *
 * @brief	Get the instance of the model (singleton).
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @return	null if it fails, else the model.
 **************************************************************************************************/
CMVModel *CDataHandler::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * @fn	int CDataHandler::getSizeSavedBreath()
 *
 * @brief	Gets size saved breath.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @return	The size saved breath.
 **************************************************************************************************/
int CDataHandler::getSizeSavedBreath()
{
	EnterCriticalSection(&csSavedBreath);
	int iSize=m_iSizeSavedBreath;
	LeaveCriticalSection(&csSavedBreath);
	return iSize;
}

/**********************************************************************************************//**
 * @fn	void CDataHandler::setSizeSavedBreath(int iSize)
 *
 * @brief	Sets size saved breath.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @param	iSize	Zero-based index of the size.
 **************************************************************************************************/
void CDataHandler::setSizeSavedBreath(int iSize)
{
	EnterCriticalSection(&csSavedBreath);
	m_iSizeSavedBreath=iSize;
	LeaveCriticalSection(&csSavedBreath);
}

/**********************************************************************************************//**
 * @fn	void CDataHandler::init()
 *
 * @brief	Initializes the data handler.
 *
 *   This function initializes the members of the data handler and:
 *   - initializes the operating times
 *   - reads the licensing
 *   - serializes the trend data from FFSDISK
 *   - load the configuration
 *   - load the numeric configuration
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
void CDataHandler::init()
{
	m_szLicenseFile=_T("AL");

	if(CTlsRegistry::IsValid(_T("HKLM\\Drivers\\Display\\PXA27x\\Mode20")))
	{
		CTlsRegistry regDisplay(_T("HKLM\\Drivers\\Display\\PXA27x\\Mode20"),false);
		if(regDisplay.ReadString(_T("name"), _T("G104SN02NDCU11LED"))==_T("G104SN02NDCU11LED"))
		{
			m_bLEDdisplay=true;

			m_iContrastParm=GetContrast();

			if(m_iContrastParm<103 || m_iContrastParm>1023)
			{
				IncreaseMaxContrast();
			}
		}
		else if(regDisplay.ReadString(_T("name"), _T("LQ104SNDCU11LCD"))==_T("LQ104SNDCU11LCD"))
		{
			m_bLEDdisplay=false;
		}
		else if(regDisplay.ReadString(_T("name"), _T("G104SN02"))==_T("G104SN02"))//old config
		{
			m_bLEDdisplay=true;

			m_iContrastParm=GetContrast();

			if(m_iContrastParm<103 || m_iContrastParm>1023)
			{
				IncreaseMaxContrast();
			}
		}
		else
		{
			m_bLEDdisplay=false;
		}
	}
	else if(CTlsRegistry::IsValid(_T("HKLM\\Drivers\\Display\\SMIVGX\\Mode20")))
	{
		CTlsRegistry regDisplay(_T("HKLM\\Drivers\\Display\\SMIVGX\\Mode20"),false);
		if(regDisplay.ReadString(_T("name"), _T("G104SN02NDCU9LED"))==_T("G104SN02NDCU9LED"))
		{
			m_bLEDdisplay=true;

			m_iContrastParm=GetContrast();

			if(m_iContrastParm<103 || m_iContrastParm>1023)
			{
				IncreaseMaxContrast();
			}
		}
		else if(regDisplay.ReadString(_T("name"), _T("LQ104S1NDCU9LCD"))==_T("LQ104S1NDCU9LCD"))
		{
			m_bLEDdisplay=false;
		}
		else
		{
			m_bLEDdisplay=false;
		}
	}
	else if(CTlsRegistry::IsValid(_T("HKLM\\Drivers\\Display\\SMIVGX\\Mode1")))
	{
		m_bLEDdisplay=false;
	}
	else
	{
		m_bLEDdisplay=false;
	}

	//test reset demo timestamp
	/*COleDateTime dtCurrentTime;
	dtCurrentTime.SetStatus(COleDateTime::null);
	getModel()->getCONFIG()->SetFOTdemoTimestamp(dtCurrentTime);*/
			
	/*COleDateTime dtCurrentTime(2014,11,7,0,0,0);
	getModel()->getCONFIG()->SetFOTdemoTimestamp(dtCurrentTime);

	getModel()->getCONFIG()->SetPRICOdemoTimestamp(dtCurrentTime);

	COleDateTime dtTime=getModel()->getCONFIG()->GetPRICOdemoTimestamp();
	if(dtTime.GetStatus()==COleDateTime::valid)
	{
		int iDiffYear=dtTime.GetYear();
		int iDiffMonth=dtTime.GetMonth();
		int iDiffDay=dtTime.GetDay();

		int iStop=0;
	}*/
	

	checkLicensing();

	if(!m_bVLIMITLicenseAvailable && getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
	{
		if(m_bVGUARANTLicenseAvailable)
			getModel()->getCONFIG()->SetVolumeControl(VCTRL_VGARANT);
	}

	if(!m_bVGUARANTLicenseAvailable && getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
	{
		if(m_bVLIMITLicenseAvailable)
			getModel()->getCONFIG()->SetVolumeControl(VCTRL_VLIMIT);
	}

	//initTrend();

	loadIniConfiguration();
	loadDiagrammColor();
	loadNumerics();
	loadConfig();

	if(IsCurrentModeVGarantStateOn()==true)
	{
		m_eCurVgarantState=VOLUMEGAR_ON;
	}
	else
	{
		m_eCurVgarantState=VOLUMEGAR_OFF;
	}

	checkVentRangeSettings();

	m_iContrastParm=GetContrast();

	m_iO2Difference=getModel()->getCONFIG()->GetO2Difference();
	m_iPmeanDifference=getModel()->getCONFIG()->GetPmeanDifference();
	m_iPEEP_PPSVdifference=getModel()->getCONFIG()->getPEEP_PpsvDifference();

	m_dwOpTimeDevTickCountStarted=GetTickCount();
	m_dwOpTimeBattTickCountStarted=0;
	if(IsHFOrunning())
		m_dwOpTimeHFOTickCountStarted=m_dwOpTimeDevTickCountStarted;
	else
	{
		m_dwOpTimeHFOTickCountStarted=0;
	}

	checkTriggerTubeDependency();
}

/**********************************************************************************************//**
 * @fn	void CDataHandler::checkVentRangeSettings()
 *
 * @brief	Check vent range settings.
 * 			This function checks the settings against the limits.
 *
 * @author	Rainer
 * @date	14.03.2016
 **************************************************************************************************/
void CDataHandler::checkVentRangeSettings()//EFLOW_CHECK
{
	//check E-Flow
	WORD iLowerLimit=0;
	WORD iUpperLimit=0;
	if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
	{
		iLowerLimit=PARADATA()->GetEFLOWMinPara_IPPV();
		iUpperLimit=PARADATA()->GetEFLOWMaxPara_IPPV();
	}
	else
	{
		iLowerLimit=PARADATA()->GetEFLOWMinPara_TRIGGER();
		iUpperLimit=PARADATA()->GetEFLOWMaxPara_TRIGGER();
	}
	WORD iValue=PARADATA()->GetEFLOWPara_TRIGGER();
	if(iValue>iUpperLimit)
	{
		SetEFlowParadata_TRIGGER(iUpperLimit,false);
	}
	else if(iValue<iLowerLimit)
	{
		SetEFlowParadata_TRIGGER(iLowerLimit,false);
	}
}

/**********************************************************************************************//**
 * @fn	void CDataHandler::initTrend()
 *
 * @brief	Initializes the trend.
 * 			This function deserializes the trend data if license is available
 *
 * @author	Rainer
 * @date	14.03.2016
 **************************************************************************************************/
void CDataHandler::initTrend()
{
	CString sFile=_T("");
	bool bRes=true;
	CString szPath=_T("\\sdcard\\");
	if(CTlsFile::Exists(szPath)==false)
	{
		szPath=_T("\\FFSDISK\\");
	}

	if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER))
	{
		bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER);

		if(!bRes)
		{
			sFile+=_T("TREND ");
		}
	}
	if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_PINSP) && bRes)
	{
		bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_PINSP);

		if(!bRes)
		{
			sFile+=_T("PINSP ");
		}
	}

	//TEST
	/*getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_PINSP, 2);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_PMEAN, 2);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_FIO2, 2);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_VTE, 2);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_COMPLIANCE, 2);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_CO2HFO, 2);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_MV, 2);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_HFAMP, 2);*/


	bool bDeserialize=true;
	
	//TREND
	if(isTRENDLicenseAvailable())
	{
		EnterCriticalSection(&csTrend);

		if(bRes && bDeserialize)
		{
			m_chFilenumPINSP=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_PINSP);


			if(m_chFilenumPINSP>0)
			{
				bDeserialize=DeserializeTrend(TREND_PINSP, m_chFilenumPINSP);
			}
			else
				m_chFilenumPINSP=1;
		}
		//##################################################################
		if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_PMEAN) && bRes)
		{
			bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_PMEAN);
			if(!bRes)
			{
				sFile+=_T("PMEAN ");
			}
		}
		if(bRes && bDeserialize)
		{
			m_chFilenumPMEAN=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_PMEAN);
			if(m_chFilenumPMEAN>0)
			{
				bDeserialize=DeserializeTrend(TREND_PMEAN, m_chFilenumPMEAN);
			}
			else
				m_chFilenumPMEAN=1;
		}
		//##################################################################
		if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_FIO2) && bRes)
		{
			bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_FIO2);
			if(!bRes)
			{
				sFile+=_T("FIO2 ");
			}
		}
		if(bRes && bDeserialize)
		{
			m_chFilenumFIO2=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_FIO2);
			if(m_chFilenumFIO2>0)
			{
				bDeserialize=DeserializeTrend(TREND_FIO2, m_chFilenumFIO2);
			}
			else
				m_chFilenumFIO2=1;
		}
		//##################################################################
		if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_VTE) && bRes)
		{
			bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_VTE);
			if(!bRes)
			{
				sFile+=_T("VTE ");
			}
		}
		if(bRes && bDeserialize)
		{
			m_chFilenumVTE=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_VTE);
			if(m_chFilenumVTE>0)
			{
				bDeserialize=DeserializeTrend(TREND_VTE, m_chFilenumVTE);
			}
			else
				m_chFilenumVTE=1;
		}
		//##################################################################
		if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_COMPLIANCE) && bRes)
		{
			bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_COMPLIANCE);
			if(!bRes)
			{
				sFile+=_T("COMPLIANCE ");
			}
		}
		if(bRes && bDeserialize)
		{
			m_chFilenumCOMPLIANCE=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_COMPLIANCE);
			if(m_chFilenumCOMPLIANCE>0)
			{
				bDeserialize=DeserializeTrend(TREND_COMPLIANCE, m_chFilenumCOMPLIANCE);
			}
			else
				m_chFilenumCOMPLIANCE=1;
		}
		//##################################################################
		if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_CO2HFO) && bRes)
		{
			bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_CO2HFO);
			if(!bRes)
			{
				sFile+=_T("CO2HFO ");
			}
		}
		if(bRes && bDeserialize)
		{
			m_chFilenumCO2HFO=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_CO2HFO);
			if(m_chFilenumCO2HFO>0)
			{
				bDeserialize=DeserializeTrend(TREND_CO2HFO, m_chFilenumCO2HFO);
			}
			else
				m_chFilenumCO2HFO=1;
		}
		//##################################################################
		if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_MV) && bRes)
		{
			bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_MV);
			if(!bRes)
			{
				sFile+=_T("MV ");
			}
		}
		if(bRes && bDeserialize)
		{
			m_chFilenumMV=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_MV);
			if(m_chFilenumMV>0)
			{
				bDeserialize=DeserializeTrend(TREND_MV, m_chFilenumMV);
			}
			else
				m_chFilenumMV=1;
		}
		//##################################################################
		if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_HFAMP) && bRes)
		{
			bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_HFAMP);
			if(!bRes)
			{
				sFile+=_T("HFAMP ");
			}
		}
		if(bRes && bDeserialize)
		{
			m_chFilenumHFAMP=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_HFAMP);
			if(m_chFilenumHFAMP>0)
			{
				bDeserialize=DeserializeTrend(TREND_HFAMP, m_chFilenumHFAMP);
			}
			else
				m_chFilenumHFAMP=1;
		}
		//##################################################################
		if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_RSBI) && bRes)
		{
			bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_RSBI);
			if(!bRes)
			{
				sFile+=_T("RSBI ");
			}
		}
		if(bRes && bDeserialize)
		{
			m_chFilenumRSBI=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_RSBI);
			if(m_chFilenumRSBI>0)
			{
				bDeserialize=DeserializeTrend(TREND_RSBI, m_chFilenumRSBI);
			}
			else
				m_chFilenumRSBI=1;
		}
		//##################################################################
		if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_SHAREMVMAND) && bRes)
		{
			bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_SHAREMVMAND);
			if(!bRes)
			{
				sFile+=_T("SHAREMVMAND ");
			}
		}
		if(bRes && bDeserialize)
		{
			m_chFilenumShareMVmand=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_SHAREMVMAND);
			if(m_chFilenumShareMVmand>0)
			{
				bDeserialize=DeserializeTrend(TREND_SHAREMVMAND, m_chFilenumShareMVmand);
			}
			else
				m_chFilenumShareMVmand=1;
		}
		//##################################################################
		if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_RESISTANCE) && bRes)
		{
			bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_RESISTANCE);
			if(!bRes)
			{
				sFile+=_T("RESISTANCE ");
			}
		}
		if(bRes && bDeserialize)
		{
			m_chFilenumResistance=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_RESISTANCE);
			if(m_chFilenumResistance>0)
			{
				bDeserialize=DeserializeTrend(TREND_RESISTANCE, m_chFilenumResistance);
			}
			else
				m_chFilenumResistance=1;
		}
		//##################################################################
		if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_LEAK) && bRes)
		{
			bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_LEAK);
			if(!bRes)
			{
				sFile+=_T("LEAK ");
			}
		}
		if(bRes && bDeserialize)
		{
			m_chFilenumLeak=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_LEAK);
			if(m_chFilenumLeak>0)
			{
				bDeserialize=DeserializeTrend(TREND_LEAK, m_chFilenumLeak);
			}
			else
				m_chFilenumLeak=1;
		}
		//##################################################################
		if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_SPO2) && bRes)
		{
			bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_SPO2);
			if(!bRes)
			{
				sFile+=_T("SPO2 ");
			}
		}
		if(bRes && bDeserialize)
		{
			m_chFilenumSpO2=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_SPO2);
			if(m_chFilenumSpO2>0)
			{
				bDeserialize=DeserializeTrend(TREND_SPO2, m_chFilenumSpO2);
			}
			else
				m_chFilenumSpO2=1;
		}
		//##################################################################
		if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_SPO2PI) && bRes)
		{
			bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_SPO2PI);
			if(!bRes)
			{
				sFile+=_T("SPO2PI ");
			}
		}
		if(bRes && bDeserialize)
		{
			m_chFilenumSPO2PI=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_SPO2PI);
			if(m_chFilenumSPO2PI>0)
			{
				bDeserialize=DeserializeTrend(TREND_SPO2PI, m_chFilenumSPO2PI);
			}
			else
				m_chFilenumSPO2PI=1;
		}
		//##################################################################
		if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_ETCO2) && bRes)
		{
			bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_ETCO2);
			if(!bRes)
			{
				sFile+=_T("ETCO2 ");
			}
		}
		if(bRes && bDeserialize)
		{
			m_chFilenumEtCO2=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_ETCO2);
			if(m_chFilenumEtCO2>0)
			{
				bDeserialize=DeserializeTrend(TREND_ETCO2, m_chFilenumEtCO2);
			}
			else
				m_chFilenumEtCO2=1;
		}
		//##################################################################
		if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_SPO2PR) && bRes)
		{
			bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_SPO2PR);
			if(!bRes)
			{
				sFile+=_T("SPO2PR ");
			}
		}
		if(bRes && bDeserialize)
		{
			m_chFilenumSpO2PR=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_SPO2PR);
			if(m_chFilenumSpO2PR>0)
			{
				bDeserialize=DeserializeTrend(TREND_SPO2PR, m_chFilenumSpO2PR);
			}
			else
				m_chFilenumSpO2PR=1;
		}
		//##################################################################
		if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_FREQUENCY) && bRes)
		{
			bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_FREQUENCY);
			if(!bRes)
			{
				sFile+=_T("SPO2PR ");
			}
		}
		if(bRes && bDeserialize)
		{
			m_chFilenumFrequency=getModel()->getCONFIG()->GetLastWrittenTrendFile(TREND_FREQUENCY);
			if(m_chFilenumFrequency>0)
			{
				bDeserialize=DeserializeTrend(TREND_FREQUENCY, m_chFilenumFrequency);
			}
			else
				m_chFilenumFrequency=1;
		}
		LeaveCriticalSection(&csTrend);

		if(!bRes)
		{
			//TREND ERROR
			CStringW szError=_T("#HFO:0056: ");
			szError+=sFile;
			theApp.getLog()->WriteLine(szError);
		}
		
		if(bDeserialize==false)
		{
			theApp.getLog()->WriteLine(_T("#HFO:0057"));
			DeleteAllTrendData();
		}
		else if(m_chFilenumPINSP!=m_chFilenumPMEAN
			||	m_chFilenumPINSP!=m_chFilenumFIO2
			||	m_chFilenumPINSP!=m_chFilenumVTE
			||	m_chFilenumPINSP!=m_chFilenumCOMPLIANCE
			||	m_chFilenumPINSP!=m_chFilenumCO2HFO
			||	m_chFilenumPINSP!=m_chFilenumMV
			||	m_chFilenumPINSP!=m_chFilenumHFAMP
			||	m_chFilenumPINSP!=m_chFilenumRSBI
			||	m_chFilenumPINSP!=m_chFilenumShareMVmand
			||	m_chFilenumPINSP!=m_chFilenumResistance
			||	m_chFilenumPINSP!=m_chFilenumLeak
			||	m_chFilenumPINSP!=m_chFilenumSpO2
			||	m_chFilenumPINSP!=m_chFilenumSPO2PI
			||	m_chFilenumPINSP!=m_chFilenumEtCO2
			||	m_chFilenumPINSP!=m_chFilenumSpO2PR
			||	m_chFilenumPINSP!=m_chFilenumFrequency)
		{
			CStringW sz=_T("");
			sz.Format(_T("#HFO:0058 PINSP:%d PMEAN:%d FIO2:%d VTE:%d COMPLIANCE:%d CO2HFO2:%d MV:%d HFAMP:%d RSBI:%d ShareMVmand:%d Resistance:%d Leak:%d SpO2:%d SPO2PI:%d EtCO2:%d SpO2PR:%d FREQ:%d"),
				m_chFilenumPINSP,
				m_chFilenumPMEAN,
				m_chFilenumFIO2,
				m_chFilenumVTE,
				m_chFilenumCOMPLIANCE,
				m_chFilenumCO2HFO,
				m_chFilenumMV,
				m_chFilenumHFAMP,
				m_chFilenumRSBI,
				m_chFilenumShareMVmand,
				m_chFilenumResistance,
				m_chFilenumLeak,
				m_chFilenumSpO2,
				m_chFilenumSPO2PI,
				m_chFilenumEtCO2,
				m_chFilenumSpO2PR,
				m_chFilenumFrequency);

			theApp.getLog()->WriteLine(sz);
			DeleteAllTrendData();
		}
	}

	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_DEL_TRENDDATA);
}

/**********************************************************************************************//**
 * @fn	bool CDataHandler::checkLicensing()
 *
 * @brief	Checks the licensing data.
 * 			
 * 			This function read the licensing and enables/disables  the modules.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @return	false if no unique ID, else true
 **************************************************************************************************/
bool CDataHandler::checkLicensing()
{
	if(getModel()->GetUniqueID()==_T("-ERROR-"))
	{
		return false;
	}

	CStringA szParsedMasterOldKey="";
	CStringA szParsedHFOKey="";
	CStringA szParsedNMODEKey="";
	CStringA szParsedVGUARANTYKey="";
	CStringA szParsedVLIMITKey="";
	CStringA szParsedLUNGRECKey="";
	CStringA szParsedTRENDKey="";
	CStringA szParsedTHERAPYKey="";
	CStringA szParsedPRICOKey="";
	CStringA szParsedFOTKey="";
	CStringA szParsedNIVTRIGGERKey="";

	bool bCheckVGUARANTFeature=false;
	bool bCheckVLIMITFeature=false;
	bool bCheckHFOFeature=false;
	bool bCheckNMODEFeature=false;
	bool bCheckLUNGRECFeature=false;
	bool bCheckTRENDFeature=false;
	bool bCheckTHERAPYFeature=false;
	bool bCheckPRICOFeature=false;
	bool bCheckFOTFeature=false;
	bool bCheckNIVTRIGGERFeature=false;

	bool bOldMasterFeature=false;
	bool bOldMasterKey=false;

	m_szLicenseFile+=getModel()->GetUniqueID();
	m_szLicenseFile+=_T(".alic");

	CStringW szFile=_T("\\FFSDISK\\")+m_szLicenseFile;

	if(CTlsFile::Exists(szFile))
	{
		DWORD dwAttrs;

		dwAttrs = GetFileAttributes(szFile); 
		if ((dwAttrs & FILE_ATTRIBUTE_READONLY)) 
		{ 
			SetFileAttributes(szFile, 
				dwAttrs & ~FILE_ATTRIBUTE_READONLY); 
		} 


		CTlsIStream_File is(szFile);
		CTlsXmlScanner scanner(&is);

		// +++ skip start +++
		if(scanner.NextToken() == CTlsXmlScanner::TOK_TYPESTART)
		{
			while(scanner.NextToken() != CTlsXmlScanner::TOK_TYPEEND)
				; // Skip
		}

		if(CString(_T("FabianHFO")).CompareNoCase(CString(scanner.CurrText())) == 0)
		{
			scanner.NextToken();

			scanner.NextToken();

			// +++ Worker-Parameter +++
			CTlsXmlScanner::eTokenType Type;
			CString sKey;
			CString sValue;

			CString sModuleName=_T("");
			CString sModuleKey=_T("");

			while(true)
			{
				if(!scanner.Parse(scanner, Type, sKey, sValue))
				{
					//return false;
					break;
				}

				if(Type == CTlsXmlScanner::TT_BS)
				{
					if(sKey == "license")
					{
						while(true)
						{
							sModuleName=_T("");
							sModuleKey=_T("");

							if(!scanner.Parse(scanner, Type, sKey, sValue))
							{
								//return false;
								break;
							}

							if(Type == CTlsXmlScanner::TT_ATT)
							{
								if(sKey == "module")
								{
									sModuleName=sValue;
								}
								else
								{
									CString sTemp = _T("Falsche Option: ");
									sTemp += sKey;
									sTemp += " ";
									sTemp += sValue;
									break;
								}
							}
							else if(Type == CTlsXmlScanner::TT_BS)
							{
								break;
							}
							else if(Type == CTlsXmlScanner::TT_ES)
							{
								break;
							}

							if(!scanner.Parse(scanner, Type, sKey, sValue))
							{
								break;
							}

							if(Type == CTlsXmlScanner::TT_ATT)
							{
								if(sKey == "key")
								{
									sModuleKey=sValue;
								}
								else
								{
									CString sTemp = _T("Falsche Option: ");
									sTemp += sKey;
									sTemp += " ";
									sTemp += sValue;
									break;
								}
							}
							else if(Type == CTlsXmlScanner::TT_BS)
							{
								break;
							}
							else if(Type == CTlsXmlScanner::TT_ES)
							{
								break;
							}

							if(sModuleName!=_T("") && sModuleKey!=_T(""))
							{
								if(sModuleName==_T("MASTER"))
								{
									bOldMasterFeature=true;
									szParsedMasterOldKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bOldMasterKey=true;
									}
								}
								else if(sModuleName==_T("HFO"))
								{
									szParsedHFOKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckHFOFeature=true;
									}
								}
								else if(sModuleName==_T("CO2"))
								{
									//old, do nothing here
								}
								else if(sModuleName==_T("NMODE"))
								{
									szParsedNMODEKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckNMODEFeature=true;
									}
								}
								else if(sModuleName==_T("VGUARANTY"))
								{
									szParsedVGUARANTYKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckVGUARANTFeature=true;
									}
								}
								else if(sModuleName==_T("VLIMIT"))
								{
									szParsedVLIMITKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckVLIMITFeature=true;
									}
								}
								else if(sModuleName==_T("LUNGREC"))
								{
									szParsedLUNGRECKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckLUNGRECFeature=true;
									}
								}
								else if(sModuleName==_T("TRENDS"))
								{
									szParsedTRENDKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckTRENDFeature=true;
									}
								}
								else if(sModuleName==_T("THERAPY"))
								{
									szParsedTHERAPYKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckTHERAPYFeature=true;
									}
								}
								else if(sModuleName==_T("PRICO"))
								{
									szParsedPRICOKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckPRICOFeature=true;
									}
								}
								else if(sModuleName==_T("FOT"))
								{
									szParsedFOTKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckFOTFeature=true;
									}
								}
								else if(sModuleName==_T("NIVTRIGGER"))
								{
									szParsedNIVTRIGGERKey=sModuleKey;
									if(sModuleKey!=_T("0"))
									{
										bCheckNIVTRIGGERFeature=true;
									}
								}
							}
						}
					}
					else 
					{
						break;
					}
				}
				else if(Type == CTlsXmlScanner::TT_ES)
					break;
				else
				{
					break;
				}
			}
		}
	}
	else
	{
		m_bVGUARANTLicenseAvailable=false;
		m_bVLIMITLicenseAvailable=false;
		m_bHFOLicenseAvailable=false;
		m_bNMODELicenseAvailable=false;
		m_bLUNGRECLicenseAvailable=false;
		m_bTRENDLicenseAvailable=false;
		m_bTHERAPYLicenseAvailable=false;
		m_bPRICOLicenseAvailable=false;
		m_bFOTLicenseAvailable=false;
		m_bNIVTRIGGERLicenseAvailable=false;

		theApp.getLog()->WriteLine(_T("***allmodulesdisabled***"));
	}

	CStringA szUniqueID16=getModel()->GetUniqueID();
	int iLenUniqueID=szUniqueID16.GetLength();

	if(iLenUniqueID>16)
	{
		theApp.getLog()->WriteLine(_T("#HFO:0059"));
	}

	for(iLenUniqueID;iLenUniqueID<16;iLenUniqueID++)
	{
		szUniqueID16.AppendChar('0');
	}

	//old master key available
	if(bOldMasterFeature && bOldMasterKey)
	{
		if(szParsedMasterOldKey==encryptKey(MOD_MASTER))
		{
			enableHFOLicense();
			enableNMODELicense();
			enableVGUARANTLicense();
			enableVLIMITLicense();
			enableLUNGRECLicense();
			enableTRENDLicense();
			enableTHERAPYLicense();

			disablePRICOLicense();
			disableFOTLicense();
			disableNIVTRIGGERLicense();
		}
	}
	else
	{
		if(bCheckHFOFeature)
		{
			//check key
			if(szParsedHFOKey==encryptKey(MOD_HFO))
			{
				enableHFOLicense();
				delDemoLicense(MOD_HFO,true);
			}
			else if(szParsedHFOKey==encryptDEMOKey(MOD_HFO))
			{
				checkDemoLicense(MOD_HFO);
			}
		}
		if(bCheckNMODEFeature)
		{
			//check key
			if(szParsedNMODEKey==encryptKey(MOD_NMODE))
			{
				enableNMODELicense();
				delDemoLicense(MOD_NMODE,true);
			}
			else if(szParsedNMODEKey==encryptDEMOKey(MOD_NMODE))
			{
				checkDemoLicense(MOD_NMODE);
			}
		}
		if(bCheckVGUARANTFeature)
		{
			//check key
			if(szParsedVGUARANTYKey==encryptKey(MOD_VGUARANTY))
			{
				enableVGUARANTLicense();
				delDemoLicense(MOD_VGUARANTY,true);
			}
			else if(szParsedVGUARANTYKey==encryptDEMOKey(MOD_VGUARANTY))
			{
				checkDemoLicense(MOD_VGUARANTY);
			}
		}
		if(bCheckVLIMITFeature)
		{
			//check key
			if(szParsedVLIMITKey==encryptKey(MOD_VLIMIT))
			{
				enableVLIMITLicense();
				delDemoLicense(MOD_VLIMIT,true);
			}
			else if(szParsedVLIMITKey==encryptDEMOKey(MOD_VLIMIT))
			{
				checkDemoLicense(MOD_VLIMIT);
			}
		}
		if(bCheckLUNGRECFeature)
		{
			//check key
			if(szParsedLUNGRECKey==encryptKey(MOD_LUNGREC))
			{
				enableLUNGRECLicense();
				delDemoLicense(MOD_LUNGREC,true);
			}
			else if(szParsedLUNGRECKey==encryptDEMOKey(MOD_LUNGREC))
			{
				checkDemoLicense(MOD_LUNGREC);
			}
		}
		if(bCheckTRENDFeature)
		{
			//check key
			if(szParsedTRENDKey==encryptKey(MOD_TREND))
			{
				enableTRENDLicense();
				delDemoLicense(MOD_TREND,true);
			}
			else if(szParsedTRENDKey==encryptDEMOKey(MOD_TREND))
			{
				checkDemoLicense(MOD_TREND);
			}
		}
		if(bCheckTHERAPYFeature)
		{
			//check key
			if(szParsedTHERAPYKey==encryptKey(MOD_THERAPY))
			{
				enableTHERAPYLicense();
				delDemoLicense(MOD_THERAPY,true);
			}
			else if(szParsedTHERAPYKey==encryptDEMOKey(MOD_THERAPY))
			{
				checkDemoLicense(MOD_THERAPY);
			}
		}
		if(getModel()->getCONFIG()->isSPO2REGISTRYenabled()==true)
		{
			if(bCheckPRICOFeature)
			{
				//check key
				if(szParsedPRICOKey==encryptKey(MOD_PRICO))
				{
					enablePRICOLicense();
					delDemoLicense(MOD_PRICO,true);
				}
				else if(szParsedPRICOKey==encryptDEMOKey(MOD_PRICO))
				{
					checkDemoLicense(MOD_PRICO);
				}
			}
		}
		if(bCheckFOTFeature)
		{
			//check key
			if(szParsedFOTKey==encryptKey(MOD_FOT))
			{
				enableFOTLicense();
				delDemoLicense(MOD_FOT,true);
			}
			else if(szParsedFOTKey==encryptDEMOKey(MOD_FOT))
			{
				checkDemoLicense(MOD_FOT);
			}
		}
		if(bCheckNIVTRIGGERFeature)
		{
			//check key
			if(szParsedNIVTRIGGERKey==encryptKey(MOD_NIVTRIGGER))
			{
				enableNIVTRIGGERLicense();
				delDemoLicense(MOD_NIVTRIGGER,true);
			}
			else if(szParsedNIVTRIGGERKey==encryptDEMOKey(MOD_NIVTRIGGER))
			{
				checkDemoLicense(MOD_NIVTRIGGER);
			}
		}
		else
		{
			getModel()->getCONFIG()->disableNIVTRIGGER();
		}
	}

	if(bOldMasterFeature)
		WriteLicenseFile();

	return true;
}


COleDateTime CDataHandler::GetdemoTimestamp(eModule module)
{
	switch(module)
	{
	case MOD_HFO:
		{
			return getModel()->getCONFIG()->GetHFOdemoTimestamp();
		}
		break;
	case MOD_NMODE:
		{
			return getModel()->getCONFIG()->GetNMODEdemoTimestamp();
		}
		break;
	case MOD_VGUARANTY:
		{
			return getModel()->getCONFIG()->GetVGUARANTYdemoTimestamp();
		}
		break;
	case MOD_VLIMIT:
		{
			return getModel()->getCONFIG()->GetVLIMITdemoTimestamp();
		}
		break;
	case MOD_LUNGREC:
		{
			return getModel()->getCONFIG()->GetLUNGRECdemoTimestamp();
		}
		break;
	case MOD_TREND:
		{
			return getModel()->getCONFIG()->GetTRENDdemoTimestamp();
		}
		break;
	case MOD_THERAPY:
		{
			return getModel()->getCONFIG()->GetTHERAPYdemoTimestamp();
		}
		break;
	case MOD_PRICO:
		{
			return getModel()->getCONFIG()->GetPRICOdemoTimestamp();
		}
		break;
	case MOD_FOT:
		{
			return getModel()->getCONFIG()->GetFOTdemoTimestamp();
		}
		break;
	case MOD_NIVTRIGGER:
		{
			return getModel()->getCONFIG()->GetNIVTRIGGERdemoTimestamp();
		}
		break;
	default:
		{
			COleDateTime dtLicenseTimestamp;
			dtLicenseTimestamp.SetStatus(COleDateTime::null);
			return dtLicenseTimestamp;
		}
		break;
	}
}

void CDataHandler::checkDemoLicense(eModule module)
{
	switch(module)
	{
	case MOD_HFO:
		{
			DEBUGMSG(TRUE, (TEXT("checkDemoLicense MOD_HFO\r\n")));
			checkDemoLicense_HFO();
		}
		break;
	case MOD_NMODE:
		{
			DEBUGMSG(TRUE, (TEXT("checkDemoLicense MOD_NMODE\r\n")));
			checkDemoLicense_NMODE();
		}
		break;
	case MOD_VGUARANTY:
		{
			DEBUGMSG(TRUE, (TEXT("checkDemoLicense MOD_VGUARANTY\r\n")));
			checkDemoLicense_VGUARANTY();
		}
		break;
	case MOD_VLIMIT:
		{
			DEBUGMSG(TRUE, (TEXT("checkDemoLicense MOD_VLIMIT\r\n")));
			checkDemoLicense_VLIMIT();
		}
		break;
	case MOD_LUNGREC:
		{
			DEBUGMSG(TRUE, (TEXT("checkDemoLicense MOD_LUNGREC\r\n")));
			checkDemoLicense_LUNGREC();
		}
		break;
	case MOD_TREND:
		{
			DEBUGMSG(TRUE, (TEXT("checkDemoLicense MOD_TREND\r\n")));
			checkDemoLicense_TREND();
		}
		break;
	case MOD_THERAPY:
		{
			DEBUGMSG(TRUE, (TEXT("checkDemoLicense MOD_THERAPY\r\n")));
			checkDemoLicense_THERAPY();
		}
		break;
	case MOD_PRICO:
		{
			//DEBUGMSG(TRUE, (TEXT("checkDemoLicense MOD_PRICO\r\n")));
			checkDemoLicense_PRICO();
		}
		break;
	case MOD_FOT:
		{
			DEBUGMSG(TRUE, (TEXT("checkDemoLicense MOD_FOT\r\n")));
			checkDemoLicense_FOT();
		}
		break;
	case MOD_NIVTRIGGER:
		{
			DEBUGMSG(TRUE, (TEXT("checkDemoLicense MOD_NIVTRIGGER\r\n")));
			checkDemoLicense_NIVTRIGGER();
		}
		break;
	default:
		{

		}
		break;
	}
}

void CDataHandler::checkDemoLicense_HFO()
{
	COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetHFOdemoTimestamp();

	if(dtLicenseTimestamp.GetStatus() != COleDateTime::valid)//license timestamp not yet set
	{
		enableHFOLicense();
		setDemoLicense(MOD_HFO);
	}
	else//check elapsed time
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);

		COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

		if(dtElapsePeriod.GetTotalDays()>DEMOLICENSE)
		{
			//license time elapsed
			disableHFOLicense();
			delDemoLicense(MOD_HFO,false);
		} 
		else
		{
			enableHFOLicense();
			endableDemoLicense(MOD_HFO);
		}
	}
}
void CDataHandler::checkDemoLicense_NMODE()
{
	COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetNMODEdemoTimestamp();

	if(dtLicenseTimestamp.GetStatus() != COleDateTime::valid)//license timestamp not yet set
	{
		enableNMODELicense();
		setDemoLicense(MOD_NMODE);
	}
	else//check elapsed time
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);

		COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

		if(dtElapsePeriod.GetTotalDays()>DEMOLICENSE)
		{
			//license time elapsed
			disableNMODELicense();
			delDemoLicense(MOD_NMODE,false);
		} 
		else
		{
			enableNMODELicense();
			endableDemoLicense(MOD_NMODE);
		}
	}
}
void CDataHandler::checkDemoLicense_VGUARANTY()
{
	COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetVGUARANTYdemoTimestamp();

	if(dtLicenseTimestamp.GetStatus() != COleDateTime::valid)//license timestamp not yet set
	{
		enableVGUARANTLicense();
		setDemoLicense(MOD_VGUARANTY);
	}
	else//check elapsed time
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);

		COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

		if(dtElapsePeriod.GetTotalDays()>DEMOLICENSE)
		{
			//license time elapsed
			disableVGUARANTLicense();
			delDemoLicense(MOD_VGUARANTY,false);
		} 
		else
		{
			enableVGUARANTLicense();
			endableDemoLicense(MOD_VGUARANTY);
		}
	}
}
void CDataHandler::checkDemoLicense_VLIMIT()
{
	COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetVLIMITdemoTimestamp();

	if(dtLicenseTimestamp.GetStatus() != COleDateTime::valid)//license timestamp not yet set
	{
		enableVLIMITLicense();
		setDemoLicense(MOD_VLIMIT);
	}
	else//check elapsed time
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);

		COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

		if(dtElapsePeriod.GetTotalDays()>DEMOLICENSE)
		{
			//license time elapsed
			disableVLIMITLicense();
			delDemoLicense(MOD_VLIMIT,false);
		} 
		else
		{
			enableVLIMITLicense();
			endableDemoLicense(MOD_VLIMIT);
		}
	}
}
void CDataHandler::checkDemoLicense_LUNGREC()
{
	COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetLUNGRECdemoTimestamp();

	if(dtLicenseTimestamp.GetStatus() != COleDateTime::valid)//license timestamp not yet set
	{
		enableLUNGRECLicense();
		setDemoLicense(MOD_LUNGREC);
	}
	else//check elapsed time
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);

		COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

		if(dtElapsePeriod.GetTotalDays()>DEMOLICENSE)
		{
			//license time elapsed
			disableLUNGRECLicense();
			delDemoLicense(MOD_LUNGREC,false);
		} 
		else
		{
			enableLUNGRECLicense();
			endableDemoLicense(MOD_LUNGREC);
		}
	}
}
void CDataHandler::checkDemoLicense_TREND()
{
	COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetTRENDdemoTimestamp();

	if(dtLicenseTimestamp.GetStatus() != COleDateTime::valid)//license timestamp not yet set
	{
		enableTRENDLicense();
		setDemoLicense(MOD_TREND);
	}
	else//check elapsed time
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);

		COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

		if(dtElapsePeriod.GetTotalDays()>DEMOLICENSE)
		{
			//license time elapsed
			disableTRENDLicense();
			delDemoLicense(MOD_TREND,false);
		} 
		else
		{
			enableTRENDLicense();
			endableDemoLicense(MOD_TREND);
		}
	}
}
void CDataHandler::checkDemoLicense_THERAPY()
{
	COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetTHERAPYdemoTimestamp();

	if(dtLicenseTimestamp.GetStatus() != COleDateTime::valid)//license timestamp not yet set
	{
		enableTHERAPYLicense();
		setDemoLicense(MOD_THERAPY);
	}
	else//check elapsed time
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);

		COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

		if(dtElapsePeriod.GetTotalDays()>DEMOLICENSE)
		{
			//license time elapsed
			disableTHERAPYLicense();
			delDemoLicense(MOD_THERAPY,false);
		} 
		else
		{
			enableTHERAPYLicense();
			endableDemoLicense(MOD_THERAPY);
		}
	}
}
void CDataHandler::checkDemoLicense_PRICO()
{
	COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetPRICOdemoTimestamp();

	if(dtLicenseTimestamp.GetStatus() != COleDateTime::valid)//license timestamp not yet set
	{
		enablePRICOLicense();
		setDemoLicense(MOD_PRICO);
	}
	else//check elapsed time
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);

		COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

		if(dtElapsePeriod.GetTotalDays()>DEMOLICENSE)
		{
			//license time elapsed
			disablePRICOLicense();
			delDemoLicense(MOD_PRICO,false);
		} 
		else
		{
			enablePRICOLicense();
			endableDemoLicense(MOD_PRICO);
		}
	}
}
void CDataHandler::checkDemoLicense_FOT()
{
	COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetFOTdemoTimestamp();

	if(dtLicenseTimestamp.GetStatus() != COleDateTime::valid)//license timestamp not yet set
	{
		enableFOTLicense();
		setDemoLicense(MOD_FOT);
	}
	else//check elapsed time
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);

		//COleDateTimeSpan dtElapsePeriod;
		//dtElapsePeriod.SetDateTimeSpan(90,0,0,0);//90 days ~ 3 month

		COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

		if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
		{
			int iLicMonth=dtLicenseTimestamp.GetMonth();
			int iLicDay=dtLicenseTimestamp.GetDay();

			int iCurMonth=dtLicenseTimestamp.GetMonth();
			int iLCurDay=dtLicenseTimestamp.GetDay();

			int iStop=0;
		}
		int iTest=dtElapsePeriod.GetTotalDays();
		if(dtElapsePeriod.GetTotalDays()>DEMOLICENSE)
		///if (dtCurrentTime >= dtLicenseTimestamp - dtElapsePeriod)
		{
			//license time elapsed
			disableFOTLicense();
			delDemoLicense(MOD_FOT,false);
		} 
		else
		{
			enableFOTLicense();
			endableDemoLicense(MOD_FOT);
		}
	}
}

void CDataHandler::checkDemoLicense_NIVTRIGGER()
{
	COleDateTime dtLicenseTimestamp = getModel()->getCONFIG()->GetNIVTRIGGERdemoTimestamp();

	if(dtLicenseTimestamp.GetStatus() != COleDateTime::valid)//license timestamp not yet set
	{
		enableNIVTRIGGERLicense();
		setDemoLicense(MOD_NIVTRIGGER);
	}
	else//check elapsed time
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);

		//COleDateTimeSpan dtElapsePeriod;
		//dtElapsePeriod.SetDateTimeSpan(90,0,0,0);//90 days ~ 3 month

		COleDateTimeSpan dtElapsePeriod=dtCurrentTime-dtLicenseTimestamp;

		if(dtLicenseTimestamp.GetYear()==dtCurrentTime.GetYear())
		{
			int iLicMonth=dtLicenseTimestamp.GetMonth();
			int iLicDay=dtLicenseTimestamp.GetDay();

			int iCurMonth=dtLicenseTimestamp.GetMonth();
			int iLCurDay=dtLicenseTimestamp.GetDay();

			int iStop=0;
		}
		int iTest=dtElapsePeriod.GetTotalDays();
		if(dtElapsePeriod.GetTotalDays()>DEMOLICENSE)
			///if (dtCurrentTime >= dtLicenseTimestamp - dtElapsePeriod)
		{
			//license time elapsed
			disableNIVTRIGGERLicense();
			delDemoLicense(MOD_NIVTRIGGER,false);
		} 
		else
		{
			enableNIVTRIGGERLicense();
			endableDemoLicense(MOD_NIVTRIGGER);
		}
	}
}

void CDataHandler::setDemoLicense(eModule module)
{
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	COleDateTime dtCurrentTime(stTime);

	switch(module)
	{
	case MOD_HFO:
		{
			getModel()->getCONFIG()->SetHFOdemoTimestamp(dtCurrentTime);
			DEBUGMSG(TRUE, (TEXT("setDemoLicense MOD_HFO\r\n")));
		}
		break;
	case MOD_NMODE:
		{
			getModel()->getCONFIG()->SetNMODEdemoTimestamp(dtCurrentTime);
			DEBUGMSG(TRUE, (TEXT("setDemoLicense MOD_NMODE\r\n")));
		}
		break;
	case MOD_VGUARANTY:
		{
			getModel()->getCONFIG()->SetVGUARANTYdemoTimestamp(dtCurrentTime);
			DEBUGMSG(TRUE, (TEXT("setDemoLicense MOD_VGUARANTY\r\n")));
		}
		break;
	case MOD_VLIMIT:
		{
			getModel()->getCONFIG()->SetVLIMITdemoTimestamp(dtCurrentTime);
			DEBUGMSG(TRUE, (TEXT("setDemoLicense MOD_VLIMIT\r\n")));
		}
		break;
	case MOD_LUNGREC:
		{
			getModel()->getCONFIG()->SetLUNGRECdemoTimestamp(dtCurrentTime);
			DEBUGMSG(TRUE, (TEXT("setDemoLicense MOD_LUNGREC\r\n")));
		}
		break;
	case MOD_TREND:
		{
			getModel()->getCONFIG()->SetTRENDdemoTimestamp(dtCurrentTime);
			DEBUGMSG(TRUE, (TEXT("setDemoLicense MOD_TREND\r\n")));
		}
		break;
	case MOD_THERAPY:
		{
			getModel()->getCONFIG()->SetTHERAPYdemoTimestamp(dtCurrentTime);
			DEBUGMSG(TRUE, (TEXT("setDemoLicense MOD_THERAPY\r\n")));
		}
		break;
	case MOD_PRICO:
		{
			getModel()->getCONFIG()->SetPRICOdemoTimestamp(dtCurrentTime);
			DEBUGMSG(TRUE, (TEXT("setDemoLicense MOD_PRICO\r\n")));
		}
		break;
	case MOD_FOT:
		{
			getModel()->getCONFIG()->SetFOTdemoTimestamp(dtCurrentTime);
			DEBUGMSG(TRUE, (TEXT("setDemoLicense MOD_FOT\r\n")));
		}
		break;
	case MOD_NIVTRIGGER:
		{
			getModel()->getCONFIG()->SetNIVTRIGGERdemoTimestamp(dtCurrentTime);
			DEBUGMSG(TRUE, (TEXT("setDemoLicense MOD_NIVTRIGGER\r\n")));
		}
		break;
	default:
		{

		}
		break;
	}

	endableDemoLicense(module);
}
void CDataHandler::delDemoLicense(eModule module, bool bReset)
{
	
	COleDateTime dtLicenseTimestamp;//midnight, 30 December 1899
	dtLicenseTimestamp.SetStatus(COleDateTime::null);

	switch(module)
	{
	case MOD_HFO:
		{
			m_bHFODemoLicAvailable=false;
			if(bReset)
			{
				getModel()->getCONFIG()->SetHFOdemoTimestamp(dtLicenseTimestamp);
				//DEBUGMSG(TRUE, (TEXT("delDemoLicense MOD_HFO\r\n")));
			}
		}
		break;
	case MOD_NMODE:
		{
			m_bNMODEDemoLicAvailable=false;
			if(bReset)
			{
				getModel()->getCONFIG()->SetNMODEdemoTimestamp(dtLicenseTimestamp);
				//DEBUGMSG(TRUE, (TEXT("delDemoLicense MOD_NMODE\r\n")));
			}
		}
		break;
	case MOD_VGUARANTY:
		{
			m_bVGUARANTDemoLicAvailable=false;
			if(bReset)
			{
				getModel()->getCONFIG()->SetVGUARANTYdemoTimestamp(dtLicenseTimestamp);
				//DEBUGMSG(TRUE, (TEXT("delDemoLicense MOD_VGUARANTY\r\n")));
			}
		}
		break;
	case MOD_VLIMIT:
		{
			m_bVLIMITDemoLicAvailable=false;
			if(bReset)
			{
				getModel()->getCONFIG()->SetVLIMITdemoTimestamp(dtLicenseTimestamp);
				//DEBUGMSG(TRUE, (TEXT("delDemoLicense MOD_VLIMIT\r\n")));
			}
		}
		break;
	case MOD_LUNGREC:
		{
			m_bLUNGRECDemoLicAvailable=false;
			if(bReset)
			{
				getModel()->getCONFIG()->SetLUNGRECdemoTimestamp(dtLicenseTimestamp);
				//DEBUGMSG(TRUE, (TEXT("delDemoLicense MOD_LUNGREC\r\n")));
			}
		}
		break;
	case MOD_TREND:
		{
			m_bTRENDDemoLicAvailable=false;
			if(bReset)
			{
				getModel()->getCONFIG()->SetTRENDdemoTimestamp(dtLicenseTimestamp);
				//DEBUGMSG(TRUE, (TEXT("delDemoLicense MOD_TREND\r\n")));
			}
		}
		break;
	case MOD_THERAPY:
		{
			m_bTHERAPYDemoLicAvailable=false;
			if(bReset)
			{
				getModel()->getCONFIG()->SetTHERAPYdemoTimestamp(dtLicenseTimestamp);
				//DEBUGMSG(TRUE, (TEXT("delDemoLicense MOD_THERAPY\r\n")));
			}
		}
		break;
	case MOD_PRICO:
		{
			m_bPRICODemoLicAvailable=false;
			if(bReset)
			{
				getModel()->getCONFIG()->SetPRICOdemoTimestamp(dtLicenseTimestamp);
				//DEBUGMSG(TRUE, (TEXT("delDemoLicense MOD_PRICO\r\n")));
			}
		}
		break;
	case MOD_FOT:
		{
			m_bFOTDemoLicAvailable=false;
			if(bReset)
			{
				getModel()->getCONFIG()->SetFOTdemoTimestamp(dtLicenseTimestamp);
				//DEBUGMSG(TRUE, (TEXT("delDemoLicense MOD_FOT\r\n")));
			}
		}
		break;
	case MOD_NIVTRIGGER:
		{
			m_bNIVTRIGGERDemoLicAvailable=false;
			if(bReset)
			{
				getModel()->getCONFIG()->SetNIVTRIGGERdemoTimestamp(dtLicenseTimestamp);
				//DEBUGMSG(TRUE, (TEXT("delDemoLicense MOD_NIVTRIGGER\r\n")));
			}
		}
		break;
	default:
		{
			
		}
		break;
	}

	/*if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_LICENSING_CHANGED);*/

}
void CDataHandler::endableDemoLicense(eModule module)
{
	switch(module)
	{
	case MOD_HFO:
		{
			m_bHFODemoLicAvailable=true;
		}
		break;
	case MOD_NMODE:
		{
			m_bNMODEDemoLicAvailable=true;
		}
		break;
	case MOD_VGUARANTY:
		{
			m_bVGUARANTDemoLicAvailable=true;
		}
		break;
	case MOD_VLIMIT:
		{
			m_bVLIMITDemoLicAvailable=true;
		}
		break;
	case MOD_LUNGREC:
		{
			m_bLUNGRECDemoLicAvailable=true;
		}
		break;
	case MOD_TREND:
		{
			m_bTRENDDemoLicAvailable=true;
		}
		break;
	case MOD_THERAPY:
		{
			m_bTHERAPYDemoLicAvailable=true;
		}
		break;
	case MOD_PRICO:
		{
			m_bPRICODemoLicAvailable=true;
		}
		break;
	case MOD_FOT:
		{
			m_bFOTDemoLicAvailable=true;
		}
		break;
	case MOD_NIVTRIGGER:
		{
			m_bNIVTRIGGERDemoLicAvailable=true;
		}
		break;
	default:
		{

		}
		break;
	}

	/*if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_LICENSING_CHANGED);*/

}

/**********************************************************************************************//**
 * @fn	void CDataHandler::WriteLicenseFile()
 *
 * @brief	Writes the license file.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
void CDataHandler::WriteLicenseFile()
{
	CString szLicenseFile=_T("\\FFSDISK\\AL");
	szLicenseFile+=getModel()->GetUniqueID();
	szLicenseFile+=_T(".alic");

	CTlsOStream_File os(szLicenseFile);
	CTlsXmlWriter xml(&os);

	CString szEncryptedHFOKey=_T("");
	CString szEncryptedNMODEKey=_T("");
	CString szEncryptedVGUARANTYKey=_T("");
	CString szEncryptedVLIMITKey=_T("");
	CString szEncryptedTRENDSKey=_T("");
	CString szEncryptedLUNGRECKey=_T("");
	CString szEncryptedTHERAPYKey=_T("");
	CString szEncryptedPRICOKey=_T("");
	CString szEncryptedFOTKey=_T("");
	CString szEncryptedNIVTRIGGERKey=_T("");

	if(isHFOLicenseAvailable())
	{
		if(isHFODemoLicAvailable())
			szEncryptedHFOKey+=encryptDEMOKey(MOD_HFO);
		else
			szEncryptedHFOKey+=encryptKey(MOD_HFO);
	}
	else
		szEncryptedHFOKey=_T("0");

	if(isNMODELicenseAvailable())
	{
		if(isNMODEDemoLicAvailable())
			szEncryptedNMODEKey+=encryptDEMOKey(MOD_NMODE);
		else
			szEncryptedNMODEKey+=encryptKey(MOD_NMODE);
	}
	else
		szEncryptedNMODEKey=_T("0");

	if(isVGUARANTLicenseAvailable())
	{
		if(isVGUARANTDemoLicAvailable())
			szEncryptedVGUARANTYKey+=encryptDEMOKey(MOD_VGUARANTY);
		else
			szEncryptedVGUARANTYKey+=encryptKey(MOD_VGUARANTY);
	}
	else
		szEncryptedVGUARANTYKey=_T("0");

	if(isVLIMITLicenseAvailable())
	{
		if(isVLIMITDemoLicAvailable())
			szEncryptedVLIMITKey+=encryptDEMOKey(MOD_VLIMIT);
		else
			szEncryptedVLIMITKey+=encryptKey(MOD_VLIMIT);
	}
	else
		szEncryptedVLIMITKey=_T("0");

	if(isLUNGRECLicenseAvailable())
	{
		if(isLUNGRECDemoLicAvailable())
			szEncryptedLUNGRECKey+=encryptDEMOKey(MOD_LUNGREC);
		else
			szEncryptedLUNGRECKey+=encryptKey(MOD_LUNGREC);
	}
	else
		szEncryptedLUNGRECKey=_T("0");

	if(isTRENDLicenseAvailable())
	{
		if(isTRENDDemoLicAvailable())
			szEncryptedTRENDSKey+=encryptDEMOKey(MOD_TREND);
		else
			szEncryptedTRENDSKey+=encryptKey(MOD_TREND);
	}
	else
		szEncryptedTRENDSKey=_T("0");

	if(isTHERAPYLicenseAvailable())
	{
		if(isTHERAPYDemoLicAvailable())
			szEncryptedTHERAPYKey+=encryptDEMOKey(MOD_THERAPY);
		else
			szEncryptedTHERAPYKey+=encryptKey(MOD_THERAPY);
	}
	else
		szEncryptedTHERAPYKey=_T("0");

	if(isPRICOLicenseAvailable())
	{
		if(isPRICODemoLicAvailable())
			szEncryptedPRICOKey+=encryptDEMOKey(MOD_PRICO);
		else
			szEncryptedPRICOKey+=encryptKey(MOD_PRICO);
	}
	else
		szEncryptedPRICOKey=_T("0");

	if(isFOTLicenseAvailable())
	{
		if(isFOTDemoLicAvailable())
			szEncryptedFOTKey+=encryptDEMOKey(MOD_FOT);
		else
			szEncryptedFOTKey+=encryptKey(MOD_FOT);
	}
	else
		szEncryptedFOTKey=_T("0");
	
	if(isNIVTRIGGERLicenseAvailable())
	{
		if(isNIVTRIGGERDemoLicAvailable())
			szEncryptedNIVTRIGGERKey+=encryptDEMOKey(MOD_NIVTRIGGER);
		else
			szEncryptedNIVTRIGGERKey+=encryptKey(MOD_NIVTRIGGER);
	}
	else
		szEncryptedNIVTRIGGERKey=_T("0");

	xml.OpenTag(_T("FabianHFO"), true);

	int iSizeOfArray = 10;
	if(0 < iSizeOfArray)
	{
		for(int iIndex = 0; iIndex < iSizeOfArray; iIndex++)	
		{																
			xml.OpenTag(_T("License"), true);

			switch(iIndex)
			{
			case 0:
				{
					xml.WriteTextTag(_T("MODULE"), _T("HFO"));
					xml.WriteTextTag(_T("KEY"), szEncryptedHFOKey);
				}
				break;
			case 1:
				{
					xml.WriteTextTag(_T("MODULE"), _T("NMODE"));
					xml.WriteTextTag(_T("KEY"), szEncryptedNMODEKey);
				}
				break;
			case 2:
				{
					xml.WriteTextTag(_T("MODULE"), _T("VGUARANTY"));
					xml.WriteTextTag(_T("KEY"), szEncryptedVGUARANTYKey);
				}
				break;
			case 3:
				{
					xml.WriteTextTag(_T("MODULE"), _T("VLIMIT"));
					xml.WriteTextTag(_T("KEY"), szEncryptedVLIMITKey);
				}
				break;
			case 4:
				{
					xml.WriteTextTag(_T("MODULE"), _T("LUNGREC"));
					xml.WriteTextTag(_T("KEY"), szEncryptedLUNGRECKey);
				}
				break;
			case 5:
				{
					xml.WriteTextTag(_T("MODULE"), _T("TRENDS"));
					xml.WriteTextTag(_T("KEY"), szEncryptedTRENDSKey);
				}
				break;
			case 6:
				{
					xml.WriteTextTag(_T("MODULE"), _T("THERAPY"));
					xml.WriteTextTag(_T("KEY"), szEncryptedTHERAPYKey);
				}
				break;
			case 7:
				{
					xml.WriteTextTag(_T("MODULE"), _T("PRICO"));
					xml.WriteTextTag(_T("KEY"), szEncryptedPRICOKey);
				}
				break;
			case 8:
				{
					xml.WriteTextTag(_T("MODULE"), _T("FOT"));
					xml.WriteTextTag(_T("KEY"), szEncryptedFOTKey);
				}
				break;
			case 9:
				{
					xml.WriteTextTag(_T("MODULE"), _T("NIVTRIGGER"));
					xml.WriteTextTag(_T("KEY"), szEncryptedNIVTRIGGERKey);
				}
				break;
			default:
				break;
			}
			xml.CloseTag(_T("License"), true);
		}
	}

	xml.CloseTag(_T("FabianHFO"), true);
	os.Close();
}
/**********************************************************************************************//**
 * @fn	CString CDataHandler::getFormattedEncryptKey(CStringA szEncryptedKey)
 *
 * @brief	Gets formatted encrypt key for licensing.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @param	szEncryptedKey	The encrypted key.
 *
 * @return	The formatted encrypt key.
 **************************************************************************************************/
CString CDataHandler::getFormattedEncryptKey(CStringA szEncryptedKey)
{
	CString szForamttedKey=_T("");

	char szHex[30];

	strcpy_s(szHex,_countof(szHex),szEncryptedKey);


	int i=1;
	for(i;i<=20;i++)
	{
		szForamttedKey.AppendChar(szHex[i-1]);
		if(i%4==0 && i!=20)
			szForamttedKey.AppendChar('-');
	}

	return szForamttedKey;
}
//=============================================================================
/**
 * @brief Encrypt the license key with 
 * @param [in] The module to be enabled/disabled.
 *
 * @return the encrypted key string
 *
 *   This function encrypt the license key with Rijndael algorithm.
 *   Rijndael (pronounced Reindaal) is a block cipher, designed by Joan Daemen and Vincent Rijmen as a candidate algorithm for the AES.
 *   The cipher has a variable block length and key length. The authors currently specify how to use keys with a length
 *   of 128, 192, or 256 bits to encrypt blocks with al length of 128, 192 or 256 bits (all nine combinations of
 *   key length and block length are possible). Both block length and key length can be extended very easily to
 *   multiples of 32 bits.
 **/
//=============================================================================
CStringA CDataHandler::encryptKey(eModule module)
{
	CStringA szUniqueID16="";
	

	if(getModel()->GetUniqueID()=="" || getModel()->GetUniqueID()=="-ERROR-")
		return "";

	char szDataIn[17]= {0};
	char szDataOut[16] = {0};
	char szHex[30];
	CRijndael oRijndael;

	szUniqueID16=getModel()->GetUniqueID();
	int iLenUniqueID=szUniqueID16.GetLength();

	if(iLenUniqueID>16)
	{
		theApp.getLog()->WriteLine(_T("#HFO:0060"));
	}

	for(iLenUniqueID;iLenUniqueID<16;iLenUniqueID++)
	{
		szUniqueID16.AppendChar('0');
	}

	switch(module)
	{
	case MOD_MASTER:
		{
			//Initialization
			oRijndael.MakeKey("AcutronicMaster0", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_HFO:
		{
			//Initialization
			oRijndael.MakeKey("AcutronicHFO0000", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_NMODE:
		{
			oRijndael.MakeKey("AcutronicNMODE00", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_VGUARANTY:
		{
			oRijndael.MakeKey("AcutronicVGUARAN", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_VLIMIT:
		{
			oRijndael.MakeKey("AcutronicVLIMIT0", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_LUNGREC:
		{
			oRijndael.MakeKey("AcutronicLUNGREC", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_TREND:
		{
			oRijndael.MakeKey("AcutronicTRENDS0", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_THERAPY:
		{
			oRijndael.MakeKey("AcutronicTHERAPY", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_PRICO:
		{
			oRijndael.MakeKey("AcutronicPRICO00", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_FOT:
		{
			oRijndael.MakeKey("AcutronicFOT0000", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_NIVTRIGGER:
		{
			oRijndael.MakeKey("AcutronicNIVTRIG", CRijndael::sm_chain0, 16, 16);
		}
		break;
	default:
		{
			return "";
		}
		break;
	}

	strcpy_s(szDataIn,_countof(szDataIn),szUniqueID16);
	
	//Encryption
	oRijndael.EncryptBlock(szDataIn, szDataOut);

	charStr2HexStr((unsigned char*)szDataOut, szHex, 10);

	CStringA szRes(szHex);
	return szRes;
}

/**********************************************************************************************//**
 * @fn	CStringA CDataHandler::encryptDEMOKey(eModule module)
 *
 * @brief	Encrypt demo key.
 *
 * @author	Rainer Kuehner
 * @date	09.02.2017
 *
 * @param	module	The module.
 *
 * @return	A CStringA.
 **************************************************************************************************/
CStringA CDataHandler::encryptDEMOKey(eModule module)
{
	CStringA szUniqueID16="";


	if(getModel()->GetUniqueID()=="" || getModel()->GetUniqueID()=="-ERROR-")
		return "";

	char szDataIn[17]= {0};
	char szDataOut[16] = {0};
	char szHex[30];
	CRijndael oRijndael;

	szUniqueID16=getModel()->GetUniqueID();
	int iLenUniqueID=szUniqueID16.GetLength();

	if(iLenUniqueID>16)
	{
		theApp.getLog()->WriteLine(_T("#HFO:0060"));
	}

	for(iLenUniqueID;iLenUniqueID<16;iLenUniqueID++)
	{
		szUniqueID16.AppendChar('0');
	}

	switch(module)
	{
	case MOD_HFO:
		{
			//Initialization
			oRijndael.MakeKey("AcutrDemoHFO0000", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_NMODE:
		{
			oRijndael.MakeKey("AcutrDemoNMODE00", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_VGUARANTY:
		{
			oRijndael.MakeKey("AcutrDemoVGUARAN", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_VLIMIT:
		{
			oRijndael.MakeKey("AcutrDemoVLIMIT0", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_LUNGREC:
		{
			oRijndael.MakeKey("AcutrDemoLUNGREC", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_TREND:
		{
			oRijndael.MakeKey("AcutrDemoTRENDS0", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_THERAPY:
		{
			oRijndael.MakeKey("AcutrDemoTHERAPY", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_PRICO:
		{
			oRijndael.MakeKey("AcutrDemoPRICO00", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_FOT:
		{
			oRijndael.MakeKey("AcutrDemoFOT0000", CRijndael::sm_chain0, 16, 16);
		}
		break;
	case MOD_NIVTRIGGER:
		{
			oRijndael.MakeKey("AcutrDemoNIVTRIG", CRijndael::sm_chain0, 16, 16);
		}
		break;
	default:
		{
			return "";
		}
		break;
	}

	strcpy_s(szDataIn,_countof(szDataIn),szUniqueID16);

	//Encryption
	oRijndael.EncryptBlock(szDataIn, szDataOut);

	charStr2HexStr((unsigned char*)szDataOut, szHex, 10);

	CStringA szRes(szHex);
	return szRes;
}
//=============================================================================
/**
 * @brief Function to convert unsigned char to string of length 2
 * @param [in] unsigned char.
 * @param [out] char* of hex value
 **/
//=============================================================================
void CDataHandler::char2Hex(unsigned char ch, char* szHex)
{
	unsigned char byte[2];
	byte[0] = ch/16;
	byte[1] = ch%16;
	for(int i=0; i<2; i++)
	{
		if(byte[i] >= 0 && byte[i] <= 9)
			szHex[i] = '0' + byte[i];
		else
			szHex[i] = 'A' + byte[i] - 10;
	}
	szHex[2] = 0;
}

//=============================================================================
/**
 * @brief Function to string of length 2 to unsigned char
 * @param [in] const char* of of length 2.
 * @param [out] unsigned char
 **/
//=============================================================================
void CDataHandler::hex2Char(char const* szHex, unsigned char& rch)
{
	rch = 0;
	for(int i=0; i<2; i++)
	{
		if(*(szHex + i) >='0' && *(szHex + i) <= '9')
			rch = (rch << 4) + (*(szHex + i) - '0');
		else if(*(szHex + i) >='A' && *(szHex + i) <= 'F')
			rch = (rch << 4) + (*(szHex + i) - 'A' + 10);
		else
			break;
	}
} 
//=============================================================================
/**
 * @brief Function to convert string of unsigned chars to string of chars
 * @param [in] string of unsigned chars.
 * @param [out] string of chars
 **/
//=============================================================================
void CDataHandler::charStr2HexStr(unsigned char const* pucCharStr, char* pszHexStr, int iSize)
{
	int i;
	char szHex[3];
	pszHexStr[0] = 0;
	for(i=0; i<iSize; i++)
	{
		char2Hex(pucCharStr[i], szHex);
		strcat(pszHexStr, szHex);
	}
}

//=============================================================================
/**
 * @brief Function to convert string of chars to string of unsigned chars
 * @param [in] string of chars.
 * @param [out] string unsigned of chars
 **/
//=============================================================================
void CDataHandler::hexStr2CharStr(char const* pszHexStr, unsigned char* pucCharStr, int iSize)
{
	int i;
	unsigned char ch;
	for(i=0; i<iSize; i++)
	{
		hex2Char(pszHexStr+2*i, ch);
		pucCharStr[i] = ch;
	}
}

/**********************************************************************************************//**
 * @fn	BYTE CDataHandler::getPRICO_SPO2lowRange()
 *
 * @brief	Gets prico spo2low range.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @return	The calculated prico spo2low range.
 **************************************************************************************************/
BYTE CDataHandler::getPRICO_SPO2lowRange()
{
	return getModel()->getCONFIG()->getPRICO_SPO2lowRange();
}

/**********************************************************************************************//**
 * @fn	void CDataHandler::setPRICO_SPO2lowRange(BYTE SPO2low)
 *
 * @brief	Sets prico spo2low range.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @param	SPO2low	The spo2low.
 **************************************************************************************************/
void CDataHandler::setPRICO_SPO2lowRange(BYTE SPO2low)
{
	getModel()->getCONFIG()->setPRICO_SPO2lowRange(SPO2low);
	if(getModel()->getPRICOThread())
	{
		if(getModel()->getPRICOThread()->isPRICOalgorithmRunning())
		{
			getModel()->getPRICOThread()->setLowSpO2PRICOlimit(getPRICO_SPO2lowRange());
		}
	}

	if(getModel()->getAcuLink() != NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2LOW,SPO2low);
}

/**********************************************************************************************//**
 * @fn	BYTE CDataHandler::getPRICO_SPO2highRange()
 *
 * @brief	Gets prico spo2high range.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @return	The calculated prico spo2high range.
 **************************************************************************************************/
BYTE CDataHandler::getPRICO_SPO2highRange()
{
	return getModel()->getCONFIG()->getPRICO_SPO2highRange();
}

/**********************************************************************************************//**
 * @fn	void CDataHandler::setPRICO_SPO2highRange(BYTE SPO2high)
 *
 * @brief	Sets prico spo2high range.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @param	SPO2high	The spo2high.
 **************************************************************************************************/
void CDataHandler::setPRICO_SPO2highRange(BYTE SPO2high)
{
	getModel()->getCONFIG()->setPRICO_SPO2highRange(SPO2high);
	if(getModel()->getPRICOThread())
	{
		if(getModel()->getPRICOThread()->isPRICOalgorithmRunning())
		{
			getModel()->getPRICOThread()->setHighSpO2PRICOlimit(getPRICO_SPO2highRange());
		}
	}

	if(getModel()->getAcuLink() != NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2HIGH,SPO2high);
}

/**********************************************************************************************//**
 * @fn	BYTE CDataHandler::getPRICO_FIO2lowRange()
 *
 * @brief	Gets prico fio2low range.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @return	The calculated prico fio2low range.
 **************************************************************************************************/
BYTE CDataHandler::getPRICO_FIO2lowRange()
{
	return getModel()->getCONFIG()->getPRICO_FIO2lowRange();
}

/**********************************************************************************************//**
 * @fn	void CDataHandler::setPRICO_FIO2lowRange(BYTE FIO2low)
 *
 * @brief	Sets prico fio2low range.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @param	FIO2low	The fio2low.
 **************************************************************************************************/
void CDataHandler::setPRICO_FIO2lowRange(BYTE FIO2low)
{
	getModel()->getCONFIG()->setPRICO_FIO2lowRange(FIO2low);
	if(getModel()->getPRICOThread())
	{
		if(		getModel()->getPRICOThread()->isPRICOalgorithmRunning() 
			&&	((SHORT)CTlsFloat::Round(((double)getAppliedFiO2para())/10, 0) < FIO2low))
		{
			getModel()->Send_PARA_OXY_RATIO(FIO2low,true,true);
			getModel()->getPRICOThread()->setStartupOxyValue(FIO2low); //pro, change 3
		}
	}
	if(getModel()->getAcuLink() != NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2LOW,FIO2low);
}

/**********************************************************************************************//**
 * @fn	BYTE CDataHandler::getPRICO_FIO2highRange()
 *
 * @brief	Gets prico fio2high range.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @return	The calculated prico fio2high range.
 **************************************************************************************************/
BYTE CDataHandler::getPRICO_FIO2highRange()
{
	return getModel()->getCONFIG()->getPRICO_FIO2highRange();
}

/**********************************************************************************************//**
 * @fn	void CDataHandler::setPRICO_FIO2highRange(BYTE FIO2high)
 *
 * @brief	Sets prico fio2high range.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @param	FIO2high	The fio2high.
 **************************************************************************************************/
void CDataHandler::setPRICO_FIO2highRange(BYTE FIO2high)
{
	getModel()->getCONFIG()->setPRICO_FIO2highRange(FIO2high);
	if(getModel()->getPRICOThread())
	{
		if(		getModel()->getPRICOThread()->isPRICOalgorithmRunning()
			&&	((SHORT)CTlsFloat::Round(((double)getAppliedFiO2para())/10, 0)>FIO2high))
		{
			getModel()->Send_PARA_OXY_RATIO(FIO2high,true,true);
			getModel()->getPRICOThread()->setStartupOxyValue(FIO2high); //pro, change 3
		}
	}
	if(getModel()->getAcuLink() != NULL)
		getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2HIGH,FIO2high);
	
}

/**********************************************************************************************//**
 * @fn	bool CDataHandler::getPRICOState()
 *
 * @brief	Gets prico state.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @return	true if it succeeds, false if it fails.
 **************************************************************************************************/
bool CDataHandler::getPRICOState()
{
	return m_bPRICOrunning;
}

/**********************************************************************************************//**
 * @fn	void CDataHandler::setPRICOon()
 *
 * @brief	Sets PRICO on.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
void CDataHandler::setPRICOon()
{
	m_bPRICOrunning=true;
	//DEBUGMSG(TRUE, (TEXT("setPRICOState on\r\n")));
	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_START_PRICO);
}

/**********************************************************************************************//**
 * @fn	void CDataHandler::setPRICOoff()
 *
 * @brief	Sets PRICO off.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
void CDataHandler::setPRICOoff()
{
	m_bPRICOrunning=false;
	//DEBUGMSG(TRUE, (TEXT("setPRICOState off\r\n")));
	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_STOP_PRICO);
}

//=============================================================================
 /**
 * @brief Check HFO module license.
 *
 * @return true if available, else false
 **/
//=============================================================================
bool CDataHandler::isHFOLicenseAvailable()
{
	return m_bHFOLicenseAvailable;
}
void CDataHandler::enableHFOLicense()
{
	//m_bHFOLicenseAvailable=false;
	m_bHFOLicenseAvailable=true;
	theApp.getLog()->WriteLine(_T("-enableHFOmodule"));
}
void CDataHandler::disableHFOLicense()
{
	m_bHFOLicenseAvailable=false;
	theApp.getLog()->WriteLine(_T("-disableHFOmodule"));

	delDemoLicense(MOD_HFO,false);
}

bool CDataHandler::isHFODemoLicAvailable()
{
	return m_bHFODemoLicAvailable;
}

//=============================================================================
 /**
 * @brief Check NMODE module license.
 *
 * @return true if available, else false
 **/
//=============================================================================
bool CDataHandler::isNMODELicenseAvailable()
{
	return m_bNMODELicenseAvailable;
}
void CDataHandler::enableNMODELicense()
{
	m_bNMODELicenseAvailable=true;
	theApp.getLog()->WriteLine(_T("-enableNMODEmodule"));
}
void CDataHandler::disableNMODELicense()
{
	m_bNMODELicenseAvailable=false;
	theApp.getLog()->WriteLine(_T("-disableNMODEmodule"));

	delDemoLicense(MOD_NMODE,false);

}

bool CDataHandler::isNMODEDemoLicAvailable()
{
	return m_bNMODEDemoLicAvailable;
}

//=============================================================================
 /**
 * @brief Check Volume Guarantee module license.
 *
 * @return true if available, else false
 **/
//=============================================================================
bool CDataHandler::isVGUARANTLicenseAvailable()
{
	//return false;
	return m_bVGUARANTLicenseAvailable;
}
void CDataHandler::enableVGUARANTLicense()
{
	m_bVGUARANTLicenseAvailable=true;
	theApp.getLog()->WriteLine(_T("-enableVGUARANTmodule"));
}
void CDataHandler::disableVGUARANTLicense()
{
	m_bVGUARANTLicenseAvailable=false;
	theApp.getLog()->WriteLine(_T("-disableVGUARANTmodule"));

	delDemoLicense(MOD_VGUARANTY,false);

}
bool CDataHandler::isVGUARANTDemoLicAvailable()
{
	return m_bVGUARANTDemoLicAvailable;
}
//=============================================================================
 /**
 * @brief Check Volume Limit module license.
 *
 * @return true if available, else false
 **/
//=============================================================================
bool CDataHandler::isVLIMITLicenseAvailable()
{
	return m_bVLIMITLicenseAvailable;
}
void CDataHandler::enableVLIMITLicense()
{
	m_bVLIMITLicenseAvailable=true;
	theApp.getLog()->WriteLine(_T("-enableVLIMITmodule"));
}
void CDataHandler::disableVLIMITLicense()
{
	m_bVLIMITLicenseAvailable=false;
	theApp.getLog()->WriteLine(_T("-disableVLIMITmodule"));

	delDemoLicense(MOD_VLIMIT,false);

}
bool CDataHandler::isVLIMITDemoLicAvailable()
{
	return m_bVLIMITDemoLicAvailable;
}
//=============================================================================
 /**
 * @brief Check Lungrecruitment module license.
 *
 * @return true if available, else false
 **/
//=============================================================================
bool CDataHandler::isLUNGRECLicenseAvailable()
{
	return m_bLUNGRECLicenseAvailable;
}
void CDataHandler::enableLUNGRECLicense()
{
	m_bLUNGRECLicenseAvailable=true;
	theApp.getLog()->WriteLine(_T("-enableLUNGRECmodule"));
}
void CDataHandler::disableLUNGRECLicense()
{
	m_bLUNGRECLicenseAvailable=false;
	theApp.getLog()->WriteLine(_T("-disableLUNGRECmodule"));

	delDemoLicense(MOD_LUNGREC,false);

}
bool CDataHandler::isLUNGRECDemoLicAvailable()
{
	return m_bLUNGRECDemoLicAvailable;
}

//=============================================================================
 /**
 * @brief Check Trend module license.
 *
 * @return true if available, else false
 **/
//=============================================================================
bool CDataHandler::isTRENDLicenseAvailable()
{
	return m_bTRENDLicenseAvailable;
}
void CDataHandler::enableTRENDLicense()
{
	m_bTRENDLicenseAvailable=true;
	theApp.getLog()->WriteLine(_T("-enableTRENDmodule"));
}
void CDataHandler::disableTRENDLicense()
{
	m_bTRENDLicenseAvailable=false;
	theApp.getLog()->WriteLine(_T("-disableTRENDmodule"));

	delDemoLicense(MOD_TREND,false);
}
bool CDataHandler::isTRENDDemoLicAvailable()
{
	return m_bTRENDDemoLicAvailable;
}

/**********************************************************************************************//**
 * Queries if the nivtrigger license is available.
 *
 * \author	Rainer
 * \date	07.07.2017
 *
 * \return	True if the nivtrigger license is available, false if not.
 **************************************************************************************************/

bool CDataHandler::isNIVTRIGGERLicenseAvailable()
{
	return m_bNIVTRIGGERLicenseAvailable;
}
void CDataHandler::enableNIVTRIGGERLicense()
{
	m_bNIVTRIGGERLicenseAvailable=true;
	theApp.getLog()->WriteLine(_T("-enableNIVTRIGGERmodule"));
}
void CDataHandler::disableNIVTRIGGERLicense()
{
	m_bNIVTRIGGERLicenseAvailable=false;
	theApp.getLog()->WriteLine(_T("-disableNIVTRIGGERmodule"));

	delDemoLicense(MOD_NIVTRIGGER,false);

	getModel()->getCONFIG()->disableNIVTRIGGER();
}

bool CDataHandler::isNIVTRIGGERDemoLicAvailable()
{
	return m_bNIVTRIGGERDemoLicAvailable;
}

bool CDataHandler::isNIVTRIGGERAvailable()
{
	if(m_bNIVTRIGGERLicenseAvailable || m_bNIVTRIGGERDemoLicAvailable)
		return true;
	else
		return false;
}
//=============================================================================
 /**
 * @brief Check FOT module license.
 *
 * @return true if available, else false
 **/
//=============================================================================
bool CDataHandler::isFOTLicenseAvailable()
{
	return m_bFOTLicenseAvailable;
}
void CDataHandler::enableFOTLicense()
{
	m_bFOTLicenseAvailable=true;
	theApp.getLog()->WriteLine(_T("-enableFOTmodule"));

	getModel()->initFOTthread();
}
void CDataHandler::disableFOTLicense()
{
	m_bFOTLicenseAvailable=false;
	theApp.getLog()->WriteLine(_T("-disableFOTmodule"));

	delDemoLicense(MOD_FOT,false);

	getModel()->deinitFOTthread();
}

bool CDataHandler::isFOTDemoLicAvailable()
{
	return m_bFOTDemoLicAvailable;
}


//=============================================================================
 /**
 * @brief Check Therapy module license.
 *
 * @return true if available, else false
 **/
//=============================================================================
bool CDataHandler::isTHERAPYLicenseAvailable()
{
	return m_bTHERAPYLicenseAvailable;
}
void CDataHandler::enableTHERAPYLicense()
{
	m_bTHERAPYLicenseAvailable=true;
	theApp.getLog()->WriteLine(_T("-enableTHERAPYmodule"));
}
void CDataHandler::disableTHERAPYLicense()
{
	m_bTHERAPYLicenseAvailable=false;
	theApp.getLog()->WriteLine(_T("-disableTHERAPYmodule"));

	delDemoLicense(MOD_THERAPY,false);

}
bool CDataHandler::isTHERAPYDemoLicAvailable()
{
	return m_bTHERAPYDemoLicAvailable;
}
//=============================================================================
 /**
 * @brief Check PRICO module license.
 *
 * @return true if available, else false
 **/
//=============================================================================
bool CDataHandler::isPRICOLicenseAvailable()
{
	return m_bPRICOLicenseAvailable;
}
void CDataHandler::enablePRICOLicense()
{
	m_bPRICOLicenseAvailable=true;
	theApp.getLog()->WriteLine(_T("-enablePRICOmodule"));

	getModel()->initPRICOthread();

}
void CDataHandler::disablePRICOLicense()
{
	m_bPRICOLicenseAvailable=false;
	theApp.getLog()->WriteLine(_T("-disablePRICOmodule"));

	delDemoLicense(MOD_PRICO,false);

	getModel()->deinitPRICOthread();

}
bool CDataHandler::isPRICODemoLicAvailable()
{
	return m_bPRICODemoLicAvailable;
}

/**********************************************************************************************//**
 * @fn	BOOL CDataHandler::showFlowAC()
 *
 * @brief	Shows the flow AC.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @return	true if it succeeds, false if it fails.
 **************************************************************************************************/
BOOL CDataHandler::showFlowAC()
{
	return m_bShowFlowAC;
}

//=============================================================================
/**
 * @brief Load the configuration from file 'fabianhfo.ini'.
 *
 *   - load the configuration of displaying FlowAC value
 **/
//=============================================================================
void CDataHandler::loadIniConfiguration()
{
	CStringW szState=CTlsIniFile::ReadIniStr(_T("CONFIG"), _T("FLOWAC"), _T("FALSE"), FABIANINI);

	if(szState==_T("TRUE"))
	{
		m_bShowFlowAC=TRUE;
	}
	else
	{
		m_bShowFlowAC=FALSE;
	}
}
//=============================================================================
/**
 * @brief Load the configuration of diagram colors from file 'fabianhfo.ini'.
 *
 *   - load the color for following graphs:
 *		Pressure wave
 *		Flow wave
 *		Volume wave
 *		etCO2 wave
 *		SpO2 wave
 *		Loops
 *		Saved Loop
 *		Trigger
 **/
//=============================================================================
void CDataHandler::loadDiagrammColor()
{
	m_crGraphColor_PRESSURE=CTlsIniFile::ReadIniDWORD(_T("WAVE"), _T("PRESSURE"), 0x8c8c8c, FABIANINI);
	m_crGraphColor_FLOW=CTlsIniFile::ReadIniDWORD(_T("WAVE"), _T("FLOW"), 0x8c8c8c, FABIANINI);
	m_crGraphColor_VOLUME=CTlsIniFile::ReadIniDWORD(_T("WAVE"), _T("VOLUME"), 0x8c8c8c, FABIANINI);
	m_crGraphColor_ETCO2=CTlsIniFile::ReadIniDWORD(_T("WAVE"), _T("ETCO2"), 0x8c8c8c, FABIANINI);
	m_crGraphColor_SPO2=CTlsIniFile::ReadIniDWORD(_T("WAVE"), _T("SPO2"), 0x8c8c8c, FABIANINI);
	m_crGraphColor_LOOP=CTlsIniFile::ReadIniDWORD(_T("WAVE"), _T("LOOP"), 0x0000ff, FABIANINI);
	m_crGraphColor_SAVEDLOOP=CTlsIniFile::ReadIniDWORD(_T("WAVE"),_T("SAVEDLOOP") , 0x000000, FABIANINI);
	m_crGraphColor_TRIGGER=CTlsIniFile::ReadIniDWORD(_T("WAVE"),  _T("TRIGGER"), 0x00dc00, FABIANINI);
}

COLORREF CDataHandler::getGraphColor_PRESSURE()
{
	return m_crGraphColor_PRESSURE;
}
COLORREF CDataHandler::getGraphColor_FLOW()
{
	return m_crGraphColor_FLOW;
}
COLORREF CDataHandler::getGraphColor_VOLUME()
{
	return m_crGraphColor_VOLUME;
}
COLORREF CDataHandler::getGraphColor_ETCO2()
{
	return m_crGraphColor_ETCO2;
}
COLORREF CDataHandler::getGraphColor_SPO2()
{
	return m_crGraphColor_SPO2;
}
COLORREF CDataHandler::getGraphColor_LOOP()
{
	return m_crGraphColor_LOOP;
}
COLORREF CDataHandler::getGraphColor_SAVEDLOOP()
{
	return m_crGraphColor_SAVEDLOOP;
}
COLORREF CDataHandler::getGraphColor_TRIGGER()
{
	return m_crGraphColor_TRIGGER;
}

/**********************************************************************************************//**
 * @fn	void CDataHandler::loadNumerics()
 *
 * @brief	Loads the numerics.
 * 			
 +			This function load the numeric blocks from the file fabianhfo.ini.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
void CDataHandler::loadNumerics()
{
	NUMERICINI* pbufNumeric=NULL;
	eNumericMode eMode=NUMMODE_IPPV;

	CString szMode=_T("");
	for(BYTE iMode=0;iMode<MAX_NUMERICMODE;iMode++)
	{
		switch(iMode)
		{
		case NUMMODE_IPPV:
			{
				szMode=_T("NUMERIC_IPPV");
				eMode=NUMMODE_IPPV;
				break;
			}
		case NUMMODE_SIPPV:
			{
				szMode=_T("NUMERIC_SIPPV");
				eMode=NUMMODE_SIPPV;
				break;
			}
		case NUMMODE_SIMV:
			{
				szMode=_T("NUMERIC_SIMV");
				eMode=NUMMODE_SIMV;
				break;
			}
		case NUMMODE_SIMVPSV:
			{
				szMode=_T("NUMERIC_SIMVPSV");
				eMode=NUMMODE_SIMVPSV;
				break;
			}
		case NUMMODE_PSV:
			{
				szMode=_T("NUMERIC_PSV");
				eMode=NUMMODE_PSV;
				break;
			}
		case NUMMODE_CPAP:
			{
				szMode=_T("NUMERIC_CPAP");
				eMode=NUMMODE_CPAP;
				break;
			}
		case NUMMODE_HFO:
			{
				szMode=_T("NUMERIC_HFO");
				eMode=NUMMODE_HFO;
				break;
			}
		case NUMMODE_NCPAP:
			{
				szMode=_T("NUMERIC_NCPAP");
				eMode=NUMMODE_NCPAP;
				break;
			}
		case NUMMODE_DUOPAP:
			{
				szMode=_T("NUMERIC_DUOPAP");
				eMode=NUMMODE_DUOPAP;
				break;
			}
		case NUMMODE_THERAPY:
			{
				szMode=_T("NUMERIC_THERAPY");
				eMode=NUMMODE_THERAPY;
				break;
			}
		case NUMMODE_FLOWOFFCONV:
			{
				szMode=_T("NUMERIC_FLOWOFFCONV");
				eMode=NUMMODE_FLOWOFFCONV;
				break;
			}
		case NUMMODE_FLOWOFFCPAP:
			{
				szMode=_T("NUMERIC_FLOWOFFCPAP");
				eMode=NUMMODE_FLOWOFFCPAP;
				break;
			}
		case NUMMODE_FLOWOFFHFO:
			{
				szMode=_T("NUMERIC_FLOWOFFHFO");
				eMode=NUMMODE_FLOWOFFHFO;
				break;
			}
		}

		for(BYTE iBlock=0;iBlock<MAX_NUMERICMODEBLOCK;iBlock++)
		{
			CString szNum=_T("");
			szNum.Format(_T("%s%d"),szMode,iBlock+1);
			switch(eMode)
			{
			case NUMMODE_IPPV:
				pbufNumeric=&m_pbufNumericIPPV[iBlock];
				break;
			case NUMMODE_SIPPV:
				pbufNumeric=&m_pbufNumericSIPPV[iBlock];
				break;
			case NUMMODE_SIMV:
				pbufNumeric=&m_pbufNumericSIMV[iBlock];
				break;
			case NUMMODE_SIMVPSV:
				pbufNumeric=&m_pbufNumericSIMVPSV[iBlock];
				break;
			case NUMMODE_PSV:
				pbufNumeric=&m_pbufNumericPSV[iBlock];
				break;
			case NUMMODE_CPAP:
				pbufNumeric=&m_pbufNumericCPAP[iBlock];
				break;
			case NUMMODE_HFO:
				pbufNumeric=&m_pbufNumericHFO[iBlock];
				break;
			case NUMMODE_NCPAP:
				pbufNumeric=&m_pbufNumericNCPAP[iBlock];
				break;
			case NUMMODE_DUOPAP:
				pbufNumeric=&m_pbufNumericDUOPAP[iBlock];
				break;
			case NUMMODE_THERAPY:
				pbufNumeric=&m_pbufNumericTHERAPY[iBlock];
				break;
			case NUMMODE_FLOWOFFCONV:
				pbufNumeric=&m_pbufNumericFLOWOFFCONV[iBlock];
				break;
			case NUMMODE_FLOWOFFCPAP:
				pbufNumeric=&m_pbufNumericFLOWOFFCPAP[iBlock];
				break;
			case NUMMODE_FLOWOFFHFO:
				pbufNumeric=&m_pbufNumericFLOWOFFHFO[iBlock];
				break;
			}
			CStringW szState=CTlsIniFile::ReadIniStr(szNum, _T("SHOW"), _T("DEFAULT"), NUMERICCONFINI);
			setNumericINIstate(pbufNumeric,iBlock,szState);

			bool bCount=false;
			if(szState==_T("TRUE"))
			{
				for(BYTE num=1;num<=MAX_NUMERICWND;num++)
				{
					readNumericINIvalues(pbufNumeric,iBlock,num,szNum);
				}
				bCount=true;
			}

			if(bCount)
			{
				switch(eMode)
				{
				case NUMMODE_IPPV:
					m_iNumericIPPVcount++;
					break;
				case NUMMODE_SIPPV:
					m_iNumericSIPPVcount++;
					break;
				case NUMMODE_SIMV:
					m_iNumericSIMVcount++;
					break;
				case NUMMODE_SIMVPSV:
					m_iNumericSIMVPSVcount++;
					break;
				case NUMMODE_PSV:
					m_iNumericPSVcount++;
					break;
				case NUMMODE_CPAP:
					m_iNumericCPAPcount++;
					break;
				case NUMMODE_HFO:
					m_iNumericHFOcount++;
					break;
				case NUMMODE_NCPAP:
					m_iNumericNCPAPcount++;
					break;
				case NUMMODE_DUOPAP:
					m_iNumericDUOPAPcount++;
					break;
				case NUMMODE_THERAPY:
					m_iNumericTHERAPYcount++;
					break;
				case NUMMODE_FLOWOFFCONV:
					m_iNumericFLOWOFFCONVcount++;
					break;
				case NUMMODE_FLOWOFFCPAP:
					m_iNumericFLOWOFFCPAPcount++;
					break;
				case NUMMODE_FLOWOFFHFO:
					m_iNumericFLOWOFFHFOcount++;
					break;
				}
			}
		}
	}
}

void CDataHandler::setNumericINIstate(NUMERICINI* pbufNumeric,BYTE iBlock,CStringW szState)
{
	if(szState==_T("FALSE"))
	{
		pbufNumeric->SHOW=FALSE;
	}
	else if(szState==_T("TRUE"))
	{
		pbufNumeric->SHOW=TRUE;
	}
	else
	{
		pbufNumeric->SHOW=FALSE;
	}
}

void CDataHandler::readNumericINIvalues(NUMERICINI* pbufNumeric,BYTE iBlock,BYTE num,CStringW szNum)
{
	switch(num)
	{
	case 1:
		{
			if(pbufNumeric->eNumMode==NUMMODE_THERAPY)
			{
				pbufNumeric->VAL1=NUMT_INFO_PATAL;
				pbufNumeric->SIZE1=NUMERICSIZE_1;
			}
			else if(pbufNumeric->eNumMode==NUMMODE_NCPAP || pbufNumeric->eNumMode==NUMMODE_DUOPAP)
			{
				pbufNumeric->VAL1=NUMT_INFO_TUBE;
				pbufNumeric->SIZE1=NUMERICSIZE_1;
			}
			else if(pbufNumeric->eNumMode==NUMMODE_FLOWOFFCONV || pbufNumeric->eNumMode==NUMMODE_FLOWOFFCPAP || pbufNumeric->eNumMode==NUMMODE_FLOWOFFHFO)
			{
				pbufNumeric->VAL1=NUMT_INFO_FLOWOFF;
				pbufNumeric->SIZE1=NUMERICSIZE_1;
			}
			else
			{
				pbufNumeric->VAL1=getNumericType(CTlsIniFile::ReadIniStr(szNum, _T("VAL1"), _T("NONE"), NUMERICCONFINI));
				pbufNumeric->SIZE1=getNumericSize(CTlsIniFile::ReadIniStr(szNum, _T("SIZE1"), _T("0"), NUMERICCONFINI));
			}
		}
		break;
	case 2:
		{
			pbufNumeric->VAL2=getNumericType(CTlsIniFile::ReadIniStr(szNum, _T("VAL2"), _T("NONE"), NUMERICCONFINI));
			pbufNumeric->SIZE2=getNumericSize(CTlsIniFile::ReadIniStr(szNum, _T("SIZE2"), _T("0"), NUMERICCONFINI));
		}
		break;
	case 3:
		{
			pbufNumeric->VAL3=getNumericType(CTlsIniFile::ReadIniStr(szNum, _T("VAL3"), _T("NONE"), NUMERICCONFINI));
			pbufNumeric->SIZE3=getNumericSize(CTlsIniFile::ReadIniStr(szNum, _T("SIZE3"), _T("0"), NUMERICCONFINI));
		}
		break;
	case 4:
		{
			pbufNumeric->VAL4=getNumericType(CTlsIniFile::ReadIniStr(szNum, _T("VAL4"), _T("NONE"), NUMERICCONFINI));
			pbufNumeric->SIZE4=getNumericSize(CTlsIniFile::ReadIniStr(szNum, _T("SIZE4"), _T("0"), NUMERICCONFINI));
		}
		break;
	case 5:
		{
			pbufNumeric->VAL5=getNumericType(CTlsIniFile::ReadIniStr(szNum, _T("VAL5"), _T("NONE"), NUMERICCONFINI));
			pbufNumeric->SIZE5=getNumericSize(CTlsIniFile::ReadIniStr(szNum, _T("SIZE5"), _T("0"), NUMERICCONFINI));
		}
		break;
	case 6:
		{
			pbufNumeric->VAL6=getNumericType(CTlsIniFile::ReadIniStr(szNum, _T("VAL6"), _T("NONE"), NUMERICCONFINI));
			pbufNumeric->SIZE6=getNumericSize(CTlsIniFile::ReadIniStr(szNum, _T("SIZE6"), _T("0"), NUMERICCONFINI));
		}
		break;
	case 7:
		{
			pbufNumeric->VAL7=getNumericType(CTlsIniFile::ReadIniStr(szNum, _T("VAL7"), _T("NONE"), NUMERICCONFINI));
			pbufNumeric->SIZE7=getNumericSize(CTlsIniFile::ReadIniStr(szNum, _T("SIZE7"), _T("0"), NUMERICCONFINI));
		}
		break;
	case 8:
		{
			pbufNumeric->VAL8=getNumericType(CTlsIniFile::ReadIniStr(szNum, _T("VAL8"), _T("NONE"), NUMERICCONFINI));
			pbufNumeric->SIZE8=getNumericSize(CTlsIniFile::ReadIniStr(szNum, _T("SIZE8"), _T("0"), NUMERICCONFINI));
		}
		break;
	}
}
eNumericType CDataHandler::getNumericType(CStringW szVal)
{
	if(szVal==_T("PPEAK"))
	{
		return NUMT_PPEAK;
	}
	else if(szVal==_T("PMEAN"))
	{
		return NUMT_PMEAN;
	}
	else if(szVal==_T("PEEP"))
	{
		return NUMT_PEEP;
	}
	else if(szVal==_T("PSVTIME"))
	{
		return NUMT_PSVTIME;
	}
	else if(szVal==_T("COMPLIANCE"))
	{
		return NUMT_COMPLIANCE;
	}
	else if(szVal==_T("C20C"))
	{
		return NUMT_C20C;
	}
	else if(szVal==_T("RESISTANCE"))
	{
		return NUMT_RESISTANCE;
	}
	else if(szVal==_T("MVEXP"))
	{
		return NUMT_MVEXP;
	}
	else if(szVal==_T("SHAREMV"))
	{
		return NUMT_SHAREMV;
	}
	else if(szVal==_T("VTE"))
	{
		return NUMT_VTE;
	}
	else if(szVal==_T("VTEMAND"))
	{
		return NUMT_VTEMAND;
	}
	else if(szVal==_T("VTESPONT"))
	{
		return NUMT_VTESPONT;
	}
	else if(szVal==_T("LEAK"))
	{
		return NUMT_LEAK;
	}
	else if(szVal==_T("FREQ"))
	{
		return NUMT_FREQ;
	}
	else if(szVal==_T("TRIGGER"))
	{
		return NUMT_TRIGGER;
	}
	else if(szVal==_T("VTI"))
	{
		return NUMT_VTI;
	}
	else if(szVal==_T("HFOAMP"))
	{
		return NUMT_HFOAMP;
	}
	else if(szVal==_T("HFOVTE"))
	{
		return NUMT_HFOVTE;
	}
	else if(szVal==_T("DCO2"))
	{
		return NUMT_DCO2;
	}
	else if(szVal==_T("HFOFREQ"))
	{
		return NUMT_HFOFREQ;
	}
	else if(szVal==_T("OXY"))
	{
		return NUMT_OXY;
	}
	else if(szVal==_T("ETCO2"))
	{
		return NUMT_ETCO2;
	}
	else if(szVal==_T("DEMANDFLOW"))
	{
		return NUMT_DEMANDFLOW;
	}
	else if(szVal==_T("IFLOW"))
	{
		return NUMT_IFLOW;
	}
	else if(szVal==_T("EFLOW"))
	{
		return NUMT_EFLOW;
	}
	else if(szVal==_T("TAU"))
	{
		return NUMT_TAU;
	}
	else if(szVal==_T("FREQETCO2"))
	{
		return NUMT_FREQETCO2;
	}
	else if(szVal==_T("SPO2"))
	{
		return NUMT_SPO2;
	}
	else if(szVal==_T("SPO2_PI"))
	{
		return NUMT_SPO2_PI;
	}
	else if(szVal==_T("SPO2_PR"))
	{
		return NUMT_SPO2_PR;
	}
	else if(szVal==_T("RSBI"))
	{
		return NUMT_RSBI;
	}
	else if(szVal==_T("FICO2"))
	{
		return NUMT_FICO2;
	}
	else if(szVal==_T("MECHANIC"))
	{
		return NUMT_MECHANIC;
	}
	else if(szVal==_T("VTE2"))
	{
		return NUMT_VTE2;
	}
	else if(szVal==_T("VTEKG"))
	{
		return NUMT_VTEKG;
	}
	else if(szVal==_T("INFO_PATAL"))
	{
		return NUMT_INFO_PATAL;
	}
	else if(szVal==_T("INFO_TUBE"))
	{
		return NUMT_INFO_TUBE;
	}
	else if(szVal==_T("INFO_FLOWOFF"))
	{
		return NUMT_INFO_FLOWOFF;
	}
	else if(szVal==_T("SPACE"))
	{
		return NUMT_SPACE;
	}
	else
	{
		return NUMT_PPEAK;
	}
}
eNumericSize CDataHandler::getNumericSize(CStringW szSize)
{
	if(szSize==_T("0"))
	{
		return NUMERICSIZE_0;
	}
	else if(szSize==_T("1"))
	{
		return NUMERICSIZE_1;
	}
	else if(szSize==_T("2"))
	{
		return NUMERICSIZE_2;
	}
	else if(szSize==_T("3"))
	{
		return NUMERICSIZE_3;
	}
	else
	{
		return NUMERICSIZE_0;
	}

}
//=============================================================================
/**
 * @brief Load the configuration of parameter data and ventilation mode.
 *
 *   This function Load the configuration of parameter data and ventilation
 *   mode a makes an entry to log file of ventilation mode.
 *   Checks the state of flow sensor regarding ventilation mode.
 *   Checks the ventilation mode regarding enabled/disabled licenses of mode.
 *   Writes the date of last flow calibration into log file.
 **/
//
//  2015/07/02: flow sensor handling checked for thread safe
//=============================================================================
void CDataHandler::loadConfig()
{
	PARADATA()->Init();
	PRESET()->Init();
	
	EnterCriticalSection(&csFlowsensorState);
	m_eFlowSensorState=getModel()->getCONFIG()->GetFlowSensorState();
	if(m_eFlowSensorState==FLOWSENSOR_AUTOOFF)
	{
		m_eFlowSensorState=FLOWSENSOR_ON;
	}
	LeaveCriticalSection(&csFlowsensorState);

	//check for Flowsensor state
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_PSV:
		{
			theApp.getLog()->WriteLine(_T("### Start with Ventmode PSV"));
		}
		break;
	case VM_SIMVPSV:
		{
			theApp.getLog()->WriteLine(_T("### Start with Ventmode SIMVPSV"));
		}
		break;
	case VM_IPPV:
		{
			theApp.getLog()->WriteLine(_T("### Start with Ventmode IPPV"));
		}
		break;
	case VM_SIMV:
		{
			theApp.getLog()->WriteLine(_T("### Start with Ventmode SIMV"));
		}
		break;
	case VM_SIPPV:
		{
			theApp.getLog()->WriteLine(_T("### Start with Ventmode SIPPV"));
		}
		break;
	case VM_CPAP:
		{
			theApp.getLog()->WriteLine(_T("### Start with Ventmode CPAP"));
		}
		break;
	case VM_HFO:
		{
			if(isHFOLicenseAvailable()==false)
			{
				getModel()->getCONFIG()->SetCurMode(VM_IPPV);
				theApp.getLog()->WriteLine(_T("### Start with Ventmode VM_HFO->VM_IPPV"));
			}
			else
			{
				theApp.getLog()->WriteLine(_T("### Start with Ventmode HFO"));
				SetHFOrunning(true);
			}
			
		}
		break;
	case VM_DUOPAP:
		{
			if(isNMODELicenseAvailable()==false)
			{
				getModel()->getCONFIG()->SetCurMode(VM_IPPV);
				theApp.getLog()->WriteLine(_T("### Start with Ventmode VM_DUOPAP->VM_IPPV"));
			}
			else
			{
				theApp.getLog()->WriteLine(_T("### Start with Ventmode DUOPAP"));
				if(GetFlowSensorState()==FLOWSENSOR_AUTOOFF) //rku,PRETRIGGER
				{
					EnterCriticalSection(&csFlowsensorState);
					m_eFlowSensorState=FLOWSENSOR_OFF;
					LeaveCriticalSection(&csFlowsensorState);
					
					theApp.getLog()->WriteLine(_T("#HFO:0061"));
				}
				else if(GetFlowSensorState()!=FLOWSENSOR_OFF && getModel()->getDATAHANDLER()->PARADATA()->GetTriggerPara_DUOPAP()==MAXRANGE_TRIGGER_OFF)
				{
					EnterCriticalSection(&csFlowsensorState);
					m_eFlowSensorState=FLOWSENSOR_OFF;
					LeaveCriticalSection(&csFlowsensorState);
				}
				/*else if(GetFlowSensorState()==FLOWSENSOR_OFF && GetPrevFlowSensorState()==FLOWSENSOR_ON)
				{
					EnterCriticalSection(&csFlowsensorState);
					m_eFlowSensorState=FLOWSENSOR_ON;
					LeaveCriticalSection(&csFlowsensorState);
					//theApp.getLog()->WriteLine(_T("#HFO:0062"));
				}
				else if(GetFlowSensorState()!=FLOWSENSOR_OFF)
				{
					EnterCriticalSection(&csFlowsensorState);
					m_eFlowSensorState=FLOWSENSOR_OFF;
					LeaveCriticalSection(&csFlowsensorState);
					theApp.getLog()->WriteLine(_T("#HFO:0062"));
				}*/
			}
			
		}
		break;
	case VM_NCPAP:
		{
			if(isNMODELicenseAvailable()==false)
			{
				getModel()->getCONFIG()->SetCurMode(VM_IPPV);
				theApp.getLog()->WriteLine(_T("### Start with Ventmode VM_NCPAP->VM_IPPV"));
			}
			else
			{
				theApp.getLog()->WriteLine(_T("### Start with Ventmode NCPAP"));
				if(GetFlowSensorState()==FLOWSENSOR_AUTOOFF) //rku,PRETRIGGER
				{
					EnterCriticalSection(&csFlowsensorState);
					m_eFlowSensorState=FLOWSENSOR_OFF;
					LeaveCriticalSection(&csFlowsensorState);

					theApp.getLog()->WriteLine(_T("#HFO:0063"));
				}
				else if(GetFlowSensorState()!=FLOWSENSOR_OFF && getModel()->getDATAHANDLER()->PARADATA()->GetTriggerPara_NCPAP()==MAXRANGE_TRIGGER_OFF)
				{
					EnterCriticalSection(&csFlowsensorState);
					m_eFlowSensorState=FLOWSENSOR_OFF;
					LeaveCriticalSection(&csFlowsensorState);

					getModel()->getCONFIG()->SetFlowSensorState(FLOWSENSOR_OFF);
				}
				//else if(GetFlowSensorState()==FLOWSENSOR_OFF && GetPrevFlowSensorState()==FLOWSENSOR_ON)
				//{
				//	EnterCriticalSection(&csFlowsensorState);
				//	m_eFlowSensorState=FLOWSENSOR_ON;
				//	LeaveCriticalSection(&csFlowsensorState);
				//	//theApp.getLog()->WriteLine(_T("#HFO:0062"));
				//}

				//if(/*false==isNebulizerOn() &&*/ GetFlowSensorState()==FLOWSENSOR_AUTOOFF) rku,PRETRIGGER
				//{
				//	EnterCriticalSection(&csFlowsensorState);
				//	m_eFlowSensorState=FLOWSENSOR_OFF;
				//	LeaveCriticalSection(&csFlowsensorState);
				//	theApp.getLog()->WriteLine(_T("#HFO:0063"));
				//}
				//else if(GetFlowSensorState()!=FLOWSENSOR_OFF)
				//{
				//	EnterCriticalSection(&csFlowsensorState);
				//	m_eFlowSensorState=FLOWSENSOR_OFF;
				//	LeaveCriticalSection(&csFlowsensorState);
				//	theApp.getLog()->WriteLine(_T("#HFO:0064"));
				//	//SetFlowSensorState(FLOWSENSOR_OFF);
				//}
			}
			
		}
		break;
	case VM_THERAPIE:
		{
			if(isTHERAPYLicenseAvailable()==false)
			{
				getModel()->getCONFIG()->SetCurMode(VM_IPPV);
				theApp.getLog()->WriteLine(_T("### Start with Ventmode VM_THERAPIE->VM_IPPV"));
			}
			else
			{
				theApp.getLog()->WriteLine(_T("Start with Ventmode THERAPIE"));
				if(/*false==isNebulizerOn() &&*/ GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
				{
					EnterCriticalSection(&csFlowsensorState);
					m_eFlowSensorState=FLOWSENSOR_OFF;
					LeaveCriticalSection(&csFlowsensorState);
					theApp.getLog()->WriteLine(_T("#HFO:0065"));
				}
				else if(GetFlowSensorState()!=FLOWSENSOR_OFF)
				{
					EnterCriticalSection(&csFlowsensorState);
					m_eFlowSensorState=FLOWSENSOR_OFF;
					LeaveCriticalSection(&csFlowsensorState);
					theApp.getLog()->WriteLine(_T("#HFO:0066"));
					//SetFlowSensorState(FLOWSENSOR_OFF);
				}
			}
			
		}
		break;
	case VM_PRE_IPPV://darf nicht vorkommen!!!!!
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_SERVICE:
	default:
		{
			theApp.getLog()->WriteLine(_T("#HFO:0067"));

			switch(getModel()->getCONFIG()->GetPrevMode())
			{
			case VM_PSV:
			case VM_SIMVPSV:
			case VM_IPPV:
			case VM_SIMV:
			case VM_SIPPV:
			case VM_CPAP:
			case VM_HFO:
			case VM_DUOPAP: //rku,PRETRIGGER
			case VM_NCPAP: //rku,PRETRIGGER
				{
					getModel()->getCONFIG()->SetCurMode(getModel()->getCONFIG()->GetPrevMode());
					/*if(GetFlowSensorState()==FLOWSENSOR_AUTOOFF) //rku,PRETRIGGER
					{
						EnterCriticalSection(&csFlowsensorState);
						m_eFlowSensorState=FLOWSENSOR_OFF;
						LeaveCriticalSection(&csFlowsensorState);
					}
					else if(GetFlowSensorState()==FLOWSENSOR_OFF)
					{
						EnterCriticalSection(&csFlowsensorState);
						m_eFlowSensorState=FLOWSENSOR_OFF;
						LeaveCriticalSection(&csFlowsensorState);
					}*/
				}
				break;
			//case VM_DUOPAP: //rku,PRETRIGGER
			//case VM_NCPAP://rku,PRETRIGGER
			case VM_THERAPIE:
				{
					getModel()->getCONFIG()->SetCurMode(getModel()->getCONFIG()->GetPrevMode());
					if(/*false==isNebulizerOn() &&*/ GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
					{
						EnterCriticalSection(&csFlowsensorState);
						m_eFlowSensorState=FLOWSENSOR_OFF;
						LeaveCriticalSection(&csFlowsensorState);
					}
					else if(GetFlowSensorState()!=FLOWSENSOR_OFF)
					{
						EnterCriticalSection(&csFlowsensorState);
						m_eFlowSensorState=FLOWSENSOR_OFF;
						LeaveCriticalSection(&csFlowsensorState);
					}
				}
				break;
			default:
				{
					getModel()->getCONFIG()->SetCurMode(VM_IPPV);
					/*if(GetFlowSensorState()==FLOWSENSOR_AUTOOFF) case VM_DUOPAP: //rku,PRETRIGGER
					{
						EnterCriticalSection(&csFlowsensorState);
						m_eFlowSensorState=FLOWSENSOR_OFF;
						LeaveCriticalSection(&csFlowsensorState);
					}
					else if(GetFlowSensorState()==FLOWSENSOR_OFF)
					{
						EnterCriticalSection(&csFlowsensorState);
						m_eFlowSensorState=FLOWSENSOR_OFF;
						LeaveCriticalSection(&csFlowsensorState);
					}*/
				}
				break;
			}
		}
		break;
	}


	getModel()->getCONFIG()->SetPrevMode(getModel()->getCONFIG()->GetCurMode());

	CStringW szTimeLastFlowCal = getModel()->getCONFIG()->GetLastCalFlow();
	CStringW szLastFlowCal=_T("");
	COleDateTime dtTimeLastFlowCal;
	if(szTimeLastFlowCal == _T(""))
	{
		theApp.getLog()->WriteLine(_T("*** last Flowcal: unknown"));
	}
	else
	{
		if(false==dtTimeLastFlowCal.ParseDateTime(szTimeLastFlowCal,LOCALE_NOUSEROVERRIDE))
		{
			theApp.getLog()->WriteLine(_T("*** last Flowcal: unknown"));
		}
		else
		{
			szLastFlowCal.Format(_T("***last Flowcal:%02d.%02d.%04d %02d:%02d:%02d***"),
				dtTimeLastFlowCal.GetDay(),
				dtTimeLastFlowCal.GetMonth(),
				dtTimeLastFlowCal.GetYear(),
				dtTimeLastFlowCal.GetHour(),
				dtTimeLastFlowCal.GetMinute(),
				dtTimeLastFlowCal.GetSecond());
			theApp.getLog()->WriteLine(szLastFlowCal);
		}
	}

	
}

void CDataHandler::writeDataBuffer(const PBUFSPI& pBufData)
{
	/*EnterCriticalSection(&csSPIDataBuffer);
	m_rbufSPI.write(pBufData);
	LeaveCriticalSection(&csSPIDataBuffer);

	EnterCriticalSection(&csVentDataBuffer);
	m_rbufVent.write(pBufData);
	LeaveCriticalSection(&csVentDataBuffer);*/

	EnterCriticalSection(&csSPIDataBuffer);
	EnterCriticalSection(&csVentDataBuffer);
	m_rbufSPI.write(pBufData);
	m_rbufVent.write(pBufData);
	//int iWriteIndex=m_rbufVent.getWriteIndex();
	LeaveCriticalSection(&csVentDataBuffer);
	LeaveCriticalSection(&csSPIDataBuffer);

	/*if(bTestStart)
	{
		DEBUGMSG(TRUE, (TEXT("write %d %d\r\n"),iWriteIndex, pBufData.iValPressure));
	}*/
	
}

void CDataHandler::writeSPO2Buffer(const PBUFSPO2& pBufData)
{
	EnterCriticalSection(&csSPO2DataBuffer);
	m_rbufSPO2.write(pBufData);
	LeaveCriticalSection(&csSPO2DataBuffer);
}

void CDataHandler::writeSPO2waveData(SHORT iVal)
{
	EnterCriticalSection(&csSPO2DataBuffer);
	m_iSPO2waveData=iVal;
	LeaveCriticalSection(&csSPO2DataBuffer);
}

SHORT CDataHandler::readSPO2waveData()
{
	EnterCriticalSection(&csSPO2DataBuffer);
	SHORT iTemp=m_iSPO2waveData;
	LeaveCriticalSection(&csSPO2DataBuffer);
	return iTemp;
}

void CDataHandler::writeSpO2SIQ(SHORT iSIQvalue)
{
	getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_SPO2_SIQ,iSIQvalue);

	if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
	{
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SPO2_SIQDATA);
	}
	
}

SHORT CDataHandler::readSpO2SIQ()
{
	return getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_SPO2_SIQ);
}

//void CDataHandler::writeEMGBuffer(const PBUFEMG& pBufData)
//{
//	EnterCriticalSection(&csEMGDataBuffer);
//	m_rbufEMG.write(pBufData);
//	LeaveCriticalSection(&csEMGDataBuffer);
//}
//
//void CDataHandler::writeEMGwaveData(SHORT iVal)
//{
//	EnterCriticalSection(&csEMGDataBuffer);
//	m_iEMGwaveData=iVal;
//	LeaveCriticalSection(&csEMGDataBuffer);
//}
//
//SHORT CDataHandler::readEMGwaveData()
//{
//	EnterCriticalSection(&csEMGDataBuffer);
//	SHORT iTemp=m_iEMGwaveData;
//	LeaveCriticalSection(&csEMGDataBuffer);
//	return iTemp;
//}

//void CDataHandler::setSpO2InterpolationValues(SPO2_INTERPOLATION* pStruct)
//{
//	memcpy(&m_structSpO2Interpolation,pStruct,sizeof(SPO2_INTERPOLATION));
//}



//void CDataHandler::writeCO2Buffer(const PBUFCO2& pBufData)
void CDataHandler::writeCO2Buffer(const SHORT& pBufData)
{
	EnterCriticalSection(&csCO2DataBuffer);
	m_rbufCO2.write(pBufData);
	LeaveCriticalSection(&csCO2DataBuffer);
}

void CDataHandler::writeCopyDataBuffer(const PBUFSPI& pBufData)
{
	EnterCriticalSection(&csCopyDataBuffer);
	m_rbufCopy.write(pBufData);
	LeaveCriticalSection(&csCopyDataBuffer);
}

void CDataHandler::updateCopyDataBuffer()
{
	EnterCriticalSection(&csVentDataBuffer);
	EnterCriticalSection(&csCopyDataBuffer);
	m_rbufCopy=m_rbufVent;
	
	//int iWriteIndex1=m_rbufVent.getWriteIndex();
	//SHORT iVal1=m_rbufVent[0].iValPressure;

	//int iWriteIndex2=m_rbufCopy.getWriteIndex();
	//SHORT iVal2=m_rbufCopy[0].iValPressure;

	LeaveCriticalSection(&csCopyDataBuffer);
	LeaveCriticalSection(&csVentDataBuffer);

	//DEBUGMSG(TRUE, (TEXT("write1 %d %d\r\n"),iWriteIndex1, iVal1));
	//DEBUGMSG(TRUE, (TEXT("write2 %d %d\r\n"),iWriteIndex2, iVal2));
}

//void CDataHandler::updateFOTDataBuffer()
//{
//	static PBUFFOT m_abufFOT[];
//
//	updateCopyDataBuffer();
//	int iCountVal=MAXSIZE_FOT_BUFFER;
//
//	m_rbufFOT.reset();//MAXSIZE_FOT_BUFFER
//	EnterCriticalSection(&csCopyDataBuffer);
//	EnterCriticalSection(&csFOTDataBuffer);
//	if(m_rbufCopy.getCount()<MAXSIZE_FOT_BUFFER)
//		iCountVal=m_rbufCopy.getCount();
//	for (int iCnt=0;iCnt<iCountVal;iCnt++)
//	{
//		m_rbufFOT[iCnt].iValPressure=m_rbufCopy[iCnt].iValPressure;
//		m_rbufFOT[iCnt].iValFlow=m_rbufCopy[iCnt].iValFlow();
//	}
//	LeaveCriticalSection(&csFOTDataBuffer);
//	LeaveCriticalSection(&csCopyDataBuffer);
//}




BYTE CDataHandler::setBitOfByte(BYTE x, unsigned int n)
{
	x = x | (1 << n);

	return x;
}

WORD CDataHandler::setBitOfWord(WORD x, unsigned int n)
{
	x = x | (1 << n);

	return x;
}

// **************************************************************************
// 
// **************************************************************************
WORD CDataHandler::getHWconfig()
{
	return m_wHardwareConfig;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::start()
{
	m_PRESET = CParaDataPRESET::GetInstance();
	m_PARADATA = CParaData::GetInstance();

	//send HARDWARE CONFIGURATION to SPI and SERIAL
	BYTE mainVersion=getModel()->getCONFIG()->GetMainBoardVersion();
	//BYTE iSystemHWrevision=getModel()->getCONFIG()->getSystemHWrevision();
	//eDevType devType=getModel()->getCONFIG()->getDevType();
	eFlowSensorType flowSensorType=getModel()->getCONFIG()->getFlowSensorType();
	eAVRevision AVRevision=getModel()->getCONFIG()->getAVRevision();
	eHFModuleRevision HFRevision=getModel()->getCONFIG()->getHFModuleRevision();

	int iMask=mainVersion;
	iMask=iMask << 7;
	m_wHardwareConfig = m_wHardwareConfig | iMask;

	//range of flow sensor, 1=only NEONATAL
	if(flowSensorType==FSENS_NEONATAL)
		m_wHardwareConfig=setBitOfWord(m_wHardwareConfig, FLOWSENSORRANGE_BIT);

	//device type HFO
	m_wHardwareConfig=setBitOfWord(m_wHardwareConfig, DEVICETYPE_HFO_BIT);

	//patient gas supply, 0=Revision1-3, 1=Revision4
	if(AVRevision==AVREV_4)
		m_wHardwareConfig=setBitOfWord(m_wHardwareConfig, PATIENTGASSUPPLY1_BIT);

	if(HFRevision==HFREV_2)
		m_wHardwareConfig=setBitOfWord(m_wHardwareConfig, HFOMODULE_BIT);

	/*HARDWARE CONFIG is the start command for CTL PIC!!!!*/
	getModel()->getSERIAL()->Send_HARDWARE_CONFIG(m_wHardwareConfig);
	getModel()->getSPI()->Send_HARDWARE_CONFIG(m_wHardwareConfig);


	setMainboardData();

	//CStringW szName = getModel()->getCONFIG()->GetPatientName();
	int iPatientID = getModel()->getCONFIG()->GetPatientID();

	/*if(szName!=_T(""))
		m_bPatientDataAvailable =true;
	else */
	if(iPatientID!=0)
		m_bPatientDataAvailable =true;

	if(getModel()->getCONFIG()->IsEFLOWequalILFOW() && getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW)
	{
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SET_EFLOWEQUIFLOW);
	}

	
}





// **************************************************************************
// 
// **************************************************************************
eTubeSet CDataHandler::GetTubeSet()
{
	return getModel()->getCONFIG()->GetTubeSet();
}
void CDataHandler::SetTubeSet(eTubeSet tube)
{
	getModel()->getCONFIG()->SetTubeSet(tube);

	eVentMode actMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

	if(tube==TUBE_INFANTFLOW)
	{
		getModel()->getCONFIG()->setTriggerOption_NCPAP(TRIGGER_FLOW);
		getModel()->getCONFIG()->setTriggerOption_DUOPAP(TRIGGER_FLOW);

		if(		GetFlowSensorState()==FLOWSENSOR_OFF 
			&&	GetPrevFlowSensorState()==FLOWSENSOR_ON)
		{
			if(actMode==VM_DUOPAP || actMode==VM_NCPAP)
			{
				if(true==getModel()->getDATAHANDLER()->isNIVTRIGGERAvailable())
					SetFlowSensorState(FLOWSENSOR_ON);
			}
			else
				SetFlowSensorState(FLOWSENSOR_ON);
		}
	}
	else if(tube==TUBE_INFANTFLOW_LP)
	{
		getModel()->getCONFIG()->setTriggerOption_NCPAP(TRIGGER_FLOW);
		getModel()->getCONFIG()->setTriggerOption_DUOPAP(TRIGGER_FLOW);

		if(		GetFlowSensorState()==FLOWSENSOR_OFF 
			&&	GetPrevFlowSensorState()==FLOWSENSOR_ON)
		{
			if(actMode==VM_DUOPAP || actMode==VM_NCPAP)
			{
				if(true==getModel()->getDATAHANDLER()->isNIVTRIGGERAvailable())
					SetFlowSensorState(FLOWSENSOR_ON);
			}
			else
				SetFlowSensorState(FLOWSENSOR_ON);
		}
	}
	else if(tube==TUBE_MEDIJET)
	{
		getModel()->getCONFIG()->setTriggerOption_NCPAP(TRIGGER_PRESSURE);
		getModel()->getCONFIG()->setTriggerOption_DUOPAP(TRIGGER_PRESSURE);

		if(GetFlowSensorState()==FLOWSENSOR_ON)
		{
			SetFlowSensorState(FLOWSENSOR_OFF);
		}
	}
	
	checkTriggerTubeDependency();

	getModel()->Send_MODE_OPTION1();
}




//************************************
// Method:    GetFlowSensorCalState
// FullName:  CDataHandler::GetFlowSensorCalState
// Access:    public 
// Returns:   eFlowSensCalibrationstate
// Qualifier:
//
//  2015/07/02: flow sensor handling checked for thread safe
//************************************
eFlowSensCalibrationstate CDataHandler::GetFlowSensorCalState()
{
	EnterCriticalSection(&csFlowsensorState);
	eFlowSensCalibrationstate eState=m_eFlowSensorCalState;
	LeaveCriticalSection(&csFlowsensorState);

	return eState;
}
//************************************
// Method:    SetFlowSensorCalState
// FullName:  CDataHandler::SetFlowSensorCalState
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: eFlowSensCalibrationstate state
//
//  2015/07/02: flow sensor handling checked for thread safe
//************************************
void CDataHandler::SetFlowSensorCalState(eFlowSensCalibrationstate state)
{
	EnterCriticalSection(&csFlowsensorState);
	m_eFlowSensorCalState=state;
	LeaveCriticalSection(&csFlowsensorState);
}

/**********************************************************************************************//**
 * @fn	void CDataHandler::SetPrevFlowSensorState(eFlowSensorState prevState)
 *
 * @brief	Sets previous flow sensor state.
 *
 * @author	Rainer Kuehner
 * @date	10.01.2017
 *
 * @param	prevState	State of the previous.
 **************************************************************************************************/
void CDataHandler::SetPrevFlowSensorState(eFlowSensorState prevState)
{
	m_ePrevFlowSensorState=prevState;
}

/**********************************************************************************************//**
 * @fn	eFlowSensorState CDataHandler::GetPrevFlowSensorState()
 *
 * @brief	Gets the previous flow sensor state.
 *
 * @author	Rainer Kuehner
 * @date	10.01.2017
 *
 * @return	The previous flow sensor state.
 **************************************************************************************************/
eFlowSensorState CDataHandler::GetPrevFlowSensorState()
{
	return m_ePrevFlowSensorState;
}
//************************************
// Method:    IsFlowSensorCalibrating
// FullName:  CDataHandler::IsFlowSensorCalibrating
// Access:    public 
// Returns:   bool
// Qualifier:
//
//  2015/07/02: flow sensor handling checked for thread safe
//************************************
bool CDataHandler::IsFlowSensorCalibrating()
{
	EnterCriticalSection(&csFlowsensorState);
	bool bstate=m_bFlowSensorCalState;
	LeaveCriticalSection(&csFlowsensorState);
	return bstate;
}
//************************************
// Method:    SetFlowSensorCalibrating
// FullName:  CDataHandler::SetFlowSensorCalibrating
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: bool state
//
//  2015/07/02: flow sensor handling checked for thread safe
//************************************
void CDataHandler::SetFlowSensorCalibrating(bool state)
{
	EnterCriticalSection(&csFlowsensorState);
	m_bFlowSensorCalState=state;
	LeaveCriticalSection(&csFlowsensorState);
	getModel()->Send_MODE_OPTION1();
}
//************************************
// Method:    IsFlowSensorStateOff
// FullName:  CDataHandler::IsFlowSensorStateOff
// Access:    public 
// Returns:   bool
// Qualifier:
//
//  2015/07/02: flow sensor handling checked for thread safe
//************************************
bool CDataHandler::IsFlowSensorStateOff()
{
	bool bRes=false;
	EnterCriticalSection(&csFlowsensorState);
	if(		m_eFlowSensorState==FLOWSENSOR_OFF
		||	m_eFlowSensorState==FLOWSENSOR_MANOFF
		||	m_eFlowSensorState==FLOWSENSOR_AUTOOFF)
		bRes=true;
	LeaveCriticalSection(&csFlowsensorState);

	return bRes;
}
//************************************
// Method:    GetFlowSensorState
// FullName:  CDataHandler::GetFlowSensorState
// Access:    public 
// Returns:   eFlowSensorState
// Qualifier:
//
//  2015/07/02: flow sensor handling checked for thread safe
//************************************
eFlowSensorState CDataHandler::GetFlowSensorState()
{
	EnterCriticalSection(&csFlowsensorState);
	eFlowSensorState eState=m_eFlowSensorState;
	LeaveCriticalSection(&csFlowsensorState);
	return eState;
}
//************************************
// Method:    SetFlowSensorState
// FullName:  CDataHandler::SetFlowSensorState
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: eFlowSensorState state
//
//  2015/07/02: flow sensor handling checked for thread safe
//************************************
void CDataHandler::SetFlowSensorState(eFlowSensorState state)
{
	bool bDeleteFlowDependAlarms=false;
	eFlowSensorState prevState=FLOWSENSOR_ON;
	EnterCriticalSection(&csFlowsensorState);
	prevState=m_eFlowSensorState;
	m_eFlowSensorState=state;
	LeaveCriticalSection(&csFlowsensorState);

	SetPrevFlowSensorState(prevState);

	getModel()->getCONFIG()->SetFlowSensorState(state);

	switch(state)
	{
	case FLOWSENSOR_AUTOOFF:
		{
			DEBUGMSG(TRUE, (TEXT("FLOWSENSOR_AUTOOFF\r\n")));
			if(IsActiveModeVGarantStateOn())
			{
				getModel()->getALARMHANDLER()->setVgarantAutoTurnedOff_FlowSensor();
				getModel()->getALARMHANDLER()->resetVlimitAutoTurnedOff_FlowSensor();

				ChangeVgarantState(VOLUMEGAR_PARA_AUTODISABLED);
			}
			else if(IsActiveModeVLimitStateOn())
			{
				SaveLastVLimitParam();
				
				if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					SetVLimitParadata_IPPV(PARADATA()->GetVLimitParam_IPPV(),false,true);
				else
					SetVLimitParadata_TRIGGER(PARADATA()->GetVLimitParam_TRIGGER(),false,true);
				getModel()->getALARMHANDLER()->setVlimitAutoTurnedOff_FlowSensor();
				getModel()->getALARMHANDLER()->resetVgarantAutoTurnedOff_FlowSensor();
				getModel()->getALARMHANDLER()->resetVgAutoTurnedOff_FOT();

				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_VLIMIT_PARA_AUTODISABLED);
				DEBUGMSG(TRUE, (TEXT("WM_VLIMIT_PARA_AUTODISABLED 5 ###\r\n")));
			}
			else
			{
				getModel()->getALARMHANDLER()->resetVgVlAutoTurnedOff_FlowSensor();
			}
			theApp.getLog()->WriteLine(_T("***SetFlowSensorState FLOWSENSOR_AUTOOFF"));

			bDeleteFlowDependAlarms=true;
		}
		break;
	case FLOWSENSOR_MANOFF:
		{
			DEBUGMSG(TRUE, (TEXT("FLOWSENSOR_MANOFF\r\n")));
			theApp.getLog()->WriteLine(_T("***SetFlowSensorState FLOWSENSOR_MANOFF"));
			getModel()->getALARMHANDLER()->resetVgVlAutoTurnedOff();

			bDeleteFlowDependAlarms=true;
		}
		break;
	case FLOWSENSOR_OFF:
		{
			DEBUGMSG(TRUE, (TEXT("FLOWSENSOR_OFF\r\n")));
			theApp.getLog()->WriteLine(_T("***SetFlowSensorState FLOWSENSOR_OFF"));
			getModel()->getALARMHANDLER()->resetVgVlAutoTurnedOff();

			bDeleteFlowDependAlarms=true;
		}
		break;
	case FLOWSENSOR_ON:
		{
			DEBUGMSG(TRUE, (TEXT("FLOWSENSOR_ON\r\n")));
			theApp.getLog()->WriteLine(_T("***SetFlowSensorState FLOWSENSOR_ON"));
		}
		break;
	}

	if(getModel()->isSPIavailabel())
	{
		int iMode=0;

		if(		state==FLOWSENSOR_AUTOOFF
			||	state==FLOWSENSOR_MANOFF
			||	state==FLOWSENSOR_OFF)
			iMode=CMD_FLOWSENSOROFF;
		else 
			iMode=CMD_FLOWSENSORON;

		getModel()->getSPI()->Send_FLOWSENS_CMND(iMode);

		Sleep(50);

		int iSPIstate=getModel()->getSPI()->Read_FLOWSENS_CMND();
		
		bool bTimeout=false;
		BYTE iCnt=0;

		while(bTimeout==false && iSPIstate == iMode)//rkuTICKCOUNT
		{
			if(iCnt>=25)
				bTimeout=true;

			Sleep(20);
			iSPIstate=getModel()->getSPI()->Read_FLOWSENS_CMND();
			iCnt++;
		}

		if(bTimeout || iSPIstate!=0)
		{
			if(getModel()->getALARMHANDLER()->CanSetAlarm_IF_SPI())
			{
				theApp.getLog()->WriteLine(_T("#HFO:0073"));

				setSPIErrorCode(ERRC_SPI_SET_FLOWSENSSTATE);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
			}
			return;
		}
		/*else //newSPI
		{
			DeleteSPIErrorCode(ERRC_SPI_SET_FLOWSENSSTATE);
		}*/
	}

	if(bDeleteFlowDependAlarms)
	{
		if(getModel()->getALARMHANDLER()->ALARM_PatAl_MVmax->getAlarmState()!=AS_NONE)
			getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_MVmax);

		if(getModel()->getALARMHANDLER()->ALARM_PatAl_MVmin->getAlarmState()!=AS_NONE)
			getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_MVmin);

		if(getModel()->getFOTThread())
		{
			if(getModel()->getFOTThread()->getFOTstate()!=FOT_OFF)
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_STOP_FOT);
			}
		}
	}

	//reset conventional trigger to previous
	if(state==FLOWSENSOR_ON)
	{
		if(getModel()->getVMODEHANDLER()->activeModeIsCPAP())
		{

		}
		else if(getPrevTriggerOptionCONV()!=TRIGGER_NONE)
		{
			setTriggerOptionCONV(getPrevTriggerOptionCONV());
			SetPrevTriggerOptionCONV(TRIGGER_NONE);
		}
	}

	checkTriggerTubeDependency();

	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_FLOWSENSORSTATE);
}
void CDataHandler::checkPRESETTriggerTubeDependency()
{
	if(VM_PRE_DUOPAP==getModel()->getCONFIG()->GetCurMode())
	{
		if(		true==getModel()->getDATAHANDLER()->isNIVTRIGGERAvailable())
		{
			if(GetTubeSet()==TUBE_MEDIJET)//pressure trigger
			{
				if(		PRESET()->GetTriggerPara_DUOPAP()!=MAXRANGE_TRIGGER_OFF 
					&& PRESET()->GetITimeNMODEPara()>600)
				{
					SetDUOPAPtriggerAutoenableFlag(PRESET()->GetTriggerPara_DUOPAP());
					PRESET()->SetTriggerPara_DUOPAP(MAXRANGE_TRIGGER_OFF, false, false);

					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
				}
				else if(false==GetTriggerDUOPAPenabled())
				{
					if(		PRESET()->GetTriggerPara_DUOPAP()==MAXRANGE_TRIGGER_OFF 
						&&	PRESET()->GetITimeNMODEPara()<=600)
					{
						if(AfxGetApp() != NULL)
							AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
					}
				}
				else if(	true==GetTriggerDUOPAPenabled() 
					&& PRESET()->GetITimeNMODEPara()>600)
				{
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
				}
			}
			else//flow trigger
			{
				if(IsFlowSensorStateOff()==true)
				{
					/*if(PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
					{
						if(AfxGetApp() != NULL)
							AfxGetApp()->GetMainWnd()->PostMessage(WM_TURNOFF_TRIGGERNMODE);
					}
					if(getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit()!=AL_OFF)
					{
						if(AfxGetApp() != NULL)
							AfxGetApp()->GetMainWnd()->PostMessage(WM_TURNOFF_APNEA);
					}*/
				}
				else
				{
					if(		PRESET()->GetTriggerPara_DUOPAP()!=MAXRANGE_TRIGGER_OFF 
						&& PRESET()->GetITimeNMODEPara()>600)
					{
						SetDUOPAPtriggerAutoenableFlag(PRESET()->GetTriggerPara_DUOPAP());
						PRESET()->SetTriggerPara_DUOPAP(MAXRANGE_TRIGGER_OFF, false, false);

						if(AfxGetApp() != NULL)
							AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
					}
					else if(GetDUOPAPtriggerAutoenableFlag())
					{
						if(		PRESET()->GetTriggerPara_DUOPAP()==MAXRANGE_TRIGGER_OFF 
							&&	PRESET()->GetITimeNMODEPara()<=600)
						{
							SetTriggerDUOPAPParadata(GetPrevTRIGGERDUOPAPPara(),true);
							ResetDUOPAPtriggerAutoenableFlag();

							if(AfxGetApp() != NULL)
								AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
						}
					}
					else if(false==GetTriggerDUOPAPenabled())
					{
						if(		PRESET()->GetTriggerPara_DUOPAP()==MAXRANGE_TRIGGER_OFF 
							&&	PRESET()->GetITimeNMODEPara()<=600)
						{
							if(AfxGetApp() != NULL)
								AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
						}
					}
					else if(	true==GetTriggerDUOPAPenabled() 
						&& PRESET()->GetITimeNMODEPara()>600)
					{
						if(AfxGetApp() != NULL)
							AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
					}
				}
			}
		}
	}
}
void CDataHandler::checkTriggerTubeDependency()
{
	//DEBUGMSG(TRUE, (TEXT("checkTriggerTubeDependency\r\n")));

	switch(getModel()->getVMODEHANDLER()->getCurrentActiveMode())
	{
	case VM_DUOPAP:
		{
			//DEBUGMSG(TRUE, (TEXT("checkTriggerTubeDependency_DUOPAP\r\n")));
			checkTriggerTubeDependency_DUOPAP();
		}
		break;
	case VM_NCPAP:
		{
			//DEBUGMSG(TRUE, (TEXT("checkTriggerTubeDependency_NCPAP\r\n")));
			checkTriggerTubeDependency_NCPAP();
		}
		break;
	case VM_CPAP:
		{
			//DEBUGMSG(TRUE, (TEXT("checkTriggerTubeDependency_CPAP\r\n")));
			checkTriggerTubeDependency_CPAP();
		}
		break;
	case VM_IPPV:
		{
			//DEBUGMSG(TRUE, (TEXT("checkTriggerTubeDependency_IPPV\r\n")));
			checkTriggerTubeDependency_IPPV();
		}
		break;
	case VM_SIPPV:
		{
			//DEBUGMSG(TRUE, (TEXT("checkTriggerTubeDependency_SIPPV\r\n")));
			checkTriggerTubeDependency_SIPPV();
		}
		break;
	case VM_SIMV:
		{
			//DEBUGMSG(TRUE, (TEXT("checkTriggerTubeDependency_SIMV\r\n")));
			checkTriggerTubeDependency_SIMV();
		}
		break;
	}
}

void CDataHandler::checkTriggerTubeDependency_DUOPAP()
{
	if(true==getModel()->getDATAHANDLER()->isNIVTRIGGERAvailable())
	{
		if(GetTubeSet()==TUBE_MEDIJET)//pressure trigger
		{
			if(		PARADATA()->GetTriggerPara_DUOPAP()!=MAXRANGE_TRIGGER_OFF 
				&& PARADATA()->GetITimeNMODEPara()>600)
			{
				SetDUOPAPtriggerAutoenableFlag(PARADATA()->GetTriggerPara_DUOPAP());
				SetTriggerDUOPAPParadata(MAXRANGE_TRIGGER_OFF,true);

				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);

				if(getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit()!=AL_OFF)
				{
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_TURNOFF_APNEA);
				}
			}
			else if(GetDUOPAPtriggerAutoenableFlag())
			{
				if(		PARADATA()->GetTriggerPara_DUOPAP()==MAXRANGE_TRIGGER_OFF 
					&&	PARADATA()->GetITimeNMODEPara()<=600)
				{
					SetTriggerDUOPAPParadata(GetPrevTRIGGERDUOPAPPara(),true);
					ResetDUOPAPtriggerAutoenableFlag();

					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
				}
			}
			else if(PARADATA()->GetTriggerPara_DUOPAP()==MAXRANGE_TRIGGER_OFF && getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit()!=AL_OFF)
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TURNOFF_APNEA);
			}
			else if(false==GetTriggerDUOPAPenabled())
			{
				if(		PARADATA()->GetTriggerPara_DUOPAP()==MAXRANGE_TRIGGER_OFF
					&&	PARADATA()->GetITimeNMODEPara()<=600)
				{
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
				}
			}
			else if(	true==GetTriggerDUOPAPenabled() 
				&& PARADATA()->GetITimeNMODEPara()>600)
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
			}
		}
		else//flow trigger
		{
			if(IsFlowSensorStateOff()==true)
			{
				if(PARADATA()->GetTriggerPara_DUOPAP()!=MAXRANGE_TRIGGER_OFF)
				{
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_TURNOFF_TRIGGER_DUOPAP);
				}
				if(getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit()!=AL_OFF)
				{
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_TURNOFF_APNEA);
				}
			}
			else
			{
				if(		PARADATA()->GetTriggerPara_DUOPAP()!=MAXRANGE_TRIGGER_OFF 
					&& PARADATA()->GetITimeNMODEPara()>600)
				{
					SetDUOPAPtriggerAutoenableFlag(PARADATA()->GetTriggerPara_DUOPAP());
					SetTriggerDUOPAPParadata(MAXRANGE_TRIGGER_OFF,true);

					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);

					if(getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit()!=AL_OFF)
					{
						if(AfxGetApp() != NULL)
							AfxGetApp()->GetMainWnd()->PostMessage(WM_TURNOFF_APNEA);
					}
				}
				else if(GetDUOPAPtriggerAutoenableFlag())
				{
					if(		PARADATA()->GetTriggerPara_DUOPAP()==MAXRANGE_TRIGGER_OFF 
						&&	PARADATA()->GetITimeNMODEPara()<=600)
					{
						SetTriggerDUOPAPParadata(GetPrevTRIGGERDUOPAPPara(),true);
						ResetDUOPAPtriggerAutoenableFlag();

						if(AfxGetApp() != NULL)
							AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
					}
				}
				else if(PARADATA()->GetTriggerPara_DUOPAP()==MAXRANGE_TRIGGER_OFF && getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit()!=AL_OFF)
				{
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_TURNOFF_APNEA);
				}
				else if(false==GetTriggerDUOPAPenabled())
				{
					if(		PARADATA()->GetTriggerPara_DUOPAP()==MAXRANGE_TRIGGER_OFF 
						&&	PARADATA()->GetITimeNMODEPara()<=600)
					{
						if(AfxGetApp() != NULL)
							AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
					}
				}
				else if(	true==GetTriggerDUOPAPenabled() 
					&& PARADATA()->GetITimeNMODEPara()>600)
				{
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
				}
			}
		}
	}
	else
	{
		if(getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit()!=AL_OFF)
		{
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_TURNOFF_APNEA);
		}
	}
}
void CDataHandler::checkTriggerTubeDependency_NCPAP()
{
	if(true==getModel()->getDATAHANDLER()->isNIVTRIGGERAvailable())
	{
		if(GetTubeSet()==TUBE_MEDIJET)//pressure trigger
		{
			if(PARADATA()->GetTriggerPara_NCPAP()!=TRIGGER_SENSITIVITY_AUTO)
			{
				SetTriggerNCPAPParadata(TRIGGER_SENSITIVITY_AUTO,true);

				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
			}
		}
		else//flow trigger
		{
			if(IsFlowSensorStateOff()==true)
			{
				if(PARADATA()->GetTriggerPara_NCPAP()!=MAXRANGE_TRIGGER_OFF)
				{
					SetTriggerNCPAPParadata(MAXRANGE_TRIGGER_OFF,true);

					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);

					//if(AfxGetApp() != NULL)
					//	AfxGetApp()->GetMainWnd()->PostMessage(WM_TURNOFF_TRIGGER_NCPAP);//send MAXRANGE_TRIGGER_OFF
				}
				if(getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit()!=AL_OFF)
				{
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_TURNOFF_APNEA);
				}
			}
			else
			{
				if(PARADATA()->GetTriggerPara_NCPAP()!=TRIGGER_SENSITIVITY_AUTO)
				{
					SetTriggerNCPAPParadata(TRIGGER_SENSITIVITY_AUTO,true);

					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
				}
			}
		}
	}
	else
	{
		if(getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit()!=AL_OFF)
		{
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_TURNOFF_APNEA);
		}
	}
}
void CDataHandler::checkTriggerTubeDependency_CPAP()
{
	if(		IsFlowSensorStateOff()==true
		&&	getTriggerOptionCONV()!=TRIGGER_PRESSURE)
	{
		SetTriggerCONVParadata(TRIGGER_SENSITIVITY_AUTO,true);

		//SetPrevTriggerOptionCONV(getTriggerOptionCONV());
		setTriggerOptionCONV(TRIGGER_PRESSURE);
		getModel()->Send_MODE_OPTION1();

		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
	}
	else if(		IsFlowSensorStateOff()==false
		&&	getTriggerOptionCONV()==TRIGGER_PRESSURE)
	{
		SetTriggerCONVParadata(TRIGGER_SENSITIVITY_AUTO,true);

		//SetPrevTriggerOptionCONV(getTriggerOptionCONV());
		setTriggerOptionCONV(TRIGGER_FLOW);
		getModel()->Send_MODE_OPTION1();

		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_STATE);
	}

	if(PARADATA()->GetTriggerPara_CPAP()!=TRIGGER_SENSITIVITY_AUTO)
	{
		SetTriggerCONVParadata(TRIGGER_SENSITIVITY_AUTO,true);
	}
}
void CDataHandler::checkTriggerTubeDependency_IPPV()
{
	if(		GetFlowSensorState()==FLOWSENSOR_MANOFF
		&&	getTriggerOptionCONV()!=TRIGGER_PRESSURE)
	{
		SetPrevTriggerOptionCONV(getTriggerOptionCONV());
		setTriggerOptionCONV(TRIGGER_PRESSURE);
		getModel()->Send_MODE_OPTION1();
	}
}
void CDataHandler::checkTriggerTubeDependency_SIPPV()
{
	if(		GetFlowSensorState()==FLOWSENSOR_MANOFF
		&&	getTriggerOptionCONV()!=TRIGGER_PRESSURE)
	{
		SetPrevTriggerOptionCONV(getTriggerOptionCONV());
		setTriggerOptionCONV(TRIGGER_PRESSURE);
		getModel()->Send_MODE_OPTION1();
	}
}
void CDataHandler::checkTriggerTubeDependency_SIMV()
{
	if(		GetFlowSensorState()==FLOWSENSOR_MANOFF
		&&	getTriggerOptionCONV()!=TRIGGER_PRESSURE)
	{
		SetPrevTriggerOptionCONV(getTriggerOptionCONV());
		setTriggerOptionCONV(TRIGGER_PRESSURE);
		getModel()->Send_MODE_OPTION1();
	}
}

void CDataHandler::SetTriggerDUOPAPenabled()
{
	m_bTriggerDUOPAPenabled=true;
}
void CDataHandler::SetTriggerDUOPAPdisabled()
{
	m_bTriggerDUOPAPenabled=false;
}
bool CDataHandler::GetTriggerDUOPAPenabled()
{
	return m_bTriggerDUOPAPenabled;
}


//void CDataHandler::SetTriggerNCPAPenabled()
//{
//	m_bTriggerNCPAPenabled=true;
//}
//void CDataHandler::SetTriggerNCPAPdisabled()
//{
//	m_bTriggerNCPAPenabled=false;
//}
//bool CDataHandler::GetTriggerNCPAPenabled()
//{
//	return m_bTriggerNCPAPenabled;
//}
//***************************************************************************
//      
//**************************************************************************
void CDataHandler::SetVentilBurnRunning(bool state)
{
	m_bVentilBurnRunning=state;
}
bool CDataHandler::IsVentilBurnRunning()
{
	return m_bVentilBurnRunning;
}

//************************************
// Method:    GetOxyCalRunning
// FullName:  CDataHandler::GetOxyCalRunning
// Access:    public 
// Returns:   bool
// Qualifier:
//
//  2015/07/02: o2 handling checked for thread safe
//************************************
bool CDataHandler::GetOxyCalRunning()
{
	EnterCriticalSection(&csOxyCal);
	bool bState=m_bOxyCalRunning;
	LeaveCriticalSection(&csOxyCal);
	return bState;
}
//************************************
// Method:    SetOxyCalRunning
// FullName:  CDataHandler::SetOxyCalRunning
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: bool state
//
//  2015/07/02: o2 handling checked for thread safe
//************************************
void CDataHandler::SetOxyCalRunning(bool state)
{
	/*if(state)
	{
		DEBUGMSG(TRUE, (TEXT("SetOxyCalRunning true\r\n")));
	}
	else
	{
		DEBUGMSG(TRUE, (TEXT("SetOxyCalRunning false\r\n")));
	}*/
	EnterCriticalSection(&csOxyCal);
	m_bOxyCalRunning=state;
	LeaveCriticalSection(&csOxyCal);

	if(state==false && getModel()->getPRICOThread())
	{
		if(getModel()->getPRICOThread()->isPRICOalgorithmRunning())
		{
			getModel()->getPRICOThread()->resetDelay();
		}
	}
	
}

void CDataHandler::SetOxyHourglassRunning(bool state)
{
	/*if(state)
	{
		DEBUGMSG(TRUE, (TEXT("SetOxyHourglassRunning true\r\n")));
	}
	else
	{
		DEBUGMSG(TRUE, (TEXT("SetOxyHourglassRunning false\r\n")));
	}*/
	EnterCriticalSection(&csOxyCal);
	m_bOxyHourglassRunning=state;
	LeaveCriticalSection(&csOxyCal);
}

bool CDataHandler::isOxyHourglassRunning()
{
	bool bOxyHour=false;
	EnterCriticalSection(&csOxyCal);
	bOxyHour=m_bOxyHourglassRunning;
	LeaveCriticalSection(&csOxyCal);
	return bOxyHour;
}


//************************************
// Method:    GetOxySensorCalState
// FullName:  CDataHandler::GetOxySensorCalState
// Access:    public 
// Returns:   eOxySensCalibrationstate
// Qualifier:
//
//  2015/07/02: o2 handling checked for thread safe
//************************************
eOxySensCalibrationstate CDataHandler::GetOxySensorCalState()
{
	EnterCriticalSection(&csOxyCal);
	eOxySensCalibrationstate state = m_eOxySensorCalState;
	LeaveCriticalSection(&csOxyCal);
	return state;
}
//************************************
// Method:    SetOxySensorCalState
// FullName:  CDataHandler::SetOxySensorCalState
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: eOxySensCalibrationstate state
//
//  2015/07/02: o2 handling checked for thread safe
//************************************
void CDataHandler::SetOxySensorCalState(eOxySensCalibrationstate state)
{
	EnterCriticalSection(&csOxyCal);
	m_eOxySensorCalState=state;
	LeaveCriticalSection(&csOxyCal);
}

//************************************
// Method:    GetO2SensorState
// FullName:  CDataHandler::GetO2SensorState
// Access:    public 
// Returns:   eOxySensorState
// Qualifier:
//
//  2015/07/02: o2 handling checked for thread safe
//************************************
eOxySensorState CDataHandler::GetO2SensorState()
{
	EnterCriticalSection(&csOxyState);
	eOxySensorState state = m_eO2SensorState;
	LeaveCriticalSection(&csOxyState);
	return state;
}
//************************************
// Method:    SetO2SensorState
// FullName:  CDataHandler::SetO2SensorState
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: eOxySensorState state
//
//  2015/07/02: o2 handling checked for thread safe
//************************************
void CDataHandler::SetO2SensorState(eOxySensorState state)
{
	EnterCriticalSection(&csOxyState);
	m_eO2SensorState=state;
	LeaveCriticalSection(&csOxyState);

	if(state==OXYSENS_OFF)
	{
		getModel()->getAcuLink()->setParaData(ALINK_SETT_STATE_OXYSENSOR,0);
	}
	else
	{
		getModel()->getAcuLink()->setParaData(ALINK_SETT_STATE_OXYSENSOR,1);
	}
}

//************************************
// Method:    GetO21SensorCalState
// FullName:  CDataHandler::GetO21SensorCalState
// Access:    public 
// Returns:   bool
// Qualifier:
//
//  2015/07/02: o2 handling checked for thread safe
//************************************
bool CDataHandler::GetO21SensorCalState()
{
	EnterCriticalSection(&csOxyState);
	bool bState=m_bO21SensorCalState;
	LeaveCriticalSection(&csOxyState);
	return bState;
}
//************************************
// Method:    SetO21SensorCalState
// FullName:  CDataHandler::SetO21SensorCalState
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: bool state
//
//  2015/07/02: o2 handling checked for thread safe
//************************************
void CDataHandler::SetO21SensorCalState(bool state)
{
	EnterCriticalSection(&csOxyState);
	m_bO21SensorCalState=state;
	LeaveCriticalSection(&csOxyState);
}

//************************************
// Method:    GetO100SensorCalState
// FullName:  CDataHandler::GetO100SensorCalState
// Access:    public 
// Returns:   bool
// Qualifier:
//
//  2015/07/02: o2 handling checked for thread safe
//************************************
bool CDataHandler::GetO100SensorCalState()
{
	EnterCriticalSection(&csOxyState);
	bool bState=m_bO100SensorCalState;
	LeaveCriticalSection(&csOxyState);
	return bState;
}
//************************************
// Method:    SetO100SensorCalState
// FullName:  CDataHandler::SetO100SensorCalState
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: bool state
//
//  2015/07/02: o2 handling checked for thread safe
//************************************
void CDataHandler::SetO100SensorCalState(bool state)
{
	EnterCriticalSection(&csOxyState);
	m_bO100SensorCalState=state;
	LeaveCriticalSection(&csOxyState);
}



//***************************************************************************
//      
//**************************************************************************
void CDataHandler::SetExhalValvCalMode(bool state)
{
	m_bExhalValvCalMode=state;
}
bool CDataHandler::GetExhalValvCalMode()
{
	return m_bExhalValvCalMode;
}



//***************************************************************************
//      
//**************************************************************************
void CDataHandler::setMainboardData()
{
	EnterCriticalSection(&csMainboardData);
	m_dataMainboard.m_iMonitorChecksum=getModel()->getSPI()->Read_CHECKSUM();
	if(m_dataMainboard.m_iMonitorChecksum==-1)
	{
		theApp.getLog()->WriteLine(_T("#HFO:0068"));
	}

	WORD result;
	result=getModel()->getSPI()->Read_VERSION1();
	if(result==0xFF)
	{
		Sleep(100);
		result=getModel()->getSPI()->Read_VERSION1();

		theApp.getLog()->WriteLine(_T("#HFO:0069"));
	}
	else if(result==0)
	{
		theApp.getLog()->WriteLine(_T("#HFO:0070"));
	}
	int iXVal=LOBYTE(result);

	result=getModel()->getSPI()->Read_VERSION2();
	if(result==0xFF)
	{
		theApp.getLog()->WriteLine(_T("#HFO:0071"));
	}
	else if(result==0)
	{
		theApp.getLog()->WriteLine(_T("#HFO:0072"));
	}
	int iYVal=HIBYTE(result);
	int iZVal=LOBYTE(result);	

	m_dataMainboard.m_szHfoVersion=getModel()->getSERIAL()->GetHFO_VERSION();
	//m_dataMainboard.m_iHfoChecksum=getModel()->getSERIAL()->GetHFO_CHECKSUM();
	m_dataMainboard.m_iHfoChecksum=0;

	m_dataMainboard.m_iMonitorVersion_x=iXVal;
	m_dataMainboard.m_iMonitorVersion_y=iYVal;
	m_dataMainboard.m_iMonitorVersion_z=iZVal;
	m_dataMainboard.m_szConductorVersion=getModel()->getSERIAL()->GetM_VERSION();
	m_dataMainboard.m_iConductorChecksum=getModel()->getSERIAL()->GetM_CHECKSUM();
	m_dataMainboard.m_szBlenderVersion=_T("0.0");
	LeaveCriticalSection(&csMainboardData);

}
void CDataHandler::getMainboardData(MAINBOARD_DATA* pDataMainboard)
{
	EnterCriticalSection(&csMainboardData);
	//memcpy(pDataMainboard,&m_dataMainboard,sizeof(MAINBOARD_DATA));
	pDataMainboard->m_iConductorChecksum=m_dataMainboard.m_iConductorChecksum;
	pDataMainboard->m_szConductorVersion=m_dataMainboard.m_szConductorVersion;
	pDataMainboard->m_iMonitorChecksum=m_dataMainboard.m_iMonitorChecksum;
	pDataMainboard->m_iMonitorVersion_x=m_dataMainboard.m_iMonitorVersion_x;
	pDataMainboard->m_iMonitorVersion_y=m_dataMainboard.m_iMonitorVersion_y;
	pDataMainboard->m_iMonitorVersion_z=m_dataMainboard.m_iMonitorVersion_z;
	pDataMainboard->m_iHfoChecksum=m_dataMainboard.m_iHfoChecksum;
	pDataMainboard->m_szHfoVersion=m_dataMainboard.m_szHfoVersion;
	pDataMainboard->m_szBlenderVersion=m_dataMainboard.m_szBlenderVersion;
	LeaveCriticalSection(&csMainboardData);
}

void CDataHandler::setMessureDataAVG(BYTE type, INT val)
{
	EnterCriticalSection(&csMessureDataAVG);
	if(type<MAX_NUMERICVALUES)
	{
		m_pbufMessureAVG[type]=val;
	}
	LeaveCriticalSection(&csMessureDataAVG);
}

INT CDataHandler::getMessureDataAVG(BYTE type)
{
	INT iTemp=0;

	EnterCriticalSection(&csMessureDataAVG);
	if(type<MAX_NUMERICVALUES)
		iTemp=m_pbufMessureAVG[type];
	else
	{
		CStringW sz=_T("");
		sz.Format(_T("#HFO:0287 type%d"),type);
		theApp.getLog()->WriteLine(sz);
	}
	LeaveCriticalSection(&csMessureDataAVG);
	return iTemp;
}

void CDataHandler::setMessureDataAVGasTrendData()
{
	if(isTRENDLicenseAvailable())
	{
		int iHFAmpl=getMessureDataAVG(ALINK_MSMNT_HFO_AMP);
		int iMV=getMessureDataAVG(ALINK_MSMNT_MIN_VOL);
		int iDCO2=getMessureDataAVG(ALINK_MSMNT_DCO2);
		int iCompliance=getMessureDataAVG(ALINK_MSMNT_DYNCOMPL);
		int iTVE=getMessureDataAVG(ALINK_MSMNT_TVE);
		int iPmax=getMessureDataAVG(ALINK_MSMNT_P_PEAK);
		int iFiO2=getMessureDataO2();
		if(iFiO2<0)
			iFiO2=0;
		int iPmitt=getMessureDataAVG(ALINK_MSMNT_P_MEAN);

		int iBPM=getMessureDataAVG(ALINK_MSMNT_FREQ);
		int iTVEpat=getMessureDataAVG(ALINK_MSMNT_TVE_PATIENT);
		int iShareMVmand=getMessureDataAVG(ALINK_MSMNT_ANTEIL_RESP);
		int iResistance=getMessureDataAVG(ALINK_MSMNT_RESISTANCE);
		int iLeak=getMessureDataAVG(ALINK_MSMNT_LEAK);
		int iSpO2=getMessureDataAVG(ALINK_MSMNT_SPO2);
		int iPI=getMessureDataAVG(ALINK_MSMNT_SPO2_PI);
		int iEtCO2=getMessureDataAVG(ALINK_MSMNT_ETCO2);
		int iSpO2PR=getMessureDataAVG(ALINK_MSMNT_SPO2_PR);
		int iFreq=getMessureDataAVG(ALINK_MSMNT_FREQ);

		EnterCriticalSection(&csTrendFileData);
		m_fTrendData_HFAMP+=iHFAmpl;
		m_fTrendData_MV+=iMV;
		m_fTrendData_CO2HFO+=iDCO2;
		m_fTrendData_Compliance+=iCompliance;
		m_fTrendData_Vte+=iTVE;
		m_fTrendData_Pinsp+=iPmax;
		m_fTrendData_FiO2+=iFiO2;
		m_fTrendData_Pmean+=iPmitt;

		if(iTVEpat!=0)
		{
			//double RSBI=(double)iBPM/((double)iTVEpat/10);
			m_fTrendData_RSBI+=((double)iBPM/((double)iTVEpat/10))*10;
		}
		m_fTrendData_ShareMVmand+=iShareMVmand;
		m_fTrendData_Resistance+=iResistance;
		m_fTrendData_Leak+=iLeak;
		m_fTrendData_SpO2+=iSpO2;
		m_fTrendData_PI+=iPI;
		m_fTrendData_etCO2+=iEtCO2;
		m_fTrendData_SpO2PR+=iSpO2PR;
		m_fTrendData_Frequency+=iFreq;

		m_iTrendCnt++;
		LeaveCriticalSection(&csTrendFileData);
	}
}


void CDataHandler::setMessureDataBTB(BYTE type, INT val)
{
	EnterCriticalSection(&csMessureDataBTB);
	if(type<MAX_NUMERICVALUES)
		m_pbufMessureBTB[type]=val;
	LeaveCriticalSection(&csMessureDataBTB);
}
INT CDataHandler::getMessureDataBTB(BYTE type)
{
	INT iTemp=0;

	EnterCriticalSection(&csMessureDataBTB);
	if(type<MAX_NUMERICVALUES)
		iTemp=m_pbufMessureBTB[type];
	else
	{
		CStringW sz=_T("");
		sz.Format(_T("#HFO:0288 type%d"),type);
		theApp.getLog()->WriteLine(sz);
	}
	LeaveCriticalSection(&csMessureDataBTB);

	return iTemp;
}




void CDataHandler::checkLimits()
{
	if(		getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT
		&&	getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SILENT
		&&	getModel()->getVIEWHANDLER()->getViewState()!=VS_STARTUP)
	{
		getModel()->getALARMHANDLER()->checkLimits();
	}
}




//***************************************************************************
//      
//**************************************************************************
SHORT CDataHandler::getMessureDataPRESSURE()
{
	EnterCriticalSection(&csMessureData);
	SHORT iTemp = m_dataMessure.m_iPRESSURE;
	LeaveCriticalSection(&csMessureData);
	return iTemp;
}
void CDataHandler::setMessureDataPRESSURE(SHORT value)
{
	EnterCriticalSection(&csMessureData);
	m_dataMessure.m_iPRESSURE=value;
	LeaveCriticalSection(&csMessureData);
}
//***************************************************************************
//      
//**************************************************************************
SHORT CDataHandler::getMessureDataFLOW()
{
	EnterCriticalSection(&csMessureData);
	SHORT iTemp = m_dataMessure.m_iFLOW;	//Achtung, tatsächlicher Meß-Wert ist um Faktor 2 größer
	LeaveCriticalSection(&csMessureData);
	return iTemp;
}
void CDataHandler::setMessureDataFLOW(SHORT value)
{
	EnterCriticalSection(&csMessureData);
	m_dataMessure.m_iFLOW=value;	//Achtung, tatsächlicher Meß-Wert ist um Faktor 2 größer
	LeaveCriticalSection(&csMessureData);
}
//***************************************************************************
//      
//**************************************************************************
SHORT CDataHandler::getMessureDataVOLUME()
{
	EnterCriticalSection(&csMessureData);
	SHORT iTemp = m_dataMessure.m_iVOLUME;
	LeaveCriticalSection(&csMessureData);
	return iTemp;
}
void CDataHandler::setMessureDataVOLUME(SHORT value)
{
	EnterCriticalSection(&csMessureData);
	m_dataMessure.m_iVOLUME=value;
	LeaveCriticalSection(&csMessureData);
}
//***************************************************************************
//      
//**************************************************************************
SHORT CDataHandler::getAVGMessureDataPINSP()
{
	return (SHORT)getMessureDataAVG(ALINK_MSMNT_P_PEAK);
}
//void CDataHandler::setAVGMessureDataPINSP(SHORT value)
//{
//	setMessureDataAVG(NUMT_PPEAK,value);
//}
SHORT CDataHandler::getBTBMessureDataPINSP()
{
	return (SHORT)getMessureDataBTB(ALINK_MSMNT_P_PEAK);
}
//void CDataHandler::setBTBMessureDataPINSP(SHORT value)
//{
//	setMessureDataBTB(NUMT_PPEAK,value);
//}


//***************************************************************************
//      
//**************************************************************************
WORD CDataHandler::getAVGMessureDataTrigger()
{
	return (WORD)getMessureDataAVG(ALINK_MSMNT_TRIGGER);
}
//void CDataHandler::setAVGMessureDataTrigger(WORD value)
//{
//	setMessureDataAVG(NUMT_TRIGGER,value);
//}
WORD CDataHandler::getBTBMessureDataTrigger()
{
	return (WORD)getMessureDataBTB(ALINK_MSMNT_TRIGGER);
}
//void CDataHandler::setBTBMessureDataTrigger(WORD value)
//{
//	setMessureDataBTB(NUMT_TRIGGER,value);
//}

//***************************************************************************
//     
//**************************************************************************
WORD CDataHandler::getAVGMessureDataITimePSV()
{
	return (WORD)getMessureDataAVG(ALINK_MSMNT_T_INSP_PSV);
}
//void CDataHandler::setAVGMessureDataITimePSV(WORD value)
//{
//	setMessureDataAVG(NUMT_PSVTIME,value);
//}
WORD CDataHandler::getBTBMessureDataITimePSV()
{
	return (WORD)getMessureDataBTB(ALINK_MSMNT_T_INSP_PSV);
}
//void CDataHandler::setBTBMessureDataITimePSV(WORD value)
//{
//	setMessureDataBTB(NUMT_PSVTIME,value);
//}

//***************************************************************************
//     
//**************************************************************************
SHORT CDataHandler::getAVGMessureDataPmitt()
{
	return (SHORT)getMessureDataAVG(ALINK_MSMNT_P_MEAN);
}
//void CDataHandler::setAVGMessureDataPmitt(SHORT value)
//{
//	setMessureDataAVG(NUMT_PMEAN,value);
//}
SHORT CDataHandler::getBTBMessureDataPmitt()
{
	return (SHORT)getMessureDataBTB(ALINK_MSMNT_P_MEAN);
}
//void CDataHandler::setBTBMessureDataPmitt(SHORT value)
//{
//	setMessureDataBTB(NUMT_PMEAN,value);
//}
//***************************************************************************
//     
//**************************************************************************
SHORT CDataHandler::getAVGMessureDataPEEP()
{
	return (SHORT)getMessureDataAVG(ALINK_MSMNT_PEEP);
}
//void CDataHandler::setAVGMessureDataPEEP(SHORT value)
//{
//	setMessureDataAVG(NUMT_PEEP,value);
//}
SHORT CDataHandler::getBTBMessureDataPEEP()
{
	return (SHORT)getMessureDataBTB(ALINK_MSMNT_PEEP);
}

//void CDataHandler::setBTBMessureDataPEEP(SHORT value)
//{
//	setMessureDataBTB(NUMT_PEEP,value);
//}
//***************************************************************************
//     
//**************************************************************************
WORD CDataHandler::getAVGMessureDataCompliance()
{
	return (WORD)getMessureDataAVG(ALINK_MSMNT_DYNCOMPL);
}
//void CDataHandler::setAVGMessureDataCompliance(WORD value)
//{
//	setMessureDataAVG(ALINK_MSMNT_DYNCOMPL,value);
//	int iTau=value*getMessureDataAVG(ALINK_MSMNT_RESISTANCE);
//	setMessureDataAVG(NUM_TAU,iTau);
//}

WORD CDataHandler::getBTBMessureDataCompliance()
{
	return (WORD)getMessureDataBTB(ALINK_MSMNT_DYNCOMPL);
}
//void CDataHandler::setBTBMessureDataCompliance(WORD value)
//{
//	setMessureDataBTB(ALINK_MSMNT_DYNCOMPL,value);
//	int iTau=value*getMessureDataBTB(ALINK_MSMNT_RESISTANCE);
//	setMessureDataBTB(NUM_TAU,iTau);
//}
//***************************************************************************
//     
//**************************************************************************
WORD CDataHandler::getAVGMessureDataC20C()
{
	return (WORD)getMessureDataAVG(ALINK_MSMNT_C20C);
}
//void CDataHandler::setAVGMessureDataC20C(WORD value)
//{
//	setMessureDataAVG(NUMT_C20C,value);
//}

WORD CDataHandler::getBTBMessureDataC20C()
{
	return (WORD)getMessureDataBTB(ALINK_MSMNT_C20C);
}
//void CDataHandler::setBTBMessureDataC20C(WORD value)
//{
//	setMessureDataBTB(NUMT_C20C,value);
//}
//***************************************************************************
//     
//**************************************************************************
WORD CDataHandler::getAVGMessureDataResistance()
{
	return (WORD)getMessureDataAVG(ALINK_MSMNT_RESISTANCE);
}
//void CDataHandler::setAVGMessureDataResistance(WORD value)
//{
//	setMessureDataAVG(NUMT_RESISTANCE,value);
//
//	int iTau=value*getMessureDataAVG(NUMT_COMPLIANCE);
//	setMessureDataAVG(NUMT_TAU,iTau);
//}
WORD CDataHandler::getBTBMessureDataResistance()
{
	return (WORD)getMessureDataBTB(ALINK_MSMNT_RESISTANCE);
}
//void CDataHandler::setBTBMessureDataResistance(WORD value)
//{
//	setMessureDataBTB(ALINK_MSMNT_RESISTANCE,value);
//
//	int iTau=value*getMessureDataBTB(NUM_COMPLIANCE);
//	setMessureDataBTB(NUM_TAU,iTau);
//}
//***************************************************************************
//     
//**************************************************************************
WORD CDataHandler::getAVGMessureDataMV()
{
	return (WORD)getMessureDataAVG(ALINK_MSMNT_MIN_VOL);
}
//void CDataHandler::setAVGMessureDataMV(WORD value)
//{
//	setMessureDataAVG(NUMT_MVEXP,value);
//}
WORD CDataHandler::getBTBMessureDataMV()
{
	return (WORD)getMessureDataBTB(ALINK_MSMNT_MIN_VOL);
}
//void CDataHandler::setBTBMessureDataMV(WORD value)
//{
//	setMessureDataBTB(NUMT_MVEXP,value);
//}
//***************************************************************************
//     
//**************************************************************************
BYTE CDataHandler::getAVGMessureDataPercent()
{
	return (BYTE)getMessureDataAVG(ALINK_MSMNT_ANTEIL_RESP);
}
//void CDataHandler::setAVGMessureDataPercent(BYTE value)
//{
//	setMessureDataAVG(NUMT_SHAREMV,value);
//}
BYTE CDataHandler::getBTBMessureDataPercent()
{
	return (BYTE)getMessureDataBTB(ALINK_MSMNT_ANTEIL_RESP);
}
//void CDataHandler::setBTBMessureDataPercent(BYTE value)
//{
//	setMessureDataBTB(NUMT_SHAREMV,value);
//}

//***************************************************************************
//     
//**************************************************************************
WORD CDataHandler::getAVGMessureDataHFAmpl()
{
	return (WORD)getMessureDataAVG(ALINK_MSMNT_HFO_AMP);
}
//void CDataHandler::setAVGMessureDataHFAmpl(WORD value)
//{
//	setMessureDataAVG(NUMT_HFOAMP,value);
//}
WORD CDataHandler::getBTBMessureDataHFAmpl()
{
	return (WORD)getMessureDataBTB(ALINK_MSMNT_HFO_AMP);
}
//void CDataHandler::setBTBMessureDataHFAmpl(WORD value)
//{
//	setMessureDataBTB(NUMT_HFOAMP,value);
//}
//***************************************************************************
//     
//**************************************************************************
BYTE CDataHandler::getAVGMessureDataHFFreq()
{
	return (BYTE)getMessureDataAVG(ALINK_MSMNT_HFO_FREQ);
}
//void CDataHandler::setAVGMessureDataHFFreq(BYTE value)
//{
//	setMessureDataAVG(NUMT_HFOFREQ,value);
//}
BYTE CDataHandler::getBTBMessureDataHFFreq()
{
	return (BYTE)getMessureDataBTB(ALINK_MSMNT_HFO_FREQ);
}
//void CDataHandler::setBTBMessureDataHFFreq(BYTE value)
//{
//	setMessureDataBTB(NUMT_HFOFREQ,value);
//}

//***************************************************************************
//     
//**************************************************************************
WORD CDataHandler::getAVGMessureDataTVEHFO()
{
	WORD iTemp = 0;
	if(getModel()->getCONFIG()->useTveBTB())
	{
		iTemp = (WORD)getMessureDataBTB(ALINK_MSMNT_TVE_HFO);
	}
	else
	{
		iTemp = (WORD)getMessureDataAVG(ALINK_MSMNT_TVE_HFO);
	}
	
	return iTemp;
}
//void CDataHandler::setAVGMessureDataTVEHFO(WORD value)
//{
//	setMessureDataAVG(NUMT_HFOVTE,value);
//}
WORD CDataHandler::getBTBMessureDataTVEHFO()
{
	return (WORD)getMessureDataBTB(ALINK_MSMNT_TVE_HFO);
}
//void CDataHandler::setBTBMessureDataTVEHFO(WORD value)
//{
//	setMessureDataBTB(NUMT_HFOVTE,value);
//}

//***************************************************************************
//     
//**************************************************************************
WORD CDataHandler::getAVGMessureDataDCO2()
{
	return (WORD)getMessureDataAVG(ALINK_MSMNT_DCO2);
}
//void CDataHandler::setAVGMessureDataDCO2(WORD value)
//{
//	setMessureDataAVG(NUMT_DCO2,value);
//}
WORD CDataHandler::getBTBMessureDataDCO2()
{
	return (WORD)getMessureDataBTB(ALINK_MSMNT_DCO2);
}
//void CDataHandler::setBTBMessureDataDCO2(WORD value)
//{
//	setMessureDataBTB(NUMT_DCO2,value);
//}
//***************************************************************************
//     
//**************************************************************************
WORD CDataHandler::getMessureDataTVI()
{
	WORD iTemp = 0;
	if(getModel()->getCONFIG()->useTveBTB())
	{
		iTemp = (WORD)getMessureDataBTB(ALINK_MSMNT_TVI);
	}
	else
	{
		iTemp = (WORD)getMessureDataAVG(ALINK_MSMNT_TVI);
	}

	return iTemp;
}

//void CDataHandler::setAVGMessureDataTVI(WORD value)
//{
//	setMessureDataAVG(NUMT_VTI,value);
//}

//void CDataHandler::setBTBMessureDataTVI(WORD value)
//{
//	setMessureDataBTB(NUMT_VTI,value);
//}

//***************************************************************************
//     
//**************************************************************************
//void CDataHandler::setAVGMessureDataSPO2(SHORT value)
//{
//	setMessureDataAVG(NUMT_SPO2,value);
//
//	/*if(getModel()->getPRICOThread())
//	{
//		if(getModel()->getPRICOThread()->isPRICOrunning())
//		{
//			getModel()->getPRICOThread()->setMeasuredSpO2Value(value);
//		}
//	}*/
//}
SHORT CDataHandler::getAVGMessureDataSPO2()
{
	return (SHORT)getMessureDataAVG(ALINK_MSMNT_SPO2);
}

//void CDataHandler::setAVGMessureDataSpO2PulseRate(SHORT value)
//{
//	setMessureDataAVG(NUMT_SPO2_PR,value);
//}
SHORT CDataHandler::getAVGMessureDataSpO2PulseRate()
{
	return (SHORT)getMessureDataAVG(ALINK_MSMNT_SPO2_PR);
}

//void CDataHandler::setAVGMessureDataSpO2PerfusionIndex(SHORT value)
//{
//	setMessureDataAVG(NUMT_SPO2_PI,value);
//}
SHORT CDataHandler::getAVGMessureDataSpO2PerfusionIndex()
{
	return (SHORT)getMessureDataAVG(ALINK_MSMNT_SPO2_PI);
}

void CDataHandler::resetSPO2MessureData()
{
	setMessureDataAVG(ALINK_MSMNT_SPO2,0);
	setMessureDataAVG(ALINK_MSMNT_SPO2_PR,0);
	setMessureDataAVG(ALINK_MSMNT_SPO2_PI,0);

	EnterCriticalSection(&csSPO2DataBuffer);
	m_rbufSPO2.reset();
	m_iSPO2waveData=0;
	LeaveCriticalSection(&csSPO2DataBuffer);
	getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_SPO2_SIQ,0);
}

//***************************************************************************
//     
//**************************************************************************
//void CDataHandler::resetEMGMessureData()
//{
//	//setMessureDataAVG(NUMT_SPO2,0);
//	//setMessureDataAVG(NUMT_SPO2_PR,0);
//	//setMessureDataAVG(NUMT_SPO2_PI,0);
//
//	EnterCriticalSection(&csEMGDataBuffer);
//	m_rbufEMG.reset();
//	m_iEMGwaveData=0;
//	LeaveCriticalSection(&csEMGDataBuffer);
//}

//***************************************************************************
//     
//**************************************************************************
//void CDataHandler::setAVGMessureDataETCO2(SHORT value)
//{
//	setMessureDataAVG(NUMT_ETCO2,value);
//}
SHORT CDataHandler::getAVGMessureDataETCO2()
{
	return (SHORT)getMessureDataAVG(ALINK_MSMNT_ETCO2);
}

//***************************************************************************
//     
//**************************************************************************
//void CDataHandler::setAVGMessureDataFICO2(int value)
//{
//	setMessureDataAVG(NUMT_FICO2,value);
//}
int CDataHandler::getAVGMessureDataFICO2()
{
	return (SHORT)getMessureDataAVG(ALINK_MSMNT_FICO2);
}

//***************************************************************************
//     
//**************************************************************************
void CDataHandler::resetCO2MessureData()
{
	setMessureDataAVG(ALINK_MSMNT_ETCO2,0);

	EnterCriticalSection(&csCO2DataBuffer);
	m_rbufCO2.reset();
	LeaveCriticalSection(&csCO2DataBuffer);
}
//***************************************************************************
//     
//**************************************************************************
void CDataHandler::sendMessureDataVolumeCorrection()
{
	EnterCriticalSection(&csMessureData);
	BYTE iTemp=m_dataMessure.m_iVolumeCorr;
	LeaveCriticalSection(&csMessureData);
	getModel()->getSERIAL()->Send_VOLUME_CORR(iTemp);
	
}

void CDataHandler::setMessureDataVolumeCorrection(BYTE value)
{
	EnterCriticalSection(&csMessureData);
	m_dataMessure.m_iVolumeCorr=value;
	LeaveCriticalSection(&csMessureData);
}
//***************************************************************************
//     
//**************************************************************************
WORD CDataHandler::getMessureDataTVE()
{
	WORD iTemp = 0;
	if(getModel()->getCONFIG()->useTveBTB())
	{
		iTemp = (WORD)getMessureDataBTB(ALINK_MSMNT_TVE);
	}
	else
	{
		iTemp = (WORD)getMessureDataAVG(ALINK_MSMNT_TVE);
	}

	return iTemp;
}

//void CDataHandler::setAVGMessureDataTVE(WORD value)
//{
//	setMessureDataAVG(NUMT_VTE,value);
//}

//void CDataHandler::setBTBMessureDataTVE(WORD value)
//{
//	setMessureDataBTB(NUMT_VTE,value);
//}
//***************************************************************************
//     
//**************************************************************************
WORD CDataHandler::getMessureDataTVEresp()
{
	WORD iTemp = 0;
	if(getModel()->getCONFIG()->useTveBTB())
	{
		iTemp = (WORD)getMessureDataBTB(ALINK_MSMNT_TVE_RESP);
	}
	else
	{
		iTemp = (WORD)getMessureDataAVG(ALINK_MSMNT_TVE_RESP);
	}

	return iTemp;
}

//void CDataHandler::setAVGMessureDataTVEresp(WORD value)
//{
//	setMessureDataAVG(NUMT_VTEMAND,value);
//}
//void CDataHandler::setBTBMessureDataTVEresp(WORD value)
//{
//	setMessureDataBTB(NUMT_VTEMAND,value);
//}
//***************************************************************************
//     
//**************************************************************************
WORD CDataHandler::getMessureDataTVEpat()
{
	WORD iTemp = 0;
	if(getModel()->getCONFIG()->useTveBTB())
	{
		iTemp = (WORD)getMessureDataBTB(ALINK_MSMNT_TVE_PATIENT);
	}
	else
	{
		iTemp = (WORD)getMessureDataAVG(ALINK_MSMNT_TVE_PATIENT);
	}

	return iTemp;
}

//void CDataHandler::setAVGMessureDataTVEpat(WORD value)
//{
//	setMessureDataAVG(NUMT_VTESPONT,value);
//}
//
//void CDataHandler::setBTBMessureDataTVEpat(WORD value)
//{
//	setMessureDataBTB(NUMT_VTESPONT,value);
//}

//***************************************************************************
//     
//**************************************************************************
BYTE CDataHandler::getAVGMessureDataBPMco2()
{
	return (BYTE)getMessureDataAVG(ALINK_MSMNT_FREQETCO2);
}
//void CDataHandler::setAVGMessureDataBPMco2(BYTE value)
//{
//	setMessureDataAVG(NUMT_FREQETCO2,value);
//}
BYTE CDataHandler::getBTBMessureDataBPMco2()
{
	return (BYTE)getMessureDataBTB(ALINK_MSMNT_FREQETCO2);
}
//void CDataHandler::setBTBMessureDataBPMco2(BYTE value)
//{
//	setMessureDataBTB(NUMT_FREQETCO2,value);
//}

//***************************************************************************
//     
//**************************************************************************
BYTE CDataHandler::getAVGMessureDataBPM()
{
	return (BYTE)getMessureDataAVG(ALINK_MSMNT_FREQ);
}
//void CDataHandler::setAVGMessureDataBPM(BYTE value)
//{
//	setMessureDataAVG(NUMT_FREQ,value);
//}
BYTE CDataHandler::getBTBMessureDataBPM()
{
	return (BYTE)getMessureDataBTB(ALINK_MSMNT_FREQ);
}
//void CDataHandler::setBTBMessureDataBPM(BYTE value)
//{
//	setMessureDataBTB(NUMT_FREQ,value);
//}



//***************************************************************************
//     
//**************************************************************************
SHORT CDataHandler::getAppliedFiO2para()
{
	SHORT iTemp = 0;
	
	bool bPRICOrunning=false;
	if(getModel()->getPRICOThread())
	{
		if(getModel()->getPRICOThread()->isPRICOalgorithmRunning())
		{
			bPRICOrunning=true;
		}
	}
	if(bPRICOrunning)
	{
		//iTemp=getModel()->getPRICOThread()->getCalculatedOxyValue();
		iTemp=getModel()->getPRICOThread()->getCalculatedOxyValue()*10; //pro, change 3
	}
	else
	{
		if(getModel()->isO2FlushActive()==false)
		{
			iTemp = PARADATA()->GetO2Para()*10;
		}
		else
		{
			iTemp = PARADATA()->GetO2FlushPara()*10;
		}
	}
	return iTemp;
}

SHORT CDataHandler::getMessureDataO2()
{
	EnterCriticalSection(&csMessureDataBTB);
	SHORT iTemp = (SHORT)m_pbufMessureBTB[ALINK_MSMNT_OXY];
	LeaveCriticalSection(&csMessureDataBTB);

	return iTemp;
}
void CDataHandler::setMessureDataO2(SHORT value)
{
	EnterCriticalSection(&csMessureDataBTB);
	m_pbufMessureBTB[ALINK_MSMNT_OXY]=value;
	LeaveCriticalSection(&csMessureDataBTB);

	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_NEW_FIO2DATA);

}
//***************************************************************************
//     
//**************************************************************************
BYTE CDataHandler::getAVGMessureDataLeak()
{
	return (BYTE)getMessureDataAVG(ALINK_MSMNT_LEAK);
}
//void CDataHandler::setAVGMessureDataLeak(BYTE value)
//{
//	setMessureDataAVG(NUMT_LEAK,value);
//}
BYTE CDataHandler::getBTBMessureDataLeak()
{
	return (BYTE)getMessureDataBTB(ALINK_MSMNT_LEAK);
}
//void CDataHandler::setBTBMessureDataLeak(BYTE value)
//{
//	setMessureDataBTB(NUMT_LEAK,value);
//}


//***************************************************************************
//     
//**************************************************************************
//void CDataHandler::getMessureRangeData(MESSURE_RANGE_DATA* pMessureRangeData)
//{
//	memcpy(pMessureRangeData,&m_dataMessureRange,sizeof(MESSURE_RANGE_DATA));
//}
//void CDataHandler::setMessureRangeData(MESSURE_RANGE_DATA* pMessureRangeData)
//{
//	memcpy(&m_dataMessureRange,pMessureRangeData,sizeof(MESSURE_RANGE_DATA));
//}
//***************************************************************************
//     
//**************************************************************************
//SHORT CDataHandler::getMessureRangeDataPINSP_UpperLimit()
//{
//	return m_dataMessureRange.m_iPINSP_UpperLimit;
//}
//void CDataHandler::setMessureRangeDataPINSP_UpperLimit(SHORT value)
//{
//	m_dataMessureRange.m_iPINSP_UpperLimit=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::getMessureRangeDataPINSP_LowerLimit()
//{
//	return m_dataMessureRange.m_iPINSP_LowerLimit;
//}
//void CDataHandler::setMessureRangeDataPINSP_LowerLimit(SHORT value)
//{
//	m_dataMessureRange.m_iPINSP_LowerLimit=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::getMessureRangeDataPmitt_UpperLimit()
//{
//	return m_dataMessureRange.m_iPmitt_UpperLimit;
//}
//void CDataHandler::setMessureRangeDataPmitt_UpperLimit(SHORT value)
//{
//	m_dataMessureRange.m_iPmitt_UpperLimit=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::getMessureRangeDataPmitt_LowerLimit()
//{
//	return m_dataMessureRange.m_iPmitt_LowerLimit;
//}
//void CDataHandler::setMessureRangeDataPmitt_LowerLimit(SHORT value)
//{
//	m_dataMessureRange.m_iPmitt_LowerLimit=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::getMessureRangeDataPEEP_UpperLimit()
//{
//	return m_dataMessureRange.m_iPEEP_UpperLimit;
//}
//void CDataHandler::setMessureRangeDataPEEP_UpperLimit(SHORT value)
//{
//	m_dataMessureRange.m_iPEEP_UpperLimit=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::getMessureRangeDataPEEP_LowerLimit()
//{
//	return m_dataMessureRange.m_iPEEP_LowerLimit;
//}
//void CDataHandler::setMessureRangeDataPEEP_LowerLimit(SHORT value)
//{
//	m_dataMessureRange.m_iPEEP_LowerLimit=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::getMessureRangeDataMV_UpperLimit_NEONATAL()
//{
//	return m_dataMessureRange.m_iMV_UpperLimit_NEONATAL;
//}
//void CDataHandler::setMessureRangeDataMV_UpperLimit_NEONATAL(SHORT value)
//{
//	m_dataMessureRange.m_iMV_UpperLimit_NEONATAL=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::getMessureRangeDataMV_LowerLimit_NEONATAL()
//{
//	return m_dataMessureRange.m_iMV_LowerLimit_NEONATAL;
//}
//void CDataHandler::setMessureRangeDataMV_LowerLimit_NEONATAL(SHORT value)
//{
//	m_dataMessureRange.m_iMV_LowerLimit_NEONATAL=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::getMessureRangeDataMV_UpperLimit_PEDIATRIC()
//{
//	return m_dataMessureRange.m_iMV_UpperLimit_PEDIATRIC;
//}
//void CDataHandler::setMessureRangeDataMV_UpperLimit_PEDIATRIC(SHORT value)
//{
//	m_dataMessureRange.m_iMV_UpperLimit_PEDIATRIC=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::getMessureRangeDataMV_LowerLimit_PEDIATRIC()
//{
//	return m_dataMessureRange.m_iMV_LowerLimit_PEDIATRIC;
//}
//void CDataHandler::setMessureRangeDataMV_LowerLimit_PEDIATRIC(SHORT value)
//{
//	m_dataMessureRange.m_iMV_LowerLimit_PEDIATRIC=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::getMessureRangeDataTVE_UpperLimit_NEONATAL()
//{
//	return m_dataMessureRange.m_iTVE_UpperLimit_NEONATAL;
//}
//void CDataHandler::setMessureRangeDataTVE_UpperLimit_NEONATAL(SHORT value)
//{
//	m_dataMessureRange.m_iTVE_UpperLimit_NEONATAL=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::getMessureRangeDataTVE_LowerLimit_NEONATAL()
//{
//	return m_dataMessureRange.m_iTVE_LowerLimit_NEONATAL;
//}
//void CDataHandler::setMessureRangeDataTVE_LowerLimit_NEONATAL(SHORT value)
//{
//	m_dataMessureRange.m_iTVE_LowerLimit_NEONATAL=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::getMessureRangeDataTVE_UpperLimit_PEDIATRIC()
//{
//	return m_dataMessureRange.m_iTVE_UpperLimit_PEDIATRIC;
//}
//void CDataHandler::setMessureRangeDataMTVE_UpperLimit_PEDIATRIC(SHORT value)
//{
//	m_dataMessureRange.m_iTVE_UpperLimit_PEDIATRIC=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::getMessureRangeDataTVE_LowerLimit_PEDIATRIC()
//{
//	return m_dataMessureRange.m_iTVE_LowerLimit_PEDIATRIC;
//}
//void CDataHandler::setMessureRangeDataTVE_LowerLimit_PEDIATRIC(SHORT value)
//{
//	m_dataMessureRange.m_iTVE_LowerLimit_PEDIATRIC=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::getMessureRangeDataLeak_UpperLimit()
//{
//	return m_dataMessureRange.m_iLeak_UpperLimit;
//}
//void CDataHandler::SetMessureRangeDataLeak_UpperLimit(SHORT value)
//{
//	m_dataMessureRange.m_iLeak_UpperLimit=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::GetMessureRangeDataLeak_LowerLimit()
//{
//	return m_dataMessureRange.m_iLeak_LowerLimit;
//}
//void CDataHandler::SetMessureRangeDataLeak_LowerLimit(SHORT value)
//{
//	m_dataMessureRange.m_iLeak_LowerLimit=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::GetMessureRangeDataDynCompl_UpperLimit()
//{
//	return m_dataMessureRange.m_iDynCompl_UpperLimit;
//}
//void CDataHandler::SetMessureRangeDataDynCompl_UpperLimit(SHORT value)
//{
//	m_dataMessureRange.m_iDynCompl_UpperLimit=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::GetMessureRangeDataDynCompl_LowerLimit()
//{
//	return m_dataMessureRange.m_iDynCompl_LowerLimit;
//}
//void CDataHandler::SetMessureRangeDataDynCompl_LowerLimit(SHORT value)
//{
//	m_dataMessureRange.m_iDynCompl_LowerLimit=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::GetMessureRangeDataResistance_UpperLimit()
//{
//	return m_dataMessureRange.m_iResistance_UpperLimit;
//}
//void CDataHandler::SetMessureRangeDataResistance_UpperLimit(SHORT value)
//{
//	m_dataMessureRange.m_iResistance_UpperLimit=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::GetMessureRangeDataResistance_LowerLimit()
//{
//	return m_dataMessureRange.m_iResistance_LowerLimit;
//}
//void CDataHandler::SetMessureRangeDataResistance_LowerLimit(SHORT value)
//{
//	m_dataMessureRange.m_iResistance_LowerLimit=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::GetMessureRangeDataO2_UpperLimit()
//{
//	return m_dataMessureRange.m_iO2_UpperLimit;
//}
//void CDataHandler::SetMessureRangeDataO2_UpperLimit(SHORT value)
//{
//	m_dataMessureRange.m_iO2_UpperLimit=value;
//}
////***************************************************************************
////     
////**************************************************************************
//SHORT CDataHandler::GetMessureRangeDataO2_LowerLimit()
//{
//	return m_dataMessureRange.m_iO2_LowerLimit;
//}
//void CDataHandler::SetMessureRangeDataO2_LowerLimit(SHORT value)
//{
//	m_dataMessureRange.m_iO2_LowerLimit=value;
//}

//***************************************************************************
//     
//**************************************************************************
//SHORT CDataHandler::GetMessureDataHFAmpl_UpperLimit()
//{
//	return m_dataMessureRange.m_iHFAmpl_UpperLimit;
//}
//void CDataHandler::SetMessureDataHFAmpl_UpperLimit(SHORT value)
//{
//	m_dataMessureRange.m_iHFAmpl_UpperLimit=value;
//}
//***************************************************************************
//     
//**************************************************************************
//SHORT CDataHandler::GetMessureDataHFAmpl_LowerLimit()
//{
//	return m_dataMessureRange.m_iHFAmpl_LowerLimit;
//}
//void CDataHandler::SetMessureDataHFAmpl_LowerLimit(SHORT value)
//{
//	m_dataMessureRange.m_iHFAmpl_LowerLimit=value;
//}

//***************************************************************************
//     
//**************************************************************************
//SHORT CDataHandler::GetMessureDataHFFreq_UpperLimit()
//{
//	return m_dataMessureRange.m_iHFFreq_UpperLimit;
//}
//void CDataHandler::SetMessureDataHFFreq_UpperLimit(SHORT value)
//{
//	m_dataMessureRange.m_iHFFreq_UpperLimit=value;
//}
//***************************************************************************
//     
//**************************************************************************
//SHORT CDataHandler::GetMessureDataHFFreq_LowerLimit()
//{
//	return m_dataMessureRange.m_iHFFreq_LowerLimit;
//}
//void CDataHandler::SetMessureDataHFFreq_LowerLimit(SHORT value)
//{
//	m_dataMessureRange.m_iHFFreq_LowerLimit=value;
//}

//***************************************************************************
//     
//**************************************************************************
//SHORT CDataHandler::GetMessureDataDCO2_UpperLimit()
//{
//	return m_dataMessureRange.m_iDCO2_UpperLimit;
//}
//void CDataHandler::SetMessureDataDCO2_UpperLimit(SHORT value)
//{
//	m_dataMessureRange.m_iDCO2_UpperLimit=value;
//}
//***************************************************************************
//     
//**************************************************************************
//SHORT CDataHandler::GetMessureDataDCO2_LowerLimit()
//{
//	return m_dataMessureRange.m_iDCO2_LowerLimit;
//}
//void CDataHandler::SetMessureDataDCO2_LowerLimit(SHORT value)
//{
//	m_dataMessureRange.m_iDCO2_LowerLimit=value;
//}


//***************************************************************************
//     
//**************************************************************************
bool CDataHandler::CheckIErelation(int iITime,int iBPM,int iETime)
{
	bool bSetFactory=false;

	if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
	{
		if(false==ConfirmItoBPMrelation(iITime,iBPM,iETime))
		{
			CStringW sz=_T("");
			sz.Format(_T("#HFO:0074 iITime%d iBPM%d iETime%d"),iITime,iBPM,iETime);
			theApp.getLog()->WriteLine(sz);

			bSetFactory=true;
		}
	}
	else
	{
		if(false==ConfirmItoErelation(iITime,iETime,iBPM))
		{
			CStringW sz=_T("");
			sz.Format(_T("#HFO:0075 iITime%d iBPM%d iETime%d."),iITime,iBPM,iETime);
			theApp.getLog()->WriteLine(sz);

			bSetFactory=true;
		}
	}

	if(bSetFactory)
	{
		switch(getModel()->getCONFIG()->GetCurMode())
		{
		case VM_IPPV:
		case VM_PRE_IPPV:
			{
				iBPM=FACTORY_IPPV_BPM;
				iITime=FACTORY_IPPV_ITIME;
				iETime=FACTORY_IPPV_ETIME;
			}
			break;
		case VM_PSV:
		case VM_PRE_PSV:
			{
				iBPM=FACTORY_PSV_BPM;
				iITime=FACTORY_PSV_ITIME;
				iETime=FACTORY_PSV_ETIME;
			}
			break;
		case VM_SIMV:
		case VM_PRE_SIMV:
			{
				iBPM=FACTORY_SIMV_BPM;
				iITime=FACTORY_SIMV_ITIME;
				iETime=FACTORY_SIMV_ETIME;
			}
			break;
		case VM_SIMVPSV:
		case VM_PRE_SIMVPSV:
			{
				iBPM=FACTORY_SIMVPSV_BPM;
				iITime=FACTORY_SIMVPSV_ITIME;
				iETime=FACTORY_SIMVPSV_ETIME;
			}
			break;
		case VM_SIPPV:
		case VM_PRE_SIPPV:
			{
				iBPM=FACTORY_SIPPV_BPM;
				iITime=FACTORY_SIPPV_ITIME;
				iETime=FACTORY_SIPPV_ETIME;
			}
			break;
		case VM_DUOPAP:
		case VM_PRE_DUOPAP:
			{
				iBPM=FACTORY_NMODE_BPM;
				iITime=FACTORY_NMODE_ITIME;
				iETime=FACTORY_NMODE_ETIME;
			}
			break;
		default:
			{
			}
			break;
		}

		SetCurrentITimePara(iITime);
		SetCurrentETimePara(iETime);
		SetCurrentBPMPara(iBPM);
		if(AfxGetApp() != NULL)
		{
			AfxGetApp()->GetMainWnd()->PostMessage(WM_ITIME_CHANGED);
			AfxGetApp()->GetMainWnd()->PostMessage(WM_ETIME_CHANGED);
			AfxGetApp()->GetMainWnd()->PostMessage(WM_BPM_CHANGED);
		}
		
	}
	
	return bSetFactory;
}

//***************************************************************************
//     
//**************************************************************************
bool CDataHandler::ConfirmItoBPMrelation(int iITime,int iBPM,int iETime)
{
	bool bRes=false;
	
	int iResult = CTlsFloat::Round((60000/(double)(iBPM))-iITime, 0);

	if(iResult==iETime)
		bRes=true;

	return bRes;
}

//***************************************************************************
//     
//**************************************************************************
bool CDataHandler::ConfirmItoErelation(int iITime,int iETime,int iBPM)
{
	bool bRes=false;
	int iResult= CTlsFloat::Round(60000/(double)((iETime+iITime)), 0);

	if(iResult==iBPM)
		bRes=true;


	return bRes;
}

//***************************************************************************
//     
//**************************************************************************
bool CDataHandler::InitialCalculateETime_ITimeChanged_IPPV(int fITime, bool bTemp)
{
	//double fResult=0;
	int iResult = CTlsFloat::Round((60000/(double)(PARADATA()->GetBPMPara_IPPV()))-fITime, 0);

	if(iResult>PARADATA()->GetETIMEMaxPara_IPPV())
	{
		return false;
	}

	if(iResult<PARADATA()->GetETIMEMinPara_IPPV())
	{
		return false;
	}

	if(!bTemp)
	{
		//new paradata
		//PARADATA()->SetETIMEPara(iResult, true);
		SetETIMEParadata_IPPV(iResult, true);
	}

	SetCurrentTempETime(iResult);

	return true;
}

//***************************************************************************
//     
//**************************************************************************
bool CDataHandler::InitialCalculateETime_ITimeChanged_TRIGGER(int fITime, bool bTemp)
{
	//double fResult=0;
	int iResult = CTlsFloat::Round((60000/(double)(PARADATA()->GetBPMPara_TRIGGER()))-fITime, 0);

	if(iResult>PARADATA()->GetETIMEMaxPara_TRIGGER())
	{
		return false;
	}

	if(iResult<PARADATA()->GetETIMEMinPara_TRIGGER())
	{
		return false;
	}

	if(!bTemp)
	{
		//new paradata
		//PARADATA()->SetETIMEPara(iResult, true);
		SetETIMEParadata_TRIGGER(iResult, true);
	}

	SetCurrentTempETime(iResult);

	return true;
}

//***************************************************************************
//     
//**************************************************************************
bool CDataHandler::InitialCalculateETime_ITimeChanged_NMODE(int fITime, bool bTemp)
{
	//double fResult=0;
	int iResult = CTlsFloat::Round((60000/(double)(PARADATA()->GetBPMNMODEPara()))-fITime, 0);

	if(iResult>PARADATA()->GetETIMENMODEMaxPara())
	{
		return false;
	}

	if(iResult<PARADATA()->GetETIMENMODEMinPara())
	{
		return false;
	}

	if(!bTemp)
	{
		//new paradata
		//PARADATA()->SetETIMENMODEPara(iResult, true);
		SetETIMENMODEParadata(iResult, true);
	}

	SetCurrentTempETime(iResult);

	return true;
}

bool CDataHandler::InitialCalculateBPM_ITimeChanged_TRIGGER(int fITime, bool bTemp)
{
	//double fResult=0;
	int iResult = CTlsFloat::Round(60000/(double)((PARADATA()->GetETIMEPara_TRIGGER()+fITime)), 0);

	if(iResult>PARADATA()->GetBPMMaxPara_TRIGGER())
	{
		return false;
	}

	if(iResult<PARADATA()->GetBPMMinPara_TRIGGER())
	{
		return false;
	}

	//new paradata
	//PARADATA()->SetBPMPara(iResult);
	SetBPMParadata_TRIGGER(iResult,true);

	return true;
}
bool CDataHandler::InitialCalculateBPM_ITimeChanged_IPPV(int fITime, bool bTemp)
{
	//double fResult=0;
	int iResult = CTlsFloat::Round(60000/(double)((PARADATA()->GetETIMEPara_IPPV()+fITime)), 0);

	if(iResult>PARADATA()->GetBPMMaxPara_IPPV())
	{
		return false;
	}

	if(iResult<PARADATA()->GetBPMMinPara_IPPV())
	{
		return false;
	}

	//new paradata
	//PARADATA()->SetBPMPara(iResult);
	SetBPMParadata_IPPV(iResult,true);

	return true;
}
bool CDataHandler::InitialCalculateBPM_ITimeChanged_NMODE(int fITime, bool bTemp)
{
	//double fResult=0;
	int iResult = CTlsFloat::Round(60000/(double)((PARADATA()->GetETIMENMODEPara()+fITime)), 0);

	if(iResult>PARADATA()->GetBPMNMODEMaxPara())
	{
		return false;
	}

	if(iResult<PARADATA()->GetBPMNMODEMinPara())
	{
		return false;
	}

	//new paradata
	//PARADATA()->SetBPMNMODEPara(iResult);
	SetBPMNMODEParadata(iResult,true);

	return true;
}


//***************************************************************************
//     
//**************************************************************************
bool CDataHandler::CalculateETime_ITimeChanged(int fITime, bool bTemp)
{
	int iResult=0;
	
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			iResult = CTlsFloat::Round((60000/(double)(PARADATA()->GetBPMPara_IPPV()))-fITime, 0);

			if(iResult>PARADATA()->GetETIMEMaxPara_IPPV())
			{
				return false;
			}

			if(iResult<PARADATA()->GetETIMEMinPara_IPPV())
			{
				return false;
			}

			if(!bTemp)
			{
				SetETIMEParadata_IPPV(iResult, true);
			}

			SetCurrentTempETime(iResult);
		}
		break;
	case VM_SIPPV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			iResult = CTlsFloat::Round((60000/(double)(PARADATA()->GetBPMPara_TRIGGER()))-fITime, 0);

			if(iResult>PARADATA()->GetETIMEMaxPara_TRIGGER())
			{
				return false;
			}

			if(iResult<PARADATA()->GetETIMEMinPara_TRIGGER())
			{
				return false;
			}

			if(!bTemp)
			{
				SetETIMEParadata_TRIGGER(iResult, true);
			}
			
			SetCurrentTempETime(iResult);
		}
		break;
	case VM_DUOPAP:
		{
			iResult = CTlsFloat::Round((60000/(double)(PARADATA()->GetBPMNMODEPara()))-fITime, 0);

			if(iResult>PARADATA()->GetETIMENMODEMaxPara())
			{
				return false;
			}

			if(iResult<PARADATA()->GetETIMENMODEMinPara())
			{
				return false;
			}

			if(!bTemp)
			{
				//new paradata
				//PARADATA()->SetETIMENMODEPara(iResult, true);
				SetETIMENMODEParadata(iResult, true);
			}

			SetCurrentTempETime(iResult);
		}
		break;
	case VM_PRE_IPPV:
		{
			iResult = CTlsFloat::Round((60000/(double)(PRESET()->GetBPMPara_IPPV()))-fITime, 0);

			if(iResult>PARADATA()->GetETIMEMaxPara_IPPV())
			{
				return false;
			}

			if(iResult<PARADATA()->GetETIMEMinPara_IPPV())
			{
				return false;
			}

			if(!bTemp)
				PRESET()->SetETIMEPara_IPPV(iResult, false, false);

			SetCurrentTempETime(iResult);
		}
		break;
	case VM_PRE_SIPPV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_PSV:
		{
			iResult = CTlsFloat::Round((60000/(double)(PRESET()->GetBPMPara_TRIGGER()))-fITime, 0);

			if(iResult>PARADATA()->GetETIMEMaxPara_TRIGGER())
			{
				return false;
			}

			if(iResult<PARADATA()->GetETIMEMinPara_TRIGGER())
			{
				return false;
			}

			if(!bTemp)
				PRESET()->SetETIMEPara_TRIGGER(iResult, false, false);
			
			SetCurrentTempETime(iResult);
		}
		break;
	case VM_PRE_DUOPAP:
		{
			iResult = CTlsFloat::Round((60000/(double)(PRESET()->GetBPMNMODEPara()))-fITime, 0);

			if(iResult>PARADATA()->GetETIMENMODEMaxPara())
			{
				return false;
			}

			if(iResult<PARADATA()->GetETIMENMODEMinPara())
			{
				return false;
			}

			if(!bTemp)
				PRESET()->SetETIMENMODEPara(iResult, false,false);

			SetCurrentTempETime(iResult);
		}
		break;
	default:
		{
			CStringW sz=_T("");
			sz.Format(_T("#HFO:0076 %d"),(int)getModel()->getCONFIG()->GetCurMode());
			theApp.getLog()->WriteLine(sz);
		}
		break;
	}

	return true;
}

bool CDataHandler::CalculateBPM_ITimeChanged(int fITime, bool bTemp)
{
	//double fResult=0;
	int iResult=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			iResult = CTlsFloat::Round(60000/(double)((PARADATA()->GetETIMEPara_IPPV()+fITime)), 0);

			if(iResult>PARADATA()->GetBPMMaxPara_IPPV())
			{
				return false;
			}

			if(iResult<PARADATA()->GetBPMMinPara_IPPV())
			{
				return false;
			}

			//new paradata
			//PARADATA()->SetBPMPara(iResult);
			SetBPMParadata_IPPV(iResult,true);
		}
		break;
	case VM_SIPPV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			iResult = CTlsFloat::Round(60000/(double)((PARADATA()->GetETIMEPara_TRIGGER()+fITime)), 0);

			if(iResult>PARADATA()->GetBPMMaxPara_TRIGGER())
			{
				return false;
			}

			if(iResult<PARADATA()->GetBPMMinPara_TRIGGER())
			{
				return false;
			}

			//new paradata
			//PARADATA()->SetBPMPara(iResult);
			SetBPMParadata_TRIGGER(iResult,true);
		}
		break;
	case VM_DUOPAP:
		{
			iResult = CTlsFloat::Round(60000/(double)((PARADATA()->GetETIMENMODEPara()+fITime)), 0);

			if(iResult>PARADATA()->GetBPMNMODEMaxPara())
			{
				return false;
			}

			if(iResult<PARADATA()->GetBPMNMODEMinPara())
			{
				return false;
			}

			//new paradata
			//PARADATA()->SetBPMNMODEPara(iResult);
			SetBPMNMODEParadata(iResult,true);
		}
		break;
	case VM_PRE_IPPV:
		{
			iResult = CTlsFloat::Round(60000/(double)((PRESET()->GetETIMEPara_IPPV()+fITime)), 0);

			if(iResult>PARADATA()->GetBPMMaxPara_IPPV())
			{
				return false;
			}

			if(iResult<PARADATA()->GetBPMMinPara_IPPV())
			{
				return false;
			}
			PRESET()->SetBPMPara_IPPV(iResult,false,false);
		}
		break;
	case VM_PRE_SIPPV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_PSV:
		{
			iResult = CTlsFloat::Round(60000/(double)((PRESET()->GetETIMEPara_TRIGGER()+fITime)), 0);

			if(iResult>PARADATA()->GetBPMMaxPara_TRIGGER())
			{
				return false;
			}

			if(iResult<PARADATA()->GetBPMMinPara_TRIGGER())
			{
				return false;
			}
			PRESET()->SetBPMPara_TRIGGER(iResult,false,false);
		}
		break;
	case VM_PRE_DUOPAP:
		{
			iResult = CTlsFloat::Round(60000/(double)((PRESET()->GetETIMENMODEPara()+fITime)), 0);

			if(iResult>PARADATA()->GetBPMNMODEMaxPara())
			{
				return false;
			}

			if(iResult<PARADATA()->GetBPMNMODEMinPara())
			{
				return false;
			}
			PRESET()->SetBPMNMODEPara(iResult,false,false);
		}
		break;
	default:
		{
			CStringW sz=_T("");
			sz.Format(_T("#HFO:0077 %d"),(int)getModel()->getCONFIG()->GetCurMode());
			theApp.getLog()->WriteLine(sz);
		}
		break;
	}

	return true;
}

bool CDataHandler::CalculateETime_BPMChanged(int iBpm, bool bTemp)
{
	//double fResult=0;
	int iResult=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			iResult = CTlsFloat::Round((60000/(double)(iBpm))-PARADATA()->GetITimePara_IPPV(), 0);

			if(iResult>PARADATA()->GetETIMEMaxPara_IPPV())
			{
				return false;
			}

			if(iResult<PARADATA()->GetETIMEMinPara_IPPV())
			{
				return false;
			}

			if(!bTemp)
			{
				//new paradata
				//PARADATA()->SetETIMEPara(iResult, true);
				SetETIMEParadata_IPPV(iResult, true);
			}
			SetCurrentTempETime(iResult);
			SetCurrentTempITime(PARADATA()->GetITimePara_IPPV());
		}
		break;
	case VM_SIPPV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			iResult = CTlsFloat::Round((60000/(double)(iBpm))-PARADATA()->GetITimePara_TRIGGER(), 0);

			if(iResult>PARADATA()->GetETIMEMaxPara_TRIGGER())
			{
				return false;
			}

			if(iResult<PARADATA()->GetETIMEMinPara_TRIGGER())
			{
				return false;
			}

			if(!bTemp)
			{
				//new paradata
				//PARADATA()->SetETIMEPara(iResult, true);
				SetETIMEParadata_TRIGGER(iResult, true);
			}
			SetCurrentTempETime(iResult);
			SetCurrentTempITime(PARADATA()->GetITimePara_TRIGGER());
		}
		break;
	case VM_DUOPAP:
		{
			iResult = CTlsFloat::Round((60000/(double)(iBpm))-GetCurrentITimePara(), 0);

			if(iResult>PARADATA()->GetETIMENMODEMaxPara())
			{
				return false;
			}

			if(iResult<PARADATA()->GetETIMENMODEMinPara())
			{
				return false;
			}

			if(!bTemp)
			{
				//new paradata
				//PARADATA()->SetETIMENMODEPara(iResult, true);
				SetETIMENMODEParadata(iResult, true);
			}
			SetCurrentTempETime(iResult);
			SetCurrentTempITime(PARADATA()->GetITimeNMODEPara());
		}
		break;
	case VM_PRE_IPPV:
		{
			iResult = CTlsFloat::Round((60000/(double)(iBpm))-PRESET()->GetITimePara_IPPV(), 0);

			if(iResult>PARADATA()->GetETIMEMaxPara_IPPV())
			{
				return false;
			}

			if(iResult<PARADATA()->GetETIMEMinPara_IPPV())
			{
				return false;
			}

			if(!bTemp)
				PRESET()->SetETIMEPara_IPPV(iResult, false,false);
			SetCurrentTempETime(iResult);
			SetCurrentTempITime(PRESET()->GetITimePara_IPPV());
		}
		break;
	case VM_PRE_SIPPV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_PSV:
		{
			iResult = CTlsFloat::Round((60000/(double)(iBpm))-PRESET()->GetITimePara_TRIGGER(), 0);

			if(iResult>PARADATA()->GetETIMEMaxPara_TRIGGER())
			{
				return false;
			}

			if(iResult<PARADATA()->GetETIMEMinPara_TRIGGER())
			{
				return false;
			}

			if(!bTemp)
				PRESET()->SetETIMEPara_TRIGGER(iResult, false,false);
			SetCurrentTempETime(iResult);
			SetCurrentTempITime(PRESET()->GetITimePara_TRIGGER());
		}
		break;
	case VM_PRE_DUOPAP:
		{
			iResult = CTlsFloat::Round((60000/(double)(iBpm))-GetCurrentITimePara(), 0);

			if(iResult>PARADATA()->GetETIMENMODEMaxPara())
			{
				return false;
			}

			if(iResult<PARADATA()->GetETIMENMODEMinPara())
			{
				return false;
			}

			if(!bTemp)
				PRESET()->SetETIMENMODEPara(iResult, false,false);
			SetCurrentTempETime(iResult);
			SetCurrentTempITime(PRESET()->GetITimeNMODEPara());
		}
		break;
	default:
		{
			CStringW sz=_T("");
			sz.Format(_T("#HFO:0078 %d"),(int)getModel()->getCONFIG()->GetCurMode());
			theApp.getLog()->WriteLine(sz);
		}
		break;
	}

	return true;
}

bool CDataHandler::CalculateBPM_ETimeChanged(int fETime, bool bTemp)
{
	//double dResult=0;
	int iResult=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			iResult = CTlsFloat::Round((60000/((double)(fETime+PARADATA()->GetITimePara_IPPV()))), 0);

			if(iResult>PARADATA()->GetBPMMaxPara_IPPV())
			{
				return false;
			}

			if(iResult<PARADATA()->GetBPMMinPara_IPPV())
			{
				return false;
			}

			SetCurrentTempETime(fETime);
			if(!bTemp)
			{
				SetETIMEParadata_IPPV(fETime, true);
			}
			SetBPMParadata_IPPV(iResult,true);
		}
		break;
	case VM_SIPPV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			iResult = CTlsFloat::Round((60000/((double)(fETime+PARADATA()->GetITimePara_TRIGGER()))), 0);

			if(iResult>PARADATA()->GetBPMMaxPara_TRIGGER())
			{
				return false;
			}

			if(iResult<PARADATA()->GetBPMMinPara_TRIGGER())
			{
				return false;
			}

			SetCurrentTempETime(fETime);
			if(!bTemp)
			{
				SetETIMEParadata_TRIGGER(fETime, true);
			}
			SetBPMParadata_TRIGGER(iResult,true);
		}
		break;
	case VM_DUOPAP:
		{
			iResult = CTlsFloat::Round((60000/((double)(fETime+GetCurrentITimePara()))), 0);

			if(iResult>PARADATA()->GetBPMNMODEMaxPara())
			{
				return false;
			}

			if(iResult<PARADATA()->GetBPMNMODEMinPara())
			{
				return false;
			}

			SetCurrentTempETime(fETime);
			if(!bTemp)
			{
				//new paradata
				//PARADATA()->SetETIMENMODEPara(fETime, true);
				SetETIMENMODEParadata(fETime, true);
			}
			//new paradata
			//PARADATA()->SetBPMNMODEPara(iResult);
			SetBPMNMODEParadata(iResult,true);
		}
		break;
	case VM_PRE_IPPV:
		{
			iResult = CTlsFloat::Round((60000/((double)(fETime+PRESET()->GetITimePara_IPPV()))), 0);

			if(iResult>PARADATA()->GetBPMMaxPara_IPPV())
			{
				return false;
			}

			if(iResult<PARADATA()->GetBPMMinPara_IPPV())
			{
				return false;
			}

			SetCurrentTempETime(fETime);
			if(!bTemp)
				PRESET()->SetETIMEPara_IPPV(fETime, false,false);
			PRESET()->SetBPMPara_IPPV(iResult,false,false);
		}
		break;
	case VM_PRE_SIPPV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_PSV:
		{
			iResult = CTlsFloat::Round((60000/((double)(fETime+PRESET()->GetITimePara_TRIGGER()))), 0);

			if(iResult>PARADATA()->GetBPMMaxPara_TRIGGER())
			{
				return false;
			}

			if(iResult<PARADATA()->GetBPMMinPara_TRIGGER())
			{
				return false;
			}

			SetCurrentTempETime(fETime);
			if(!bTemp)
				PRESET()->SetETIMEPara_TRIGGER(fETime, false,false);
			PRESET()->SetBPMPara_TRIGGER(iResult,false,false);
		}
		break;
	case VM_PRE_DUOPAP:
		{
			iResult = CTlsFloat::Round((60000/((double)(fETime+GetCurrentITimePara()))), 0);

			if(iResult>PARADATA()->GetBPMNMODEMaxPara())
			{
				return false;
			}

			if(iResult<PARADATA()->GetBPMNMODEMinPara())
			{
				return false;
			}

			SetCurrentTempETime(fETime);
			if(!bTemp)
				PRESET()->SetETIMENMODEPara(fETime, false,false);
			PRESET()->SetBPMNMODEPara(iResult,false,false);
		}
		break;
	default:
		{
			CStringW sz=_T("");
			sz.Format(_T("#HFO:0079 %d"),(int)getModel()->getCONFIG()->GetCurMode());
			theApp.getLog()->WriteLine(sz);
		}
		break;
	}

	return true;
}

//***************************************************************************
//     
//**************************************************************************
int CDataHandler::GetCurrentTempITime()
{
	return m_iTempITime;
}
void CDataHandler::SetCurrentTempITime(int val)
{
	m_iTempITime=val;
}



//***************************************************************************
//     
//**************************************************************************
int CDataHandler::GetSaveIERelationValue_ITIME()
{
	return m_iSaveIERelationValue_ITIME;
}
void CDataHandler::SetSaveIERelationValue_ITIME(int val)
{
	m_iSaveIERelationValue_ITIME=val;
}
//***************************************************************************
//     
//**************************************************************************
int CDataHandler::GetSaveIERelationValue_ETIME()
{
	return m_iSaveIERelationValue_ETIME;
}
void CDataHandler::SetSaveIERelationValue_ETIME(int val)
{
	m_iSaveIERelationValue_ETIME=val;
}



//***************************************************************************
//     
//**************************************************************************
int CDataHandler::GetCurrentTempETime()
{
	return m_iTempETime;
}
void CDataHandler::SetCurrentTempETime(int val)
{
	m_iTempETime=val;
}

//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitVgaranty()
{
	int iValue = 0;
	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		iValue = getAVGMessureDataTVEHFO();
	}
	else
	{
		bool bSIMVmode=false;
		if(getModel()->getCONFIG()->CurModeIsPresetMode())
		{
			if(		getModel()->getCONFIG()->GetPrevMode()==VM_SIMV
				||	getModel()->getCONFIG()->GetPrevMode()==VM_SIMVPSV)
			{
				bSIMVmode=true;
			}
		}
		else
		{
			if(		getModel()->getCONFIG()->GetCurMode()==VM_SIMV
				||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV)
			{
				bSIMVmode=true;
			}
		}

		if(bSIMVmode)
		{
			iValue = getMessureDataTVEresp();
		}
		else
		{
			iValue = getMessureDataTVE();
		}
	}

	WORD iMinVG=GetCurrentVGarantMinParaData();

	if(iValue<iMinVG)
	{
		iValue=iMinVG;
	}
	else if(iValue<=30)			
	{
		//iValue=(iValue/10)*10;
	}
	else if(iValue<=100)			
	{
		iValue=(iValue/2)*2;
	}
	else if(iValue<=200)			
	{
		iValue=(CTlsFloat::Round(((double)iValue)/10, 0))*10;
	}
	else if(iValue<=500)			
	{
		iValue=(CTlsFloat::Round(((double)iValue)/20, 0))*20;
	}
	else if(iValue<=1000)			
	{
		iValue=(CTlsFloat::Round(((double)iValue)/50, 0))*50;
	}
	else if(iValue<=2000)			
	{
		iValue=(CTlsFloat::Round(((double)iValue)/100, 0))*100;
	}
	else if(iValue<=5000)			
	{
		iValue=(CTlsFloat::Round(((double)iValue)/200, 0))*200;
	}
	else		
	{
		iValue=(iValue/500)*500;
	}

	return iValue;
}

//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitVlimit()
{
	int iValue = 0;
	bool bSIMVmode=false;
	if(getModel()->getCONFIG()->CurModeIsPresetMode())
	{
		if(		getModel()->getCONFIG()->GetPrevMode()==VM_SIMV
			||	getModel()->getCONFIG()->GetPrevMode()==VM_SIMVPSV)
		{
			bSIMVmode=true;
		}
	}
	else
	{
		if(		getModel()->getCONFIG()->GetCurMode()==VM_SIMV
			||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV)
		{
			bSIMVmode=true;
		}
	}

	if(bSIMVmode)
	{
		iValue = getMessureDataTVEresp();
	}
	else
	{
		iValue = getMessureDataTVE();
	}
	 
	if(iValue!=0)
		iValue = ((double)iValue/100)*AUTOALARMLIMIT_VLIMIT;

	
	if(iValue>=5000)
	{
		iValue=(iValue/500)*500;
	}
	else if(iValue>=2000)
	{
		iValue=(iValue/200)*200;
	}
	else if(iValue>=1000)
	{
		iValue=(iValue/100)*100;
	}
	else if(iValue>=500)
	{
		iValue=(iValue/50)*50;
	}
	else if(iValue>=200)
	{
		iValue=(iValue/20)*20;
	}
	else if(iValue>=100)
	{
		iValue=(iValue/10)*10;
	}
	else if(iValue>=50)
	{
		iValue=(iValue/5)*5;
	}
	else if(iValue>=30)			
	{
		iValue=(iValue/2)*2;
	}
	else
		iValue=GetVLimitMinPara();
	
	

	return iValue;
}

//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitMVmax()
{
	int iValue=getAVGMessureDataMV();
	if(iValue!=0)
	{
		iValue = ((double)iValue/100)*AUTOALARMLIMIT_MVMAX;
		if(iValue>=1000)
		{
			iValue=CTlsFloat::Round(((double)iValue)/1000, 1)*1000;
		}
		else
		{
			iValue=CTlsFloat::Round(((double)iValue)/1000, 2)*1000;
		}
	}
	else
	{
		CStringW sz=_T("");
		sz.Format(_T("#HFO:0080"));
		theApp.getLog()->WriteLine(sz);

		iValue=FACTORY_ALIMIT_VAL_MVMAX;
		getModel()->getALARMHANDLER()->setAlimitMVmin(FACTORY_ALIMIT_VAL_MVMIN);
	}

	getModel()->getALARMHANDLER()->setAlimitMVmax(iValue);

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitMVmin()
{
	int iValue = getAVGMessureDataMV();
	if(iValue!=0)
	{
		iValue = ((double)iValue/100)*AUTOALARMLIMIT_MVMIN;
		if(iValue>=1000)
		{
			iValue=CTlsFloat::Round(((double)iValue)/1000, 1)*1000;
		}
		else
		{
			iValue=CTlsFloat::Round(((double)iValue)/1000, 2)*1000;
		}

	}
	else
	{
		iValue=FACTORY_ALIMIT_VAL_MVMIN;
		getModel()->getALARMHANDLER()->setAlimitMVmax(FACTORY_ALIMIT_VAL_MVMAX);
	}
	getModel()->getALARMHANDLER()->setAlimitMVmin(iValue);

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitPIPHIGH()
{
	//DEBUGMSG(TRUE, (TEXT("calculateAutoAlarmlimitPIPHIGH\r\n")));
	int iValue = 0;

	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

	if(eActiveVentMode==VM_THERAPIE)
	{

	}
	else if(eActiveVentMode==VM_HFO)
	{
		double fPpeak=0;
		double fAuto=0;
		eRatioIE ratio = RIE_1_1;

		if(true==IsActiveModeVGarantStateOn())
		{
			if(		isLUNGRECLicenseAvailable()==true 
				&&	PARADATA()->GetFreqRecPara()!=0)
			{
				int iAmp=PARADATA()->GetHFAMPLmaxPara();
				int iPmeanrec=PARADATA()->GetHFPMeanRecPara();

				ratio = PARADATA()->GetIERatioParaHFO();

				switch(ratio)
				{
				case RIE_1_3:
					{
						fPpeak=iPmeanrec+(iAmp*0.75);
						fAuto=(fPpeak/100)*25;
					}
					break;
				case RIE_1_2:
					{
						fPpeak=iPmeanrec+(iAmp*0.65);
						fAuto=(fPpeak/100)*15;
					}
					break;
				case RIE_1_1:
					{
						fPpeak=iPmeanrec+(iAmp*0.55);
						fAuto=(fPpeak/100)*2;
					}
					break;
				}

				iValue=(int)(fPpeak+fAuto+AUTOALARMLIMIT_PMAXHFO);
			}
			else
			{
				int iAmp=PARADATA()->GetHFAMPLmaxPara();
				int iPmean=PARADATA()->GetHFPMeanPara();

				
				ratio = PARADATA()->GetIERatioParaHFO();

				switch(ratio)
				{
				case RIE_1_3:
					{
						fPpeak=iPmean+(iAmp*0.75);
						fAuto=(fPpeak/100)*25;
					}
					break;
				case RIE_1_2:
					{
						fPpeak=iPmean+(iAmp*0.65);
						fAuto=(fPpeak/100)*15;
					}
					break;
				case RIE_1_1:
					{
						fPpeak=iPmean+(iAmp*0.55);
						fAuto=(fPpeak/100)*2;
					}
					break;
				}

				iValue=(int)(fPpeak+fAuto+AUTOALARMLIMIT_PMAXHFO);
			}
		}
		else
		{
			if(		isLUNGRECLicenseAvailable()==true 
				&&	PARADATA()->GetFreqRecPara()!=0)
			{
				int iAmp=PARADATA()->GetHFAMPLPara();
				int iPmeanrec=PARADATA()->GetHFPMeanRecPara();

				ratio = PARADATA()->GetIERatioParaHFO();

				switch(ratio)
				{
				case RIE_1_3:
					{
						fPpeak=iPmeanrec+(iAmp*0.75);
						fAuto=(fPpeak/100)*25;
					}
					break;
				case RIE_1_2:
					{
						fPpeak=iPmeanrec+(iAmp*0.65);
						fAuto=(fPpeak/100)*15;
					}
					break;
				case RIE_1_1:
					{
						fPpeak=iPmeanrec+(iAmp*0.55);
						fAuto=(fPpeak/100)*2;
					}
					break;
				}

				iValue=(int)(fPpeak+fAuto+AUTOALARMLIMIT_PMAXHFO);
			}
			else
			{
				/*eAlarmLimitState state=getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit();
				if(state==AL_AUTO)
				{
					getModel()->getALARMHANDLER()->setAlimitState_PIPmaxLimit(AL_ON);
				}
				iValue = getAVGMessureDataPINSP()+AUTOALARMLIMIT_PMAXHFO;*/

				int iAmp=PARADATA()->GetHFAMPLPara();
				int iPmean=PARADATA()->GetHFPMeanPara();

				ratio = PARADATA()->GetIERatioParaHFO();

				switch(ratio)
				{
				case RIE_1_3:
					{
						fPpeak=iPmean+(iAmp*0.75);
						fAuto=(fPpeak/100)*25;
					}
					break;
				case RIE_1_2:
					{
						fPpeak=iPmean+(iAmp*0.65);
						fAuto=(fPpeak/100)*15;
					}
					break;
				case RIE_1_1:
					{
						fPpeak=iPmean+(iAmp*0.55);
						fAuto=(fPpeak/100)*2;
					}
					break;
				}

				iValue=(int)(fPpeak+fAuto+AUTOALARMLIMIT_PMAXHFO);
			}
		}

		

		if(iValue==0)
		{
			CStringW sz=_T("");
			sz.Format(_T("#HFO:0081: Pmax %d, Ppeak %d, Auto %d, RatioIE %d"), getAVGMessureDataPINSP(),fPpeak,fAuto,ratio);
			theApp.getLog()->WriteLine(sz);
		}
		
	}
	else if(eActiveVentMode==VM_CPAP)
	{
		iValue = PARADATA()->GetCPAPPara()+AUTOALARMLIMIT_CPAP;
		int iPminLimit=getModel()->getALARMHANDLER()->getAlimitPEEPmin();
		if(iValue<iPminLimit+DIFF_PMAX_PMIN_LIMIT)
		{
			if(iPminLimit%10==0)
			{
				iValue=iPminLimit+DIFF_PMAX_PMIN_LIMIT;
			}
			else
			{
				iValue=iPminLimit+5;
			}
		}

	}
	else if(eActiveVentMode==VM_NCPAP)
	{
		iValue = PARADATA()->GetCPAPNMODEPara()+AUTOALARMLIMIT_CPAP;
		int iPminLimit=getModel()->getALARMHANDLER()->getAlimitPEEPmin();
		if(iValue<iPminLimit+DIFF_PMAX_PMIN_LIMIT)
		{
			if(iPminLimit%10==0)
			{
				iValue=iPminLimit+DIFF_PMAX_PMIN_LIMIT;
			}
			else
			{
				iValue=iPminLimit+5;
			}
		}

	}
	else if(eActiveVentMode==VM_DUOPAP)
	{
		iValue = PARADATA()->GetPManualNMODEPara()+AUTOALARMLIMIT_CPAP;
		int iPminLimit=getModel()->getALARMHANDLER()->getAlimitPEEPmin();
		if(iValue<iPminLimit+DIFF_PMAX_PMIN_LIMIT)
		{
			if(iPminLimit%10==0)
			{
				iValue=iPminLimit+DIFF_PMAX_PMIN_LIMIT;
			}
			else
			{
				iValue=iPminLimit+5;
			}
		}

	}
	else
	{
		if(IsCurrentModeVGarantStateOn())//newVG
			iValue = GetActiveModePMAXVGPara()+AUTOALARMLIMIT_PIPMAX;
		else
			iValue = GetActiveModePINSPPara()+AUTOALARMLIMIT_PIPMAX;
		int iPEEPminLimit=getModel()->getALARMHANDLER()->getAlimitPEEPmin();
		int iPIPminLimit=getModel()->getALARMHANDLER()->getAlimitPIPmin();
		
		if(iValue<iPIPminLimit+DIFF_PMAX_PMIN_LIMIT)
		{
			iValue=iPIPminLimit-DIFF_PMAX_PMIN_LIMIT;
		}
		else if(iValue<iPEEPminLimit+DIFF_PMAX_PMIN_LIMIT)
		{
			if(iPEEPminLimit%10==0)
			{
				iValue=iPEEPminLimit+DIFF_PMAX_PMIN_LIMIT;
			}
			else
			{
				iValue=iPEEPminLimit+5;
			}
		}




		/*if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
		{
			int iCurPEEP=getModel()->getALARMHANDLER()->getAlimitPEEPmin();
			if(iCurPEEP%10!=0)
			{
				iCurPEEP+=5;
			}
			iCurPEEP = iCurPEEP+AUTOALARMLIMIT_PIPMIN;

			int iPIPmaxLimit=getModel()->getALARMHANDLER()->getAlimitPIPmax();
			if(iCurPEEP>iPIPmaxLimit-DIFF_PMAX_PMIN_LIMIT)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_CALC_AUTOALARMLIMITS_PIPLOW);
			}
		}*/
	}

	
	getModel()->getALARMHANDLER()->setAlimitPIPmax(iValue);

	return iValue;
}

//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitPIPLOW()
{
	int iValue = 0;

	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

	if(eActiveVentMode==VM_THERAPIE)
	{
	}
	else if(eActiveVentMode==VM_HFO)
	{
	}
	else if(eActiveVentMode==VM_CPAP)
	{
	}
	else if(eActiveVentMode==VM_NCPAP)
	{
	}
	else if(eActiveVentMode==VM_DUOPAP)
	{
	}
	else
	{
		int iCurPEEP=getModel()->getALARMHANDLER()->getAlimitPEEPmin();
		if(iCurPEEP%10!=0)
		{
			iCurPEEP+=5;
		}
		iValue = iCurPEEP+AUTOALARMLIMIT_PIPMIN;

		int iPIPmaxLimit=getModel()->getALARMHANDLER()->getAlimitPIPmax();
		if(iValue>iPIPmaxLimit-DIFF_PMAX_PMIN_LIMIT)
		{
			iValue=iPIPmaxLimit-DIFF_PMAX_PMIN_LIMIT;
		}
	}
	
	getModel()->getALARMHANDLER()->setAlimitPIPmin(iValue);

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitPEEPmin()
{
	//DEBUGMSG(TRUE, (TEXT("calculateAutoAlarmlimitPEEPmin\r\n")));
	int iValue = 0;

	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

	if(eActiveVentMode==VM_HFO)
	{
		
	}
	else if(eActiveVentMode==VM_CPAP)
	{
		iValue = PARADATA()->GetCPAPPara()-AUTOALARMLIMIT_CPAP;

		int iPmaxLimit=getModel()->getALARMHANDLER()->getAlimitPIPmax();
		if(iValue>iPmaxLimit-DIFF_PMIN_PMAX_LIMIT)
		{
			iValue=iPmaxLimit-DIFF_PMIN_PMAX_LIMIT;
		}
		getModel()->getALARMHANDLER()->setAlimitPEEPmin(iValue);
	}
	else if(eActiveVentMode==VM_THERAPIE)
	{

	}
	else if(eActiveVentMode==VM_NCPAP)
	{
		iValue = PARADATA()->GetCPAPNMODEPara()-AUTOALARMLIMIT_NCPAP;
		int iPmaxLimit=getModel()->getALARMHANDLER()->getAlimitPIPmax();
		if(iValue>iPmaxLimit-DIFF_PMIN_PMAX_LIMIT)
		{
			iValue=iPmaxLimit-DIFF_PMIN_PMAX_LIMIT;
		}
		getModel()->getALARMHANDLER()->setAlimitPEEPmin(iValue);
	}
	else if(eActiveVentMode==VM_DUOPAP)
	{
		iValue = PARADATA()->GetCPAPNMODEPara()-AUTOALARMLIMIT_DUOPAP;
		int iPmaxLimit=getModel()->getALARMHANDLER()->getAlimitPIPmax();
		if(iValue>iPmaxLimit-DIFF_PMIN_PMAX_LIMIT)
		{
			iValue=iPmaxLimit-DIFF_PMIN_PMAX_LIMIT;
		}
		getModel()->getALARMHANDLER()->setAlimitPEEPmin(iValue);
	}
	else
	{
		if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
			iValue = PARADATA()->GetPEEPPara_IPPV()-AUTOALARMLIMIT_PEEP;
		else
			iValue = PARADATA()->GetPEEPPara_TRIGGER()-AUTOALARMLIMIT_PEEP;
		int iPmaxLimit=getModel()->getALARMHANDLER()->getAlimitPIPmax();
		if(iValue>iPmaxLimit-DIFF_PMIN_PMAX_LIMIT)
		{
			iValue=iPmaxLimit-DIFF_PMIN_PMAX_LIMIT;
		}
		getModel()->getALARMHANDLER()->setAlimitPEEPmin(iValue);

		if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
		{
			int iCurPEEP=getModel()->getALARMHANDLER()->getAlimitPEEPmin();
			if(iCurPEEP%10!=0)
			{
				iCurPEEP+=5;
			}
			
			int iPIPminLimit=getModel()->getALARMHANDLER()->getAlimitPIPmin();
			if(iCurPEEP>iPIPminLimit)
			{
				getModel()->getALARMHANDLER()->setAlimitPIPmin(iCurPEEP);
				/*if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_CALC_AUTOALARMLIMITS_PIPLOW);*/
			}
		}
	}

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitBPMmax()
{
	int iValue = getAVGMessureDataBPM();
	if(iValue!=0)
		iValue = (int)CTlsFloat::Round(((double)iValue/100)*AUTOALARMLIMIT_BPM, 0);
	else
		iValue=FACTORY_ALIMIT_VAL_BPMMAX;
	getModel()->getALARMHANDLER()->setAlimitBPMmax(iValue);

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitLeakmax()
{
	int iValue = getAVGMessureDataLeak();
	if(iValue!=0)
		iValue = (int)CTlsFloat::Round(((double)iValue/100)*AUTOALARMLIMIT_LEAK, 0);
	else
		iValue=AUTOALARMLIMIT_LEAKMIN;

	if(iValue>AUTOALARMLIMIT_LEAKMAX)
		iValue=AUTOALARMLIMIT_LEAKMAX;
	else if(iValue<AUTOALARMLIMIT_LEAKMIN)
		iValue=AUTOALARMLIMIT_LEAKMIN;
	getModel()->getALARMHANDLER()->setAlimitLeakmax(iValue);

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitApnoe()
{
	getModel()->getALARMHANDLER()->setAlimitApnoe(AUTOALARMLIMIT_APNOE);

	return AUTOALARMLIMIT_APNOE;
}



int CDataHandler::calculateAutoAlarmlimitMAPmax()
{
	int iValue = 0;

	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

	if(eActiveVentMode==VM_THERAPIE)
	{
	}
	else if(eActiveVentMode==VM_HFO)
	{
		iValue=PARADATA()->GetHFPMeanPara()+AUTOALARMLIMIT_MAPMAX;
	}
	else if(eActiveVentMode==VM_CPAP)
	{
	}
	else if(eActiveVentMode==VM_NCPAP)
	{
	}
	else if(eActiveVentMode==VM_DUOPAP)
	{
	}
	else
	{
		
	}

	getModel()->getALARMHANDLER()->setAlimitMAPmax(iValue);

	return iValue;
}

int CDataHandler::calculateAutoAlarmlimitMAPmin()
{
	int iValue = 0;

	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

	if(eActiveVentMode==VM_THERAPIE)
	{
	}
	else if(eActiveVentMode==VM_HFO)
	{
		iValue=PARADATA()->GetHFPMeanPara()-AUTOALARMLIMIT_MAPMIN;
	}
	else if(eActiveVentMode==VM_CPAP)
	{
	}
	else if(eActiveVentMode==VM_NCPAP)
	{
	}
	else if(eActiveVentMode==VM_DUOPAP)
	{
	}
	else
	{

	}

	getModel()->getALARMHANDLER()->setAlimitMAPmin(iValue);

	return iValue;
}

//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitDCO2max()
{
	int iValue = getAVGMessureDataDCO2();

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO()==false)
	{
		iValue= FACTORY_ALIMIT_VAL_DCO2MAX;
	}
	else
	{
		if(iValue!=0)
			iValue = ((double)iValue/100)*AUTOALARMLIMIT_DCO2MAX;
		else
		{
			iValue=AUTOALARMLIMIT_DCO2MAX;
		}
		if(iValue<=200)
		{

		}
		else if(iValue<=500)
		{
			iValue=(iValue/5)*5;
		}
		else if(iValue<=1000)
		{
			iValue=(iValue/10)*10;
		}
		else 
		{
			iValue=(iValue/100)*100;
		}
	}

	getModel()->getALARMHANDLER()->setAlimitDCO2max(iValue);

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitDCO2min()
{
	int iValue = getAVGMessureDataDCO2();

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO()==false)
		iValue =  FACTORY_ALIMIT_VAL_DCO2MIN;
	else
	{
		if(iValue!=0)
			iValue = ((double)iValue/100)*AUTOALARMLIMIT_DCO2MIN;
		else
			iValue=FACTORY_ALIMIT_VAL_DCO2MIN;
		if(iValue<=200)
		{
			//dValue
		}
		else if(iValue<=500)
		{
			iValue=(iValue/5)*5;
		}
		else if(iValue<=1000)
		{
			iValue=(iValue/10)*10;
		}
		else 
		{
			iValue=(iValue/100)*100;
		}
	}

	getModel()->getALARMHANDLER()->setAlimitDCO2min(iValue);

	return iValue;
}


//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitETCO2max()
{
	int iValue = getAVGMessureDataETCO2();

	if(getModel()->getETCO2())
	{
		if(		getModel()->getCONFIG()->getCO2module()==CO2MODULE_NONE
			||	false==getModel()->getETCO2()->isStateOk()
			||	getModel()->getALARMHANDLER()->ALARM_Sens_CO2_MODULE_NOTCONNECTED->getAlarmState()==AS_ACTIVE
			||	iValue==0)
		{
			iValue=FACTORY_ALIMIT_VAL_ETCO2MAX_MMHG;
			getModel()->getALARMHANDLER()->setAlimitETCO2min(FACTORY_ALIMIT_VAL_ETCO2MIN_MMHG);
		}
		else
		{
			if(iValue!=0)
				iValue = ((double)iValue/100)*AUTOALARMLIMIT_ETCO2MAX;
			else
			{
				iValue=FACTORY_ALIMIT_VAL_ETCO2MAX_MMHG;
			}
		}
	}
	else
	{
		iValue=FACTORY_ALIMIT_VAL_ETCO2MAX_MMHG;
	}
	

	getModel()->getALARMHANDLER()->setAlimitETCO2max(iValue);

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitETCO2min()
{
	int iValue = getAVGMessureDataETCO2();

	if(getModel()->getETCO2())
	{
		if(		getModel()->getCONFIG()->getCO2module()==CO2MODULE_NONE
			||	false==getModel()->getETCO2()->isStateOk()
			||	getModel()->getALARMHANDLER()->ALARM_Sens_CO2_MODULE_NOTCONNECTED->getAlarmState()==AS_ACTIVE
			||	iValue==0)
		{
			iValue = FACTORY_ALIMIT_VAL_ETCO2MIN_MMHG;
			getModel()->getALARMHANDLER()->setAlimitETCO2max(FACTORY_ALIMIT_VAL_ETCO2MAX_MMHG);
		}
		else
		{
			if(iValue!=0)
				iValue = ((double)iValue/100)*AUTOALARMLIMIT_ETCO2MIN;
			else
				iValue=FACTORY_ALIMIT_VAL_ETCO2MIN_MMHG;
		}
	}
	else
	{
		iValue=FACTORY_ALIMIT_VAL_ETCO2MIN_MMHG;
	}

	getModel()->getALARMHANDLER()->setAlimitETCO2min(iValue);

	return iValue;
}

//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitFICO2max()
{
	int iValue = getAVGMessureDataFICO2();

	if(getModel()->getETCO2())
	{
		if(		getModel()->getCONFIG()->getCO2module()==CO2MODULE_NONE
			||	false==getModel()->getETCO2()->isStateOk()
			||	getModel()->getALARMHANDLER()->ALARM_Sens_CO2_MODULE_NOTCONNECTED->getAlarmState()==AS_ACTIVE
			||	iValue==0)
		{
			iValue=FACTORY_ALIMIT_VAL_FICO2MAX_MMHG;
			getModel()->getALARMHANDLER()->setAlimitFICO2min(FACTORY_ALIMIT_VAL_FICO2MIN_MMHG);
		}
		else
		{
			if(iValue!=0)
				iValue = ((double)iValue/100)*AUTOALARMLIMIT_FICO2MAX;
			else
			{
				iValue=FACTORY_ALIMIT_VAL_FICO2MAX_MMHG;
			}
		}
	}
	else
	{
		iValue=FACTORY_ALIMIT_VAL_FICO2MAX_MMHG;
	}
	

	getModel()->getALARMHANDLER()->setAlimitFICO2max(iValue);

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitFICO2min()
{
	int iValue = getAVGMessureDataFICO2();

	if(getModel()->getETCO2())
	{
		if(		getModel()->getCONFIG()->getCO2module()==CO2MODULE_NONE
			||	false==getModel()->getETCO2()->isStateOk()
			||	getModel()->getALARMHANDLER()->ALARM_Sens_CO2_MODULE_NOTCONNECTED->getAlarmState()==AS_ACTIVE
			||	iValue==0)
		{
			iValue = FACTORY_ALIMIT_VAL_FICO2MIN_MMHG;
			getModel()->getALARMHANDLER()->setAlimitFICO2max(FACTORY_ALIMIT_VAL_FICO2MAX_MMHG);
		}
		else
		{
			if(iValue!=0)
				iValue = ((double)iValue/100)*AUTOALARMLIMIT_FICO2MIN;
			else
				iValue=FACTORY_ALIMIT_VAL_FICO2MIN_MMHG;
		}
	}
	else
	{
		iValue=FACTORY_ALIMIT_VAL_FICO2MIN_MMHG;
	}

	getModel()->getALARMHANDLER()->setAlimitFICO2min(iValue);

	return iValue;
}

//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitSPO2max()
{
	int iValue = getAVGMessureDataSPO2();

	if(getModel()->getSPO2())
	{
		if(		getModel()->getCONFIG()->getSPO2module()==SPO2MODULE_NONE
			||	false==getModel()->getSPO2()->isStateOk())
		{
			iValue = FACTORY_ALIMIT_VAL_SPO2MAX;
			getModel()->getALARMHANDLER()->setAlimitSPO2min(FACTORY_ALIMIT_VAL_SPO2MIN);
		}
		else
		{
			if(iValue!=0)
			{
				iValue = ((double)iValue/100)*AUTOALARMLIMIT_SPO2MAX;
				iValue=(iValue/10)*10;
			}
			else
				iValue=FACTORY_ALIMIT_VAL_SPO2MAX;
		}
	}
	else
	{
		iValue=FACTORY_ALIMIT_VAL_SPO2MAX;
	}
	

	if(iValue<getModel()->getALARMHANDLER()->getAlimitSPO2min()+20)
	{
		getModel()->getALARMHANDLER()->setAlimitSPO2min(iValue-20);
	}

	if(iValue>FACTORY_ALIMIT_VAL_SPO2MAX)
	{
		//iValue=FACTORY_ALIMIT_VAL_SPO2MAX;
		getModel()->getALARMHANDLER()->setAlimitState_SPO2maxLimit(AL_OFF);
	}

	getModel()->getALARMHANDLER()->setAlimitSPO2max(iValue);

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
int CDataHandler::calculateAutoAlarmlimitSPO2min()
{
	int iValue = getAVGMessureDataSPO2();

	if(getModel()->getSPO2())
	{
		if(		getModel()->getCONFIG()->getSPO2module()==SPO2MODULE_NONE
			||	false==getModel()->getSPO2()->isStateOk())
		{
			iValue = FACTORY_ALIMIT_VAL_SPO2MIN;
			getModel()->getALARMHANDLER()->setAlimitSPO2max(FACTORY_ALIMIT_VAL_SPO2MAX);
		}
		else
		{
			if(iValue!=0)
			{
				iValue = ((double)iValue/100)*AUTOALARMLIMIT_SPO2MIN;
				iValue=(iValue/10)*10;
			}
			else
				iValue=FACTORY_ALIMIT_VAL_SPO2MIN;
		}
	}
	else
	{
		iValue=FACTORY_ALIMIT_VAL_SPO2MIN;
	}
	

	if(iValue>getModel()->getALARMHANDLER()->getAlimitSPO2max()-20)
	{
		getModel()->getALARMHANDLER()->setAlimitSPO2max(iValue+20);
	}
	else if(iValue<10)
	{
		//iValue=10;
		getModel()->getALARMHANDLER()->setAlimitState_SPO2minLimit(AL_OFF);
	}

	getModel()->getALARMHANDLER()->setAlimitSPO2min(iValue);

	return iValue;
}
int CDataHandler::calculateAutoAlarmlimitPulseRatemax()
{
	int iValue = getAVGMessureDataSpO2PulseRate();
	
	if(getModel()->getSPO2())
	{
		if(		getModel()->getCONFIG()->getSPO2module()==SPO2MODULE_NONE
			||	false==getModel()->getSPO2()->isStateOk())
		{
			iValue = FACTORY_ALIMIT_VAL_PULSERATEMAX;
			getModel()->getALARMHANDLER()->setAlimitPulseRatemin(FACTORY_ALIMIT_VAL_PULSERATEMIN);
		}
		else
		{
			if(iValue!=0)
			{
				iValue=(iValue/5)*5;
				if(iValue!=0)
					iValue = ((double)iValue/100)*AUTOALARMLIMIT_PULSERATEMAX;
			}
			else
				iValue=FACTORY_ALIMIT_VAL_PULSERATEMAX;
		}
	}
	else
	{
		iValue=FACTORY_ALIMIT_VAL_PULSERATEMAX;
	}
	

	getModel()->getALARMHANDLER()->setAlimitPulseRatemax(iValue);

	return iValue;
}
int CDataHandler::calculateAutoAlarmlimitPulseRatemin()
{
	int iValue = getAVGMessureDataSpO2PulseRate();

	if(getModel()->getSPO2())
	{
		if(		getModel()->getCONFIG()->getSPO2module()==SPO2MODULE_NONE
			||	false==getModel()->getSPO2()->isStateOk())
		{
			iValue = FACTORY_ALIMIT_VAL_PULSERATEMIN;
			getModel()->getALARMHANDLER()->setAlimitPulseRatemax(FACTORY_ALIMIT_VAL_PULSERATEMAX);
		}
		else
		{
			if(iValue!=0)
			{
				iValue=(iValue/5)*5;
				if(iValue!=0)
					iValue = ((double)iValue/100)*AUTOALARMLIMIT_PULSERATEMIN;
			}
			else
				iValue=FACTORY_ALIMIT_VAL_PULSERATEMIN;
		}
	}
	else
	{
		iValue=FACTORY_ALIMIT_VAL_PULSERATEMIN;
	}
	

	getModel()->getALARMHANDLER()->setAlimitPulseRatemin(iValue);

	return iValue;
}
int CDataHandler::calculateAutoAlarmlimitSPO2_PImin()
{
	int iValue = getAVGMessureDataSpO2PerfusionIndex();

	if(getModel()->getSPO2())
	{
		if(		getModel()->getCONFIG()->getSPO2module()==SPO2MODULE_NONE
			||	false==getModel()->getSPO2()->isStateOk())
		{
			iValue=FACTORY_ALIMIT_VAL_SPO2_PIMIN;
		}
		else
		{
			if(iValue!=0)
				iValue = ((double)iValue/100)*AUTOALARMLIMIT_SPO2_PIMIN;
			else
				iValue=FACTORY_ALIMIT_VAL_SPO2_PIMIN;
		}
	}
	else
	{
		iValue=FACTORY_ALIMIT_VAL_SPO2_PIMIN;
	}

	getModel()->getALARMHANDLER()->setAlimitSPO2_PImin(iValue);

	return iValue;
}

int CDataHandler::calculateAutoAlarmlimitSPO2_SIQmin()
{
	getModel()->getALARMHANDLER()->setAlimitSPO2_SIQmin(AUTOALARMLIMIT_SPO2_SIQMIN);
	return AUTOALARMLIMIT_SPO2_SIQMIN;
}

int CDataHandler::GetCurrentIERelation100()
{
	double fValue=0;
	int iETimeValue=GetCurrentTempETime();
	int iITimeValue=GetCurrentTempITime();
	
	if(iETimeValue==0)
		return 0;

	fValue= (double)iETimeValue/(double)iITimeValue;

	return (int)(fValue*100);
}

int CDataHandler::GetCurrentEIRelation100()
{
	double fValue=0;
	int iETimeValue=GetCurrentTempETime();
	int iITimeValue=GetCurrentTempITime();
	
	if(iITimeValue==0)
		return 0;

	fValue= (double)iITimeValue/(double)iETimeValue;

	return (int)(fValue*100);
}




void CDataHandler::setTriggerOptionCONV(eTriggereType type)
{
	//DEBUGMSG(TRUE, (TEXT("setTriggerOption_CONV %d\r\n"),(int)type));
	getModel()->getCONFIG()->setTriggerOption_CONV(type);

	getModel()->Send_MODE_OPTION1();
}
eTriggereType CDataHandler::getTriggerOptionCONV()
{
	return getModel()->getCONFIG()->getTriggerOption_CONV();
}
void CDataHandler::SetPrevTriggerOptionCONV(eTriggereType type)
{
	m_ePrevTrigger_CONV=type;
}
eTriggereType CDataHandler::getPrevTriggerOptionCONV()
{
	return m_ePrevTrigger_CONV;
}
//###########################
//void CDataHandler::setTriggerOption_CPAP(eTriggereType type)
//{
//	//DEBUGMSG(TRUE, (TEXT("setTriggerOption_CPAP %d\r\n"),(int)type));
//	getModel()->getCONFIG()->setTriggerOption_CPAP(type);
//
//	getModel()->Send_MODE_OPTION1();
//}
//eTriggereType CDataHandler::getTriggerOption_CPAP()
//{
//	return getModel()->getCONFIG()->getTriggerOption_CPAP();
//}
//void CDataHandler::SetPrevTriggerOption_CPAP(eTriggereType type)
//{
//	m_ePrevTrigger_CPAP=type;
//}
//eTriggereType CDataHandler::getPrevTriggerOption_CPAP()
//{
//	return m_ePrevTrigger_CPAP;
//}

void CDataHandler::setTriggerOptionDUOPAP(eTriggereType type)
{
	getModel()->getCONFIG()->setTriggerOption_DUOPAP(type);

	getModel()->Send_MODE_OPTION1();
}
eTriggereType CDataHandler::getTriggerOptionDUOPAP()
{
	return getModel()->getCONFIG()->getTriggerOption_DUOPAP();
}


void CDataHandler::setTriggerOptionNCPAP(eTriggereType type)
{
	getModel()->getCONFIG()->setTriggerOption_NCPAP(type);

	getModel()->Send_MODE_OPTION1();
}
eTriggereType CDataHandler::getTriggerOptionNCPAP()
{
	return getModel()->getCONFIG()->getTriggerOption_NCPAP();
}


void CDataHandler::setPpsvAsDeltaPEEPValue(BOOL bPPSVasDeltaPEEPValue)
{
	getModel()->getCONFIG()->setPpsvAsDeltaPEEPValue(bPPSVasDeltaPEEPValue);

	if(getModel()->getCONFIG()->CurModeIsPresetMode())
		getModel()->Send_VENT_MODE(getModel()->getCONFIG()->GetPrevMode());
	else
		getModel()->Send_VENT_MODE(getModel()->getCONFIG()->GetCurMode());
}

void CDataHandler::SetCurO2FlushTime(BYTE iTime)
{
	getModel()->getCONFIG()->SetFlushTime(iTime);

	if(0==iTime && true==getModel()->isO2FlushActive())
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_O2FLUSH_CHANGED);
	}
}

BYTE CDataHandler::GetCurO2FlushTime()
{
	return getModel()->getCONFIG()->GetFlushTime();
}


WORD CDataHandler::GetCurrentVGarantMinParaData()
{
	WORD iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			iValue=PARADATA()->GetVGarantMinPara_IPPV();
		}
		break;
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_PSV:
		{
			iValue=PARADATA()->GetVGarantMinPara_TRIGGER();
		}
		break;
	case VM_HFO:
		{
			iValue=PARADATA()->GetHFVGarantMinPara();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
		{
			iValue=GetPreviousVGarantMinParaData();
		}
		break;
	case VM_SERVICE:
		{
			iValue=GetPreviousVGarantMinParaData();
		}
		break;
	default:
		{
			}
		break;
	}

	return iValue;
}
WORD CDataHandler::GetPreviousVGarantMinParaData()
{
	WORD iVal=0;
	switch(getModel()->getCONFIG()->GetPrevMode())
	{
	case VM_IPPV:
		{
			iVal = PARADATA()->GetVGarantMinPara_IPPV();
		}
		break;
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			iVal = PARADATA()->GetVGarantMinPara_TRIGGER();
		}
		break;
	case VM_HFO:
		{
			iVal = PARADATA()->GetHFVGarantMinPara();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
	default:
		{
		}
		break;
	}

	return iVal;
}

WORD CDataHandler::GetCurrentVGarantParaData()
{
	WORD iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			iValue=PARADATA()->GetVGarantPara_IPPV();
		}
		break;
	case VM_SERVICE:
	case VM_SIMV:
	case VM_SIPPV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			iValue=PARADATA()->GetVGarantPara_TRIGGER();
		}
		break;
	case VM_HFO:
		{
			iValue=PARADATA()->GetHFVGarantPara();
		}
		break;
	case VM_PRE_IPPV:
		{
			iValue=PRESET()->GetVGarantPara_IPPV();
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
		{
			iValue=PRESET()->GetVGarantPara_TRIGGER();
		}
		break;
	case VM_PRE_HFO:
		{
			iValue=PRESET()->GetHFVGarantPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;

}


WORD CDataHandler::GetPreviousVGarantParaData()
{
	double iVal=0;
	switch(getModel()->getCONFIG()->GetPrevMode())
	{
	case VM_IPPV:
		{
			iVal = PARADATA()->GetVGarantPara_IPPV();
		}
		break;
	case VM_SIMV:
	case VM_SIPPV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			iVal = PARADATA()->GetVGarantPara_TRIGGER();
		}
		break;
	case VM_HFO:
		{
			iVal = PARADATA()->GetHFVGarantPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return iVal;
}



WORD CDataHandler::GetVLimitMinPara()
{
	WORD iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			iValue=PARADATA()->GetVLimitMinPara_IPPV();
		}
		break;
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			iValue=PARADATA()->GetVLimitMinPara_TRIGGER();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
	case VM_PRE_THERAPIE:
		{
			iValue=GetPrevVLimitMinPara();
		}
		break;
	case VM_SERVICE:
		{
			iValue=GetPrevVLimitMinPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}
WORD CDataHandler::GetPrevVLimitMinPara()
{
	WORD iVal=0;
	switch(getModel()->getCONFIG()->GetPrevMode())
	{
	case VM_IPPV:
		{
			iVal = PARADATA()->GetVLimitMinPara_IPPV();
		}
		break;
	case VM_PSV: 
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			iVal = PARADATA()->GetVLimitMinPara_TRIGGER();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
	case VM_PRE_THERAPIE:
		{
			iVal = 0;
		}
		break;
	default:
		{
		}
		break;
	}

	return iVal;
}

/**********************************************************************************************//**
 * Gets current v limit para.
 *
 * \author	Rainer
 * \date	12.07.2017
 *
 * \return	The current v limit para.
 **************************************************************************************************/

WORD CDataHandler::GetCurrentVLimitPara()
{
	WORD iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			iValue=PARADATA()->GetVLimitParam_IPPV();
		}
		break;
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			iValue=PARADATA()->GetVLimitParam_TRIGGER();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
	case VM_PRE_THERAPIE:
		{
			iValue=GetPrevVLimitPara();
		}
		break;
	case VM_SERVICE:
		{
			iValue=GetPrevVLimitPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}


WORD CDataHandler::GetPrevVLimitPara()
{
	//problems for VL=0 - still not good, handle the other modes too
	WORD iVal=0;
	switch(getModel()->getCONFIG()->GetPrevMode())
	{
	case VM_IPPV:
	case VM_PRE_IPPV:
		{
			iVal = PARADATA()->GetVLimitParam_IPPV();
		}
		break;
	//case VM_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PSV: 
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			iVal = PARADATA()->GetVLimitParam_TRIGGER();
		}
		break;
	//case VM_PRE_IPPV:
	//case VM_PRE_PSV:
	//case VM_PRE_SIMV:
	//case VM_PRE_SIMVPSV:
	//case VM_PRE_SIPPV:
	//case VM_PRE_CPAP:
	//case VM_PRE_DUOPAP:
	//case VM_PRE_NCPAP:
	//case VM_PRE_HFO:
	//case VM_PRE_THERAPIE:
	//	{
	//		iVal = 0;
	//	}
	//	break;
	default:
		{
			iVal = PARADATA()->GetVLimitParam_TRIGGER();
		}
		break;
	}

	return iVal;
}




void CDataHandler::SaveLastVLimitParam()
{
	if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
		m_iLastVLimitParam=PARADATA()->GetVLimitParam_IPPV();
	else
		m_iLastVLimitParam=PARADATA()->GetVLimitParam_TRIGGER();
}
WORD CDataHandler::GetLastVLimitParam()
{
	return m_iLastVLimitParam;
}



//***************************************************************************
//     
//**************************************************************************
bool CDataHandler::IsActiveModeVGarantStateOn()
{
	bool bResult=false;
	//bool bTest=false;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			bResult=PARADATA()->IsVGarantStateOn_IPPV();
			/*if(bResult)
				DEBUGMSG(TRUE, (TEXT("IsActiveModeVGarantStateOn true\r\n")));
			else
				DEBUGMSG(TRUE, (TEXT("IsActiveModeVGarantStateOn false\r\n")));
			bTest=true;*/
		}
		break;
	case VM_SERVICE:
	case VM_SIMV:
	case VM_SIPPV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			bResult=PARADATA()->IsVGarantStateOn_TRIGGER();
		}
		break;
	case VM_HFO:
		{
			bResult=PARADATA()->IsHFVGarantStateOn();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_HFO:
		{
			bResult=IsPrevVGarantStateOn();
		}
		break;
	default:
		{
		}
		break;
	}
	/*if(bTest==false)
	{
		DEBUGMSG(TRUE, (TEXT("IsActiveModeVGarantStateOn() %d\r\n"),(int)eMode));
	}*/
	return bResult;

	
}

//***************************************************************************
//     
//**************************************************************************
bool CDataHandler::IsCurrentModeVGarantStateOn()
{
	bool bResult=false;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			bResult=PARADATA()->IsVGarantStateOn_IPPV();
		}
		break;
	case VM_SERVICE:
	case VM_SIMV:
	case VM_SIPPV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			bResult=PARADATA()->IsVGarantStateOn_TRIGGER();
		}
		break;
	case VM_HFO:
		{
			bResult=PARADATA()->IsHFVGarantStateOn();
		}
		break;
	case VM_PRE_IPPV:
		{
			bResult=PRESET()->IsVGarantStateOn_IPPV();
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
		{
			bResult=PRESET()->IsVGarantStateOn_TRIGGER();
		}
		break;
	case VM_PRE_HFO:
		{
			bResult=PRESET()->IsHFVGarantStateOn();
		}
		break;
	default:
		{
		}
		break;
	}

	return bResult;

	
}

//***************************************************************************
//     
//**************************************************************************
bool CDataHandler::IsPrevVGarantStateOn()
{
	bool bResult=false;

	switch(getModel()->getCONFIG()->GetPrevMode())
	{
	case VM_IPPV:
		{
			bResult=PARADATA()->IsVGarantStateOn_IPPV();
		}
		break;
	case VM_SIMV:
	case VM_SIPPV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			bResult=PARADATA()->IsVGarantStateOn_TRIGGER();
		}
		break;
	case VM_HFO:
		{
			bResult=PARADATA()->IsHFVGarantStateOn();
		}
		break;
	default:
		{
			bResult=false;
		}
		break;
	}

	return bResult;
}




//***************************************************************************
//     
//**************************************************************************
bool CDataHandler::IsCurrentModeVLimitStateOn()
{
	bool bResult=false;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			bResult=PARADATA()->IsVLimitParamOn_IPPV();
		}
		break;
	case VM_SERVICE:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			bResult=PARADATA()->IsVLimitParamOn_TRIGGER();
		}
		break;
	case VM_PRE_IPPV:
		{
			bResult=PRESET()->IsVLimitParamOn_IPPV();
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
		{
			bResult=PRESET()->IsVLimitParamOn_TRIGGER();
		}
		break;
	default:
		{
		}
		break;
	}
	
	
	return bResult;
}

//***************************************************************************
//     
//**************************************************************************
bool CDataHandler::IsActiveModeVLimitStateOn()
{
	bool bResult=false;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			bResult=PARADATA()->IsVLimitParamOn_IPPV();
		}
		break;
	case VM_SERVICE:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			bResult=PARADATA()->IsVLimitParamOn_TRIGGER();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
		{
			bResult=IsPrevVLimitStateOn();
		}
		break;
	default:
		{
		}
		break;
	}
	
	
	return bResult;
}
//***************************************************************************
//     
//**************************************************************************
bool CDataHandler::IsPrevVLimitStateOn()
{
	bool bResult=false;

	switch(getModel()->getCONFIG()->GetPrevMode())
	{
	case VM_IPPV:
		{
			bResult=PARADATA()->IsVLimitParamOn_IPPV();
		}
		break;
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			bResult=PARADATA()->IsVLimitParamOn_TRIGGER();
		}
		break;
	default:
		{
		}
		break;
	}

	return bResult;
}

//***************************************************************************
//     
//**************************************************************************
SHORT CDataHandler::GetCurrentPManualMaxKey()
{
	SHORT iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_CPAP:
	case VM_HFO:
		{
			iValue=PARADATA()->GetPManualMaxKey();
		}
		break;
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_THERAPIE:
		{
			iValue=PARADATA()->GetPManualNMODEMaxKey();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_HFO:
		{
			iValue=PRESET()->GetPManualMaxKey();
		}
		break;
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_THERAPIE:
		{
			iValue=PRESET()->GetPManualNMODEMaxKey();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
SHORT CDataHandler::GetCurrentTherapieFlowMaxKey()
{
	int iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_THERAPIE:
		{
			iValue=PARADATA()->GetTherapieFlowMaxKey();
		}
		break;
	case VM_PRE_THERAPIE:
		{
			iValue=PRESET()->GetTherapieFlowMaxKey();
		}
		break;
	case VM_IPPV:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_CPAP:
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	default:
		{
		}
		break;
	}

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
SHORT CDataHandler::GetCurrentCPAPMaxKey()
{
	SHORT iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_CPAP:
	case VM_HFO:
		{
			iValue=PARADATA()->GetCPAPMaxKey();
		}
		break;
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_THERAPIE:
		{
			iValue=PARADATA()->GetCPAPNMODEMaxKey();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_HFO:
		{
			iValue=PRESET()->GetCPAPMaxKey();
		}
		break;
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_THERAPIE:
		{
			iValue=PRESET()->GetCPAPNMODEMaxKey();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
//SHORT CDataHandler::GetCurrentHFampMaxKey()
//{
//	SHORT iValue=0;
//
//	switch(getModel()->getCONFIG()->GetCurMode())
//	{
//	case VM_HFO:
//		{
//			iValue=PARADATA()->GetHFampMaxKey();
//		}
//		break;
//	case VM_PRE_HFO:
//		{
//			iValue=PRESET()->GetHFampMaxKey();
//		}
//		break;
//	default:
//		{
//		}
//		break;
//	}
//
//	return iValue;
//}
//***************************************************************************
//     
//**************************************************************************
SHORT CDataHandler::GetCurrentPInspMaxKey()
{
	SHORT iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_CPAP:
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_HFO:
	case VM_THERAPIE:
		{
			iValue=PARADATA()->GetPInspMaxKey();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
	case VM_PRE_THERAPIE:
		{
			iValue=PRESET()->GetPInspMaxKey();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}

//***************************************************************************
//     
//**************************************************************************
WORD CDataHandler::GetCurrentIFlowMaxKey()
{
	WORD iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_CPAP:
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_HFO:
	case VM_THERAPIE:
		{
			iValue=PARADATA()->GetIFlowMaxKey();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
	case VM_PRE_THERAPIE:
		{
			iValue=PRESET()->GetIFlowMaxKey();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
WORD CDataHandler::GetCurrentEFlowMaxKey()
{
	WORD iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_CPAP:
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_HFO:
	case VM_THERAPIE:
		{
			iValue=PARADATA()->GetEFlowMaxKey();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
	case VM_PRE_THERAPIE:
		{
			iValue=PRESET()->GetEFlowMaxKey();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
WORD CDataHandler::GetCurrentVlimitMaxKey()
{
	WORD iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			iValue=PARADATA()->GetVlimitMaxKey_IPPV();
		}
		break;
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_CPAP:
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_HFO:
	case VM_THERAPIE:
		{
			iValue=PARADATA()->GetVlimitMaxKey_TRIGGER();
		}
		break;
	case VM_PRE_IPPV:
		{
			iValue=PRESET()->GetVlimitMaxKey_IPPV();
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
	case VM_PRE_THERAPIE:
		{
			iValue=PRESET()->GetVlimitMaxKey_TRIGGER();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
WORD CDataHandler::GetCurrentVgarantMaxKey()
{
	WORD iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			iValue=PARADATA()->GetVgarantMaxKey_IPPV();
		}
		break;
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_CPAP:
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_THERAPIE:
		{
			iValue=PARADATA()->GetVgarantMaxKey_TRIGGER();
		}
		break;
	case VM_HFO:
		{
			iValue=PARADATA()->GetHFVgarantMaxKey();
		}
		break;
	case VM_PRE_IPPV:
		{
			iValue=PRESET()->GetVgarantMaxKey_IPPV();
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_THERAPIE:
		{
			iValue=PRESET()->GetVgarantMaxKey_TRIGGER();
		}
		break;
	case VM_PRE_HFO:
		{
			iValue=PRESET()->GetHFVgarantMaxKey();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}
//***************************************************************************
//     
//**************************************************************************
SHORT CDataHandler::GetCurrentPpsvMaxKey()
{
	SHORT iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_PSV:
	case VM_SIMVPSV:
		{
			iValue=PARADATA()->GetPpsvMaxKey();
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMVPSV:
		{
			iValue=PRESET()->GetPpsvMaxKey();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}

//***************************************************************************
//     
//**************************************************************************
SHORT CDataHandler::GetCurrentPeepMaxKey()
{
	SHORT iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_HFO:
	case VM_CPAP:
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_THERAPIE:
		{
			iValue=PARADATA()->GetPEEPMaxKey();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_HFO:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_THERAPIE:
		{
			iValue=PRESET()->GetPEEPMaxKey();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}

//***************************************************************************
//     
//**************************************************************************
SHORT CDataHandler::GetCurrentPeepMinKey()
{
	SHORT iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_CPAP:
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_HFO:
	case VM_THERAPIE:
		{
			iValue=PARADATA()->GetPEEPMinKey();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
	case VM_PRE_THERAPIE:
		{
			iValue=PRESET()->GetPEEPMinKey();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}

//***************************************************************************
//     
//**************************************************************************
CStringW CDataHandler::GetCurrentModeString()
{
	CStringW sz = _T("");
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			sz = getModel()->GetLanguageString(IDS_IPPV);
		}
		break;
	case VM_PSV: 
		{
			sz = getModel()->GetLanguageString(IDS_PSV);
		}
		break;
	case VM_SIMV:
		{
			sz = getModel()->GetLanguageString(IDS_SIMV);
		}
		break;
	case VM_SIMVPSV:
		{
			sz = getModel()->GetLanguageString(IDS_SIMVPSV);
		}
		break;
	case VM_SIPPV:
		{
			sz = getModel()->GetLanguageString(IDS_SIPPV);
		}
		break;
	case VM_HFO:
		{
			sz = getModel()->GetLanguageString(IDS_HFO);
		}
		break;
	case VM_CPAP:
		{
			sz = getModel()->GetLanguageString(IDS_CPAP);
		}
		break;
	case VM_NCPAP:
		{
			sz = getModel()->GetLanguageString(IDS_NCPAP);
		}
		break;
	case VM_THERAPIE:
		{
			sz = getModel()->GetLanguageString(IDS_O2THERAPIE);
		}
		break;
	case VM_DUOPAP:
		{
			if(getModel()->getDATAHANDLER()->PARADATA()->GetTriggerPara_DUOPAP()!=MAXRANGE_TRIGGER_OFF)
			{
				sz = _T("S-");
				sz += getModel()->GetLanguageString(IDS_DUOPAP);
			}
			else
			{
				sz = getModel()->GetLanguageString(IDS_DUOPAP);
			}
		}
		break;
	case VM_PRE_IPPV:
		{
			sz = getModel()->GetLanguageString(IDS_IPPV_PRE);
		}
		break;
	case VM_PRE_PSV:
		{
			sz = getModel()->GetLanguageString(IDS_PSV_PRE);
		}
		break;
	case VM_PRE_SIMV:
		{
			sz = getModel()->GetLanguageString(IDS_SIMV_PRE);
		}
		break;
	case VM_PRE_SIMVPSV:
		{
			sz = getModel()->GetLanguageString(IDS_SIMVPSV_PRE);
		}
		break;
	case VM_PRE_SIPPV:
		{
			sz = getModel()->GetLanguageString(IDS_SIPPV_PRE);
		}
		break;
	case VM_PRE_CPAP:
		{
			sz = getModel()->GetLanguageString(IDS_CPAP_PRE);
		}
		break;
	case VM_PRE_DUOPAP:
		{
			if(getModel()->getDATAHANDLER()->PRESET()->GetTriggerPara_DUOPAP()!=MAXRANGE_TRIGGER_OFF)
			{
				sz = _T("S-");
				sz += getModel()->GetLanguageString(IDS_DUOPAP_PRE);
			}
			else
			{
				sz = getModel()->GetLanguageString(IDS_DUOPAP_PRE);
			}
		}
		break;
	case VM_PRE_NCPAP:
		{
			sz = getModel()->GetLanguageString(IDS_NCPAP_PRE);
		}
		break;
	case VM_PRE_HFO:
		{
			sz = getModel()->GetLanguageString(IDS_HFO_PRE);
		}
		break;
	case VM_PRE_THERAPIE:
		{
			sz = getModel()->GetLanguageString(IDS_THERAPIE_PRE);
		}
		break;
	case VM_SERVICE:
		{
			sz = getModel()->GetLanguageString(IDS_SERVICE);
		}
		break;
	default:
		{
			sz = GetPrevModeString();
		}
		break;
	}

	return sz;
}

CStringW CDataHandler::GetPrevModeString()
{
	CStringW sz = _T("");
	switch(getModel()->getCONFIG()->GetPrevMode())
	{
	case VM_IPPV:
		{
			sz = getModel()->GetLanguageString(IDS_IPPV);
		}
		break;
	case VM_PSV: 
		{
			sz = getModel()->GetLanguageString(IDS_PSV);
		}
		break;
	case VM_SIMV:
		{
			sz = getModel()->GetLanguageString(IDS_SIMV);
		}
		break;
	case VM_SIMVPSV:
		{
			sz = getModel()->GetLanguageString(IDS_SIMVPSV);
		}
		break;
	case VM_SIPPV:
		{
			sz = getModel()->GetLanguageString(IDS_SIPPV);
		}
		break;
	case VM_HFO:
		{
			sz = getModel()->GetLanguageString(IDS_HFO);
		}
		break;
	case VM_CPAP:
		{
			sz = getModel()->GetLanguageString(IDS_CPAP);
		}
		break;
	case VM_NCPAP:
		{
			sz = getModel()->GetLanguageString(IDS_NCPAP);
		}
		break;
	case VM_THERAPIE:
		{
			sz = getModel()->GetLanguageString(IDS_O2THERAPIE);
		}
		break;
	case VM_DUOPAP:
		{
			sz = getModel()->GetLanguageString(IDS_DUOPAP);
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
	case VM_PRE_THERAPIE:
	default:
		{
		}
		break;
	}

	return sz;
}

/**********************************************************************************************//**
 * @fn	void CDataHandler::SetCurrentTriggerPara(BYTE val,bool bSetAutoEnable)
 *
 * @brief	Sets current trigger para.
 *
 * @author	Rainer Kuehner
 * @date	12.01.2017
 *
 * @param	val			  	The value.
 * @param	bSetAutoEnable	true if set automatic enable.
 **************************************************************************************************/
void CDataHandler::SetCurrentTriggerPara(BYTE val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			if(PARADATA()->GetTriggerPara_CONV()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetTriggerCONVParadata(val,true);
		}
		break;
	case VM_CPAP:
		{
			if(PARADATA()->GetTriggerPara_CPAP()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetTriggerCPAPParadata(val,true);
		}
		break;
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_PSV:
		{
			PRESET()->SetTriggerPara_CONV(val,false,false);
		}
		break;
	case VM_PRE_CPAP:
		{
			PRESET()->SetTriggerPara_CPAP(val,false,false);
		}
		break;
	case VM_DUOPAP:
		{
			BYTE iOldVal=PARADATA()->GetTriggerPara_DUOPAP();
			if(iOldVal!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}

			if(iOldVal==MAXRANGE_TRIGGER_OFF && val !=MAXRANGE_TRIGGER_OFF)//MAXRANGE_PED_TRIGGER_NMODE==0ff
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_FLOWSENSORSTATE);
			}
			else if(iOldVal!=MAXRANGE_TRIGGER_OFF && val ==MAXRANGE_TRIGGER_OFF)
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_FLOWSENSORSTATE);
			}
			SetTriggerDUOPAPParadata(val,true);

			if(GetDUOPAPtriggerAutoenableFlag())
			{
				ResetDUOPAPtriggerAutoenableFlag();
			}

			if(val==MAXRANGE_TRIGGER_OFF)
				checkTriggerTubeDependency();
		}
		break;
	case VM_NCPAP:
		{
			BYTE iOldVal=PARADATA()->GetTriggerPara_NCPAP();
			if(iOldVal!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}

			if(iOldVal==MAXRANGE_TRIGGER_OFF && val !=MAXRANGE_TRIGGER_OFF)//MAXRANGE_PED_TRIGGER_NMODE==0ff
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_FLOWSENSORSTATE);
			}
			else if(iOldVal!=MAXRANGE_TRIGGER_OFF && val ==MAXRANGE_TRIGGER_OFF)
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_FLOWSENSORSTATE);
			}
			SetTriggerNCPAPParadata(val,true);
			
			if(val==MAXRANGE_TRIGGER_OFF)
				checkTriggerTubeDependency();
		}
		break;
	case VM_PRE_DUOPAP:
		{
			BYTE iOldVal=PRESET()->GetTriggerPara_DUOPAP();
			if(iOldVal==MAXRANGE_TRIGGER_OFF && val !=MAXRANGE_TRIGGER_OFF)//MAXRANGE_PED_TRIGGER_NMODE==0ff
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_FLOWSENSORSTATE);
			}
			else if(iOldVal!=MAXRANGE_TRIGGER_OFF && val ==MAXRANGE_TRIGGER_OFF)
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_FLOWSENSORSTATE);
			}
			PRESET()->SetTriggerPara_DUOPAP(val,false,false);
		}
		break;
	case VM_PRE_NCPAP:
		{
			BYTE iOldVal=PRESET()->GetTriggerPara_NCPAP();
			if(iOldVal==MAXRANGE_TRIGGER_OFF && val !=MAXRANGE_TRIGGER_OFF)//MAXRANGE_PED_TRIGGER_NMODE==0ff
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_FLOWSENSORSTATE);
			}
			else if(iOldVal!=MAXRANGE_TRIGGER_OFF && val ==MAXRANGE_TRIGGER_OFF)
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_FLOWSENSORSTATE);
			}
			PRESET()->SetTriggerPara_NCPAP(val,false,false);
		}
		break;
	default:
		break;
	}

	
}

/**********************************************************************************************//**
 * @fn	BYTE CDataHandler::GetCurrentTriggerPara()
 *
 * @brief	Gets current trigger para.
 *
 * @author	Rainer Kuehner
 * @date	12.01.2017
 *
 * @return	The current trigger para.
 **************************************************************************************************/
BYTE CDataHandler::GetCurrentTriggerPara()
{
	BYTE iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			iValue=PARADATA()->GetTriggerPara_CONV();
		}
		break;
	case VM_CPAP:
		{
			iValue=PARADATA()->GetTriggerPara_CPAP();
		}
		break;
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_PSV:
		{
			iValue=PRESET()->GetTriggerPara_CONV();
		}
		break;
	case VM_PRE_CPAP:
		{
			iValue=PRESET()->GetTriggerPara_CPAP();
		}
		break;
	case VM_DUOPAP:
		{
			iValue=PARADATA()->GetTriggerPara_DUOPAP();
		}
		break;
	case VM_NCPAP:
		{
			iValue=PARADATA()->GetTriggerPara_NCPAP();
		}
		break;
	case VM_PRE_DUOPAP:
		{
			iValue=PRESET()->GetTriggerPara_DUOPAP();
		}
		break;
	case VM_PRE_NCPAP:
		{
			iValue=PRESET()->GetTriggerPara_NCPAP();
		}
		break;
	default:
		break;
	}

	return iValue;
}

void CDataHandler::SetDUOPAPtriggerAutoenableFlag(BYTE iPrevTRIGGERDUOPAPPara)
{
	m_bDUOPAPtriggerAutoEnable=true;
	m_iPrevTRIGGERDUOPAPPara=iPrevTRIGGERDUOPAPPara;
}

BOOL CDataHandler::GetDUOPAPtriggerAutoenableFlag()
{
	return m_bDUOPAPtriggerAutoEnable;
}
WORD CDataHandler::GetPrevTRIGGERDUOPAPPara()
{
	return m_iPrevTRIGGERDUOPAPPara;
}

void CDataHandler::ResetDUOPAPtriggerAutoenableFlag()
{
	m_bDUOPAPtriggerAutoEnable=false;
	m_iPrevTRIGGERDUOPAPPara=0;
}

//#############
//void CDataHandler::SetNCPAPtriggerAutoenableFlag(BYTE iPrevTRIGGERNCPAPPara)
//{
//	m_bNCPAPtriggerAutoEnable=true;
//	m_iPrevTRIGGERNCPAPPara=iPrevTRIGGERNCPAPPara;
//}
//
//BOOL CDataHandler::GetNCPAPtriggerAutoenableFlag()
//{
//	return m_bNCPAPtriggerAutoEnable;
//}
//WORD CDataHandler::GetPrevTRIGGERNCPAPPara()
//{
//	return m_iPrevTRIGGERNCPAPPara;
//}
//
//void CDataHandler::ResetNCPAPtriggerAutoenableFlag()
//{
//	m_bNCPAPtriggerAutoEnable=false;
//	m_iPrevTRIGGERNCPAPPara=0;
//}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentITimePara(WORD val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			if(PARADATA()->GetITimePara_IPPV()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetITimeParadata_IPPV(val,true);
			SetETIMEParadata_IPPV(PARADATA()->GetETIMEPara_IPPV(),true);

			if(getModel()->getAcuLink()!=NULL)
				CalculateIERatioParts();
		}
		break;
	case VM_SERVICE:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			if(PARADATA()->GetITimePara_TRIGGER()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetITimeParadata_TRIGGER(val,true);
			SetETIMEParadata_TRIGGER(PARADATA()->GetETIMEPara_TRIGGER(),true);

			if(getModel()->getAcuLink()!=NULL)
				CalculateIERatioParts();
		}
		break;
	case VM_DUOPAP:
		{
			if(PARADATA()->GetITimeNMODEPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetITimeNMODEParadata(val,true);
			SetETIMENMODEParadata(PARADATA()->GetETIMENMODEPara(),true);

			if(getModel()->getAcuLink()!=NULL)
				CalculateIERatioParts();

			checkTriggerTubeDependency();
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
		{
			PRESET()->SetITimePara_TRIGGER(val,false,false);
			PRESET()->SetETIMEPara_TRIGGER(PRESET()->GetETIMEPara_TRIGGER(),false,false);
		}
		break;
	case VM_PRE_IPPV:
		{
			PRESET()->SetITimePara_IPPV(val,false,false);
			PRESET()->SetETIMEPara_IPPV(PRESET()->GetETIMEPara_IPPV(),false,false);
		}
		break;
	case VM_PRE_DUOPAP:
		{
			PRESET()->SetITimeNMODEPara(val,false,false);
			PRESET()->SetETIMENMODEPara(PRESET()->GetETIMENMODEPara(),false,false);

			checkTriggerTubeDependency();
		}
		break;
	default:
		break;
	}
}
WORD CDataHandler::GetCurrentITimePara()
{
	WORD fValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			fValue=PARADATA()->GetITimePara_IPPV();
		}
		break;
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			fValue=PARADATA()->GetITimePara_TRIGGER();
		}
		break;
	case VM_DUOPAP:
		{
			fValue=PARADATA()->GetITimeNMODEPara();
		}
		break;
	case VM_PRE_IPPV:
		{
			fValue=PRESET()->GetITimePara_IPPV();
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
		{
			fValue=PRESET()->GetITimePara_TRIGGER();
		}
		break;
	case VM_PRE_DUOPAP:
		{
			fValue=PRESET()->GetITimeNMODEPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return fValue;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::CalculateIERatioParts()
{
	int iTempIErelation =GetCurrentIERelation100();
	if(iTempIErelation<100)
	{
		iTempIErelation = GetCurrentEIRelation100();
		if(iTempIErelation<110)
		{
			SetIERatioIpart(11);
			SetIERatioEpart(10);
			//wsprintf(psz,_T("1.1 : 1"));
		}
		else
		{
			SetIERatioIpart(CTlsFloat::Round((double)iTempIErelation/100, 1)*10);
			SetIERatioEpart(10);
			//wsprintf(psz,_T("%0.1f : 1"),CTlsFloat::Round((double)iTempIErelation/100, 1));
		}
	}
	else if(iTempIErelation==100)
	{
		SetIERatioIpart(10);
		SetIERatioEpart(CTlsFloat::Round((double)iTempIErelation/100, 1)*10);
		//wsprintf(psz,_T("1 : %0.0f"),CTlsFloat::Round((double)iTempIErelation/100, 1));
	}
	/*else if(iTempIErelation>1000)
	{
		wsprintf(psz,_T("< 1 : 10"));
	}*/
	else
	{
		SetIERatioIpart(10);
		SetIERatioEpart(CTlsFloat::Round((double)iTempIErelation/100, 1)*10);
		//wsprintf(psz,_T("1 : %0.1f"),CTlsFloat::Round((double)iTempIErelation/100, 1));
	}

	getModel()->getAcuLink()->setParaData(ALINK_SETT_IERATIO_IPART,GetIERatioIpart());
	getModel()->getAcuLink()->setParaData(ALINK_SETT_IERATIO_EPART,GetIERatioEpart());
}
int CDataHandler::GetIERatioIpart()
{
	return m_iIERatioIpart;
}
void CDataHandler::SetIERatioIpart(int iVal)
{
	m_iIERatioIpart=iVal;
}
int CDataHandler::GetIERatioEpart()
{
	return 	m_iIERatioEpart;
}
void CDataHandler::SetIERatioEpart(int iVal)
{
	m_iIERatioEpart=iVal;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentETimePara(WORD val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			if(PARADATA()->GetETIMEPara_IPPV()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetETIMEParadata_IPPV(val,true);

			if(getModel()->getAcuLink()!=NULL)
				CalculateIERatioParts();
		}
		break;
	case VM_SERVICE:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			if(PARADATA()->GetETIMEPara_TRIGGER()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetETIMEParadata_TRIGGER(val,true);

			if(getModel()->getAcuLink()!=NULL)
				CalculateIERatioParts();
		}
		break;
	case VM_DUOPAP:
		{
			if(PARADATA()->GetETIMENMODEPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetETIMENMODEParadata(val,true);

			if(getModel()->getAcuLink()!=NULL)
				CalculateIERatioParts();
		}
		break;
	case VM_PRE_IPPV:
		{
			PRESET()->SetETIMEPara_IPPV(val,false, false);
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
		{
			PRESET()->SetETIMEPara_TRIGGER(val,false, false);
		}
		break;
	case VM_PRE_DUOPAP:
		{
			PRESET()->SetETIMENMODEPara(val,false, false);
		}
		break;
	default:
		break;
	}
}
WORD CDataHandler::GetCurrentETimePara()
{
	WORD fValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			fValue=PARADATA()->GetETIMEPara_IPPV();
		}
		break;
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_CPAP:
	case VM_HFO:
		{
			fValue=PARADATA()->GetETIMEPara_TRIGGER();
		}
		break;
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_THERAPIE:
		{
			fValue=PARADATA()->GetETIMENMODEPara();
		}
		break;
	case VM_PRE_IPPV:
		{
			fValue=PRESET()->GetETIMEPara_IPPV();
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_HFO:
		{
			fValue=PRESET()->GetETIMEPara_TRIGGER();
		}
		break;
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_THERAPIE:
		{
			fValue=PRESET()->GetETIMENMODEPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return fValue;
}



// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentBPMPara(WORD val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			if(PARADATA()->GetBPMPara_IPPV()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetBPMParadata_IPPV(val,true);
			SetETIMEParadata_IPPV(PARADATA()->GetETIMEPara_IPPV(),true);

			if(getModel()->getAcuLink()!=NULL)
				CalculateIERatioParts();
		}
		break;
	case VM_SERVICE:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			if(PARADATA()->GetBPMPara_TRIGGER()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetBPMParadata_TRIGGER(val,true);
			SetETIMEParadata_TRIGGER(PARADATA()->GetETIMEPara_TRIGGER(),true);

			if(getModel()->getAcuLink()!=NULL)
				CalculateIERatioParts();
		}
		break;
	case VM_DUOPAP:
		{
			if(PARADATA()->GetBPMNMODEPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetBPMNMODEParadata(val,true);
			SetETIMENMODEParadata(PARADATA()->GetETIMENMODEPara(),true);

			if(getModel()->getAcuLink()!=NULL)
				CalculateIERatioParts();
		}
		break;
	case VM_PRE_IPPV:
		{
			PRESET()->SetBPMPara_IPPV(val,false,false);
			PRESET()->SetETIMEPara_IPPV(PRESET()->GetETIMEPara_IPPV(),false,false);
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
		{
			PRESET()->SetBPMPara_TRIGGER(val,false,false);
			PRESET()->SetETIMEPara_TRIGGER(PRESET()->GetETIMEPara_TRIGGER(),false,false);
		}
		break;
	case VM_PRE_DUOPAP:
		{
			PRESET()->SetBPMNMODEPara(val,false,false);
			PRESET()->SetETIMENMODEPara(PRESET()->GetETIMENMODEPara(),false,false);
		}
		break;
	default:
		break;
	}
}
WORD CDataHandler::GetCurrentBPMPara()
{
	WORD iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			iValue=PARADATA()->GetBPMPara_IPPV();
		}
		break;
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_CPAP:
	case VM_HFO:
		{
			iValue=PARADATA()->GetBPMPara_TRIGGER();
		}
		break;
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_THERAPIE:
		{
			iValue=PARADATA()->GetBPMNMODEPara();
		}
		break;
	case VM_PRE_IPPV:
		{
			iValue=PRESET()->GetBPMPara_IPPV();
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_HFO:
		{
			iValue=PRESET()->GetBPMPara_TRIGGER();
		}
		break;
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_THERAPIE:
		{
			iValue=PRESET()->GetBPMNMODEPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentO2Para(BYTE val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_SERVICE:
	case VM_IPPV:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_CPAP:
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_HFO:
	case VM_THERAPIE:
		{
			//if(PARADATA()->GetO2Para()!=val)
			//{
			//	//new paradata
			//	//PARADATA()->SetO2Para(val,true);
			//	SetO2Paradata(val,true);
			//}

			//PRICO04
			bool bPRICOrunning=false;
			if(getModel()->getPRICOThread())
			{
				if(getModel()->getPRICOThread()->isPRICOalgorithmRunning())
				{
					bPRICOrunning=true;
				}
			}
			if(false==bPRICOrunning) 
			{
				setO2FlushDiff(val);
				SetO2Paradata(val,true);
			}
			else 
			{
				setO2FlushDiff(val);
				SetO2Paradata(val,false);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TXT_PRICO_RUNNING);
			}
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
	case VM_PRE_THERAPIE:
		{
			setO2FlushDiff(val); //PRICO04
			PRESET()->SetO2Para(val,false,false);
		}
		break;
	default:
		break;
	}
}
BYTE CDataHandler::GetCurrentO2Para()
{
	BYTE fValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_CPAP:
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_HFO:
	case VM_THERAPIE:
		{
			fValue=PARADATA()->GetO2Para();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
	case VM_PRE_THERAPIE:
		{
			fValue=PRESET()->GetO2Para();
		}
		break;
	default:
		{
		}
		break;
	}

	return fValue;
}
// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentO2FlushPara(BYTE val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_CPAP:
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_HFO:
	case VM_THERAPIE:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_O2FLUSH_CHANGED);
			//new paradata
			//PARADATA()->SetO2FlushPara(val);
			SetO2FlushParadata(val);
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
	case VM_PRE_THERAPIE:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_O2FLUSH_CHANGED);
			PRESET()->SetO2FlushPara(val,false);
		}
		break;
	default:
		break;
	}
}
BYTE CDataHandler::GetCurrentO2FlushPara()
{
	int iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_CPAP:
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_HFO:
	case VM_THERAPIE:
		{
			iValue=PARADATA()->GetO2FlushPara();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
	case VM_PRE_THERAPIE:
		{
			iValue=PRESET()->GetO2FlushPara();
		}
		break;
	default:
		{
		}
		break;
	}


	return iValue;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentRisetimePara(WORD val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			if(PARADATA()->GetRisetimePara_IPPV()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetRisetimeParadata_IPPV(val,true);
		}
		break;
	case VM_SERVICE:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			if(PARADATA()->GetRisetimePara_TRIGGER()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetRisetimeParadata_TRIGGER(val,true);
		}
		break;
	case VM_PRE_IPPV:
		{
			PRESET()->SetRisetimePara_IPPV(val,false,false);
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
		{
			PRESET()->SetRisetimePara_TRIGGER(val,false,false);
		}
		break;
	case VM_CPAP:
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_HFO:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
	default:
		break;
	}
}
WORD CDataHandler::GetCurrentRisetimePara()
{
	WORD fValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			fValue=PARADATA()->GetRisetimePara_IPPV();
		}
		break;
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			fValue=PARADATA()->GetRisetimePara_TRIGGER();
		}
		break;
	case VM_PRE_IPPV:
		{
			fValue=PRESET()->GetRisetimePara_IPPV();
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
		{
			fValue=PRESET()->GetRisetimePara_TRIGGER();
		}
		break;
	case VM_CPAP:
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_HFO:
	default:
		{
		}
		break;
	}

	return fValue;
}
// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentHFFlowPara(WORD val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_SERVICE:
	case VM_HFO:
		{
			if(PARADATA()->GetHFFlowPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetHFFlowParadata(val,true);
		}
		break;
	case VM_PRE_HFO:
		{
			PRESET()->SetHFFlowPara(val,false,false);
		}
		break;
	default:
		break;
	}
}
WORD CDataHandler::GetCurrentHFFlowPara()
{
	WORD fValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			fValue=PARADATA()->GetHFFlowPara();
		}
		break;
	case VM_PRE_HFO:
		{
			fValue=PRESET()->GetHFFlowPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return fValue;
}
// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentIFlowPara(WORD val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			if(PARADATA()->GetIFlowPara_IPPV()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetIFlowParadata_IPPV(val,true);

			if(		getModel()->getCONFIG()->IsEFLOWequalILFOW() 
				&&	getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW
				&&	false==GetExhalValvCalMode())
			{
				SetEFlowParadata_IPPV(val,true);
			}
		}
		break;
	case VM_SERVICE:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			if(PARADATA()->GetIFlowPara_TRIGGER()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetIFlowParadata_TRIGGER(val,true);

			if(		getModel()->getCONFIG()->IsEFLOWequalILFOW() 
				&&	getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW
				&&	false==GetExhalValvCalMode())
			{
				SetEFlowParadata_TRIGGER(val,true);
			}
		}
		break;
	case VM_PRE_IPPV:
		{
			PRESET()->SetIFlowPara_IPPV(val,false,false);

			if(		getModel()->getCONFIG()->IsEFLOWequalILFOW() 
				&&	getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW
				&&	false==GetExhalValvCalMode())
			{
				PRESET()->SetEFLOWPara_IPPV(val,false,false);
			}
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
		{
			PRESET()->SetIFlowPara_TRIGGER(val,false,false);

			if(		getModel()->getCONFIG()->IsEFLOWequalILFOW() 
				&&	getModel()->getCONFIG()->GetCurPressureRiseCtrl()==CURVE_IFLOW
				&&	false==GetExhalValvCalMode())
			{
				PRESET()->SetEFLOWPara_TRIGGER(val,false,false);
			}
		}
		break;
	default:
		break;
	}
}

WORD CDataHandler::GetCurrentIFlowPara()
{
	WORD fValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			fValue=PARADATA()->GetIFlowPara_IPPV();
		}
		break;
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			fValue=PARADATA()->GetIFlowPara_TRIGGER();
		}
		break;
	case VM_PRE_IPPV:
		{
			fValue=PRESET()->GetIFlowPara_IPPV();
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
		{
			fValue=PRESET()->GetIFlowPara_TRIGGER();
		}
		break;
	default:
		{
		}
		break;
	}

	return fValue;
}
// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentEFlowPara(WORD val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
	case VM_SERVICE:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			if(PARADATA()->GetEFLOWPara_TRIGGER()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetEFlowParadata_TRIGGER(val,true);
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
		{
			PRESET()->SetEFLOWPara_TRIGGER(val,false,false);
		}
		break;
	case VM_CPAP://EFLOW1
	case VM_NCPAP://EFLOW1
	case VM_DUOPAP://EFLOW1
	case VM_HFO://EFLOW1
	case VM_THERAPIE://EFLOW1
		{
			SetEFlowParadata_TRIGGER(val,false);
		}
		break;
	default://EFLOW1
		{
			PRESET()->SetEFLOWPara_TRIGGER(val,false,false);
		}
		break;
	}
}
void CDataHandler::SetFlowMinParadata(WORD val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			if(PARADATA()->GetEFLOWPara_IPPV()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetEFlowParadata_IPPV(val,true);
		}
		break;
	case VM_SERVICE:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			if(PARADATA()->GetEFLOWPara_TRIGGER()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetEFlowParadata_TRIGGER(val,true);
		}
		break;
	case VM_CPAP:
		{
			if(PARADATA()->GetFlowminPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetFlowminParadata(val,true);
		}
		break;
	case VM_PRE_IPPV:
		{
			PRESET()->SetEFLOWPara_IPPV(val,false,false);
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
		{
			PRESET()->SetEFLOWPara_TRIGGER(val,false,false);
		}
		break;
	case VM_PRE_CPAP:
		{
			PRESET()->SetFlowminPara(val,false,false);
		}
		break;
	default:
		break;
	}
}

WORD CDataHandler::GetCurrentEFlowPara()
{
	WORD fValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			fValue=PARADATA()->GetEFLOWPara_TRIGGER();
		}
		break;
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
		{
			fValue=PRESET()->GetEFLOWPara_TRIGGER();
		}
		break;
	default:
		{
		}
		break;
	}


	return fValue;
}
WORD CDataHandler::GetFlowminParaData()
{
	WORD fValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_CPAP:
		{
			fValue=PARADATA()->GetFlowminPara();
		}
		break;
	case VM_PRE_CPAP:
		{
			fValue=PRESET()->GetFlowminPara();
		}
		break;
	/*case VM_THERAPIE:
		{
			fValue=PARADATA()->GetTherapieFLOWPara();
		}
		break;
	case VM_PRE_THERAPIE:
		{
			fValue=PRESET()->GetTherapieFLOWPara();
		}
		break;*/
	default:
		{
		}
		break;
	}


	return fValue;
}


// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentTherapieFlowParaData(WORD val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_THERAPIE:
		{
			if(PARADATA()->GetTherapieFLOWPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetTherapieFLOWParadata(val,true);
		}
		break;
	case VM_PRE_THERAPIE:
		{
			PRESET()->SetTherapieFLOWPara(val,false,false);
		}
		break;
	default:
		break;
	}
}

WORD CDataHandler::GetCurrentTherapieFlowParaData()
{
	WORD fValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_THERAPIE:
		{
			fValue=PARADATA()->GetTherapieFLOWPara();
		}
		break;
	case VM_PRE_THERAPIE:
		{
			fValue=PRESET()->GetTherapieFLOWPara();
		}
		break;
	default:
		{
		}
		break;
	}


	return fValue;
}



eVgarantState CDataHandler::GetVgarantState()
{
	return m_eCurVgarantState;
}

void CDataHandler::ChangeVgarantState(eVgarantState state)
{
	eVgarantState stateOld=m_eCurVgarantState;
	//DEBUGMSG(TRUE, (TEXT("ChangeVgarantState old %d\r\n"),(int)m_eCurVgarantState));
	//DEBUGMSG(TRUE, (TEXT("ChangeVgarantState new %d\r\n"),(int)state));
	m_eCurVgarantState=state;

	switch(state)
	{
	case VOLUMEGAR_ON:
		{
			DEBUGMSG(TRUE, (TEXT("CDataHandler::ChangeVgarantState VGAR_ON\r\n")));
			if(getModel()->getCONFIG()->CurModeIsPresetMode())
			{
				//
			}
			else
			{
				SetActiveModeVolumeGarantyState(true);

				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_VOLUMEGARANTY_ON);
			}
		}
		break;
	case VOLUMEGAR_OFF:
		{
			DEBUGMSG(TRUE, (TEXT("CDataHandler::ChangeVgarantState VGAR_OFF\r\n")));
			if(getModel()->getCONFIG()->CurModeIsPresetMode())
			{
				//
			}
			else
			{
				SetActiveModeVolumeGarantyState(false);

				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_VOLUMEGARANTY_OFF);
			}

		}
		break;
	case VOLUMEGAR_PRESET_ON:
		{
			DEBUGMSG(TRUE, (TEXT("CDataHandler::ChangeVgarantState VGAR_PRESET_ON\r\n")));
			SetPresetModeVolumeGarantyState(true);
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VOLUMEGARANTY_PRESET_ON);
		}
		break;
	case VOLUMEGAR_PRESET_OFF:
		{
			DEBUGMSG(TRUE, (TEXT("CDataHandler::ChangeVgarantState VGAR_PRESET_OFF\r\n")));
			SetPresetModeVolumeGarantyState(false);
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VOLUMEGARANTY_PRESET_OFF);
		}
		break;
	case VOLUMEGAR_PARA_ENABLED:
		{
			DEBUGMSG(TRUE, (TEXT("CDataHandler::ChangeVgarantState VGAR_PARA_ENABLED\r\n")));
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VOLUMEGARANTY_PARA_ENABLED);
		}
		break;
	case VOLUMEGAR_PARA_DISABLED:
		{
			DEBUGMSG(TRUE, (TEXT("CDataHandler::ChangeVgarantState VGAR_PARA_DISABLED\r\n")));
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VOLUMEGARANTY_PARA_DISABLED);
		}
		break;
	case VOLUMEGAR_PARA_AUTOENABLED:
		{
			DEBUGMSG(TRUE, (TEXT("CDataHandler::ChangeVgarantState VOLUMEGAR_PARA_AUTOENABLED\r\n")));
			SetActiveModeVolumeGarantyState(true);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VOLUMEGARANTY_PARA_AUTOENABLED);

			//m_eCurVgarantState=VOLUMEGAR_ON;
		}
		break;
	case VOLUMEGAR_PARA_AUTODISABLED:
		{
			DEBUGMSG(TRUE, (TEXT("CDataHandler::ChangeVgarantState VOLUMEGAR_PARA_AUTODISABLED\r\n")));
			SetActiveModeVolumeGarantyState(false);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VOLUMEGARANTY_PARA_AUTODISABLED);//autovgarant
		}
		break;
	default:
		{

		}
		break;
	}

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_VGARANT);

}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentPmaxVolGPara(SHORT val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			if(		PARADATA()->GetPmaxVolGPara_IPPV()!=val
				&&	PARADATA()->IsVGarantStateOn_IPPV()==true)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetPmaxVolGParadata_IPPV(val,true);//newVG

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_PMAXVOLG);
		}
		break;
	case VM_SERVICE:
	case VM_SIMV:
	case VM_SIPPV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			if(		PARADATA()->GetPmaxVolGPara_TRIGGER()!=val
				&&	PARADATA()->IsVGarantStateOn_TRIGGER()==true)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			
			SetPmaxVolGParadata_TRIGGER(val,true);//newVG

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_PMAXVOLG);
		}
		break;
	/*case VM_HFO:
		{
			if(		PARADATA()->GetHFAMPLMaxPara()!=val
				&&	PARADATA()->IsHFVGarantStateOn()==true)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}

			SetHFAMPLmaxParadata(val,PARADATA()->IsHFVGarantStateOn());

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_PMAXVOLG);
		}
		break;*/
	case VM_PRE_IPPV:
		{
			PRESET()->SetPmaxVolGPara_IPPV(val,false,false);
		}
		break;
	case VM_PRE_SIMV:
	case VM_PRE_SIPPV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_PSV:
		{
			PRESET()->SetPmaxVolGPara_TRIGGER(val,false,false);
		}
		break;
	/*case VM_PRE_HFO:
		{
			PRESET()->SetHFAMPLmaxPara(val,false,false);
		}
		break;*/
	default:
		break;
	}
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentPINSPPara(SHORT val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV://newVG
		{
			if(		PARADATA()->GetPINSPPara_IPPV()!=val
				&&	PARADATA()->IsVGarantStateOn_IPPV()==false)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetPINSPParadata_IPPV(val,true);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_PINSP);
		}
		break;
	case VM_SERVICE:
	case VM_SIMV:
	case VM_SIPPV:
	case VM_SIMVPSV:
	case VM_PSV://newVG
		{
			if(		PARADATA()->GetPINSPPara_TRIGGER()!=val
				&&	PARADATA()->IsVGarantStateOn_TRIGGER()==false)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			
			SetPINSPParadata_TRIGGER(val,true);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_PINSP);
		}
		break;
	case VM_HFO:
		{
			
		}
		break;
	case VM_PRE_IPPV:
		{
			PRESET()->SetPINSPPara_IPPV(val,false,false);
		}
		break;
	case VM_PRE_SIMV:
	case VM_PRE_SIPPV:
	case VM_PRE_HFO:
	case VM_PRE_SIMVPSV:
	case VM_PRE_PSV:
		{
			PRESET()->SetPINSPPara_TRIGGER(val,false,false);
		}
		break;
	default:
		break;
	}
}

//newVG
//SHORT CDataHandler::GetCurrentPIPPara()
//{
//	SHORT iValue=0;
//
//	eVentMode mode=getModel()->getCONFIG()->GetCurMode();
//
//	switch(mode)
//	{
//	case VM_IPPV:
//		{
//			if(IsCurrentModeVGarantStateOn())
//				iValue=PARADATA()->GetPmaxVolGPara_IPPV();
//			else
//				iValue=PARADATA()->GetPINSPPara_IPPV();
//		}
//		break;
//	case VM_SIMV:
//	case VM_SIPPV:
//	case VM_SIMVPSV:
//	//case VM_PSV:
//		{
//			if(IsCurrentModeVGarantStateOn())
//				iValue=PARADATA()->GetPmaxVolGPara_TRIGGER();
//			else
//				iValue=PARADATA()->GetPINSPPara_TRIGGER();
//		}
//		break;
//	case VM_PSV:
//		{
//			if(IsCurrentModeVGarantStateOn())
//				iValue=PARADATA()->GetPmaxVolGPara_TRIGGER();
//			else
//				iValue=PARADATA()->GetPpsvPara();
//		}
//		break;
//	case VM_HFO:
//		{
//			iValue=PARADATA()->GetPINSPPara_TRIGGER();
//		}
//		break;
//	case VM_CPAP:
//		{
//			iValue=PARADATA()->GetPManualPara();
//		}
//		break;
//	case VM_DUOPAP:
//	case VM_NCPAP:
//	case VM_THERAPIE:
//	case VM_PRE_IPPV:
//	case VM_PRE_SIMV:
//	case VM_PRE_SIPPV:
//	case VM_PRE_PSV:
//	case VM_PRE_SIMVPSV:
//	case VM_PRE_HFO:
//	case VM_PRE_DUOPAP:
//	case VM_PRE_NCPAP:
//	case VM_PRE_THERAPIE:
//	case VM_PRE_CPAP:
//	default:
//		{
//			iValue=GetCurrentPINSPPara();
//		}
//		break;
//	}
//
//	return iValue;
//}

SHORT CDataHandler::GetActiveModePRESSUREPara()//newVG
{
	if(IsActiveModeVGarantStateOn())
		return GetActiveModePMAXVGPara();
	else
		return GetActiveModePINSPPara();
}
SHORT CDataHandler::GetActiveModePINSPPara()//newVG
{
	SHORT iValue=0;

	eVentMode mode=VM_NONE;
	if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
		mode=getModel()->getCONFIG()->GetCurMode();
	else
		mode=getModel()->getCONFIG()->GetPrevMode();

	switch(mode)
	{
	case VM_IPPV:
		{	//newVG
			/*if(IsCurrentModeVGarantStateOn())
				iValue=PARADATA()->GetPmaxVolGPara_IPPV();
			else*/
				iValue=PARADATA()->GetPINSPPara_IPPV();
		}
		break;
	case VM_SIMV:
	case VM_SIPPV:
	case VM_SIMVPSV:
	case VM_PSV://newVG
		{
			/*if(IsCurrentModeVGarantStateOn())
				iValue=PARADATA()->GetPmaxVolGPara_TRIGGER();
			else*///newVG
				iValue=PARADATA()->GetPINSPPara_TRIGGER();
		}
		break;
	/*case VM_PSV:
		{
			if(IsCurrentModeVGarantStateOn())
				iValue=PARADATA()->GetPmaxVolGPara_TRIGGER();
			else
				iValue=PARADATA()->GetPpsvPara();
		}
		break;*///newVG
	case VM_HFO:
		{
			iValue=PARADATA()->GetPINSPPara_TRIGGER();
		}
		break;
	case VM_CPAP:
		{
			iValue=PARADATA()->GetPManualCPAPPara();
		}
		break;
	case VM_PRE_PSV:
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_THERAPIE:
	case VM_PRE_IPPV:
	case VM_PRE_SIMV:
	case VM_PRE_SIPPV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_HFO:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_THERAPIE:
	case VM_PRE_CPAP:
	default:
		{
			iValue=GetCurrentPINSPPara();
		}
		break;
	}

	return iValue;
}

SHORT CDataHandler::GetActiveModePMAXVGPara()//newVG
{
	SHORT iValue=0;

	eVentMode mode=VM_NONE;
	if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
		mode=getModel()->getCONFIG()->GetCurMode();
	else
		mode=getModel()->getCONFIG()->GetPrevMode();

	switch(mode)
	{
	case VM_IPPV:
		{	
			iValue=PARADATA()->GetPmaxVolGPara_IPPV();
		}
		break;
	case VM_SIMV:
	case VM_SIPPV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			iValue=PARADATA()->GetPmaxVolGPara_TRIGGER();
		}
		break;
	case VM_HFO:
		{
			iValue=PARADATA()->GetPINSPPara_TRIGGER();
		}
		break;
	case VM_CPAP:
		{
			iValue=PARADATA()->GetPManualCPAPPara();
		}
		break;
	case VM_PRE_PSV:
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_THERAPIE:
	case VM_PRE_IPPV:
	case VM_PRE_SIMV:
	case VM_PRE_SIPPV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_HFO:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_THERAPIE:
	case VM_PRE_CPAP:
	default:
		{
			iValue=GetCurrentPmaxVolGPara();
		}
		break;
	}

	return iValue;
}

SHORT CDataHandler::GetCurrentPmaxVolGPara()
{
	SHORT iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			iValue=PARADATA()->GetPmaxVolGPara_IPPV();
		}
		break;
	case VM_SIMV:
	case VM_SIPPV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			iValue=PARADATA()->GetPmaxVolGPara_TRIGGER();
		}
		break;
	case VM_HFO:
		{
			
		}
		break;
	case VM_CPAP:
		{
			
		}
		break;
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_THERAPIE:
		{
			
		}
		break;
	case VM_PRE_IPPV:
		{
			iValue=PRESET()->GetPmaxVolGPara_IPPV();
		}
		break;
	case VM_PRE_SIMV:
	case VM_PRE_SIPPV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_PSV:
		{
			iValue=PRESET()->GetPmaxVolGPara_TRIGGER();
		}
		break;
	case VM_PRE_HFO:
		{
		}
		break;
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_THERAPIE:
		{
		}
		break;
	case VM_PRE_CPAP:
		{
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}
SHORT CDataHandler::GetCurrentPINSPPara()
{
	SHORT iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			iValue=PARADATA()->GetPINSPPara_IPPV();
		}
		break;
	case VM_SIMV:
	case VM_SIPPV:
	case VM_SIMVPSV:
	case VM_PSV://newVG
		{
			iValue=PARADATA()->GetPINSPPara_TRIGGER();
		}
		break;
	/*case VM_PSV: //newVG
		{
			iValue=PARADATA()->GetPpsvPara();
		}
		break;*/
	case VM_HFO:
		{
			iValue=PARADATA()->GetPINSPPara_IPPV();
		}
		break;
	case VM_CPAP:
		{
			iValue=PARADATA()->GetPManualCPAPPara();
		}
		break;
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_THERAPIE:
		{
			iValue=PARADATA()->GetPManualNMODEPara();
		}
		break;
	case VM_PRE_IPPV:
		{
			iValue=PRESET()->GetPINSPPara_IPPV();
		}
		break;
	case VM_PRE_SIMV:
	case VM_PRE_SIPPV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_PSV: //newVG
		{
			iValue=PRESET()->GetPINSPPara_TRIGGER();
		}
		break;
	/*case VM_PRE_PSV: //newVG
		{
			iValue=PRESET()->GetPpsvPara();
		}
		break;*/
	case VM_PRE_HFO:
		{
			iValue=PRESET()->GetPINSPPara_TRIGGER();
		}
		break;
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_THERAPIE:
		{
			iValue=PRESET()->GetPManualNMODEPara();

		}
		break;
	case VM_PRE_CPAP:
		{
			iValue=PRESET()->GetPManualCPAPPara();

		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentPEEPPara(SHORT val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			if(PARADATA()->GetPEEPPara_IPPV()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetPEEPParadata_IPPV(val,true);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_PEEP);
		}
		break;
	case VM_SERVICE:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
		{
			if(PARADATA()->GetPEEPPara_TRIGGER()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetPEEPParadata_TRIGGER(val,true);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_PEEP);
		}
		break;
	case VM_PRE_IPPV:
		{
			PRESET()->SetPEEPPara_IPPV(val,false,false);
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
		{
			PRESET()->SetPEEPPara_TRIGGER(val,false,false);
		}
		break;
	default:
		break;
	}
}

SHORT CDataHandler::GetCurrentPEEPPara()
{
	SHORT iValue=0;

	eVentMode mode = getModel()->getCONFIG()->GetCurMode();

	switch(mode)
	{
	case VM_IPPV:
		{
			iValue=PARADATA()->GetPEEPPara_IPPV();
		}
		break;
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_HFO:
		{
			iValue=PARADATA()->GetPEEPPara_TRIGGER();
		}
		break;
	case VM_CPAP:
		{
			iValue=PARADATA()->GetCPAPPara();
		}
		break;
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_THERAPIE:
		{
			iValue=PARADATA()->GetCPAPNMODEPara();
		}
		break;
	case VM_PRE_IPPV:
		{
			iValue=PRESET()->GetPEEPPara_IPPV();
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_HFO:
		{
			iValue=PRESET()->GetPEEPPara_TRIGGER();
		}
		break;
	case VM_PRE_CPAP:
		{
			iValue=PRESET()->GetCPAPPara();
		}
		break;
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_THERAPIE:
		{
			iValue=PRESET()->GetCPAPNMODEPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentPpsvPara(SHORT val)
{
	bool bSend=true;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	//case VM_PSV: //newVG
	//	{
	//		if(PARADATA()->GetPpsvPara()!=val)
	//		{
	//			getModel()->getALARMHANDLER()->setAutoSilent();
	//		}
	//		if(PARADATA()->IsVGarantStateOn_TRIGGER()==true)
	//			bSend=false;

	//		SetPpsvParadata(val,true);

	//		//needed for manual breath
	//		if(bSend)
	//			getModel()->getSERIAL()->Send_PARAVAL_P_INSP(val);

	//		if(AfxGetApp())
	//			AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_PPSV);
	//	}
	//	break;
	case VM_PSV:
	case VM_SIMVPSV:
		{
			if(PARADATA()->GetPpsvPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetPpsvParadata(val,true);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_PPSV);
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMVPSV:
		{
			PRESET()->SetPpsvPara(val,false,false);
		}
		break;
	default:
		break;
	}
}
SHORT CDataHandler::GetCurrentPpsvPara()
{
	int iValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_PSV:
	case VM_SIMVPSV:
		{
			iValue=PARADATA()->GetPpsvPara();
		}
		break;
	case VM_PRE_PSV:
	case VM_PRE_SIMVPSV:
		{
			iValue=PRESET()->GetPpsvPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentCPAPPara(SHORT val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_CPAP:
		{
			if(PARADATA()->GetCPAPPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetCPAPParadata(val,true);
			
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_CPAP);
		}
		break;
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_THERAPIE:
		{
			if(PARADATA()->GetCPAPNMODEPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetCPAPNMODEParadata(val,true);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_CPAP);
		}
		break;
	case VM_PRE_CPAP:
		{
			PRESET()->SetCPAPPara(val,false,false);
		}
		break;
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_THERAPIE:
		{
			PRESET()->SetCPAPNMODEPara(val,false,false);
		}
		break;
	default:
		break;
	}
}

void CDataHandler::SetCurrentPManPara(SHORT val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_CPAP:
		{
			if(PARADATA()->GetPManualCPAPPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetPManualCPAPParadata(val,true);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_PMAN);
		}
		break;
	case VM_HFO:
		{
			if(PARADATA()->GetPManualHFOPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetPManualHFOParadata(val,true);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_PMAN);
		}
		break;
	case VM_DUOPAP:
	case VM_NCPAP:
	case VM_THERAPIE:
		{
			if(PARADATA()->GetPManualNMODEPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetPManualNMODEParadata(val,true);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_PMAN);
		}
		break;
	case VM_PRE_HFO:
		{
			PRESET()->SetPManualHFOPara(val,false,false);
		}
		break;
	case VM_PRE_CPAP:
		{
			PRESET()->SetPManualCPAPPara(val,false,false);
		}
		break;
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_THERAPIE:
		{
			PRESET()->SetPManualNMODEPara(val,false,false);
		}
		break;
	case VM_IPPV:
	case VM_PSV:
	case VM_SIMV:
	case VM_SIMVPSV:
	case VM_SIPPV:
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	default:
		break;
	}
}


// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentPMeanPara(WORD val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			if(PARADATA()->GetHFPMeanPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetHFPMeanParadata(val);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_PMITT);
		}
		break;
	case VM_PRE_HFO:
		{
			PRESET()->SetHFPMeanPara(val,false,false);
		}
		break;
	default:
		break;
	}
}
WORD CDataHandler::GetCurrentPmeanPara()
{
	WORD fValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			fValue=PARADATA()->GetHFPMeanPara();
		}
		break;
	case VM_PRE_HFO:
		{
			fValue=PRESET()->GetHFPMeanPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return fValue;
}


// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentBackupPara(BYTE val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_CPAP:
		{
			if(PARADATA()->GetBackupPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetBackupParadata(val,true);

			if(val==0)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_DEL_BACKUP);
			}
		}
		break;
	case VM_PRE_CPAP:
		{
			PRESET()->SetBackupPara(val,false,false);
		}
		break;
	default:
		break;
	}
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentIERatioParaHFO(eRatioIE val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			if(PARADATA()->GetIERatioParaHFO()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetIERatioParadataHFO(val);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_IERATIO);
		}
		break;
	case VM_PRE_HFO:
		{
			PRESET()->SetIERatioParaHFO(val,false,false);
		}
		break;
	default:
		break;
	}
}
eRatioIE CDataHandler::GetCurrentIERatioParaHFO()
{
	eRatioIE iValue=RIE_1_1;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			iValue=PARADATA()->GetIERatioParaHFO();
		}
		break;
	case VM_PRE_HFO:
		{
			iValue=PRESET()->GetIERatioParaHFO();
		}
		break;
	default:
		{
		}
		break;
	}

	return iValue;
}


// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentHFFreqPara(BYTE val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			if(PARADATA()->GetHFFreqPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetHFFreqParadata(val);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_HFFREQ);

			if(PARADATA()->IsHFVGarantStateOn()==true)
				SetHFAMPLmaxParadata(PARADATA()->GetHFAMPLmaxPara(), true);
			else
				SetHFAMPLParadata(PARADATA()->GetHFAMPLPara(), true);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_HFAMPL);
		}
		break;
	case VM_PRE_HFO:
		{
			PRESET()->SetHFFreqPara(val,false,false);
		}
		break;
	default:
		break;
	}
}
BYTE CDataHandler::GetCurrentHFFreqPara()
{
	BYTE fValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			fValue=PARADATA()->GetHFFreqPara();
		}
		break;
	case VM_PRE_HFO:
		{
			fValue=PRESET()->GetHFFreqPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return fValue;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentHFAMPLmaxPara(WORD val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			if(PARADATA()->GetHFAMPLmaxPara()!=val
				&& PARADATA()->IsHFVGarantStateOn()==true)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetHFAMPLmaxParadata(val,PARADATA()->IsHFVGarantStateOn());

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_HFAMPL);
		}
		break;
	case VM_PRE_HFO:
		{
			PRESET()->SetHFAMPLmaxPara(val,false,false);
		}
		break;
	default:
		break;
	}
}
WORD CDataHandler::GetCurrentHFAMPLmaxPara()
{
	WORD fValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			fValue=PARADATA()->GetHFAMPLmaxPara();
		}
		break;
	case VM_PRE_HFO:
		{
			fValue=PRESET()->GetHFAMPLmaxPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return fValue;
}
void CDataHandler::SetCurrentITimeRecPara(WORD val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			if(PARADATA()->GetITimeRecPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetITimeRecParadata(val);
		}
		break;
	case VM_PRE_HFO:
		{
			PRESET()->SetITimeRecPara(val,false,false);
		}
		break;
	default:
		break;
	}
}
WORD CDataHandler::GetCurrentITimeRecPara()
{
	int fValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			fValue=PARADATA()->GetITimeRecPara();
		}
		break;
	case VM_PRE_HFO:
		{
			fValue=PRESET()->GetITimeRecPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return fValue;
}

int CDataHandler::CalculateHF_ETIME_REC(int valFREQ, int valITIME)
{
	if(valFREQ==0)
	{
		return 1; //sec
	}

	int iETimeRec=(3600000/valFREQ)-valITIME;
	
	if(iETimeRec<MIN_ETIME_RECRUITMENT)
		iETimeRec=0;

	return iETimeRec; //sec
}

int CDataHandler::GetHF_ETIME_REC(int valFREQ)
{
	if(valFREQ==0)
	{
		return 2000; //sec
	}

	int iItemRec=GetCurrentITimeRecPara();
	int iETimeRec=(3600000/valFREQ)-iItemRec;

	if(iETimeRec<2000)
		iETimeRec=2000;

	return iETimeRec; //sec
}
void CDataHandler::SetCurrentFreqRecPara(WORD val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			WORD valOld=PARADATA()->GetFreqRecPara();
			if(valOld!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			SetFreqRecParadata(val);

			if(valOld==0)
			{
				getModel()->getALARMHANDLER()->setAlimitState_PIPmaxLimit(AL_AUTO);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_LUNGRECSTATE_CHANGED);
			}
			else if(val==0)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_LUNGRECSTATE_CHANGED);
			}

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_HFFREQREC);

		}
		break;
	case VM_PRE_HFO:
		{
			WORD valOld=PRESET()->GetFreqRecPara();
			//if(valOld!=val)
			{
				if(valOld==0)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_LUNGRECSTATE_CHANGED);
				}
				else if(val==0)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_LUNGRECSTATE_CHANGED);
				}

				
			}
			PRESET()->SetFreqRecPara(val,false,false);
		}
		break;
	default:
		break;
	}
}
WORD CDataHandler::GetCurrentFreqRecPara()
{
	int fValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			fValue=PARADATA()->GetFreqRecPara();
		}
		break;
	case VM_PRE_HFO:
		{
			fValue=PRESET()->GetFreqRecPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return fValue;
}

void CDataHandler::SetCurrentHFPMeanRecPara(WORD val)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			if(PARADATA()->GetHFPMeanRecPara()!=val)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PMEANREC_CHANGED);

			SetHFPMeanRecParadata(val,true);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_PMITT);
		}
		break;
	case VM_PRE_HFO:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PMEANREC_CHANGED);

			PRESET()->SetHFPMeanRecPara(val,false,false);
		}
		break;
	default:
		break;
	}
}
WORD CDataHandler::GetCurrentHFPMeanRecPara()
{
	WORD fValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			fValue=PARADATA()->GetHFPMeanRecPara();
		}
		break;
	case VM_PRE_HFO:
		{
			fValue=PRESET()->GetHFPMeanRecPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return fValue;
}

void CDataHandler::SetCurrentHFAMPLPara(WORD val)
{
	bool bSend=true;
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			if(		PARADATA()->GetHFAMPLPara()!=val
				&& PARADATA()->IsHFVGarantStateOn()==false)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}

			if(PARADATA()->IsHFVGarantStateOn()==true)
				bSend=false;
			SetHFAMPLParadata(val,bSend);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_HFAMPL);
		}
		break;
	case VM_PRE_HFO:
		{
			PRESET()->SetHFAMPLPara(val,false,false);
		}
		break;
	default:
		break;
	}
}
WORD CDataHandler::GetCurrentHFAMPLPara()
{
	WORD fValue=0;

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_HFO:
		{
			fValue=PARADATA()->GetHFAMPLPara();
		}
		break;
	case VM_PRE_HFO:
		{
			fValue=PRESET()->GetHFAMPLPara();
		}
		break;
	default:
		{
		}
		break;
	}

	return fValue;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCurrentVGarantParamData(WORD val)
{
	//bool bReset=false;
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			if(		PARADATA()->GetVGarantPara_IPPV()!=val
				&&	PARADATA()->IsVGarantStateOn_IPPV()==true)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}

			if(PARADATA()->IsVGarantStateOn_IPPV()==true)
			{
				SetCurrentVLimitParam(GetCurrentVLimitPara(),false,true);
			}
			SetVGarantParadata_IPPV(val, true ,true);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_VGARANT);
		}
		break;
	case VM_SERVICE:
	case VM_SIMV:
	case VM_SIPPV:
	case VM_SIMVPSV:
	case VM_PSV:
		{
			if(		PARADATA()->GetVGarantPara_TRIGGER()!=val
				&&	PARADATA()->IsVGarantStateOn_TRIGGER()==true)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}

			if(PARADATA()->IsVGarantStateOn_TRIGGER()==true)
			{
				SetCurrentVLimitParam(GetCurrentVLimitPara(),false,true);
			}
			SetVGarantParadata_TRIGGER(val, true ,true);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_VGARANT);
		}
		break;
	case VM_PRE_IPPV:
		{
			if(PRESET()->IsVGarantStateOn_IPPV()==true)
			{
				SetCurrentVLimitParam(GetCurrentVLimitPara(),false,false);
			}
			PRESET()->SetVGarantPara_IPPV(val,false,false);
		}
		break;
	case VM_PRE_SIMV:
	case VM_PRE_SIPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMVPSV:
		{
			if(PRESET()->IsVGarantStateOn_TRIGGER()==true)
			{
				SetCurrentVLimitParam(GetCurrentVLimitPara(),false,false);
			}
			PRESET()->SetVGarantPara_TRIGGER(val,false,false);
		}
		break;
	case VM_HFO:
		{
			if(		PARADATA()->GetHFVGarantPara()!=val
				&&	PARADATA()->IsHFVGarantStateOn()==true)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}

			SetHFVGarantParadata(val, true ,true);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_VGARANT);

		}
		break;
	case VM_PRE_HFO:
		{
			PRESET()->SetHFVGarantPara(val,false, false);
		}
		break;
	default:
		break;
	}
}

void CDataHandler::SetCurrentVLimitParam(WORD val, bool bOn, bool bSend)
{
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			if(		PARADATA()->GetVLimitParam_IPPV()!=val
				||	PARADATA()->IsVLimitParamOn_IPPV()!=bOn)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}

			if(bOn)
			{
				if(IsCurrentModeVGarantStateOn())
				{
					ChangeVgarantState(VOLUMEGAR_OFF);
				}
			}
			SetVLimitParamdata_IPPV(val, bOn ,bSend);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_VLIMIT);

		}
		break;
	case VM_SERVICE:
	case VM_SIMV:
	case VM_SIPPV:
	case VM_PSV:
	case VM_SIMVPSV:	
		{
			if(		PARADATA()->GetVLimitParam_TRIGGER()!=val
				||	PARADATA()->IsVLimitParamOn_TRIGGER()!=bOn)
			{
				getModel()->getALARMHANDLER()->setAutoSilent();
			}

			if(bOn)
			{
				if(IsCurrentModeVGarantStateOn())
				{
					//SetCurrentVGarantParamData(GetCurrentVGarantParaData(),false,true);
					ChangeVgarantState(VOLUMEGAR_OFF);
					//SetActiveModeVolumeGarantyState(false,true,true);
				}
			}
			SetVLimitParamdata_TRIGGER(val, bOn ,bSend);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_VLIMIT);

		}
		break;
	case VM_PRE_IPPV:
		{
			if(bOn)
			{
				/*if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABUTTON_TURNEDOWN,IDC_BTN_PARA_VLIMIT,0);*/
				SetPresetModeVolumeGarantyState(false);
			}
			PRESET()->SetVLimitParam_IPPV(val, bOn ,false,false);
		}
		break;
	case VM_PRE_SIMV:
	case VM_PRE_SIPPV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_PSV:
		{
			if(bOn)
			{
				/*if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABUTTON_TURNEDOWN,IDC_BTN_PARA_VLIMIT,0);*/
				SetPresetModeVolumeGarantyState(false);
			}
			PRESET()->SetVLimitParam_TRIGGER(val, bOn ,false,false);
		}
		break;
	default:
		break;
	}
}

//void CDataHandler::setCurrentFOT_AMPLITUDEPara(WORD val)
//{
//	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
//	{
//		getModel()->getALARMHANDLER()->setAutoSilent();
//		SetFOThfo_AMPLITUDEParadata(val,getFOTstate());
//	}
//	else
//	{
//		getModel()->getALARMHANDLER()->setAutoSilent();
//		SetFOTconv_AMPLITUDEParadata(val,getFOTstate());
//	}
//
//	//getModel()->getThreadFOT()->
//}

//void CDataHandler::setCurrentFOT_FREQPara(WORD val)
//{
//	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
//	{
//		getModel()->getALARMHANDLER()->setAutoSilent();
//		SetFOThfo_FREQParadata(val,getFOTstate());
//	}
//	else
//	{
//		getModel()->getALARMHANDLER()->setAutoSilent();
//		SetFOTconv_FREQParadata(val,getFOTstate());
//	}
//	
//}

void CDataHandler::setCurrentFOT_STEPSPara(WORD val)
{
	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		getModel()->getALARMHANDLER()->setAutoSilent();
		SetFOThfo_STEPSParadata(val,false);

		checkFOTPmeanPara();
	}
	else
	{
		getModel()->getALARMHANDLER()->setAutoSilent();
		SetFOTconv_STEPSParadata(val,false);

		checkFOTPEEPPara();
	}
}
void CDataHandler::checkFOTPEEPPara()
{
	bool bUpdatePEEPParas=false;
	WORD iDiffPEEPFOT=getModel()->getCONFIG()->getDiffPEEPFOT();
	WORD iFOTconv_PEEPSTART=PARADATA()->getFOTconv_PEEPSTARTPara();
	WORD iFOTconv_PEEPEND=PARADATA()->getFOTconv_PEEPENDPara();

	if(iFOTconv_PEEPSTART>iFOTconv_PEEPEND-iDiffPEEPFOT)
	{
		//iFOTconv_PEEPSTART=FACTORY_FOT_CONV_PEEP_START;
		//iFOTconv_PEEPEND=FACTORY_FOT_CONV_PEEP_END;
		iFOTconv_PEEPSTART=PARADATA()->getFOTconv_PEEPSTARTPara();
		iFOTconv_PEEPEND=PARADATA()->getFOTconv_PEEPSTARTPara()+iDiffPEEPFOT;
		PARADATA()->setFOTconv_PEEPSTARTPara(iFOTconv_PEEPSTART,true,true);
		PARADATA()->setFOTconv_PEEPENDPara(iFOTconv_PEEPEND,true,true);
		bUpdatePEEPParas=true;
	}
	if(iFOTconv_PEEPEND<iFOTconv_PEEPSTART+iDiffPEEPFOT)
	{
		//iFOTconv_PEEPSTART=FACTORY_FOT_CONV_PEEP_START;
		//iFOTconv_PEEPEND=FACTORY_FOT_CONV_PEEP_END;
		iFOTconv_PEEPSTART=PARADATA()->getFOTconv_PEEPSTARTPara();
		iFOTconv_PEEPEND=PARADATA()->getFOTconv_PEEPSTARTPara()+iDiffPEEPFOT;
		PARADATA()->setFOTconv_PEEPSTARTPara(iFOTconv_PEEPSTART,true,true);
		PARADATA()->setFOTconv_PEEPENDPara(iFOTconv_PEEPEND,true,true);
		bUpdatePEEPParas=true;
	}
	if(bUpdatePEEPParas)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_FOT_UPDATE_PEEP);
	}
}

void CDataHandler::setCurrentFOT_PEEPSTARTPara(WORD val)
{
	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		
	}
	else
	{
		getModel()->getALARMHANDLER()->setAutoSilent();
		SetFOTconv_PEEPSTARTParadata(val,false);
	}
}
void CDataHandler::setCurrentFOT_PEEPENDPara(WORD val)
{
	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		
	}
	else
	{
		getModel()->getALARMHANDLER()->setAutoSilent();
		SetFOTconv_PEEPENDParadata(val,false);
	}
}

void CDataHandler::checkFOTPmeanPara()
{
	bool bUpdatePmeanParas=false;
	WORD iDiffPmeanFOT=getModel()->getCONFIG()->getDiffPmeanFOT();
	WORD iFOThfo_PMEANSTART=PARADATA()->getFOThfo_PMEANSTARTPara();
	WORD iFOThfo_PMEANEND=PARADATA()->getFOThfo_PMEANENDPara();

	if(iFOThfo_PMEANSTART>iFOThfo_PMEANEND-iDiffPmeanFOT)
	{
		//iFOThfo_PMEANSTART=FACTORY_FOT_HFO_PMEAN_START;
		//iFOThfo_PMEANEND=FACTORY_FOT_HFO_PMEAN_END;
		iFOThfo_PMEANSTART=PARADATA()->getFOThfo_PMEANSTARTPara();
		iFOThfo_PMEANEND=PARADATA()->getFOThfo_PMEANSTARTPara()+iDiffPmeanFOT;
		PARADATA()->setFOThfo_PMEANSTARTPara(iFOThfo_PMEANSTART,true,true);
		PARADATA()->setFOThfo_PMEANENDPara(iFOThfo_PMEANEND,true,true);
		bUpdatePmeanParas=true;
	}
	if(iFOThfo_PMEANEND<iFOThfo_PMEANSTART+iDiffPmeanFOT)
	{
		//iFOThfo_PMEANSTART=FACTORY_FOT_HFO_PMEAN_START;
		//iFOThfo_PMEANEND=FACTORY_FOT_HFO_PMEAN_END;
		iFOThfo_PMEANSTART=PARADATA()->getFOThfo_PMEANSTARTPara();
		iFOThfo_PMEANEND=PARADATA()->getFOThfo_PMEANSTARTPara()+iDiffPmeanFOT;
		PARADATA()->setFOThfo_PMEANSTARTPara(iFOThfo_PMEANSTART,true,true);
		PARADATA()->setFOThfo_PMEANENDPara(iFOThfo_PMEANEND,true,true);
		bUpdatePmeanParas=true;
	}
	if(bUpdatePmeanParas)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_FOT_UPDATE_PMEAN);
	}
}
void CDataHandler::setCurrentFOT_PMEANSTARTPara(WORD val)
{
	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		getModel()->getALARMHANDLER()->setAutoSilent();
		SetFOThfo_PMEANSTARTParadata(val,false);
	}
	else
	{
		//error	
	}
}
void CDataHandler::setCurrentFOT_PMEANENDPara(WORD val)
{
	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		getModel()->getALARMHANDLER()->setAutoSilent();
		SetFOThfo_PMEANENDParadata(val,false);
	}
	else
	{
		//error	
	}
}

// **************************************************************************
// CContrastDlg helper functions
// **************************************************************************
DWORD CDataHandler::GetContrast(void )
{
	struct ContrastCmdInputParm ContrastParm, RetParm;  
	HDC dcLCD=NULL;;
	int nRet;
	DWORD dwEscFctn;
	TCHAR szDllName[50] = _T("ddi.dll"); 

	// get name of DLL with display driver
	CRegKey cKey; 
	if( ERROR_SUCCESS == cKey.Open( HKEY_LOCAL_MACHINE, _T("System\\GDI\\Drivers")) ) 
	{ 
		cKey.QueryValue( _T("Display"), szDllName ); 
	}

	ContrastParm.command	= CONTRAST_CMD_GET;
	ContrastParm.parm		= 0;

	//ASSERT(dcLCD = CreateDC( szDllName, NULL, NULL, NULL)); 
	dcLCD = CreateDC( szDllName, NULL, NULL, NULL);

	dwEscFctn = CONTRASTCOMMAND;

	// Check if platform supports this ESC function
	nRet = ExtEscape( dcLCD, QUERYESCSUPPORT, sizeof (dwEscFctn ), (LPCSTR)&dwEscFctn, 0, NULL );
	if( 0 == nRet )
	{
		theApp.getLog()->WriteLine( _T("#HFO:0082"));
	}
	else
	{
		// get contrast value
		nRet = ExtEscape( dcLCD, dwEscFctn, sizeof (struct ContrastCmdInputParm), (LPCSTR)&ContrastParm, 
			sizeof (struct ContrastCmdInputParm), (LPSTR)&RetParm );
	}

	DeleteDC( dcLCD );

	//m_iContrastParm=RetParm.parm;

	return RetParm.parm;

}

void CDataHandler::SetContrast(DWORD dwValue )
{
	struct ContrastCmdInputParm ContrastParm, RetParm;  
	HDC dcLCD=NULL;
	int nRet=0;
	TCHAR szDllName[50] = _T("ddi.dll"); 

	// get name of DLL with display driver
	CRegKey cKey; 
	if( ERROR_SUCCESS == cKey.Open( HKEY_LOCAL_MACHINE, _T("System\\GDI\\Drivers")) ) 
	{ 
		cKey.QueryValue( _T("Display"), szDllName ); 
	}

	ContrastParm.command	= CONTRAST_CMD_SET;
	ContrastParm.parm		= dwValue;

	m_iContrastParm=ContrastParm.parm;

	//ASSERT(dcLCD = CreateDC( szDllName, NULL, NULL, NULL));
	dcLCD = CreateDC( szDllName, NULL, NULL, NULL);

	nRet = ExtEscape( dcLCD, CONTRASTCOMMAND, sizeof (struct ContrastCmdInputParm), (LPCSTR)&ContrastParm, 
		sizeof (struct ContrastCmdInputParm), (LPSTR)&RetParm );

	DeleteDC( dcLCD );
}

DWORD CDataHandler::IncreaseContrast()
{
	struct ContrastCmdInputParm ContrastParm, RetParm;  
	HDC dcLCD=NULL;
	int nRet;
	DWORD dwEscFctn;
	TCHAR szDllName[50] = _T("ddi.dll"); 

	// get name of DLL with display driver
	CRegKey cKey; 
	if( ERROR_SUCCESS == cKey.Open( HKEY_LOCAL_MACHINE, _T("System\\GDI\\Drivers")) ) 
	{ 
		cKey.QueryValue( _T("Display"), szDllName ); 
	}

	ContrastParm.command	= CONTRAST_CMD_GET;
	ContrastParm.parm		= 0;

	//ASSERT(dcLCD = CreateDC( szDllName, NULL, NULL, NULL)); 
	dcLCD = CreateDC( szDllName, NULL, NULL, NULL);

	dwEscFctn = CONTRASTCOMMAND;

	// Check if platform supports this ESC function
	nRet = ExtEscape( dcLCD, QUERYESCSUPPORT, sizeof (dwEscFctn ), (LPCSTR)&dwEscFctn, 0, NULL );
	if( 0 == nRet )
	{
		theApp.getLog()->WriteLine( _T("#HFO:0082"));
	}
	else
	{
		// get contrast value
		nRet = ExtEscape( dcLCD, dwEscFctn, sizeof (struct ContrastCmdInputParm), (LPCSTR)&ContrastParm, 
			sizeof (struct ContrastCmdInputParm), (LPSTR)&RetParm );

		// set contrast value
		ContrastParm.command	= CONTRAST_CMD_SET;
		if(m_bLEDdisplay)
		{
			if(RetParm.parm==1023)
			{
				ContrastParm.parm=1023;
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else if(RetParm.parm+230>1023)
			{
				ContrastParm.parm=1023;
			}
			else
				ContrastParm.parm=RetParm.parm+230;
		}
		else
		{
			if(RetParm.parm==0)
			{
				ContrastParm.parm=23;
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else if(RetParm.parm-250<23)
			{
				ContrastParm.parm=0;
			}
			else
				ContrastParm.parm=RetParm.parm-250;
		}
		

		m_iContrastParm=ContrastParm.parm;

		nRet = ExtEscape( dcLCD, CONTRASTCOMMAND, sizeof (struct ContrastCmdInputParm), (LPCSTR)&ContrastParm, 
			sizeof (struct ContrastCmdInputParm), (LPSTR)&RetParm );

		// Call function to save registry.
		RegFlushKey( HKEY_CURRENT_USER );
	}

	DeleteDC( dcLCD );

	return m_iContrastParm;

}
DWORD CDataHandler::DecreaseContrast()
{
	struct ContrastCmdInputParm ContrastParm, RetParm;  
	HDC dcLCD=NULL;
	int nRet;
	DWORD dwEscFctn;
	TCHAR szDllName[50] = _T("ddi.dll"); 

	// get name of DLL with display driver
	CRegKey cKey; 
	if( ERROR_SUCCESS == cKey.Open( HKEY_LOCAL_MACHINE, _T("System\\GDI\\Drivers")) ) 
	{ 
		cKey.QueryValue( _T("Display"), szDllName ); 
	}

	ContrastParm.command	= CONTRAST_CMD_GET;
	ContrastParm.parm		= 0;

	//ASSERT(dcLCD = CreateDC( szDllName, NULL, NULL, NULL)); 
	dcLCD = CreateDC( szDllName, NULL, NULL, NULL);

	dwEscFctn = CONTRASTCOMMAND;

	// Check if platform supports this ESC function
	nRet = ExtEscape( dcLCD, QUERYESCSUPPORT, sizeof (dwEscFctn ), (LPCSTR)&dwEscFctn, 0, NULL );
	if( 0 == nRet )
	{
		theApp.getLog()->WriteLine( _T("#HFO:0082"));
	}
	else
	{
		// get contrast value
		nRet = ExtEscape( dcLCD, dwEscFctn, sizeof (struct ContrastCmdInputParm), (LPCSTR)&ContrastParm, 
			sizeof (struct ContrastCmdInputParm), (LPSTR)&RetParm );

		// set contrast value
		ContrastParm.command	= CONTRAST_CMD_SET;

		if(m_bLEDdisplay)
		{
			if(RetParm.parm<=103)
			{
				ContrastParm.parm=103;
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else if(RetParm.parm-230<103)
			{
				ContrastParm.parm=103;
			}
			else
				ContrastParm.parm=RetParm.parm-230;
		}
		else
		{
			if(RetParm.parm==1023)
			{
				ContrastParm.parm=1023;
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else if(RetParm.parm+250>1023)
				ContrastParm.parm=1023;
			else
				ContrastParm.parm=RetParm.parm+250;
		}
		

		m_iContrastParm=ContrastParm.parm;

		nRet = ExtEscape( dcLCD, CONTRASTCOMMAND, sizeof (struct ContrastCmdInputParm), (LPCSTR)&ContrastParm, 
			sizeof (struct ContrastCmdInputParm), (LPSTR)&RetParm );

		// Call function to save registry.
		RegFlushKey( HKEY_CURRENT_USER );
	}

	DeleteDC( dcLCD );

	return m_iContrastParm;
}

DWORD CDataHandler::IncreaseMaxContrast()
{
	struct ContrastCmdInputParm ContrastParm, RetParm;  
	HDC dcLCD=NULL;
	int nRet;
	DWORD dwEscFctn;
	TCHAR szDllName[50] = _T("ddi.dll"); 

	// get name of DLL with display driver
	CRegKey cKey; 
	if( ERROR_SUCCESS == cKey.Open( HKEY_LOCAL_MACHINE, _T("System\\GDI\\Drivers")) ) 
	{ 
		cKey.QueryValue( _T("Display"), szDllName ); 
	}

	ContrastParm.command	= CONTRAST_CMD_GET;
	ContrastParm.parm		= 0;

	//ASSERT(dcLCD = CreateDC( szDllName, NULL, NULL, NULL)); 
	dcLCD = CreateDC( szDllName, NULL, NULL, NULL);

	dwEscFctn = CONTRASTCOMMAND;

	// Check if platform supports this ESC function
	nRet = ExtEscape( dcLCD, QUERYESCSUPPORT, sizeof (dwEscFctn ), (LPCSTR)&dwEscFctn, 0, NULL );
	if( 0 == nRet )
	{
		theApp.getLog()->WriteLine( _T("#HFO:0082"));
	}
	else
	{
		// get contrast value
		nRet = ExtEscape( dcLCD, dwEscFctn, sizeof (struct ContrastCmdInputParm), (LPCSTR)&ContrastParm, 
			sizeof (struct ContrastCmdInputParm), (LPSTR)&RetParm );

		// set contrast value
		ContrastParm.command	= CONTRAST_CMD_SET;
		if(m_bLEDdisplay)
		{
			ContrastParm.parm=1023;
		}
		else
		{
			ContrastParm.parm=23;
		}
		

		m_iContrastParm=ContrastParm.parm;

		nRet = ExtEscape( dcLCD, CONTRASTCOMMAND, sizeof (struct ContrastCmdInputParm), (LPCSTR)&ContrastParm, 
			sizeof (struct ContrastCmdInputParm), (LPSTR)&RetParm );

		// Call function to save registry.
		RegFlushKey( HKEY_CURRENT_USER );
	}

	DeleteDC( dcLCD );

	return m_iContrastParm;
}
DWORD CDataHandler::DecreaseMaxContrast()
{
	struct ContrastCmdInputParm ContrastParm, RetParm;  
	HDC dcLCD=NULL;
	int nRet;
	DWORD dwEscFctn;
	TCHAR szDllName[50] = _T("ddi.dll"); 

	// get name of DLL with display driver
	CRegKey cKey; 
	if( ERROR_SUCCESS == cKey.Open( HKEY_LOCAL_MACHINE, _T("System\\GDI\\Drivers")) ) 
	{ 
		cKey.QueryValue( _T("Display"), szDllName ); 
	}

	ContrastParm.command	= CONTRAST_CMD_GET;
	ContrastParm.parm		= 0;

	//ASSERT(dcLCD = CreateDC( szDllName, NULL, NULL, NULL)); 
	dcLCD = CreateDC( szDllName, NULL, NULL, NULL);

	dwEscFctn = CONTRASTCOMMAND;

	// Check if platform supports this ESC function
	nRet = ExtEscape( dcLCD, QUERYESCSUPPORT, sizeof (dwEscFctn ), (LPCSTR)&dwEscFctn, 0, NULL );
	if( 0 == nRet )
	{
		theApp.getLog()->WriteLine( _T("#HFO:0082"));
	}
	else
	{
		// get contrast value
		nRet = ExtEscape( dcLCD, dwEscFctn, sizeof (struct ContrastCmdInputParm), (LPCSTR)&ContrastParm, 
			sizeof (struct ContrastCmdInputParm), (LPSTR)&RetParm );

		// set contrast value
		ContrastParm.command	= CONTRAST_CMD_SET;

		if(m_bLEDdisplay)
		{
			ContrastParm.parm=103;
		}
		else
		{
			ContrastParm.parm=1023;
		}
		

		m_iContrastParm=ContrastParm.parm;
	
		nRet = ExtEscape( dcLCD, CONTRASTCOMMAND, sizeof (struct ContrastCmdInputParm), (LPCSTR)&ContrastParm, 
			sizeof (struct ContrastCmdInputParm), (LPSTR)&RetParm );

		// Call function to save registry.
		RegFlushKey( HKEY_CURRENT_USER );
	}

	DeleteDC( dcLCD );

	return m_iContrastParm;
}

DWORD CDataHandler::GetCurContrastParm()
{
	return m_iContrastParm;
}

//bool CDataHandler::activeModeIsCPAP()
//{
//	if(getModel()->getCONFIG()->GetCurMode()==VM_CPAP)
//	{
//		return true;
//	}
//	else if(getModel()->getCONFIG()->CurModeIsPresetMode() && getModel()->getCONFIG()->GetPrevMode()==VM_CPAP)
//	{
//		return true;
//	}
//	else
//		return false;
//}
//bool CDataHandler::activeModeIsIPPV()
//{
//	if(getModel()->getCONFIG()->GetCurMode()==VM_IPPV)
//	{
//		return true;
//	}
//	else if(getModel()->getCONFIG()->CurModeIsPresetMode() && getModel()->getCONFIG()->GetPrevMode()==VM_IPPV)
//	{
//		return true;
//	}
//	else
//		return false;
//}
//
//bool CDataHandler::activeModeIsHFO()
//{
//	if(getModel()->getCONFIG()->GetCurMode()==VM_HFO)
//	{
//		return true;
//	}
//	else if(getModel()->getCONFIG()->CurModeIsPresetMode() && getModel()->getCONFIG()->GetPrevMode()==VM_HFO)
//	{
//		return true;
//	}
//	else
//		return false;
//}
//bool CDataHandler::activeModeHasPSV()
//{
//	if(		getModel()->getCONFIG()->GetCurMode()==VM_PSV
//		||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV)
//	{
//		return true;
//	}
//	else if(	getModel()->getCONFIG()->CurModeIsPresetMode()==true 
//		&&	(	getModel()->getCONFIG()->GetPrevMode()==VM_PSV
//		||	getModel()->getCONFIG()->GetPrevMode()==VM_SIMVPSV))
//	{
//		return true;
//	}
//	else
//		return false;
//}
//bool CDataHandler::activeModeIsVM_PSV()
//{
//	if(getModel()->getCONFIG()->GetCurMode()==VM_PSV)
//	{
//		return true;
//	}
//	else if(	getModel()->getCONFIG()->CurModeIsPresetMode()==true 
//			&&	getModel()->getCONFIG()->GetPrevMode()==VM_PSV)
//	{
//		return true;
//	}
//	else
//		return false;
//}
//bool CDataHandler::modeIsPSVmode()
//{
//	if(getModel()->getCONFIG()->GetCurMode()==VM_PSV)
//	{
//		return true;
//	}
//	else if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_PSV)
//	{
//		return true;
//	}
//	else
//		return false;
//}
//
//
//
//bool CDataHandler::activeModeIsNMODE()
//{
//	if(		getModel()->getCONFIG()->GetCurMode()==VM_NCPAP 
//		||	getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
//		||	getModel()->getCONFIG()->GetCurMode()==VM_THERAPIE)
//	{
//		return true;
//	}
//	else if(	getModel()->getCONFIG()->CurModeIsPresetMode()==true 
//			&&	(		getModel()->getCONFIG()->GetPrevMode()==VM_DUOPAP
//					||	getModel()->getCONFIG()->GetPrevMode()==VM_NCPAP
//					||	getModel()->getCONFIG()->GetPrevMode()==VM_THERAPIE))
//	{
//		return true;
//	}
//	else
//		return false;
//}
//
//bool CDataHandler::curModeIsVolumeSupportMode()
//{
//	bool bResult = false;
//
//	if(		getModel()->getCONFIG()->GetCurMode()==VM_IPPV
//		||	getModel()->getCONFIG()->GetCurMode()==VM_SIPPV
//		||	getModel()->getCONFIG()->GetCurMode()==VM_SIMV
//		||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
//		||	getModel()->getCONFIG()->GetCurMode()==VM_PSV
//		||	getModel()->getCONFIG()->GetCurMode()==VM_HFO
//		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_IPPV
//		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIPPV
//		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMV
//		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV
//		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_PSV
//		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO)
//	{
//		bResult = true;
//	}
//
//	return bResult;
//}
//
//bool CDataHandler::activeModeIsTriggerMode()
//{
//	bool bResult = false;
//
//	if(		getModel()->getCONFIG()->GetCurMode()==VM_SIPPV
//		||	getModel()->getCONFIG()->GetCurMode()==VM_SIMV
//		||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
//		||	getModel()->getCONFIG()->GetCurMode()==VM_PSV)
//	{
//		bResult = true;
//	}
//	else if(	getModel()->getCONFIG()->CurModeIsPresetMode() && 
//				(	getModel()->getCONFIG()->GetPrevMode()==VM_SIPPV
//				||	getModel()->getCONFIG()->GetPrevMode()==VM_SIMV
//				||	getModel()->getCONFIG()->GetPrevMode()==VM_SIMVPSV
//				||	getModel()->getCONFIG()->GetPrevMode()==VM_PSV))
//	{
//		bResult = true;
//	}
//
//	return bResult;
//}




// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetAlarmStateBytes(int iAlarmState)
{
	EnterCriticalSection(&csAlarmStateBytes);
	m_iAlarmState=iAlarmState;
	LeaveCriticalSection(&csAlarmStateBytes);
}
// **************************************************************************
// 
// **************************************************************************
int CDataHandler::GetAlarmStateBytes()
{
	EnterCriticalSection(&csAlarmStateBytes);
	int iTemp=m_iAlarmState;
	LeaveCriticalSection(&csAlarmStateBytes);
	return iTemp;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::ResetAlarmStateBytes()
{
	EnterCriticalSection(&csAlarmStateBytes);
	m_iAlarmState=0;
	LeaveCriticalSection(&csAlarmStateBytes);
}


//************************************
// Method:    SetFlowsensorStateBytes
// FullName:  CDataHandler::SetFlowsensorStateBytes
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int iState
//
//  2015/07/02: flow sensor handling checked for thread safe
//************************************
void CDataHandler::SetFlowsensorStateBytes(int iState)
{
	EnterCriticalSection(&csFlowsensorStateBytes);
	m_iFlowsensorState=iState;
	LeaveCriticalSection(&csFlowsensorStateBytes);
}
//************************************
// Method:    GetFlowsensorStateBytes
// FullName:  CDataHandler::GetFlowsensorStateBytes
// Access:    public 
// Returns:   int
// Qualifier:
//
//  2015/07/02: flow sensor handling checked for thread safe
//************************************
int CDataHandler::GetFlowsensorStateBytes()
{
	EnterCriticalSection(&csFlowsensorStateBytes);
	int iTemp=m_iFlowsensorState;
	LeaveCriticalSection(&csFlowsensorStateBytes);
	return iTemp;
}

//************************************
// Method:    ResetFlowsensorStateBytes
// FullName:  CDataHandler::ResetFlowsensorStateBytes
// Access:    public 
// Returns:   void
// Qualifier:
//
//  2015/07/02: flow sensor handling checked for thread safe
//************************************
void CDataHandler::ResetFlowsensorStateBytes()
{
	EnterCriticalSection(&csFlowsensorStateBytes);
	m_iFlowsensorState=0;
	LeaveCriticalSection(&csFlowsensorStateBytes);
}


// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetConPICChecksumError(bool state)
{
	m_bConPICChecksumError=state;
}

// **************************************************************************
// 
// **************************************************************************
bool CDataHandler::GetConPICChecksumError()
{
	return m_bConPICChecksumError;
}


// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetControllerPIC_Checksum(int iChecksum)
{
	m_dataMainboard.m_iConductorChecksum=iChecksum;
}

int CDataHandler::GetControllerPIC_Checksum()
{
	return m_dataMainboard.m_iConductorChecksum;
}


// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetControllerPIC_CALPRESSSCALE(int iVal)
{
	m_iConductorCALPRESSSCALE=iVal;
}

int CDataHandler::GetControllerPIC_CALPRESSSCALE()
{
	return m_iConductorCALPRESSSCALE;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetControllerPIC_CALPRESSOFFSET(int iVal)
{
	m_iConductorCALPRESSOFFSET=iVal;
}

int CDataHandler::GetControllerPIC_CALPRESSOFFSET()
{
	return m_iConductorCALPRESSOFFSET;
}


// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetControllerPIC_Version(CStringW sz)
{
	m_dataMainboard.m_szConductorVersion=sz;
}
CStringW CDataHandler::GetControllerPIC_Version()
{
	return m_dataMainboard.m_szConductorVersion;
}
// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetBlenderPIC_Version(CStringW sz)
{
	m_dataMainboard.m_szBlenderVersion=sz;
}
CStringW CDataHandler::GetBlenderPIC_Version()
{
	return m_dataMainboard.m_szBlenderVersion;
}


// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetHFOPIC_Version(CStringW sz)
{
	m_dataMainboard.m_szHfoVersion=sz;
}
CStringW CDataHandler::GetHFOPIC_Version()
{
	return m_dataMainboard.m_szHfoVersion;
}
void CDataHandler::SetHFOPIC_Checksum(int iChecksum)
{
	m_dataMainboard.m_iHfoChecksum=iChecksum;
}
int CDataHandler::GetHFOPIC_Checksum()
{
	return m_dataMainboard.m_iHfoChecksum;
}

// **************************************************************************
// 
// **************************************************************************
WORD CDataHandler::GetAccuState()
{
	return m_wACCU_DAT_STATE;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::CheckAkku(bool bRange)
{
	WORD wBatteryState=0;
	WORD temp=0;
	if(getModel()->getI2C()->IsI2C_ACCUavailability())
	{
		temp=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_STATE);

		if(temp==0)
		{
			theApp.getLog()->WriteLine(_T("ERROR 999"));
			m_bI2CError=true;
			getModel()->getI2C()->SetI2Cerror(DEVICE_ADDRESS_ACCUPIC);
		}
		else if(	(temp & BIT8) 
			||	(temp & BIT9) 
			|| (temp & BIT10) 
			|| (temp & BIT11) 
			|| (temp & BIT12))
		{
			theApp.getLog()->WriteLine(_T("ERROR 998"));
			m_bI2CError=true;
			getModel()->getI2C()->SetI2Cerror(DEVICE_ADDRESS_ACCUPIC);
		}
		else
		{
			m_wACCU_DAT_STATE=temp;
			m_bI2CError=false;
			getModel()->getI2C()->ResetI2Cerror();

			if(getModel()->getALARMHANDLER()->ALARM_SysFail_IF_I2C->getAlarmState()!=AS_NONE)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_SysFail_IF_I2C);
				getModel()->getI2C()->SetI2C_ACCUavailability(TRUE);
			}
		}
		
		Sleep(0);
		
		if(!m_bI2CError)
		{
			if(m_wACCU_DAT_STATE & BIT0)
			{
				if(true==SetAccuSupply(true))
				{
					if(getModel()->getALARMHANDLER()->CanSetAlarm_Accu_POWER())
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_Accu_POWER);
					}
					if(getModel()->getAcuLink()!=NULL)
					{
						getModel()->getAcuLink()->setParaData(ALINK_SETT_POWERSTATE,1);
					}
				}
			}
			else
			{
				if(getModel()->getALARMHANDLER()->ALARM_Accu_POWER->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_Accu_POWER);
				}
				if(true==SetAccuSupply(false))
				{
					if(getModel()->getAcuLink()!=NULL)
					{
						getModel()->getAcuLink()->setParaData(ALINK_SETT_POWERSTATE,0);
					}
				}
			}

			if(m_wACCU_DAT_STATE & BIT13)//Accu defect
			{
				if(getModel()->getALARMHANDLER()->CanSetAlarm_AccuDefect())
				{
					if(getModel()->getALARMHANDLER()->setAlarm(AL_Accu_Defect,_T("")))
					{
						m_bShowBatteryEmpty=false;
						m_bShowBatteryState15=false;
						m_bShowBattery30=false;
						m_bShowBattery60=false;

						if(isTRENDLicenseAvailable())
						{
							theApp.getLog()->WriteLine(_T("#HFO:0083"));
							//rkuNEWFIX
							SerializeAllTrends(false);
						}
					}
				}
			}
			else
			{
				if(getModel()->getALARMHANDLER()->ALARM_Accu_Defect->getAlarmState()!=AS_NONE)
				{
					eVentSilentState state=getModel()->getALARMHANDLER()->getAlarmSilentState();
					if(		state==ASTATE_AUTOSILENT
						||	state==ASTATE_MANSILENT
						//||	state==ASTATE_MANSILENT_SYSALARM
						||	state==ASTATE_ACTIVE)
						getModel()->getALARMHANDLER()->setAutoSilent(false,false,false);

					getModel()->getALARMHANDLER()->deleteAlarm(AL_Accu_Defect);

					m_bShowBatteryEmpty=true;
					m_bShowBatteryState15=true;
					m_bShowBattery30=true;
					m_bShowBattery60=false;
				}


			}

			if(m_wACCU_DAT_STATE & BIT14)//notaus
			{
				getModel()->SetAccuTurnoff();
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_VENT_TURNOFF);
			}

			if(!m_bI2CError)
			{
				m_wBattStatMinutes=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_REMAIN);

				Sleep(0);

				m_wBattStatPercents=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_PERCENT);
				
			}

			if(		m_wBattStatMinutes==65535
				&&	m_wBattStatPercents==65535)
			{
				getModel()->getI2C()->SetI2C_ACCUavailability(FALSE);
				if(getModel()->getAcuLink()!=NULL)
				{
					getModel()->getAcuLink()->setParaData(ALINK_SETT_BATTERYSTATE_MINUTES,ALINK_NOTVALID);
					getModel()->getAcuLink()->setParaData(ALINK_SETT_BATTERYSTATE_PERCENT,ALINK_NOTVALID);
				}
			}
			else
			{
				if(getModel()->getAcuLink()!=NULL)
				{
					if(IsAccuSupply()==true)
						getModel()->getAcuLink()->setParaData(ALINK_SETT_BATTERYSTATE_MINUTES,m_wBattStatMinutes);
					else
						getModel()->getAcuLink()->setParaData(ALINK_SETT_BATTERYSTATE_MINUTES,ALINK_NOTVALID);
					getModel()->getAcuLink()->setParaData(ALINK_SETT_BATTERYSTATE_PERCENT,m_wBattStatPercents);
				}
			}
		}
		
	}
	else if(getModel()->getI2C()->GetACCUstate()==true && getModel()->getI2C()->IsI2C_ACCUavailability()==FALSE)
	{
		temp=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_STATE);
	
		if(temp==0)
		{
			m_bI2CError=true;

		}
		else if(	(temp & BIT8) 
			||	(temp & BIT9) 
			|| (temp & BIT10) 
			|| (temp & BIT11) 
			|| (temp & BIT12))
		{
			m_bI2CError=true;
		}
		else
		{
			m_bI2CError=false;
		}

		if(!m_bI2CError)
		{
			if(getModel()->getALARMHANDLER()->ALARM_SysFail_IF_I2C->getAlarmState()!=AS_NONE)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_SysFail_IF_I2C);
				getModel()->getI2C()->SetI2C_ACCUavailability(TRUE);
			}

			m_wBattStatMinutes=getModel()->getI2C()->ReadAccuWord(ACCU_DAT_REMAIN);
			if(m_wBattStatMinutes!=65535)
			{
				if(getModel()->getAcuLink()!=NULL)
				{
					getModel()->getAcuLink()->setParaData(ALINK_SETT_BATTERYSTATE_MINUTES,m_wBattStatMinutes);
					getModel()->getAcuLink()->setParaData(ALINK_SETT_BATTERYSTATE_PERCENT,ALINK_NOTVALID);
				}
			}
			else
			{
				if(getModel()->getAcuLink()!=NULL)
				{
					getModel()->getAcuLink()->setParaData(ALINK_SETT_BATTERYSTATE_MINUTES,ALINK_NOTVALID);
					getModel()->getAcuLink()->setParaData(ALINK_SETT_BATTERYSTATE_PERCENT,ALINK_NOTVALID);
				}
			}
		}
	}
	/*else if(getModel()->getI2C()->GetACCUstate()==false)
	{
		m_wACCU_DAT_STATE=(WORD)getModel()->getSPI()->Read_Batt_stat();
		m_wBattStatMinutes=LOBYTE(m_wACCU_DAT_STATE);
		m_wBattStatPercents=HIBYTE(m_wACCU_DAT_STATE);
		if(getModel()->getAcuLink()!=NULL)
		{
			if(IsAccuSupply()==true)
				getModel()->getAcuLink()->setParaData(ALINK_SETT_BATTERYSTATE_MINUTES,m_wBattStatMinutes);
			else
				getModel()->getAcuLink()->setParaData(ALINK_SETT_BATTERYSTATE_MINUTES,ALINK_NOTVALID);
			getModel()->getAcuLink()->setParaData(ALINK_SETT_BATTERYSTATE_PERCENT,m_wBattStatPercents);
		}
	}*///newVG
	

	if(	false==IsAccuSupply())
		return;

	

	int iHours=m_wBattStatMinutes/60;
	int iMin=m_wBattStatMinutes-(iHours*60);
	CStringW szTemp=_T("");
	szTemp.Format(_T("Battery remain %.2d:%.2d h %d%s"),iHours,iMin,m_wBattStatPercents,_T("%"));
	//getModel()->SetDebugLog(szTemp);

	if(getModel()->getVIEWHANDLER()->getViewState()==VS_STARTUP)
	{
		return;
	}



	if(bRange && !m_bI2CError)
	{
		if(m_wBattStatMinutes<1 && m_bShowBatteryEmpty)
		{
			if(getModel()->getALARMHANDLER()->ALARM_Accu_Empty->getAlarmState()==AS_NONE)
			{
				if(getModel()->getALARMHANDLER()->setAlarm(AL_Accu_Empty,_T("")))
				{
					if(isTRENDLicenseAvailable())
					{
						//TREND
						theApp.getLog()->WriteLine(_T("#HFO:0084"));
						theApp.getLog()->WriteLine(_T("#HFO:0084"));
						//rkuNEWFIX
						SerializeAllTrends(false);
					}
				}
			}

			m_bShowBatteryEmpty=false;
		}
		else if(m_wBattStatMinutes<15 && m_bShowBatteryState15)
		{
			if(		getModel()->getALARMHANDLER()->ALARM_Accu_15->getAlarmState()==AS_NONE
				&&	getModel()->getALARMHANDLER()->CanSetAlarm_Accu_15())
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_Accu_15);
			}

			m_bShowBatteryState15=false;
		}
		else if(m_wBattStatMinutes<30 && m_bShowBattery30)
		{
			if(		getModel()->getALARMHANDLER()->ALARM_Accu_30->getAlarmState()==AS_NONE
				&&	getModel()->getALARMHANDLER()->CanSetAlarm_Accu_30())
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_Accu_30);
			}

			m_bShowBattery30=false;
		}
		else if(m_wBattStatMinutes<60 && m_bShowBattery60)
		{
			if(		getModel()->getALARMHANDLER()->ALARM_Accu_60->getAlarmState()==AS_NONE
				&&	getModel()->getALARMHANDLER()->CanSetAlarm_Accu_60())
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_Accu_60);
			}

			m_bShowBattery60=false;
		}
	}
	else if(!m_bI2CError)
	{
		if(m_wBattStatMinutes<1 && m_bShowBatteryEmpty)
		{
			if(		getModel()->getALARMHANDLER()->ALARM_Accu_Empty->getAlarmState()==AS_NONE
				&&	getModel()->getALARMHANDLER()->CanSetAlarm_AccuEmpty())
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_AccuEmpty);
			}

			m_bShowBatteryEmpty=false;
		}
		else if(m_wBattStatMinutes==15 && m_bShowBatteryState15)
		{
			if(		getModel()->getALARMHANDLER()->ALARM_Accu_15->getAlarmState()==AS_NONE
				&&	getModel()->getALARMHANDLER()->CanSetAlarm_Accu_15())
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_Accu_15);
			}

			m_bShowBatteryState15=false;
		}
		else if(m_wBattStatMinutes==30 && m_bShowBattery30)
		{
			if(		getModel()->getALARMHANDLER()->ALARM_Accu_30->getAlarmState()==AS_NONE
				&&	getModel()->getALARMHANDLER()->CanSetAlarm_Accu_30())
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_Accu_30);
			}

			m_bShowBattery30=false;
		}
		else if(m_wBattStatMinutes==60 && m_bShowBattery60)
		{
			if(		getModel()->getALARMHANDLER()->ALARM_Accu_60->getAlarmState()==AS_NONE
				&&	getModel()->getALARMHANDLER()->CanSetAlarm_Accu_60())
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_Accu_60);
			}

			m_bShowBattery60=false;
		}
	}
	
	if(m_wBattStatMinutes>2 && getModel()->getALARMHANDLER()->ALARM_Accu_Empty->getAlarmState()!=AS_NONE)
	{
		getModel()->getALARMHANDLER()->deleteAlarm(AL_Accu_Empty);
	}
	
	if(m_wBattStatMinutes>63)
	{
		m_bShowBatteryEmpty=true;
		m_bShowBatteryState15=true;
		m_bShowBattery30=true;
		m_bShowBattery60=false;//es wird erst ab 30 Minuten alarmiert!
	}
	else if(m_wBattStatMinutes>33)
	{
		m_bShowBatteryEmpty=true;
		m_bShowBatteryState15=true;
		m_bShowBattery30=true;
	}
	else if(m_wBattStatMinutes>18)
	{
		m_bShowBatteryEmpty=true;
		m_bShowBatteryState15=true;
	}
	else if(m_wBattStatMinutes>2)
	{
		m_bShowBatteryEmpty=true;
	}
}

WORD CDataHandler::GetBatteryStatMinutes()
{
	return m_wBattStatMinutes;
}

WORD CDataHandler::GetBatteryStatPercent()
{
	return m_wBattStatPercents;
}
// **************************************************************************
// 
// **************************************************************************
void CDataHandler::setOpTimeHFOMin(UINT min)
{
	getModel()->getCONFIG()->setOperatingMinutesHFO(min, true);

	EnterCriticalSection(&csOpTime);
	if(IsHFOrunning())
		m_dwOpTimeHFOTickCountStarted=GetTickCount();
	else
		m_dwOpTimeHFOTickCountStarted=0;
	LeaveCriticalSection(&csOpTime);
}
DWORD CDataHandler::getOpTimeHFOMin(bool bSave)
{
	DWORD curOpMinHFO=getModel()->getCONFIG()->getOperatingMinutesHFO();
	DWORD iTempMillisec=0;
	DWORD curTick=GetTickCount();

	EnterCriticalSection(&csOpTime);
	if(m_dwOpTimeHFOTickCountStarted!=0)
	{
		if(curTick<m_dwOpTimeHFOTickCountStarted)//overrun
		{
			DWORD diff=0xFFFFFFFF-m_dwOpTimeHFOTickCountStarted;
			iTempMillisec = curTick+diff;
		}
		else
		{
			iTempMillisec = curTick-m_dwOpTimeHFOTickCountStarted;
		}
	}
	LeaveCriticalSection(&csOpTime);
	curOpMinHFO+=(iTempMillisec/60000);

	if(bSave)
		saveOpTime();

	return curOpMinHFO;
}
void CDataHandler::resetOpTimeHFO()
{
	getModel()->getCONFIG()->setOperatingMinutesHFO(0, true);
	EnterCriticalSection(&csOpTime);
	m_dwOpTimeHFOTickCountStarted=GetTickCount();
	LeaveCriticalSection(&csOpTime);
	saveOpTime();
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::setOpTimeDeviceMin(UINT min)
{
	getModel()->getCONFIG()->setOperatingMinutesDevice(min, true);

	EnterCriticalSection(&csOpTime);
	m_dwOpTimeDevTickCountStarted=GetTickCount();
	LeaveCriticalSection(&csOpTime);
}

DWORD CDataHandler::getOpTimeDeviceMin(bool bSave)
{
	DWORD curOpMinDEVICE=getModel()->getCONFIG()->getOperatingMinutesDevice();
	DWORD iTempMillisec=0;
	DWORD curTick=GetTickCount();

	EnterCriticalSection(&csOpTime);
	if(m_dwOpTimeDevTickCountStarted!=0)
	{
		if(curTick<m_dwOpTimeDevTickCountStarted)//overrun
		{
			DWORD diff=0xFFFFFFFF-m_dwOpTimeDevTickCountStarted;
			iTempMillisec = curTick+diff;
		}
		else
		{
			iTempMillisec = curTick-m_dwOpTimeDevTickCountStarted;
		}
	}
	LeaveCriticalSection(&csOpTime);
	curOpMinDEVICE+=(iTempMillisec/60000);

	if(bSave)
		saveOpTime();

	return curOpMinDEVICE;
}

void CDataHandler::resetOpTimeDevice()
{
	getModel()->getCONFIG()->setOperatingMinutesDevice(0, true);
	EnterCriticalSection(&csOpTime);
	m_dwOpTimeDevTickCountStarted=GetTickCount();
	LeaveCriticalSection(&csOpTime);
	saveOpTime();
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::setOpTimeBatteryMin(UINT min)
{
	getModel()->getCONFIG()->setOperatingMinutesBattery(min, true);

	EnterCriticalSection(&csOpTime);
	if(IsAccuSupply())
		m_dwOpTimeBattTickCountStarted=GetTickCount();
	else
		m_dwOpTimeBattTickCountStarted=0;
	LeaveCriticalSection(&csOpTime);
}
DWORD CDataHandler::getOpTimeBatteryMin(bool bSave)
{
	DWORD curOpMinBATTERY=getModel()->getCONFIG()->getOperatingMinutesBattery();

	DWORD iTempMillisec=0;
	DWORD curTick=GetTickCount();

	EnterCriticalSection(&csOpTime);
	if(m_dwOpTimeBattTickCountStarted!=0)
	{
		if(curTick<m_dwOpTimeBattTickCountStarted)//overrun
		{
			DWORD diff=0xFFFFFFFF-m_dwOpTimeBattTickCountStarted;
			iTempMillisec = curTick+diff;
		}
		else
		{
			iTempMillisec = curTick-m_dwOpTimeBattTickCountStarted;
		}
	}
	LeaveCriticalSection(&csOpTime);
	curOpMinBATTERY+=(iTempMillisec/60000);

	if(bSave)
		saveOpTime();

	return curOpMinBATTERY;
}

void CDataHandler::resetOpTimeBattery()
{
	getModel()->getCONFIG()->setOperatingMinutesBattery(0, true);
	EnterCriticalSection(&csOpTime);
	m_dwOpTimeBattTickCountStarted=GetTickCount();
	LeaveCriticalSection(&csOpTime);
	saveOpTime();
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::saveOpTime()
{
	DWORD dwOpTimeDevMin=getModel()->getCONFIG()->getOperatingMinutesDevice();
	DWORD dwOpTimeBattMin=getModel()->getCONFIG()->getOperatingMinutesBattery();
	DWORD dwOpTimeHFOMin=getModel()->getCONFIG()->getOperatingMinutesHFO();

	DWORD iTempMillisec=0;
	DWORD curTick=GetTickCount();

	EnterCriticalSection(&csOpTime);

	if(m_dwOpTimeDevTickCountStarted!=0)
	{
		//iTemp = tickCount-m_ullOpTimeDevTickCountStarted;
		if(curTick<m_dwOpTimeDevTickCountStarted)//overrun
		{
			DWORD diff=0xFFFFFFFF-m_dwOpTimeDevTickCountStarted;
			iTempMillisec = curTick+diff;
		}
		else
		{
			iTempMillisec = curTick-m_dwOpTimeDevTickCountStarted;
		}
	}
	else
		iTempMillisec=0;
	dwOpTimeDevMin+=(iTempMillisec/60000);

	if(m_dwOpTimeBattTickCountStarted!=0)
	{
		//iTemp = tickCount-m_ullOpTimeBattTickCountStarted;
		if(curTick<m_dwOpTimeBattTickCountStarted)//overrun
		{
			DWORD diff=0xFFFFFFFF-m_dwOpTimeBattTickCountStarted;
			iTempMillisec = curTick+diff;
		}
		else
		{
			iTempMillisec = curTick-m_dwOpTimeBattTickCountStarted;
		}
	}
	else
		iTempMillisec=0;
	dwOpTimeBattMin+=(iTempMillisec/60000);

	if(m_dwOpTimeHFOTickCountStarted!=0)
	{
		//iTemp = tickCount-m_ullOpTimeHFOTickCountStarted;
		if(curTick<m_dwOpTimeHFOTickCountStarted)//overrun
		{
			DWORD diff=0xFFFFFFFF-m_dwOpTimeHFOTickCountStarted;
			iTempMillisec = curTick+diff;
		}
		else
		{
			iTempMillisec = curTick-m_dwOpTimeHFOTickCountStarted;
		}
	}
	else
		iTempMillisec=0;
	dwOpTimeHFOMin+=(iTempMillisec/60000);

	m_dwOpTimeDevTickCountStarted=GetTickCount();
	if(m_dwOpTimeBattTickCountStarted!=0)
		m_dwOpTimeBattTickCountStarted=m_dwOpTimeDevTickCountStarted;
	if(m_dwOpTimeHFOTickCountStarted!=0)
		m_dwOpTimeHFOTickCountStarted=m_dwOpTimeDevTickCountStarted;
	LeaveCriticalSection(&csOpTime);

	getModel()->getCONFIG()->setOperatingMinutesDevice(dwOpTimeDevMin, true);
	getModel()->getCONFIG()->setOperatingMinutesBattery(dwOpTimeBattMin, true);
	getModel()->getCONFIG()->setOperatingMinutesHFO(dwOpTimeHFOMin, true);

	/*DEBUGMSG(TRUE, (TEXT("OpTime DEV %d %d\r\n"),(int)dwOpTimeDevMin,(int)m_dwOpTimeDevTickCountStarted));
	DEBUGMSG(TRUE, (TEXT("OpTime ACU %d %d\r\n"),(int)dwOpTimeBattMin,(int)m_dwOpTimeBattTickCountStarted));
	DEBUGMSG(TRUE, (TEXT("OpTime HFO %d %d\r\n"),(int)dwOpTimeHFOMin,(int)m_dwOpTimeHFOTickCountStarted));*/
}

// **************************************************************************
// 
// **************************************************************************
bool CDataHandler::SetAccuSupply(bool state)
{
	if(state == m_bAccuSupply)
		return false;
	else
	{
		m_bAccuSupply=state;

		if(state)
		{
			theApp.getLog()->WriteLine(_T("***SetAccuSupply(true)"));

			//check HF-amplitude
			CTlsRegistry regLimit(_T("HKCU\\Software\\FabianHFO\\WorkState\\Limits"),true);
			WORD iMAXRANGE_NEO_HFAMPL=regLimit.ReadDWORD(_T("MAXRANGE_ACCU_HFAMPL"), MAXRANGE_ACCU_HFAMPL);
			WORD iMAXRANGE_PED_HFAMPL=regLimit.ReadDWORD(_T("MAXRANGE_ACCU_HFAMPL"), MAXRANGE_ACCU_HFAMPL);

			PARADATA()->SetHFAmplMaxParaNEONATAL(iMAXRANGE_NEO_HFAMPL);
			PARADATA()->SetHFAmplMaxParaPEDIATRIC(iMAXRANGE_PED_HFAMPL);
		}
		else
		{
			//check HF-amplitude
			CTlsRegistry regLimit(_T("HKCU\\Software\\FabianHFO\\WorkState\\Limits"),true);
			WORD iMAXRANGE_NEO_HFAMPL=regLimit.ReadDWORD(_T("MAXRANGE_NEO_HFAMPL"), MAXRANGE_NEO_HFAMPL);
			WORD iMAXRANGE_PED_HFAMPL=regLimit.ReadDWORD(_T("MAXRANGE_PED_HFAMPL"), MAXRANGE_PED_HFAMPL);
			PARADATA()->SetHFAmplMaxParaNEONATAL(iMAXRANGE_NEO_HFAMPL);
			PARADATA()->SetHFAmplMaxParaPEDIATRIC(iMAXRANGE_PED_HFAMPL);

			theApp.getLog()->WriteLine(_T("***SetAccuSupply(false)"));
		}

		EnterCriticalSection(&csOpTime);
		DWORD dwTempOpTime=m_dwOpTimeBattTickCountStarted;
		LeaveCriticalSection(&csOpTime);

		if(dwTempOpTime==0 && state==true)
		{
			EnterCriticalSection(&csOpTime);
			m_dwOpTimeBattTickCountStarted = GetTickCount();
			LeaveCriticalSection(&csOpTime);
		}
		else
		{
			saveOpTime();

			EnterCriticalSection(&csOpTime);
			m_dwOpTimeBattTickCountStarted = 0;
			LeaveCriticalSection(&csOpTime);
		}

		if(getModel()->getALARMHANDLER()->deleteAkkuAlarms())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
		}

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_POWERSTATE_CHANGED);

		return true;
	}
}

// **************************************************************************
// 
// **************************************************************************
bool CDataHandler::IsAccuSupply()
{
	return m_bAccuSupply;
}


// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetMSTATUS_ConductorPIC(int state)
{
	m_iMSTATUS_ConPIC=state;
}

int CDataHandler::GetMSTATUS_ConductorPIC()
{
	return m_iMSTATUS_ConPIC;
}

// **************************************************************************
// Die Codes vom Mischer:  
// 1            Luft Regelung gestört
// 2            Oxi Regelung gestört
// 4            Kommunikation zum Ctl-PIC Checksum fail
// 8            Hitzdraht Luft defekt
// 16           Hitzdraht Oxi defekt
// 32           FRAM / Memory Checksum fail
// 64           Zero Kal fail
// 128 oder 160        FRAM fail
//
// Sample:
// 65 setzt sich zusammen aus 64 + 1:
// --> 64: Zero fail -> d.h. Nullpunkt kalibrierung fehlgeschlagen
// --> 1:  Luft Regelung gestört  -> Flow wird nicht erreicht
//
// **************************************************************************
void CDataHandler::SetMSTATUS_BlenderPIC(int state)
{
	m_iMSTATUS_BlenderPIC=state;
}

int CDataHandler::GetMSTATUS_BlenderPIC()
{
	return m_iMSTATUS_BlenderPIC;
}

// **************************************************************************
// 
// **************************************************************************
UINT CDataHandler::CheckLastTrendData()
{
	EnterCriticalSection(&csTrendUpdate);
	m_bTrendUpdateRunning=true;
	LeaveCriticalSection(&csTrendUpdate);
	DEBUGMSG(TRUE, (TEXT("CheckLastTrendData start\r\n")));
	//rkuTOCHECKFIX
	//DWORD dwStart=GetTickCount();
	int iRes=0;

	COleDateTime dtTimeLastTrendPINSP = m_cTendPINSP.GetTimestamp();
	COleDateTime dtTimeLastTrendPMEAN = m_cTendPMEAN.GetTimestamp();
	COleDateTime dtTimeLastTrendFIO2 = m_cTendFIO2.GetTimestamp();
	COleDateTime dtTimeLastTrendVTE = m_cTendVTE.GetTimestamp();
	COleDateTime dtTimeLastTrendCOMPLIANCE = m_cTendCOMPLIANCE.GetTimestamp();
	COleDateTime dtTimeLastTrendCO2HFO = m_cTendCO2HFO.GetTimestamp();
	COleDateTime dtTimeLastTrendMV = m_cTendMV.GetTimestamp();
	COleDateTime dtTimeLastTrendHFAMP = m_cTendHFAMP.GetTimestamp();
	COleDateTime dtTimeLastTrendRSBI = m_cTendRSBI.GetTimestamp();
	COleDateTime dtTimeLastTrendShareMVmand = m_cTendShareMVmand.GetTimestamp();
	COleDateTime dtTimeLastTrendResistance = m_cTendResistance.GetTimestamp();
	COleDateTime dtTimeLastTrendLeak = m_cTendLeak.GetTimestamp();
	COleDateTime dtTimeLastTrendSpO2 = m_cTendSpO2.GetTimestamp();
	COleDateTime dtTimeLastTrendSpO2PI = m_cTendSpO2PI.GetTimestamp();
	COleDateTime dtTimeLastTrendEtCO2 = m_cTendEtCO2.GetTimestamp();
	COleDateTime dtTimeLastTrendSpO2PR = m_cTendSpO2PR.GetTimestamp();
	COleDateTime dtTimeLastTrendFrequency = m_cTendFrequency.GetTimestamp();


	SYSTEMTIME st;
	GetLocalTime(&st);
	COleDateTime dtTime(st);

	EnterCriticalSection(&csTrend);
	if(dtTimeLastTrendPINSP.GetStatus() != COleDateTime::null)
	{
		iRes=UpdateTrendData(TREND_PINSP, dtTime);
		Sleep(0);
	}
	if(dtTimeLastTrendPMEAN.GetStatus() != COleDateTime::null && iRes>=0)
	{
		iRes=UpdateTrendData(TREND_PMEAN, dtTime);
		Sleep(0);
	}
	if(dtTimeLastTrendFIO2.GetStatus() != COleDateTime::null && iRes>=0)
	{
		iRes=UpdateTrendData(TREND_FIO2, dtTime);
		Sleep(0);
	}
	if(dtTimeLastTrendVTE.GetStatus() != COleDateTime::null && iRes>=0)
	{
		iRes=UpdateTrendData(TREND_VTE, dtTime);
		Sleep(0);
	}
	if(dtTimeLastTrendCOMPLIANCE.GetStatus() != COleDateTime::null && iRes>=0)
	{
		iRes=UpdateTrendData(TREND_COMPLIANCE, dtTime);
		Sleep(0);
	}
	if(dtTimeLastTrendCO2HFO.GetStatus() != COleDateTime::null && iRes>=0)
	{
		iRes=UpdateTrendData(TREND_CO2HFO, dtTime);
		Sleep(0);
	}
	if(dtTimeLastTrendMV.GetStatus() != COleDateTime::null && iRes>=0)
	{
		iRes=UpdateTrendData(TREND_MV, dtTime);
		Sleep(0);
	}
	if(dtTimeLastTrendHFAMP.GetStatus() != COleDateTime::null && iRes>=0)
	{
		iRes=UpdateTrendData(TREND_HFAMP, dtTime);
		Sleep(0);
	}
	if(dtTimeLastTrendRSBI.GetStatus() != COleDateTime::null && iRes>=0)
	{
		iRes=UpdateTrendData(TREND_RSBI, dtTime);
		Sleep(0);
	}
	if(dtTimeLastTrendShareMVmand.GetStatus() != COleDateTime::null && iRes>=0)
	{
		iRes=UpdateTrendData(TREND_SHAREMVMAND, dtTime);
		Sleep(0);
	}
	if(dtTimeLastTrendResistance.GetStatus() != COleDateTime::null && iRes>=0)
	{
		iRes=UpdateTrendData(TREND_RESISTANCE, dtTime);
		Sleep(0);
	}
	if(dtTimeLastTrendLeak.GetStatus() != COleDateTime::null && iRes>=0)
	{
		iRes=UpdateTrendData(TREND_LEAK, dtTime);
		Sleep(0);
	}
	if(dtTimeLastTrendSpO2.GetStatus() != COleDateTime::null && iRes>=0)
	{
		iRes=UpdateTrendData(TREND_SPO2, dtTime);
		Sleep(0);
	}
	if(dtTimeLastTrendSpO2PI.GetStatus() != COleDateTime::null && iRes>=0)
	{
		iRes=UpdateTrendData(TREND_SPO2PI, dtTime);
		Sleep(0);
	}
	if(dtTimeLastTrendEtCO2.GetStatus() != COleDateTime::null && iRes>=0)
	{
		iRes=UpdateTrendData(TREND_ETCO2, dtTime);
		Sleep(0);
	}
	if(dtTimeLastTrendSpO2PR.GetStatus() != COleDateTime::null && iRes>=0)
	{
		iRes=UpdateTrendData(TREND_SPO2PR, dtTime);
		Sleep(0);
	}
	if(dtTimeLastTrendFrequency.GetStatus() != COleDateTime::null && iRes>=0)
	{
		iRes=UpdateTrendData(TREND_FREQUENCY, dtTime);
		Sleep(0);
	}
	LeaveCriticalSection(&csTrend);

	//DWORD dwEnd=GetTickCount();
	//DEBUGMSG(TRUE, (TEXT("CheckLastTrendData time %d\r\n"),dwEnd-dwStart));
	//DEBUGMSG(TRUE, (TEXT("CheckLastTrendData end \r\n")));

	if(iRes<0)
		iRes=0;

	EnterCriticalSection(&csTrendUpdate);
	m_bTrendUpdateRunning=false;
	LeaveCriticalSection(&csTrendUpdate);

	return iRes;
}

// **************************************************************************
// 
// **************************************************************************
int CDataHandler::UpdateTrendData(UINT type, COleDateTime dtTime)
{
	int iResult=0;

	COleDateTime dtTimeLastTrend;
	dtTimeLastTrend.SetStatus(COleDateTime::null);

	CTrend* m_cpTend=NULL;

	//EnterCriticalSection(&csTrend); //rkuNEWFIX

	switch(type)
	{
	case TREND_PINSP:
		{
			m_cpTend=&m_cTendPINSP;
		}
		break;
	case TREND_PMEAN:
		{
			m_cpTend=&m_cTendPMEAN;
		}
		break;
	case TREND_FIO2:
		{
			m_cpTend=&m_cTendFIO2;
		}
		break;
	case TREND_VTE:
		{
			m_cpTend=&m_cTendVTE;
		}
		break;
	case TREND_COMPLIANCE:
		{
			m_cpTend=&m_cTendCOMPLIANCE;
		}
		break;
	case TREND_CO2HFO:
		{
			m_cpTend=&m_cTendCO2HFO;
		}
		break;
	case TREND_MV:
		{
			m_cpTend=&m_cTendMV;
		}
		break;
	case TREND_HFAMP:
		{
			m_cpTend=&m_cTendHFAMP;
		}
		break;
	case TREND_RSBI:
		{
			m_cpTend=&m_cTendRSBI;
		}
		break;
	case TREND_SHAREMVMAND:
		{
			m_cpTend=&m_cTendShareMVmand;
		}
		break;
	case TREND_RESISTANCE:
		{
			m_cpTend=&m_cTendResistance;
		}
		break;
	case TREND_LEAK:
		{
			m_cpTend=&m_cTendLeak;
		}
		break;
	case TREND_SPO2:
		{
			m_cpTend=&m_cTendSpO2;
		}
		break;
	case TREND_SPO2PR:
		{
			m_cpTend=&m_cTendSpO2PR;
		}
		break;
	case TREND_SPO2PI:
		{
			m_cpTend=&m_cTendSpO2PI;
		}
		break;
	case TREND_ETCO2:
		{
			m_cpTend=&m_cTendEtCO2;
		}
		break;
	case TREND_FREQUENCY:
		{
			m_cpTend=&m_cTendFrequency;
		}
		break;
	}

	dtTimeLastTrend = m_cpTend->GetTimestamp();

	CStringW szTime = dtTimeLastTrend.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);

	DEBUGMSG(TRUE, (TEXT("UpdateTrendData trend %d\r\n"),(int)type));

	if(	dtTimeLastTrend.GetStatus() != COleDateTime::null)
	{
		COleDateTimeSpan tsFillup;

		UINT secTotal=TRENDTIME_SEC*SERIALZEBUFFER;
		COleDateTimeSpan tsFILE(0, 0, 0, secTotal);

		COleDateTimeSpan ts = dtTime - dtTimeLastTrend;
		//eTrendTimespanToDelete enMaxTime = TTTD_NEVER;	 
		double dbTotalSeconds = ts.GetTotalSeconds();

		//rkuNEWTREND1
		int iCount=(dbTotalSeconds/TRENDTIME_SEC);


		if(		(dbTotalSeconds<0)
			||	(iCount>MAXSIZE_TREND_BUFFER)
			/*||	((dbTotalSeconds > enMaxTime) &&	(TTTD_NEVER != enMaxTime))*/)
		{
			theApp.getLog()->WriteLine(_T("#HFO:0085"));
			DeleteAllTrendData();
			iResult=-1;
		}
		else
		{
			//int iCount=(dbTotalSeconds/TRENDTIME_SEC);//rkuNEWTREND1
			iResult=dbTotalSeconds-(iCount*TRENDTIME_SEC);//remaining time until next trenddata need to be written

			DEBUGMSG(TRUE, (TEXT("UpdateTrendData %d: %d\r\n"),iCount,iResult));

			if(iCount>MAXSIZE_TREND_BUFFER)
				iCount=MAXSIZE_TREND_BUFFER;

			if(iCount>0)
			{
				WORD iCurBufSize=m_cpTend->GetBufferCount();
				WORD wBufCount=0;

				if(iCurBufSize+iCount<SERIALZEBUFFER)
				{
					try
					{
						for(int i=0;i<iCount;i++)
						{
							wBufCount = m_cpTend->WriteBuffer(0,dtTime);

							if(wBufCount>=SERIALZEBUFFER)//darf hier nie passieren!!! sonst stimmt Berechnung nicht
							{
								dtTimeLastTrend+=tsFillup;
								if(dtTimeLastTrend.GetStatus()== COleDateTime::valid)
								{
									m_cpTend->SetTimestamp(dtTimeLastTrend);
								}
								SerializeTrend(type,true);	
							}
							Sleep(0);
						}
					}
					catch (...)
					{
						theApp.ReportException(_T("CDataHandler::UpdateTrendData1"));
					}
				}
				else
				{
					//restlichen aktuellen buffer mit Nullen füllen
					if(		iCurBufSize>0 
						&&	iCurBufSize<SERIALZEBUFFER)
					{
						try
						{
							for(int i=iCurBufSize;i<SERIALZEBUFFER;i++)
							{
								wBufCount = m_cpTend->WriteBuffer(0,dtTime);

								if(wBufCount>=SERIALZEBUFFER)
								{
									dtTimeLastTrend+=tsFillup;
									if(dtTimeLastTrend.GetStatus()== COleDateTime::valid)
									{
										m_cpTend->SetTimestamp(dtTimeLastTrend);
									}

									SerializeTrend(type,true);
								}
								Sleep(0);
							}
						}
						catch (...)
						{
							theApp.ReportException(_T("CDataHandler::UpdateTrendData2"));
						}
					}

					wBufCount=0;

					int iCompleteNullFiles=(iCount-(SERIALZEBUFFER-iCurBufSize))/SERIALZEBUFFER;
					tsFillup.SetDateTimeSpan(0, 0, 0, TRENDTIME_SEC*(SERIALZEBUFFER-iCurBufSize));

					if(iCompleteNullFiles>0)
					{
						m_cpTend->ResetBuffer();

						try
						{
							for(int i=0;i<iCompleteNullFiles;i++)
							{
								m_cpTend->SetBufferCount(SERIALZEBUFFER);

								dtTimeLastTrend+=tsFILE;
								if(dtTimeLastTrend.GetStatus()== COleDateTime::valid)
								{
									m_cpTend->SetTimestamp(dtTimeLastTrend);
								}

								SerializeTrend(type,true);
								Sleep(0);
							}
						}
						catch (...)
						{
							theApp.ReportException(_T("CDataHandler::UpdateTrendData3"));
						}
					}

					wBufCount = 0;

					int iRest=iCount-(iCompleteNullFiles*SERIALZEBUFFER)-(SERIALZEBUFFER-iCurBufSize);

					try
					{
						for(int i=0;i<iRest;i++)
						{
							wBufCount = m_cpTend->WriteBuffer(0,dtTime);
							if(wBufCount>=SERIALZEBUFFER)
							{
								SerializeTrend(type,true);
							}
							Sleep(0);
						}
					}
					catch (...)
					{
						theApp.ReportException(_T("CDataHandler::UpdateTrendData4"));
					}
				}
			}
		}
	}

	
	//LeaveCriticalSection(&csTrend); //rkuNEWFIX

	return iResult;
	
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetInspFlowData(int valueInsp)
{
	setMessureDataAVG(ALINK_MSMNT_IFLOW,valueInsp);
	setMessureDataBTB(ALINK_MSMNT_IFLOW,valueInsp);

	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getVMODEHANDLER()->activeModeIsTHERAPY())
			getModel()->getAcuLink()->setMeasurementData(ALINK_MSMNT_IFLOW,ALINK_NOTVALID);
		else
			getModel()->getAcuLink()->setMeasurementData(ALINK_MSMNT_IFLOW,valueInsp);
	}
	
}
void CDataHandler::SetExpFlowData(int valueExp)
{
	setMessureDataAVG(ALINK_MSMNT_EFLOW,valueExp);
	setMessureDataBTB(ALINK_MSMNT_EFLOW,valueExp);

	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setMeasurementData(ALINK_MSMNT_EFLOW,valueExp);
	
}
int CDataHandler::GetInspFlowData()
{
	return getMessureDataAVG(ALINK_MSMNT_IFLOW);
}
int CDataHandler::GetExpFlowData()
{
	return getMessureDataAVG(ALINK_MSMNT_EFLOW);
}
// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetDemandFlowData(int valueDem)
{
	setMessureDataAVG(ALINK_MSMNT_DEMAND_FLOW,valueDem);
	setMessureDataBTB(ALINK_MSMNT_DEMAND_FLOW,valueDem);

	if(getModel()->getAcuLink()!=NULL)
		getModel()->getAcuLink()->setMeasurementData(ALINK_MSMNT_DEMAND_FLOW,valueDem);
	
}
int CDataHandler::GetDemandFlowData()
{
	return getMessureDataAVG(ALINK_MSMNT_DEMAND_FLOW);
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SaveTrendData()
{
	//DEBUGMSG(TRUE, (TEXT("************SaveTrendData\r\n")));
	EnterCriticalSection(&csTrendFileData);
	int iTrendCnt=m_iTrendCnt;
	double fTrendData_Pmean=m_fTrendData_Pmean;
	double fTrendData_FiO2=m_fTrendData_FiO2;
	double fTrendData_Pinsp=m_fTrendData_Pinsp;
	double fTrendData_Vte=m_fTrendData_Vte;
	double fTrendData_Compliance=m_fTrendData_Compliance;
	double fTrendData_CO2HFO=m_fTrendData_CO2HFO;
	double fTrendData_MV=m_fTrendData_MV;
	double fTrendData_HFAMP=m_fTrendData_HFAMP;
	double fTrendData_RSBI=m_fTrendData_RSBI;
	double fTrendData_ShareMVmand=m_fTrendData_ShareMVmand;
	double fTrendData_Resistance=m_fTrendData_Resistance;
	double fTrendData_Leak=m_fTrendData_Leak;
	double fTrendData_SpO2=m_fTrendData_SpO2;
	double fTrendData_PI=m_fTrendData_PI;
	double fTrendData_etCO2=m_fTrendData_etCO2;
	double fTrendData_SpO2PR=m_fTrendData_SpO2PR;
	double fTrendData_Frequency=m_fTrendData_Frequency;

	m_iTrendCnt=0;
	m_fTrendData_Pmean=0;
	m_fTrendData_FiO2=0;
	m_fTrendData_Pinsp=0;
	m_fTrendData_Vte=0;
	m_fTrendData_Compliance=0;
	m_fTrendData_CO2HFO=0;
	m_fTrendData_MV=0;
	m_fTrendData_HFAMP=0;
	m_fTrendData_RSBI=0;
	m_fTrendData_ShareMVmand=0;
	m_fTrendData_Resistance=0;
	m_fTrendData_Leak=0;
	m_fTrendData_SpO2=0;
	m_fTrendData_PI=0;
	m_fTrendData_etCO2=0;
	m_fTrendData_SpO2PR=0;
	m_fTrendData_Frequency=0;

	LeaveCriticalSection(&csTrendFileData);

	WORD wTrendData_Pinsp=0;
	short sTrendData_Pmean=0;
	WORD wTrendData_FiO2=0;
	WORD wTrendData_Vte=0;
	WORD wTrendData_Compliance=0;
	WORD wTrendData_CO2HFO=0;
	WORD wTrendData_MV=0;
	WORD wTrendData_HFAMP=0;
	WORD wTrendData_RSBI=0;
	WORD wTrendData_ShareMVmand=0;
	WORD wTrendData_Resistance=0;
	WORD wTrendData_Leak=0;
	WORD wTrendData_SpO2=0;
	WORD wTrendData_PI=0;
	WORD wTrendData_etCO2=0;
	WORD wTrendData_SpO2PR=0;
	WORD wTrendData_Frequency=0;

	if(iTrendCnt>0)
	{
		wTrendData_Pinsp=fTrendData_Pinsp/iTrendCnt;//(fTrendData_Pinsp/iTrendCnt)/10;
		sTrendData_Pmean=fTrendData_Pmean/iTrendCnt;
		wTrendData_FiO2=fTrendData_FiO2/iTrendCnt;//(fTrendData_FiO2/iTrendCnt)/10;
		wTrendData_Vte=fTrendData_Vte/iTrendCnt;
		wTrendData_Compliance=fTrendData_Compliance/iTrendCnt;
		wTrendData_CO2HFO=fTrendData_CO2HFO/iTrendCnt;
		wTrendData_MV=fTrendData_MV/iTrendCnt;
		wTrendData_HFAMP=fTrendData_HFAMP/iTrendCnt;

		wTrendData_RSBI=fTrendData_RSBI/iTrendCnt;
		wTrendData_ShareMVmand=fTrendData_ShareMVmand/iTrendCnt;
		wTrendData_Resistance=fTrendData_Resistance/iTrendCnt;
		wTrendData_Leak=fTrendData_Leak/iTrendCnt;
		wTrendData_SpO2=fTrendData_SpO2/iTrendCnt;
		wTrendData_PI=fTrendData_PI/iTrendCnt;
		wTrendData_etCO2=fTrendData_etCO2/iTrendCnt;
		wTrendData_SpO2PR=fTrendData_SpO2PR/iTrendCnt;
		wTrendData_Frequency=fTrendData_Frequency/iTrendCnt;
	}

	//function to save trend temporary if trend update is still running
	EnterCriticalSection(&csTrendUpdate);
	bool bUpdateRunnig=m_bTrendUpdateRunning;
	LeaveCriticalSection(&csTrendUpdate);

	if(bUpdateRunnig)
	{
		DEBUGMSG(TRUE, (TEXT("************TREND UPDATE RUNNING -> save temp\r\n")));
		m_bInitialSaveTrend=true;
		iTrendCnt=0;

		m_wTempTrendData_Pinsp=wTrendData_Pinsp;
		m_sTempTrendData_Pmean=sTrendData_Pmean;
		m_wTempTrendData_FiO2=wTrendData_FiO2;
		m_wTempTrendData_Vte=wTrendData_Vte;
		m_wTempTrendData_Compliance=wTrendData_Compliance;
		m_wTempTrendData_CO2HFO=wTrendData_CO2HFO;
		m_wTempTrendData_MV=wTrendData_MV;
		m_wTempTrendData_HFAMP=wTrendData_HFAMP;
		m_wTempTrendData_RSBI=wTrendData_RSBI;
		m_wTempTrendData_ShareMVmand=wTrendData_ShareMVmand;
		m_wTempTrendData_Resistance=wTrendData_Resistance;
		m_wTempTrendData_Leak=wTrendData_Leak;
		m_wTempTrendData_SpO2=wTrendData_SpO2;
		m_wTempTrendData_PI=wTrendData_PI;
		m_wTempTrendData_etCO2=wTrendData_etCO2;
		m_wTempTrendData_SpO2PR=wTrendData_SpO2PR;
		m_wTempTrendData_Frequency=wTrendData_Frequency;
		SYSTEMTIME st;
		GetLocalTime(&st);
		m_dtTempTimestamp=COleDateTime(st);
	}
	else if(m_bInitialSaveTrend)
	{
		m_bInitialSaveTrend=false;

		saveTempTrends();	
	}

	CStringW szVal=_T("");
	

	bool bSerialize=false;
	if((iTrendCnt>0) /*&& bRunning*/)
	{

		WORD wBufCount = 0;

		SYSTEMTIME st;
		GetLocalTime(&st);
		COleDateTime dtTimestamp=COleDateTime(st);

		EnterCriticalSection(&csTrend);//rkuNEWFIX
		wBufCount = m_cTendPINSP.WriteBuffer(wTrendData_Pinsp, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}

		wBufCount = m_cTendPMEAN.WriteBuffer(sTrendData_Pmean, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}

		wBufCount = m_cTendFIO2.WriteBuffer(wTrendData_FiO2, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}

		wBufCount = m_cTendVTE.WriteBuffer(wTrendData_Vte, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}

		wBufCount = m_cTendCOMPLIANCE.WriteBuffer(wTrendData_Compliance, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}

		wBufCount = m_cTendCO2HFO.WriteBuffer(wTrendData_CO2HFO, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}

		wBufCount = m_cTendMV.WriteBuffer(wTrendData_MV, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}

		wBufCount = m_cTendHFAMP.WriteBuffer(wTrendData_HFAMP, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}

		wBufCount = m_cTendRSBI.WriteBuffer(wTrendData_RSBI, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}

		wBufCount = m_cTendShareMVmand.WriteBuffer(wTrendData_ShareMVmand, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}

		wBufCount = m_cTendResistance.WriteBuffer(wTrendData_Resistance, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}

		wBufCount = m_cTendLeak.WriteBuffer(wTrendData_Leak, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}

		wBufCount = m_cTendSpO2.WriteBuffer(wTrendData_SpO2, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}

		wBufCount = m_cTendSpO2PI.WriteBuffer(wTrendData_PI, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}

		wBufCount = m_cTendEtCO2.WriteBuffer(wTrendData_etCO2, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}

		wBufCount = m_cTendSpO2PR.WriteBuffer(wTrendData_SpO2PR, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}

		wBufCount = m_cTendFrequency.WriteBuffer(wTrendData_Frequency, dtTimestamp);
		if(wBufCount>=SERIALZEBUFFER)
		{
			bSerialize=true;
		}
		LeaveCriticalSection(&csTrend);//rkuNEWFIX

		if(bSerialize)
		{
			//rkuNEWFIX
			SerializeAllTrends(true);
		}
	}
}

void CDataHandler::saveTempTrends()
{
	WORD wBufCount = 0;
	bool bSerialize=false;

	EnterCriticalSection(&csTrend);
	wBufCount = m_cTendPINSP.WriteBuffer(m_wTempTrendData_Pinsp, m_dtTempTimestamp);
	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}

	wBufCount = m_cTendPMEAN.WriteBuffer(m_sTempTrendData_Pmean, m_dtTempTimestamp);
	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}

	wBufCount = m_cTendFIO2.WriteBuffer(m_wTempTrendData_FiO2, m_dtTempTimestamp);
	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}

	wBufCount = m_cTendVTE.WriteBuffer(m_wTempTrendData_Vte, m_dtTempTimestamp);
	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}

	wBufCount = m_cTendCOMPLIANCE.WriteBuffer(m_wTempTrendData_Compliance, m_dtTempTimestamp);
	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}

	wBufCount = m_cTendCO2HFO.WriteBuffer(m_wTempTrendData_CO2HFO, m_dtTempTimestamp);
	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}

	wBufCount = m_cTendMV.WriteBuffer(m_wTempTrendData_MV, m_dtTempTimestamp);
	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}

	wBufCount = m_cTendHFAMP.WriteBuffer(m_wTempTrendData_HFAMP, m_dtTempTimestamp);
	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}

	wBufCount = m_cTendRSBI.WriteBuffer(m_wTempTrendData_RSBI, m_dtTempTimestamp);
	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}

	wBufCount = m_cTendShareMVmand.WriteBuffer(m_wTempTrendData_ShareMVmand, m_dtTempTimestamp);
	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}

	wBufCount = m_cTendResistance.WriteBuffer(m_wTempTrendData_Resistance, m_dtTempTimestamp);
	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}

	wBufCount = m_cTendLeak.WriteBuffer(m_wTempTrendData_Leak, m_dtTempTimestamp);
	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}

	wBufCount = m_cTendSpO2.WriteBuffer(m_wTempTrendData_SpO2, m_dtTempTimestamp);
	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}

	wBufCount = m_cTendSpO2PI.WriteBuffer(m_wTempTrendData_PI, m_dtTempTimestamp);
	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}

	wBufCount = m_cTendEtCO2.WriteBuffer(m_wTempTrendData_etCO2, m_dtTempTimestamp);
	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}

	wBufCount = m_cTendSpO2PR.WriteBuffer(m_wTempTrendData_SpO2PR, m_dtTempTimestamp);
	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}

	wBufCount = m_cTendFrequency.WriteBuffer(m_wTempTrendData_Frequency, m_dtTempTimestamp);

	if(wBufCount>=SERIALZEBUFFER)
	{
		bSerialize=true;
	}
	LeaveCriticalSection(&csTrend);

	if(bSerialize)
	{
		SerializeAllTrends(true);
	}

	m_sTempTrendData_Pmean=0;
	m_wTempTrendData_Pinsp=0;
	m_wTempTrendData_FiO2=0;
	m_wTempTrendData_Vte=0;
	m_wTempTrendData_Compliance=0;
	m_wTempTrendData_CO2HFO=0;
	m_wTempTrendData_MV=0;
	m_wTempTrendData_HFAMP=0;
	m_wTempTrendData_RSBI=0;
	m_wTempTrendData_ShareMVmand=0;
	m_wTempTrendData_Resistance=0;
	m_wTempTrendData_Leak=0;
	m_wTempTrendData_SpO2=0;
	m_wTempTrendData_PI=0;
	m_wTempTrendData_etCO2=0;
	m_wTempTrendData_SpO2PR=0;
	m_wTempTrendData_Frequency=0;
}
void CDataHandler::SerializeTrend(UINT type, bool bIncreaseFileNum)
{
	CStringW szFile=_T("");

	CTrend* m_cpTend=NULL;
	BYTE chFilenum=0;
	CString szTrendFolder=_T("\\sdcard\\");
	if(CTlsFile::Exists(szTrendFolder)==false)
	{
		szTrendFolder=_T("\\FFSDISK\\");
	}


	switch(type)
	{
	case TREND_PINSP:
		{
			m_cpTend=&m_cTendPINSP;
			chFilenum=m_chFilenumPINSP;
			szTrendFolder+=IDS_TRD_FOLDER_PINSP;
		}
		break;
	case TREND_PMEAN:
		{
			m_cpTend=&m_cTendPMEAN;
			chFilenum=m_chFilenumPMEAN;
			szTrendFolder+=IDS_TRD_FOLDER_PMEAN;
		}
		break;
	case TREND_FIO2:
		{
			m_cpTend=&m_cTendFIO2;
			chFilenum=m_chFilenumFIO2;
			szTrendFolder+=IDS_TRD_FOLDER_FIO2;
		}
		break;
	case TREND_VTE:
		{
			m_cpTend=&m_cTendVTE;
			chFilenum=m_chFilenumVTE;
			szTrendFolder+=IDS_TRD_FOLDER_VTE;
		}
		break;
	case TREND_COMPLIANCE:
		{
			m_cpTend=&m_cTendCOMPLIANCE;
			chFilenum=m_chFilenumCOMPLIANCE;
			szTrendFolder+=IDS_TRD_FOLDER_COMPLIANCE;
		}
		break;
	case TREND_CO2HFO:
		{
			m_cpTend=&m_cTendCO2HFO;
			chFilenum=m_chFilenumCO2HFO;
			szTrendFolder+=IDS_TRD_FOLDER_CO2HFO;
		}
		break;
	case TREND_MV:
		{
			m_cpTend=&m_cTendMV;
			chFilenum=m_chFilenumMV;
			szTrendFolder+=IDS_TRD_FOLDER_MV;
		}
		break;
	case TREND_HFAMP:
		{
			m_cpTend=&m_cTendHFAMP;
			chFilenum=m_chFilenumHFAMP;
			szTrendFolder+=IDS_TRD_FOLDER_HFAMP;
		}
		break;
	case TREND_RSBI:
		{
			m_cpTend=&m_cTendRSBI;
			chFilenum=m_chFilenumRSBI;
			szTrendFolder+=IDS_TRD_FOLDER_RSBI;
		}
		break;
	case TREND_SHAREMVMAND:
		{
			m_cpTend=&m_cTendShareMVmand;
			chFilenum=m_chFilenumShareMVmand;
			szTrendFolder+=IDS_TRD_FOLDER_SHAREMVMAND;
		}
		break;
	case TREND_RESISTANCE:
		{
			m_cpTend=&m_cTendResistance;
			chFilenum=m_chFilenumResistance;
			szTrendFolder+=IDS_TRD_FOLDER_RESISTANCE;
		}
		break;
	case TREND_LEAK:
		{
			m_cpTend=&m_cTendLeak;
			chFilenum=m_chFilenumLeak;
			szTrendFolder+=IDS_TRD_FOLDER_LEAK;
		}
		break;
	case TREND_SPO2:
		{
			m_cpTend=&m_cTendSpO2;
			chFilenum=m_chFilenumSpO2;
			szTrendFolder+=IDS_TRD_FOLDER_SPO2;
		}
		break;
	case TREND_SPO2PR:
		{
			m_cpTend=&m_cTendSpO2PR;
			chFilenum=m_chFilenumSpO2PR;
			szTrendFolder+=IDS_TRD_FOLDER_SPO2PR;
		}
		break;
	case TREND_SPO2PI:
		{
			m_cpTend=&m_cTendSpO2PI;
			chFilenum=m_chFilenumSPO2PI;
			szTrendFolder+=IDS_TRD_FOLDER_SPO2PI;
		}
		break;
	case TREND_ETCO2:
		{
			m_cpTend=&m_cTendEtCO2;
			chFilenum=m_chFilenumEtCO2;
			szTrendFolder+=IDS_TRD_FOLDER_ETCO2;
		}
		break;
	case TREND_FREQUENCY:
		{
			m_cpTend=&m_cTendFrequency;
			chFilenum=m_chFilenumFrequency;
			szTrendFolder+=IDS_TRD_FOLDER_FREQUENCY;
		}
		break;
	}


	szFile.Format(_T("%s%d%s"),szTrendFolder,chFilenum, IDS_TRD_FILE_ID);
	CFile archivDatei;
	CFileException ex;

	try
	{
		if(archivDatei.Open(szFile,CFile::modeCreate | CFile::modeWrite, &ex)!=0)
		{
			CArchive objektArchiv(&archivDatei, CArchive::store);

			m_cpTend->Serialize(objektArchiv);

			objektArchiv.Close();
			archivDatei.Close();

			//zuletzt serialisierte Dateinummer im FRAM speichern
			getModel()->getCONFIG()->SetLastWrittenTrendFile(type, chFilenum);

			if(bIncreaseFileNum)
			{
				chFilenum++;

				if(chFilenum>MAXTRENDFILES)
					chFilenum=1;

				m_cpTend->ResetBuffer();
			}
		}
		else
		{
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);

			CStringW strFormatted;
			strFormatted = _T("#HFO:0093: ");
			strFormatted += szFile;
			strFormatted += _T(" ");
			strFormatted += szError;

			theApp.getLog()->WriteLine(strFormatted);
		}
	}
	catch (...)
	{
		archivDatei.Close();

		theApp.ReportException(_T("CDataHandler::SerializeTrend"));
	}

	switch(type)
	{
	case TREND_PINSP:
		{
			m_chFilenumPINSP=chFilenum;
		}
		break;
	case TREND_PMEAN:
		{
			m_chFilenumPMEAN=chFilenum;
		}
		break;
	case TREND_FIO2:
		{
			m_chFilenumFIO2=chFilenum;
		}
		break;
	case TREND_VTE:
		{
			m_chFilenumVTE=chFilenum;
		}
		break;
	case TREND_COMPLIANCE:
		{
			m_chFilenumCOMPLIANCE=chFilenum;
		}
		break;
	case TREND_CO2HFO:
		{
			m_chFilenumCO2HFO=chFilenum;
		}
		break;
	case TREND_MV:
		{
			m_chFilenumMV=chFilenum;
		}
		break;
	case TREND_HFAMP:
		{
			m_chFilenumHFAMP=chFilenum;
		}
		break;
	case TREND_RSBI:
		{
			m_chFilenumRSBI=chFilenum;
		}
		break;
	case TREND_SHAREMVMAND:
		{
			m_chFilenumShareMVmand=chFilenum;
		}
		break;
	case TREND_RESISTANCE:
		{
			m_chFilenumResistance=chFilenum;
		}
		break;
	case TREND_LEAK:
		{
			m_chFilenumLeak=chFilenum;
		}
		break;
	case TREND_SPO2:
		{
			m_chFilenumSpO2=chFilenum;
		}
		break;
	case TREND_SPO2PR:
		{
			m_chFilenumSpO2PR=chFilenum;
		}
		break;
	case TREND_SPO2PI:
		{
			m_chFilenumSPO2PI=chFilenum;
		}
		break;
	case TREND_ETCO2:
		{
			m_chFilenumEtCO2=chFilenum;
		}
		break;
	case TREND_FREQUENCY:
		{
			m_chFilenumFrequency=chFilenum;
		}
		break;
	}
}

void CDataHandler::SerializeAllTrends(bool bIncreaseFileNum)//rkuNEWFIX
{
	//rkuNEWFIX
	EnterCriticalSection(&csTrend);
	SerializeTrend(TREND_PINSP, bIncreaseFileNum);
	SerializeTrend(TREND_PMEAN, bIncreaseFileNum);
	SerializeTrend(TREND_FIO2, bIncreaseFileNum);
	SerializeTrend(TREND_VTE, bIncreaseFileNum);
	Sleep(0);
	SerializeTrend(TREND_COMPLIANCE, bIncreaseFileNum);
	SerializeTrend(TREND_CO2HFO, bIncreaseFileNum);
	SerializeTrend(TREND_MV, bIncreaseFileNum);
	SerializeTrend(TREND_HFAMP, bIncreaseFileNum);
	Sleep(0);
	SerializeTrend(TREND_RSBI, bIncreaseFileNum);
	SerializeTrend(TREND_SHAREMVMAND, bIncreaseFileNum);
	SerializeTrend(TREND_RESISTANCE, bIncreaseFileNum);
	SerializeTrend(TREND_LEAK, bIncreaseFileNum);
	Sleep(0);
	SerializeTrend(TREND_SPO2, bIncreaseFileNum);
	SerializeTrend(TREND_SPO2PI, bIncreaseFileNum);
	SerializeTrend(TREND_ETCO2, bIncreaseFileNum);
	SerializeTrend(TREND_SPO2PR, bIncreaseFileNum);
	SerializeTrend(TREND_FREQUENCY, bIncreaseFileNum);
	LeaveCriticalSection(&csTrend);
}
bool CDataHandler::DeserializeTrend(UINT type, BYTE fileNum)
{
	bool bRes=true;
	CStringW szFile=_T("");

	CTrend* m_cpTend=NULL;
	CString szTrendFolder=_T("\\sdcard\\");

	if(CTlsFile::Exists(szTrendFolder)==false)
	{
		szTrendFolder=_T("\\FFSDISK\\");
	}


	switch(type)
	{
	case TREND_PINSP:
		{
			m_cpTend=&m_cTendPINSP;
			szTrendFolder+=IDS_TRD_FOLDER_PINSP;
		}
		break;
	case TREND_PMEAN:
		{
			m_cpTend=&m_cTendPMEAN;
			szTrendFolder+=IDS_TRD_FOLDER_PMEAN;
		}
		break;
	case TREND_FIO2:
		{
			m_cpTend=&m_cTendFIO2;
			szTrendFolder+=IDS_TRD_FOLDER_FIO2;
		}
		break;
	case TREND_VTE:
		{
			m_cpTend=&m_cTendVTE;
			szTrendFolder+=IDS_TRD_FOLDER_VTE;
		}
		break;
	case TREND_COMPLIANCE:
		{
			m_cpTend=&m_cTendCOMPLIANCE;
			szTrendFolder+=IDS_TRD_FOLDER_COMPLIANCE;
		}
		break;
	case TREND_CO2HFO:
		{
			m_cpTend=&m_cTendCO2HFO;
			szTrendFolder+=IDS_TRD_FOLDER_CO2HFO;
		}
		break;
	case TREND_MV:
		{
			m_cpTend=&m_cTendMV;
			szTrendFolder+=IDS_TRD_FOLDER_MV;
		}
		break;
	case TREND_HFAMP:
		{
			m_cpTend=&m_cTendHFAMP;
			szTrendFolder+=IDS_TRD_FOLDER_HFAMP;
		}
		break;
	case TREND_RSBI:
		{
			m_cpTend=&m_cTendRSBI;
			szTrendFolder+=IDS_TRD_FOLDER_RSBI;
		}
		break;
	case TREND_SHAREMVMAND:
		{
			m_cpTend=&m_cTendShareMVmand;
			szTrendFolder+=IDS_TRD_FOLDER_SHAREMVMAND;
		}
		break;
	case TREND_RESISTANCE:
		{
			m_cpTend=&m_cTendResistance;
			szTrendFolder+=IDS_TRD_FOLDER_RESISTANCE;
		}
		break;
	case TREND_LEAK:
		{
			m_cpTend=&m_cTendLeak;
			szTrendFolder+=IDS_TRD_FOLDER_LEAK;
		}
		break;
	case TREND_SPO2:
		{
			m_cpTend=&m_cTendSpO2;
			szTrendFolder+=IDS_TRD_FOLDER_SPO2;
		}
		break;
	case TREND_SPO2PR:
		{
			m_cpTend=&m_cTendSpO2PR;
			szTrendFolder+=IDS_TRD_FOLDER_SPO2PR;
		}
		break;
	case TREND_SPO2PI:
		{
			m_cpTend=&m_cTendSpO2PI;
			szTrendFolder+=IDS_TRD_FOLDER_SPO2PI;
		}
		break;
	case TREND_ETCO2:
		{
			m_cpTend=&m_cTendEtCO2;
			szTrendFolder+=IDS_TRD_FOLDER_ETCO2;
		}
		break;
	case TREND_FREQUENCY:
		{
			m_cpTend=&m_cTendFrequency;
			szTrendFolder+=IDS_TRD_FOLDER_FREQUENCY;
		}
		break;
	}


	szFile.Format(_T("%s%d%s"),szTrendFolder,fileNum, IDS_TRD_FILE_ID);
	CFile archivDatei;
	CFileException ex;

	try
	{
		if(archivDatei.Open(szFile,CFile::modeRead, &ex)!=0)
		{
			CArchive objektArchiv(&archivDatei, CArchive::load); 

			m_cpTend->Serialize(objektArchiv);

			objektArchiv.Close();
			archivDatei.Close();

		}
		else
		{
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);

			CStringW strFormatted;
			strFormatted = _T("#HFO:0094: ");
			strFormatted += szFile;
			strFormatted += _T(" ");
			strFormatted += szError;

			theApp.getLog()->WriteLine(strFormatted);

			bRes=false;
		}
	}
	catch (...)
	{
		archivDatei.Close();

		theApp.ReportException(_T("CDataHandler::DeserializeTrend"));
	}


	return bRes;
}



bool CDataHandler::DeserializeTempTrend(UINT type, WORD fileNum)
{
	bool bResult=false;
	CStringW szFile=_T("");

	CString szTrendFolder=_T("\\sdcard\\");

	if(CTlsFile::Exists(szTrendFolder)==false)
	{
		szTrendFolder=_T("\\FFSDISK\\");
	}

	switch(type)
	{
	case TREND_PINSP:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_PINSP,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	case TREND_PMEAN:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_PMEAN,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	case TREND_FIO2:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_FIO2,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	case TREND_VTE:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_VTE,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	case TREND_COMPLIANCE:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_COMPLIANCE,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	case TREND_CO2HFO:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_CO2HFO,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	case TREND_MV:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_MV,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	case TREND_HFAMP:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_HFAMP,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	case TREND_RSBI:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_RSBI,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	case TREND_SHAREMVMAND:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_SHAREMVMAND,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	case TREND_RESISTANCE:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_RESISTANCE,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	case TREND_LEAK:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_LEAK,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	case TREND_SPO2:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_SPO2,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	case TREND_SPO2PR:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_SPO2PR,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	case TREND_SPO2PI:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_SPO2PI,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	case TREND_ETCO2:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_ETCO2,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	case TREND_FREQUENCY:
		{
			szFile.Format(_T("%s%s%d%s"),szTrendFolder,IDS_TRD_FOLDER_FREQUENCY,fileNum, IDS_TRD_FILE_ID);
		}
		break;
	default:
		{

		}
		break;
	}

	if(szFile!=_T(""))
	{
		CFile archivDatei;
		CFileException ex;

		if(archivDatei.Open(szFile,CFile::modeRead, &ex)!=0)
		{
			CArchive objektArchiv(&archivDatei, CArchive::load);
			m_cTrendTemporary.Serialize(objektArchiv);

			objektArchiv.Close();
			archivDatei.Close();

			bResult=true;
		}
		else
		{
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			CStringW strFormatted;
			strFormatted = _T("#HFO:0095: ");
			strFormatted += szFile;
			strFormatted += _T(" ");
			strFormatted += szError;

			theApp.getLog()->WriteLine(strFormatted);
		}
	}

	return bResult;
}


void CDataHandler::DeleteAllTrendData()
{
	DEBUGMSG(TRUE, (TEXT("DeleteAllTrendData\r\n")));

	CStringW sPathOld = _T("");
	CStringW sPathOrigin = _T("");
	CStringW sPathNew = _T("");
	CString szPath=_T("\\sdcard\\");

	if(CTlsFile::Exists(szPath)==false)
	{
		szPath=_T("\\FFSDISK\\");
		sPathOld = szPath+IDS_OLDTRD_FOLDER;
		sPathOrigin = szPath+IDS_TRD_FOLDER;
		sPathNew.Format(_T("\\FFSDISK\\TrendOld\\Trend%d"), m_nCountDelTrends);

		/*if(CTlsFile::Exists(sPathNew)==true)
		{
			m_nCountDelTrends++;
			sPathNew.Format(_T("\\FFSDISK\\TrendOld\\Trend%d"), m_nCountDelTrends);
		}*/
	}
	else
	{
		sPathOld = szPath+IDS_OLDTRD_FOLDER;
		sPathOrigin = szPath+IDS_TRD_FOLDER;
		sPathNew.Format(_T("\\sdcard\\TrendOld\\Trend%d"), m_nCountDelTrends);
		/*if(CTlsFile::Exists(sPathNew)==true)
		{
			m_nCountDelTrends++;
			sPathNew.Format(_T("\\sdcard\\TrendOld\\Trend%d"), m_nCountDelTrends);
		}*/
	}

	if(m_nCountDelTrends<5)//rkuNEWFIX
		m_nCountDelTrends++;
	else
		m_nCountDelTrends=0;

	EnterCriticalSection(&csTrend);//rkuNEWFIX
	if(CTlsFile::Exists(sPathOrigin)==true)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_STOPDEL_TRENDDATA);

		BYTE iCnt=0;
		while(true==getDelTrendThread() && iCnt<150)//rkuNEWFIX 150
		{
			Sleep(10);
			iCnt++;
		}

		if (!CTlsFile::Exists(szPath+IDS_OLDTRD_FOLDER))
		{
			CTlsFile::MkDir(szPath+IDS_OLDTRD_FOLDER);
		}

		if(CTlsFile::Rename(sPathOrigin,sPathNew,true)==true)
		{
			bool bRes=true;
			if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER))
			{
				bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER);

				if(!bRes)
				{
				}
				else
				{
					CStringW sFileError= _T("");
					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_PINSP) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_PINSP);

						if(!bRes)
						{
							sFileError+=_T("PInsp ");
						}
					}

					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_PMEAN) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_PMEAN);
						if(!bRes)
						{
							sFileError+=_T("PMEAN ");
						}
					}

					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_FIO2) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_FIO2);
						if(!bRes)
						{
							sFileError+=_T("FIO2 ");
						}
					}

					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_VTE) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_VTE);
						if(!bRes)
						{
							sFileError+=_T("VTE ");
						}
					}

					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_COMPLIANCE) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_COMPLIANCE);
						if(!bRes)
						{
							sFileError+=_T("COMPLIANCE ");
						}
					}

					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_CO2HFO) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_CO2HFO);
						if(!bRes)
						{
							sFileError+=_T("CO2HFO ");
						}
					}

					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_MV) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_MV);
						if(!bRes)
						{
							sFileError+=_T("MV ");
						}
					}

					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_HFAMP) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_HFAMP);
						if(!bRes)
						{
							sFileError+=_T("HFAMP ");
						}
					}

					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_RSBI) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_RSBI);
						if(!bRes)
						{
							sFileError+=_T("RSBI ");
						}
					}

					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_SHAREMVMAND) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_SHAREMVMAND);
						if(!bRes)
						{
							sFileError+=_T("SHAREMVMAND ");
						}
					}

					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_RESISTANCE) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_RESISTANCE);
						if(!bRes)
						{
							sFileError+=_T("RESISTANCE ");
						}
					}

					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_LEAK) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_LEAK);
						if(!bRes)
						{
							sFileError+=_T("LEAK ");
						}
					}

					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_SPO2) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_SPO2);
						if(!bRes)
						{
							sFileError+=_T("SPO2 ");
						}
					}

					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_SPO2PI) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_SPO2PI);
						if(!bRes)
						{
							sFileError+=_T("SPO2PI ");
						}
					}

					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_ETCO2) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_ETCO2);
						if(!bRes)
						{
							sFileError+=_T("ETCO2 ");
						}
					}
					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_SPO2PR) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_SPO2PR);
						if(!bRes)
						{
							sFileError+=_T("SPO2PR ");
						}
					}

					if (!CTlsFile::Exists(szPath+IDS_TRD_FOLDER_FREQUENCY) && bRes)
					{
						bRes=CTlsFile::MkDir(szPath+IDS_TRD_FOLDER_FREQUENCY);
						if(!bRes)
						{
							sFileError+=_T("FREQ ");
						}
					}

					if(sFileError!=_T(""))
					{
						theApp.getLog()->WriteLine(_T("#HFO:0096 ")+sFileError);
					}
				}
			}
		}
		else
		{
		}
	}

	m_chFilenumPINSP=1;
	m_chFilenumPMEAN=1;
	m_chFilenumFIO2=1;
	m_chFilenumVTE=1;
	m_chFilenumCOMPLIANCE=1;
	m_chFilenumCO2HFO=1;
	m_chFilenumMV=1;
	m_chFilenumHFAMP=1;
	m_chFilenumRSBI=1;
	m_chFilenumShareMVmand=1;
	m_chFilenumResistance=1;
	m_chFilenumLeak=1;
	m_chFilenumSpO2=1;
	m_chFilenumSPO2PI=1;
	m_chFilenumEtCO2=1;
	m_chFilenumSpO2PR=1;
	m_chFilenumFrequency=1;

	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_PINSP, 1);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_PMEAN, 1);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_FIO2, 1);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_VTE, 1);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_COMPLIANCE, 1);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_CO2HFO, 1);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_MV, 1);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_HFAMP, 1);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_RSBI, 1);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_SHAREMVMAND, 1);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_RESISTANCE, 1);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_LEAK, 1);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_SPO2, 1);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_SPO2PI, 1);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_ETCO2, 1);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_SPO2PR, 1);
	getModel()->getCONFIG()->SetLastWrittenTrendFile(TREND_FREQUENCY, 1);

	m_cTendPINSP.ResetBuffer();
	m_cTendPMEAN.ResetBuffer();
	m_cTendFIO2.ResetBuffer();
	m_cTendVTE.ResetBuffer();
	m_cTendCOMPLIANCE.ResetBuffer();
	m_cTendCO2HFO.ResetBuffer();
	m_cTendMV.ResetBuffer();
	m_cTendHFAMP.ResetBuffer();
	m_cTendRSBI.ResetBuffer();
	m_cTendShareMVmand.ResetBuffer();
	m_cTendResistance.ResetBuffer();
	m_cTendLeak.ResetBuffer();
	m_cTendSpO2.ResetBuffer();
	m_cTendSpO2PI.ResetBuffer();
	m_cTendEtCO2.ResetBuffer();
	m_cTendSpO2PR.ResetBuffer();
	m_cTendFrequency.ResetBuffer();
	LeaveCriticalSection(&csTrend);//rkuNEWFIX

	EnterCriticalSection(&csTrendFileData);//rkuNEWFIX
	m_iTrendCnt=0;
	m_fTrendData_Pmean=0;
	m_fTrendData_FiO2=0;
	m_fTrendData_Pinsp=0;
	m_fTrendData_Vte=0;
	m_fTrendData_Compliance=0;
	m_fTrendData_CO2HFO=0;
	m_fTrendData_MV=0;
	m_fTrendData_HFAMP=0;
	m_fTrendData_RSBI=0;
	m_fTrendData_ShareMVmand=0;
	m_fTrendData_Resistance=0;
	m_fTrendData_Leak=0;
	m_fTrendData_SpO2=0;
	m_fTrendData_PI=0;
	m_fTrendData_etCO2=0;
	m_fTrendData_SpO2PR=0;
	m_fTrendData_Frequency=0;
	LeaveCriticalSection(&csTrendFileData);//rkuNEWFIX

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_DEL_TRENDDATA);
}



// **************************************************************************
// 
// **************************************************************************
bool CDataHandler::isPatientDataAvailable()
{
	return m_bPatientDataAvailable;
}

void CDataHandler::setPatientDataAvailable(bool bState)
{
	m_bPatientDataAvailable=bState;
}


// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetPinspNotReachedFlag(bool state)
{
	if(		getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_SYSTEM_SILENT
		||	getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getVIEWHANDLER()->getViewState()==VS_STARTUP)
		return;

	//checkhere
	m_bPinspNotReached=state;

	

	eStateOfAlarm statePinsp_NotReached = getModel()->getALARMHANDLER()->ALARM_SysLimit_Pinsp_NotReached->getAlarmState();

	if(		state==true
		&&	getModel()->getALARMHANDLER()->ALARM_DISCONNECTION->getAlarmState()!=AS_ACTIVE
		&&	getModel()->getSERIAL()->IsSerialDisconnection()==false
		&&	getModel()->IsSPIDisconnection()==false
		&&	getModel()->IsCalibrationViewActive()==false
		&&	!getModel()->isO2FlushActive()
		&&	!getModel()->isMANBREATHrunning())
	{
		if(getModel()->getALARMHANDLER()->CanSetAlarm_Pinsp_NotReached())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_PINSPNOTREACHED);
		}
	}
	else if(	state==false
		&&	statePinsp_NotReached!=AS_NONE)
	{
		getModel()->getALARMHANDLER()->deleteAlarm(AL_SysLimit_Pinsp_NotReached);
	}
	else if(	getModel()->IsCalibrationViewActive()==true
		&&	statePinsp_NotReached!=AS_NONE)
	{
		getModel()->getALARMHANDLER()->deleteAlarm(AL_SysLimit_Pinsp_NotReached);
	}
	else if(	getModel()->isO2FlushActive()
		&&	statePinsp_NotReached!=AS_NONE)
	{
		getModel()->getALARMHANDLER()->deleteAlarm(AL_SysLimit_Pinsp_NotReached);
	}
	else if(	getModel()->isMANBREATHrunning()
		&&	statePinsp_NotReached!=AS_NONE)
	{
		getModel()->getALARMHANDLER()->deleteAlarm(AL_SysLimit_Pinsp_NotReached);
	}
}
bool CDataHandler::GetPinspNotReachedFlag()
{
	return m_bPinspNotReached;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::setSPIErrorCode(int bit, bool bIgnoreSilentState)
{
	eVentSilentState silentState=getModel()->getALARMHANDLER()->getAlarmSilentState();
	if(		silentState!=ASTATE_SYSTEM_SILENT
		&&	silentState!=ASTATE_SILENT)
	{
		m_iSPIErrorCodeBits=SetBit(m_iSPIErrorCodeBits,bit);
	}
	else if(bIgnoreSilentState)
	{
		m_iSPIErrorCodeBits=SetBit(m_iSPIErrorCodeBits,bit);
	}
	
}
//void CDataHandler::DeleteSPIErrorCode(int bit)
//{
//	m_iSPIErrorCodeBits=DeleteBit(m_iSPIErrorCodeBits,bit);
//}
void CDataHandler::deleteSPIErrorCode()
{
	m_iSPIErrorCodeBits=0;
}
int CDataHandler::getSPIErrorCode()
{
	return m_iSPIErrorCodeBits;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetCO2ErrorCode(int bit, bool bIgnoreSilentState)
{
	eVentSilentState silentState=getModel()->getALARMHANDLER()->getAlarmSilentState();
	if(		silentState!=ASTATE_SYSTEM_SILENT
		&&	silentState!=ASTATE_SILENT)
	{
		m_iCO2ErrorCodeBits=SetBit(m_iCO2ErrorCodeBits,bit);
	}
	else if(bIgnoreSilentState)
	{
		m_iCO2ErrorCodeBits=SetBit(m_iCO2ErrorCodeBits,bit);
	}
}
void CDataHandler::DeleteCO2ErrorCode(int bit)
{
	m_iCO2ErrorCodeBits=DeleteBit(m_iCO2ErrorCodeBits,bit);
}
void CDataHandler::DeleteCO2ErrorCode()
{
	m_iCO2ErrorCodeBits=0;
}
int CDataHandler::GetCO2ErrorCode()
{
	return m_iCO2ErrorCodeBits;
}

void CDataHandler::setCOMErrorCode(int iERRORcode/*,int iSerialCommand, bool bIgnoreSilentState*/)//newVG
{
	m_iCOMErrorCodeBits=SetBit(m_iCOMErrorCodeBits,iERRORcode);
	/*if(iERRORcode==ERRC_COM_CHECK_DATA)
	{
		m_iCOMErrorCommandBits=SetBit(m_iCOMErrorCommandBits,iSerialCommand);
	}*/
	
	CString szTemp=_T("");
	szTemp.Format(_T("#HFO:0298: %d"), iERRORcode);
	theApp.getLog()->WriteLine(szTemp);
	DEBUGMSG(TRUE, (szTemp));
}
void CDataHandler::deleteCOMErrorCode(int iERRORcode)//newVG
{
	m_iCOMErrorCodeBits=DeleteBit(m_iCOMErrorCodeBits,iERRORcode);

	if(iERRORcode & ERRC_COM_CHECK_DATA)//newSerialAlarm
	{
		getModel()->getSERIAL()->resetErrorCountCheckThread();
	}
}
//void CDataHandler::deleteCOMErrorCommand(int iERRORCommand)//newVG
//{
//	m_iCOMErrorCommandBits=DeleteBit(m_iCOMErrorCommandBits,iERRORCommand);
//}
void CDataHandler::deleteAllCOMError()//newVG
{
	m_iCOMErrorCodeBits=0;
	
	getModel()->getSERIAL()->resetErrorCountCheckThread();//newSerialAlarm
}
int CDataHandler::getCOMErrorCode()//newVG
{
	return m_iCOMErrorCodeBits;
}
//int CDataHandler::getCOMErrorCommand()//newVG
//{
//	return m_iCOMErrorCommandBits;
//}

bool CDataHandler::setCOMLastSendError(CStringW szData)//newVG
{
	bool bAlarm=true;

	CStringW szCommand=szData.Mid( 0, 1 );
	if(szCommand==_T("N"))//M_CHECKSUM
	{
		bAlarm=false;
	}
	else
	{
		setCOMErrorCode(ERRC_COM_SEND_DATA);
		CString szTemp=_T("");
		szTemp.Format(_T("#setCOMLastSendError: %s"), szData);
		DEBUGMSG(TRUE, (szTemp));
	}
	m_szCOMlastSendError=szCommand;

	return bAlarm;
}
bool CDataHandler::checkCOMlastSendError(CStringW szData)//newVG
{
	bool bSameError=false;
	CStringW szCommand=szData.Mid( 0, 1 );
	if(szCommand==m_szCOMlastSendError)
	{
		bSameError=true;
	}
	return bSameError;
}
void CDataHandler::checkCOMresetLastSendError(CStringW szData)//newVG
{
	CStringW szCommand=szData.Mid( 0, 1 );
	if(szCommand==m_szCOMlastSendError)
	{
		deleteCOMErrorCode(ERRC_COM_SEND_DATA);
		if(getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE) //newVG
		{
			getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
		}
	}
}
// **************************************************************************
// 
// **************************************************************************
//void CDataHandler::SetI2CErrorCode(int bit)
//{
//	m_iI2CErrorCodeBits=SetBit(m_iI2CErrorCodeBits,bit);
//}
//void CDataHandler::DeleteI2CErrorCode(int bit)
//{
//	m_iI2CErrorCodeBits=DeleteBit(m_iI2CErrorCodeBits,bit);
//}
//void CDataHandler::DeleteI2CErrorCode()
//{
//	m_iI2CErrorCodeBits=0;
//}
//int CDataHandler::GetI2CErrorCode()
//{
//	return m_iI2CErrorCodeBits;
//}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::setPSVapnoe(bool bState)
{
	
	/*CStringW alarmTxt=getModel()->GetLanguageString(IDS_ALARM_BACKUP_ACTIVE);
	alarmTxt+=_T(": ");
	
	if(m_bPSVapnoe!=bState)
	{
		if(bState)
		{
			alarmTxt+=getModel()->GetLanguageString(IDS_TXT_ON);
		}
		else
		{
			alarmTxt+=getModel()->GetLanguageString(IDS_TXT_OFF);
		}

		CStringW szTemp=_T("");
		szTemp.Format(_T("#%d"), 0);
		alarmTxt+=szTemp;

		getModel()->getALARMHANDLER()->SetAlarmLog(alarmTxt, true);
	}*/

	m_bPSVapnoe=bState;
}
bool CDataHandler::isPSVapnoe()
{
	return m_bPSVapnoe;
}


void CDataHandler::SetNurscallAlarm(bool state)
{
	DWORD dwResult=0;
	
	if(state==true && m_bNurscallAlarm==false)
	{
		dwResult=getModel()->getDIO()->WritePin (NURSEPINBIT);
		if(dwResult==0)
		{
			if(m_iCountDIOERROR>=5)
			{
				m_iCountDIOERROR=0;
				if(getModel()->getALARMHANDLER()->CanSetAlarm_IF_DIO())
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_DIO);
				}
				
			}
			else
				m_iCountDIOERROR++;
		}
	}
	else if(state==false && m_bNurscallAlarm==true)
	{
		dwResult=getModel()->getDIO()->ClearPin (NURSEPINBIT);
		if(dwResult==0)
		{
			if(m_iCountDIOERROR>=5)
			{
				m_iCountDIOERROR=0;
				if(getModel()->getALARMHANDLER()->CanSetAlarm_IF_DIO())
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_DIO);
				}
				
			}
			else
				m_iCountDIOERROR++;
		}
	}

	m_bNurscallAlarm=state;
	
}
bool CDataHandler::isNurscallAlarm()
{
	return m_bNurscallAlarm;
}

// **************************************************************************
// 
// **************************************************************************
//void CDataHandler::setPRICO_O2(BYTE iCalcFiO2)
//{
//	//getModel()->getVIEWHANDLER()->setOxyPPRICO(iCalcFiO2);//PRICO04
//
//	//setO2FlushDiff(iCalcFiO2);//PRICO04
//	//SetO2Paradata(iCalcFiO2,true);//PRICO04
//
//	//getModel()->Send_PARA_OXY_RATIO(iCalcFiO2,true,true);//PRICO04
//}
void CDataHandler::setO2Diff(BYTE iFlushVal)
{
	m_iO2Difference=iFlushVal-GetCurrentO2Para();
	if(m_iO2Difference<MAXDIFF_O2_FLUSH)
		m_iO2Difference=MAXDIFF_O2_FLUSH;

	getModel()->getCONFIG()->SetO2Difference(m_iO2Difference);
}

void CDataHandler::setO2FlushDiff(BYTE iVal)
{
	m_iO2Difference=GetCurrentO2FlushPara()-iVal;
	if(m_iO2Difference<MAXDIFF_O2_FLUSH)
		m_iO2Difference=MAXDIFF_O2_FLUSH;

	getModel()->getCONFIG()->SetO2Difference(m_iO2Difference);
}

BYTE CDataHandler::getO2Diff()
{
	return m_iO2Difference;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::setPEEP_PpsvDifference(int iDiff)
{
	DEBUGMSG(TRUE, (TEXT("CDataHandler::setPEEP_PPSVdifference %d\r\n"),iDiff));
	if(iDiff!=m_iPEEP_PPSVdifference)
	{
		m_iPEEP_PPSVdifference=iDiff;

		getModel()->getCONFIG()->setPEEP_PpsvDifference(m_iPEEP_PPSVdifference);
	}

}

int CDataHandler::getPEEP_PpsvDifference()
{
	return m_iPEEP_PPSVdifference;
}

/**********************************************************************************************//**
 * @fn	void CDataHandler::checkVGdependency()
 *
 * @brief	Check VG dependency.
 *
 * @author	Rainer Kuehner
 * @date	06.05.2016
 **************************************************************************************************/
void CDataHandler::checkVGdependency()//rku, PMAX check
{
	SHORT iPmax_IPPV=PARADATA()->GetPmaxVolGPara_IPPV();
	SHORT iPmax_TRIGGER=PARADATA()->GetPmaxVolGPara_TRIGGER();
	SHORT iPEEP_IPPV=PARADATA()->GetPEEPPara_IPPV();
	SHORT iPEEP_TRIGGER=PARADATA()->GetPEEPPara_TRIGGER();

	if(iPEEP_IPPV+DIFF_PEEP_PINSP>iPmax_IPPV)
	{
		if(iPEEP_IPPV%10==0)
		{
			SetPmaxVolGParadata_IPPV(iPEEP_IPPV+DIFF_PEEP_PINSP, true);
		}
		else
		{
			SetPmaxVolGParadata_IPPV(iPEEP_IPPV+DIFF_PEEP_PINSP+5, true);
		}
	}

	if(iPEEP_TRIGGER+DIFF_PEEP_PINSP>iPmax_TRIGGER)
	{
		if(iPEEP_IPPV%10==0)
		{
			SetPmaxVolGParadata_TRIGGER(iPEEP_TRIGGER+DIFF_PEEP_PINSP, true);
		}
		else
		{
			SetPmaxVolGParadata_TRIGGER(iPEEP_TRIGGER+DIFF_PEEP_PINSP+5, true);
		}
	}
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::setPmeanRecDifference(int iVal)
{
	m_iPmeanDifference=iVal;
	/*m_iPmeanDifference=iVal-GetCurrentPmeanPara();
	if(m_iPmeanDifference<MAXDIFF_PMEANREC)
		m_iPmeanDifference=MAXDIFF_PMEANREC;*/

	getModel()->getCONFIG()->SetPmeanDifference(m_iPmeanDifference);
}
//void CDataHandler::setPmeanDiff(int iVal)
//{
//	m_iPmeanDifference=GetCurrentHFPMeanRecPara()-iVal;
//	if(m_iPmeanDifference<MAXDIFF_PMEANREC)
//		m_iPmeanDifference=MAXDIFF_PMEANREC;
//
//	getModel()->getCONFIG()->SetPmeanDifference(m_iPmeanDifference);
//	SetCurrentHFPMeanRecPara(GetCurrentHFPMeanRecPara());
//
//}
int CDataHandler::getPmeanDifference()
{
	return m_iPmeanDifference;
}
// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetHFOrunning(bool state)
{
	bool bTemp=m_bHFOrunning;
	m_bHFOrunning = state;

	if(state!=bTemp)
	{
		EnterCriticalSection(&csOpTime);
		DWORD dwTemp=m_dwOpTimeHFOTickCountStarted;
		LeaveCriticalSection(&csOpTime);

		if(dwTemp==0 && state==true)
		{
			EnterCriticalSection(&csOpTime);
			m_dwOpTimeHFOTickCountStarted = GetTickCount();
			LeaveCriticalSection(&csOpTime);
		}
		else if(state==false)
		{
			saveOpTime();

			EnterCriticalSection(&csOpTime);
			m_dwOpTimeHFOTickCountStarted = 0;
			LeaveCriticalSection(&csOpTime);
		}
	}
}
bool CDataHandler::IsHFOrunning()
{
	return m_bHFOrunning;
}

// **************************************************************************
// 
// **************************************************************************
int CDataHandler::SetBit(int x, unsigned int n)
{
	//x = x | (1 << n);
	x = x | n;

	return x;
}
int CDataHandler::DeleteBit(int x, unsigned int n)
{
	//x = x & ~(1 << n);
	x = x & ~n;

	return x;
}


bool CDataHandler::IsGraphFreezed()
{
	return m_bFreezedGraphs;
}
void CDataHandler::SetGraphFreezed(bool state)
{
	m_bFreezedGraphs=state;
}

void CDataHandler::SetSavingTrendToUSBactiv()
{
	m_bSavingTrendToUSB=true;
}
void CDataHandler::SetSavingTrendToUSBinactiv()
{
	m_bSavingTrendToUSB=false;
}
bool CDataHandler::IsSavingTrendToUSBactiv()
{
	return m_bSavingTrendToUSB;
}

void CDataHandler::LoadingTrendData(bool bState)
{
	m_bTrendsLoading=bState;
}

bool CDataHandler::IsTrendDataLoading()
{
	return m_bTrendsLoading;
}

void CDataHandler::ReadI2CWatchdogState()
{
	m_byI2CWatchdogState=getModel()->getI2C()->ReadMCP_Port0();
	CString szTemp=_T("");
	szTemp.Format(_T("***WatchdogState %d"), (int) m_byI2CWatchdogState);
	theApp.getLog()->WriteLine(szTemp);
}

void CDataHandler::ResetI2CWatchdogState()
{
	m_byI2CWatchdogState=0;
}

BYTE CDataHandler::GetI2CWatchdogState()
{
	return m_byI2CWatchdogState;
}

void CDataHandler::SetDelTrendThread(bool state)
{
	//rkuTOCHECKFIX check why this is signaled so late
	EnterCriticalSection(&csDelTrendThread);//rkuNEWFIX
	m_bDoDelTrendThread=state;
	LeaveCriticalSection(&csDelTrendThread);//rkuNEWFIX

	/*if(state)
	{
		DEBUGMSG(TRUE, (TEXT("SetDelTrendThread true\r\n")));
	}
	else
	{
		DEBUGMSG(TRUE, (TEXT("SetDelTrendThread false\r\n")));
	}*/
}

bool CDataHandler::getDelTrendThread()//rkuNEWFIX
{
	EnterCriticalSection(&csDelTrendThread);//rkuNEWFIX
	bool state=m_bDoDelTrendThread;
	LeaveCriticalSection(&csDelTrendThread);//rkuNEWFIX

	return state;
}


// **************************************************************************
// 
// **************************************************************************
bool CDataHandler::IsLEDdisplay()
{
	return m_bLEDdisplay;
}


// **************************************************************************
// 
// **************************************************************************
void CDataHandler::SetVGarantParadata_IPPV(WORD val, bool bSend,bool bConfig)
{
	PARADATA()->SetVGarantPara_IPPV(val,bSend,bConfig);
	PRESET()->SetVGarantPara_IPPV(val,false, false);
}
void CDataHandler::SetVGarantParadata_TRIGGER(WORD val, bool bSend,bool bConfig)
{
	PARADATA()->SetVGarantPara_TRIGGER(val,bSend,bConfig);
	PRESET()->SetVGarantPara_TRIGGER(val,false,false);
}
void CDataHandler::SetHFVGarantParadata(WORD val, bool bSend,bool bConfig)
{
	PARADATA()->SetHFVGarantPara(val,bSend,bConfig);
	PRESET()->SetHFVGarantPara(val,false,false);
}
void CDataHandler::setVGARANTControlEnabled()//EFLOW1
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

	if(eActiveVentMode==VM_HFO
		||	eActiveVentMode==VM_CPAP
		||	eActiveVentMode==VM_NCPAP
		||	eActiveVentMode==VM_DUOPAP
		||	eActiveVentMode==VM_THERAPIE)
	{
		SetVLimitParamdata_IPPV(PARADATA()->GetVLimitParam_IPPV(), false,false);

		SetVLimitParamdata_TRIGGER(PARADATA()->GetVLimitParam_TRIGGER(), false,false);
	}
	else if(eActiveVentMode==VM_IPPV)
	{
		SetVLimitParamdata_IPPV(PARADATA()->GetVLimitParam_IPPV(), false,true);
	}
	else
	{
		SetVLimitParamdata_TRIGGER(PARADATA()->GetVLimitParam_TRIGGER(), false,true);
	}
	getModel()->getALARMHANDLER()->resetVgVlAutoTurnedOff();
}
void CDataHandler::setVLIMITControlEnabled()//EFLOW1
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

	if(eActiveVentMode==VM_HFO
		||	eActiveVentMode==VM_CPAP
		||	eActiveVentMode==VM_NCPAP
		||	eActiveVentMode==VM_DUOPAP
		||	eActiveVentMode==VM_THERAPIE)
	{
		SetPINSPParadata_IPPV(PARADATA()->GetPINSPPara_IPPV(), false);
		PARADATA()->SetVGarantState_IPPV(false,false,true);
		PRESET()->SetVGarantState_IPPV(false,false,false);

		SetPpsvParadata(PARADATA()->GetPpsvPara(), false);
		SetPINSPParadata_TRIGGER(PARADATA()->GetPINSPPara_TRIGGER(), false);
		PARADATA()->SetVGarantState_TRIGGER(false,false,true);
		PRESET()->SetVGarantState_TRIGGER(false,false,false);
	}
	else if(eActiveVentMode==VM_IPPV)
	{
		SetPINSPParadata_IPPV(PARADATA()->GetPINSPPara_IPPV(), true);
		SetVGarantState_IPPV(false);
	}
	else if(eActiveVentMode==VM_PSV)
	{
		SetPpsvParadata(PARADATA()->GetPpsvPara(), true);
		SetVGarantState_TRIGGER(false);
	}
	else
	{
		SetPINSPParadata_TRIGGER(PARADATA()->GetPINSPPara_TRIGGER(), true);
		SetVGarantState_TRIGGER(false);
	}
	getModel()->getALARMHANDLER()->resetVgVlAutoTurnedOff();
}
void CDataHandler::SetActiveModeVolumeGarantyState(bool bOn)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

	if(eActiveVentMode==VM_HFO)
	{
		if(bOn)
			SetHFAMPLmaxParadata(PARADATA()->GetHFAMPLmaxPara(), true);
		else
			SetHFAMPLParadata(PARADATA()->GetHFAMPLPara(), true);
		SetHFVGarantState(bOn/*,bSend,bConfig,bAutoOn,bAutoOff*/);
	}
	else if(eActiveVentMode==VM_IPPV)
	{
		if(bOn)
			SetPmaxVolGParadata_IPPV(PARADATA()->GetPmaxVolGPara_IPPV(), true);
		else
			SetPINSPParadata_IPPV(PARADATA()->GetPINSPPara_IPPV(), true);
		SetVGarantState_IPPV(bOn/*,bSend,bConfig,bAutoOn,bAutoOff*/);
	}
	else if(eActiveVentMode==VM_PSV)
	{
		if(bOn)
			SetPmaxVolGParadata_TRIGGER(PARADATA()->GetPmaxVolGPara_TRIGGER(), true);
		else
			SetPpsvParadata(PARADATA()->GetPpsvPara(), true);
		SetVGarantState_TRIGGER(bOn/*,bSend,bConfig,bAutoOn,bAutoOff*/);
	}
	else
	{
		if(bOn)
			SetPmaxVolGParadata_TRIGGER(PARADATA()->GetPmaxVolGPara_TRIGGER(), true);
		else
			SetPINSPParadata_TRIGGER(PARADATA()->GetPINSPPara_TRIGGER(), true);
		SetVGarantState_TRIGGER(bOn/*,bSend,bConfig,bAutoOn,bAutoOff*/);
	}
}
void CDataHandler::SetPresetModeVolumeGarantyState(bool bOn/*,bool bSend,bool bConfig,bool bAutoOn,bool bAutoOff*/)
{
	if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO)
		PRESET()->SetHFVGarantState(bOn,false,false);
	else if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_IPPV)
		PRESET()->SetVGarantState_IPPV(bOn,false,false);
	else
		PRESET()->SetVGarantState_TRIGGER(bOn,false,false);
}
void CDataHandler::SetVGarantState_TRIGGER(bool bOn/*,bool bSend,bool bConfig,bool bAutoOn,bool bAutoOff*/)
{
	/*WORD iValue=PARADATA()->GetPmaxVolGPara_TRIGGER();
	SetVGarantParamVal_TRIGGER(iValue, bOn,bSend,bConfig,bAutoOn,bAutoOff)*/
	PARADATA()->SetVGarantState_TRIGGER(bOn,true,true);
	PRESET()->SetVGarantState_TRIGGER(bOn,false,false);
}
void CDataHandler::SetVGarantState_IPPV(bool bOn/*,bool bSend,bool bConfig,bool bAutoOn,bool bAutoOff*/)
{
	/*WORD iValue=PARADATA()->GetPmaxVolGPara_IPPV();
	SetVGarantParamVal_IPPV(iValue, bOn,bSend,bConfig,bAutoOn,bAutoOff)*/
	PARADATA()->SetVGarantState_IPPV(bOn,true,true);
	PRESET()->SetVGarantState_IPPV(bOn,false,false);
}
void CDataHandler::SetHFVGarantState(bool bOn/*,bool bSend,bool bConfig,bool bAutoOn,bool bAutoOff*/)
{
	/*WORD iValue=PARADATA()->GetHFAMPLVolGPara();
	SetHFVGarantParaVal(iValue, bOn,bSend,bConfig,bAutoOn,bAutoOff)*/
	PARADATA()->SetHFVGarantState(bOn,true,true);
	PRESET()->SetHFVGarantState(bOn,false,false);
}



void CDataHandler::SetVLimitParadata_IPPV(WORD val, bool bOn,bool bSend)
{
	PARADATA()->SetVLimitParam_IPPV(val,bOn,bSend,true);
	PRESET()->SetVLimitParam_IPPV(val,bOn,false,false);
}
void CDataHandler::SetVLimitParadata_TRIGGER(WORD val, bool bOn,bool bSend)
{
	PARADATA()->SetVLimitParam_TRIGGER(val,bOn,bSend,true);
	PRESET()->SetVLimitParam_TRIGGER(val,bOn,false,false);
}



void CDataHandler::SetVLimitParamdata_IPPV(WORD val, bool bOn,bool bSend)
{
	PARADATA()->SetVLimitParam_IPPV(val, bOn,bSend,true);
	PRESET()->SetVLimitParam_IPPV(val, bOn,false,false);
}
void CDataHandler::SetVLimitParamdata_TRIGGER(WORD val, bool bOn,bool bSend)
{
	PARADATA()->SetVLimitParam_TRIGGER(val, bOn,bSend,true);
	PRESET()->SetVLimitParam_TRIGGER(val, bOn,false,false);
}

void CDataHandler::SetETIMEParadata_TRIGGER(WORD value, bool bSend)
{
	PARADATA()->SetETIMEPara_TRIGGER(value, bSend,true);
	PRESET()->SetETIMEPara_TRIGGER(value, false, false);
}
void CDataHandler::SetETIMEParadata_IPPV(WORD value, bool bSend)
{
	PARADATA()->SetETIMEPara_IPPV(value, bSend,true);
	PRESET()->SetETIMEPara_IPPV(value, false, false);
}

void CDataHandler::SetETIMENMODEParadata(WORD value, bool bSend)
{
	PARADATA()->SetETIMENMODEPara(value, bSend,true);
	PRESET()->SetETIMENMODEPara(value, false, false);
}
void CDataHandler::SetBPMParadata_TRIGGER(WORD value, bool bSend)
{
	PARADATA()->SetBPMPara_TRIGGER(value, bSend,true);
	PRESET()->SetBPMPara_TRIGGER(value, false, false);
}
void CDataHandler::SetBPMParadata_IPPV(WORD value, bool bSend)
{
	PARADATA()->SetBPMPara_IPPV(value, bSend,true);
	PRESET()->SetBPMPara_IPPV(value, false, false);
}
void CDataHandler::SetBPMNMODEParadata(WORD value, bool bSend)
{
	PARADATA()->SetBPMNMODEPara(value, bSend,true);
	PRESET()->SetBPMNMODEPara(value, false, false);
}
void CDataHandler::SetTriggerCPAPParadata(BYTE value, bool bSend)
{
	PARADATA()->SetTriggerPara_CPAP(value, bSend,true);
	PRESET()->SetTriggerPara_CPAP(value, false, false);
}
void CDataHandler::SetTriggerCONVParadata(BYTE value, bool bSend)
{
	PARADATA()->SetTriggerPara_CONV(value, bSend,true);
	PRESET()->SetTriggerPara_CONV(value, false, false);
}
void CDataHandler::SetTriggerDUOPAPParadata(BYTE value, bool bSend)
{
	PARADATA()->SetTriggerPara_DUOPAP(value, bSend,true);
	PRESET()->SetTriggerPara_DUOPAP(value, false, false);
}

void CDataHandler::SetTriggerNCPAPParadata(BYTE value, bool bSend)
{
	PARADATA()->SetTriggerPara_NCPAP(value, bSend,true);
	PRESET()->SetTriggerPara_NCPAP(value, false, false);
}

void CDataHandler::SetITimeParadata_TRIGGER(WORD value, bool bSend)
{
	PARADATA()->SetITimePara_TRIGGER(value, bSend,true);
	PRESET()->SetITimePara_TRIGGER(value, false, false);
}
void CDataHandler::SetITimeParadata_IPPV(WORD value, bool bSend)
{
	PARADATA()->SetITimePara_IPPV(value, bSend,true);
	PRESET()->SetITimePara_IPPV(value, false, false);
}
void CDataHandler::SetITimeNMODEParadata(WORD value, bool bSend)
{
	PARADATA()->SetITimeNMODEPara(value, bSend,true);
	PRESET()->SetITimeNMODEPara(value, false, false);
}
void CDataHandler::SetO2Paradata(BYTE value, bool bSend)
{
	PARADATA()->SetO2Para(value, bSend,true);
	PRESET()->SetO2Para(value, false, false);
}
void CDataHandler::SetO2FlushParadata(BYTE value)
{
	PARADATA()->SetO2FlushPara(value,true);
	PRESET()->SetO2FlushPara(value, false);
}
void CDataHandler::SetRisetimeParadata_TRIGGER(WORD value, bool bSend)
{
	PARADATA()->SetRisetimePara_TRIGGER(value, bSend,true);
	PRESET()->SetRisetimePara_TRIGGER(value, false, false);
}
void CDataHandler::SetRisetimeParadata_IPPV(WORD value, bool bSend)
{
	PARADATA()->SetRisetimePara_IPPV(value, bSend,true);
	PRESET()->SetRisetimePara_IPPV(value, false, false);
}
void CDataHandler::SetHFFlowParadata(WORD value, bool bSend)
{
	PARADATA()->SetHFFlowPara(value, bSend,true);
	PRESET()->SetHFFlowPara(value, false, false);
}
void CDataHandler::SetIFlowParadata_TRIGGER(WORD value, bool bSend)
{
	PARADATA()->SetIFlowPara_TRIGGER(value, bSend,true);
	PRESET()->SetIFlowPara_TRIGGER(value, false, false);
}
void CDataHandler::SetIFlowParadata_IPPV(WORD value, bool bSend)
{
	PARADATA()->SetIFlowPara_IPPV(value, bSend,true);
	PRESET()->SetIFlowPara_IPPV(value, false, false);
}
void CDataHandler::SetEFlowParadata_TRIGGER(WORD value, bool bSend)
{
	PARADATA()->SetEFLOWPara_TRIGGER(value, bSend,true);
	PRESET()->SetEFLOWPara_TRIGGER(value, false, false);
}
void CDataHandler::SetEFlowParadata_IPPV(WORD value, bool bSend)
{
	PARADATA()->SetEFLOWPara_IPPV(value, bSend,true);
	PRESET()->SetEFLOWPara_IPPV(value, false, false);
}
void CDataHandler::SetFlowminParadata(WORD value, bool bSend)
{
	PARADATA()->SetFlowminPara(value, bSend,true);
	PRESET()->SetFlowminPara(value, false, false);
}
void CDataHandler::SetTherapieFLOWParadata(WORD value, bool bSend)
{
	PARADATA()->SetTherapieFLOWPara(value, bSend,true);
	PRESET()->SetTherapieFLOWPara(value, false, false);
}
void CDataHandler::SetPmaxVolGParadata_TRIGGER(SHORT value, bool bSend)
{
	PARADATA()->SetPmaxVolGPara_TRIGGER(value, bSend,true);
	PRESET()->SetPmaxVolGPara_TRIGGER(value, false, false);
}
void CDataHandler::SetPmaxVolGParadata_IPPV(SHORT value, bool bSend)
{
	PARADATA()->SetPmaxVolGPara_IPPV(value, bSend,true);
	PRESET()->SetPmaxVolGPara_IPPV(value, false, false);
}
void CDataHandler::SetPINSPParadata_TRIGGER(SHORT value, bool bSend)
{
	PARADATA()->SetPINSPPara_TRIGGER(value, bSend,true);
	PRESET()->SetPINSPPara_TRIGGER(value, false, false);
}
void CDataHandler::SetPINSPParadata_IPPV(SHORT value, bool bSend)
{
	if(value%10!=0)//check test
	{
		int iStop=0;
	}
	PARADATA()->SetPINSPPara_IPPV(value, bSend,true);
	PRESET()->SetPINSPPara_IPPV(value, false, false);
}
void CDataHandler::SetPEEPParadata_TRIGGER(SHORT value, bool bSend)
{
	PARADATA()->SetPEEPPara_TRIGGER(value, bSend,true);
	PRESET()->SetPEEPPara_TRIGGER(value, false, false);
}
void CDataHandler::SetPEEPParadata_IPPV(SHORT value, bool bSend)
{
	PARADATA()->SetPEEPPara_IPPV(value, bSend,true);
	PRESET()->SetPEEPPara_IPPV(value, false, false);
}
void CDataHandler::SetPpsvParadata(SHORT value, bool bSend)
{
	PARADATA()->SetPpsvPara(value, bSend,true);
	PRESET()->SetPpsvPara(value, false, false);
}
void CDataHandler::SetCPAPParadata(SHORT value, bool bSend)
{
	PARADATA()->SetCPAPPara(value, bSend,true);
	PRESET()->SetCPAPPara(value, false, false);
}
void CDataHandler::SetCPAPNMODEParadata(SHORT value, bool bSend)
{
	PARADATA()->SetCPAPNMODEPara(value, bSend,true);
	PRESET()->SetCPAPNMODEPara(value, false, false);
}


void CDataHandler::SetPManualHFOParadata(SHORT value, bool bSend)
{
	PARADATA()->SetPManualHFOPara(value, bSend,true);
	PRESET()->SetPManualHFOPara(value, false, false);
}
void CDataHandler::SetPManualCPAPParadata(SHORT value, bool bSend)
{
	PARADATA()->SetPManualCPAPPara(value, bSend,true);
	PRESET()->SetPManualCPAPPara(value, false, false);
}


void CDataHandler::SetPManualNMODEParadata(SHORT value, bool bSend)
{
	PARADATA()->SetPManualNMODEPara(value, bSend,true);
	PRESET()->SetPManualNMODEPara(value, false, false);
}
void CDataHandler::SetHFPMeanParadata(WORD value, bool bSend)
{
	PARADATA()->SetHFPMeanPara(value, bSend,true);
	PRESET()->SetHFPMeanPara(value, false, false);
}
void CDataHandler::SetBackupParadata(BYTE value, bool bSend)
{
	PARADATA()->SetBackupPara(value, bSend,true);
	PRESET()->SetBackupPara(value, false, false);
}
void CDataHandler::SetIERatioParadataHFO(eRatioIE ratio, bool bSend)
{
	PARADATA()->SetIERatioParaHFO(ratio, bSend,true);
	PRESET()->SetIERatioParaHFO(ratio, false, false);
}
void CDataHandler::SetHFFreqParadata(BYTE value, bool bSend)
{
	PARADATA()->SetHFFreqPara(value, bSend,true);
	PRESET()->SetHFFreqPara(value, false, false);
}
void CDataHandler::SetHFAMPLmaxParadata(WORD value, bool bSend)
{
	PARADATA()->SetHFAMPLmaxPara(value, bSend,true);
	PRESET()->SetHFAMPLmaxPara(value, false, false);
}
void CDataHandler::SetITimeRecParadata(WORD value, bool bSend)
{
	PARADATA()->SetITimeRecPara(value, bSend,true);
	PRESET()->SetITimeRecPara(value, false, false);
}
void CDataHandler::SetFreqRecParadata(WORD value, bool bSend)
{
	PARADATA()->SetFreqRecPara(value, bSend,true);
	PRESET()->SetFreqRecPara(value, false, false);
}
void CDataHandler::SetHFPMeanRecParadata(WORD value, bool bSend)
{
	PARADATA()->SetHFPMeanRecPara(value, bSend,true);
	PRESET()->SetHFPMeanRecPara(value, false, false);
}
void CDataHandler::SetHFAMPLParadata(WORD value, bool bSend)
{
	PARADATA()->SetHFAMPLPara(value, bSend,true);
	PRESET()->SetHFAMPLPara(value, false, false);
}
void CDataHandler::SetFOTconv_AMPLITUDEParadata(WORD value, bool bSend)
{
	PARADATA()->setFOTconv_AMPLITUDEPara(value, bSend,true);
	PRESET()->setFOTconv_AMPLITUDEPara(value, false, false);
}
void CDataHandler::SetFOTconv_FREQParadata(WORD value, bool bSend)
{
	PARADATA()->setFOTconv_FREQPara(value, bSend,true);
	PRESET()->setFOTconv_FREQPara(value, false, false);
}
void CDataHandler::SetFOTconv_STEPSParadata(WORD value, bool bSend)
{
	PARADATA()->setFOTconv_STEPSPara(value, bSend,true);
	PRESET()->setFOTconv_STEPSPara(value, false, false);
}
void CDataHandler::SetFOTconv_PEEPSTARTParadata(WORD value, bool bSend)
{
	PARADATA()->setFOTconv_PEEPSTARTPara(value, bSend,true);
	PRESET()->setFOTconv_PEEPSTARTPara(value, false, false);
}
void CDataHandler::SetFOTconv_PEEPENDParadata(WORD value, bool bSend)
{
	PARADATA()->setFOTconv_PEEPENDPara(value, bSend,true);
	PRESET()->setFOTconv_PEEPENDPara(value, false, false);
}

void CDataHandler::SetFOThfo_AMPLITUDEParadata(WORD value, bool bSend)
{
	PARADATA()->setFOThfo_AMPLITUDEPara(value, bSend,true);
	PRESET()->setFOThfo_AMPLITUDEPara(value, false, false);
}
void CDataHandler::SetFOThfo_FREQParadata(WORD value, bool bSend)
{
	PARADATA()->setFOThfo_FREQPara(value, bSend,true);
	PRESET()->setFOThfo_FREQPara(value, false, false);
}
void CDataHandler::SetFOThfo_STEPSParadata(WORD value, bool bSend)
{
	PARADATA()->setFOThfo_STEPSPara(value, bSend,true);
	PRESET()->setFOThfo_STEPSPara(value, false, false);
}
void CDataHandler::SetFOThfo_PMEANSTARTParadata(WORD value, bool bSend)
{
	PARADATA()->setFOThfo_PMEANSTARTPara(value, bSend,true);
	PRESET()->setFOThfo_PMEANSTARTPara(value, false, false);
}
void CDataHandler::SetFOThfo_PMEANENDParadata(WORD value, bool bSend)
{
	PARADATA()->setFOThfo_PMEANENDPara(value, bSend,true);
	PRESET()->setFOThfo_PMEANENDPara(value, false, false);
}

void CDataHandler::setFOToscillationState(bool bOsciRun)
{
	EnterCriticalSection(&csFOTosciState);
	m_bFOToscillationState=bOsciRun;
	LeaveCriticalSection(&csFOTosciState);

	getModel()->Send_MODE_OPTION1();
}

bool CDataHandler::getFOToscillationState()
{
	bool bState=false;
	EnterCriticalSection(&csFOTosciState);
	bState=m_bFOToscillationState;
	LeaveCriticalSection(&csFOTosciState);
	return bState;
}

void CDataHandler::setBodyweight(WORD weightGramm, bool bLog)
{
	if(weightGramm>BODYWEIGHTMAXIMUM)
		weightGramm=BODYWEIGHTMAXIMUM;
	m_iBodyweightGramm=weightGramm;

	if(bLog)
	{
		CStringW szBW=_T("");
		szBW.Format(_T("### BODYWEIGHT %d"), m_iBodyweightGramm);
		theApp.getLog()->WriteLine(szBW);
	}
	
}
WORD CDataHandler::getBodyweight()
{
	return m_iBodyweightGramm;
}

// **************************************************************************
// 
// **************************************************************************
void CDataHandler::setStatus2(SHORT state)
{
	m_Status2=state;
}
SHORT CDataHandler::getStatus2()
{
	return m_Status2;
}

BYTE CDataHandler::getCountNumericFLOWOFFCONV()
{
	return m_iNumericFLOWOFFCONVcount;
}
BYTE CDataHandler::getCountNumericFLOWOFFCPAP()
{
	return m_iNumericFLOWOFFCPAPcount;
}
BYTE CDataHandler::getCountNumericFLOWOFFHFO()
{
	return m_iNumericFLOWOFFHFOcount;
}

BYTE CDataHandler::getCountNumericIPPV()
{
	return m_iNumericIPPVcount;
}

BYTE CDataHandler::getCountNumericSIPPV()
{
	return m_iNumericSIPPVcount;
}

BYTE CDataHandler::getCountNumericSIMV()
{
	return m_iNumericSIMVcount;
}

BYTE CDataHandler::getCountNumericSIMVPSV()
{
	return m_iNumericSIMVPSVcount;
}

BYTE CDataHandler::getCountNumericPSV()
{
	return m_iNumericPSVcount;
}

BYTE CDataHandler::getCountNumericCPAP()
{
	return m_iNumericCPAPcount;
}

BYTE CDataHandler::getCountNumericHFO()
{
	return m_iNumericHFOcount;
}

BYTE CDataHandler::getCountNumericNCPAP()
{
	return m_iNumericNCPAPcount;
}
BYTE CDataHandler::getCountNumericDUOPAP()
{
	return m_iNumericDUOPAPcount;
}
BYTE CDataHandler::getCountNumericTHERAPY()
{
	return m_iNumericTHERAPYcount;
}


//void CDataHandler::setProcPressureCal60(int iVal)
//{
//	DEBUGMSG(TRUE, (TEXT("setProcPressureCal60 %d\r\n"),iVal));
//	m_iProcPressureCal60=iVal;
//}
//int CDataHandler::getProcPressureCal60()
//{
//	return m_iProcPressureCal60;
//}

void CDataHandler::enableProcPressureCal60()
{
	m_bProcPressureCal60=true;
}
void CDataHandler::disableProcPressureCal60()
{
	m_bProcPressureCal60=false;
}
bool CDataHandler::isProcPressureCal60enabled()
{
	return m_bProcPressureCal60;
}

void CDataHandler::setRemainCO2PumpTime(DWORD time)
{
	m_dwRemainCO2PumpTime=time;
}
DWORD CDataHandler::getRemainCO2PumpTime()
{
	return m_dwRemainCO2PumpTime;
}

double CDataHandler::getAmpCorFactor(BYTE iFreq)
{
	return getModel()->getCONFIG()->getAmpCorFactor(iFreq);
}