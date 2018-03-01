// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "MainFrm.h"
#include "globDefs.h"
#include <wdogapi.h>
#include <afxwin.h>
#include "TlsRegistry.h"
#include "logfile.h"
#include <service.h>
#include <commctrl.h>

extern "C" BOOL WINAPI TouchCalibrate( void );

/**********************************************************************************************//**
 * *** Preprocessor *******************************************************
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

#define WD_NAME			_T("Verifying")		// watch dog name

/**********************************************************************************************//**
 * A macro that defines wd periode
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

#define WD_PERIODE		35000				// watch dog refresh time in ms

/**********************************************************************************************//**
 * A macro that defines wd wait
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

#define WD_WAIT			11000				// wait time in ms if watch dog timer is not refreshed

/**********************************************************************************************//**
 * A macro that defines matrixdelay
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

#define MATRIXDELAY 150

/**********************************************************************************************//**
 * A macro that defines errorcnt spi
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

#define ERRORCNT_SPI	4

/***** Global **************************************************************/		
HANDLE	g_hWatchDog;	///< Handle of the watch dog
HANDLE	g_hAction;  ///< Handle of the action
bool	m_bRun; ///< True to run
bool	g_bMainWatchdogFlag;	///< True to main watchdog flag
bool	g_bMainWatchdogPending; ///< True to main watchdog pending

bool Action(void*);


#ifdef _DEBUG

/**********************************************************************************************//**
 * A macro that defines new
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

#define new DEBUG_NEW
#endif

/**********************************************************************************************//**
 * TimerValue
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

#define AUTOLIMITMINUTE	60000

/**********************************************************************************************//**
 * A macro that defines timemodechange
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

#define TIMEMODECHANGE	45000

/**********************************************************************************************//**
 * A macro that defines timestartstop
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

#define TIMESTARTSTOP	500

/**********************************************************************************************//**
 * A macro that defines counttimestartstop
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

#define COUNTTIMESTARTSTOP	1000

/**********************************************************************************************//**
 * A macro that defines timealarmsilent
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

#define TIMEALARMSILENT	1000

/**********************************************************************************************//**
 * A macro that defines standby
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

#define STANDBY 120



//global font objects
HFONT g_hf5AcuNorm; ///< The hf 5 acu normalise
HFONT g_hf6AcuNorm; ///< The hf 6 acu normalise
HFONT g_hf6AcuBold; ///< The hf 6 acu bold
HFONT g_hf7AcuNorm; ///< The hf 7 acu normalise
HFONT g_hf7AcuBold; ///< The hf 7 acu bold
HFONT g_hf7AcuBold90degree; ///< The hf 7 acu bold 90degree
HFONT g_hf8AcuNorm; ///< The hf 8 acu normalise
HFONT g_hf8AcuBold; ///< The hf 8 acu bold
HFONT g_hf9AcuBold; ///< The hf 9 acu bold
HFONT g_hf10AcuBold;	///< The hf 10 acu bold
HFONT g_hf11AcuBold;	///< The hf 11 acu bold
HFONT g_hf11AcuBoldNum; ///< The hf 11 acu bold number
HFONT g_hf13AcuBold;	///< The hf 13 acu bold
HFONT g_hf13AcuBoldNum; ///< The hf 13 acu bold number
HFONT g_hf14AcuMed; ///< The hf 14 acu median
HFONT g_hf14AcuBold;	///< The hf 14 acu bold
HFONT g_hf14AcuNormNum; ///< The hf 14 acu normalise number
HFONT g_hf15AcuMed; ///< The hf 15 acu median
//HFONT g_hf24Normal;
//HFONT g_hf24Medium;
HFONT g_hf17AcuBold;	///< The hf 17 acu bold
HFONT g_hf19AcuMed; ///< The hf 19 acu median
HFONT g_hf21AcuBold;	///< The hf 21 acu bold
HFONT g_hf23AcuBold;	///< The hf 23 acu bold
HFONT g_hf25AcuMed; ///< The hf 25 acu median
HFONT g_hf27AcuBold;	///< The hf 27 acu bold
HFONT g_hf31AcuBold;	///< The hf 31 acu bold
HFONT g_hf31AcuBoldNum; ///< The hf 31 acu bold number
HFONT g_hf33AcuBold;	///< The hf 33 acu bold
HFONT g_hf33AcuBoldNum; ///< The hf 33 acu bold number
HFONT g_hf34BoldNum;	///< The hf 34 bold number

//HFONT g_hf42Bold;
HFONT g_hf43AcuBold;	///< The hf 43 acu bold
HFONT g_hf53AcuBold;	///< The hf 53 acu bold
HFONT g_hf70Bold;   ///< The hf 70 bold
HFONT g_hf70BoldNum;	///< The hf 70 bold number



CEvent g_eventFOT;  ///< The event fot
CEvent g_eventPRICO;	///< The event prico
CEvent eventDoTestTimerFunctions;   ///< The event do test timer functions
CEvent g_eventGraphData;	///< Information describing the event graph
CEvent eventWaitTrendUSBfinish; ///< The event wait trend us bfinish
CEvent g_eventNewMonitorData;   ///< Information describing the event new monitor
CEvent g_eventNewAlarmData; ///< Information describing the event new alarm
CEvent g_eventNewFlowsensorData;	///< Information describing the event new flowsensor
CEvent g_eventExspiration;  ///< The event exspiration
CEvent g_eventCheckSensor;  ///< The event check sensor
CEvent g_eventI2Cdata;  ///< The event i 2 cdata
CEvent g_eventI2Cdone;  ///< The event i 2 cdone
CEvent g_eventCOM;  ///< The event com
CEvent g_eventCO2CheckData; ///< Information describing the event co 2 check
CEvent g_eventCO2Data;  ///< Information describing the event co 2
CEvent g_eventETCO2SendData;	///< Information describing the event etco 2 send
CEvent g_eventSPO2Data; ///< Information describing the event spo 2
CEvent g_eventSPO2SendData; ///< Information describing the event spo 2 send
CEvent g_eventSPO2CheckData;	///< Information describing the event spo 2 check
CEvent g_eventCOMIFData;	///< Information describing the event comif
CEvent g_eventCOMIFSendData;	///< Information describing the event comif send
CEvent g_eventCOMIFCheckData;   ///< Information describing the event comif check
CEvent g_eventTerminalSendData; ///< Information describing the event terminal send
CEvent g_evBLENDER_STATUS;  ///< l
CEvent g_evBLENDER_VERSION; ///< H
CEvent g_evCOM_VERSION; ///< Y
CEvent g_evCOM_CAL_PRESS_OFFSET;	///< G
CEvent g_evCOM_CAL_PRESS_SCALE; ///< B
CEvent g_evCOM_CHECKSUM;	///< N
CEvent g_evCOM_STATUS;  ///< L
CEvent g_evCOM_M_INSP_FLOW; ///< X
CEvent g_evCOM_M_EXP_FLOW;  ///< x
CEvent g_evCOM_M_DEMAND_FLOW;   ///< b

CRITICAL_SECTION CMainFrame::m_csI2Cinit;

/**********************************************************************************************//**
 * CMainFrame
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CMainFrame construction/destruction
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CMainFrame::CMainFrame()
{
	InitializeCriticalSection(&m_csI2Cinit);

	m_bInitialized=false;//WEC2013
	m_bWD0error=false;//WEC2013
	m_bWDERRORI2C=false;//WEC2013

	m_bStartInstaller=false;
	m_bI2Cinitialized=false;

	m_bForceShutdown=false;//WEC2013
	m_pModel = NULL;//WEC2013

	m_pcLogo=NULL;//WEC2013
	
	m_nX=0;//WEC2013
	m_nY=0;//WEC2013
	m_pszFontName[0]=0x0000;//WEC2013

	m_numtasks=0;//WEC2013
	m_bExit=false;//WEC2013
	m_bCancelOxiCal=false;//WEC2013
	m_bO2Sensor21manual=false;//WEC2013
	m_bO2Sensor100manual=false;//WEC2013
	m_bO2SensorAutocal=false;//WEC2013

	m_bBackup=false;//WEC2013
	//m_ullRemainCO2PumpTime=0;//WEC2013
	m_bShowCO2PumpOnMsg=true;//WEC2013
	
	m_iTestCntMemory=0;//WEC2013

	m_bMatrxDwn_STARTSTOP=false;//WEC2013
	m_bMatrxDwn_OFF=false;//WEC2013
	m_bMatrxDwn_MANBREATH=false;//WEC2013
	m_bMatrxDwn_MANBREATHdisabled=false;//WEC2013
	m_bMatrxDwn_O2Flush=false;//WEC2013
	m_bMatrxDwn_Alarmsilent=false;//WEC2013
	m_bMatrxDwn_Graph=false;//WEC2013
	m_bMatrxDwn_ALimits=false;//WEC2013
	m_bMatrxDwn_Home=false;//WEC2013
	m_bMatrxDwn_Menu=false;//WEC2013
	m_bMatrxDwn_FREE=false;//WEC2013
	m_bNebulizerDwn=false;//WEC2013

	m_bSysSilent=false;//WEC2013

	m_dwLastAutoScreenTimer=0;//WEC2013
	m_dwLastMatrixdwn=0;//WEC2013
	m_dwSystemsilent=0;//WEC2013
	m_dwFactorydefaults=0;//WEC2013

	m_dwLastWatchdogMemoryCheck=0;//WEC2013

	m_dwMemory=0;//WEC2013

	//m_bNebulizerRunning=false;//WEC2013
	//m_bNebulizerDelayRunning=false;//WEC2013

	m_eVentModeCalcStarted=VM_NONE;//WEC2013

	m_iCountCollectFOTTimer=0;
	m_iCountFOTimer=0;
	m_iO2FlushCount=0;//WEC2013
	m_iCountDownStart=0;//WEC2013
	m_iCountDownAlarmSilent=0;//WEC2013
	m_iCountTimeUntilStop=0;//WEC2013
	m_iCountTimeUntilOff=0;//WEC2013

	//m_pcwtThreadWatchdogThread=NULL;//rkuNEWFIX
	m_pcwtTimerThread=NULL;//WEC2013
	m_pcwtOxyCalThread=NULL;//WEC2013
	m_pcwtI2CWatchdogThread=NULL;//WEC2013
	m_pcwtSaveTrendUSBThread=NULL;//WEC2013
	m_pcwtDelTrendThread=NULL;//WEC2013

	//m_bDoThreadWatchdogThread=false;//rkuNEWFIX
	m_bDoSaveTrendUSBThread=false;//WEC2013
	m_bDoI2CWatchdogThread=false;//WEC2013
	m_bDoOxyCalThread=false;//WEC2013
	m_bDoDoTimerFunctionsThread=false;//WEC2013
	m_bDoDelTrendThread=false;//WEC2013

	//m_hThreadThreadWatchdog=INVALID_HANDLE_VALUE;//rkuNEWFIX
	m_hThreadSaveTrendUSB=INVALID_HANDLE_VALUE;//WEC2013
	m_hThreadI2CWatchdog=INVALID_HANDLE_VALUE;//WEC2013
	m_hThreadOxyCal=INVALID_HANDLE_VALUE;//WEC2013
	m_hThreadTimerFunctions=INVALID_HANDLE_VALUE;//WEC2013
	m_hThreadDelTrend=INVALID_HANDLE_VALUE;//WEC2013

	g_bMainWatchdogFlag=true;//WEC2013
	g_bMainWatchdogPending=false;//WEC2013

	m_dwHourglasstimeout=0;//WEC2013
	m_bStartTrend=false;//WEC2013
	m_bDelayAutoOxyCal=true;//WEC2013


	m_wLanguageID=0;//WEC2013

	//WEC2013
	m_hf10Bold=NULL;
	m_hf13Bold=NULL;
	m_hf10Norm=NULL;
	m_hf11Norm=NULL;
	m_hf12Norm=NULL;
	m_hf12Norm90degree=NULL;
	m_hf13Norm=NULL;
	m_hf14Norm=NULL;
	m_hf14Bold=NULL;
	m_hf15Normal=NULL;
	m_hf12AcuNormNum=NULL;
	m_hf14AcuNormNum=NULL;
	m_hf14Bold90degree=NULL;
	m_hf15Bold=NULL;
	m_hf16Normal=NULL;
	m_hf16Bold=NULL;
	m_hf16Bold90degree=NULL;
	m_hf17Bold=NULL;
	m_hf18Normal=NULL;
	m_hf18Bold=NULL;
	m_hf18BoldNum=NULL;
	m_hf20Bold=NULL;
	m_hf20BoldNum=NULL;
	m_hf21Medium=NULL;
	m_hf21Bold=NULL;
	m_hf22Medium=NULL;
	m_hf22Bold=NULL;
	m_hf24Bold=NULL;
	m_hf26Medium=NULL;
	m_hf28Bold=NULL;
	m_hf30Bold=NULL;
	m_hf32Medium=NULL;
	m_hf34Bold=NULL;
	m_hf34BoldNum=NULL;
	m_hf38Bold=NULL;
	m_hf40Bold=NULL;
	m_hf50Bold=NULL;
	m_hf60Bold=NULL;
	m_hf70Bold=NULL;
	m_hf31AcuBoldNum=NULL;
	m_hf33AcuBoldNum=NULL;
	m_hf70BoldNum=NULL;

	m_iOldOxyValue=0;

	/*for(int i=0;i<MAXSIZE_THREADS;i++)
	{
		faThreadWatchdog[i]=0;
		faPrevstateThreadWatchdog[i]=0;
	}*/
	//InitializeCriticalSection(&csThreadWatchdog);
}

/**********************************************************************************************//**
 * Finalizes an instance of the CMainFrame class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CMainFrame::~CMainFrame()
{
	if(m_pcwtSaveTrendUSBThread!=NULL)
	{
		delete m_pcwtSaveTrendUSBThread;
		m_pcwtSaveTrendUSBThread=NULL;

		if(m_hThreadSaveTrendUSB!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSaveTrendUSB);
			m_hThreadSaveTrendUSB=INVALID_HANDLE_VALUE;
		}
	}

	if(m_pcwtDelTrendThread!=NULL)
	{
		delete m_pcwtDelTrendThread;
		m_pcwtDelTrendThread=NULL;

		if(m_hThreadDelTrend!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadDelTrend);
			m_hThreadDelTrend=INVALID_HANDLE_VALUE;
		}
	}

	if(m_pcwtTimerThread!=NULL)
	{
		delete m_pcwtTimerThread;
		m_pcwtTimerThread=NULL;

		if(m_hThreadTimerFunctions!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadTimerFunctions);
			m_hThreadTimerFunctions=INVALID_HANDLE_VALUE;
		}
	}

	if(m_pcwtOxyCalThread!=NULL)
	{
		delete m_pcwtOxyCalThread;
		m_pcwtOxyCalThread=NULL;

		if(m_hThreadOxyCal!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadOxyCal);
			m_hThreadOxyCal=INVALID_HANDLE_VALUE;
		}
	}

	if(m_pcwtI2CWatchdogThread!=NULL)
	{
		delete m_pcwtI2CWatchdogThread;
		m_pcwtI2CWatchdogThread=NULL;

		if(m_hThreadI2CWatchdog!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadI2CWatchdog);
			m_hThreadI2CWatchdog=INVALID_HANDLE_VALUE;
		}
	}

	DeleteCriticalSection(&m_csI2Cinit);
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CMainFrame::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

//int CALLBACK EnumFontFamProc(LOGFONT *lpLF, TEXTMETRIC *lpTM, DWORD dwTyp, LPARAM lpData)
//{
//	/*CArray<CString,CString&> *paFonts=(CArray<CString,CString&>*)lpData;
//	paFonts->Add(CString(lpLF->lfFaceName));*/
//	CStringW temp=_T("font: ");
//	temp+=CString(lpLF->lfFaceName);
//	return 1;
//}

/**********************************************************************************************//**
 * Called when this window is created
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	lpCreateStruct	The create structure.
 *
 * \return	An int.
 **************************************************************************************************/

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

#ifndef _DEBUG
	DWORD dwID;

	g_hWatchDog=CreateWatchDogTimer(WD_NAME,WD_PERIODE,WD_WAIT,WDOG_NO_DFLT_ACTION,0,0);

	g_hAction=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Action,NULL,NULL,&dwID);
	// TODO: create thread failed

	if(g_hWatchDog==NULL)
	{
		theApp.writeLogError(GetLastError(), _T("WD: g_hWatchDog"));
		return 0;
	}
	if(!StartWatchDogTimer(g_hWatchDog,0))
	{
		theApp.writeLogError(GetLastError(), _T("WD: Unable to start"));
		return 0;
	}

	//SetTimer(WATCHDOGTIMER,2000,NULL);
	//StartMainWatchdogThread();

 
	if(!RefreshWatchDogTimer(g_hWatchDog,0))
	{
		theApp.writeLogError(GetLastError(), _T("WD: Unable to refresh#1"));
	}
#endif	

	CClientDC dc(this);

	m_nX=GetSystemMetrics(SM_CXSCREEN);
	m_nY=GetSystemMetrics(SM_CYSCREEN);
	//m_nBPP=GetDeviceCaps(dc,BITSPIXEL);

	
	::SetWindowPos (	this->m_hWnd,
		HWND_TOPMOST,
		0,
		0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN), 
		SWP_SHOWWINDOW);

	Invalidate();
	UpdateWindow();


	CStringW sBMP = _T("\\FFSDISK\\logoFabianHFO.bmp");

	if(CTlsFile::Exists(sBMP))
	{
		m_pcLogo	= new CBmp(dc.m_hDC,sBMP);

		if(m_pcLogo)
			m_pcLogo->Draw(dc.m_hDC,390,30);
	}
	else
	{
		m_pcLogo	= new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_PIC_ACULOGO);
		//m_pcBaby	= new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_PIC_FabianHFO);

		if(m_pcLogo)
			m_pcLogo->Draw(dc.m_hDC,430,55);
		//if(m_pcBaby)
			//m_pcBaby->Draw(dc.m_hDC,390,100);
	}



	//SHUTDOWN services which listen on a port
	HANDLE hServiceHTP = GetServiceHandle(_T("HTP0:"),NULL,NULL);
	if(hServiceHTP!=INVALID_HANDLE_VALUE)
	{
		BOOL bSuccess=DeregisterService(hServiceHTP);
		theApp.writeLogError(GetLastError(), _T("***SECURITY: disable HTP"));
	}

	HANDLE hServiceTEL = GetServiceHandle(_T("TEL0:"),NULL,NULL);
	if(hServiceTEL!=INVALID_HANDLE_VALUE)
	{
		BOOL bSuccess=DeregisterService(hServiceTEL);
		theApp.writeLogError(GetLastError(), _T("***SECURITY: disable HTP"));
	}

	CTlsRegistry regLang(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	m_wLanguageID=(WORD)regLang.ReadDWORD(_T("LanguageID"), 0);
	
	CreateAcuFonts(m_wLanguageID,false);
	LoadGlobalAcuFonts(m_wLanguageID);
	
	SetTimer(WATCHDOGTIMER,2000,NULL);

	CString szLAN=_T("");
	szLAN.Format(_T("***Init LanguageID %s ID %d***"), m_pszFontName, m_wLanguageID);
	theApp.getLog()->WriteLine(szLAN);
		
	StartI2CWatchdogThread();


	getModel()->Init(m_pszFontName,m_wLanguageID);

	getModel()->getALARMHANDLER()->setSystemSilent();

	
#ifndef _DEBUG
	if(!RefreshWatchDogTimer(g_hWatchDog,0))
	{
		theApp.writeLogError(GetLastError(), _T("WD: Unable to refresh#2"));
	}
#endif

	WIN32_FIND_DATA FileData;
	HANDLE hSearch = ::FindFirstFile(_T("\\FFSDISK\\*.dl_"), &FileData);
	if(hSearch != INVALID_HANDLE_VALUE)
	{
		do
		{
			CString sFile = _T("\\FFSDISK\\");
			sFile += FileData.cFileName;

			if(CTlsFile::Exists(sFile))
			{
				if (CTlsFile::Delete(sFile)==false)
				{
					CString csDel=_T("");
					csDel.Format(_T("#HFO:0216 %s"), sFile);
					theApp.getLog()->WriteLine(csDel);
				}
			}

		} while(::FindNextFile(hSearch, &FileData));

		FindClose(hSearch);
	}

	hSearch = ::FindFirstFile(_T("\\FFSDISK\\*.ex_"), &FileData);
	if(hSearch != INVALID_HANDLE_VALUE)
	{
		do
		{
			CString sFile = _T("\\FFSDISK\\");
			sFile += FileData.cFileName;

			if(CTlsFile::Exists(sFile))
			{
				if (CTlsFile::Delete(sFile)==false)
				{
					CString csDel=_T("");
					csDel.Format(_T("#HFO:0216 %s"), sFile);
					theApp.getLog()->WriteLine(csDel);
				}
			}

		} while(::FindNextFile(hSearch, &FileData));

		FindClose(hSearch);
	}

	if(CTlsFile::Exists(_T("\\Hard Disk\\netdcu11\\AutoStart.ex_")))
	{
		if (CTlsFile::Delete(_T("\\Hard Disk\\netdcu11\\AutoStart.ex_"))==false)
		{
			theApp.getLog()->WriteLine(_T("#HFO:0216 AutoStart.ex_"));
		}
	}
	else if(CTlsFile::Exists(_T("\\Hard Disk\\netdcu9\\AutoStart.exe")))
	{
		if (CTlsFile::Delete(_T("\\Hard Disk\\netdcu9\\AutoStart.ex_"))==false)
		{
			theApp.getLog()->WriteLine(_T("#HFO:0216 AutoStart.ex_"));
		}
	}


	StartTimerThread();

	m_bInitialized=true;


	//TEST
	/*char *divisor = "K";

	MEMORYSTATUS stat;
	GlobalMemoryStatus (&stat);
	m_dwMemoryStart=stat.dwAvailPhys/1024;*/

	//TEST END

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_THR_START,THR_MAIN);//rkuNEWFIX

	return 0;
}

/**********************************************************************************************//**
 * Pre create window
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	cs	The create struct.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	/*WNDCLASS wc;
	wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
	SetCursor(NULL);*/



	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}



// CMainFrame diagnostics

#ifdef _DEBUG

/**********************************************************************************************//**
 * Assert valid
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}
#endif //_DEBUG

/**********************************************************************************************//**
 * CMainFrame message handlers
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pOldWnd	If non-null, the old window.
 **************************************************************************************************/

void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	//CFrameWnd::OnSetFocus(pOldWnd);

	// forward focus to the view window
	//m_wndView.SetFocus();
	//m_vGraph->SetFocus();
	this->SetFocus();
	//DEBUGMSG(TRUE, (TEXT("CMainFrame::OnSetFocus()\r\n")));
	getModel()->getVIEWHANDLER()->SetCurrentFocusedView(NULL);
}

//BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
//{
//	// let the view have first crack at the command
//	/*if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
//		return TRUE;*/
//
//	// otherwise, do default handling
//	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
//}

/**********************************************************************************************//**
 * Creates acu fonts
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	wLanguageID	   	Identifier for the language.
 * \param	bSetFaceToModel	True to set face to model.
 *
 * \return	The new acu fonts.
 **************************************************************************************************/

WORD CMainFrame::CreateAcuFonts(WORD wLanguageID, bool bSetFaceToModel)
{
	CClientDC dc(this);

	HANDLE hSearch;
	WIN32_FIND_DATA fileData;
	//BOOL bFinished = false;

	bool bSDCARDfont=false;

	//TCHAR szFileName[255];

	hSearch = FindFirstFile(_T("\\sdcard\\fonts\\*.*"), &fileData);

	if (hSearch != INVALID_HANDLE_VALUE)
	{
		bSDCARDfont=true;

		FindClose(hSearch);
	}
	else
		hSearch = NULL;

	if(bSDCARDfont)
	{
		theApp.getLog()->WriteLine(_T("***SDCARDfont true"));
		//DEBUGMSG(TRUE, (TEXT("SDCARDfont true\r\n")));
	}
	else
	{
		theApp.getLog()->WriteLine(_T("***SDCARDfont false"));
		//DEBUGMSG(TRUE, (TEXT("SDCARDfont false\r\n")));
	}
	

	if(bSDCARDfont)
	{
		if(wLanguageID==LAN_CHINESE)
		{
			_tcscpy_s(m_pszFontName,_countof(m_pszFontName),_T("Arial Unicode MS"));
			//_tcscpy_s(m_pszFontName,_countof(m_pszFontName),_T("SimHei"));
		}
		else if(wLanguageID==LAN_JAPANESE)
		{
			_tcscpy_s(m_pszFontName,_countof(m_pszFontName),_T("MS PGothic"));
			//_tcscpy_s(m_pszFontName,_countof(m_pszFontName),_T("SimHei"));
		}
		else
		{
			//RegisterFFSDISKFonts();
			_tcscpy_s(m_pszFontName,_countof(m_pszFontName),_T("arial"));
		}
		
		RegisterFFSDISKFonts();
		RegisterSDCardFonts();
	}
	else
	{
		if(wLanguageID==LAN_CHINESE || wLanguageID==LAN_JAPANESE)
		{
			CTlsRegistry regLang(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
			regLang.WriteDWORD(_T("LanguageID"), 0);
			m_wLanguageID=0;
		}
		_tcscpy_s(m_pszFontName,_countof(m_pszFontName),_T("arial"));
		RegisterFFSDISKFonts();
	}

	if(bSetFaceToModel)
		getModel()->SetFontFace(m_pszFontName);

	if(m_hf10Norm!=NULL)
	{
		DeleteObject(m_hf10Norm);
		m_hf10Norm=NULL;
	}
	if(m_hf10Bold!=NULL)
	{
		DeleteObject(m_hf10Bold);
		m_hf10Bold=NULL;
	}
	if(m_hf13Bold!=NULL)
	{
		DeleteObject(m_hf13Bold);
		m_hf13Bold=NULL;
	}

	if(m_hf11Norm!=NULL)
	{
		DeleteObject(m_hf11Norm);
		m_hf11Norm=NULL;
	}
	if(m_hf12Norm!=NULL)
	{
		DeleteObject(m_hf12Norm);
		m_hf12Norm=NULL;
	}
	if(m_hf12Norm90degree!=NULL)
	{
		DeleteObject(m_hf12Norm90degree);
		m_hf12Norm90degree=NULL;
	}
	if(m_hf13Norm!=NULL)
	{
		DeleteObject(m_hf13Norm);
		m_hf13Norm=NULL;
	}
	if(m_hf14Norm!=NULL)
	{
		DeleteObject(m_hf14Norm);
		m_hf14Norm=NULL;
	}
	if(m_hf14Bold!=NULL)
	{
		DeleteObject(m_hf14Bold);
		m_hf14Bold=NULL;
	}
	if(m_hf14Bold90degree!=NULL)
	{
		DeleteObject(m_hf14Bold90degree);
		m_hf14Bold90degree=NULL;
	}
	if(m_hf15Normal!=NULL)
	{
		DeleteObject(m_hf15Normal);
		m_hf15Normal=NULL;
	}
	if(m_hf12AcuNormNum!=NULL)
	{
		DeleteObject(m_hf12AcuNormNum);
		m_hf12AcuNormNum=NULL;
	}
	if(m_hf14AcuNormNum!=NULL)
	{
		DeleteObject(m_hf14AcuNormNum);
		m_hf14AcuNormNum=NULL;
	}
	if(m_hf15Bold!=NULL)
	{
		DeleteObject(m_hf15Bold);
		m_hf15Bold=NULL;
	}
	if(m_hf16Normal!=NULL)
	{
		DeleteObject(m_hf16Normal);
		m_hf16Normal=NULL;
	}
	if(m_hf16Bold!=NULL)
	{
		DeleteObject(m_hf16Bold);
		m_hf16Bold=NULL;
	}
	if(m_hf16Bold90degree!=NULL)
	{
		DeleteObject(m_hf16Bold90degree);
		m_hf16Bold90degree=NULL;
	}
	if(m_hf17Bold!=NULL)
	{
		DeleteObject(m_hf17Bold);
		m_hf17Bold=NULL;
	}
	if(m_hf18Normal!=NULL)
	{
		DeleteObject(m_hf18Normal);
		m_hf18Normal=NULL;
	}
	if(m_hf18Bold!=NULL)
	{
		DeleteObject(m_hf18Bold);
		m_hf18Bold=NULL;
	}
	if(m_hf18BoldNum!=NULL)
	{
		DeleteObject(m_hf18BoldNum);
		m_hf18BoldNum=NULL;
	}
	if(m_hf20Bold!=NULL)
	{
		DeleteObject(m_hf20Bold);
		m_hf20Bold=NULL;
	}
	if(m_hf20BoldNum!=NULL)
	{
		DeleteObject(m_hf20BoldNum);
		m_hf20BoldNum=NULL;
	}
	if(m_hf21Medium!=NULL)
	{
		DeleteObject(m_hf21Medium);
		m_hf21Medium=NULL;
	}
	if(m_hf21Bold!=NULL)
	{
		DeleteObject(m_hf21Bold);
		m_hf21Bold=NULL;
	}
	if(m_hf22Medium!=NULL)
	{
		DeleteObject(m_hf22Medium);
		m_hf22Medium=NULL;
	}
	if(m_hf22Bold!=NULL)
	{
		DeleteObject(m_hf22Bold);
		m_hf22Bold=NULL;
	}
	if(m_hf24Bold!=NULL)
	{
		DeleteObject(m_hf24Bold);
		m_hf24Bold=NULL;
	}
	if(m_hf26Medium!=NULL)
	{
		DeleteObject(m_hf26Medium);
		m_hf26Medium=NULL;
	}
	if(m_hf28Bold!=NULL)
	{
		DeleteObject(m_hf28Bold);
		m_hf28Bold=NULL;
	}
	if(m_hf30Bold!=NULL)
	{
		DeleteObject(m_hf30Bold);
		m_hf30Bold=NULL;
	}
	if(m_hf32Medium!=NULL)
	{
		DeleteObject(m_hf32Medium);
		m_hf32Medium=NULL;
	}
	if(m_hf34Bold!=NULL)
	{
		DeleteObject(m_hf34Bold);
		m_hf34Bold=NULL;
	}
	if(m_hf34BoldNum!=NULL)
	{
		DeleteObject(m_hf34BoldNum);
		m_hf34BoldNum=NULL;
	}
	if(m_hf38Bold!=NULL)
	{
		DeleteObject(m_hf38Bold);
		m_hf38Bold=NULL;
	}
	if(m_hf40Bold!=NULL)
	{
		DeleteObject(m_hf40Bold);
		m_hf40Bold=NULL;
	}
	if(m_hf50Bold!=NULL)
	{
		DeleteObject(m_hf50Bold);
		m_hf50Bold=NULL;
	}
	if(m_hf60Bold!=NULL)
	{
		DeleteObject(m_hf60Bold);
		m_hf60Bold=NULL;
	}
	if(m_hf70Bold!=NULL)
	{
		DeleteObject(m_hf70Bold);
		m_hf70Bold=NULL;
	}
	if(m_hf31AcuBoldNum!=NULL)
	{
		DeleteObject(m_hf31AcuBoldNum);
		m_hf31AcuBoldNum=NULL;
	}
	if(m_hf33AcuBoldNum!=NULL)
	{
		DeleteObject(m_hf33AcuBoldNum);
		m_hf33AcuBoldNum=NULL;
	}
	if(m_hf70BoldNum!=NULL)
	{
		DeleteObject(m_hf70BoldNum);
		m_hf70BoldNum=NULL;
	}

	m_hf10Bold=CreateFontHandle(&dc,10,m_pszFontName,FW_BOLD);
	m_hf13Bold=CreateFontHandle(&dc,13,m_pszFontName,FW_BOLD);
	m_hf10Norm=CreateFontHandle(&dc,10,m_pszFontName,FW_NORMAL);
	m_hf11Norm=CreateFontHandle(&dc,11,m_pszFontName,FW_NORMAL);
	m_hf12Norm=CreateFontHandle(&dc,12,m_pszFontName,FW_NORMAL);
	m_hf12Norm90degree=CreateFontHandle(&dc,-((8*dc.GetDeviceCaps(LOGPIXELSY))/72),m_pszFontName,FW_NORMAL,900);
	m_hf13Norm=CreateFontHandle(&dc,13,m_pszFontName,FW_NORMAL);
	m_hf14Norm=CreateFontHandle(&dc,14,m_pszFontName,FW_NORMAL);
	m_hf14Bold=CreateFontHandle(&dc,14,m_pszFontName,FW_BOLD);
	m_hf14Bold90degree=CreateFontHandle(&dc,-((9*dc.GetDeviceCaps(LOGPIXELSY))/72),m_pszFontName,FW_NORMAL,900);
	m_hf15Normal=CreateFontHandle(&dc,15,m_pszFontName,FW_NORMAL);
	m_hf15Bold=CreateFontHandle(&dc,15,m_pszFontName,FW_BOLD);
	m_hf16Bold=CreateFontHandle(&dc,16,m_pszFontName,FW_BOLD);
	m_hf16Normal=CreateFontHandle(&dc,16,m_pszFontName,FW_NORMAL);
	m_hf16Bold90degree=CreateFontHandle(&dc,-((10*dc.GetDeviceCaps(LOGPIXELSY))/72),m_pszFontName,FW_NORMAL,900);
	m_hf17Bold=CreateFontHandle(&dc,17,m_pszFontName,FW_BOLD);
	m_hf18Normal=CreateFontHandle(&dc,18,m_pszFontName,FW_NORMAL);
	m_hf18Bold=CreateFontHandle(&dc,18,m_pszFontName,FW_BOLD);
	m_hf20Bold=CreateFontHandle(&dc,20,m_pszFontName,FW_BOLD);
	m_hf21Medium=CreateFontHandle(&dc,21,m_pszFontName,FW_MEDIUM);
	m_hf21Bold=CreateFontHandle(&dc,21,m_pszFontName,FW_BOLD);
	m_hf22Medium=CreateFontHandle(&dc,22,m_pszFontName,FW_MEDIUM);
	m_hf22Bold=CreateFontHandle(&dc,22,m_pszFontName,FW_BOLD);
	m_hf24Bold=CreateFontHandle(&dc,24,m_pszFontName,FW_BOLD);
	m_hf26Medium=CreateFontHandle(&dc,26,m_pszFontName,FW_MEDIUM);
	m_hf28Bold=CreateFontHandle(&dc,28,m_pszFontName,FW_BOLD);
	m_hf30Bold=CreateFontHandle(&dc,30,m_pszFontName,FW_BOLD);
	m_hf32Medium=CreateFontHandle(&dc,32,m_pszFontName,FW_MEDIUM);
	m_hf34Bold=CreateFontHandle(&dc,34,m_pszFontName,FW_BOLD);
	m_hf38Bold=CreateFontHandle(&dc,38,m_pszFontName,FW_BOLD);
	m_hf40Bold=CreateFontHandle(&dc,40,m_pszFontName,FW_BOLD);
	m_hf50Bold=CreateFontHandle(&dc,50,m_pszFontName,FW_BOLD);
	m_hf60Bold=CreateFontHandle(&dc,60,m_pszFontName,FW_BOLD);
	m_hf70Bold=CreateFontHandle(&dc,70,m_pszFontName,FW_BOLD);

	m_hf31AcuBoldNum=CreateFontHandle(&dc,38,m_pszFontName,FW_BOLD);
	m_hf33AcuBoldNum=CreateFontHandle(&dc,40,m_pszFontName,FW_BOLD);
	m_hf70BoldNum=CreateFontHandle(&dc,70,m_pszFontName,FW_BOLD);
	m_hf14AcuNormNum=CreateFontHandle(&dc,14,m_pszFontName,FW_NORMAL);
	m_hf12AcuNormNum=CreateFontHandle(&dc,12,m_pszFontName,FW_NORMAL);
	m_hf34BoldNum=CreateFontHandle(&dc,34,m_pszFontName,FW_BOLD);
	m_hf20BoldNum=CreateFontHandle(&dc,20,m_pszFontName,FW_BOLD);
	m_hf18BoldNum=CreateFontHandle(&dc,18,m_pszFontName,FW_BOLD);

	/*m_hf31AcuBoldNum=CreateFontHandle(&dc,38,_T("arial"),FW_BOLD);
	m_hf33AcuBoldNum=CreateFontHandle(&dc,40,_T("arial"),FW_BOLD);
	m_hf70BoldNum=CreateFontHandle(&dc,70,_T("arial"),FW_BOLD);
	m_hf14AcuNormNum=CreateFontHandle(&dc,14,_T("arial"),FW_NORMAL);
	m_hf34BoldNum=CreateFontHandle(&dc,34,_T("arial"),FW_BOLD);
	m_hf20BoldNum=CreateFontHandle(&dc,20,_T("arial"),FW_BOLD);
	m_hf18BoldNum=CreateFontHandle(&dc,18,_T("arial"),FW_BOLD);*/

	return wLanguageID;
}

/**********************************************************************************************//**
 * Loads global acu fonts
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	wLanguageID	Identifier for the language.
 **************************************************************************************************/

void CMainFrame::LoadGlobalAcuFonts(WORD wLanguageID)
{
	switch(wLanguageID)
	{
	case LAN_JAPANESE:
		{
			g_hf5AcuNorm=m_hf10Norm;//m_hf12Norm;
			g_hf6AcuNorm=m_hf10Norm;//m_hf13Norm;
			g_hf6AcuBold=m_hf10Bold;
			g_hf7AcuNorm=m_hf12Norm;//m_hf15Normal;
			g_hf7AcuBold=m_hf12Norm;//m_hf16Normal;
			g_hf7AcuBold90degree=m_hf12Norm90degree;
			g_hf8AcuNorm=m_hf13Norm;//m_hf16Normal;
			g_hf8AcuBold=m_hf14Norm;//m_hf16Normal;
			g_hf9AcuBold=m_hf14Norm;//m_hf16Bold;
			g_hf10AcuBold=m_hf14Norm;//m_hf16Normal;
			g_hf11AcuBold=m_hf14Bold;//m_hf17Bold;
			g_hf11AcuBoldNum=m_hf18BoldNum;
			g_hf13AcuBold=m_hf14Bold;//m_hf22Bold;
			g_hf14AcuMed=m_hf15Bold;//m_hf22Bold;
			g_hf14AcuBold=m_hf15Bold;//m_hf21Bold;
			//g_hf14AcuNormNum=m_hf14AcuNormNum;
			g_hf14AcuNormNum=m_hf12AcuNormNum;
			g_hf15AcuMed=m_hf16Bold;//m_hf24Bold;
			g_hf17AcuBold=m_hf22Bold;//m_hf26Medium;
			g_hf19AcuMed=m_hf24Bold;//m_hf28Bold;
			g_hf21AcuBold=m_hf24Bold;//m_hf28Bold;
			g_hf23AcuBold=m_hf24Bold;//m_hf30Bold;
			g_hf25AcuMed=m_hf30Bold;//m_hf34Bold;
			g_hf27AcuBold=m_hf30Bold;//m_hf34Bold;
			g_hf31AcuBold=m_hf32Medium;//m_hf38Bold;
			g_hf31AcuBoldNum=m_hf31AcuBoldNum;
			g_hf33AcuBold=m_hf34Bold;//m_hf40Bold;
			g_hf33AcuBoldNum=m_hf33AcuBoldNum;
			g_hf43AcuBold=m_hf40Bold;//m_hf50Bold;
			g_hf53AcuBold=m_hf50Bold;//m_hf60Bold;
			g_hf70Bold=m_hf60Bold;//m_hf70Bold;
			g_hf70BoldNum=m_hf70BoldNum;
			g_hf34BoldNum=m_hf34BoldNum;
			g_hf13AcuBoldNum=m_hf20BoldNum;
		}
		break;
	case LAN_CHINESE:
		{
			g_hf5AcuNorm=m_hf12Norm;
			g_hf6AcuNorm=m_hf13Norm;
			g_hf6AcuBold=m_hf13Bold;
			g_hf7AcuNorm=m_hf15Normal;//m_hf14Norm;
			g_hf7AcuBold=m_hf16Bold;//m_hf14Bold;
			g_hf7AcuBold90degree=m_hf16Bold90degree;
			g_hf8AcuNorm=m_hf16Normal;//m_hf15Normal;
			g_hf8AcuBold=m_hf16Normal;//m_hf15Bold;
			g_hf9AcuBold=m_hf16Bold;
			g_hf10AcuBold=m_hf18Bold;//m_hf17Bold;
			g_hf11AcuBold=m_hf20Bold;//m_hf18Bold;
			g_hf11AcuBoldNum=m_hf18BoldNum;
			g_hf13AcuBold=m_hf22Bold;//m_hf20Bold;
			g_hf14AcuMed=m_hf22Bold;//m_hf21Medium;
			g_hf14AcuBold=m_hf24Bold;//m_hf21Bold;
			g_hf14AcuNormNum=m_hf14AcuNormNum;
			g_hf15AcuMed=m_hf24Bold;//m_hf22Medium;
			g_hf17AcuBold=m_hf26Medium;//m_hf24Bold;
			g_hf19AcuMed=m_hf28Bold;//m_hf26Medium;
			g_hf21AcuBold=m_hf28Bold;
			g_hf23AcuBold=m_hf30Bold;
			g_hf25AcuMed=m_hf34Bold;//m_hf32Medium;
			g_hf27AcuBold=m_hf34Bold;
			g_hf31AcuBold=m_hf38Bold;
			g_hf31AcuBoldNum=m_hf31AcuBoldNum;
			g_hf33AcuBold=m_hf40Bold;
			g_hf33AcuBoldNum=m_hf33AcuBoldNum;
			g_hf43AcuBold=m_hf50Bold;
			g_hf53AcuBold=m_hf60Bold;
			g_hf70Bold=m_hf70Bold;
			g_hf70BoldNum=m_hf70BoldNum;
			g_hf34BoldNum=m_hf34BoldNum;
			g_hf13AcuBoldNum=m_hf20BoldNum;
		}
		break;
	default:
		{
			g_hf5AcuNorm=m_hf12Norm;
			g_hf6AcuNorm=m_hf13Norm;
			g_hf6AcuBold=m_hf13Bold;
			g_hf7AcuNorm=m_hf14Norm;
			g_hf7AcuBold=m_hf14Bold;
			g_hf7AcuBold90degree=m_hf14Bold90degree;
			g_hf8AcuNorm=m_hf15Normal;
			g_hf8AcuBold=m_hf15Bold;
			g_hf9AcuBold=m_hf16Bold;
			g_hf10AcuBold=m_hf17Bold;
			g_hf11AcuBold=m_hf18Bold;
			g_hf11AcuBoldNum=m_hf18BoldNum;
			g_hf13AcuBold=m_hf20Bold;
			g_hf14AcuMed=m_hf21Medium;
			g_hf14AcuBold=m_hf21Bold;
			g_hf14AcuNormNum=m_hf14AcuNormNum;
			g_hf15AcuMed=m_hf22Medium;
			g_hf17AcuBold=m_hf24Bold;
			g_hf19AcuMed=m_hf26Medium;
			g_hf21AcuBold=m_hf28Bold;
			g_hf23AcuBold=m_hf30Bold;
			g_hf25AcuMed=m_hf32Medium;
			g_hf27AcuBold=m_hf34Bold;
			g_hf31AcuBold=m_hf38Bold;
			g_hf31AcuBoldNum=m_hf31AcuBoldNum;
			g_hf33AcuBold=m_hf40Bold;
			g_hf33AcuBoldNum=m_hf33AcuBoldNum;
			g_hf43AcuBold=m_hf50Bold;
			g_hf53AcuBold=m_hf60Bold;
			g_hf70Bold=m_hf70Bold;
			g_hf70BoldNum=m_hf70BoldNum;
			g_hf34BoldNum=m_hf34BoldNum;
			g_hf13AcuBoldNum=m_hf20BoldNum;
		}
		break;
	}
	
}

/**********************************************************************************************//**
 * Registers the ffsdisk fonts
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::RegisterFFSDISKFonts()
{
	HANDLE hSearch;
	WIN32_FIND_DATA fileData;
	//BOOL bFinished = false;
	TCHAR szFileName[255];

	hSearch = FindFirstFile(_T("\\ffsdisk\\*.ttf"), &fileData);

	if (hSearch != INVALID_HANDLE_VALUE)
	{
		do
		{
			swprintf(szFileName/*,sizeof(szFileName)*/, _T("\\ffsdisk\\%s"), fileData.cFileName);

			// Prevent mulitple addition for the same font
			RemoveFontResource(szFileName);

			AddFontResource (szFileName);
			
		}
		while (FindNextFile (hSearch, &fileData) && !m_bExit);

		FindClose(hSearch);
		
	}

	
	//CloseHandle (hSearch);
	hSearch = NULL;

	//::SendMessage(HWND_BROADCAST,WM_FONTCHANGE,0,0);
	//delete fileData;
}

/**********************************************************************************************//**
 * Registers the SD card fonts
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::RegisterSDCardFonts()
{
	HANDLE hSearch;
	WIN32_FIND_DATA fileData;
	//BOOL bFinished = false;
	TCHAR szFileName[255];

	hSearch = FindFirstFile(_T("\\sdcard\\fonts\\*.*"), &fileData);

	if (hSearch != INVALID_HANDLE_VALUE)
	{
		do
		{
			swprintf(szFileName/*,sizeof(szFileName)*/, _T("\\sdcard\\fonts\\%s"), fileData.cFileName);

			// Prevent mulitple addition for the same font
			RemoveFontResource(szFileName);

			AddFontResource (szFileName);
			
		}
		while (FindNextFile (hSearch, &fileData) && !m_bExit);

		FindClose(hSearch);

	}

	


	//CloseHandle (hSearch);
	hSearch = NULL;

	//::SendMessage(HWND_BROADCAST,WM_FONTCHANGE,0,0);
	//delete fileData;
}

/**********************************************************************************************//**
 * Unregisters the ffsdisk fonts
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::UnregisterFFSDISKFonts()
{
	HANDLE hSearch;
	WIN32_FIND_DATA fileData;
	//BOOL bFinished = false;
	TCHAR szFileName[255];

	hSearch = FindFirstFile(_T("\\ffsdisk\\*.ttf"), &fileData);

	if (hSearch != INVALID_HANDLE_VALUE)
	{
		do
		{
			swprintf(szFileName/*,sizeof(szFileName)*/, _T("\\ffsdisk\\%s"), fileData.cFileName);

			// Prevent mulitple addition for the same font
			RemoveFontResource(szFileName);

			//AddFontResource (szFileName);
			
		}
		while (FindNextFile (hSearch, &fileData) && !m_bExit);

		FindClose(hSearch);

	}


	//CloseHandle (hSearch);
	hSearch = NULL;

	::SendMessage(HWND_BROADCAST,WM_FONTCHANGE,0,0);
	//delete fileData;
}

/**********************************************************************************************//**
 * Unregisters the SD card fonts
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::UnregisterSDCardFonts()
{
	HANDLE hSearch;
	WIN32_FIND_DATA fileData;
	//BOOL bFinished = false;
	TCHAR szFileName[255];

	hSearch = FindFirstFile(_T("\\sdcard\\fonts\\*.ttf"), &fileData);

	if (hSearch != INVALID_HANDLE_VALUE)
	{
		do
		{
			swprintf(szFileName/*,sizeof(szFileName)*/, _T("\\sdcard\\fonts\\%s"), fileData.cFileName);

			// Prevent mulitple addition for the same font
			RemoveFontResource(szFileName);

			//AddFontResource (szFileName);
			
		}
		while (FindNextFile (hSearch, &fileData) && !m_bExit);

		FindClose(hSearch);

	}


	//CloseHandle (hSearch);
	hSearch = NULL;

	::SendMessage(HWND_BROADCAST,WM_FONTCHANGE,0,0);
	//delete fileData;
}

/**********************************************************************************************//**
 * Creates font handle
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pDC				If non-null, the device-context.
 * \param 		  	nPixHeight  	Height of the pix.
 * \param [in,out]	pszFacename 	If non-null, the facename.
 * \param 		  	lFontWeight 	The font weight.
 * \param 		  	iOrientation	Zero-based index of the orientation.
 *
 * \return	The new font handle.
 **************************************************************************************************/

HFONT CMainFrame::CreateFontHandle(CDC* pDC, int nPixHeight, TCHAR* pszFacename, LONG lFontWeight, int iOrientation) 
{
	// Standard settings
	LOGFONT lf;
	lf.lfWidth = 0; 
	lf.lfEscapement = iOrientation; 
	lf.lfOrientation = iOrientation; 
	lf.lfItalic = FALSE; 
	lf.lfUnderline = FALSE; 
	lf.lfStrikeOut = FALSE; 
	lf.lfCharSet = DEFAULT_CHARSET; 
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS; 
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	//lf.lfQuality = DEFAULT_QUALITY; 
	lf.lfQuality = ANTIALIASED_QUALITY; 
	//lf.lfQuality = CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily = FF_ROMAN;
	// To modify
	lf.lfWeight=lFontWeight;
	_tcscpy_s(lf.lfFaceName,_countof(lf.lfFaceName),pszFacename);
	//lf.lfHeight = -((nPixHeight*pDC->GetDeviceCaps(LOGPIXELSY))/72); // in Pixel; 
	lf.lfHeight = nPixHeight; // in Pixel;

	return CreateFontIndirect( &lf );
}

/**********************************************************************************************//**
 * Definition window proc
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	message	The message.
 * \param	wParam 	The wParam field of the message.
 * \param	lParam 	The lParam field of the message.
 *
 * \return	A LRESULT.
 **************************************************************************************************/

LRESULT CMainFrame::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
#ifndef _DEBUG
	if(!g_bMainWatchdogPending)
	{
		if(!RefreshWatchDogTimer(g_hWatchDog,0))
		{
			//theApp.writeLogError(GetLastError(), _T("WD: Unable to refresh#3"));
		}
	}
#endif

	//EnterCriticalSection(&csThreadWatchdog);
	//faThreadWatchdog[THR_MAIN]=faThreadWatchdog[THR_MAIN]+1;
	//LeaveCriticalSection(&csThreadWatchdog);
	
	try
	{
	
		switch (message)
		{
		case WM_START_I2CWATCHDOG:
			{
				//StartI2CWatchdogThread();
				EnterCriticalSection(&m_csI2Cinit);
				m_bI2Cinitialized=true;
				LeaveCriticalSection(&m_csI2Cinit);
				return 1;
			}
		case WM_TERMINAL_IPPV:
			{
				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_IPPV))
					theApp.getLog()->WriteLine(_T("#HFO:T400"));
				return 1;
			}
			break;
		case WM_TERMINAL_SIPPV:
			{
				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_SIPPV))
					theApp.getLog()->WriteLine(_T("#HFO:T401"));
				return 1;
			}
			break;
		case WM_TERMINAL_SIMV:
			{
				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_SIMV))
					theApp.getLog()->WriteLine(_T("#HFO:T402"));
				return 1;
			}
			break;
		case WM_TERMINAL_SIMVPSV:
			{
				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_SIMVPSV))
					theApp.getLog()->WriteLine(_T("#HFO:T403"));
				return 1;
			}
			break;
		case WM_TERMINAL_PSV:
			{
				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_PSV))
					theApp.getLog()->WriteLine(_T("#HFO:T404"));
				return 1;
			}
			break;
		case WM_TERMINAL_CPAP:
			{
				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_CPAP))
					theApp.getLog()->WriteLine(_T("#HFO:T405"));
				return 1;
			}
			break;
		case WM_TERMINAL_NCPAP:
			{
				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_NCPAP))
					theApp.getLog()->WriteLine(_T("#HFO:T406"));
				return 1;
			}
			break;
		case WM_TERMINAL_DUOPAP:
			{
				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_DUOPAP))
					theApp.getLog()->WriteLine(_T("#HFO:T407"));
				return 1;
			}
			break;
		case WM_TERMINAL_HFO:
			{
				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_HFO))
					theApp.getLog()->WriteLine(_T("#HFO:T408"));
				return 1;
			}
			break;
		case WM_TERMINAL_THERAPY:
			{
				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_THERAPIE))
					theApp.getLog()->WriteLine(_T("#HFO:T409"));
				return 1;
			}
			break;

		case WM_RETRYCO2:
			{
				getModel()->DeinitCO2module();
				Sleep(0);
				getModel()->initCO2module();
				return 1;
			}
		case WM_FABAIN_SHUTDOWN:
			{
				m_bExit=true;

				theApp.getLog()->WriteLine(L"**** BROADCAST QuitVentilator ****");

				//getModel()->Quit();

				m_bForceShutdown=true;
				PostMessage(WM_QUIT_VENTILATOR);

				return 1;
			}
		case WM_CAL_DISPLAY:
			{
				//getModel()->SetMainthreadPending(true);
				g_bMainWatchdogPending=true;
	#ifndef _DEBUG
				KillTimer(WATCHDOGTIMER);

				if(!StopWatchDogTimer(g_hWatchDog,0))
				{
					// TODO:		
					//DWORD dwError=GetLastError();
					//return;
				}
				CloseHandle(g_hWatchDog);

	#endif

				if(TouchCalibrate()==FALSE)
				{
					//int iSt=0;//Error
				}
				else
					RegFlushKey(HKEY_LOCAL_MACHINE);

	#ifndef _DEBUG
				DWORD dwID;

				//g_hWatchDog=CreateWatchDogTimer(WD_NAME,WD_PERIODE,WD_WAIT,WDOG_RESET_DEVICE,0,0);
				//g_hWatchDog=CreateWatchDogTimer(WD_NAME,WD_PERIODE,WD_WAIT,WDOG_KILL_PROCESS,0,0);
				g_hWatchDog=CreateWatchDogTimer(WD_NAME,WD_PERIODE,WD_WAIT,WDOG_NO_DFLT_ACTION,0,0);

				g_hAction=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Action,NULL,NULL,&dwID);
				// TODO: create thread failed

				if(g_hWatchDog==NULL)
				{
					// TODO:
					//DWORD dwError=GetLastError();
					return 0;
				}
				if(!StartWatchDogTimer(g_hWatchDog,0))
				{
					// TODO:		
					//DWORD dwError=GetLastError();
					return 0;
				}

				//StartMainWatchdogThread();


				if(!RefreshWatchDogTimer(g_hWatchDog,0))
				{
					theApp.writeLogError(GetLastError(), _T("WD: Unable to refresh#4"));
				}
	#endif
				SetTimer(WATCHDOGTIMER,2000,NULL);
				g_bMainWatchdogPending=false;

				getModel()->getVIEWHANDLER()->UpdateServiceView();

				return 1;
			}
			break;
		case WM_MENUBUTTONDOWN:
			{
				switch(wParam)
				{
				case IDC_BTN_MENU_IPPV:
					{
						theApp.getLog()->WriteLine(_T("$M20$"));

						//test fmea
						//getModel()->closeInterface();

						//test deadlock
						/*while(true)
						{
							int i=0;
						}*/

						//test memory leak
						/*m_pcTest=NULL;
						CClientDC dc(this);
						int iCount=0;
						while(true)
						{
							m_pcTest= new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_PIC_SYSALARM);

							Sleep(1000);
							iCount++;

							if(iCount>10)
							{
								iCount=0;
								MEMORYSTATUS stat;
								GlobalMemoryStatus (&stat);
								DWORD dwMem=stat.dwAvailPhys/1024;

								CString szTemp=_T("");
								szTemp.Format(_T("%*ld free "),7, stat.dwAvailPhys/1024);
								DEBUGMSG(TRUE, (szTemp));
								DEBUGMSG(TRUE, (TEXT("\r\n")));
							}

						}*/

						//HANDLE hDriver;

						//DeviceSearchType DeviceSearch = DeviceSearchByLegacyName;
						//DEVMGR_DEVICE_INFORMATION DeviceInfo;

						//DeviceInfo.dwSize = sizeof( DEVMGR_DEVICE_INFORMATION );

						//hDriver = FindFirstDevice(DeviceSearch, L"*", &DeviceInfo);

						//if( hDriver != INVALID_HANDLE_VALUE )
						//{
						//	//RETAILMSG( 1, (TEXT("Legacy\n")));
						//	do
						//	{
						//		RETAILMSG( 1, (TEXT("%s\n"), 
						//			DeviceInfo.szLegacyName ));
						//		CString sz=DeviceInfo.szLegacyName;
						//		if(sz==_T("PIF1:"))
						//		{
						//			BOOL bTest=DeactivateDevice(hDriver);
						//			int iStop=0;

						//		}
						//	} while( FindNextDevice( hDriver, &DeviceInfo ) );
						//	FindClose( hDriver );
						//}

						CMVEventUI event(CMVEventUI::EV_BN_IPPV);
						getModel()->triggerEvent(&event);
						return 1;
					}
					break;
				case IDC_BTN_MENU_SIPPV:
					{
						theApp.getLog()->WriteLine(_T("$M21$"));
						CMVEventUI event(CMVEventUI::EV_BN_SIPPV);
						getModel()->triggerEvent(&event);
						return 1;
					}
					break;
				case IDC_BTN_MENU_SIMV:
					{
						theApp.getLog()->WriteLine(_T("$M22$"));
						CMVEventUI event(CMVEventUI::EV_BN_SIMV);
						getModel()->triggerEvent(&event);
						return 1;
					}
					break;
				case IDC_BTN_MENU_SIMVPSV:
					{
						theApp.getLog()->WriteLine(_T("$M23$"));
						CMVEventUI event(CMVEventUI::EV_BN_SIMVPSV);
						getModel()->triggerEvent(&event);
						return 1;
					}
					break;
				case IDC_BTN_MENU_PSV:
					{
						theApp.getLog()->WriteLine(_T("$M24$"));
						CMVEventUI event(CMVEventUI::EV_BN_PSV);
						getModel()->triggerEvent(&event);
						return 1;
					}
					break;
				case IDC_BTN_MENU_HFO:
					{
						theApp.getLog()->WriteLine(_T("$M25$"));
						CMVEventUI event(CMVEventUI::EV_BN_HFO);
						getModel()->triggerEvent(&event);
						return 1;
					}
					break;
				case IDC_BTN_MENU_CPAP:
					{
						theApp.getLog()->WriteLine(_T("$M26$"));
						CMVEventUI event(CMVEventUI::EV_BN_CPAP);
						getModel()->triggerEvent(&event);
						return 1;
					}
					break;
				case IDC_BTN_MENU_NCPAP:
					{
						theApp.getLog()->WriteLine(_T("$M27$"));
						CMVEventUI event(CMVEventUI::EV_BN_NCPAP);
						getModel()->triggerEvent(&event);
						return 1;
					}
					break;
				case IDC_BTN_MENU_DUOPAP:
					{
						theApp.getLog()->WriteLine(_T("$M28$"));
						CMVEventUI event(CMVEventUI::EV_BN_DUOPAP);
						getModel()->triggerEvent(&event);
						return 1;
					}
					break;
				case IDC_BTN_MENU_THERAPIE:
					{
						theApp.getLog()->WriteLine(_T("$M29$"));
						CMVEventUI event(CMVEventUI::EV_BN_THERAPIE);
						getModel()->triggerEvent(&event);
						return 1;
					}
					break;
				}
			}
			break;
		case WM_SAVETREND_TOUSB:
			{
				StartSaveTrendUSBThread();
				eventSaveTrendUSBfinish.SetEvent();
				return 1;
			}
			break;
		//case WM_LOAD_LANGUAGE:
		//	{
		//		/*CreateWndHourglass();
		//		ShowWndHourglass(true);

		//		m_wLanguageToLoad=wParam;
		//		StartLoadLanguageThread();
		//		eventLoadLanguage.SetEvent();*/

		//		if(getModel()->GetLanguageID()!=m_wLanguageID)
		//		{
		//			bool bChangeFace=false;
		//			if(m_wLanguageID==LAN_CHINESE)
		//			{
		//				bChangeFace=true;
		//			}
		//			else if(getModel()->GetLanguageID()==LAN_CHINESE)
		//			{
		//				bChangeFace=true;
		//			}

		//			if(bChangeFace)
		//			{
		//				CreateAcuFonts(getModel()->GetLanguageID());
		//				LoadGlobalAcuFonts(getModel()->GetLanguageID());
		//				m_wLanguageID=getModel()->GetLanguageID();
		//			}
		//		}
		//	}
		//	break;
		case WM_LANGUAGE_CHANGED:
			{
				if(getModel()->GetLanguageID()!=m_wLanguageID)
				{
					bool bChangeFace=false;
					if(m_wLanguageID==LAN_CHINESE &&  getModel()->GetLanguageID()!=LAN_JAPANESE)
					{
						bChangeFace=true;
					}
					else if(getModel()->GetLanguageID()==LAN_CHINESE && m_wLanguageID!=LAN_JAPANESE)
					{
						bChangeFace=true;
					}
					else if(m_wLanguageID==LAN_JAPANESE &&  getModel()->GetLanguageID()!=LAN_CHINESE)
					{
						bChangeFace=true;
					}
					else if(getModel()->GetLanguageID()==LAN_JAPANESE && m_wLanguageID!=LAN_CHINESE)
					{
						bChangeFace=true;
					}

					if(bChangeFace)
					{
						CreateAcuFonts(getModel()->GetLanguageID(),true);
						LoadGlobalAcuFonts(getModel()->GetLanguageID());
						m_wLanguageID=getModel()->GetLanguageID();

						::SendMessage(HWND_BROADCAST,WM_FONTCHANGE,0,0);
					}

				}

				

				/*CMVEventUI event(CMVEventUI::EV_LANGUAGE);
				getModel()->Trigger(&event);*/
				return 1;
			}
			break;
		case WM_FONTCHANGE:
			{
				if(!m_bExit)
				{
					CMVEventUI event(CMVEventUI::EV_LANGUAGE);
					getModel()->triggerEvent(&event);
				}
			}
			break;
		case WM_EXCEPTION:
			{
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, _T("! EXCEPTION: please restart ASAP !"), 5000);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				Sleep(300);
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				return 1;
			}
			break;
		case WM_VIDEO_STOP:
			{
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, _T("! video stopped !"), 3500);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				Sleep(300);
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				return 1;
			}
			break;
		case WM_PDMS_ACTIVE:
			{
				//CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, _T("! NOT POSSIBLE, PDMS IS ACTIVE !"), 3500);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, getModel()->GetLanguageString(IDS_TXT_PDMSACTIVE), 3500);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				Sleep(300);
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				return 1;
			}
			break;
		case WM_NO_SDCARD:
			{
				//CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, _T("!!! no SD card available !!!"), 3500);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, getModel()->GetLanguageString(IDS_TXT_NOSDCARD), 3500);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				Sleep(300);
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				return 1;
			}
			break;
		case WM_LOADHOSPITALSETTINGS_FAIL:
			{
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, getModel()->GetLanguageString(IDS_TXT_DIFF_CONFIGURATION), 3500);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				Sleep(300);
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				return 1;
			}
			break;
		/*case WM_EV_TIMETEXT_3SECPUSH:
			{
				CMVEventInfotext event2(CMVEventInfotext::EV_TIMETEXT, getModel()->GetLanguageString(IDS_TXT_3SECPUSH), 3500);
				getModel()->triggerEvent(&event2);
				return 1;
			}
			break;*/
		case WM_EV_TIMETEXT_RELOADCONFIG:
			{
				CStringW sData = getModel()->GetLanguageString(IDS_TXT_CONFIGLOADED);
				CMVEventInfotext event2(CMVEventInfotext::EV_TIMETEXT,  sData, 3000);
				getModel()->triggerEvent(&event2);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);

				Sleep(2000);
				AfxGetApp()->GetMainWnd()->PostMessage(WM_LANGUAGE_CHANGED);
				return 1;
			}
			break;
		case WM_SPO2HIGH_LIMITEDLOW:
			{
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, getModel()->GetLanguageString(IDS_TXT_SPO2_LOWRANGE), 3500); 
				//CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, _T("limitted by SpO2 low range"), 3500);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				Sleep(300);
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
			}
			break;
		case WM_SPO2HIGH_LIMITEDHIGH:
		case WM_SPO2LOW_LIMITEDLOW:
			{
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, getModel()->GetLanguageString(IDS_TXT_SPO2_LIMITTED), 3500);
				//CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, _T("limitted by SpO2 alarm limit"), 3500);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				Sleep(300);
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
			}
			break;
		case WM_SPO2LOW_LIMITEDHIGH:
			{
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, getModel()->GetLanguageString(IDS_TXT_SPO2_HIGHRANGE), 3500);
				//CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, _T("limitted by SpO2 high range"), 3500);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				Sleep(300);
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
			}
			break;
		case WM_EV_TIMETEXT_ERR_DATE:
			{
				CStringW sData = getModel()->GetLanguageString(IDS_ERR_DATE);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TIMETEXT_ERR_YEAR:
			{
				CStringW sData = getModel()->GetLanguageString(IDS_ERR_YEAR);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TIMETEXT_ERR_PERSID:
			{
				CStringW sData = getModel()->GetLanguageString(IDS_ERR_PERSID);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TIMETEXT_PATDATASAVE:
			{
				CStringW sData = getModel()->GetLanguageString(IDS_TXT_PATDATASAVE);

				CMVEventInfotext event2(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->triggerEvent(&event2);
				return 1;
			}
			break;
		case WM_EV_TIMETEXT_CONFIRMRANGE:
			{
				CStringW sData = getModel()->GetLanguageString(IDS_TXT_CONFIRMRANGE);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TIMETEXT_CONFIRMLOWRANGE:
			{
				CStringW sData = getModel()->GetLanguageString(IDS_TXT_CONFIRMLOWRANGE);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TIMETEXT_CONFIRMHIGHRANGE:
			{
				CStringW sData = getModel()->GetLanguageString(IDS_TXT_CONFIRMHIGHRANGE);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TIMETEXT_NOTAVAILABLE:
			{
				CStringW sData = getModel()->GetLanguageString(IDS_TXT_NOTAVAILABLE);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				return 1;
			}
			break;
		case WM_EV_TIMETEXT_SCALELIMIT:
			{
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  getModel()->GetLanguageString(IDS_TXT_SCALELIMIT), 2000);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				return 1;
			}
			break;
		case WM_EV_TIMETEXT_ENDRANGE:
			{
				CStringW sData = getModel()->GetLanguageString(IDS_TXT_ENDRANGE);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				return 1;
			}
			break;
		case WM_EV_PATDATA_CHANGED:
			{
				CMVEventUI event(CMVEventUI::EV_PATDATA_CHANGED);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_NUMERIC:
			{
				CMVEventUI event(CMVEventUI::EV_BN_NUMERIC);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		//case WM_EV_ALIMIT_NUMERIC:
		//	{
		//		getModel()->getVIEWHANDLER()->xxx;
		//		/*CMVEventUI event(CMVEventUI::EV_BN_NUMERIC);
		//		getModel()->Trigger(&event);*/
		//		return 1;
		//	}
		//	break;
		case WM_EV_BN_GRAPH_FREEZED:
			{
				CMVEventUI event(CMVEventUI::EV_BN_GRAPH_FREEZED);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_GRAPH_CONTINUE:
			{
				CMVEventUI event(CMVEventUI::EV_BN_GRAPH_CONTINUE);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_GRAPH_WAVE:
			{
				CMVEventUI event(CMVEventUI::EV_BN_GRAPH_WAVE);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TEXT_SAVINGTRENDS:
			{
				CStringW sz = getModel()->GetLanguageString(IDS_TREND_SAVEING);

				CMVEventInfotext event(CMVEventInfotext::EV_TEXT,  sz, 0);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TEXT_SERVICE_SYSTEM:
			{
				CStringW sz = getModel()->GetLanguageString(IDS_SERVICE);
				sz+=_T(" - System");

				CMVEventInfotext event(CMVEventInfotext::EV_TEXT,  sz, 0);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TEXT_SERVICE_DISPLAY:
			{
				CStringW sz = getModel()->GetLanguageString(IDS_SERVICE);
				sz+=_T(" - Display");

				CMVEventInfotext event(CMVEventInfotext::EV_TEXT,  sz, 0);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TEXT_SERVICE_SETTINGS:
			{
				CStringW sz = getModel()->GetLanguageString(IDS_SERVICE);
				sz+=_T(" - Settings");

				CMVEventInfotext event(CMVEventInfotext::EV_TEXT,  sz, 0);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TEXT_SERVICE_CALIBRATION:
			{
				CStringW sz = getModel()->GetLanguageString(IDS_SERVICE);
				sz+=_T(" - Calibration");

				CMVEventInfotext event(CMVEventInfotext::EV_TEXT,  sz, 0);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TEXT_SERVICE_FLOWBOARD:
			{
				CStringW sz = getModel()->GetLanguageString(IDS_SERVICE);
				sz+=_T(" - Blenderboard");

				CMVEventInfotext event(CMVEventInfotext::EV_TEXT,  sz, 0);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TEXT_SERVICE_ACCUBOARD:
			{
				CStringW sz = getModel()->GetLanguageString(IDS_SERVICE);
				sz+=_T(" - Accuboard");

				CMVEventInfotext event(CMVEventInfotext::EV_TEXT,  sz, 0);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TEXT_SERVICE_VALVES:
			{
				CStringW sz = getModel()->GetLanguageString(IDS_SERVICE);
				sz+=_T(" - Valves");

				CMVEventInfotext event(CMVEventInfotext::EV_TEXT,  sz, 0);
				getModel()->triggerEvent(&event);
				return 1;

			}
			break;
		case WM_EV_TEXT_SERVICE_MONITOR:
			{
				CStringW sz = getModel()->GetLanguageString(IDS_SERVICE);
				sz+=_T(" - MonitorPIC");

				CMVEventInfotext event(CMVEventInfotext::EV_TEXT,  sz, 0);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TEXT_SERVICE_INTERFACES:
			{
				CStringW sz = getModel()->GetLanguageString(IDS_SERVICE);
				sz+=_T(" - Interfaces");

				CMVEventInfotext event(CMVEventInfotext::EV_TEXT,  sz, 0);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TEXT_SERVICE_NETWORK:
			{
				CStringW sz = getModel()->GetLanguageString(IDS_SERVICE);
				sz+=_T(" - Network");

				CMVEventInfotext event(CMVEventInfotext::EV_TEXT,  sz, 0);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TEXT_SERVICE_LICENSING:
			{
				CStringW sz = getModel()->GetLanguageString(IDS_SERVICE);
				sz+=_T(" - Licensing");

				CMVEventInfotext event(CMVEventInfotext::EV_TEXT,  sz, 0);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_TEXT_SERVICE_MAINTENANCE:
			{
				CStringW sz = _T("");
				sz=getModel()->GetLanguageString(IDS_SERVICE);
				sz+=_T(" - Maintenance");

				CMVEventInfotext event(CMVEventInfotext::EV_TEXT,  sz, 0);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_ALIMIT5:
			{
				CMVEventUI event(CMVEventUI::EV_BN_ALIMIT5);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_RESET_ALARMS:
			{
				CMVEventUI event(CMVEventUI::EV_BN_ALIMIT4);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_GRAPH_LOOP:
			{
				CMVEventUI event(CMVEventUI::EV_BN_GRAPH_LOOP);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_GRAPH_CO2:
			{
				CMVEventUI event(CMVEventUI::EV_BN_GRAPH_CO2);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_GRAPH_SPO2:
			{
				CMVEventUI event(CMVEventUI::EV_BN_GRAPH_SPO2);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_GRAPH_FOT:
			{
				CMVEventUI event(CMVEventUI::EV_BN_GRAPH_FOT);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		/*case WM_EV_BN_GRAPH_EMG:
			{
				CMVEventUI event(CMVEventUI::EV_BN_GRAPH_EMG);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_GRAPH_CLFIO2:
			{
				CMVEventUI event(CMVEventUI::EV_BN_GRAPH_CLFIO2);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;*/
		case WM_EV_BN_GRAPH_TREND:
			{
				CMVEventUI event(CMVEventUI::EV_BN_GRAPH_TREND);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_GRAPH_SAVE:
			{
				CMVEventUI event(CMVEventUI::EV_BN_GRAPH_SAVE);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_MENUBUTTON_CLOSE:
			{
				CMVEventUI event(CMVEventUI::EV_BN_MENUBUTTON_CLOSE);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_MENUBUTTON_OPEN:
			{
				CMVEventUI event(CMVEventUI::EV_BN_MENUBUTTON_OPEN);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_MENU_MOVELEFT:
			{
				CMVEventUI event(CMVEventUI::EV_BN_MENU_MOVELEFT);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_MENU_MOVERIGHT:
			{
				CMVEventUI event(CMVEventUI::EV_BN_MENU_MOVERIGHT);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_TREND1:
			{
				CMVEventUI event(CMVEventUI::EV_BN_TREND1);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_TREND2:
			{
				CMVEventUI event(CMVEventUI::EV_BN_TREND2);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_TREND3:
			{
				CMVEventUI event(CMVEventUI::EV_BN_TREND3);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_TREND4:
			{
				CMVEventUI event(CMVEventUI::EV_BN_TREND4);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_TREND5:
			{
				CMVEventUI event(CMVEventUI::EV_BN_TREND5);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_CURSOR_UP:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_UP);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_CURSOR_DOWN:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_DOWN);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_CURSOR_LEFT:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_LEFT);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_CURSOR_RIGHT:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_RIGHT);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_CURSOR_MOVEXUP:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_MOVEXUP);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_CURSOR_MOVEXDOWN:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_MOVEXDOWN);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_CURSOR_MOVEYLEFT:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_MOVEYLEFT);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_CURSOR_MOVEYRIGHT:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_MOVEYRIGHT);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_SERVICE_BACK:
			{
				CMVEventUI event(CMVEventUI::EV_BN_SERVICE_BACK);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_GRAPH_REDRAW:
			{
				getModel()->getVIEWHANDLER()->redrawGraph();
			}
			break;
		case WM_START_FOT:
			{
				if(getModel()->getFOTThread())
					getModel()->getFOTThread()->startFOT();
				
				getModel()->getVIEWHANDLER()->redrawINFO();
			}
			break;
		case WM_STOP_FOT:
			{
				if(getModel()->getFOTThread())
					getModel()->getFOTThread()->stopFOT();

				//getModel()->getVIEWHANDLER()->redrawINFO();
			}
			break;
		case WM_STOPANDRESET_FOT:
			{
				if(getModel()->getFOTThread())
				{
					if(getModel()->getFOTThread()->getFOTstate()!=FOT_OFF)
					{
						getModel()->getFOTThread()->stopFOT();
					}

					getModel()->getFOTThread()->resetFOTdisplayBuffer();
				}
			}
			break;
		case WM_TXT_STOP_FOT:
			{
				//CStringW sData = _T("- FOT turned off -");
				CStringW sData = getModel()->GetLanguageString(IDS_TXT_FOT_TURNEDOFF);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 3500);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				return 1;
			}
			break;
		case WM_REDRAW_FOT_STATE:
			{
				getModel()->getVIEWHANDLER()->redrawFOTmenu();
				return 1;
			}
			break;
		case WM_NEXT_FOT_STEP:
			{
				if(getModel()->getFOTThread())
					getModel()->getFOTThread()->continueSequence();
				return 1;
			}
			break;
		case WM_DECREASE_FOT_STEP:
			{
				if(getModel()->getFOTThread())
					getModel()->getFOTThread()->decreaseSequence();

				//CStringW sData = _T("- increase FOT stopped -");
				//CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				//getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				return 1;
			}
			break;
		case WM_START_FOT_STEP:
			{
				if(getModel()->getFOTThread())
					getModel()->getFOTThread()->runStep();

				//CStringW sData = _T("- FOT step started -");
				CStringW sData = getModel()->GetLanguageString(IDS_TXT_FOT_STEPSTARTED);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				return 1;
			}
			break;
		case WM_REPEAT_FOT_STEP:
			{
				if(getModel()->getFOTThread())
					getModel()->getFOTThread()->repeatSequence();

				//CStringW sData = _T("- repeat FOT step -");
				CStringW sData = getModel()->GetLanguageString(IDS_TXT_FOT_STEPREPEAT);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				return 1;
			}
			break;
		case WM_EV_CONTROL_START_FOT_CONVENTIONAL:
			{
				//DEBUGMSG(TRUE, (TEXT("WM_EV_CONTROL_START_FOT_CONVENTIONAL\r\n")));

				CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_START_FOT_CONVENTIONAL);
				getModel()->triggerEvent(&eventCtrl);

				return 1;
			}
			break;
		case WM_EV_CONTROL_STOP_FOT_CONVENTIONAL:
			{
				//DEBUGMSG(TRUE, (TEXT("WM_EV_CONTROL_STOP_FOT_CONVENTIONAL\r\n")));

				CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_RESTORE_FOT_CONVENTIONAL);
				getModel()->triggerEvent(&eventCtrl);

				return 1;
			}
			break;
		case WM_FOT_UPDATE_PEEP:
			{
				getModel()->getVIEWHANDLER()->updateFOTPEEPPara();
			}
			break;
		case WM_FOT_UPDATE_PMEAN:
			{
				getModel()->getVIEWHANDLER()->updateFOTPmeanPara();
			}
			break;
		case WM_EV_CONTROL_SET_MODE_EXHALVALVCAL:
			{
				CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_SET_MODE_EXHALVALVCAL);
				getModel()->triggerEvent(&eventCtrl);
				return 1;
			}
			break;
		case WM_EV_BN_SERVICE_SYSTEM:
			{
				CMVEventUI event(CMVEventUI::EV_BN_SERVICE_SYSTEM);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_SERVICE_CALIBR:
			{
				CMVEventUI event(CMVEventUI::EV_BN_SERVICE_CALIBR);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_SERVICE_FLOW:
			{
				CMVEventUI event(CMVEventUI::EV_BN_SERVICE_FLOW);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_SERVICE_MONITOR:
			{
				CMVEventUI event(CMVEventUI::EV_BN_SERVICE_MONITOR);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_SERVICE_ACCU:
			{
				CMVEventUI event(CMVEventUI::EV_BN_SERVICE_ACCU);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_SERVICE_VALVES:
			{
				CMVEventUI event(CMVEventUI::EV_BN_SERVICE_VALVES);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_SERVICE_DISPLAY:
			{
				CMVEventUI event(CMVEventUI::EV_BN_SERVICE_DISPLAY);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_SERVICE_SETTINGS:
			{
				CMVEventUI event(CMVEventUI::EV_BN_SERVICE_SETTINGS);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_SERVICE_IF:
			{
				CMVEventUI event(CMVEventUI::EV_BN_SERVICE_IF);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_SERVICE_NETWORK:
			{
				CMVEventUI event(CMVEventUI::EV_BN_SERVICE_NETWORK);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_SERVICE_LICENSING:
			{
				CMVEventUI event(CMVEventUI::EV_BN_SERVICE_LICENSING);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_SERVICE_MAINTENANCE:
			{
				CMVEventUI event(CMVEventUI::EV_BN_SERVICE_MAINTENANCE);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_EV_BN_SYSALARM_SILENT:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_SYSALARM_SILENT);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_DATA_ALARMLIMITS_CHANGED:
			{
				CMVEventData event(CMVEventData::EV_DATA_ALARMLIMITS_CHANGED);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_DATA_CALCULATE_ALARMLIMITS:
			{
				CMVEventData event(CMVEventData::EV_DATA_CALCULATE_ALARMLIMITS);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_REDRAW_VIEW:
			{
				getModel()->getVIEWHANDLER()->RedrawCurViews();
				return 1;
			}
			break;
		case WM_BN_MATRIX_NEBULIZER:
			{
				CStringW sData = _T("- OPTIONAL -");
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);

				//getModel()->getVIEWHANDLER()->runNebulizer();
				
				return 1;
			}
			break;
		case WM_BN_MATRIX_MENU:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_MENU);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_BN_MATRIX_HOME:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_HOME);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_BN_MATRIX_ALARMLIMITS:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_ALARMLIMITS);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		/*case WM_EV_TIMETEXT_ACTIVEALARM:
			{
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				Sleep(300);
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);

				CStringW sData = getModel()->GetLanguageString(IDS_TXT_ACTIVEALARM);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->Trigger(&event);
				return 1;
			}
			break;*/
		case WM_CONTROL_MANBREATH_LUNGREC:
			{
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				Sleep(300);
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);

				CStringW sData = getModel()->GetLanguageString(IDS_TXT_MANBREATH_LUNGREC);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_CONTROL_MANBREATH_TXTDISABLED:
			{
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				Sleep(300);
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);

				CStringW sData = getModel()->GetLanguageString(IDS_TXT_HFOMANBREATH);
				sData+=_T(": ")+getModel()->GetLanguageString(IDS_TXT_DISABLED);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_CONTROL_START_MANBREATH:
			{
				if(getModel()->getDATAHANDLER()->isPSVapnoe()==true)
				{
					PostMessage(WM_PSV_APNOE_END);
				}
				if(getModel()->getFOTThread())
				{
					if(getModel()->getFOTThread()->getFOTstate()!=FOT_OFF)
					{
						if(AfxGetApp() != NULL)
							AfxGetApp()->GetMainWnd()->PostMessage(WM_STOP_FOT);
					}
				}
				CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_START_MANBREATH);
				getModel()->triggerEvent(&eventCtrl);
				return 1;
			}
			break;
		case WM_BN_MATRIX_STARTSTOP_UP:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_STARTSTOP_UP);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_BN_MATRIX_STARTSTOP_DOWN:
			{
				//getModel()->getVIEWHANDLER()->closeNebulizer();
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_STARTSTOP_DOWN);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_BN_MATRIX_ONOFF_UP:
			{
				//test entrek
	#ifndef SIMULATION_ENTREK
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_ONOFF_UP);
				getModel()->triggerEvent(&event);
				return 1;
	#endif
			}
			break;
		case WM_BN_MATRIX_ONOFF_DOWN:
			{
				getModel()->getDATAHANDLER()->saveOpTime();//newOPtime

				//test entrek
	#ifndef SIMULATION_ENTREK
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_ONOFF_DOWN);
				getModel()->triggerEvent(&event);
	#endif
				
	#ifdef SIMULATION_ENTREK
				getModel()->StartInstaller();
				getModel()->SetAccuTurnoff();
				PostMessage(WM_VENT_TURNOFF);
	#endif		
				return 1;
			}
			break;
		case WM_BN_MATRIX_GRAPHS:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_GRAPHS);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_BN_MATRIX_ALARM_SILENT:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_ALARM_SILENT);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_BN_SCREENLOCK:
			{
				CMVEventUI event(CMVEventUI::EV_BN_SCREENLOCK);
				getModel()->triggerEvent(&event);

				this->SetFocus();
				return 1;
			}
			break;
		case WM_FLOWCAL_FINNISHED:
			{
				if(getModel()->getALARMHANDLER()->isVgarantAutoTurnedOff_FlowSensor() 
					&& getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
				{
					if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
					{
						if(getModel()->getDATAHANDLER()->PARADATA()->IsVLimitParamOn_IPPV())
						{
							getModel()->getDATAHANDLER()->SetVLimitParamdata_IPPV(getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_IPPV(),false,true);
						}
					}
					else
					{
						if(getModel()->getDATAHANDLER()->PARADATA()->IsVLimitParamOn_TRIGGER())
						{
							getModel()->getDATAHANDLER()->SetVLimitParamdata_TRIGGER(getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_TRIGGER(),false,true);
						}
					}

					theApp.getLog()->WriteLine(_T("VgarantAutoTurnedOn WM_FLOWCAL_FINNISHED"));
					getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PARA_AUTOENABLED);
				}
				else if(getModel()->getALARMHANDLER()->isVlimitAutoTurnedOff_FlowSensor()
					&& getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
				{
					if(getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
					{
						getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_OFF);
					}

					theApp.getLog()->WriteLine(_T("VlimitAutoTurnedOn WM_FLOWCAL_FINNISHED"));

					if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
						getModel()->getDATAHANDLER()->SetVLimitParamdata_IPPV(getModel()->getDATAHANDLER()->GetLastVLimitParam(),true,true);
					else
						getModel()->getDATAHANDLER()->SetVLimitParamdata_TRIGGER(getModel()->getDATAHANDLER()->GetLastVLimitParam(),true,true);

					PostMessage(WM_VLIMIT_PARA_AUTOENABLED);
					DEBUGMSG(TRUE, (TEXT("WM_VLIMIT_PARA_AUTOENABLED 2 ###\r\n")));
				}
				
				getModel()->getALARMHANDLER()->resetVgVlAutoTurnedOff_FlowSensor();

				return 1;
			}
			break;
		case WM_SERIAL_SETALARM_CHECKSUMCONPIC:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_ChecksumConPIC);
				return 1;
			}
			break;
		case WM_SERIAL_DELALARM_CHECKSUMCONPIC:
			{
				getModel()->getDATAHANDLER()->SetConPICChecksumError(false);
				theApp.getLog()->WriteLine(_T("Delete AL_SysFail_ChecksumConPIC"));
				getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_ChecksumConPIC,AS_SIGNALED);
				return 1;
			}
			break;
		case WM_SERIAL_SETALARM_DISCONNECTION:
			{
				//CClientDC dc(this);
				//	try
				//	{
				//		m_pcTest->Draw(dc.m_hDC);
				//		/*test[6]=0;

				//		int iRes=test[6];*/
				//		/*int iN=0;
				//		int iRes= 500/iN;*/
				//	}
				//	__except (EXCEPTION_EXECUTE_HANDLER)
				//	{
				//		CFabianHFOApp::ReportException(_T("EXCEPTION: test !!!!!!!"), true);
				//		//AfxMessageBox( _T("test !!!!!!!") );
				//	}
				/*if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
				{
					if(getModel()->getPRICOThread()->isPRICOrunning())
					{
						getModel()->getPRICOThread()->setAlarmDisabled();
						getModel()->getDATAHANDLER()->setPRICOState(false);
					}

				}*/

				getModel()->getALARMHANDLER()->setAlarm(AL_DISCONNECTION);

				return 1;
			}
			break;
		case WM_SERIAL_DELALARM_DISCONNECTION:
			{
				/*eVentSilentState eSilentState=getModel()->getALARMHANDLER()->getAlarmSilentState();
				if(		(eSilentState==ASTATE_AUTOSILENT
					||	eSilentState==ASTATE_ACTIVE)
					&&(getModel()->getDATAHANDLER()->activeModeIsNMODE()==false))
					getModel()->getALARMHANDLER()->setAutoSilent();*/
				getModel()->getALARMHANDLER()->setStateOfAlarm(AL_DISCONNECTION,AS_SIGNALED);
				return 1;
			}
			break;
		//case WM_SERIAL_SETALARM_P_INSPIRATION:
		//	{
		//		getModel()->getALARMHANDLER()->setAlarm(AL_SysAl_P_INSPIRATIONTUBE);
		//		//getModel()->getALARMHANDLER()->SetAlarm_P_INSPIRATIONTUBE(_T(""));
		//		return 1;
		//	}
		//	break;
		//case WM_SERIAL_DELALARM_P_INSPIRATION:
		//	{
		//		eVentSilentState eSilentState=getModel()->getALARMHANDLER()->getAlarmSilentState();
		//		if(		eSilentState==ASTATE_AUTOSILENT
		//			||	eSilentState==ASTATE_MANSILENT
		//			||	eSilentState==ASTATE_ACTIVE)
		//			getModel()->getALARMHANDLER()->setAutoSilent();
		//		getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysAl_P_INSPIRATIONTUBE,AS_SIGNALED);
		//		return 1;
		//	}
		//	break;
		//case WM_SERIAL_SETALARM_P_EXSPIRATION:
		//	{
		//		getModel()->getALARMHANDLER()->setAlarm(AL_SysAl_P_EXSPIRATIONTUBE);
		//		//getModel()->getALARMHANDLER()->SetAlarm_P_EXSPIRATIONTUBE(_T(""));
		//		return 1;
		//	}
		//	break;
		//case WM_SERIAL_DELALARM_P_EXSPIRATION:
		//	{
		//		eVentSilentState eSilentState=getModel()->getALARMHANDLER()->getAlarmSilentState();
		//		if(		eSilentState==ASTATE_AUTOSILENT
		//			||	eSilentState==ASTATE_MANSILENT
		//			||	eSilentState==ASTATE_ACTIVE)
		//			getModel()->getALARMHANDLER()->setAutoSilent();
		//		getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysAl_P_EXSPIRATIONTUBE,AS_SIGNALED);
		//		return 1;
		//	}
		//	break;
		case WM_SERIAL_DELALARM_TUBE_OCCLUSION:
			{
				//eVentSilentState state=getModel()->getALARMHANDLER()->getAlarmSilentState();
				//if(		state==ASTATE_AUTOSILENT
				//	||	state==ASTATE_MANSILENT
				//	//||	state==ASTATE_MANSILENT_SYSALARM
				//	||	state==ASTATE_ACTIVE)
				//	getModel()->getALARMHANDLER()->setAutoSilent(false,false,false);

				getModel()->setActiveAlarmDelay(true);
				PostMessage(WM_SET_ACTIVEALARM_DELAY);
				getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysAl_TUBE_OCCLUSION,AS_SIGNALED);

				return 1;
			}
			break;
		case WM_SETALARM_AccuEmpty:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_Accu_Empty);
				//getModel()->getALARMHANDLER()->SetAlarm_AccuEmpty(_T(""));
				return 1;
			}
			break;
		case WM_SETALARM_AccuDefect:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_Accu_Defect);
				//getModel()->getALARMHANDLER()->SetAlarm_AccuDefect(_T(""));
				return 1;
			}
			break;
		case WM_SETALARM_ChecksumMonPIC:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_ChecksumMonPIC);
				//getModel()->getALARMHANDLER()->SetAlarm_ChecksumMonPIC(_T(""));
				return 1;
			}
			break;
		case WM_SETALARM_RELAIS_DEFECT:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_RELAIS_DEFECT);
				//getModel()->getALARMHANDLER()->SetAlarm_RELAIS_DEFECT(_T(""));
				return 1;
			}
			break;
		case WM_SERIAL_SETALARM_MIXER_AVCAL:
			{
				getModel()->getSERIAL()->GetBLENDER_StatusByte();
				getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_MIXER_AVCAL);
				return 1;
			}
			break;
		case WM_SERIAL_DELALARM_MIXER_AVCAL:
			{
				eVentSilentState state=getModel()->getALARMHANDLER()->getAlarmSilentState();
				if(		state==ASTATE_AUTOSILENT
					||	state==ASTATE_MANSILENT
					//||	state==ASTATE_MANSILENT_SYSALARM
					||	state==ASTATE_ACTIVE)
					getModel()->getALARMHANDLER()->setAutoSilent(false,false,false);

				getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_MIXER_AVCAL,AS_SIGNALED);
				return 1;
			}
			break;
		case WM_SETALARM_TUBE_OCCLUSION:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_SysAl_TUBE_OCCLUSION);
				//getModel()->getALARMHANDLER()->SetAlarm_P_PROXIMAL(_T(""));

				

				return 1;
			}
			break;
		//case WM_SETALARM_P_IN_MIXER:
		//	{
		//		getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_P_IN_MIXER);
		//		//getModel()->getALARMHANDLER()->SetAlarm_P_IN_MIXER(_T(""));
		//		return 1;
		//	}
		//	break;
		case WM_SETALARM_VOLTAGE:
			{
				getModel()->getDATAHANDLER()->ReadI2CWatchdogState();
				getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_VOLTAGE);
				return 1;
			}
			break;
		case WM_SETALARM_IF_SPI:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_IF_SPI);
				return 1;
			}
			break;
		case WM_SETALARM_IF_DIO:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_IF_DIO);
				//getModel()->getALARMHANDLER()->SetAlarm_IF_DIO(_T(""));
				return 1;
			}
			break;
		case WM_SETALARM_IF_COM://newVG
			{
				getModel()->setSERIALavailable(FALSE);
				if(getModel()->getALARMHANDLER()->CanSetAlarm_IF_COM())
					getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_IF_COM);
				return 1;
			}
			break;
		case WM_SETALARM_IF_COM_REINIT:
			{
				bool bCom_ERROR=true;
				bool bReinit=getModel()->reinitSERIAL();

				Sleep(100);

				if(bReinit)
				{
					DEBUGMSG(TRUE, (TEXT("### ReinitSERIAL success ###\r\n")));

					if(getModel()->getSERIAL()->GetM_CHECKSUM(true)>0)
					{
						bCom_ERROR=false;
						theApp.getLog()->WriteLine(_T("#HFO:0217"));
					}
					else
					{
						bCom_ERROR=true;
						theApp.getLog()->WriteLine(_T("#HFO:0218"));
					}
				}
				else
				{
					bCom_ERROR=true;
					theApp.getLog()->WriteLine(_T("#HFO:0219"));
				}

				if(bCom_ERROR)
				{
					getModel()->setSERIALavailable(FALSE);

					KillTimer(CHECKCOM_TIMER);
					SetTimer(CHECKCOM_TIMER, 500, NULL);
					DEBUGMSG(TRUE, (TEXT("#CHECKCOM_TIMER\r\n")));
					//getModel()->SetReinitSERIALFlag(FALSE);

					if(getModel()->getALARMHANDLER()->CanSetAlarm_IF_COM())
						getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_IF_COM);
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("#COM ok\r\n")));

					getModel()->getDATAHANDLER()->deleteCOMErrorCode(ERRC_COM_INIT); 
					//getModel()->getDATAHANDLER()->deleteCOMErrorCode(ERRC_COM_CHECK_DATA); //newSerialAlarm
					getModel()->getDATAHANDLER()->deleteCOMErrorCode(ERRC_COM_READ_MSTATUS); 
					//getModel()->getDATAHANDLER()->deleteCOMError(); //newVG

					getModel()->setSERIALavailable(TRUE);
					if(getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE) //newVG
					{
						getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
					}
					if(getModel()->getCONFIG()->CurModeIsPresetMode())
						getModel()->Send_VENT_MODE(getModel()->getCONFIG()->GetPrevMode());
					else
						getModel()->Send_VENT_MODE(getModel()->getCONFIG()->GetCurMode());
				}
				return 1;
			}
			break;
		/*case WM_SETALARM_IF_COM_NOREINIT:
			{
				getModel()->SetReinitSERIALFlag(FALSE);

				if(getModel()->getALARMHANDLER()->CanSetAlarm_IF_COM())
					getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_IF_COM);
				
				return 1;
			}
			break;*/
		//case WM_SETALARM_IF_CO2:
		//	{
		//		getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_IF_CO2);
		//		//getModel()->getALARMHANDLER()->SetAlarm_IF_CO2(_T(""));
		//		return 1;
		//	}
		//	break;
		case WM_SETALARM_IF_I2C:
			{
				if(getModel()->getALARMHANDLER()->CanSetAlarm_IF_I2C())
				{
					getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_IF_I2C);
				}
				return 1;
			}
			break;
		case WM_SETALARM_IF_PIF:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_IF_PIF);
				m_bWD0error=true;
				return 1;
			}
			break;
		//case WM_SETALARM_IF_ACULINK:
		//	{
		//		getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_IF_ACULINK);
		//		//getModel()->getALARMHANDLER()->SetAlarm_IF_ACULINK(_T(""));
		//		return 1;
		//	}
		//	break;
		case WM_SETALARM_Fan:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_Fan);
				//getModel()->getALARMHANDLER()->SetAlarm_Fan(_T(""));
				return 1;
			}
			break;
		case WM_SETALARM_OUTOFMEMORY:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_OUTOFMEMORY);
				//getModel()->getALARMHANDLER()->SetAlarm_OUTOFMEMORY(_T(""));
				return 1;
			}
			break;
		case WM_SETALARM_P_IN_O2:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_SysAl_P_IN_O2);
				//getModel()->getALARMHANDLER()->SetAlarm_P_IN_O2(_T(""));
				return 1;
			}
			break;
		case WM_SETALARM_P_IN_AIR:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_SysAl_P_IN_AIR);
				//getModel()->getALARMHANDLER()->SetAlarm_P_IN_AIR(_T(""));
				return 1;
			}
			break;
		//case WM_SETALARM_P_EXSPIRATIONTUBE:
		//	{
		//		getModel()->getALARMHANDLER()->setAlarm(AL_SysAl_P_EXSPIRATIONTUBE);
		//		//getModel()->getALARMHANDLER()->SetAlarm_P_EXSPIRATIONTUBE(_T(""));
		//		return 1;
		//	}
		//	break;
		//case WM_SETALARM_P_INSPIRATIONTUBE:
		//	{
		//		getModel()->getALARMHANDLER()->setAlarm(AL_SysAl_P_INSPIRATIONTUBE);
		//		//getModel()->getALARMHANDLER()->SetAlarm_P_INSPIRATIONTUBE(_T(""));
		//		return 1;
		//	}
		//	break;
		case WM_SETALARM_DISCONNECTION:
			{
				//CClientDC dc(this);
				//	try
				//	{
				//		m_pcTest->Draw(dc.m_hDC);
				//		/*test[6]=0;

				//		int iRes=test[6];*/
				//		/*int iN=0;
				//		int iRes= 500/iN;*/
				//	}
				//	__except (EXCEPTION_EXECUTE_HANDLER)
				//	{
				//		CFabianHFOApp::ReportException(_T("EXCEPTION: test !!!!!!!"), true);
				//		AfxMessageBox( _T("test !!!!!!!") );
				//	}
				/*if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
				{
					if(getModel()->getPRICOThread()->isPRICOrunning())
					{
						getModel()->getPRICOThread()->setAlarmDisabled();
						getModel()->getDATAHANDLER()->setPRICOState(false);
					}

				}*/
				getModel()->getALARMHANDLER()->setAlarm(AL_DISCONNECTION);

				return 1;
			}
			break;
		case WM_SETALARM_TUBUSBLOCKED:
			{
				/*if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
				{
					if(getModel()->getPRICOThread()->isPRICOrunning())
					{
						getModel()->getPRICOThread()->setAlarmDisabled();
						getModel()->getDATAHANDLER()->setPRICOState(false);
					}

				}*/
				getModel()->getALARMHANDLER()->setAlarm(AL_TUBUSBLOCKED);

				if(getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
				{
					getModel()->getALARMHANDLER()->setVgarantAutoTurnedOff_Tube();
					getModel()->getALARMHANDLER()->resetVlimitAutoTurnedOff_Tube();

					getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PARA_AUTODISABLED);
				}
				else if(getModel()->getDATAHANDLER()->IsActiveModeVLimitStateOn())
				{
					getModel()->getDATAHANDLER()->SaveLastVLimitParam();
					getModel()->getDATAHANDLER()->PARADATA()->SetVLimitParam_IPPV(getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_IPPV(),false,true,true);
					getModel()->getDATAHANDLER()->PARADATA()->SetVLimitParam_TRIGGER(getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_TRIGGER(),false,true,true);
					getModel()->getALARMHANDLER()->setVlimitAutoTurnedOff_Tube();
					getModel()->getALARMHANDLER()->resetVgarantAutoTurnedOff_Tube();

					if(AfxGetMainWnd())
						AfxGetMainWnd()->PostMessage(WM_VLIMIT_PARA_AUTODISABLED);
					DEBUGMSG(TRUE, (TEXT("WM_VLIMIT_PARA_AUTODISABLED 1 ###\r\n")));
				}
				else
				{
					/*getModel()->getALARMHANDLER()->setVgarantAutoTurnedOff(false);
					getModel()->getALARMHANDLER()->setVlimitAutoTurnedOff(false);*/
				}

				

				return 1;
			}
			break;
		case WM_FLOW_SENSOR_DEFECT:
			{
				if(getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
				{
					getModel()->getALARMHANDLER()->setVgarantAutoTurnedOff_FlowSensor();
					getModel()->getALARMHANDLER()->resetVlimitAutoTurnedOff_FlowSensor();

					getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PARA_AUTODISABLED);
				}
				else if(getModel()->getDATAHANDLER()->IsActiveModeVLimitStateOn())
				{
					getModel()->getDATAHANDLER()->SaveLastVLimitParam();
					getModel()->getDATAHANDLER()->PARADATA()->SetVLimitParam_IPPV(getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_IPPV(),false,true,true);
					getModel()->getDATAHANDLER()->PARADATA()->SetVLimitParam_TRIGGER(getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_TRIGGER(),false,true,true);
					getModel()->getALARMHANDLER()->setVlimitAutoTurnedOff_FlowSensor();
					getModel()->getALARMHANDLER()->resetVgarantAutoTurnedOff_FlowSensor();
					getModel()->getALARMHANDLER()->resetVgAutoTurnedOff_FOT();

					if(AfxGetMainWnd())
						AfxGetMainWnd()->PostMessage(WM_VLIMIT_PARA_AUTODISABLED);
					DEBUGMSG(TRUE, (TEXT("WM_VLIMIT_PARA_AUTODISABLED 2 ###\r\n")));
				}

				PostMessage(WM_SETALARM_FLOW_SENSOR_DEFECT);


				return 1;
			}
			break;
		case WM_SETALARM_FLOW_SENSOR_DEFECT:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_Sens_FLOW_SENSOR_DEFECT);
				return 1;
			}
			break;
		case WM_FLOW_SENSOR_CLEANING:
			{
				if(getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
				{
					getModel()->getALARMHANDLER()->setVgarantAutoTurnedOff_FlowSensor();
					getModel()->getALARMHANDLER()->resetVlimitAutoTurnedOff_FlowSensor();

					getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PARA_AUTODISABLED);
				}
				else if(getModel()->getDATAHANDLER()->IsActiveModeVLimitStateOn())
				{
					getModel()->getDATAHANDLER()->SaveLastVLimitParam();
					getModel()->getDATAHANDLER()->PARADATA()->SetVLimitParam_IPPV(getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_IPPV(),false,true,true);
					getModel()->getDATAHANDLER()->PARADATA()->SetVLimitParam_TRIGGER(getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_TRIGGER(),false,true,true);
					getModel()->getALARMHANDLER()->setVlimitAutoTurnedOff_FlowSensor();
					getModel()->getALARMHANDLER()->resetVgarantAutoTurnedOff_FlowSensor();
					getModel()->getALARMHANDLER()->resetVgAutoTurnedOff_FOT();

					if(AfxGetMainWnd())
						AfxGetMainWnd()->PostMessage(WM_VLIMIT_PARA_AUTODISABLED);
					DEBUGMSG(TRUE, (TEXT("WM_VLIMIT_PARA_AUTODISABLED 3 ###\r\n")));
				}

				PostMessage(WM_SETALARM_FLOW_SENSOR_CLEANING);

				return 1;
			}
			break;
		case WM_SETALARM_FLOW_SENSOR_CLEANING:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_Sens_FLOW_SENSOR_CLEANING);
				return 1;
			}
			break;
		case WM_FLOW_SENSOR_NOTCONNECTED:
			{
				if(getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
				{
					getModel()->getALARMHANDLER()->setVgarantAutoTurnedOff_FlowSensor();
					getModel()->getALARMHANDLER()->resetVlimitAutoTurnedOff_FlowSensor();

					getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PARA_AUTODISABLED);
				}
				else if(getModel()->getDATAHANDLER()->IsActiveModeVLimitStateOn())
				{
					getModel()->getDATAHANDLER()->SaveLastVLimitParam();
					getModel()->getDATAHANDLER()->PARADATA()->SetVLimitParam_IPPV(getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_IPPV(),false,true,true);
					getModel()->getDATAHANDLER()->PARADATA()->SetVLimitParam_TRIGGER(getModel()->getDATAHANDLER()->PARADATA()->GetVLimitParam_TRIGGER(),false,true,true);
					getModel()->getALARMHANDLER()->setVlimitAutoTurnedOff_FlowSensor();
					getModel()->getALARMHANDLER()->resetVgarantAutoTurnedOff_FlowSensor();
					getModel()->getALARMHANDLER()->resetVgAutoTurnedOff_FOT();

					if(AfxGetMainWnd())
						AfxGetMainWnd()->PostMessage(WM_VLIMIT_PARA_AUTODISABLED);
					DEBUGMSG(TRUE, (TEXT("WM_VLIMIT_PARA_AUTODISABLED 4 ###\r\n")));
				}
				//else
				//{
				//	getModel()->getALARMHANDLER()->setVgarantAutoTurnedOff(false);
				//	getModel()->getALARMHANDLER()->setVlimitAutoTurnedOff(false);
				//}


				PostMessage(WM_SETALARM_FLOW_SENSOR_NOTCONNECTED);

				return 1;
			}
			break;
		case WM_SETALARM_FLOW_SENSOR_NOTCONNECTED:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_Sens_FLOW_SENSOR_NOTCONNECTED);
				return 1;
			}
			break;
		case WM_SETALARM_O2_SENSOR_DEFECT:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_Sens_O2_SENSOR_DEFECT);
				return 1;
			}
			break;
		case WM_SETALARM_O2_SENSOR_USED:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_Sens_O2_SENSOR_USED);
				return 1;
			}
			break;
		case WM_SETALARM_O2_VALUE_INCORRECT:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_Sens_O2_VALUE_INCORRECT);
				return 1;
			}
			break;
		case WM_SETALARM_O2_NOT_CALIBRATED:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_Sens_O2_NOT_CALIBRATED);
				return 1;
			}
			break;
		case WM_SETALARM_NEBULIZER_DISCON:
			{
				/*getModel()->getALARMHANDLER()->setAlarm(AL_Nebulizer_Disconnection);
				PostMessage(WM_BN_MATRIX_NEBULIZER);*/
				return 1;
			}
			break;
		case WM_SETALARM_NEBULIZER_ERROR:
			{
				/*getModel()->getALARMHANDLER()->setAlarm(AL_Nebulizer_SysError);
				PostMessage(WM_BN_MATRIX_NEBULIZER);*/
				return 1;
			}
			break;
		case WM_REINIT_SPO2_MODULE:
			{
				if(getModel()->isSPO2running() && false==getModel()->getCONFIG()->isSpO2ConfigInProgress())
				{
					getModel()->getCONFIG()->setSPO2module(getModel()->getCONFIG()->getSPO2module(),true);
				}
				
				return 1;
			}
			break;
		case WM_NEW_FIO2DATA:
			{
				SHORT iFiO2value=getModel()->getDATAHANDLER()->getMessureDataO2();
				if(false==getModel()->getDATAHANDLER()->GetOxyCalRunning())
					m_iOldOxyValue=iFiO2value;

				if(getModel()->getAcuLink()!=NULL)
				{
					if(iFiO2value<0)
					{
						getModel()->getAcuLink()->setMeasurementData(ALINK_MSMNT_OXY,0);
					}
					else
					{
						if(getModel()->getDATAHANDLER()->GetOxyCalRunning())
							getModel()->getAcuLink()->setMeasurementData(ALINK_MSMNT_OXY,m_iOldOxyValue);
						else
							getModel()->getAcuLink()->setMeasurementData(ALINK_MSMNT_OXY,iFiO2value);
					}
				}

				getModel()->getVIEWHANDLER()->drawMeasuredFiO2Value();
				return 1;
			}
			break;
		/*case WM_PRICO_FIO2_OUTOFRANGE:
			{
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, _T("! PRICO: calculated FiO2 not possible !"), 3500);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				Sleep(300);
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
			}
			break;*/
		case WM_SPO2_SIQDATA:
			{
				if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
					getModel()->getVIEWHANDLER()->setSIQdata();
			}
			break;
		case WM_TXT_PRICO_RUNNING:
			{
				CMVEventInfotext event2(CMVEventInfotext::EV_TIMETEXT,  getModel()->GetLanguageString(IDS_TXT_PRICORUNS), 3000);
				getModel()->triggerEvent(&event2);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
			}
			break;
		case WM_START_PRICO:
			{
				if(NULL==getModel()->getPRICOThread())
					return 1;
				theApp.getLog()->WriteLine(_T("*** PRICO started"));

				//SHORT iMessureDataO2=getModel()->getDATAHANDLER()->PARADATA()->GetO2Para()*10;
				BYTE iMessureDataO2=getModel()->getDATAHANDLER()->PARADATA()->GetO2Para();  //pro, change 3
				BYTE byHighSpO2PRICOlimit=getModel()->getDATAHANDLER()->getPRICO_SPO2highRange();
				BYTE byLowSpO2PRICOlimit=getModel()->getDATAHANDLER()->getPRICO_SPO2lowRange();

				CString sz=_T("");
				sz.Format(_T("#PRICO SpO2 low:%d, "), byLowSpO2PRICOlimit);
				theApp.getLog()->WriteLine(sz);
				sz.Format(_T("#PRICO SpO2 high:%d, "), byHighSpO2PRICOlimit);
				theApp.getLog()->WriteLine(sz);

				BYTE byPRICO_FIO2lowRange=getModel()->getDATAHANDLER()->getPRICO_FIO2lowRange();
				sz.Format(_T("#PRICO FiO2 low:%d, "), byPRICO_FIO2lowRange);
				theApp.getLog()->WriteLine(sz);

				BYTE byPRICO_FIO2highRange=getModel()->getDATAHANDLER()->getPRICO_FIO2highRange();
				sz.Format(_T("#PRICO FiO2 high:%d, "), byPRICO_FIO2highRange);
				theApp.getLog()->WriteLine(sz);

				sz.Format(_T("#PRICO FiO2 measured:%d, "), iMessureDataO2);
				theApp.getLog()->WriteLine(sz);

				//set the starting FiO2 to the range setting
				//if(getModel()->getDATAHANDLER()->PARADATA()->GetO2Para()< byPRICO_FIO2lowRange)
				if(iMessureDataO2 < byPRICO_FIO2lowRange) //pro, change 3
				{
					getModel()->Send_PARA_OXY_RATIO(byPRICO_FIO2lowRange,true,true);
					//getModel()->getPRICOThread()->setStartupOxyValue(byPRICO_FIO2lowRange*10);
					getModel()->getPRICOThread()->setStartupOxyValue(byPRICO_FIO2lowRange); //pro, change 3
				}
				//else if(getModel()->getDATAHANDLER()->PARADATA()->GetO2Para() > byPRICO_FIO2highRange)
				else if(iMessureDataO2 > byPRICO_FIO2highRange) //pro, change 3
				{
					getModel()->Send_PARA_OXY_RATIO(byPRICO_FIO2highRange,true,true);
					//getModel()->getPRICOThread()->setStartupOxyValue(byPRICO_FIO2highRange*10);
					getModel()->getPRICOThread()->setStartupOxyValue(byPRICO_FIO2highRange); //pro, change 3
				}
				else
				{
					//getModel()->getPRICOThread()->setStartupOxyValue(iMessureDataO2*10);
					getModel()->getPRICOThread()->setStartupOxyValue(iMessureDataO2); //pro, change 3
				}

				//getModel()->getPRICOThread()->setMeasuredOxyValue(iMessureDataO2);
				getModel()->getPRICOThread()->setHighSpO2PRICOlimit(byHighSpO2PRICOlimit);
				getModel()->getPRICOThread()->setLowSpO2PRICOlimit(byLowSpO2PRICOlimit);
				
				getModel()->getPRICOThread()->startPRICOThread();
				getModel()->getVIEWHANDLER()->setPRICOrunning(true);

				if(getModel()->getAcuLink() != NULL)
				{
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2LOW, getModel()->getDATAHANDLER()->getPRICO_SPO2lowRange());		
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2HIGH, getModel()->getDATAHANDLER()->getPRICO_SPO2highRange());
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2LOW, getModel()->getDATAHANDLER()->getPRICO_FIO2lowRange());
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2HIGH, getModel()->getDATAHANDLER()->getPRICO_FIO2highRange());
				}

				return 1;
			}
			break;
		case WM_STOP_PRICO:
			{
				if(NULL==getModel()->getPRICOThread())
					return 1;

				getModel()->getVIEWHANDLER()->setPRICOrunning(false);

				getModel()->getPRICOThread()->stopPRICOThread();
				theApp.getLog()->WriteLine(_T("*** PRICO stopped"));

				if(getModel()->isO2FlushActive()==false)//PRICO04
				{
					getModel()->getDATAHANDLER()->SetCurrentO2Para(getModel()->getDATAHANDLER()->PARADATA()->GetO2Para()); 
				}
				/*else
				{
					getModel()->Send_PARA_OXY_RATIO(getModel()->getDATAHANDLER()->GetCurrentO2FlushPara(),true,true);
				}*/

				if(getModel()->getALARMHANDLER()->ALARM_PRICO_FiO2max->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PRICO_FiO2max);
				}

				if(getModel()->getALARMHANDLER()->ALARM_PRICO_FiO2min->getAlarmState()==AS_ACTIVE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PRICO_FiO2min);
				}
				
				if(getModel()->getAcuLink() != NULL)
				{
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2LOW, ALINK_NOTVALID);		
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2HIGH, ALINK_NOTVALID);
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2LOW, ALINK_NOTVALID);
					getModel()->getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2HIGH, ALINK_NOTVALID);
				}
				
				return 1;
			}
			break;
		case WM_SETALARM_SPO2_MODULE_NOTCONNECTED:
			{
				if(getModel()->isSPO2running())
				{
					/*if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
					{
						if(getModel()->getPRICOThread()->isPRICOrunning())
						{
							getModel()->getPRICOThread()->setAlarmDisabled();
							getModel()->getDATAHANDLER()->setPRICOState(false);
						}
					}*/

					getModel()->getALARMHANDLER()->setAlarm(AL_Sens_SPO2_MODULE_NOTCONNECTED);
				}
				
				return 1;
			}
			break;
		case WM_SETALARM_SPO2_SENSORFAULTY:
			{
				/*if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
				{
					if(getModel()->getPRICOThread()->isPRICOrunning())
					{
						getModel()->getPRICOThread()->setAlarmDisabled();
						getModel()->getDATAHANDLER()->setPRICOState(false);
					}
				}*/

				getModel()->getALARMHANDLER()->setAlarm(AL_Sens_SPO2_SENSORFAULTY);
				return 1;
			}
			break;
		case WM_SETALARM_SPO2_CHECKSENSOR:
			{
				/*if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
				{
					if(getModel()->getPRICOThread()->isPRICOrunning())
					{
						getModel()->getPRICOThread()->setAlarmDisabled();
						getModel()->getDATAHANDLER()->setPRICOState(false);
					}
				}*/

				getModel()->getALARMHANDLER()->setAlarm(AL_Sens_SPO2_CHECKSENSOR);
				return 1;
			}
			break;
		/*case WM_CO2_STATE_CHANGED:
			{
				CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_CO2STATE_CHANGED);
				getModel()->Trigger(&eventCtrl);
			}
			break;*/
		case WM_SETALARM_CO2_MODULE_NOTCONNECTED:
			{
				KillTimer(CO2PUMPTIMER);
				m_bShowCO2PumpOnMsg=true;
				
				if(getModel()->getDATAHANDLER()->getRemainCO2PumpTime()!=0)
				{
					getModel()->getDATAHANDLER()->setRemainCO2PumpTime(0);
					getModel()->getVIEWHANDLER()->UpdateCO2InfoData(false);
				}
				//m_ullRemainCO2PumpTime=0;
				getModel()->getALARMHANDLER()->setAlarm(AL_Sens_CO2_MODULE_NOTCONNECTED);
				return 1;
			}
			break;
		case WM_SETALARM_CO2_FILTERLINE_NOTCONNECTED:
			{
				KillTimer(CO2PUMPTIMER);
				m_bShowCO2PumpOnMsg=true;
				
				if(getModel()->getDATAHANDLER()->getRemainCO2PumpTime()!=0)
				{
					getModel()->getDATAHANDLER()->setRemainCO2PumpTime(0);
					getModel()->getVIEWHANDLER()->UpdateCO2InfoData(false);
				}
				//m_ullRemainCO2PumpTime=0;
				getModel()->getALARMHANDLER()->setAlarm(AL_Sens_CO2_FILTERLINE_NOTCONNECTED);
				return 1;
			}
			break;
		case WM_SETALARM_CO2_CHECKSAMPLINGLINE:
			{
				KillTimer(CO2PUMPTIMER);
				m_bShowCO2PumpOnMsg=true;
				//m_ullRemainCO2PumpTime=0;
				
				getModel()->getDATAHANDLER()->setRemainCO2PumpTime(0);
				getModel()->getVIEWHANDLER()->UpdateCO2InfoData(false);


				getModel()->getALARMHANDLER()->setAlarm(AL_Sens_CO2_CHECKSAMPLINGLINE);
				return 1;
			}
			break;
		case WM_SETALARM_CO2_CHECKAIRWAYADAPTER:
			{
				KillTimer(CO2PUMPTIMER);
				m_bShowCO2PumpOnMsg=true;
				//m_ullRemainCO2PumpTime=0;
				
				getModel()->getDATAHANDLER()->setRemainCO2PumpTime(0);
				getModel()->getVIEWHANDLER()->UpdateCO2InfoData(false);

				getModel()->getALARMHANDLER()->setAlarm(AL_Sens_CO2_CHECKAIRWAYADAPTER);
				return 1;
			}
			break;
		case WM_SETALARM_CO2_SENSORFAULTY:
			{
				KillTimer(CO2PUMPTIMER);
				m_bShowCO2PumpOnMsg=true;
				//m_ullRemainCO2PumpTime=0;
				
				getModel()->getDATAHANDLER()->setRemainCO2PumpTime(0);
				getModel()->getVIEWHANDLER()->UpdateCO2InfoData(false);

				getModel()->getALARMHANDLER()->setAlarm(AL_Sens_CO2_SENSORFAULTY);
				return 1;
			}
			break;
		case WM_SETALARM_MVmax:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_PatAl_MVmax);
				return 1;
			}
			break;
		case WM_SETALARM_MVmin:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_PatAl_MVmin);
				return 1;
			}
			break;
		//case WM_SETALARM_PRICO_SIQ:
		//	{
		//		//if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
		//		{
		//			//if(getModel()->getPRICOThread()->isPRICOrunning())
		//			{
		//				getModel()->getPRICOThread()->setAlarmDisabled();
		//				getModel()->getDATAHANDLER()->setPRICOState(false);

		//				//getModel()->getALARMHANDLER()->setAlarm(AL_PRICO_SIQvalue);
		//			}
		//		}

		//		
		//		return 1;
		//	}
		//	break;
		/*case WM_DELALARM_PRICO_SIQ:
			{
				getModel()->getALARMHANDLER()->setStateOfAlarm(AL_PRICO_SIQvalue,AS_NONE);
				return 1;
			}
			break;*/
		case WM_SETALARM_PRICO_FiO2outOfRange:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_Sens_PRICO_FiO2outOfRange);
				return 1;
			}
			break;
		case WM_SETALARM_PRICO_FiO2max:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_PRICO_FiO2max);

				int iFiO2=(int)CTlsFloat::Round(((double)getModel()->getDATAHANDLER()->getAppliedFiO2para())/10, 0);
				CString sz=_T("");
				sz.Format(_T("*** PRICO: FiO2 at max target: %d"), iFiO2);
				theApp.getLog()->WriteLine(sz);
				return 1;
			}
			break;
		case WM_SETALARM_PRICO_FiO2min:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_PRICO_FiO2min);

				int iFiO2=(int)CTlsFloat::Round(((double)getModel()->getDATAHANDLER()->getAppliedFiO2para())/10, 0);
				CString sz=_T("");
				sz.Format(_T("*** PRICO: Lower minimum FiO2 %d"), iFiO2);
				theApp.getLog()->WriteLine(sz);
				return 1;
			}
			break;
		case WM_EV_TIMETEXT_PRICO_LOWFIO2:
			{
				//MVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, _T("Lower minimum FiO2"), 10000);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, getModel()->GetLanguageString(IDS_TXT_FIO2_LOWMIN), 10000);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				Sleep(300);
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				return 1;
			}
			break;
		case WM_SETALARM_PIPmax:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_PatAl_PIPmax);
				return 1;
			}
			break;
		case WM_DELALARM_PIPmax:
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_PIPmax);
				return 1;
			}
			break;
		case WM_SetAlarm_PminLow:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_PatAl_PEEPminLow);
				return 1;
			}
			break;
		case WM_SETSIGNALEDALARM_Pmin:
			{
				getModel()->getALARMHANDLER()->setStateOfAlarm(AL_PatAl_PEEPminLow,AS_SIGNALED);
				return 1;
			}
			break;
		case WM_SetAlarm_PminHigh:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_PatAl_PEEPminHigh);
				return 1;
			}
			break;
		case WM_SETALARM_BPMmax:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_PatAl_BPMmax);
				return 1;
			}
			break;
		case WM_SETALARM_Leakmax:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_PatAl_Leakmax);
				return 1;
			}
			break;
		case WM_SETALARM_Apnoe:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_PatAl_Apnoe);

				if(getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE && getModel()->getETCO2()!=NULL)
				{
					getModel()->getETCO2()->set_restartBreathAlgorithm();
				}
				return 1;
			}
			break;
		case WM_SETALARM_DCO2max:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_PatAl_DCO2max);
				return 1;
			}
			break;
		case WM_SETALARM_DCO2min:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_PatAl_DCO2min);
				return 1;
			}
			break;
		case WM_SETALARM_Vlimitted:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_SysLimit_Vlimitted);
				return 1;
			}
			break;
		case WM_SETALARM_Vgaranty:
			{
				getModel()->getDATAHANDLER()->setStatus2(getModel()->getSPI()->Read_STATUS_2());

				getModel()->getALARMHANDLER()->setAlarm(AL_SysLimit_Vgaranty);
				return 1;
			}
			break;
		case WM_SETALARM_Accu_POWER:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_Accu_POWER);
				return 1;
			}
			break;
		case WM_SETALARM_Accu_60:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_Accu_60);
				return 1;
			}
			break;
		case WM_SETALARM_Accu_30:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_Accu_30);
				return 1;
			}
			break;
		case WM_SETALARM_Accu_15:
			{
				getModel()->getALARMHANDLER()->setAlarm(AL_Accu_15);
				//getModel()->getALARMHANDLER()->SetAlarm_Accu_15(_T(""));
				return 1;
			}
			break;
		//case WM_CHECK_SPI:
		//	{
		//		if(getModel()->getSPI()->Check_ALIVE_BYTE()==FALSE)
		//		{
		//			getModel()->SetSPIavailability(FALSE);
		//			/*getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_ALIVE);
		//			getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_IF_SPI);*/
		//			//getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T(""));

		//			getModel()->SetCheckSPIavailability(TRUE);
		//		}
		//		//else
		//		//{
		//		//	getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_IF_SPI);
		//		//	//getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T(""));
		//		//}

		//		return 1;
		//	}
		//	break;
		case WM_SETALARM_PINSPNOTREACHED:
			{
				CStringW sData = getModel()->GetLanguageString(IDS_ALARM_PINSP_NOTREACH);
				sData += _T(" > ");
				sData += getModel()->GetLanguageString(IDS_TXT_CHECKLEAK);
				CMVEventInfotext event2(CMVEventInfotext::EV_TIMETEXT,  sData, 5000);
				getModel()->triggerEvent(&event2);

				getModel()->getALARMHANDLER()->setLogTxt(sData);
				//getModel()->getALARMHANDLER()->setAlarm(AL_SysLimit_Pinsp_NotReached);
				return 1;
			}
			break;
		case WM_FLOWSENSOR_CALIBRATED:
			{
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);

				getModel()->setVentilationRangeChanged(false);

				CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_FLOWSENSOR_CALIBRATED);
				getModel()->triggerEvent(&eventCtrl);
				return 1;
			}
			break;
		case WM_SYSTEMSTATE_CHANGED:
			{
				CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_SYSTEMSTATE_CHANGED);
				getModel()->triggerEvent(&eventCtrl);
				return 1;
			}
			break;
		case WM_SET_EFLOWEQUIFLOW:
			{
				CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_SET_EFLOWEQUIFLOW);
				getModel()->triggerEvent(&eventCtrl);
				return 1;
			}
			break;
		case WM_SET_CO2PUMPTIMER:
			{
				KillTimer(CO2PUMPTIMER);
				if(getModel()->getCONFIG()->GetTimePumpAutoOn()>0)
				{
					getModel()->getDATAHANDLER()->setRemainCO2PumpTime(getModel()->getCONFIG()->GetTimePumpAutoOn()*60000);
					getModel()->getVIEWHANDLER()->UpdateCO2InfoData(true);

					m_bShowCO2PumpOnMsg=true;

					SetTimer(CO2PUMPTIMER, 1000, NULL);
				}
				return 1;
			}
			break;
		case WM_KILL_CO2PUMPTIMER:
			{
				KillTimer(CO2PUMPTIMER);
				m_bShowCO2PumpOnMsg=true;
				
				getModel()->getDATAHANDLER()->setRemainCO2PumpTime(0);
				getModel()->getVIEWHANDLER()->UpdateCO2InfoData(false);

				return 1;
			}
			break;
		case WM_DISABLE_PATALARM_O2:
			{
				getModel()->DisablePatAlarmO2dependend();
				SetTimer(DISABLE_PATALARM_O2_TIMER, 1000, NULL);
				return 1;
			}
			break;
		case WM_CONTROL_O2CALSTART:
			{
				CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_O2CALSTART);
				getModel()->triggerEvent(&eventCtrl);
				return 1;
			}
			break;
		case WM_CONTROL_O2CALSTOP:
			{
				CMVEventControl eventCtrl2(CMVEventControl::EV_CONTROL_O2CALSTOP);
				getModel()->triggerEvent(&eventCtrl2);
				return 1;
			}
			break;
		case WM_PARABN_HFFREQREC:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_HFFREQREC);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		/*case WM_LUNGRECSTATE_CHANGED:
			{
				getModel()->getVIEWHANDLER()->lungRecStateChanged();
				return 1;
			}
			break;*/
		case WM_PARABN_HFAMPL:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_HFAMPL);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_PARABN_PINSP:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_PINSP);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_PARABN_PMAXVOLG:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_PMAXVOLG);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_PARABN_PEEP:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_PEEP);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_PARABN_PPSV:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_PPSV);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_PARABN_CPAP:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_CPAP);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_PARABN_PMAN:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_PMAN);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_PARABN_PDUO:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_PDUO);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_PARABN_PMITT:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_PMITT);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_PARABN_IERATIO:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_IERATIO);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_PARABN_HFFREQ:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_HFFREQ);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_PARABN_VGARANT:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_VGARANT);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_PARABN_VLIMIT:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_VLIMIT);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_PARABN_FOT:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_FOT);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_PARADATA_CHANGED:
			{
				getModel()->getVIEWHANDLER()->paraDataChanged();
				return 1;
			}
			break;
		case WM_ITIME_CHANGED:
			{
				getModel()->getVIEWHANDLER()->ITIMEChanged();
				return 1;
			}
			break;
		case WM_ETIME_CHANGED:
			{
				getModel()->getVIEWHANDLER()->ETIMEChanged();
				return 1;
			}
			break;
		case WM_BPM_CHANGED:
			{
				getModel()->getVIEWHANDLER()->BPMChanged();
				return 1;
			}
			break;
		case WM_OXICAL_CANCEL:
			{
				m_bCancelOxiCal=true;
				return 1;
			}
			break;
		case WM_OXICAL_START21:
			{
				m_bO2Sensor21manual=true;
				eventOxyCal.SetEvent();
				return 1;
			}
			break;
		case WM_OXICAL_START100:
			{
				m_bO2Sensor100manual=true;
				eventOxyCal.SetEvent();
				return 1;
			}
			break;
		case WM_DELETE_TRENDS:
			{
				
				//TREND
				if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
					getModel()->getDATAHANDLER()->DeleteAllTrendData();
				return 1;
			}
			break;
		case WM_SET_AUTOSCREENLOG_TIMER:
			{
				if(		getModel()->getCONFIG()->IsAutoScreenlockActive()
					&&	isSafeTickCountDelayExpired(m_dwLastAutoScreenTimer, 1000))
				{
					eAutoScreenlock time=getModel()->getCONFIG()->GetAutoScreenlockTime();
					m_dwLastAutoScreenTimer=GetTickCount();

					SetTimer(AUTOSCREENLOCKTIMER, (UINT)time*1000, NULL);
				}
				return 1;
			}
			break;
		case WM_KILL_AUTOSCREENLOG_TIMER:
			{
				KillTimer(AUTOSCREENLOCKTIMER);
				return 1;
			}
			break;
		case WM_PIF_ALARM1:
			{
				getModel()->getSOUND()->SetPIFSound(PIF_ALARM1);
				return 1;
			}
			break;
		case WM_PIF_ALARM2:
			{
				getModel()->getSOUND()->SetPIFSound(PIF_ALARM2);
				return 1;
			}
			break;
		case WM_PIF_ALARM3:
			{
				getModel()->getSOUND()->SetPIFSound(PIF_ALARM3);
				return 1;
			}
			break;
		case WM_PIF_SIGNAL:
			{
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				return 1;
			}
			break;
		case WM_PIF_NONE:
			{
				getModel()->getSOUND()->SetPIFSound(PIF_NONE);
				return 1;
			}
			break;
		case WM_PIF_DOUBLESIGNAL:
			{
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				Sleep(250);
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				return 1;
			}
			break;
		case WM_SCREENFREE:
			{
				CMVEventUI event(CMVEventUI::EV_BN_SCREENFREE);
				getModel()->triggerEvent(&event);

				if(getModel()->getCONFIG()->IsAutoScreenlockActive())
				{
					PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
				}
				else
				{
					PostMessage(WM_KILL_AUTOSCREENLOG_TIMER);
				}

				int iID=0;

				CWnd* pFocWnd=CWnd::GetFocus();
				if(pFocWnd==0)
				{
				}
				else
				{
					iID=pFocWnd->GetDlgCtrlID();

					if(iID==0)
					{
						getModel()->getVIEWHANDLER()->SetFocusToPrimaryView();
					}
				}
				return 1;
			}
			break;
		case WM_O2FLUSH_CHANGED:
			{
				getModel()->getVIEWHANDLER()->O2FlushChanged();

				if(0==getModel()->getDATAHANDLER()->GetCurO2FlushTime() && true==getModel()->isO2FlushActive())
				{
					getModel()->deactivateO2Flush();
					m_iO2FlushCount=getModel()->getDATAHANDLER()->GetCurO2FlushTime();
					getModel()->getVIEWHANDLER()->DrawO2FlushTime(m_iO2FlushCount);

					getModel()->Send_PARA_OXY_RATIO(getModel()->getDATAHANDLER()->GetCurrentO2Para(),true,true);
				}
				return 1;
			}
			break;
		case WM_PMEANREC_CHANGED:
			{
				getModel()->getVIEWHANDLER()->HFPMEANRECFlushChanged(wParam);
				return 1;
			}
			break;
		/*case WM_PARABUTTON_TURNEDOWN:
			{
				getModel()->getVIEWHANDLER()->ParaButtonTurnDown(wParam, lParam);
				return 1;
			}
			break;*/
		case WM_STARTUP:
			{
				getModel()->getVIEWHANDLER()->Init();
				return 1;
			}
			break;
		case WM_RELOAD_CONFIG_ERROR:
			{
				CStringW sData = _T("ERROR: ");
				sData += getModel()->GetLanguageString(IDS_TXT_CONFIGLOADED);
				CMVEventInfotext event2(CMVEventInfotext::EV_TIMETEXT,  sData, 3000);
				getModel()->triggerEvent(&event2);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);

				Sleep(2000);
				AfxGetApp()->GetMainWnd()->PostMessage(WM_LANGUAGE_CHANGED);

				return 1;
			}
			break;
		case WM_RELOAD_CONFIG:
			{
				CMVEventUI event(CMVEventUI::EV_RELOAD_CONFIG);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_VLIMIT_ENABLED:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_VLIMIT_ENABLED);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_VLIMIT_DISABLED:
			{
				CMVEventUI event(CMVEventUI::EV_PARABN_VLIMIT_DISABLED);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_CALCULATE_AUTOALARMLIMITS:
			{
				if(getModel()->getALARMHANDLER()->getAlimitState_MVmaxLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitMVmax();

				if(getModel()->getALARMHANDLER()->getAlimitState_MVminLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitMVmin();

				if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();

				if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();

				if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();

				if(getModel()->getALARMHANDLER()->getAlimitState_BPMmaxLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitBPMmax();

				if(getModel()->getALARMHANDLER()->getAlimitState_LeakmaxLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitLeakmax();

				if(getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitApnoe();

				if(getModel()->getALARMHANDLER()->getAlimitState_MAPmaxLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitMAPmax();

				if(getModel()->getALARMHANDLER()->getAlimitState_MAPminLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitMAPmin();

				if(getModel()->getALARMHANDLER()->getAlimitState_DCO2maxLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitDCO2max();

				if(getModel()->getALARMHANDLER()->getAlimitState_DCO2minLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitDCO2min();

				if(getModel()->getALARMHANDLER()->getAlimitState_ETCO2maxLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitETCO2max();

				if(getModel()->getALARMHANDLER()->getAlimitState_ETCO2minLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitETCO2min();

				if(getModel()->getALARMHANDLER()->getAlimitState_FICO2maxLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitFICO2max();

				if(getModel()->getALARMHANDLER()->getAlimitState_FICO2minLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitFICO2min();

				if(getModel()->getALARMHANDLER()->getAlimitState_SPO2maxLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitSPO2max();

				if(getModel()->getALARMHANDLER()->getAlimitState_SPO2minLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitSPO2min();

				if(getModel()->getALARMHANDLER()->getAlimitState_PulseRatemaxLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPulseRatemax();

				if(getModel()->getALARMHANDLER()->getAlimitState_PulseRateminLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPulseRatemin();

				if(getModel()->getALARMHANDLER()->getAlimitState_SPO2_PIminLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitSPO2_PImin();

				if(getModel()->getALARMHANDLER()->getAlimitState_SPO2_SIQminLimit()==AL_AUTO)
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitSPO2_SIQmin();

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);
				return 1;
			}
			break;
		case WM_SCALE_LIMIT:
			{
				CStringW sData = getModel()->GetLanguageString(IDS_TXT_SCALELIMIT);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				return 1;
			}
			break;
		//case WM_SCREENSHOT:
		//	{
		//		//SendScreenShot();
		//		SaveScreenShot();
		//		return 1;
		//	}
		//	break;
		case WM_DELAY_AUTOOXYCAL:
			{
				m_bDelayAutoOxyCal=false;
				return 1;
			}
			break;
		case WM_START_AUTOOXYCAL:
			{
	#ifndef SIMULATION_ENTREK //rkuNEWFIX
				StartOxyCalThread();
	#endif
				return 1;
			}
			break;
		case WM_START_TRENDRECORD:
			{
				m_bStartTrend=true;
				return 1;
			}
			break;
		case WM_MENUMODE:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_MENU);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_SERVICEMODE:
			{
				CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_SERVICE);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_SET_SERVICEMODE:
			{
				CMVEventUI event(CMVEventUI::EV_BN_SET_SERVICE_MODE);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_DEL_SERVICEMODE:
			{
				CMVEventUI event(CMVEventUI::EV_BN_DEL_SERVICE_MODE);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_SET_GRAPHCURSOR:
			{
				CMVEventUI event(CMVEventUI::EV_GRAPH_SETCURSOR,(int)wParam);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_DEL_GRAPHCURSOR:
			{
				CMVEventUI event(CMVEventUI::EV_GRAPH_DELCURSOR);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_STOPDEL_TRENDDATA:
			{
				if(m_bDoDelTrendThread)
				{
					StopDelTrendThread();
				}
				getModel()->getDATAHANDLER()->SetDelTrendThread(false);//rkuNEWFIX
				return 1;
			}
			break;
		case WM_DEL_TRENDDATA:
			{
				DEBUGMSG(TRUE, (TEXT("WM_DEL_TRENDDATA\r\n")));
				CString sPathOld=_T("");
				CString szPath=_T("\\sdcard\\");
				if(CTlsFile::Exists(szPath)==false)
				{
					szPath=_T("\\FFSDISK\\");
					sPathOld = szPath+IDS_OLDTRD_FOLDER;
				}
				else
				{
					sPathOld = szPath+IDS_OLDTRD_FOLDER;
				}

				if(CTlsFile::Exists(sPathOld)==true)
				{
					getModel()->getDATAHANDLER()->SetDelTrendThread(true);
					StartDelTrendThread();
					eventDelTrend.SetEvent();
				}

				return 1;
			}
			break;
		case WM_STARTUP_ACULINK:
			{
				startAcuLink();
				return 1;
			}
			break;
		case WM_STARTUP_SUCCESS:
			{
				//SetTimer(WATCHDOGTIMER,5000,NULL);

				CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_STARTUP_SUCCESS);
				getModel()->triggerEvent(&eventCtrl);

				getModel()->getDATAHANDLER()->CheckAkku(true);
				return 1;
			}
			break;
		case WM_ALARMSTATE_CHANGED:
			{
				//getModel()->getVIEWHANDLER()->checkAlarmState();

				CMVEventAlarm event(CMVEventAlarm::EV_ALARMSTATE_CHANGED);
				getModel()->triggerEvent(&event);
				return 1;
				
			}
			break;
		case WM_TURNOFF_APNEA:
			{
				getModel()->getALARMHANDLER()->setAlimitState_ApnoeLimit(AL_OFF);
				getModel()->getALARMHANDLER()->setAlimitApnoe(0);

				CStringW sData = getModel()->GetLanguageString(IDS_TXT_TURNOFF_APNEA);
				//CStringW sData = _T("!apnea alarm disabled!"); 
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 3000);
				getModel()->triggerEvent(&event);
				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);

				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_FLOWSENSORSTATE);
				return 1;

			}
			break;
		case WM_TURNOFF_TRIGGER_NCPAP:
			{
				getModel()->getDATAHANDLER()->SetTriggerNCPAPParadata(MAXRANGE_TRIGGER_OFF,true);

				PostMessage(WM_TRIGGER_STATE);
				return 1;
			}
			break;
		case WM_TURNOFF_TRIGGER_DUOPAP:
			{
				getModel()->getDATAHANDLER()->SetTriggerDUOPAPParadata(MAXRANGE_TRIGGER_OFF,true);

				PostMessage(WM_TRIGGER_STATE);
				return 1;
			}
			break;
		/*case WM_TURNOFF_TRIGGERNMODE_AUTOENABLE:
			{
				getModel()->getDATAHANDLER()->SetNMODEtriggerAutoenableFlag(true);
				getModel()->getDATAHANDLER()->SetTriggerNMODEParadata(0,true);
				return 1;
			}
			break;*/
		case WM_POWERSTATE_CHANGED:
			{
				CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_POWERSTATE_CHANGED);
				getModel()->triggerEvent(&eventCtrl);
				return 1;
			}
			break;
		case WM_EV_CLEAR_TEXT:
			{
				CMVEventInfotext event(CMVEventInfotext::EV_CLEAR_TEXT);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_CONTROL_DEL_MODE_EXHALVALVCAL:
			{
				CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_DEL_MODE_EXHALVALVCAL);
				getModel()->triggerEvent(&eventCtrl);
				return 1;
			}
			break;
		case WM_VOLUMEGARANTY_ON:
			{
				theApp.getLog()->WriteLine(_T("***VOLUMEGARANTY_ON"));

				WORD iR=getModel()->getDATAHANDLER()->getAVGMessureDataResistance();
				WORD iC=getModel()->getDATAHANDLER()->getAVGMessureDataCompliance();
				double iTau=(((double)iC/100)*((double)iR/10))/1000;

				CString szTAU=_T("");
				szTAU.Format(_T("Tau: %0.2f"), CTlsFloat::Round(((double)iTau), 2));
				theApp.getLog()->WriteLine(szTAU);

				bool bLimitChanged=false;

				if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
				{
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
					bLimitChanged=true;
				}
				else if(getModel()->getVMODEHANDLER()->getCurrentActiveMode()==VM_HFO)
				{
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
					bLimitChanged=true;
				}
				
				if(bLimitChanged)
				{
					PostMessage(WM_DATA_ALARMLIMITS_CHANGED);
				}
				getModel()->getALARMHANDLER()->setAutoSilent();

				CMVEventUI event(CMVEventUI::EV_VOLUMEGARANT_ON);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_VOLUMEGARANTY_OFF:
			{
				theApp.getLog()->WriteLine(_T("***VOLUMEGARANTY_OFF"));
				bool bLimitChanged=false;
				if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
				{
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
					bLimitChanged=true;
				}
				else if(getModel()->getVMODEHANDLER()->getCurrentActiveMode()==VM_HFO)
				{
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
					bLimitChanged=true;
				}
				
				if(bLimitChanged)
				{
					PostMessage(WM_DATA_ALARMLIMITS_CHANGED);
				}
				getModel()->getALARMHANDLER()->setAutoSilent();

				CMVEventUI event(CMVEventUI::EV_VOLUMEGARANT_OFF);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_VOLUMEGARANTY_PRESET_ON:
			{
				CMVEventUI event(CMVEventUI::EV_VOLUMEGARANT_PRESET_ON);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_VOLUMEGARANTY_PRESET_OFF:
			{
				CMVEventUI event(CMVEventUI::EV_VOLUMEGARANT_PRESET_OFF);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_VOLUMEGARANTY_PARA_ENABLED:
			{
				if(getModel()->getCONFIG()->CurModeIsPresetMode()==true)
				{
					CMVEventUI event(CMVEventUI::EV_PARABN_VOLUMEGARANT_PRESET_SELECTED_ON);
					getModel()->triggerEvent(&event);
				}
				else
				{
					CMVEventUI event(CMVEventUI::EV_PARABN_VOLUMEGARANT_SELECTED_ON);
					getModel()->triggerEvent(&event);
				}
				return 1;
			}
			break;
		case WM_VOLUMEGARANTY_PARA_AUTODISABLED:
			{
				theApp.getLog()->WriteLine(_T("VGARANTY_AUTODISABLED"));
				CMVEventUI event(CMVEventUI::EV_PARABN_VOLUMEGARANT_AUTO_OFF);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_VOLUMEGARANTY_PARA_AUTOENABLED:
			{
				theApp.getLog()->WriteLine(_T("VGARANTY_AUTOENABLED"));
				CMVEventUI event(CMVEventUI::EV_PARABN_VOLUMEGARANT_AUTO_ON);
				getModel()->triggerEvent(&event);

				//getModel()->getVIEWHANDLER()->redrawNebulizer();
				return 1;
			}
			break;
		case WM_VLIMIT_PARA_AUTODISABLED:
			{
				theApp.getLog()->WriteLine(_T("VLIMIT_AUTODISABLED"));
				CMVEventUI event(CMVEventUI::EV_PARABN_VLIMIT_AUTO_OFF);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_VLIMIT_PARA_AUTOENABLED:
			{
				theApp.getLog()->WriteLine(_T("VLIMIT_AUTOENABLED"));
				CMVEventUI event(CMVEventUI::EV_PARABN_VLIMIT_AUTO_ON);
				getModel()->triggerEvent(&event);

				//getModel()->getVIEWHANDLER()->redrawNebulizer();
				return 1;
			}
			break;
		case WM_VOLUMEGARANTY_PARA_DISABLED:
			{
				DEBUGMSG(TRUE, (TEXT("WM_VOLUMEGARANTY_PARA_DISABLED\r\n")));
				if(getModel()->getCONFIG()->CurModeIsPresetMode()==true)
				{
					CMVEventUI event(CMVEventUI::EV_PARABN_VOLUMEGARANT_PRESET_SELECTED_OFF);
					getModel()->triggerEvent(&event);
				}
				else
				{
					CMVEventUI event(CMVEventUI::EV_PARABN_VOLUMEGARANT_SELECTED_OFF);
					getModel()->triggerEvent(&event);
				}
				return 1;
			}
			break;
		case WM_EXSPIRATION:
			{
				if(!m_bExit)
					getModel()->NotifyExspirationStart();
				return 1;
			}
			break;
		case WM_NEWMONITORDATA:
			{
				/*if(getModel()->getDATAHANDLER()->getFOTseqRunning())
				{
					getModel()->getDATAHANDLER()->checkFOTvalidMeasurementData();
				}*/
				if(!m_bExit)
					getModel()->NotifyMonitorData();

				
				if(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL_REMOTE
					&&	getModel()->getTERMINAL()->isMeasurementDataActive())
				{
					//DEBUGMSG(TRUE, (TEXT("WM_NEWMONITORDATA\r\n")));
					getModel()->getTERMINAL()->sendMessurementData();
				}
				else if(getModel()->getAcuLink()!=NULL)
				{
					getModel()->getAcuLink()->sendMeasurementData();
				}
				 
				
				return 1;
			}
			break;
		case WM_PSV_APNOE:
			{
				getModel()->setPSVapnoe(true);
				return 1;
			}
			break;
		case WM_PSV_APNOE_END:
			{
				getModel()->setPSVapnoe(false);
				return 1;
			}
			break;
		case WM_TRIGGERED_BREATH:
			{
				getModel()->getVIEWHANDLER()->SetTriggeredBreath();

				if(getModel()->getDATAHANDLER()->isPSVapnoe()==true)
				{
					getModel()->setPSVapnoe(false);
				}

				return 1;
			}
			break;
		case WM_NEW_BREATH:
			{
				if(getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
				{
					getModel()->getDATAHANDLER()->sendMessureDataVolumeCorrection();
				}

				return 1;
			}
			break;
		case WM_CHECK_AUTOFLOW:
			{
				BYTE iVal=getModel()->getSPI()->Read_AUTOFLOW_CORR();
				
				if(iVal>255)
					iVal=0;

				//Wert an STEUER-PIC weiterleiten
				getModel()->getSERIAL()->Send_AUTOFLOW_CORR(iVal);

				return 1;
			}
			break;
		case WM_DEL_BACKUP:
			{
				if(m_bBackup==true)
				{
					//DEBUGMSG(TRUE, (TEXT("WM_DEL_BACKUP\r\n")));
					m_bBackup=false;
					getModel()->getVIEWHANDLER()->SetCPAPBackup(false);
				}

				return 1;
			}
			break;
		case WM_NEW_MECHATZ:
			{
				if(	getModel()->getCONFIG()->GetCurMode()==VM_CPAP
					||
					(getModel()->getCONFIG()->CurModeIsPresetMode() && getModel()->getCONFIG()->GetPrevMode()==VM_CPAP))
				{
					if(getModel()->isMANBREATHrunning()==false && getModel()->getDATAHANDLER()->PARADATA()->GetBackupPara()!=0)
					{
						if(m_bBackup==false)
						{
							//DEBUGMSG(TRUE, (TEXT("SetCPAPBackup\r\n")));
							m_bBackup=true;
							getModel()->getVIEWHANDLER()->SetCPAPBackup(true);
						}
						if(getModel()->getCONFIG()->GetPlayBackupSound())
							getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
					}
				}
				return 1;
			}
			break;
		case WM_GET_BLENDERVERSION:
			{
				getModel()->getDATAHANDLER()->SetBlenderPIC_Version(getModel()->getSERIAL()->GetBLENDER_VERSION());
			}
			break;
		case WM_CHECK_ALARMS:
			{
				//checkhere
				getModel()->getALARMHANDLER()->CheckPRICOalarms();//rku AUTOPRICO
				getModel()->getALARMHANDLER()->CheckAlarmStateBytes();
				getModel()->getALARMHANDLER()->CheckFlowSensorState();
				return 1;
			}
			break;
		case WM_CHECK_DATE:
			{
				if(theApp.getLog()->CheckDate()) 
				{
					getModel()->isMaintenanceNeeded();
				}
				getModel()->getALARMHANDLER()->checkLogAlarmDate();
				//getModel()->getDATAHANDLER()->checkDemoLicense();
			}
			break;
		/*case WM_LICENSING_CHANGED:
			{

			}
			break;*/
		case WM_CHECK_LIMITS:
			{
				getModel()->getDATAHANDLER()->checkLimits();
				return 1;
			}
			break;
		case WM_CHECK_SERIAL:
			{
				checkSERIAL();
			}
			break;
		case WM_AUTOLIMITTIMER:
			{
				eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

				if(m_eVentModeCalcStarted!=eActiveVentMode)
				{
					getModel()->getALARMHANDLER()->SetAlimitStateCalculated();
				}
				else
				{
					if(getModel()->getALARMHANDLER()->getAlimitState_MVmaxLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitMVmax();

					if(getModel()->getALARMHANDLER()->getAlimitState_MVminLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitMVmin();

					if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();

					if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();

					if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();

					if(getModel()->getALARMHANDLER()->getAlimitState_BPMmaxLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitBPMmax();

					if(getModel()->getALARMHANDLER()->getAlimitState_LeakmaxLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitLeakmax();

					if(getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitApnoe();

					if(getModel()->getALARMHANDLER()->getAlimitState_MAPmaxLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitMAPmax();

					if(getModel()->getALARMHANDLER()->getAlimitState_MAPminLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitMAPmin();

					if(getModel()->getALARMHANDLER()->getAlimitState_DCO2maxLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitDCO2max();

					if(getModel()->getALARMHANDLER()->getAlimitState_DCO2minLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitDCO2min();

					if(getModel()->getALARMHANDLER()->getAlimitState_ETCO2maxLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitETCO2max();

					if(getModel()->getALARMHANDLER()->getAlimitState_ETCO2minLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitETCO2min();

					if(getModel()->getALARMHANDLER()->getAlimitState_FICO2maxLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitFICO2max();

					if(getModel()->getALARMHANDLER()->getAlimitState_FICO2minLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitFICO2min();

					if(getModel()->getALARMHANDLER()->getAlimitState_SPO2maxLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitSPO2max();

					if(getModel()->getALARMHANDLER()->getAlimitState_SPO2minLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitSPO2min();

					if(getModel()->getALARMHANDLER()->getAlimitState_PulseRatemaxLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPulseRatemax();

					if(getModel()->getALARMHANDLER()->getAlimitState_PulseRateminLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPulseRatemin();

					if(getModel()->getALARMHANDLER()->getAlimitState_SPO2_PIminLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitSPO2_PImin();

					if(getModel()->getALARMHANDLER()->getAlimitState_SPO2_SIQminLimit()==AL_CALC)
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitSPO2_SIQmin();

					getModel()->getALARMHANDLER()->SetAlimitStateCalculated();
				}
				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);

				return 1;
			}
			break;
		case WM_HOMETIMER:
			{
				CMVEventControl event(CMVEventControl::EV_CONTROL_SETLASTVENTMODE);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_O2FLUSH:
			{
				if(0==getModel()->getDATAHANDLER()->GetCurO2FlushTime())
				{
					if(true==getModel()->isO2FlushActive())
					{
						getModel()->deactivateO2Flush();
						m_iO2FlushCount=getModel()->getDATAHANDLER()->GetCurO2FlushTime();
						getModel()->getVIEWHANDLER()->DrawO2FlushTime(m_iO2FlushCount);

						getModel()->Send_PARA_OXY_RATIO(getModel()->getDATAHANDLER()->GetCurrentO2Para(),true,true);
					}
					
					CStringW sData = getModel()->GetLanguageString(IDS_TXT_DISABLED); //IDS_PARA_O2+IDS_PARA_FLUSH
					CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 3000);
					getModel()->triggerEvent(&event);
					getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);

					return 1;
				}

				//if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)//PRICO04
				//{
				//	if(getModel()->getPRICOThread()->isPRICOalgorithmRunning())
				//	{
				//		CStringW sData = _T("not possible while PRICO is running");
				//		CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 3000);
				//		getModel()->triggerEvent(&event);

				//		getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				//		return 1;
				//	}
				//}


				KillTimer(O2FLUSHTIMER);

				getModel()->DisablePatAlarmO2dependend();
				PostMessage(WM_DISABLE_PATALARM_O2);

				if(!getModel()->isO2FlushActive())
				{
					//stop PRICO //PRICO04
					if(getModel()->getPRICOThread())
					{
						if(getModel()->getPRICOThread()->isPRICOalgorithmRunning())
						{
							CMVEventInfotext event2(CMVEventInfotext::EV_TIMETEXT,  getModel()->GetLanguageString(IDS_TXT_PRICOOFF), 3000);
							getModel()->triggerEvent(&event2);
							getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
							getModel()->getDATAHANDLER()->setPRICOoff();
						}

						if(getModel()->getALARMHANDLER()->isPRICOAutoTurnedOff())//rku AUTOPRICO
						{
							getModel()->getALARMHANDLER()->resetPRICOAutoTurnedOff();
						}
					}

					getModel()->activateO2Flush();
					m_iO2FlushCount=getModel()->getDATAHANDLER()->GetCurO2FlushTime();
					SetTimer(O2FLUSHTIMER, 1000, NULL);

					getModel()->Send_PARA_OXY_RATIO(getModel()->getDATAHANDLER()->GetCurrentO2FlushPara(),true,true);
				}
				else
				{
					getModel()->deactivateO2Flush();
					m_iO2FlushCount=getModel()->getDATAHANDLER()->GetCurO2FlushTime();
					getModel()->getVIEWHANDLER()->DrawO2FlushTime(m_iO2FlushCount);

					//PRICO04
					/*bool bPRICOrunning=false;
					if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
					{
						
						if(getModel()->getPRICOThread()->isPRICOalgorithmRunning())
						{
							bPRICOrunning=true;
						}
					}
					if(bPRICOrunning)
					{
						getModel()->Send_PARA_OXY_RATIO(getModel()->getPRICOThread()->getCalculatedOxyValue(),true,true);
					}
					else*/
					{
						getModel()->Send_PARA_OXY_RATIO(getModel()->getDATAHANDLER()->GetCurrentO2Para(),true,true);
					}
				}
				return 1;
				
			}
			break;
		case WM_O2FLUSHTIMER:
			{
				if(m_iO2FlushCount>0)
				{
					m_iO2FlushCount--;
					getModel()->getVIEWHANDLER()->DrawO2FlushTime(m_iO2FlushCount);
				}
				else
				{
					getModel()->deactivateO2Flush();
					KillTimer(O2FLUSHTIMER);
					m_iO2FlushCount=getModel()->getDATAHANDLER()->GetCurO2FlushTime();
					getModel()->getVIEWHANDLER()->DrawO2FlushTime(m_iO2FlushCount);

					getModel()->Send_PARA_OXY_RATIO(getModel()->getDATAHANDLER()->GetCurrentO2Para(),true,true);

					CStringW sData = getModel()->GetLanguageString(IDS_TXT_O2FLUSH_STOPPED);
					CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
					getModel()->triggerEvent(&event);

					getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				}
				return 1;
			}
			break;
		case WM_STOP_O2FLUSH:
			{
				getModel()->deactivateO2Flush();
				KillTimer(O2FLUSHTIMER);
				m_iO2FlushCount=getModel()->getDATAHANDLER()->GetCurO2FlushTime();
				getModel()->getVIEWHANDLER()->DrawO2FlushTime(m_iO2FlushCount);

				getModel()->Send_PARA_OXY_RATIO(getModel()->getDATAHANDLER()->GetCurrentO2Para(),true,true);

				/*CStringW sData = getModel()->GetLanguageString(IDS_TXT_O2FLUSH_STOPPED);
				CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
				getModel()->triggerEvent(&event);

				getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);*/
				return 1;
			}
			break;
		case WM_COUNTERSTOPBREATHTIMER:
			{
				if(m_iCountDownStart>1)
				{
					m_iCountDownStart--;
					getModel()->getVIEWHANDLER()->DrawCountDownStartTime(m_iCountDownStart);
				}
				else
				{
					KillTimer(COUNTSTOPBREATHTIMER);

					getModel()->SetVentRunState(VENT_RUNNING);

					getModel()->getVIEWHANDLER()->StopCountDownStartTime();
					getModel()->getALARMHANDLER()->setAutoSilent(false,true,false);

					m_iCountDownStart=0;
					
					CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_START_VENTILATION);
					getModel()->triggerEvent(&eventCtrl);
				}
				return 1;
			}
			break;
		case WM_SERVICE_SILENT:
			{
				KillTimer(ALARMSILENTTIMER);

				getModel()->StopCountDownAlarmSilent();
				m_iCountDownAlarmSilent=0;
				
				CMVEventAlarm event(CMVEventAlarm::EV_ALARMSTATE_CHANGED);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_ALARMSILENTTIMER:
			{
				if(m_iCountDownAlarmSilent>0)
				{
					getModel()->DrawCountDownAlarmSilent(m_iCountDownAlarmSilent);
					m_iCountDownAlarmSilent--;
					getModel()->SetCountDownAlarmSilent(m_iCountDownAlarmSilent);
				}
				else
				{
					KillTimer(ALARMSILENTTIMER);

					getModel()->StopCountDownAlarmSilent();
					m_iCountDownAlarmSilent=0;
					
					getModel()->getALARMHANDLER()->setAlarmsilentActive();

					CMVEventAlarm eventCtrl(CMVEventAlarm::EV_ALARM_REACTIVATED);
					getModel()->triggerEvent(&eventCtrl);
				}
				return 1;
			}
			break;
		/*case WM_ALARMSOUND:
			{
				SetTimer(ALARMSOUND,TIMEALARMSOUND,NULL);
				return 1;
			}
			break;*/
		case WM_MANUAL_ALARMSILENT_ON:
			{
				if(getModel()->GetVentRunState()==VENT_STOPPED)
				{
					KillTimer(ALARMSILENTTIMER);
					getModel()->StopCountDownAlarmSilent();
					m_iCountDownAlarmSilent=0;
				}
				else
				{
					KillTimer(ALARMSILENTTIMER);
					m_iCountDownAlarmSilent=MANUALSILENT_TIME;
					getModel()->SetCountDownAlarmSilent(m_iCountDownAlarmSilent);
					PostMessage(WM_ALARMSILENTTIMER);
					SetTimer(ALARMSILENTTIMER,TIMEALARMSILENT,NULL);
				}
				return 1;
			}
			break;
		case WM_CALSILENT_ON:
			{
				if(getModel()->GetVentRunState()==VENT_STOPPED)
				{
					KillTimer(ALARMSILENTTIMER);
					getModel()->StopCountDownAlarmSilent();
					m_iCountDownAlarmSilent=0;
				}
				else
				{
					if(m_iCountDownAlarmSilent<STARTUPSILENT_TIME)
					{
						m_iCountDownAlarmSilent=STARTUPSILENT_TIME;
						getModel()->SetCountDownAlarmSilent(m_iCountDownAlarmSilent);
						PostMessage(WM_ALARMSILENTTIMER);
						SetTimer(ALARMSILENTTIMER,TIMEALARMSILENT,NULL);
					}
				}
				return 1;

			}
			break;
		case WM_SET_ACTIVEALARM_DELAY:
			{
				getModel()->setActiveAlarmDelay(true);
				SetTimer(ACTIVEALARMTIMER,getModel()->getCONFIG()->getCurPatientAlarmDelay()*1000,NULL);
				CString szTxt=_T("");
				szTxt.Format(L"#ACTIVEALARMTIMER [%d]",getModel()->getCONFIG()->getCurPatientAlarmDelay());
				theApp.getLog()->WriteLine(szTxt);
				return 1;
			}
			break;
		case WM_SET_STARTUPALARM_DELAY:
			{
				getModel()->setActiveAlarmDelay(true);
				//SetTimer(ACTIVEALARMTIMER,10000,NULL);
				SetTimer(ACTIVEALARMTIMER,120000,NULL);
				return 1;
			}
			break;
		case WM_MANUAL_ALARMSILENT_OFF:
			{
				KillTimer(ALARMSILENTTIMER);
				getModel()->StopCountDownAlarmSilent();
				m_iCountDownAlarmSilent=0;
				return 1;
			}
			break;
		case WM_AUTO_ALARMSILENT_ON:
			{
				if(getModel()->GetVentRunState()==VENT_STOPPED)
				{
					KillTimer(ALARMSILENTTIMER);
					getModel()->StopCountDownAlarmSilent();
					m_iCountDownAlarmSilent=0;
				}
				else
				{
					m_iCountDownAlarmSilent=AUTOSILENT_TIME;
					getModel()->SetCountDownAlarmSilent(m_iCountDownAlarmSilent);
					PostMessage(WM_ALARMSILENTTIMER);
					SetTimer(ALARMSILENTTIMER,TIMEALARMSILENT,NULL);
				}
				return 1;
			}
			break;
		/*case WM_AUTO_ALARMSILENT_OFF:
			{
				getModel()->getVIEWHANDLER()->StopCountDownAlarmSilent();
				m_iCountDownAlarmSilent=0;
				KillTimer(ALARMSILENTTIMER);

				getModel()->getALARMHANDLER()->SetAlarmsilentActive();

				CMVEventAlarm eventCtrl(CMVEventAlarm::EV_ALARM_REACTIVATED);
				getModel()->Trigger(&eventCtrl);
			}
			break;*/
		case WM_SYSALARM_OFF:
			{
				KillTimer(ALARMSILENTTIMER);

				getModel()->StopCountDownAlarmSilent();
				m_iCountDownAlarmSilent=0;
				
				CMVEventAlarm event(CMVEventAlarm::EV_ALARMSTATE_CHANGED);
				getModel()->triggerEvent(&event);
				return 1;
			}
			break;
		case WM_VENT_STARTSTOP_DOWN:
			{
				if(		getModel()->getCONFIG()->GetCurMode()==VM_NCPAP
					||	getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
					||	getModel()->getCONFIG()->GetCurMode()==VM_THERAPIE)
				{
					if(getModel()->GetVentRunState()==VENT_STANDBY)
					{
						m_iCountTimeUntilStop=0;

						getModel()->SetVentRunState(VENT_RUNNING);
						getModel()->getALARMHANDLER()->setAutoSilent(false,true,false);

						CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_START_VENTILATION);
						getModel()->triggerEvent(&eventCtrl);
					}
					else if(getModel()->GetVentRunState()==VENT_RUNNING)
					{
						SetTimer(STARTSTOPTIMER,TIMESTARTSTOP,NULL);
					}
					else //VENT_STOPPED -> sollte nicht vorkommen
					{
						
					}
				}
				else
				{
					if(getModel()->GetVentRunState()==VENT_RUNNING)
					{
						SetTimer(STARTSTOPTIMER,TIMESTARTSTOP,NULL);
					}
					else if(getModel()->GetVentRunState()==VENT_STOPPED)
					{
						KillTimer(COUNTSTOPBREATHTIMER);

						getModel()->SetVentRunState(VENT_RUNNING);
						getModel()->getALARMHANDLER()->setAutoSilent(false,true,false);
						
						CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_START_VENTILATION);
						getModel()->triggerEvent(&eventCtrl);
					}
					else  //VENT_STANDBY -> sollte nur nach Video-Modus vorkommen
					{
						KillTimer(COUNTSTOPBREATHTIMER);
						
						getModel()->SetVentRunState(VENT_RUNNING);
						getModel()->getALARMHANDLER()->setAutoSilent(false,true,false);

						CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_START_VENTILATION);
						getModel()->triggerEvent(&eventCtrl);
					}
				}
				return 1;
			}
			break;
		case WM_VENT_STARTSTOP_UP:
			{
				KillTimer(STARTSTOPTIMER);
				
				if(m_iCountTimeUntilStop<6)
				{
					getModel()->getVIEWHANDLER()->StopCountTimeUntilStop();

					if(getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_SILENT)
					{
						//getModel()->getALARMHANDLER()->SetAlarmsilentActive();
						getModel()->getALARMHANDLER()->setAutoSilent(false,true,false);
					}
				}

				m_iCountTimeUntilStop=0;
				return 1;

			}
			break;
		case WM_STARTSTOPTIMER:
			{
				if(getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SILENT)
				{
					getModel()->getALARMHANDLER()->setAlarmSilent(false,true);
				}

				getModel()->getVIEWHANDLER()->DrawCountTimeUntilStop(m_iCountTimeUntilStop);
				m_iCountTimeUntilStop++;

				if(m_iCountTimeUntilStop>5)
				{
					KillTimer(STARTSTOPTIMER);
					getModel()->getVIEWHANDLER()->StopCountTimeUntilStop();
					PostMessage(WM_VENTSTATE_STOP);
				}
				return 1;
			}
			break;
		case WM_VENT_STANDBY:
			{
				if(getModel()->GetVentRunState()==VENT_RUNNING)
				{
					getModel()->SetVentRunState(VENT_STANDBY);

					if(		getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_AUTOSILENT
						||	getModel()->getALARMHANDLER()->isManualSilentState()==true)
					{
						getModel()->StopCountDownAlarmSilent();
						m_iCountDownAlarmSilent=0;
					}
					else if(getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
					{
						m_iCountDownAlarmSilent=0;
						getModel()->SetCountDownAlarmSilent(m_iCountDownAlarmSilent);
					}

					getModel()->getALARMHANDLER()->setAlarmSilent();


					CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_STANDBY);
					getModel()->triggerEvent(&eventCtrl);
				}
				else if(getModel()->GetVentRunState()==VENT_STOPPED)
				{
					KillTimer(COUNTSTOPBREATHTIMER);

					getModel()->SetVentRunState(VENT_STANDBY);

					if(		getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_AUTOSILENT
						||	getModel()->getALARMHANDLER()->isManualSilentState()==true)
					{
						getModel()->StopCountDownAlarmSilent();
						m_iCountDownAlarmSilent=0;
					}
					else if(getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
					{
						m_iCountDownAlarmSilent=0;
						getModel()->SetCountDownAlarmSilent(m_iCountDownAlarmSilent);
					}

					getModel()->getALARMHANDLER()->setAlarmSilent();


					CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_STANDBY);
					getModel()->triggerEvent(&eventCtrl);
				}
				
				return 1;
			}
			break;
		case WM_VENT_RUN:
			{
				if(getModel()->GetVentRunState()==VENT_STANDBY)
				{
					m_iCountTimeUntilStop=0;

					getModel()->SetVentRunState(VENT_RUNNING);
					getModel()->getALARMHANDLER()->setAutoSilent(false,true,false);

					CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_START_VENTILATION);
					getModel()->triggerEvent(&eventCtrl);
				}
				return 1;
			}
			break;
		case WM_VENTSTATE_STOP:
			{
				if(		getModel()->getCONFIG()->GetCurMode()==VM_NCPAP
					||	getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
					||	getModel()->getCONFIG()->GetCurMode()==VM_THERAPIE)
				{
					if(getModel()->GetVentRunState()==VENT_RUNNING)
					{
						getModel()->SetVentRunState(VENT_STANDBY);

						if(		getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_AUTOSILENT
							||	getModel()->getALARMHANDLER()->isManualSilentState()==true)
						{
							getModel()->StopCountDownAlarmSilent();
							m_iCountDownAlarmSilent=0;
						}
						else if(getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
						{
							m_iCountDownAlarmSilent=0;
							getModel()->SetCountDownAlarmSilent(m_iCountDownAlarmSilent);
						}

						getModel()->getALARMHANDLER()->setAlarmSilent();


						CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_STANDBY);
						getModel()->triggerEvent(&eventCtrl);
					}
					else if(getModel()->GetVentRunState()==VENT_STANDBY)
					{
						//ERROR
						KillTimer(COUNTSTOPBREATHTIMER);

						getModel()->SetVentRunState(VENT_RUNNING);
						getModel()->getALARMHANDLER()->setAutoSilent(false,true,false);

						
						CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_START_VENTILATION);
						getModel()->triggerEvent(&eventCtrl);
					}
					else //VENT_STOPPED -> sollte nicht vorkommen
					{
						KillTimer(COUNTSTOPBREATHTIMER);

						getModel()->SetVentRunState(VENT_RUNNING);
						getModel()->getALARMHANDLER()->setAutoSilent(false,true,false);
						
						CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_START_VENTILATION);
						getModel()->triggerEvent(&eventCtrl);
					}
				}
				else
				{
					if(getModel()->GetVentRunState()==VENT_RUNNING)
					{
						getModel()->getALARMHANDLER()->setAlarmSilent(false,true);

						SetTimer(COUNTSTOPBREATHTIMER,COUNTTIMESTARTSTOP,NULL);
						m_iCountDownStart=STANDBY;

						if(		getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_AUTOSILENT
							||	getModel()->getALARMHANDLER()->isManualSilentState()==true)
						{
							getModel()->StopCountDownAlarmSilent();
							m_iCountDownAlarmSilent=0;
						}
						else if(getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
						{
							m_iCountDownAlarmSilent=0;
							getModel()->SetCountDownAlarmSilent(m_iCountDownAlarmSilent);
						}
						
						getModel()->SetVentRunState(VENT_STOPPED);
						getModel()->getVIEWHANDLER()->DrawCountDownStartTime(m_iCountDownStart);

						CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_STOP_VENTILATION);
						getModel()->triggerEvent(&eventCtrl);
					}
					else if(getModel()->GetVentRunState()==VENT_STOPPED)
					{
						//ERROR
						KillTimer(COUNTSTOPBREATHTIMER);

						getModel()->SetVentRunState(VENT_RUNNING);
						getModel()->getALARMHANDLER()->setAutoSilent(false,true,false);
						
						CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_START_VENTILATION);
						getModel()->triggerEvent(&eventCtrl);
					}
					else  //VENT_STANDBY -> sollte nicht vorkommen
					{
						KillTimer(COUNTSTOPBREATHTIMER);

						getModel()->SetVentRunState(VENT_RUNNING);
						getModel()->getALARMHANDLER()->setAutoSilent(false,true,false);
						
						CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_START_VENTILATION);
						getModel()->triggerEvent(&eventCtrl);
					}
				}
				//getModel()->getSERIAL()->Send_MODE_OPTION();
				//getModel()->getSPI()->Send_MODE_OPTION();

				return 1;
			}
			break;
		case WM_VENT_OFF_DOWN:
			{
				SetTimer(OFFTIMER,TIMESTARTSTOP,NULL);
				return 1;
			}
			break;
		case WM_VENT_OFF_UP:
			{
				KillTimer(OFFTIMER);

				if(m_iCountTimeUntilOff<6)
				{
					getModel()->getVIEWHANDLER()->StopCountTimeUntilOff();

					if(getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_SILENT
						&& getModel()->GetVentRunState()!=VENT_STANDBY)
					{
						getModel()->getALARMHANDLER()->setAlarmsilentActive();
					}
				}

				m_iCountTimeUntilOff=0;
				return 1;
			}
			break;
		case WM_OFFTIMER:
			{
				if(getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SILENT)
				{
					getModel()->getALARMHANDLER()->setAlarmSilent();
				}

				getModel()->getVIEWHANDLER()->DrawCountTimeUntilOff(m_iCountTimeUntilOff);
				m_iCountTimeUntilOff++;

				if(m_iCountTimeUntilOff>5)
				{
					KillTimer(OFFTIMER);
					getModel()->getVIEWHANDLER()->StopCountTimeUntilOff();
					PostMessage(WM_VENT_TURNOFF);
				}
				return 1;
			}
			break;
		case WM_VENT_TURNOFF:
			{
				theApp.SetEXITState();
				//if(AfxGetApp())
				//	AfxGetApp()->GetMainWnd()->PostMessage(WM_THR_STOP,THR_MAIN);//rkuNEWFIX
				
				m_iCountTimeUntilOff=0;
				KillTimer(OFFTIMER);

				getModel()->SetVentRunState(VENT_STOPPED);
				getModel()->Send_MODE_OPTION1();

			
				getModel()->getVIEWHANDLER()->changeViewState(VS_SHUTDOWN,VSS_NONE);
				
				PostMessage(WM_QUIT_VENTILATOR);
				
				return 1;
			}
			break;
		case WM_QUIT_VENTILATOR:
			{
				QuitVentilator();
			}
			break;
		case WM_OXYCAL_HOURGLASS_TIMER:
			{
				int iStateHourglass=getModel()->getSPI()->Read_STATUS_2();

				if(!isSafeTickCountDelayExpired(m_dwHourglasstimeout, 80000))
				{
					if(iStateHourglass&BIT3)
					{
						//do nothing
					}
					else
					{
						if(!m_bExit)
						{
							getModel()->getVIEWHANDLER()->SetOxyHourglass(false);
							getModel()->getDATAHANDLER()->SetOxyHourglassRunning(false);
						}

						KillTimer(OXYCAL_HOURGLASS_TIMER);
					}
				}
				else
				{
					if(!m_bExit)
					{
						getModel()->getVIEWHANDLER()->SetOxyHourglass(false);
						getModel()->getDATAHANDLER()->SetOxyHourglassRunning(false);
					}
					KillTimer(OXYCAL_HOURGLASS_TIMER);
				}

			}
			break;
		case WM_CO2PUMPTIMER:
			{
				if(getModel()->getDATAHANDLER()->getRemainCO2PumpTime()<60000 && m_bShowCO2PumpOnMsg)
				{
					m_bShowCO2PumpOnMsg=false;
					CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, getModel()->GetLanguageString(IDS_TXT_CO2PUMPENABLED), 5000);
					getModel()->triggerEvent(&event);
					getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
					Sleep(300);
					getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				}
				if(getModel()->getDATAHANDLER()->getRemainCO2PumpTime()<=0)
				{
					getModel()->getDATAHANDLER()->setRemainCO2PumpTime(0);
					//m_ullRemainCO2PumpTime=0;
					m_bShowCO2PumpOnMsg=true;
					KillTimer(CO2PUMPTIMER);

					if(getModel()->getETCO2()!=NULL)
						getModel()->getETCO2()->set_PumpStateOn();

					getModel()->getVIEWHANDLER()->UpdateCO2InfoData(false);

					getModel()->getALARMHANDLER()->setAutoSilent();

					CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, getModel()->GetLanguageString(IDS_TXT_CO2PUMPON), 5000);
					getModel()->triggerEvent(&event);
					getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
					Sleep(300);
					getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);

				}
				else
				{
					//getModel()->getVIEWHANDLER()->UpdateCO2InfoData(m_ullRemainCO2PumpTime/1000);
					getModel()->getVIEWHANDLER()->UpdateCO2InfoData(true);
				}
			}
			break;
		case WM_TRIGGER_MANUALBREATH_STOP:
			{
				if(getModel()->isMANBREATHrunning())
				{
					KillTimer(MANBREATHTIMER);

					
					getModel()->getSERIAL()->Send_STOP_MAN_BREATH();
					
					CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_STOP_MANBREATH);
					getModel()->triggerEvent(&eventCtrl);

					getModel()->deactivateMANBREATH();
				}
				return 1;
			}
			break;
		case WM_TRIGGER_FLOWSENSORSTATE:
			{
				CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_FLOWSENSORSTATE);
				getModel()->triggerEvent(&eventCtrl);
				return 1;
			}
			break;
		case WM_TRIGGER_STATE:
			{
				CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_TRIGGERSTATE);
				getModel()->triggerEvent(&eventCtrl);
				return 1;
			}
			break;
		case WM_TRIGGER_O2SENSORSTATE:
			{
				CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_O2SENSORSTATE);
				getModel()->triggerEvent(&eventCtrl);
				return 1;
			}
			break;
		case WM_VENTMODE_HOME:
			{
				SetTimer(HOMETIMER,TIMEMODECHANGE,NULL);
				return 1;
			}
			break;
		case WM_VENTMODE_CHANGED:
			{
				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
				{
					eVentMode eActiveVentMode=getModel()->getCONFIG()->GetCurMode();

					if(	m_eVentModeCalcStarted!=eActiveVentMode 
						&&	getModel()->CalculateAlarmlimitRunning())
					{
						getModel()->getALARMHANDLER()->SetAlimitStateCalculated();

						PostMessage(WM_DATA_ALARMLIMITS_CHANGED);
					}
				}
				

				if((getModel()->getCONFIG()->CurModeIsPresetMode()==false) && m_bBackup==true)
				{
					DEBUGMSG(TRUE, (TEXT("WM_VENTMODE_CHANGED delete backup\r\n")));
					m_bBackup=false;
					getModel()->getVIEWHANDLER()->SetCPAPBackup(false);
				}

				if(		(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					&&	(getModel()->getCONFIG()->GetPrevMode()!=getModel()->getCONFIG()->GetCurMode()))
				{
					eVentMode curMode=getModel()->getCONFIG()->GetCurMode();
					switch(curMode)
					{
					case VM_IPPV:
						{
							theApp.getLog()->WriteLine(_T("### Ventmode VM_IPPV"));
						}
						break;
					case VM_SIPPV:
						{
							theApp.getLog()->WriteLine(_T("### Ventmode VM_SIPPV"));
						}
						break;
					case VM_SIMV:
						{
							theApp.getLog()->WriteLine(_T("### Ventmode VM_SIMV"));
						}
						break;
					case VM_SIMVPSV:
						{
							theApp.getLog()->WriteLine(_T("### Ventmode VM_SIMVPSV"));
						}
						break;
					case VM_PSV:
						{
							theApp.getLog()->WriteLine(_T("### Ventmode VM_PSV"));
						}
						break;
					case VM_CPAP:
						{
							theApp.getLog()->WriteLine(_T("### Ventmode VM_CPAP"));
						}
						break;
					case VM_NCPAP:
						{
							theApp.getLog()->WriteLine(_T("### Ventmode VM_NCPAP"));
						}
						break;
					case VM_DUOPAP:
						{
							theApp.getLog()->WriteLine(_T("### Ventmode VM_DUOPAP"));
						}
						break;
					case VM_HFO:
						{
							theApp.getLog()->WriteLine(_T("### Ventmode VM_HFO"));
						}
						break;
					case VM_THERAPIE:
						{
							theApp.getLog()->WriteLine(_T("### Ventmode VM_THERAPIE"));
						}
						break;
					case VM_SERVICE:
						{
							theApp.getLog()->WriteLine(_T("### Ventmode VM_SERVICE"));
						}
						break;
					}

					if(getModel()->GetVentRunState()==VENT_STANDBY)
					{
						if(		curMode==VM_IPPV
							||	curMode==VM_SIPPV
							||	curMode==VM_SIMV
							||	curMode==VM_SIMVPSV
							||	curMode==VM_PSV
							||	curMode==VM_HFO
							||	curMode==VM_CPAP)
						{
							getModel()->getALARMHANDLER()->setAlarmSilent();

							SetTimer(COUNTSTOPBREATHTIMER,COUNTTIMESTARTSTOP,NULL);
							m_iCountDownStart=STANDBY;//120;

							if(		getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_AUTOSILENT
								||	getModel()->getALARMHANDLER()->isManualSilentState()==true)
							{
								getModel()->StopCountDownAlarmSilent();
								m_iCountDownAlarmSilent=0;
							}
							else if(getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
							{
								m_iCountDownAlarmSilent=0;
								getModel()->SetCountDownAlarmSilent(m_iCountDownAlarmSilent);
							}

							getModel()->SetVentRunState(VENT_STOPPED);
							CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_STOP_VENTILATION);
							getModel()->triggerEvent(&eventCtrl);
						}
					}
					else if(getModel()->GetVentRunState()==VENT_STOPPED)
					{
						if(		curMode==VM_NCPAP
							||	curMode==VM_DUOPAP
							||	curMode==VM_THERAPIE)
						{
							KillTimer(COUNTSTOPBREATHTIMER);
							getModel()->SetVentRunState(VENT_STANDBY);
							getModel()->getVIEWHANDLER()->StopCountDownStartTime();


							if(		getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_AUTOSILENT
								||	getModel()->getALARMHANDLER()->isManualSilentState()==true)
							{
								getModel()->StopCountDownAlarmSilent();
								m_iCountDownAlarmSilent=0;
							}
							else if(getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
							{
								m_iCountDownAlarmSilent=0;
								getModel()->SetCountDownAlarmSilent(m_iCountDownAlarmSilent);
							}

							getModel()->getALARMHANDLER()->setAlarmSilent();
							CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_STANDBY);
							getModel()->triggerEvent(&eventCtrl);
						}
					}

					if(getModel()->isO2FlushActive())
					{
						PostMessage(WM_O2FLUSH);//ausschalten wenn aktiv
					}
				}

				KillTimer(HOMETIMER);
				return 1;
			}
			break;
		case WM_CALCULATE_SINGLE_AUTOLIMIT:
			{
				CalculateSingleAutoLimit((eAlarmLimitPara)wParam);

				//getModel()->getALARMHANDLER()->setAutoSilent(false,false,false);
				getModel()->getDATAHANDLER()->checkLimits();
				return 1;
			}
			break;
		case WM_ACTIVATE_MANBREATH:
			{
				m_bMatrxDwn_MANBREATHdisabled=false;
				m_bMatrxDwn_MANBREATH=true;

				getModel()->ResetExhalValvCalMode();

				getModel()->activateMANBREATH();

				int iTime=(getModel()->getCONFIG()->GetManBreathTime())*1000;
				SetTimer(MANBREATHTIMER,iTime,NULL);

				getModel()->getSERIAL()->Send_START_MAN_BREATH();
				PostMessage(WM_CONTROL_START_MANBREATH);
				return 1;
			}
			break;
		case WM_DEACTIVATE_MANBREATH:
			{
				if(getModel()->isMANBREATHrunning())
				{
					KillTimer(MANBREATHTIMER);


					getModel()->getSERIAL()->Send_STOP_MAN_BREATH();

					CMVEventControl eventCtrl(CMVEventControl::EV_CONTROL_STOP_MANBREATH);
					getModel()->triggerEvent(&eventCtrl);

					getModel()->deactivateMANBREATH();
				}
				return 1;
			}
			break;
		/*case WM_CALCULATE_ALL_AUTOLIMIT:
			{
				CalculateAllAutoLimit();

				getModel()->getALARMHANDLER()->setAutoSilent(false,false,false);
				getModel()->getDATAHANDLER()->checkLimits();
				return 1;
			}
			break;*/
		default:
			{
				int iStop=0;
			}
			break;
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CMainFrame::DefWindowProc()"));
	}

	return CFrameWnd::DefWindowProc(message, wParam, lParam);
}


//void CMainFrame::SendScreenShot()
//{
//	//CStringW sPath1 = _T("\\FFSDISK\\Trends\\test1.bmp");
//	//ifstream bmpfile;
//	//bmpfile.open(sPath1, ios::in|ios::binary);
//	//if(bmpfile == NULL)
//	//	return;
//	//bmpfile.seekg(0,ios::end);
//	//int size = bmpfile.tellg();
//	//bmpfile.seekg(0,ios::beg);
//
//	////size = htonl(size);
//	////send(clientSocket,(char*)&size,sizeof(size),0); //send the size of file
//	////size = ntohl(size);
//
//
//	////while(!bmpfile.eof())
//	////{
//	////	char Byte[1] = {0};
//	////	Byte[0] = bmpfile.get();
//	////	send(clientSocket,Byte,1,0); //send bitmap byte by byte
//	////}
//	////while(!bmpfile.eof())
//	////{
//	////	char Byte[1] = {0};
//	////	Byte[0] = bmpfile.get();
//	////	send(clientSocket,Byte,1,0); //send bitmap byte by byte
//	////}
//
//
//	//bmpfile.close();
//
//}

//void CMainFrame::SaveScreenShot()
//{
//	CDC* pDC = GetDC();
//	
//
//	//create a memory dc 
//	CDC memDC;
//	memDC.CreateCompatibleDC(pDC);
//
//	//Create a memory bitmap
//	CBitmap newbmp;
//	newbmp.CreateCompatibleBitmap(pDC, m_nX,m_nY);
//
//	//select the bitmap in the memory dc
//	CBitmap *pOldBitmap = memDC.SelectObject(&newbmp); 
//
//	//blit from screen into memory dc
//	memDC.BitBlt(0,0,m_nX,m_nY,pDC,0,0,SRCCOPY);
//
//	//select old bitmap back into the memory dc
//	memDC.SelectObject(pOldBitmap);
//
//	//release the Display DC
//	ReleaseDC(pDC);
//
//
//	CStringW sName = _T("\\FFSDISK\\Bmp\\screen.bmp");
//	CDIBSectionLite dibsection;
//	//dibsection.SetBitmap(IDB_PIC_SYSALARM); 
//	dibsection.SetBitmap((HBITMAP)newbmp); 
//
//	//dibsection.Draw(CDC::FromHandle(hdcMem), CPoint(0,0));
//
//	CStringW sPath = _T("\\FFSDISK\\Bmp");
//	
//	dibsection.Save(sPath,sName);
//
//	//newbmp.Detach();
//	newbmp.DeleteObject();
//
//
//}









//The following is the functions that I used for my Windows CE application.
//There are two example functions at the end.
//
//
//CStringW ConvertAnsiToUnicode(const char * TheStr)
//{
//	static CStringW Transfer;
//	Transfer = TheStr;
//	return Transfer;
//}
//
//char * ConvertUnicodeToAnsi(const TCHAR * Source, char * DestBuf)
//{
//	unsigned UnicodeLen = _tcslen(Source);
//	for (unsigned d=0;d<UnicodeLen;d++)
//	{
//		DestBuf[d] = static_cast <char> (Source[d]);
//	}
//	DestBuf[d] =0;
//	return DestBuf;
//}
//char * ConvertUnicodeToAnsi(const TCHAR * Source)
//{
//	static char UnicodeToAnsiBuffer[4048];
//	return ConvertUnicodeToAnsi(Source, UnicodeToAnsiBuffer);
//}
//
//void dlgFSR_Test::OnWriteToFile() 
//{
//	char data[] = "This is a test on char line\nSecond Line for char";
//	CStdioFile file;
//	TCHAR wdata1[] = _T("This is a test1 on wdata1 line\nSecond Line for wdata1");
//	file.Open(_T("wchr1.txt"),CFile::typeText|CFile::modeWrite|CFile::modeCreate);
//	file.Write(ConvertUnicodeToAnsi(wdata1),_tcslen(wdata1));
//	PostFeedBackMsg(_T("Finish writing wchr1 data."));
//	file.Close();
//
//	CStringW strData = _T("This is a test2 on wdata2 line\nSecond Line for wdata2");
//	file.Open(_T("wchr2.txt"),CFile::typeText|CFile::modeWrite|CFile::modeCreate);
//	file.Write(ConvertUnicodeToAnsi(strData.operator LPCTSTR()),strData.GetLength());
//	PostFeedBackMsg(_T("Finish writing wchr2 data."));
//	file.Close();
//}
//
//void dlgFSR_Test::OnReadToFile2() 
//{
//	char data[300];
//	CStdioFile file;
//	file.Open(_T("wchr1.txt"),CFile::typeText|CFile::modeRead);
//	file.Read(data,sizeof(data));
//	PostFeedBackMsg(_T("Finish reading test1 data."));
//	file.Close();
//	CStringW Msg =  data;
//	PostFeedBackMsg(Msg);
//
//	file.Open(_T("wchr2.txt"),CFile::typeText|CFile::modeRead);
//	file.Read(data,sizeof(data));
//	PostFeedBackMsg(_T("Finish reading test2 data."));
//	file.Close();
//	Msg =  data;
//	PostFeedBackMsg(Msg);
//}

/**********************************************************************************************//**
 * Stops the threads
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::stopThreads()
{
	StopSaveTrendUSBThread();
	StopDelTrendThread();
	StopOxyCalThread();
	StopTimerThread();
	StopI2CWatchdogThread();
}

/**********************************************************************************************//**
 * Stops a watchdog
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::stopWatchdog()
{
#ifndef _DEBUG	
	KillTimer(WATCHDOGTIMER);

	// *"Normal" exit (m_bRun=false)*****************************************
	if(!StopWatchDogTimer(g_hWatchDog,0))
	{
		// TODO:		
		//DWORD dwError=GetLastError();
		//return;
	}
	CloseHandle(g_hWatchDog);

	//DWORD dwResult=WaitForSingleObject(g_hAction,2*WD_PERIODE);
	DWORD dwResult=WaitForSingleObject(g_hAction,3000);
	if(dwResult&WAIT_OBJECT_0)
	{
		// TODO:
		/*CTlsRegistry reg(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
		reg.WriteDWORD(_T("Watchdog3"), 0);*/
		//DWORD dwError=GetLastError();
		//return 0;
	}
#endif

	::CloseHandle(g_hAction);
}

/**********************************************************************************************//**
 * Quit ventilator
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::QuitVentilator()
{
	m_bExit=true;

	theApp.getLog()->WriteLine(L"**** QuitVentilator ****");

	getModel()->getSOUND()->SetPIFSound(PIF_SHUTDOWN);
	getModel()->getSPI()->send_Shutdown();

	KillTimer(CHECKCOM_TIMER);
	KillTimer(PMIN_ALARMDELAY_TIMER);
	KillTimer(OXYCAL_HOURGLASS_TIMER);
	KillTimer(DISABLE_PATALARM_O2_TIMER);
	KillTimer(UDPDOWNUPTIMER);
	KillTimer(AUTOSCREENLOCKTIMER);
	KillTimer(HOMETIMER);
	KillTimer(AUTOLIMITTIMER);
	KillTimer(O2FLUSHTIMER);
	KillTimer(COUNTSTOPBREATHTIMER);
	KillTimer(ALARMSILENTTIMER);
	KillTimer(STARTSTOPTIMER);
	KillTimer(MANBREATHTIMER);
	KillTimer(CO2PUMPTIMER);
	KillTimer(ACTIVEALARMTIMER);

	BYTE iPDMSProtocol=getModel()->getCONFIG()->GetPDMSprotocol();
	if(		iPDMSProtocol==ACL_SERIAL_ASCII
		||	iPDMSProtocol==ACL_SERIAL_WAVE
		||	iPDMSProtocol==ACL_ETHERNET_WAVE
		||	iPDMSProtocol==ACL_ETHERNET
		||	iPDMSProtocol==ACL_SERIAL_IVOI)
	{
		if(getModel()->getAcuLink()!=NULL)
			getModel()->getAcuLink()->setShutdown(1);
	}

	CString szOpTime=_T("");
	int iBat=getModel()->getDATAHANDLER()->getOpTimeBatteryMin(true);
	int iDev=getModel()->getDATAHANDLER()->getOpTimeDeviceMin(true);
	int iHFO=getModel()->getDATAHANDLER()->getOpTimeHFOMin(true);
	szOpTime.Format(_T("#OperatingTime [minutes]: battery %d device %d HFO %d#"),iBat, iDev, iHFO);
	theApp.getLog()->WriteLine(szOpTime);

	m_bStartInstaller=getModel()->IsInstaller();

	//I2C turnoff 
#ifndef SIMULATION_ENTREK
	if(getModel()->IsAccuTurnoff()==false && !m_bStartInstaller && !m_bForceShutdown)
		getModel()->getI2C()->WriteAccuWord(ACCU_DAT_CMND,ACCU_CMND_SHUTDOWN);
#endif

	//getModel()->getSOUND()->SetPIFSound(PIF_SHUTDOWN);

	stopThreads();
	stopWatchdog();

	try
	{
		getModel()->Deinit();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("getModel()->Deinit: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("getModel()->Deinit"));
	}

	getModel()->DestroyInstance();
	m_pModel=NULL;

	theApp.CloseLog();

	PostMessage(WM_CLOSE);
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();
	

	if(m_hf10Bold!=NULL)
	{
		DeleteObject(m_hf10Bold);
		m_hf10Bold=NULL;
	}
	if(m_hf13Bold!=NULL)
	{
		DeleteObject(m_hf13Bold);
		m_hf13Bold=NULL;
	}

	if(m_hf10Norm!=NULL)
	{
		DeleteObject(m_hf10Norm);
		m_hf10Norm=NULL;
	}
	if(m_hf11Norm!=NULL)
	{
		DeleteObject(m_hf11Norm);
		m_hf11Norm=NULL;
	}
	if(m_hf12Norm!=NULL)
	{
		DeleteObject(m_hf12Norm);
		m_hf12Norm=NULL;
	}
	if(m_hf12Norm90degree!=NULL)
	{
		DeleteObject(m_hf12Norm90degree);
		m_hf12Norm90degree=NULL;
	}
	if(m_hf13Norm!=NULL)
	{
		DeleteObject(m_hf13Norm);
		m_hf13Norm=NULL;
	}
	if(m_hf14Norm!=NULL)
	{
		DeleteObject(m_hf14Norm);
		m_hf14Norm=NULL;
	}
	if(m_hf14Bold!=NULL)
	{
		DeleteObject(m_hf14Bold);
		m_hf14Bold=NULL;
	}
	if(m_hf14Bold90degree!=NULL)
	{
		DeleteObject(m_hf14Bold90degree);
		m_hf14Bold90degree=NULL;
	}
	if(m_hf15Normal!=NULL)
	{
		DeleteObject(m_hf15Normal);
		m_hf15Normal=NULL;
	}
	if(m_hf12AcuNormNum!=NULL)
	{
		DeleteObject(m_hf12AcuNormNum);
		m_hf12AcuNormNum=NULL;
	}
	if(m_hf14AcuNormNum!=NULL)
	{
		DeleteObject(m_hf14AcuNormNum);
		m_hf14AcuNormNum=NULL;
	}
	if(m_hf15Bold!=NULL)
	{
		DeleteObject(m_hf15Bold);
		m_hf15Bold=NULL;
	}
	if(m_hf16Normal!=NULL)
	{
		DeleteObject(m_hf16Normal);
		m_hf16Normal=NULL;
	}
	if(m_hf16Bold!=NULL)
	{
		DeleteObject(m_hf16Bold);
		m_hf16Bold=NULL;
	}
	if(m_hf16Bold90degree!=NULL)
	{
		DeleteObject(m_hf16Bold90degree);
		m_hf16Bold90degree=NULL;
	}
	if(m_hf17Bold!=NULL)
	{
		DeleteObject(m_hf17Bold);
		m_hf17Bold=NULL;
	}
	if(m_hf18Normal!=NULL)
	{
		DeleteObject(m_hf18Normal);
		m_hf18Normal=NULL;
	}
	if(m_hf18Bold!=NULL)
	{
		DeleteObject(m_hf18Bold);
		m_hf18Bold=NULL;
	}
	if(m_hf18BoldNum!=NULL)
	{
		DeleteObject(m_hf18BoldNum);
		m_hf18BoldNum=NULL;
	}
	if(m_hf20Bold!=NULL)
	{
		DeleteObject(m_hf20Bold);
		m_hf20Bold=NULL;
	}
	if(m_hf20BoldNum!=NULL)
	{
		DeleteObject(m_hf20BoldNum);
		m_hf20BoldNum=NULL;
	}
	if(m_hf21Medium!=NULL)
	{
		DeleteObject(m_hf21Medium);
		m_hf21Medium=NULL;
	}
	if(m_hf21Bold!=NULL)
	{
		DeleteObject(m_hf21Bold);
		m_hf21Bold=NULL;
	}
	if(m_hf22Medium!=NULL)
	{
		DeleteObject(m_hf22Medium);
		m_hf22Medium=NULL;
	}
	if(m_hf22Bold!=NULL)
	{
		DeleteObject(m_hf22Bold);
		m_hf22Bold=NULL;
	}
	if(m_hf24Bold!=NULL)
	{
		DeleteObject(m_hf24Bold);
		m_hf24Bold=NULL;
	}
	if(m_hf26Medium!=NULL)
	{
		DeleteObject(m_hf26Medium);
		m_hf26Medium=NULL;
	}
	if(m_hf28Bold!=NULL)
	{
		DeleteObject(m_hf28Bold);
		m_hf28Bold=NULL;
	}
	if(m_hf30Bold!=NULL)
	{
		DeleteObject(m_hf30Bold);
		m_hf30Bold=NULL;
	}
	if(m_hf32Medium!=NULL)
	{
		DeleteObject(m_hf32Medium);
		m_hf32Medium=NULL;
	}
	if(m_hf34Bold!=NULL)
	{
		DeleteObject(m_hf34Bold);
		m_hf34Bold=NULL;
	}
	if(m_hf34BoldNum!=NULL)
	{
		DeleteObject(m_hf34BoldNum);
		m_hf34BoldNum=NULL;
	}
	if(m_hf38Bold!=NULL)
	{
		DeleteObject(m_hf38Bold);
		m_hf38Bold=NULL;
	}
	if(m_hf40Bold!=NULL)
	{
		DeleteObject(m_hf40Bold);
		m_hf40Bold=NULL;
	}
	if(m_hf50Bold!=NULL)
	{
		DeleteObject(m_hf50Bold);
		m_hf50Bold=NULL;
	}
	if(m_hf60Bold!=NULL)
	{
		DeleteObject(m_hf60Bold);
		m_hf60Bold=NULL;
	}
	if(m_hf70Bold!=NULL)
	{
		DeleteObject(m_hf70Bold);
		m_hf70Bold=NULL;
	}
	if(m_hf31AcuBoldNum!=NULL)
	{
		DeleteObject(m_hf31AcuBoldNum);
		m_hf31AcuBoldNum=NULL;
	}
	if(m_hf33AcuBoldNum!=NULL)
	{
		DeleteObject(m_hf33AcuBoldNum);
		m_hf33AcuBoldNum=NULL;
	}
	if(m_hf70BoldNum!=NULL)
	{
		DeleteObject(m_hf70BoldNum);
		m_hf70BoldNum=NULL;
	}

	UnregisterFFSDISKFonts();
	UnregisterSDCardFonts();


	if(m_pcLogo)
		delete m_pcLogo;
	m_pcLogo=NULL;

	

	//old installer version
	if(m_bStartInstaller && CTlsFile::Exists(_T("\\Hard Disk\\SETUP\\SetupFabian.exe")))
	{
		CString fileFrom=_T("\\Hard Disk\\SETUP\\SetupFabian.exe");
		CString szCopyTo=_T("");
		CString sNetdcu11 = _T("\\Hard Disk\\netdcu11");
		CString sNetdcu9 = _T("\\Hard Disk\\netdcu9");
		if(CTlsFile::Exists(sNetdcu11))
		{
			szCopyTo= _T("\\Hard Disk\\netdcu11\\AutoStart.exe");
		}
		else if(CTlsFile::Exists(sNetdcu9))
		{
			szCopyTo= _T("\\Hard Disk\\netdcu9\\AutoStart.exe");
		}

		if(szCopyTo!=_T(""))
		{
			if (CTlsFile::Copy(fileFrom, szCopyTo , true)==true)
			{
				if (CTlsFile::Exists(szCopyTo))
				{
					PROCESS_INFORMATION pi;
					//CreateProcess(_T("\\FFSDisk\\SetupFabian.exe"), 0, NULL, NULL, NULL, 0, NULL, NULL, NULL, &pi);
					CreateProcess(szCopyTo, 0, NULL, NULL, NULL, 0, NULL, NULL, NULL, &pi);
				}
			}
		}
	}




//#ifndef _DEBUG	
//	KillTimer(WATCHDOGTIMER);
//
//	// *"Normal" exit (m_bRun=false)*****************************************
//	if(!StopWatchDogTimer(g_hWatchDog,0))
//	{
//		// TODO:		
//		//DWORD dwError=GetLastError();
//		//return;
//	}
//	CloseHandle(g_hWatchDog);
//
//	//DWORD dwResult=WaitForSingleObject(g_hAction,2*WD_PERIODE);
//	DWORD dwResult=WaitForSingleObject(g_hAction,3000);
//	if(dwResult&WAIT_OBJECT_0)
//	{
//		// TODO:
//		/*CTlsRegistry reg(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
//		reg.WriteDWORD(_T("Watchdog3"), 0);*/
//		//DWORD dwError=GetLastError();
//		//return 0;
//	}
//#endif
//
//	::CloseHandle(g_hAction);

}

/**********************************************************************************************//**
 * Check serial
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::checkSERIAL()
{
	if(getModel()->getSERIAL()->GetM_CHECKSUM(true)>0)
	{
		DEBUGMSG(TRUE, (TEXT("#COM ok\r\n")));

		KillTimer(CHECKCOM_TIMER);

		getModel()->getDATAHANDLER()->deleteAllCOMError();//newVG
		getModel()->setSERIALavailable(TRUE);

		if(getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_NONE)
		{
			getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
		}
		if(getModel()->getCONFIG()->CurModeIsPresetMode())
			getModel()->Send_VENT_MODE(getModel()->getCONFIG()->GetPrevMode());
		else
			getModel()->Send_VENT_MODE(getModel()->getCONFIG()->GetCurMode());
	}
	else
	{
		if(getModel()->getALARMHANDLER()->CanSetAlarm_IF_COM())
			getModel()->getALARMHANDLER()->setAlarm(AL_SysFail_IF_COM);
	}
}

/**********************************************************************************************//**
 * Executes the timer action
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	nIDEvent	The identifier event.
 **************************************************************************************************/

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==WATCHDOGTIMER)
	{

#ifndef _DEBUG
		if(!g_bMainWatchdogPending)
		{
			if(!RefreshWatchDogTimer(g_hWatchDog,0))
			{
				theApp.writeLogError(GetLastError(), _T("WD: Unable to refresh#5"));
			}
		}
#endif
	}
	//else if(nIDEvent==FOT_COLLECT_TIMER)
	//{
	//	if(m_iCountCollectFOTTimer>1)
	//	{
	//		m_iCountCollectFOTTimer--;//sec
	//		getModel()->getVIEWHANDLER()->drawFOTCollectTime(m_iCountCollectFOTTimer);
	//	}
	//	else//m_iCountCollectFOTTimer==0
	//	{
	//		KillTimer(FOT_COLLECT_TIMER);

	//		if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	//			PostMessage(WM_RUN_FOT_HFOSTEP);
	//		else
	//			PostMessage(WM_RUN_FOT_CONVSTEP);
	//		//getModel()->getDATAHANDLER()->setFOTstate(false);
	//	}
	//}
	//else if(nIDEvent==FOT_CONV_TIMER)
	//{
	//	if(m_iCountFOTimer<15)
	//	{
	//		m_iCountFOTimer++;//sec
	//		getModel()->getVIEWHANDLER()->drawFOTtime(m_iCountFOTimer);
	//		if(true==getModel()->getDATAHANDLER()->checkFOTvalidMeasurementData())
	//		{
	//			KillTimer(FOT_CONV_TIMER);
	//			PostMessage(WM_FOT_COLLECT_DATA);
	//		}
	//	}
	//	else//m_iCountFOTimer==15 sec  without valid data!!!!
	//	{
	//		KillTimer(FOT_CONV_TIMER);
	//		PostMessage(WM_RETRY_FOT_CONVSTEP);
	//	}
	//}
	//else if(nIDEvent==FOT_HFO_TIMER)
	//{
	//	if(m_iCountFOTimer<15)
	//	{
	//		m_iCountFOTimer++;//sec
	//		getModel()->getVIEWHANDLER()->drawFOTtime(m_iCountFOTimer);
	//		if(true==getModel()->getDATAHANDLER()->checkFOTvalidMeasurementData())
	//		{
	//			KillTimer(FOT_HFO_TIMER);
	//			PostMessage(WM_FOT_COLLECT_DATA);
	//		}
	//	}
	//	else//m_iCountFOTimer==15 sec  without valid data!!!!
	//	{
	//		KillTimer(FOT_HFO_TIMER);
	//		PostMessage(WM_RETRY_FOT_HFOSTEP);
	//	}
	//}
	else if(nIDEvent==CHECKCOM_TIMER)
	{
		PostMessage(WM_CHECK_SERIAL);
	}
	/*else if(nIDEvent==FLOWSENSOR_DELAY_TIMER)
	{
		PostMessage(WM_FLOWSENSOR_DELAY_TIMER);
		KillTimer(FLOWSENSOR_DELAY_TIMER);
	}*/
	else if(nIDEvent==PMIN_ALARMDELAY_TIMER)
	{
		PostMessage(WM_SetAlarm_PminLow);
		//getModel()->getALARMHANDLER()->setAlarm(AL_PatAl_PEEPminLow);
		KillTimer(PMIN_ALARMDELAY_TIMER);
	}
	else if(nIDEvent==AUTOLIMITTIMER)
	{
		KillTimer(AUTOLIMITTIMER);
		PostMessage(WM_AUTOLIMITTIMER);
		
	}
	else if(nIDEvent==HOMETIMER)
	{
		KillTimer(HOMETIMER);
		PostMessage(WM_HOMETIMER);
	}
	else if(nIDEvent==O2FLUSHTIMER)
	{
		if(m_iO2FlushCount<=0)
		{
			KillTimer(O2FLUSHTIMER);
		}
		PostMessage(WM_O2FLUSHTIMER);

	}
	else if(nIDEvent==COUNTSTOPBREATHTIMER)
	{
		if(m_iCountDownStart<=1)
		{
			KillTimer(COUNTSTOPBREATHTIMER);
		}
		PostMessage(WM_COUNTERSTOPBREATHTIMER);
	}
	else if(nIDEvent==OFFTIMER)
	{
		PostMessage(WM_OFFTIMER);
	}
	else if(nIDEvent==STARTSTOPTIMER)
	{
		PostMessage(WM_STARTSTOPTIMER);
	}
	else if(nIDEvent==ACTIVEALARMTIMER)
	{
		getModel()->setActiveAlarmDelay(false);
		KillTimer(ACTIVEALARMTIMER);
		theApp.getLog()->WriteLine(L"#ACTIVEALARMTIMER [off]");
	}
	else if(nIDEvent==ALARMSILENTTIMER)
	{
		//KillTimer(ALARMSILENTTIMER);
		PostMessage(WM_ALARMSILENTTIMER);
	}
	/*else if(nIDEvent==ALARMSOUND)
	{
		KillTimer(ALARMSOUND);
		PostMessage(WM_ALARMSOUND);
	}*/
	else if(nIDEvent==MANBREATHTIMER)
	{
		KillTimer(MANBREATHTIMER);
		PostMessage(WM_TRIGGER_MANUALBREATH_STOP);
		//getModel()->getSERIAL()->Send_STOP_MAN_BREATH();
	}
	else if(nIDEvent==CO2PUMPTIMER)
	{
		//m_ullRemainCO2PumpTime-=1000;

		getModel()->getDATAHANDLER()->setRemainCO2PumpTime(getModel()->getDATAHANDLER()->getRemainCO2PumpTime()-1000);
		PostMessage(WM_CO2PUMPTIMER);
	}
	else if(nIDEvent==AUTOSCREENLOCKTIMER)
	{
		KillTimer(AUTOSCREENLOCKTIMER);
		if(getModel()->getCONFIG()->IsAutoScreenlockActive())
		{
			PostMessage(WM_BN_SCREENLOCK);
		}
	}
	else if(nIDEvent==DISABLE_PATALARM_O2_TIMER)
	{
		KillTimer(DISABLE_PATALARM_O2_TIMER);
		getModel()->EnablePatAlarmO2dependend();
	}
	else if(nIDEvent==UDPDOWNUPTIMER)
	{
		KillTimer(UDPDOWNUPTIMER);
		PostMessage(WM_BN_SCREENLOCK);
	}
	else if(nIDEvent==OXYCAL_HOURGLASS_TIMER)
	{
		PostMessage(WM_OXYCAL_HOURGLASS_TIMER);
	}


	CFrameWnd::OnTimer(nIDEvent);
}



//void CALLBACK CMainFrame::mmTimerProc(UINT id,UINT msg,DWORD dwUser,DWORD dw1,DWORD dw2)
//{
//
//	//getModel()->NotifyLD(iTestCount);
//
//	if(iTestCount>=449)
//		iTestCount = 1;
//	else
//		iTestCount++;
//}

//bool CMainFrame::StartTimer(UINT period, bool oneShot, UINT resolution)
//{
//	bool        res = false;
//	MMRESULT    result;
//
//	TIMECAPS    tc;
//
//	if (TIMERR_NOERROR == timeGetDevCaps(&tc, sizeof(TIMECAPS)))
//	{
//		m_timerRes = min(max(tc.wPeriodMin, resolution), tc.wPeriodMax);
//		timeBeginPeriod(m_timerRes);
//	}
//	else 
//	{
//		return false;
//	}    
//
//	result = timeSetEvent(
//		period,
//		m_timerRes,                                                 // (LPTIMECALLBACK)hEvent
//		mmTimerProc,
//		0,                                                          // (DWORD)this,
//		(oneShot ? TIME_ONESHOT : TIME_PERIODIC)                    // TIME_KILL_SYNCHRONOUS
//		);                                                          // CALLBACK_EVENT_SET
//
//	if (NULL != result)
//	{
//		m_timerId = (UINT)result;
//		res = true;
//	}
//
//	return res;
//}
//bool CMainFrame::StopTimer(bool bEndTime)
//{
//	MMRESULT    result;
//
//	result = timeKillEvent(m_timerId);
//	if (TIMERR_NOERROR == result)
//	{
//		m_timerId = 0;
//		if (bEndTime )
//		{
//			for (volatile int i=0; i<10; i++)
//			{
//				Sleep(10);                          //TIME_KILL_SYNCHRONOUS
//			}
//		}
//	}
//
//
//	if (0 != m_timerRes)
//	{
//		timeEndPeriod(m_timerRes);
//		m_timerRes = 0;
//	}
//
//	return TIMERR_NOERROR == result;
//}

/**********************************************************************************************//**
 * Executes the erase bkgnd action
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(rc);

	CBrush cbrBack(BACKGND);
	pDC->FillRect(&rc, &cbrBack);
	//cbrBack.DeleteObject();//rkuNEWFIX

	return TRUE;
}

/**********************************************************************************************//**
 * Translates all messages before they are processed by the main message loop
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pMsg	If non-null, the message.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_CURRENT_VIEWID:
		{
			/*CWnd* pFocWnd=CWnd::GetFocus();
			if(pFocWnd!=0)
			{
				int iID=pFocWnd->GetDlgCtrlID();

				if(iID==0)
				{
					getModel()->getVIEWHANDLER()->SetFocusToPrimaryView();
				}
			}*/
			getModel()->getVIEWHANDLER()->CheckCurrentView(pMsg->wParam);

		}
		break;
	case WM_FOCUS_VKUP:
		{
			getModel()->getVIEWHANDLER()->SetNextFocus();

		}
		break;
	case WM_FOCUS_VKDOWN:
		{
			getModel()->getVIEWHANDLER()->SetPrevFocus();

		}
		break;
	case WM_SETFOCUS_PIMARYVIEW:
		{
			getModel()->getVIEWHANDLER()->SetFocusToPrimaryView();

		}
		break;
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
		{
			if(getModel()->getCONFIG()->IsAutoScreenlockActive())
			{
				PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
			}
		}
		break;
	case WM_KEYDOWN:
		if(!m_bInitialized)
			return 0;

		switch(pMsg->wParam) 
		{ 
		case MATRIX_ALARMSILENT:
			{
				
				if(theApp.IsScreenlock()==false)
				{
					if(getModel()->getCONFIG()->IsAutoScreenlockActive())
					{
						PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
					}
				}

				if(m_bMatrxDwn_Home==true && m_bSysSilent==false)
				{
					if(m_dwSystemsilent==0)
						m_dwSystemsilent=GetTickCount();

					if(isSafeTickCountDelayExpired(m_dwSystemsilent, 3000))
					{
						if(getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
						{
							getModel()->getALARMHANDLER()->setSystemSilent();
						}
						else if(getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_SYSTEM_SILENT)
						{
							getModel()->getALARMHANDLER()->setAutoSilent(true,true,false);
						}
						m_bSysSilent=true;
					}
				}
				
				if(!isSafeTickCountDelayExpired(m_dwLastMatrixdwn, MATRIXDELAY))
				{
					return 1;
				}
				
				if(getModel() && !m_bMatrxDwn_Alarmsilent)
				{
					theApp.getLog()->WriteLine(_T("$M1$"));
					m_bMatrxDwn_Alarmsilent=true;
					if(getModel()->getVIEWHANDLER()->getViewState()!=VS_STARTUP)
					{
						PostMessage(WM_BN_MATRIX_ALARM_SILENT);
					}
					
				}
				return 1;
			}
			break;
		case MATRIX_GRAPH:
			{
				if(theApp.IsScreenlock()==false)
				{
					if(getModel()->getCONFIG()->IsAutoScreenlockActive())
					{
						PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
					}
				}

				if(!isSafeTickCountDelayExpired(m_dwLastMatrixdwn, MATRIXDELAY))
				{
					return 1;
				}

				if(getModel() && !m_bMatrxDwn_Graph)
				{
					theApp.getLog()->WriteLine(_T("$M2$"));
					m_bMatrxDwn_Graph=true;
					if(getModel()->getVIEWHANDLER()->getViewState()!=VS_STARTUP)
					{
						PostMessage(WM_BN_MATRIX_GRAPHS);
					}
					
				}
				return 1;
			}
			break;
		case MATRIX_ONOFF:
			{
				if(theApp.IsScreenlock()==false)
				{
					if(getModel()->getCONFIG()->IsAutoScreenlockActive())
					{
						PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
					}
				}

				if(!isSafeTickCountDelayExpired(m_dwLastMatrixdwn, MATRIXDELAY))
				{
					return 1;
				}

				if(getModel() && !m_bMatrxDwn_OFF)
				{
					theApp.getLog()->WriteLine(_T("$M3$"));
					m_bMatrxDwn_OFF=true;
					
					PostMessage(WM_BN_MATRIX_ONOFF_DOWN);
				}
				return 1;
			}
			break;
		case MATRIX_STARTSTOP:
			{
				if(theApp.IsScreenlock()==false)
				{
					if(getModel()->getCONFIG()->IsAutoScreenlockActive())
					{
						PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
					}
				}

				if(!isSafeTickCountDelayExpired(m_dwLastMatrixdwn, MATRIXDELAY))
				{
					return 1;
				}

				if(getModel() && !m_bMatrxDwn_STARTSTOP)
				{
					theApp.getLog()->WriteLine(_T("$M4$"));
					if(getModel()->getVIEWHANDLER()->getViewState()!=VS_STARTUP)
					{
						m_bMatrxDwn_STARTSTOP=true;
						
						PostMessage(WM_BN_MATRIX_STARTSTOP_DOWN);
					}
					
				}
				return 1;
			}
			break;
		case MATRIX_MANBREATH:
			{
				if(theApp.IsScreenlock()==false)
				{
					if(getModel()->getCONFIG()->IsAutoScreenlockActive())
					{
						PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
					}
				}

				if(!isSafeTickCountDelayExpired(m_dwLastMatrixdwn, MATRIXDELAY))
				{
					return 1;
				}

				if(getModel() && !m_bMatrxDwn_MANBREATH)
				{
					if(		getModel()->getCONFIG()->IsHFOManBreathEnabled()==false 
						&&	getModel()->getVMODEHANDLER()->activeModeIsHFO())
					{
						m_bMatrxDwn_MANBREATH=true;
						m_bMatrxDwn_MANBREATHdisabled=true;
						

						PostMessage(WM_CONTROL_MANBREATH_TXTDISABLED);
					}
					else if(		getModel()->getCONFIG()->IsHFOManBreathEnabled()==true 
						&&	getModel()->getVMODEHANDLER()->activeModeIsHFO()
						&&	getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
						&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
					{
						if(getModel()->GetVentRunState()==VENT_RUNNING)
						{
							m_bMatrxDwn_MANBREATH=true;
							m_bMatrxDwn_MANBREATHdisabled=true;

							PostMessage(WM_CONTROL_MANBREATH_LUNGREC);
						}
					}
					else
					{
						if(getModel()->getVIEWHANDLER()->getViewState()!=VS_STARTUP)
						{
							eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

							if(eActiveVentMode==VM_THERAPIE)
							{
								PostMessage(WM_EV_TIMETEXT_NOTAVAILABLE);
							}
							else
							{
								if(getModel()->GetVentRunState()==VENT_RUNNING)
								{
									m_bMatrxDwn_MANBREATHdisabled=false;
									m_bMatrxDwn_MANBREATH=true;

									getModel()->ResetExhalValvCalMode();

									getModel()->activateMANBREATH();

									int iTime=(getModel()->getCONFIG()->GetManBreathTime())*1000;
									SetTimer(MANBREATHTIMER,iTime,NULL);

									/*if(getModel()->getDATAHANDLER()->isProcPressureCal60enabled())
									{
										getModel()->getDATAHANDLER()->SetCurrentPINSPPara(getModel()->getDATAHANDLER()->getProcPressureCal60());
									}*/
									getModel()->getSERIAL()->Send_START_MAN_BREATH();
									PostMessage(WM_CONTROL_START_MANBREATH);
								}
							}
						}
					}
					
				}
				return 1;
			}
			break;
		case MATRIX_ALIMITS:
			{
				if(theApp.IsScreenlock()==false)
				{
					if(getModel()->getCONFIG()->IsAutoScreenlockActive())
					{
						PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
					}
				}

				if(!isSafeTickCountDelayExpired(m_dwLastMatrixdwn, MATRIXDELAY))
				{
					return 1;
				}

				if(getModel() && !m_bMatrxDwn_ALimits)
				{
					theApp.getLog()->WriteLine(_T("$M6$"));
					m_bMatrxDwn_ALimits=true;
					if(getModel()->getVIEWHANDLER()->getViewState()!=VS_STARTUP)
					{
						PostMessage(WM_BN_MATRIX_ALARMLIMITS);
					}
					
				}
				return 1;
			}
			break;
		case MATRIX_O2FLUSH:
			{
				
				if(theApp.IsScreenlock()==false)
				{
					if(getModel()->getCONFIG()->IsAutoScreenlockActive())
					{
						PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
					}
				}

				if(!isSafeTickCountDelayExpired(m_dwLastMatrixdwn, MATRIXDELAY))
				{
					return 1;
				}

				if(getModel() && !m_bMatrxDwn_O2Flush)
				{
					theApp.getLog()->WriteLine(_T("$M7$"));
					if(		(getModel()->getVIEWHANDLER()->getViewState()!=VS_STARTUP)
						&&	(getModel()->getVIEWHANDLER()->getViewState()!=VS_SERVICE))
					{
						m_bMatrxDwn_O2Flush=true;
						getModel()->ResetExhalValvCalMode();

						PostMessage(WM_O2FLUSH);
					}
				}
				return 1;
			}
			break;
		case MATRIX_HOME:
			{
				if(theApp.IsScreenlock()==false)
				{
					if(getModel()->getCONFIG()->IsAutoScreenlockActive())
					{
						PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
					}
				}

				if(m_bMatrxDwn_Alarmsilent==true && m_bSysSilent==false)
				{
					if(m_dwSystemsilent==0)
						m_dwSystemsilent=GetTickCount();

					if(isSafeTickCountDelayExpired(m_dwSystemsilent, 3000))
					{
						if(getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
							getModel()->getALARMHANDLER()->setSystemSilent();
						else if(getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_SYSTEM_SILENT)
						{
							getModel()->getALARMHANDLER()->setAutoSilent(true,true,false);
						}

						m_bSysSilent=true;
					}
				}

				if(!isSafeTickCountDelayExpired(m_dwLastMatrixdwn, MATRIXDELAY))
				{
					return 1;
				}

				if(getModel() && !m_bMatrxDwn_Home)
				{
					theApp.getLog()->WriteLine(_T("$M8$"));
					m_bMatrxDwn_Home=true;
					
					if(getModel()->getVIEWHANDLER()->getViewState()!=VS_STARTUP)
					{
						PostMessage(WM_BN_MATRIX_HOME);
					}
					
				}
				return 1;
			}
			break;
		case MATRIX_MENU:
			{
				if(theApp.IsScreenlock()==false)
				{
					if(getModel()->getCONFIG()->IsAutoScreenlockActive())
					{
						PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
					}
				}

				if(!isSafeTickCountDelayExpired(m_dwLastMatrixdwn, MATRIXDELAY))
				{
					return 1;
				}

 				if(getModel() && !m_bMatrxDwn_Menu)
				{
					theApp.getLog()->WriteLine(_T("$M9$"));
					m_bMatrxDwn_Menu=true;
					if(getModel()->getVIEWHANDLER()->getViewState()!=VS_STARTUP)
					{
						PostMessage(WM_BN_MATRIX_MENU);
					}
				}
				return 1;
			}
			break;
		case MATRIX_NEBULISER:
			{
				if(theApp.IsScreenlock()==false)
				{
					if(getModel()->getCONFIG()->IsAutoScreenlockActive())
					{
						PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
					}
				}

				if(!isSafeTickCountDelayExpired(m_dwLastMatrixdwn, MATRIXDELAY))
				{
					return 1;
				}

				if(!m_bNebulizerDwn)
				{
					theApp.getLog()->WriteLine(_T("$M10$"));
					m_bNebulizerDwn=true;
					
					PostMessage(WM_BN_MATRIX_NEBULIZER);
				}
				return 1;
			}
			break;
		case VK_SPACE: 
			{
				SetTimer(UDPDOWNUPTIMER, 3000, NULL);

				if(getModel() && getModel()->getVIEWHANDLER()->getViewState()==VS_STARTUP)
				{
					return 1;
				}

				if(getModel()->getCONFIG()->IsAutoScreenlockActive())
				{
					PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
				}

				CWnd* pFocWnd=CWnd::GetFocus();
				if(pFocWnd!=0)
				{
					int iID=pFocWnd->GetDlgCtrlID();

					if(iID==0)
					{
						getModel()->getVIEWHANDLER()->SetFocusToPrimaryView();
					}
				}
			}
			break; 
		case VK_UP: 
			{
				if(getModel() && getModel()->getVIEWHANDLER()->getViewState()==VS_STARTUP)
				{
					return 1;
				}

				if(		getModel()->getCONFIG()->IsAutoScreenlockActive()
					&&	isSafeTickCountDelayExpired(m_dwLastAutoScreenTimer, 1000))
				{
					PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
				}

				CWnd* pFocWnd=CWnd::GetFocus();
				if(pFocWnd!=0)
				{
					int iID=pFocWnd->GetDlgCtrlID();

					if(iID==0)
					{
						getModel()->getVIEWHANDLER()->SetViewVKUP();
					}
				}
			}
			break;
		case VK_DOWN: 
			{
				if(getModel() && getModel()->getVIEWHANDLER()->getViewState()==VS_STARTUP)
				{
					return 1;
				}

				if(		getModel()->getCONFIG()->IsAutoScreenlockActive()
					&&	isSafeTickCountDelayExpired(m_dwLastAutoScreenTimer, 1000))
				{
					PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
				}

				int iID=0;

				CWnd* pFocWnd=CWnd::GetFocus();
				if(pFocWnd==0)
				{
				}
				else
				{
					iID=pFocWnd->GetDlgCtrlID();

					if(iID==0)
					{
						getModel()->getVIEWHANDLER()->SetViewVKDOWN();
					}
				}
			}
			break;
		} 
		break; 
	case WM_KEYUP: 
		switch(pMsg->wParam) 
		{ 
		case VK_SPACE: 
			{
				KillTimer(UDPDOWNUPTIMER);
			}
			break;
		case MATRIX_ALARMSILENT:
			{
				m_dwSystemsilent=0;
				if(m_bMatrxDwn_Alarmsilent)
				{
					m_bMatrxDwn_Alarmsilent=false;
					m_bSysSilent=false;

					m_dwLastMatrixdwn=GetTickCount();
				}
				return 1;
			}
			break;
		case MATRIX_GRAPH:
			{
				if(m_bMatrxDwn_Graph)
				{
					m_bMatrxDwn_Graph=false;

					m_dwLastMatrixdwn=GetTickCount();
				}
				return 1;
			}
			break;
		case MATRIX_ALIMITS:
			{
				if(m_bMatrxDwn_ALimits)
				{
					m_bMatrxDwn_ALimits=false;

					m_dwLastMatrixdwn=GetTickCount();
				}
				return 1;
			}
			break;
		case MATRIX_HOME:
			{
				m_dwFactorydefaults=0;
				m_dwSystemsilent=0;
				if(m_bMatrxDwn_Home)
				{
					m_bMatrxDwn_Home=false;
					m_bSysSilent=false;

					m_dwLastMatrixdwn=GetTickCount();
				}
				return 1;
			}
			break;
		case MATRIX_MENU:
			{
				if(m_bMatrxDwn_Menu)
				{
					m_bMatrxDwn_Menu=false;

					m_dwLastMatrixdwn=GetTickCount();
				}
				return 1;
			}
			break;
		case MATRIX_NEBULISER:
			{
				if(m_bNebulizerDwn)
				{
					m_bNebulizerDwn=false;

					m_dwLastMatrixdwn=GetTickCount();
				}
				return 1;
			}
			break;
		case MATRIX_O2FLUSH:
			{
				if(m_bMatrxDwn_O2Flush)
				{
					m_bMatrxDwn_O2Flush=false;

					m_dwLastMatrixdwn=GetTickCount();
				}
			}
			break;
		case MATRIX_ONOFF:
			{
				
				if(getModel() && m_bMatrxDwn_OFF)
				{
					if(getModel()->getVIEWHANDLER()->getViewState()!=VS_STARTUP)
					{
						m_bMatrxDwn_OFF=false;
						
						PostMessage(WM_BN_MATRIX_ONOFF_UP);
					}

					m_dwLastMatrixdwn=GetTickCount();
					
				}
				return 1;
			}
			break;
		case MATRIX_STARTSTOP:
			{
				if(getModel() && m_bMatrxDwn_STARTSTOP)
				{
					if(getModel()->getVIEWHANDLER()->getViewState()!=VS_STARTUP)
					{
						m_bMatrxDwn_STARTSTOP=false;
						
						PostMessage(WM_BN_MATRIX_STARTSTOP_UP);
					}

					m_dwLastMatrixdwn=GetTickCount();
					
				}
				return 1;
			}
			break;
		case MATRIX_MANBREATH:
			{
				if(getModel() && m_bMatrxDwn_MANBREATH)
				{
					m_bMatrxDwn_MANBREATH=false;

					eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

					if(eActiveVentMode==VM_THERAPIE)
					{

					}
					else
					{
						if(		(getModel()->getVIEWHANDLER()->getViewState()!=VS_STARTUP)
							&&	m_bMatrxDwn_MANBREATHdisabled==false)
						{
							getModel()->ResetExhalValvCalMode();

							PostMessage(WM_TRIGGER_MANUALBREATH_STOP);
						}
					}
					m_dwLastMatrixdwn=GetTickCount();
				}
				return 1;
			}
			break;
		case VK_ESCAPE: 
			break; 
		} 
		break; 
	default:
		{

		}
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

/**********************************************************************************************//**
 * Calculates the single automatic limit
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	para	The para.
 **************************************************************************************************/

void CMainFrame::CalculateSingleAutoLimit(eAlarmLimitPara para)
{
	switch(para)
	{
	case AP_MVMAX:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_MVmaxLimit()!=AL_OFF)
			{
				getModel()->getALARMHANDLER()->setAlimitState_MVmaxLimit(AL_CALC);
				KillTimer(AUTOLIMITTIMER);
				SetTimer(AUTOLIMITTIMER,AUTOLIMITMINUTE,NULL);

				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetCurMode();
				else
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetPrevMode();

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_MVmax->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_MVmax);
				}

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);

				getModel()->SetAlimitStateCalculating(true);
			}
		}
		break;
	case AP_MVMIN:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_MVminLimit()!=AL_OFF)
			{
				getModel()->getALARMHANDLER()->setAlimitState_MVminLimit(AL_CALC);
				KillTimer(AUTOLIMITTIMER);
				SetTimer(AUTOLIMITTIMER,AUTOLIMITMINUTE,NULL);

				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetCurMode();
				else
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetPrevMode();

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_MVmin->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_MVmin);
				}

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);

				getModel()->SetAlimitStateCalculating(true);
			}
		}
		break;
	case AP_LEAK:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_LeakmaxLimit()!=AL_OFF)
			{
				getModel()->getALARMHANDLER()->setAlimitState_LeakmaxLimit(AL_CALC);
				KillTimer(AUTOLIMITTIMER);
				SetTimer(AUTOLIMITTIMER,AUTOLIMITMINUTE,NULL);

				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetCurMode();
				else
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetPrevMode();

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_Leakmax->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_Leakmax);
				}

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);

				getModel()->SetAlimitStateCalculating(true);
			}
		}
		break;
	case AP_BPM:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_BPMmaxLimit()!=AL_OFF)
			{
				getModel()->getALARMHANDLER()->setAlimitState_BPMmaxLimit(AL_CALC);
				KillTimer(AUTOLIMITTIMER);
				SetTimer(AUTOLIMITTIMER,AUTOLIMITMINUTE,NULL);

				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetCurMode();
				else
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetPrevMode();

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_BPMmax->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_BPMmax);
				}

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);

				getModel()->SetAlimitStateCalculating(true);
			}
		}
		break;
	case AP_APNOE:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit()!=AL_OFF)
			{
				getModel()->getALARMHANDLER()->setAlimitState_ApnoeLimit(AL_CALC);
				KillTimer(AUTOLIMITTIMER);
				PostMessage(WM_AUTOLIMITTIMER);

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_Apnoe->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_Apnoe);
				}

				PostMessage(WM_DATA_CALCULATE_ALARMLIMITS);
			}
		}
		break;
	case AP_PIPMAX:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()!=AL_OFF)
			{
				{
					getModel()->getALARMHANDLER()->setAlimitState_PIPmaxLimit(AL_AUTO);
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();

					if(getModel()->getALARMHANDLER()->ALARM_PatAl_PIPmax->getAlarmState()!=AS_NONE)
					{
						getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_PIPmax);
					}
					PostMessage(WM_DATA_ALARMLIMITS_CHANGED);
				}
			}
		}
		break;
	case AP_PIPMIN:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()!=AL_OFF)
			{
				{
					getModel()->getALARMHANDLER()->setAlimitState_PIPminLimit(AL_ON);
					getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();

					if(getModel()->getALARMHANDLER()->ALARM_PatAl_PIPmin->getAlarmState()!=AS_NONE)
					{
						getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_PIPmin);
					}
					PostMessage(WM_DATA_ALARMLIMITS_CHANGED);
				}
			}
		}
		break;
	case AP_PEEP:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()!=AL_OFF)
			{
				getModel()->getALARMHANDLER()->setAlimitState_PEEPminLimit(AL_AUTO);
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_PEEPminHigh->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_PEEPminHigh);
				}

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);
			}
		}
		break;
	case AP_MAPMAX:
		{
			if(		getModel()->getALARMHANDLER()->getAlimitState_MAPmaxLimit()!=AL_OFF
				&&	getModel()->getVMODEHANDLER()->activeModeIsHFO())
			{
				if(getModel()->getALARMHANDLER()->getAlimitState_MAPmaxLimit()!=AL_OFF)
				{
					{
						getModel()->getALARMHANDLER()->setAlimitState_MAPmaxLimit(AL_AUTO);
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitMAPmax();

						if(getModel()->getALARMHANDLER()->ALARM_PatAl_MAPmax->getAlarmState()!=AS_NONE)
						{
							getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_MAPmax);
						}
						PostMessage(WM_DATA_ALARMLIMITS_CHANGED);
					}
				}
			}
		}
		break;
	case AP_MAPMIN:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_MAPminLimit()!=AL_OFF
				&&	getModel()->getVMODEHANDLER()->activeModeIsHFO())
			{
				if(getModel()->getALARMHANDLER()->getAlimitState_MAPminLimit()!=AL_OFF)
				{
					{
						getModel()->getALARMHANDLER()->setAlimitState_MAPminLimit(AL_AUTO);
						getModel()->getDATAHANDLER()->calculateAutoAlarmlimitMAPmin();

						if(getModel()->getALARMHANDLER()->ALARM_PatAl_MAPmin->getAlarmState()!=AS_NONE)
						{
							getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_MAPmin);
						}
						PostMessage(WM_DATA_ALARMLIMITS_CHANGED);
					}
				}
			}
		}
		break;
	case AP_DCO2MAX:
		{
			if(		getModel()->getALARMHANDLER()->getAlimitState_DCO2maxLimit()!=AL_OFF
				&&	getModel()->getVMODEHANDLER()->activeModeIsHFO())
			{
				getModel()->getALARMHANDLER()->setAlimitState_DCO2maxLimit(AL_CALC);
				KillTimer(AUTOLIMITTIMER);
				SetTimer(AUTOLIMITTIMER,AUTOLIMITMINUTE,NULL);

				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetCurMode();
				else
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetPrevMode();

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_DCO2max->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_DCO2max);
				}

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);

				getModel()->SetAlimitStateCalculating(true);
			}
		}
		break;
	case AP_DCO2MIN:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_DCO2minLimit()!=AL_OFF
				&&	getModel()->getVMODEHANDLER()->activeModeIsHFO())
			{
				getModel()->getALARMHANDLER()->setAlimitState_DCO2minLimit(AL_CALC);
				KillTimer(AUTOLIMITTIMER);
				SetTimer(AUTOLIMITTIMER,AUTOLIMITMINUTE,NULL);

				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetCurMode();
				else
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetPrevMode();

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_DCO2min->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_DCO2min);
				}

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);

				getModel()->SetAlimitStateCalculating(true);
			}
		}
		break;
	case AP_ETCO2MAX:
		{
			if(		getModel()->getALARMHANDLER()->getAlimitState_ETCO2maxLimit()!=AL_OFF
				&&	getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE)
			{
				getModel()->getALARMHANDLER()->setAlimitState_ETCO2maxLimit(AL_CALC);
				KillTimer(AUTOLIMITTIMER);
				SetTimer(AUTOLIMITTIMER,AUTOLIMITMINUTE,NULL);

				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetCurMode();
				else
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetPrevMode();

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_ETCO2max->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_ETCO2max);
				}

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);

				getModel()->SetAlimitStateCalculating(true);
			}
		}
		break;
	case AP_ETCO2MIN:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_ETCO2minLimit()!=AL_OFF
				&&	getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE)
			{
				getModel()->getALARMHANDLER()->setAlimitState_ETCO2minLimit(AL_CALC);
				KillTimer(AUTOLIMITTIMER);
				SetTimer(AUTOLIMITTIMER,AUTOLIMITMINUTE,NULL);

				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetCurMode();
				else
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetPrevMode();

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_ETCO2min->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_ETCO2min);
				}

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);

				getModel()->SetAlimitStateCalculating(true);
			}
		}
		break;
	case AP_FICO2MAX:
		{
			if(		getModel()->getALARMHANDLER()->getAlimitState_FICO2maxLimit()!=AL_OFF
				&&	getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE)
			{
				getModel()->getALARMHANDLER()->setAlimitState_FICO2maxLimit(AL_CALC);
				KillTimer(AUTOLIMITTIMER);
				SetTimer(AUTOLIMITTIMER,AUTOLIMITMINUTE,NULL);

				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetCurMode();
				else
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetPrevMode();

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_FICO2max->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_FICO2max);
				}

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);

				getModel()->SetAlimitStateCalculating(true);
			}
		}
		break;
	case AP_FICO2MIN:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_FICO2minLimit()!=AL_OFF
				&&	getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE)
			{
				getModel()->getALARMHANDLER()->setAlimitState_FICO2minLimit(AL_CALC);
				KillTimer(AUTOLIMITTIMER);
				SetTimer(AUTOLIMITTIMER,AUTOLIMITMINUTE,NULL);

				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetCurMode();
				else
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetPrevMode();

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_FICO2min->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_FICO2min);
				}

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);

				getModel()->SetAlimitStateCalculating(true);
			}
		}
		break;
	case AP_SPO2MAX:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_SPO2maxLimit()!=AL_OFF
				&&	getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
			{
				getModel()->getALARMHANDLER()->setAlimitState_SPO2maxLimit(AL_CALC);
				KillTimer(AUTOLIMITTIMER);
				SetTimer(AUTOLIMITTIMER,AUTOLIMITMINUTE,NULL);

				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetCurMode();
				else
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetPrevMode();

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2max->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_SPO2max);
				}

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);

				getModel()->SetAlimitStateCalculating(true);
			}
		}
		break;
	case AP_SPO2MIN:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_SPO2minLimit()!=AL_OFF
				&&	getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
			{
				getModel()->getALARMHANDLER()->setAlimitState_SPO2minLimit(AL_CALC);
				KillTimer(AUTOLIMITTIMER);
				SetTimer(AUTOLIMITTIMER,AUTOLIMITMINUTE,NULL);

				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetCurMode();
				else
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetPrevMode();

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2min->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_SPO2min);
				}

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);

				getModel()->SetAlimitStateCalculating(true);
			}
		}
		break;
	case AP_PULSERATEMAX:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_PulseRatemaxLimit()!=AL_OFF
				&&	getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
			{
				getModel()->getALARMHANDLER()->setAlimitState_PulseRatemaxLimit(AL_CALC);
				KillTimer(AUTOLIMITTIMER);
				SetTimer(AUTOLIMITTIMER,AUTOLIMITMINUTE,NULL);

				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetCurMode();
				else
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetPrevMode();

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_PulseRatemax->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_PulseRatemax);
				}

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);

				getModel()->SetAlimitStateCalculating(true);
			}
		}
		break;
	case AP_PULSERATEMIN:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_PulseRateminLimit()!=AL_OFF
				&&	getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
			{
				getModel()->getALARMHANDLER()->setAlimitState_PulseRateminLimit(AL_CALC);
				KillTimer(AUTOLIMITTIMER);
				SetTimer(AUTOLIMITTIMER,AUTOLIMITMINUTE,NULL);

				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetCurMode();
				else
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetPrevMode();

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_PulseRatemin->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_PulseRatemin);
				}

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);

				getModel()->SetAlimitStateCalculating(true);
			}
		}
		break;
	case AP_SPO2_PIMIN:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_SPO2_PIminLimit()!=AL_OFF
				&&	getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
			{
				getModel()->getALARMHANDLER()->setAlimitState_SPO2_PIminLimit(AL_CALC);
				KillTimer(AUTOLIMITTIMER);
				SetTimer(AUTOLIMITTIMER,AUTOLIMITMINUTE,NULL);

				if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetCurMode();
				else
					m_eVentModeCalcStarted=getModel()->getCONFIG()->GetPrevMode();

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2_PImin->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_SPO2_PImin);
				}

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);

				getModel()->SetAlimitStateCalculating(true);
			}
		}
		break;
	case AP_SPO2_SIQMIN:
		{
			if(getModel()->getALARMHANDLER()->getAlimitState_SPO2_SIQminLimit()!=AL_OFF
				&&	getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
			{
				getModel()->getALARMHANDLER()->setAlimitState_SPO2_SIQminLimit(AL_ON);
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitSPO2_SIQmin();

				PostMessage(WM_DATA_ALARMLIMITS_CHANGED);
			}
		}
		break;
	default:
		break;
	}

	

	
	
}

//void CMainFrame::StartThreadWatchdogThread( void )
//{
//	m_bDoThreadWatchdogThread=true;
//
//	if(m_pcwtThreadWatchdogThread!=NULL)
//	{
//		delete m_pcwtThreadWatchdogThread;
//		m_pcwtThreadWatchdogThread=NULL;
//
//		if(m_hThreadThreadWatchdog!=INVALID_HANDLE_VALUE)
//		{
//			CloseHandle(m_hThreadThreadWatchdog);
//			m_hThreadThreadWatchdog=INVALID_HANDLE_VALUE;
//		}
//	}
//
//	m_pcwtThreadWatchdogThread=AfxBeginThread(CThreadWatchdogThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
//	m_hThreadThreadWatchdog=m_pcwtThreadWatchdogThread->m_hThread;
//	m_pcwtThreadWatchdogThread->m_bAutoDelete = FALSE; 
//	m_pcwtThreadWatchdogThread->ResumeThread();
//}
//void CMainFrame::StopThreadWatchdogThread( void )
//{
//	if(m_bDoThreadWatchdogThread)
//	{
//		m_bDoThreadWatchdogThread=false;
//		eventThreadWatchdog.SetEvent();
//
//		if (WaitForSingleObject(m_pcwtThreadWatchdogThread->m_hThread,1000) == WAIT_TIMEOUT)
//		{
//			theApp.getLog()->WriteLine(_T("#THR:018a"));
//			if(!TerminateThread(m_pcwtThreadWatchdogThread,0))
//			{
//				theApp.getLog()->WriteLine(_T("#THR:018b"));
//			}
//		}
//	}
//}
//static UINT CThreadWatchdogThread( LPVOID pc )
//{
//	((CMainFrame*)pc)->ThreadWatchdog();
//	return true;
//}
//
//DWORD CMainFrame::ThreadWatchdog(void) 
//{
//	CeSetThreadPriority(m_pcwtThreadWatchdogThread->m_hThread,CE_THREAD_PRIO_256_ABOVE_NORMAL);//PRICO04
//
//	DWORD WAIT=4000;
//	DWORD dwWait=0;
//	DWORD dwDiff = 0;
//	DWORD dwStart=0;
//	DWORD dwEnd=0;
//	bool bPrintError=true;
//
//
//	do
//	{
//		dwEnd=GetTickCount();
//
//		if(dwEnd>=dwStart)//rkuTICKCOUNTCHECK
//			dwDiff=dwEnd-dwStart;
//		else
//			dwDiff=0;
//
//		if(dwDiff<WAIT)
//		{
//			dwWait=WAIT-dwDiff;
//		}
//		else
//		{
//			dwWait=1;
//		}
//
//		DWORD dw = ::WaitForSingleObject(eventThreadWatchdog, dwWait);
//
//		dwStart = GetTickCount();
//
//		switch(dw)
//		{
//		case WAIT_OBJECT_0:
//			break;
//		default:
//			{
//				//EnterCriticalSection(&csThreadWatchdog);
//				//for(int i=0;i<MAXSIZE_THREADS;i++)
//				//{
//				//	if(faThreadWatchdog[i]!=0)
//				//	{
//				//		if(faThreadWatchdog[i]!=faPrevstateThreadWatchdog[i])
//				//		{
//				//			faPrevstateThreadWatchdog[i]=faThreadWatchdog[i];
//				//		}
//				//		else
//				//		{
//				//			//thread error
//				//			DEBUGMSG(TRUE, (TEXT("THREAD ERROR %d\r\n"),i));
//				//			CStringW sz=_T("");
//				//			sz.Format(_T("#HFO:0302 #%d"),i);
//				//			theApp.getLog()->WriteLine(sz);
//				//		}
//				//	}
//				//}
//				//LeaveCriticalSection(&csThreadWatchdog);
//			}
//			break;
//		}
//	}while(m_bDoThreadWatchdogThread);
//
//	theApp.getLog()->WriteLine(_T("#THR:1000"));
//
//	return 0;
//}

/**********************************************************************************************//**
 * Starts i2c watchdog thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::StartI2CWatchdogThread( void )
{
	m_bDoI2CWatchdogThread=true;
	//m_pcwtI2CWatchdogThread=AfxBeginThread(CI2CWatchdogThread,this);

	if(m_pcwtI2CWatchdogThread!=NULL)
	{
		delete m_pcwtI2CWatchdogThread;
		m_pcwtI2CWatchdogThread=NULL;

		if(m_hThreadI2CWatchdog!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadI2CWatchdog);
			m_hThreadI2CWatchdog=INVALID_HANDLE_VALUE;
		}
	}

	

	m_pcwtI2CWatchdogThread=AfxBeginThread(CI2CWatchdogThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadI2CWatchdog=m_pcwtI2CWatchdogThread->m_hThread;
	m_pcwtI2CWatchdogThread->m_bAutoDelete = FALSE; 
	m_pcwtI2CWatchdogThread->ResumeThread();
}

/**********************************************************************************************//**
 * Stops i2c watchdog thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::StopI2CWatchdogThread( void )
{
	if(m_bDoI2CWatchdogThread)
	{
		m_bDoI2CWatchdogThread=false;
		eventI2CWatchdog.SetEvent();

		if (WaitForSingleObject(m_pcwtI2CWatchdogThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:018a"));
			if(!TerminateThread(m_pcwtI2CWatchdogThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:018b"));
			}
		}
	}
}

/**********************************************************************************************//**
 * i2c watchdog thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

static UINT CI2CWatchdogThread( LPVOID pc )
{
	((CMainFrame*)pc)->SetI2CWatchdog();
	return TRUE;
}

/**********************************************************************************************//**
 * Sets i2c watchdog
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CMainFrame::SetI2CWatchdog(void) 
{
	//BOOL bTest=CeSetThreadPriority(m_pcwtI2CWatchdogThread->m_hThread,WATCHDOGPRIO);
	//BOOL bTest=CeSetThreadPriority(m_pcwtI2CWatchdogThread->m_hThread,CE_THREAD_PRIO_256_NORMAL);
	//CeSetThreadPriority(m_pcwtI2CWatchdogThread->m_hThread,THREAD_PRIORITY_TIME_CRITICAL);
	CeSetThreadPriority(m_pcwtI2CWatchdogThread->m_hThread,CE_THREAD_PRIO_256_ABOVE_NORMAL);//PRICO04

	//zwischen 60 und 70 Grenze
	DWORD WAIT=40;
	DWORD dwWait=0;
	DWORD dwDiff = 0;
	DWORD dwStart=0;
	DWORD dwEnd=0;
	bool bPrintError=true;
	
	bool bI2Cinitialized=false;
	EnterCriticalSection(&m_csI2Cinit);
	bI2Cinitialized=m_bI2Cinitialized;
	LeaveCriticalSection(&m_csI2Cinit);

	while(false==bI2Cinitialized && m_bDoI2CWatchdogThread)
	{
		Sleep(5);

		EnterCriticalSection(&m_csI2Cinit);
		bI2Cinitialized=m_bI2Cinitialized;
		LeaveCriticalSection(&m_csI2Cinit);
	}

	Sleep(200);

	/*while(false==getModel()->getI2Cinitialized() && m_bDoI2CWatchdogThread)
	{
		Sleep(5);
	}*/

	if(false==m_bDoI2CWatchdogThread)
		return 99;

	do
	{
		dwEnd=GetTickCount();

		if(dwEnd>=dwStart)
		{
			dwDiff=dwEnd-dwStart;
			/*if(dwDiff>1000)
			{
				DEBUGMSG(TRUE, (TEXT("I2CWatchdog %d\r\n"),dwDiff));
			}*/
		}
		else
			dwDiff=0;

		if(dwDiff<WAIT)
		{
			dwWait=WAIT-dwDiff;
		}
		else
		{
			dwWait=1;
		}

		DWORD dw = ::WaitForSingleObject(eventI2CWatchdog, dwWait);

		dwStart = GetTickCount();

		switch(dw)
		{
		case WAIT_OBJECT_0:
			break;
		default:
			{
				if(m_bDoI2CWatchdogThread && g_bMainWatchdogPending && !m_bWD0error)
				{
					getModel()->getI2C()->SetMCPwatchdog();
					m_bWDERRORI2C=false;
				}
				else if(m_bDoI2CWatchdogThread && g_bMainWatchdogFlag && !m_bWD0error)
				{
					/*dwDiff=ullStart-ullEnd;
					if(dwDiff>100)
					{
						DEBUGMSG(TRUE, (TEXT("I2CWatchdog %d\r\n"),dwDiff));
					}*/

					getModel()->getI2C()->SetMCPwatchdog();
					m_bWDERRORI2C=false;
				}
				else if(false==m_bWDERRORI2C)
				{
					m_bWDERRORI2C=true;
					theApp.getLog()->WriteLine(_T("#HFO:0221b"));
				}
			}
			break;
		}
	}while(m_bDoI2CWatchdogThread);

	//theApp.getLog()->WriteLine(_T("#THR:018"));

	return 0;
}

/**********************************************************************************************//**
 * Actions the given parameter 1
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	parameter1	If non-null, the first parameter.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool Action(void*)
{
	DWORD dwResult=WaitForSingleObject(g_hWatchDog,INFINITE);
	//DWORD dwResult=WaitForSingleObject(g_hWatchDog,WD_PERIODE/2);
	//if(dwResult&WAIT_OBJECT_0)
	//{
	//	// TODO:	
	//	/*CTlsRegistry reg(_T("HKCU\\Software\\FabianHFO\\WorkState"),true);
	//	reg.WriteDWORD(_T("Watchdog2"), 1);*/
	//	DWORD dwError=GetLastError();
	//	CloseHandle(g_hAction);
	//	return false;
	//}

	// Action before reboot/exit ********************************************
	// ...
	switch(dwResult)
	{
	case WAIT_OBJECT_0:
		{
			if(!g_bMainWatchdogPending)
			{
				theApp.getLog()->WriteLine(_T("#HFO:0222"));

				MEMORYSTATUS stat;
				GlobalMemoryStatus (&stat);
				DWORD dwMem=stat.dwAvailPhys/1024;

				CString szTemp=_T("");
				szTemp.Format(_T("%*ld free "),7, stat.dwAvailPhys/1024);
				theApp.getLog()->WriteLine(szTemp);


				g_bMainWatchdogFlag=false;
			}
			else
			{
				theApp.getLog()->WriteLine(_T("#HFO:999"));
			}
			
		}
		break;
	/*case WAIT_TIMEOUT:
		{
			
		}
		break;
	case WAIT_FAILED:
		{
			
		}
		break;
	case WAIT_ABANDONED:*/
	default:
		{
			
		}
		break;
	}
	
	

	CloseHandle(g_hAction);
	return true;
}

/**********************************************************************************************//**
 * Starts save trend USB thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::StartSaveTrendUSBThread( void )
{
	m_bDoSaveTrendUSBThread=true;

	if(m_pcwtSaveTrendUSBThread!=NULL)
	{
		delete m_pcwtSaveTrendUSBThread;
		m_pcwtSaveTrendUSBThread=NULL;

		if(m_hThreadSaveTrendUSB!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSaveTrendUSB);
			m_hThreadSaveTrendUSB=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtSaveTrendUSBThread=AfxBeginThread(CSaveTrendUSBThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadSaveTrendUSB=m_pcwtSaveTrendUSBThread->m_hThread;
	m_pcwtSaveTrendUSBThread->m_bAutoDelete = FALSE; 
	m_pcwtSaveTrendUSBThread->ResumeThread();
}

/**********************************************************************************************//**
 * Stops save trend USB thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::StopSaveTrendUSBThread( void )
{
	if(m_bDoSaveTrendUSBThread)
	{
		m_bDoSaveTrendUSBThread=false;
		eventSaveTrendUSBfinish.SetEvent();

		if (WaitForSingleObject(m_pcwtSaveTrendUSBThread->m_hThread,3000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:021a"));
			if(!TerminateThread(m_pcwtSaveTrendUSBThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:021b"));
			}
		}
	}
}

/**********************************************************************************************//**
 * Saves a trend USB thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

static UINT CSaveTrendUSBThread( LPVOID pc )
{
	try
	{
		((CMainFrame*)pc)->SaveTrendUSB();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CSaveTrendUSBThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CSaveTrendUSBThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CMainFrame*)pc)->SaveTrendUSB();
	return TRUE;
}

/**********************************************************************************************//**
 * Saves the trend USB
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CMainFrame::SaveTrendUSB(void) 
{
	DWORD dw = ::WaitForSingleObject(eventSaveTrendUSBfinish, INFINITE);

	switch(dw)
	{
	case WAIT_OBJECT_0:
		{
			if(m_bDoSaveTrendUSBThread)
			{
				if(!m_bExit && m_bDoSaveTrendUSBThread)
				{
					CStringW szFileUSB = _T("\\Hard Disk\\");
					szFileUSB+=getModel()->GetUniqueID();
					szFileUSB+=_T("-TRENDS\\");
					if(!CTlsFile::Exists(szFileUSB))
					{
						CTlsFile::MkDir(szFileUSB);
					}

					getModel()->getDATAHANDLER()->SetSavingTrendToUSBactiv();

					theApp.getLog()->WriteLine(_T("*** SerializeAllTrends SaveTrendUSB"));

					//rkuNEWFIX
					//EnterCriticalSection(&getModel()->getDATAHANDLER()->csTrend);
					getModel()->getDATAHANDLER()->SerializeAllTrends(false);//rkuNEWFIX
					Sleep(0);
					SaveTrendDataToUSB(TREND_PINSP);
					Sleep(0);
					SaveTrendDataToUSB(TREND_PMEAN);
					Sleep(0);
					SaveTrendDataToUSB(TREND_FIO2);
					Sleep(0);
					SaveTrendDataToUSB(TREND_VTE);
					Sleep(0);
					SaveTrendDataToUSB(TREND_COMPLIANCE);
					Sleep(0);
					SaveTrendDataToUSB(TREND_CO2HFO);
					Sleep(0);
					SaveTrendDataToUSB(TREND_MV);
					Sleep(0);
					SaveTrendDataToUSB(TREND_HFAMP);
					Sleep(0);
					SaveTrendDataToUSB(TREND_RSBI);
					Sleep(0);
					SaveTrendDataToUSB(TREND_SHAREMVMAND);
					Sleep(0);
					SaveTrendDataToUSB(TREND_RESISTANCE);
					Sleep(0);
					SaveTrendDataToUSB(TREND_LEAK);
					Sleep(0);
					SaveTrendDataToUSB(TREND_SPO2);
					Sleep(0);
					SaveTrendDataToUSB(TREND_SPO2PI);
					Sleep(0);
					SaveTrendDataToUSB(TREND_ETCO2);
					Sleep(0);
					SaveTrendDataToUSB(TREND_SPO2PR);
					Sleep(0);
					SaveTrendDataToUSB(TREND_FREQUENCY);
					//LeaveCriticalSection(&getModel()->getDATAHANDLER()->csTrend);

					getModel()->getDATAHANDLER()->SetSavingTrendToUSBinactiv();
				}
			}

			if(AfxGetApp() && m_bDoSaveTrendUSBThread)
			{
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_CLEAR_TEXT);
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}

			if(m_bDoSaveTrendUSBThread)
				eventWaitTrendUSBfinish.SetEvent();
		}
		break;
	default:
		break;
	}
	m_bDoSaveTrendUSBThread=false;

	//theApp.getLog()->WriteLine(_T("#THR:021"));

	return 0;
}

/**********************************************************************************************//**
 * Starts timer thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::StartTimerThread(void)
{
	m_bDoDoTimerFunctionsThread=true;

	if(m_pcwtTimerThread!=NULL)
	{
		delete m_pcwtTimerThread;
		m_pcwtTimerThread=NULL;

		if(m_hThreadTimerFunctions!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadTimerFunctions);
			m_hThreadTimerFunctions=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtTimerThread=AfxBeginThread(CTimerThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadTimerFunctions=m_pcwtTimerThread->m_hThread;
	m_pcwtTimerThread->m_bAutoDelete = FALSE; 
	m_pcwtTimerThread->ResumeThread();
}

/**********************************************************************************************//**
 * Stops timer thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::StopTimerThread( void )
{
	if(m_bDoDoTimerFunctionsThread)
	{
		m_bDoDoTimerFunctionsThread=false;
		eventDoTimerFunctions.SetEvent();

		if (WaitForSingleObject(m_pcwtTimerThread->m_hThread,3000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:020a"));
			if(!TerminateThread(m_pcwtTimerThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:020b"));
			}
		}
	}
	
}

/**********************************************************************************************//**
 * Timer thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

static UINT CTimerThread( LPVOID pc )
{
	try
	{
		((CMainFrame*)pc)->DoTimerFunctions();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CTimerThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CTimerThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CMainFrame*)pc)->DoTimerFunctions();
	return TRUE;
}

/**********************************************************************************************//**
 * Executes the timer functions operation
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CMainFrame::DoTimerFunctions(void) 
{
	DWORD WAIT=100;
	DWORD dwWait=0;
	DWORD dwDiff = 0;
	DWORD dwStart=0;
	DWORD dwEnd=0;

#if defined(OWN_ACULINK)
	DWORD dwTestDiff = 0;
	DWORD dwTestStart=0;
	DWORD dwTestEnd=0;
#endif

	task_tag *p;

	DWORD dwLastTickCountTrend=0;
	DWORD curTickCount=GetTickCount();

	if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
	{
		getModel()->getDATAHANDLER()->initTrend();
		Sleep(0);

		DEBUGMSG(TRUE, (TEXT("#########datahandler CheckLastTrendData\r\n")));
		UINT iSec=getModel()->getDATAHANDLER()->CheckLastTrendData();
		DEBUGMSG(TRUE, (TEXT("#########datahandler CheckLastTrendData finished\r\n")));

		if(iSec!=0)
		{
			dwLastTickCountTrend=(iSec*1000);
		}
		else
		{
			dwLastTickCountTrend=TRENDTIME_MS;
		}

		if(dwLastTickCountTrend>TRENDTIME_MS)
		{
			dwLastTickCountTrend=TRENDTIME_MS;
		}
		if(curTickCount<dwLastTickCountTrend)//rkuTICKCOUNTCHECK
		{
			dwLastTickCountTrend=0xFFFFFFFF-dwLastTickCountTrend;
		}
	}
	
#if defined(OWN_ACULINK)
	/* Fill task array */
	//AkkuCheck
	task[0].tickcount = curTickCount+3000;
	task[0].step = 2000;  /* in ms */
	task[0].func = TF_AKKUCHECK;
	//Trends
	task[1].tickcount = curTickCount-dwLastTickCountTrend;
	//task[1].step = 500;  /* in ms */
	task[1].step = 30000;  /* in ms */
	task[1].func = TF_TRENDS;
	//O2 Data
	task[2].tickcount = curTickCount+3000;
	task[2].step = 1000;  /* in ms */
	task[2].func = TF_O2DATA;
	//Flow Data
	task[3].tickcount = curTickCount+3000;
	task[3].step = 1000;  /* in ms */
	task[3].func = TF_FLOWDATA;
	//SPI check
	task[4].tickcount = curTickCount+3000;
	task[4].step = 1000;  /* in ms */
	task[4].func = TF_SPICHECK;
	//acuLink Measurements
	task[5].tickcount = curTickCount;
	task[5].step = 400;  /* in ms */
	task[5].func = TF_ALINK_MEASUREDATA;
	//TEST
	//task[7].tickcount = getTickCount64();
	//task[7].step = 400;  /* in ms */
	//task[7].func = TF_TEST;
	int iCountTerminal=0;

	m_numtasks = 6;
#else
	/* Fill task array */
	//AkkuCheck
	task[0].tickcount = curTickCount+3000;
	task[0].step = 2000;  /* in ms */
	task[0].func = TF_AKKUCHECK;
	//OpTime
	//task[1].tickcount = getTickCount64();
	//task[1].step = 30000;  /* in ms */
	//task[1].func = TF_OPTIME;
	//Trends
	task[1].tickcount = curTickCount-dwLastTickCountTrend;
	//task[1].step = 500;  /* in ms */
	task[1].step = 30000;  /* in ms */
	task[1].func = TF_TRENDS;
	//O2 Data
	task[2].tickcount = curTickCount+3000;
	task[2].step = 1000;  /* in ms */
	task[2].func = TF_O2DATA;
	//Flow Data
	task[3].tickcount = curTickCount+3000;
	task[3].step = 1000;  /* in ms */
	task[3].func = TF_FLOWDATA;
	//SPI check
	task[4].tickcount = curTickCount+3000;
	task[4].step = 1000;  /* in ms */
	task[4].func = TF_SPICHECK;
	//Nebulizer
	//task[5].tickcount = curTickCount+3000;
	//task[5].step = 1000;  /* in ms */
	//task[5].func = TF_NEBULIZER;
	

	m_numtasks = 5;
#endif
			
	//test memory leak
	/*m_pcTest=NULL;
	CClientDC dc(this);
	int iCount=0;
	while(true)
	{
		m_pcTest= new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_PIC_SYSALARM);
		
		Sleep(1000);
		iCount++;

		if(iCount>10)
		{
			iCount=0;
			MEMORYSTATUS stat;
			GlobalMemoryStatus (&stat);
			DWORD dwMem=stat.dwAvailPhys/1024;

			CString szTemp=_T("");
			szTemp.Format(_T("%*ld free "),7, stat.dwAvailPhys/1024);
			DEBUGMSG(TRUE, (szTemp));
			DEBUGMSG(TRUE, (TEXT("\r\n")));
		}
		
	}*/
	//TEST LOCK SERIAL
	//int iTestCnt=0;
	
	MEMORYSTATUS stat;
	m_dwLastWatchdogMemoryCheck=GetTickCount();
	bool bPrintMemoryError=false;
	 
	//UINT iNebulizeCounter=0;
	//UINT iNebulizeDelayCounter=10;
	int iHighO2=0;
	BYTE iSpiERROR=0;
	DWORD dwCurTickCount=0;
	dwStart = GetTickCount();
	//dwStart = Test_GetTickCount();

	do
	{
		dwEnd=GetTickCount();
		//dwEnd=Test_GetTickCount();

		if(dwEnd>=dwStart)
			dwDiff=dwEnd-dwStart;
		else
			dwDiff=0;

		if(dwDiff<WAIT)
		{
			dwWait=WAIT-dwDiff;
		}
		else
		{
			dwWait=1;
		}

		DWORD dw = ::WaitForSingleObject(eventDoTimerFunctions, dwWait);

		if(m_bDoDoTimerFunctionsThread==false)
			continue;

		dwStart = GetTickCount();
		//dwStart = Test_GetTickCount();

		switch(dw)
		{
		case WAIT_OBJECT_0:
			break;
		default:
			{
				//dwCurTickCount = Test_GetTickCount();
				dwCurTickCount = GetTickCount();//rkuTICKCOUNTCHECK

				/* Check task list if some need handling */
				for (int i=0; i<m_numtasks; i++)
				{
					p = &task[i];
					if (dwCurTickCount - p->tickcount >= p->step)//rkuTICKCOUNTCHECK
					{
						/* Handle task */
						//p->f();
						switch(p->func)
						{
						case TF_SPICHECK://newSPI
							{
								if(getModel()->getSPI()->check_ALIVE_BYTE())
								{
									if(FALSE==getModel()->isSPIavailabel())
									{
										getModel()->setSPIavailability(TRUE);
									}
									if(getModel()->getALARMHANDLER()->ALARM_SysFail_IF_SPI->getAlarmState()==AS_ACTIVE)
									{
										getModel()->getDATAHANDLER()->deleteSPIErrorCode();
										getModel()->getALARMHANDLER()->setAutoSilent();
										getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_SPI, AS_SIGNALED);

										eVentMode mode=VM_NONE;
										if(getModel()->getCONFIG()->CurModeIsPresetMode())
											mode=getModel()->getCONFIG()->GetPrevMode();
										else
											mode=getModel()->getCONFIG()->GetCurMode();

										getModel()->Send_VENT_MODE(mode);
									}
								}
								else
								{
									iSpiERROR++;

									if(getModel()->getALARMHANDLER()->ALARM_SysFail_IF_SPI->getAlarmState()!=AS_ACTIVE && iSpiERROR>ERRORCNT_SPI)
									{
										iSpiERROR=0;
										getModel()->setSPIavailability(FALSE);
										getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READALIVE);
										if(AfxGetApp())
											AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
									}
								}

								if(getModel()->isSPIavailabel())
								{
									int iVal=getModel()->getSPI()->Read_VENT_MODE();

									eVentMode mode=VM_NONE;
									if(getModel()->getCONFIG()->CurModeIsPresetMode())
										mode=getModel()->getCONFIG()->GetPrevMode();
									else
										mode=getModel()->getCONFIG()->GetCurMode();

									if((int)mode!=iVal)
									{
										getModel()->Send_VENT_MODE(mode);
									}
								}
							}
							break;
						case TF_O2DATA:
							{
								int iVal=getModel()->getSPI()->Read_OXY_ACTUAL();
								if(		getModel()->getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF
									||	getModel()->getDATAHANDLER()->GetOxySensorCalState()==OS_DEFECT)
								{
									getModel()->getDATAHANDLER()->setMessureDataO2(-1);
								}
								else
								{
									/*if(iVal<210)
										iVal=210;
									else*/ if(iVal>1000)
										iVal=1000;
									
									getModel()->getDATAHANDLER()->setMessureDataO2(iVal);
									//DEBUGMSG(TRUE, (TEXT("oxy\r\n")));
								}

								if(iVal<0)
									iVal=0;

								//Wert auch an STEUER-PIC==COM "S_FG_OXY_CORR, i" weiterleiten
								getModel()->Send_OXY_CORR(iVal,true,false);

								//check memory in case of memory leak					//PRICO04
#ifndef SIMULATION_ENTREK //rkuNEWFIX
								if(isSafeTickCountDelayExpired(m_dwLastWatchdogMemoryCheck, 30000)) //rkuNEWFIX
								{ 
									m_dwLastWatchdogMemoryCheck=GetTickCount();

									GlobalMemoryStatus (&stat);
									DWORD dwMem=stat.dwAvailPhys/1024;

									//rkuDEBUGMSG1
									CString szMem=_T("");
									szMem.Format(L"MEMORY [%d]",dwMem);
									//theApp.getLog()->WriteLine(szMem);
									//DEBUGMSG(TRUE, (szMem));
									//DEBUGMSG(TRUE, (TEXT("MEMORY %d\r\n"),dwMem));

									if(dwMem<2000)	//rkuNEWFIX		
									{
										theApp.getLog()->WriteLine(_T("#HFO:0220 OUTOFMEMORY"));
										PostMessage(WM_SETALARM_OUTOFMEMORY);
									}
									else if(dwMem<3000)		//rkuNEWFIX	
									{
										if(bPrintMemoryError)
										{
											CString szMem=_T("");
											szMem.Format(L"#HFO:0220 MEMORY [%d]",dwMem);
											theApp.getLog()->WriteLine(szMem);
										}
										bPrintMemoryError=false;
									}
									else
									{
										if(!bPrintMemoryError)
										{
											bPrintMemoryError=true;
										}
									}
								}
#endif

								//TEST!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
								/*if(m_dwLastWatchdogMemoryCheck+10000<GetTickCount())
								{
									m_dwLastWatchdogMemoryCheck=GetTickCount();

									GlobalMemoryStatus (&stat);
									DWORD dwMem=stat.dwAvailPhys/1024;

									CString szMem=_T("");
									szMem.Format(L"MEMORY [%d]",dwMem);
									theApp.getLog()->WriteLine(szMem);
									DEBUGMSG(TRUE, (szMem));
									
								}*/
								
								//TEST
								/*CStringW sz=_T("!!!test !!!test !!!test !!!test !!!test !!!test !!!test !!!test !!!test !!!test !!!test !!!test !!! end");
								theApp.getLog()->WriteLine(sz);*/
							}
							break;
#if defined(OWN_ACULINK)
						//case TF_TEST:
						//	{
						//		//theApp.getLog()->WriteLine(_T("#T"));
						//		//getModel()->checkVirtualCOM();
						//		iCountTerminal++;

						//		if(iCountTerminal==5)
						//		{
						//			dwTestStart = GetTickCount();
						//			getModel()->sendTerminalMeasurmentData();
						//			//getModel()->sendTestData();
						//			iCountTerminal=0;
						//			dwTestEnd=GetTickCount();

						//			if(dwTestEnd>=dwTestStart)
						//				dwTestDiff=dwTestEnd-dwTestStart;
						//			else
						//				dwTestDiff=MAXDWORD-dwTestStart+dwTestEnd;

						//			if(dwTestDiff>2000)
						//			{
						//				//theApp.getLog()->WriteLine(_T("#TdwDiff#1"));
						//				CString sz;
						//				sz.Format(_T("#TdwDiff#1: %d"), dwTestDiff);
						//				theApp.getLog()->WriteLine(sz);
						//			}
						//			
						//		}


						//		dwTestStart = GetTickCount();
						//		getModel()->sendTestData();
						//		dwTestEnd=GetTickCount();

						//		if(dwTestEnd>=dwTestStart)
						//			dwTestDiff=dwTestEnd-dwTestStart;
						//		else
						//			dwTestDiff=MAXDWORD-dwTestStart+dwTestEnd;

						//		if(dwTestDiff>2000)
						//		{
						//			theApp.getLog()->WriteLine(_T("#TdwDiff#2"));
						//		}

						//		
						//	}
						//	break;
						case TF_ALINK_MEASUREDATA:
							{
								iCountTerminal++;

								if(iCountTerminal==5)
								{
									//theApp.getLog()->WriteLine(_T("#W1"));
									getModel()->getAcuLink()->sendMeasurementData();
									//theApp.getLog()->WriteLine(_T("#W2"));
									iCountTerminal=0;

									//DEBUGMSG(TRUE, (TEXT("sendMeasurementData\r\n")));

								}

								//ullTestStart = getTickCount64();
								//getModel()->getAcuLink()->sendMeasurementData();
								//getModel()->getAcuLink()->sendTestWaveData();
								//ullTestEnd=getTickCount64();

								//if(ullTestEnd>=ullTestStart)
								//	dwTestDiff=ullTestEnd-ullTestStart;
								//else
								//	dwTestDiff=0;

								//if(dwTestDiff>1000)
								//{
								//	//theApp.getLog()->WriteLine(_T("#alink1"));
								//	CString sz;
								//	sz.Format(_T("#alink1: %d"), dwTestDiff);
								//	theApp.getLog()->WriteLine(sz);
								//}
							}
							break;
#endif
						case TF_AKKUCHECK:
							{
								getModel()->getDATAHANDLER()->CheckAkku();
							}
							break;
						case TF_TRENDS:
							{
								if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
								{
									//DEBUGMSG(TRUE, (TEXT("TIMER SaveTrendData\r\n")));
									if(m_bStartTrend)
										getModel()->getDATAHANDLER()->SaveTrendData();
								}

								PostMessage(WM_CHECK_DATE);

								const char *divisor = "K";
								MEMORYSTATUS stat;
								GlobalMemoryStatus (&stat);
								DWORD dwMem=stat.dwAvailPhys/1024;
								if(dwMem<1000)			
								{
									CStringW sz=_T("");
									sz.Format(_T("#HFO:0223 %*ld free %sbytes."),7, stat.dwAvailPhys/1024, divisor);
									theApp.getLog()->WriteLine(sz);
									PostMessage(WM_SETALARM_OUTOFMEMORY);
								}
								
								/*CStringW sz=_T("!!!test !!!test !!!test !!!test !!!test !!!test !!!test !!!test !!!test !!!test !!!test !!!test !!! end");
								theApp.getLog()->WriteLine(sz);*/
							}
							break;
						case TF_FLOWDATA:
							{
								if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
								{
									getModel()->getSERIAL()->GetM_INSP_FLOW();
									
									getModel()->getSERIAL()->GetM_EXP_FLOW();
								}
								else if(getModel()->getVMODEHANDLER()->activeModeIsCPAP())
								{
									getModel()->getSERIAL()->GetM_DEMAND_FLOW();
								}
								
							}
							break;
						}
						p->tickcount = dwCurTickCount;//rkuTICKCOUNTCHECK
					}
				}

				
			}
			break;
		}


	}while(m_bDoDoTimerFunctionsThread);

	//theApp.getLog()->WriteLine(_T("#THR:020"));

	return 0;
}

/**********************************************************************************************//**
 * Starts oxy calibration thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::StartOxyCalThread( void )
{
	m_bDoOxyCalThread=true;
	//m_pcwtOxyCalThread=AfxBeginThread(COxyCalThread,this);

	if(m_pcwtOxyCalThread!=NULL)
	{
		delete m_pcwtOxyCalThread;
		m_pcwtOxyCalThread=NULL;

		if(m_hThreadOxyCal!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadOxyCal);
			m_hThreadOxyCal=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtOxyCalThread=AfxBeginThread(COxyCalThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadOxyCal=m_pcwtOxyCalThread->m_hThread;
	m_pcwtOxyCalThread->m_bAutoDelete = FALSE; 
	m_pcwtOxyCalThread->ResumeThread();
}

/**********************************************************************************************//**
 * Stops oxy calibration thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::StopOxyCalThread( void )
{
	if(m_bDoOxyCalThread)
	{
		m_bDoOxyCalThread=false;
		m_bO2Sensor21manual=false;
		m_bO2Sensor100manual=false;
		eventOxyCal.SetEvent();

		if (WaitForSingleObject(m_pcwtOxyCalThread->m_hThread,3000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:019a"));
			if(!TerminateThread(m_pcwtOxyCalThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:019b"));
			}
		}
	}
}

/**********************************************************************************************//**
 * Oxy calibration thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

static UINT COxyCalThread( LPVOID pc )
{
	try
	{
		((CMainFrame*)pc)->CheckOxyCal();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("COxyCalThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("COxyCalThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CMainFrame*)pc)->CheckOxyCal();
	return TRUE;
}

/**********************************************************************************************//**
 * Check oxy calibrate
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CMainFrame::CheckOxyCal(void) 
{
	Sleep(1000);

	DWORD WAIT=86400000;
	//DWORD WAIT=300000; //alle 5 min
	DWORD dwWait=0;
	DWORD dwDiff = 0;
	DWORD dwStart=0;
	DWORD dwEnd=0;

	bool bO2Sensor21Calibrated=false;
	bool bO2Sensor100Calibrated=false;

	do
	{
		if(m_bDelayAutoOxyCal)
		{
			dwWait=2000;
		}
		else
		{
			dwEnd=GetTickCount();

			if(dwStart==0)
				dwDiff=WAIT;
			else if(dwEnd>=dwStart)
				dwDiff=dwEnd-dwStart;
			else
				dwDiff=0;

			if(dwDiff<WAIT)
			{
				dwWait=WAIT-dwDiff;
			}
			else
			{
				dwWait=1;
			}
		}
		bO2Sensor21Calibrated=false;
		bO2Sensor100Calibrated=false;

		DWORD dw = ::WaitForSingleObject(eventOxyCal, dwWait);

		if(m_bDoOxyCalThread==false)
			continue;

		switch(dw)
		{
		case WAIT_OBJECT_0:
			{
				theApp.getLog()->WriteLine(_T("*** O2 cal manual***"));
				if(	getModel()->getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF)
					break;
				else if(	m_bO2SensorAutocal==true
						||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating()==true)
				{
					break;
				}

				m_bDelayAutoOxyCal=false;

				KillTimer(OXYCAL_HOURGLASS_TIMER);

				getModel()->getDATAHANDLER()->SetOxyCalRunning(true);
				getModel()->getDATAHANDLER()->SetOxyHourglassRunning(true);
				getModel()->getVIEWHANDLER()->SetOxyHourglass(true);

				PostMessage(WM_CONTROL_O2CALSTART);

				if(getModel()->getALARMHANDLER()->ALARM_Sens_O2_NOT_CALIBRATED->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_O2_NOT_CALIBRATED);
				}
				if(getModel()->getALARMHANDLER()->ALARM_Sens_O2_SENSOR_USED->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_O2_SENSOR_USED);
				}
				

				if(m_bO2Sensor21manual && m_bDoOxyCalThread)
				{
					m_bO2Sensor21manual=false;
					if(m_bDoOxyCalThread)
						bO2Sensor21Calibrated=CalOxySensor21();

					if(bO2Sensor21Calibrated)
					{
						getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
					}
				}
				else if(m_bO2Sensor100manual && m_bDoOxyCalThread)
				{
					m_bO2Sensor100manual=false;
					DEBUGMSG(TRUE, (TEXT("CMainFrame CalOxySensor21\r\n")));
					if(m_bDoOxyCalThread)
						bO2Sensor21Calibrated=CalOxySensor21();

					if(bO2Sensor21Calibrated
						&& !m_bCancelOxiCal
						&& m_bDoOxyCalThread)
					{
						DEBUGMSG(TRUE, (TEXT("CMainFrame CalOxySensor100\r\n")));
						if(m_bDoOxyCalThread)
							bO2Sensor100Calibrated=CalOxySensor100();

						if(bO2Sensor100Calibrated)
						{
							getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
						}
					}
				}

				m_dwHourglasstimeout=GetTickCount();
				if(m_bDoOxyCalThread)
					SetTimer(OXYCAL_HOURGLASS_TIMER, 500, NULL);

				if(!m_bExit && m_bDoOxyCalThread)
				{
					getModel()->getDATAHANDLER()->SetOxyCalRunning(false);

					PostMessage(WM_CONTROL_O2CALSTOP);
				}
			}
			break;
		default:
			{
				if(m_bDelayAutoOxyCal)
					break;

				eStateOfAlarm state1=getModel()->getALARMHANDLER()->ALARM_SysAl_P_IN_AIR->getAlarmState();
				eStateOfAlarm state2=getModel()->getALARMHANDLER()->ALARM_SysAl_P_IN_O2->getAlarmState();
				if(		getModel()->getALARMHANDLER()->ALARM_SysAl_P_IN_O2->getAlarmState()==AS_ACTIVE
					||	getModel()->getALARMHANDLER()->ALARM_SysAl_P_IN_AIR->getAlarmState()==AS_ACTIVE)
				{
					m_bDelayAutoOxyCal=true;
					getModel()->SetO2calFlag();
					break;
				}

				theApp.getLog()->WriteLine(_T("*** O2 cal time***"));
				if(	getModel()->getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF)
					break;
				else if(	getModel()->getDATAHANDLER()->GetO21SensorCalState()==true
						||	getModel()->getDATAHANDLER()->GetO100SensorCalState()==true
						||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating()==true)
				{
					break;
				}
				KillTimer(OXYCAL_HOURGLASS_TIMER);

				m_bO2SensorAutocal=true;
				getModel()->getDATAHANDLER()->SetOxyCalRunning(true);
				getModel()->getVIEWHANDLER()->SetOxyHourglass(true);
				getModel()->getDATAHANDLER()->SetOxyHourglassRunning(true);

				PostMessage(WM_CONTROL_O2CALSTART);

				if(m_bDoOxyCalThread)
					bO2Sensor21Calibrated=CalOxySensor21();

				if(getModel()->getCONFIG()->GetAutoOxyCal()==CAL_21_100 
					&& bO2Sensor21Calibrated 
					&& !m_bCancelOxiCal
					&& m_bDoOxyCalThread)
				{
					if(m_bDoOxyCalThread)
						bO2Sensor100Calibrated=CalOxySensor100();
				}

				m_dwHourglasstimeout=GetTickCount();
				if(m_bDoOxyCalThread)
					SetTimer(OXYCAL_HOURGLASS_TIMER, 500, NULL);

				if(getModel()->getCONFIG()->GetAutoOxyCal()==CAL_21_100 && bO2Sensor21Calibrated && bO2Sensor100Calibrated)
					getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
				else if(bO2Sensor21Calibrated)
					getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);

				if(!m_bExit && m_bDoOxyCalThread)
				{
					getModel()->getDATAHANDLER()->SetOxyCalRunning(false);
					PostMessage(WM_CONTROL_O2CALSTOP);
				}
				m_bO2SensorAutocal=false;
			}
			break;
		}

		dwStart = GetTickCount();

	}while(m_bDoOxyCalThread);

	m_bDoOxyCalThread=false;

	//theApp.getLog()->WriteLine(_T("#THR:019"));

	return 0;
}

/**********************************************************************************************//**
 * Determines if we can calibrate oxy sensor 21
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMainFrame::CalOxySensor21(void) 
{
	bool bResult=true;

	theApp.getLog()->WriteLine(_T("*** OXY 21 CAL ***"));

	getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_CALIBRATING);
	getModel()->getDATAHANDLER()->SetO21SensorCalState(true);
	Sleep(100);
	getModel()->Send_MODE_OPTION1();

	int iState21=CMD_OXYCALIBRATION21;
	getModel()->getSPI()->Send_KOMMANDO(iState21);
	Sleep(100);

	DWORD timeout=GetTickCount();
	eAlarm alarm = AL_NONE;

	while(		(!isSafeTickCountDelayExpired(timeout, 120000))
		&&	(	iState21==CMD_OXYCALIBRATION21)
		&&	(	iState21!=-1)
		&&	!m_bExit && !m_bCancelOxiCal
		&&	m_bDoOxyCalThread)
	{
		iState21=getModel()->getSPI()->Read_KOMMANDO();
		Sleep(500);
	}

	if(m_bCancelOxiCal && m_bDoOxyCalThread)
	{
		getModel()->getSPI()->Send_KOMMANDO(0);
		getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_CAL_CNCL);
		getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
		m_bCancelOxiCal=false;
	}
	else if(iState21==0 && !m_bExit && m_bDoOxyCalThread)	//zurückgesetzt? -> Status holen
	{
		iState21=getModel()->getSPI()->Read_STATUS_2();

		/*Bit 0 = 1   Sens. demnächst austauschen
		Bit 1 = 1   Sensor defekt
		Bit 2 = 1   Kalibrierung fehlgeschlagen*/
		if(iState21 & BIT0)
		{
			getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_REPLACE);
			alarm=AL_Sens_O2_SENSOR_USED;
		}
		else if(iState21 & BIT1)
		{
			getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_DEFECT);
			alarm=AL_Sens_O2_SENSOR_DEFECT;
		}
		else if(iState21 & BIT2)
		{
			if(iState21 & BIT4)
			{
				getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_NOT_CALIBRATED_AIR);
				alarm=AL_Sens_O2_NOT_CALIBRATED;
			}
			else if(iState21 & BIT5)
			{
				getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_NOT_CALIBRATED_OXY);
				alarm=AL_Sens_O2_NOT_CALIBRATED;
			}
			else
			{
				getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_NOT_CALIBRATED);
				alarm=AL_Sens_O2_NOT_CALIBRATED;
			}
		}
		else
		{
			SYSTEMTIME st;
			GetLocalTime(&st);

			COleDateTime dtTimeLast21Cal(st);
			CStringW szTimeLast21Cal = dtTimeLast21Cal.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);

			getModel()->getCONFIG()->SetLastCalOxy21(szTimeLast21Cal);
			getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_21CALIBRATED);

			if(getModel()->getALARMHANDLER()->ALARM_Sens_O2_NOT_CALIBRATED->getAlarmState()!=AS_NONE)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_O2_NOT_CALIBRATED);
			}
		}
	}
	else if(m_bDoOxyCalThread)
	{
		getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_NOT_CALIBRATED);
		alarm=AL_Sens_O2_NOT_CALIBRATED;
	}

	if(m_bDoOxyCalThread)
	{
		getModel()->getDATAHANDLER()->SetO21SensorCalState(false);
		getModel()->Send_MODE_OPTION1();
	}

	if(alarm != AL_NONE && !m_bExit && m_bDoOxyCalThread)
	{
		bResult=false;

		switch(alarm)
		{
		case AL_Sens_O2_SENSOR_USED:
			{
				PostMessage(WM_SETALARM_O2_SENSOR_USED);
			}
			break;
		case AL_Sens_O2_SENSOR_DEFECT:
			{
				PostMessage(WM_SETALARM_O2_SENSOR_DEFECT);
			}
			break;
		case AL_Sens_O2_NOT_CALIBRATED:
			{
				PostMessage(WM_SETALARM_O2_NOT_CALIBRATED);
			}
			break;
		}
	}
	return bResult;
}

/**********************************************************************************************//**
 * Determines if we can calibrate oxy sensor 100
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMainFrame::CalOxySensor100(void) 
{
	bool bResult=true;
	//O 100% calibration
	theApp.getLog()->WriteLine(_T("*** OXY 100 CAL***"));
	
	getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_CALIBRATING);
	getModel()->getDATAHANDLER()->SetO100SensorCalState(true);
	Sleep(100);
	getModel()->Send_MODE_OPTION1();

	int iState100=CMD_OXYCALIBRATION100;
	getModel()->getSPI()->Send_KOMMANDO(iState100);
	Sleep(100);

	DWORD timeout=GetTickCount();
	eAlarm alarm = AL_NONE;

	while((!isSafeTickCountDelayExpired(timeout, 180000))
		&&(iState100==CMD_OXYCALIBRATION100)
		&& !m_bExit && !m_bCancelOxiCal
		&& m_bDoOxyCalThread)
	{
		iState100=getModel()->getSPI()->Read_KOMMANDO();
		Sleep(200);
	}

	if(!m_bExit && m_bCancelOxiCal && m_bDoOxyCalThread)
	{
		getModel()->getSPI()->Send_KOMMANDO(0);
		getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_CAL_CNCL);
		getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
		m_bCancelOxiCal=false;
	}
	else if(iState100==0 && !m_bExit && m_bDoOxyCalThread)	//zurückgesetzt? -> Status holen
	{
		iState100=getModel()->getSPI()->Read_STATUS_2();

		/*Bit 0 = 1   Sens. demnächst austauschen
		Bit 1 = 1   Sensor defekt
		Bit 2 = 1   Kalibrierung fehlgeschlagen*/
		if(iState100 & BIT0)
		{
			getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_REPLACE);
			alarm=AL_Sens_O2_SENSOR_USED;
		}
		else if(iState100 & BIT1)
		{
			getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_DEFECT);
			alarm=AL_Sens_O2_SENSOR_DEFECT;
		}
		else if(iState100 & BIT2)
		{
			if(iState100 & BIT4)
			{
				getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_NOT_CALIBRATED_AIR);
				alarm=AL_Sens_O2_NOT_CALIBRATED;
			}
			else if(iState100 & BIT5)
			{
				getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_NOT_CALIBRATED_OXY);
				alarm=AL_Sens_O2_NOT_CALIBRATED;
			}
			else
			{
				getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_NOT_CALIBRATED);
				alarm=AL_Sens_O2_NOT_CALIBRATED;
			}
		}
		else
		{
			SYSTEMTIME st;
			GetLocalTime(&st);

			COleDateTime dtTimeLast100Cal(st);
			CStringW szTimeLast100Cal = dtTimeLast100Cal.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);

			getModel()->getCONFIG()->SetLastCalOxy100(szTimeLast100Cal);
			getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_100CALIBRATED);
		}
	}
	else if(m_bDoOxyCalThread)
	{
		if(!m_bExit)
		{
			getModel()->getDATAHANDLER()->SetOxySensorCalState(OS_NOT_CALIBRATED);
			alarm=AL_Sens_O2_NOT_CALIBRATED;
		}
	}

	if(m_bDoOxyCalThread)
	{
		getModel()->getDATAHANDLER()->SetO100SensorCalState(false);
		getModel()->Send_MODE_OPTION1();
	}
	
	if(alarm != AL_NONE && !m_bExit && m_bDoOxyCalThread)
	{
		bResult=false;

		switch(alarm)
		{
		case AL_Sens_O2_SENSOR_USED:
			{
				PostMessage(WM_SETALARM_O2_SENSOR_USED);
			}
			break;
		case AL_Sens_O2_SENSOR_DEFECT:
			{
				PostMessage(WM_SETALARM_O2_SENSOR_DEFECT);
			}
			break;
		case AL_Sens_O2_NOT_CALIBRATED:
			{
				PostMessage(WM_SETALARM_O2_NOT_CALIBRATED);
			}
			break;
		}
	}	
	return bResult;
}

/**********************************************************************************************//**
 * Saves a trend data to USB
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	trendID	Identifier for the trend.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMainFrame::SaveTrendDataToUSB(UINT trendID)
{
	int nMax=0;
	bool bFileExists=false;
	bool bDeserialized=true;
	CString szFile=_T("");
	CString szTrendName=_T("");
	CString szTrendFileName=_T("");
	UINT iTrendBufCount=0;
	int iBufPos=0;
	WORD wFilenum=0;
	WORD wValue=0;

	CString szFolder=_T("\\sdcard\\");

	if(CTlsFile::Exists(szFolder)==false)
	{
		szFolder=_T("\\FFSDISK\\");
	}

	//WORD wBufferTrend[MAXSIZE_TREND_BUFFER];

	switch(trendID)
	{
	case TREND_PINSP:
		{
			szFolder+=IDS_TRD_FOLDER_PINSP;
			szTrendFileName=_T("PINSP.csv");
			szTrendName=_T("PINSP [mbar/10]");
		}
		break;
	case TREND_PMEAN:
		{
			szFolder+=IDS_TRD_FOLDER_PMEAN;
			szTrendFileName=_T("PMEAN.csv");
			szTrendName=_T("PMEAN [mbar/10]");
		}
		break;
	case TREND_FIO2:
		{
			szFolder+=IDS_TRD_FOLDER_FIO2;
			szTrendFileName=_T("FIO2.csv");
			szTrendName=_T("FIO2 [%/10]");
		}
		break;
	case TREND_VTE:
		{
			szFolder+=IDS_TRD_FOLDER_VTE;
			szTrendFileName=_T("VTE.csv");
			szTrendName=_T("VTE [ml/10]");
		}
		break;
	case TREND_COMPLIANCE:
		{
			szFolder+=IDS_TRD_FOLDER_COMPLIANCE;
			szTrendFileName=_T("COMPLIANCE.csv");
			szTrendName=_T("COMPLIANCE [ml/mbar/100]");
		}
		break;
	case TREND_CO2HFO:
		{
			szFolder+=IDS_TRD_FOLDER_CO2HFO;
			szTrendFileName=_T("CO2HFO.csv");
			szTrendName=_T("CO2HFO [ml^2/sec]");
			DEBUGMSG(TRUE, (TEXT("TREND: %s\r"),szTrendName));
		}
		break;
	case TREND_MV:
		{
			szFolder+=IDS_TRD_FOLDER_MV;
			szTrendFileName=_T("MV.csv");
			szTrendName=_T("MV [ml]");
		}
		break;
	case TREND_HFAMP:
		{
			szFolder+=IDS_TRD_FOLDER_HFAMP;
			szTrendFileName=_T("HFAMP.csv");
			szTrendName=_T("HFAMP [mbar/10]");
		}
		break;
	case TREND_RSBI:
		{
			szFolder+=IDS_TRD_FOLDER_RSBI;
			szTrendFileName=_T("RSBI.csv");
			szTrendName=_T("RSBI [/10]");
		}
		break;
	case TREND_SHAREMVMAND:
		{
			szFolder+=IDS_TRD_FOLDER_SHAREMVMAND;
			szTrendFileName=_T("SHAREMVMAND.csv");
			szTrendName=_T("SHAREMVMAND [%]");
		}
		break;
	case TREND_RESISTANCE:
		{
			szFolder+=IDS_TRD_FOLDER_RESISTANCE;
			szTrendFileName=_T("RESISTANCE.csv");
			szTrendName=_T("RESISTANCE [mbar/lps/10]");
		}
		break;
	case TREND_LEAK:
		{
			szFolder+=IDS_TRD_FOLDER_LEAK;
			szTrendFileName=_T("LEAK.csv");
			szTrendName=_T("LEAK [%]");
		}
		break;
	case TREND_SPO2:
		{
			szFolder+=IDS_TRD_FOLDER_SPO2;
			szTrendFileName=_T("SPO2.csv");
			szTrendName=_T("SPO2 [%/10]");
		}
		break;
	case TREND_SPO2PR:
		{
			szFolder+=IDS_TRD_FOLDER_SPO2PR;
			szTrendFileName=_T("SPO2PR.csv");
			szTrendName=_T("SPO2PR [bpm]");
		}
		break;
	case TREND_SPO2PI:
		{
			szFolder+=IDS_TRD_FOLDER_SPO2PI;
			szTrendFileName=_T("SPO2PI.csv");
			szTrendName=_T("SPO2PI [/1000]");
		}
		break;
	case TREND_ETCO2:
		{
			szFolder+=IDS_TRD_FOLDER_ETCO2;
			szTrendFileName=_T("ETCO2.csv");
			szTrendName=_T("ETCO2 [mmHg/10]");
		}
		break;
	case TREND_FREQUENCY:
		{
			szFolder+=IDS_TRD_FOLDER_FREQUENCY;
			szTrendFileName=_T("FREQ.csv");
			szTrendName=_T("FREQ [bpm]");
		}
		break;
	default:
		break;
	}

	CStringW szFileUSB = _T("\\Hard Disk\\");
	szFileUSB+=getModel()->GetUniqueID();
	szFileUSB+=_T("-TRENDS\\");
	szFileUSB+=szTrendFileName;

	if(CTlsFile::Exists(szFileUSB))
	{
		CTlsFile::Delete(szFileUSB);
	}



	CString szBuffer=_T("");
	FILE* stream = _wfopen(szFileUSB,L"w");

	if (stream == NULL)
		return false;

	//write to USB file
	try
	{
		szBuffer.Format(_T("%s\n"),szTrendName);
		fputws(szBuffer,stream);

		nMax=0;
		bDeserialized=true;
		szFile=_T("");
		iTrendBufCount=0;
		iBufPos=0;
		wValue=0;

		wFilenum=(WORD)getModel()->getCONFIG()->GetLastWrittenTrendFile(trendID);
		szFile.Format(_T("%s%d%s"),szFolder,wFilenum, IDS_TRD_FILE_ID);

		bFileExists=CTlsFile::Exists(szFile);

		if(bFileExists)
		{
			bDeserialized=getModel()->getDATAHANDLER()->DeserializeTempTrend(trendID, wFilenum);
			iBufPos=getModel()->getDATAHANDLER()->m_cTrendTemporary.GetBufferCount()-1;
		}
		if(bDeserialized)
		{
			COleDateTime dtTime=getModel()->getDATAHANDLER()->m_cTrendTemporary.GetTimestamp();
			CStringW szTime = dtTime.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);

			szBuffer.Format(_T("%s\n"),szTime);
			fputws(szBuffer,stream);
			//DEBUGMSG(TRUE, (TEXT("stream %s\r"),szBuffer));
		}

		/*DEBUGMSG(TRUE, (TEXT("Save Trend USB wFilenum %d\r"),wFilenum));
		DEBUGMSG(TRUE, (TEXT("Save Trend USB iBufPos %d\r"),iBufPos));
		DEBUGMSG(TRUE, (TEXT("Save Trend USB nMax %d\r"),nMax));
		DEBUGMSG(TRUE, (TEXT("Save Trend USB iTrendBufCount %d\r"),iTrendBufCount));
		DEBUGMSG(TRUE, (TEXT("Save Trend USB szFile %s\r"),szFile));*/

		while(		bFileExists 
			&&	bDeserialized 
			&&	nMax<MAXTRENDFILES
			&&	iTrendBufCount<MAXSIZE_TREND_BUFFER
			&&	!m_bExit)
		{
			//bDeserialized=getModel()->getDATAHANDLER()->DeserializeTempTrend(trendID, wFilenum);
			//iBufPos=getModel()->getDATAHANDLER()->m_cTrendTemporary.GetBufferCount()-1;

			for(iBufPos;iBufPos>=0;iBufPos--)
			{
				if(m_bExit)
				{
					return false;
				}

				if(iTrendBufCount<MAXSIZE_TREND_BUFFER)
				{
					wValue=getModel()->getDATAHANDLER()->m_cTrendTemporary.GetBufferValue(iBufPos);
					////m_wBufferTrend[iTrendBufCount]=wValue;
					//trendFile.Write( &wValue, sizeof( WORD ) ); 
					//trendFile.Write(pbufWrite, 1);

					//szBuffer.Format(_T("%d;"),wValue);
					szBuffer.Format(_T("%d\n"),wValue);

					fputws(szBuffer,stream);
					//fputws(_T(";"),stream);

					//DEBUGMSG(TRUE, (TEXT("value %s"),szBuffer));
				}
				else
					iBufPos=-1;
				iTrendBufCount++;

				Sleep(0);
			}

			wFilenum=wFilenum-1;
			if(wFilenum<=0)
				wFilenum=MAXTRENDFILES;
			szFile.Format(_T("%s%d%s"),szFolder,wFilenum, IDS_TRD_FILE_ID);
			bFileExists=CTlsFile::Exists(szFile);

			nMax++;

			if(		bFileExists 
				&&	nMax<MAXTRENDFILES
				&&	iTrendBufCount<MAXSIZE_TREND_BUFFER
				&&	!m_bExit)
			{
				DEBUGMSG(TRUE, (TEXT("Save Trend USB szFile %s\r\n"),szFile));
				bDeserialized=getModel()->getDATAHANDLER()->DeserializeTempTrend(trendID, wFilenum);
				iBufPos=getModel()->getDATAHANDLER()->m_cTrendTemporary.GetBufferCount()-1;
			}
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CMainFrame::SaveTrendDataToUSB"));

		if (stream != NULL)
		{
			fputws(L"\n",stream);
			fclose(stream);
		}
	}


	if (stream != NULL)
	{
		fputws(L"\n",stream);
		fclose(stream);
	}

	return true;
}

/**********************************************************************************************//**
 * Starts delete trend thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::StartDelTrendThread( void )
{
	DEBUGMSG(TRUE, (TEXT("StartDelTrendThread\r\n")));
	m_bDoDelTrendThread=true;

	if(m_pcwtDelTrendThread!=NULL)
	{
		delete m_pcwtDelTrendThread;
		m_pcwtDelTrendThread=NULL;

		if(m_hThreadDelTrend!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadDelTrend);
			m_hThreadDelTrend=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtDelTrendThread=AfxBeginThread(CDelTrendThread,this,THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadDelTrend=m_pcwtDelTrendThread->m_hThread;
	m_pcwtDelTrendThread->m_bAutoDelete = FALSE; 
	m_pcwtDelTrendThread->ResumeThread();
}

/**********************************************************************************************//**
 * Stops delete trend thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::StopDelTrendThread( void )
{
	DEBUGMSG(TRUE, (TEXT("StopDelTrendThread\r\n")));
	if(m_bDoDelTrendThread)
	{
		m_bDoDelTrendThread=false;
		eventDelTrend.SetEvent();

		if (WaitForSingleObject(m_pcwtDelTrendThread->m_hThread,500) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:022a"));
			if(!TerminateThread(m_pcwtDelTrendThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:022b"));
			}
		}
	}
}

/**********************************************************************************************//**
 * Deletes the trend thread
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

static UINT CDelTrendThread( LPVOID pc )
{
	try
	{
		((CMainFrame*)pc)->DelTrendData();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CDelTrendThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CDelTrendThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CMainFrame*)pc)->DelTrendData();
	return TRUE;
}

/**********************************************************************************************//**
 * Deletes the trend data
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CMainFrame::DelTrendData(void) 
{
	DWORD dw = ::WaitForSingleObject(eventDelTrend, INFINITE);
	switch(dw)
	{
	case WAIT_OBJECT_0:
		{
			//DEBUGMSG(TRUE, (TEXT("start DelTrendData\r\n")));
			if(m_bDoDelTrendThread)
			{
				CStringW sPathOld =_T("");
				CString szPath=_T("\\sdcard\\");
				if(CTlsFile::Exists(szPath)==false)
				{
					szPath=_T("\\FFSDISK\\");
					sPathOld = szPath+IDS_OLDTRD_FOLDER;
				}
				else
				{
					sPathOld = szPath+IDS_OLDTRD_FOLDER;
				}


				WIN32_FIND_DATA FileData;
				HANDLE hSearch = ::FindFirstFile(sPathOld + L"*.*", &FileData);
				if(hSearch != INVALID_HANDLE_VALUE)
				{
					do
					{
						CStringW sFile = sPathOld + FileData.cFileName;

						if(m_bDoDelTrendThread)
						{
							DeleteTrendData(sFile);
							Sleep(0);
						}
						if(m_bDoDelTrendThread)
							CTlsFile::DeleteDir(sFile);
						Sleep(0);
					} while(::FindNextFile(hSearch, &FileData) && m_bDoDelTrendThread);
				}
				if(hSearch!=INVALID_HANDLE_VALUE)
					FindClose(hSearch);
			}
			//DEBUGMSG(TRUE, (TEXT("stop DelTrendData\r\n")));
		}
		break;
	default:
		break;
	}

	m_bDoDelTrendThread=false;

	return 0;
}

/**********************************************************************************************//**
 * Deletes the trend data described by sFolder
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	sFolder	Pathname of the folder.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMainFrame::DeleteTrendData(CStringW sFolder)
{
	bool bResult=true;
	CStringW sFile = _T("");

	WIN32_FIND_DATA FileData;
	HANDLE hSearch = ::FindFirstFile(sFolder + _T("\\*.*"), &FileData);
	if(hSearch != INVALID_HANDLE_VALUE)
	{
		do
		{
			sFile = sFolder + _T("\\") + FileData.cFileName;

			WIN32_FIND_DATA FileData2;
			HANDLE hSearch2 = ::FindFirstFile(sFile + L"\\*.*", &FileData2);
			if(hSearch2 != INVALID_HANDLE_VALUE)
			{
				//delete trend files .trd
				do
				{
					CStringW sFile2 = sFile + L"\\" + FileData2.cFileName;
					if(m_bDoDelTrendThread)
					{
						if(CTlsFile::Delete(sFile2)==false)
						{
							DEBUGMSG(TRUE, (TEXT("DeleteTrendData fail %s\r\n"),sFile2));
						}
						else
						{
							//DEBUGMSG(TRUE, (TEXT("DeleteTrendData ok %s\r\n"),sFile2));
						}
					}
					Sleep(0);
					
				} while(::FindNextFile(hSearch2, &FileData2)&& m_bDoDelTrendThread);
				FindClose(hSearch2);
			}
			//delete folder
			if(m_bDoDelTrendThread)
			{
				if(CTlsFile::DeleteDir(sFile)==false)
				{
					DEBUGMSG(TRUE, (TEXT("DeleteTrendData fail1 %s\r\n"),sFile));
					theApp.getLog()->WriteLine(_T("#HFO:0224"));
				}
				/*else
				{
					DEBUGMSG(TRUE, (TEXT("DeleteTrendData ok2 %s\r\n"),sFile));
				}*/
			}
			Sleep(0);
		} while(::FindNextFile(hSearch, &FileData) && m_bDoDelTrendThread);
		FindClose(hSearch);
	}
	return bResult;
}

/**********************************************************************************************//**
 * Starts aculink
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMainFrame::startAcuLink()
{
	//#ifndef SIMULATION_VERSION
#ifndef MEDKOM_VERSION
	if(		getModel()->getCONFIG()->GetPDMSprotocol()==ACL_ETHERNET
		&&	getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setLinkType(ACL_LT_ETHERNET);
		getModel()->getAcuLink()->setShutdown(0);

		UINT uiPort=getModel()->getCONFIG()->GetEthernetPort();
		if(uiPort)
			getModel()->getAcuLink()->setNetPort(uiPort);

		PROCESS_INFORMATION pi;

		CreateProcess(_T("\\FFSDISK\\AcuLink.exe"), L"COM2", NULL, NULL, NULL, 0, NULL, NULL, NULL, &pi);
		theApp.getLog()->WriteLine(_T("***ACULINK: ACL_LT_ETHERNET"));
	}
	else if(		getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_IVOI
		&&	getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setLinkType(ACL_LT_SERIAL_IVOI);
		getModel()->getAcuLink()->setShutdown(0);

		getModel()->getAcuLink()->setLanguage(m_wLanguageID);

		PROCESS_INFORMATION pi;
		if(getModel()->getCONFIG()->GetMainBoardVersion()>=MAINBOARD_30)
		{
			CreateProcess(_T("\\FFSDISK\\AcuLink.exe"), L"COM2", NULL, NULL, NULL, 0, NULL, NULL, NULL, &pi);
			theApp.getLog()->WriteLine(_T("***ACULINK: ACL_LT_SERIAL_IVOI COM2"));
		}
		else	//MAINBOARD2_1
		{
			CreateProcess(_T("\\FFSDISK\\AcuLink.exe"), L"COM1", NULL, NULL, NULL, 0, NULL, NULL, NULL, &pi);
			theApp.getLog()->WriteLine(_T("***ACULINK: ACL_LT_SERIAL_IVOI COM1"));
		}
	}
	else if(		getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_ASCII
		&&	getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setLinkType(ACL_LT_SERIAL_ASCII);

#if defined(OWN_ACULINK)
		//do nothing here
#else
		getModel()->getAcuLink()->setShutdown(0);

		PROCESS_INFORMATION pi;

		if(getModel()->getCONFIG()->GetMainBoardVersion()>=MAINBOARD_30)
		{
			CreateProcess(_T("\\FFSDISK\\AcuLink.exe"), L"COM2", NULL, NULL, NULL, 0, NULL, NULL, NULL, &pi);
			theApp.getLog()->WriteLine(_T("***ACULINK: ACL_LT_SERIAL_ASCII COM2"));
		}
		else	//MAINBOARD2_1
		{
			CreateProcess(_T("\\FFSDISK\\AcuLink.exe"), L"COM1", NULL, NULL, NULL, 0, NULL, NULL, NULL, &pi);
			theApp.getLog()->WriteLine(_T("***ACULINK: ACL_LT_SERIAL_ASCII COM1"));
		}
#endif

	}
	else if(		getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_WAVE
		&&	getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setLinkType(ACL_LT_SERIAL_WAVE);

#if defined(OWN_ACULINK)
		//do nothing here
#else
		getModel()->getAcuLink()->setShutdown(0);

		//getModel()->getAcuLink()->SetLanguage(m_wLanguageID);

		PROCESS_INFORMATION pi;
		if(getModel()->getCONFIG()->GetMainBoardVersion()>=MAINBOARD_30)
		{
			CreateProcess(_T("\\FFSDISK\\AcuLink.exe"), L"COM2", NULL, NULL, NULL, 0, NULL, NULL, NULL, &pi);
			theApp.getLog()->WriteLine(_T("***ACULINK: ACL_LT_SERIAL_WAVE COM2"));
		}
		else	//MAINBOARD2_1
		{
			CreateProcess(_T("\\FFSDISK\\AcuLink.exe"), L"COM1", NULL, NULL, NULL, 0, NULL, NULL, NULL, &pi);
			theApp.getLog()->WriteLine(_T("***ACULINK: ACL_LT_SERIAL_WAVE COM1"));
		}
		
#endif
	}
	else if(		getModel()->getCONFIG()->GetPDMSprotocol()==ACL_ETHERNET_WAVE
		&&	getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setLinkType(ACL_LT_ETHERNET_WAVE);
		getModel()->getAcuLink()->setShutdown(0);

		UINT uiPort=getModel()->getCONFIG()->GetEthernetPort();
		if(uiPort)
			getModel()->getAcuLink()->setNetPort(uiPort);

		//getModel()->getAcuLink()->SetLanguage(m_wLanguageID);

		PROCESS_INFORMATION pi;
		CreateProcess(_T("\\FFSDISK\\AcuLink.exe"), L"COM2", NULL, NULL, NULL, 0, NULL, NULL, NULL, &pi);
		
		theApp.getLog()->WriteLine(_T("***ACULINK: ACL_LT_ETHERNET_WAVE"));
	}

	if(getModel()->getCONFIG()->GetAcuLinkVersion()==ALINKVERS_3)
	{
		theApp.getLog()->WriteLine(_T("***ACULINK: version 3.x"));
	}
	else
	{
		theApp.getLog()->WriteLine(_T("***ACULINK: version 4.x"));
	}

	if(getModel()->getAcuLink()!=NULL)
	{
		if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_UNIT_PRESSURE,0);
		}
		else
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_UNIT_PRESSURE,1);
		}

		if(getModel()->getDATAHANDLER()->IsAccuSupply()==true)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_POWERSTATE,1);
		}
		else
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_POWERSTATE,0);
		}

		int iVersion[4];

		CString resToken;
		int curPos=0;
		int nIdx=0;

		CStringW szBuildVersion=getModel()->GetBuildVersion();

		resToken= szBuildVersion.Tokenize(_T("."),curPos);
		while (resToken != _T(""))
		{
			iVersion[nIdx]=_ttoi(resToken);

			resToken = szBuildVersion.Tokenize(_T("."), curPos);
			nIdx++;
		}; 

		getModel()->getAcuLink()->setParaData(ALINK_SETT_VERSION_MMI_MAJOR,iVersion[0]);
		getModel()->getAcuLink()->setParaData(ALINK_SETT_VERSION_MMI_MINOR,iVersion[1]);
		getModel()->getAcuLink()->setParaData(ALINK_SETT_VERSION_MMI_PATCH,iVersion[2]);
		getModel()->getAcuLink()->setParaData(ALINK_SETT_VERSION_MMI_BUILD,iVersion[3]);

		CStringA szUnitID=getModel()->GetUniqueID();

		CString szHex=_T("");
		szHex+=szUnitID;

		BYTE byAddress[6] = {'\0'};

		int iCount=0;
		for(int i=0;i<6;i++)
		{
			CString szTemp=szHex.Mid( iCount, 2 );
			byAddress[i]=getModel()->_httoi(szTemp);
			iCount+=2;
		}

		int iHigh=byAddress[0] << 16;
		iHigh+=byAddress[1] << 8;
		iHigh+=byAddress[2] ;

		int iLow=byAddress[3] << 16;
		iLow+=byAddress[4] << 8;
		iLow+=byAddress[5] ;

		getModel()->getAcuLink()->setParaData(ALINK_SETT_DEVICEID_CATEGORY,iHigh);
		getModel()->getAcuLink()->setParaData(ALINK_SETT_DEVICEID_VARIANT,iLow);

		MAINBOARD_DATA dataMainboard;
		getModel()->getDATAHANDLER()->getMainboardData(&dataMainboard);

		CString csConductor=dataMainboard.m_szConductorVersion;

		curPos=0;
		nIdx=0;

		resToken= csConductor.Tokenize(_T("."),curPos);
		while (resToken != _T(""))
		{
			iVersion[nIdx]=_ttoi(resToken);

			resToken = csConductor.Tokenize(_T("."), curPos);
			nIdx++;
		}; 

		getModel()->getAcuLink()->setParaData(ALINK_SETT_VERSION_CTRLPIC_MAJOR,iVersion[0]);
		getModel()->getAcuLink()->setParaData(ALINK_SETT_VERSION_CTRLPIC_MINOR,iVersion[1]);
		getModel()->getAcuLink()->setParaData(ALINK_SETT_VERSION_CTRLPIC_PATCH,iVersion[2]);

		getModel()->getAcuLink()->setParaData(ALINK_SETT_VERSION_MONPIC_MAJOR,dataMainboard.m_iMonitorVersion_x);
		getModel()->getAcuLink()->setParaData(ALINK_SETT_VERSION_MONPIC_MINOR,dataMainboard.m_iMonitorVersion_y);
		getModel()->getAcuLink()->setParaData(ALINK_SETT_VERSION_MONPIC_PATCH,dataMainboard.m_iMonitorVersion_z);

		eVentSilentState silentState = getModel()->getALARMHANDLER()->getAlarmSilentState();
		if(silentState==ASTATE_ACTIVE)
		{
			getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_ENABLED,1);
		}
		else
		{
			getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_ENABLED,0);
		}


		getModel()->getAcuLink()->setParaData(ALINK_SETT_PERSID,getModel()->getCONFIG()->GetPatientID());
	}

	if(getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE)
	{
		if(getModel()->getAcuLink()!=NULL)
		{
			getModel()->getAcuLink()->setParaData(ALINK_SETT_UNIT_CO2,(int)getModel()->getCONFIG()->GetCO2unit());
			//getAcuLink()->setParaData(ALINK_SETT_O2COMPENSATION_CO2,getCONFIG()->GetO2Compensation());
			if(getModel()->getCONFIG()->getCO2module()==CO2MODULE_MICROPOD)
			{
				getModel()->getAcuLink()->setParaData(ALINK_SETT_O2COMPENSATION_CO2, ALINK_NOTVALID);
			}
			else
			{
				if(getModel()->isO2FlushActive())
					getModel()->getAcuLink()->setParaData(ALINK_SETT_O2COMPENSATION_CO2,getModel()->getDATAHANDLER()->PARADATA()->GetO2FlushPara());
				else
					getModel()->getAcuLink()->setParaData(ALINK_SETT_O2COMPENSATION_CO2,getModel()->getDATAHANDLER()->PARADATA()->GetO2Para());
			}
			

			getModel()->getAcuLink()->setParaData(ALINK_SETT_BAROPRESSURE_CO2,getModel()->getCONFIG()->GetCO2BaroPressure());

		}
	}
	else if(getModel()->getAcuLink()!=NULL)
	{
		getModel()->getAcuLink()->setParaData(ALINK_SETT_UNIT_CO2,ALINK_NOTVALID);
		getModel()->getAcuLink()->setParaData(ALINK_SETT_O2COMPENSATION_CO2,ALINK_NOTVALID);
		getModel()->getAcuLink()->setParaData(ALINK_SETT_BAROPRESSURE_CO2,ALINK_NOTVALID);
	}

	/*Sleep(1000);
	getModel()->getDATAHANDLER()->setAcuLinkStarted();*/

#endif
}

/**********************************************************************************************//**
 * Query if 'oldTickCount' is safe tick count delay expired
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	oldTickCount	Number of old ticks.
 * \param	delay			The delay.
 *
 * \return	True if safe tick count delay expired, false if not.
 **************************************************************************************************/

bool CMainFrame::isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay)////used to check if old tick count plus delay is still lower than actual tickCount, (dwLastTickCount+DELAY<getTickCount64())
{
	bool bExpired=false;
	DWORD curTickCount=GetTickCount();

	if(curTickCount<oldTickCount)//overrun
	{
		DWORD diff=0xFFFFFFFF-oldTickCount;

		if(diff+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	else
	{
		if(oldTickCount+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	return bExpired;
}