/**********************************************************************************************//**
 * \file	MVViewHandler.cpp.
 *
 * Implements the mv view handler class
 **************************************************************************************************/

#include "StdAfx.h"
#include "MVViewHandler.h"
#include "globDefs.h"

CMVViewHandler* CMVViewHandler::theViewHandler=0;

/**********************************************************************************************//**
 * Initializes a new instance of the CMVViewHandler class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CMVViewHandler::CMVViewHandler()
{
	InitializeCriticalSection(&csViewState);
	InitializeCriticalSection(&csViewDiagramm);
	InitializeCriticalSection(&csViewAlarmLimits);
	InitializeCriticalSection(&csViewDTBField);
	InitializeCriticalSection(&csViewParaBtn);
	InitializeCriticalSection(&csViewTrend);
	InitializeCriticalSection(&csViewPatData);
	InitializeCriticalSection(&csViewFullscreenMsg);
	InitializeCriticalSection(&csViewSystemAlarm);
	InitializeCriticalSection(&csViewMenu);
	InitializeCriticalSection(&csViewService);
	InitializeCriticalSection(&csViewLog);
	InitializeCriticalSection(&csViewNumeric);
	
	m_vwDTBField=NULL;
	m_vwParaBtn=NULL;
	m_vwPatData=NULL;
	m_vwStartup=NULL;
	m_vwDiagramm=NULL;
	m_vwNumericWnd=NULL;
	m_vwService=NULL;
	m_vwAlarmLimits=NULL;
	m_vwSystemAlarm=NULL;
	m_vwTrend=NULL;
	m_vwLog=NULL;
	m_vwFullscreenMsg=NULL;
	m_vwMenu=NULL;
	m_vwShutdown=NULL;

	m_vPrimaryActiveView=NULL;
	m_vCurrentFocusedView=NULL;

	m_iTrendLeft=1;
	m_iTrendRight=1;
	m_dwLastTrendMoveLeft=0;
	m_dwLastTrendCntLeft=0;
	m_dwLastTrendMoveRight=0;
	m_dwLastTrendCntRight=0;

	m_eViewState=VS_STARTUP;
	m_ePrevViewState=VS_PARA;

	m_ePrevGraphState = VS_PARA;
	m_ePrevGraphSubState = VSS_NONE;

	m_eViewSubState=VSS_NONE;
	m_ePrevViewSubState=VSS_NONE;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CMVViewHandler class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CMVViewHandler::~CMVViewHandler(void)
{
	CloseFullscreenMsgView();
	CloseStartupView();
	CloseTrendView();
	CloseSystemAlarmView();
	CloseServiceView();
	CloseLogView();
	CloseNumericView();
	CloseParaBtnView();
	CloseDiagrammView();
	CloseDTBFieldView();
	CloseAlarmLimitsView();
	ClosePatDataView();
	CloseMenuView();
	CloseShutdownView();

	DeleteCriticalSection(&csViewState);
	DeleteCriticalSection(&csViewDiagramm);
	DeleteCriticalSection(&csViewAlarmLimits);
	DeleteCriticalSection(&csViewDTBField);
	DeleteCriticalSection(&csViewParaBtn);
	DeleteCriticalSection(&csViewTrend);
	DeleteCriticalSection(&csViewPatData);
	DeleteCriticalSection(&csViewFullscreenMsg);
	DeleteCriticalSection(&csViewSystemAlarm);
	DeleteCriticalSection(&csViewMenu);
	DeleteCriticalSection(&csViewService);
	DeleteCriticalSection(&csViewLog);
	DeleteCriticalSection(&csViewNumeric);

	m_vPrimaryActiveView=NULL;
	m_vCurrentFocusedView=NULL;
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CMVViewHandler::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Destroys the instance
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::DestroyInstance()
{
	delete theViewHandler;
	theViewHandler = NULL;
}

/**********************************************************************************************//**
 * Gets the instance
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the instance.
 **************************************************************************************************/

CMVViewHandler* CMVViewHandler::GetInstance()
{
	if(theViewHandler == 0)
	{
		theViewHandler = new CMVViewHandler;
	}
	return theViewHandler;
}

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::Init()
{
	OpenStartupView();
	SetPrimaryActiveView(m_vwStartup);
}

/**********************************************************************************************//**
 * Opens trend view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bRedraw	True to redraw.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::OpenTrendView(bool bRedraw)
{
	bool result = false;

	EnterCriticalSection(&csViewTrend);
	if(!m_vwTrend)
		m_vwTrend = new CWndWrapperTrends();

	if(m_vwTrend)
	{
		result = m_vwTrend->showView(bRedraw);
	}
	LeaveCriticalSection(&csViewTrend);

	return result;
}

/**********************************************************************************************//**
 * Closes trend view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::CloseTrendView()
{
	EnterCriticalSection(&csViewTrend);
	if(m_vwTrend)
	{
		delete m_vwTrend;
	}
	m_vwTrend=NULL;
	LeaveCriticalSection(&csViewTrend);

	return true;
}

/**********************************************************************************************//**
 * Opens pattern data view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::OpenPatDataView()
{
	bool result = false;

	EnterCriticalSection(&csViewPatData);
	if(!m_vwPatData)
		m_vwPatData = new CWndWrapperPatData();

	if(m_vwPatData)
	{
		result = m_vwPatData->showView();
	}
	LeaveCriticalSection(&csViewPatData);

	return result;
}

/**********************************************************************************************//**
 * Closes pattern data view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::ClosePatDataView()
{
	EnterCriticalSection(&csViewPatData);
	if(m_vwPatData)
	{
		delete m_vwPatData;
	}
	m_vwPatData=NULL;
	LeaveCriticalSection(&csViewPatData);

	return true;
}

/**********************************************************************************************//**
 * Opens fullscreen message view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::OpenFullscreenMsgView()
{
	bool result = false;

	EnterCriticalSection(&csViewFullscreenMsg);
	if(!m_vwFullscreenMsg)
		m_vwFullscreenMsg = new CWndWrapperFullscreenMsg();

	if(m_vwFullscreenMsg)
	{
		result = m_vwFullscreenMsg->showView();
	}
	LeaveCriticalSection(&csViewFullscreenMsg);

	return result;
}

/**********************************************************************************************//**
 * Closes fullscreen message view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::CloseFullscreenMsgView()
{
	EnterCriticalSection(&csViewFullscreenMsg);
	if(m_vwFullscreenMsg)
	{
		delete m_vwFullscreenMsg;
	}
	m_vwFullscreenMsg=NULL;
	LeaveCriticalSection(&csViewFullscreenMsg);

	return true;
}

/**********************************************************************************************//**
 * Opens startup view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::OpenStartupView()
{
	bool result = false;

	if(!m_vwStartup)
		m_vwStartup = new CWndWrapperStartup();

	if(m_vwStartup)
	{
		result = m_vwStartup->showView();
	}

	return result;
}

/**********************************************************************************************//**
 * Closes startup view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::CloseStartupView()
{
	if(m_vwStartup)
	{
		delete m_vwStartup;
	}
	m_vwStartup=NULL;

	return true;
}

/**********************************************************************************************//**
 * Opens shutdown view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::OpenShutdownView()
{
	bool result = false;

	if(!m_vwShutdown)
		m_vwShutdown = new CWndWrapperShutdown();

	if(m_vwShutdown)
	{
		result = m_vwShutdown->showView();
	}

	return result;
}

/**********************************************************************************************//**
 * Closes shutdown view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::CloseShutdownView()
{
	if(m_vwShutdown)
	{
		delete m_vwShutdown;
	}
	m_vwShutdown=NULL;

	return true;
}


/**********************************************************************************************//**
 * Opens system alarm view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::OpenSystemAlarmView()
{
	bool result = false;

	EnterCriticalSection(&csViewSystemAlarm);
	if(!m_vwSystemAlarm)
		m_vwSystemAlarm = new CWndWrapperSystemAlarm();

	if(m_vwSystemAlarm)
	{
		result = m_vwSystemAlarm->showView();
	}
	LeaveCriticalSection(&csViewSystemAlarm);

	return result;
}

/**********************************************************************************************//**
 * Closes system alarm view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::CloseSystemAlarmView()
{
	EnterCriticalSection(&csViewSystemAlarm);
	if(m_vwSystemAlarm)
	{
		delete m_vwSystemAlarm;
	}
	m_vwSystemAlarm=NULL;
	LeaveCriticalSection(&csViewSystemAlarm);

	return true;
}



/**********************************************************************************************//**
 * Opens menu view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bRedraw	True to redraw.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::OpenMenuView(bool bRedraw)
{
	bool result = false;

	EnterCriticalSection(&csViewMenu);
	if(!m_vwMenu)
		m_vwMenu = new CWndWrapperMenu();

	if(m_vwMenu)
	{
		result = m_vwMenu->showView(bRedraw);
	}
	LeaveCriticalSection(&csViewMenu);

	/*MEMORYSTATUS stat;
	GlobalMemoryStatus (&stat);
	DWORD dwMem=stat.dwAvailPhys/1024;

	CString szMem=_T("");
	szMem.Format(_T("OpenMenuView %*ld free "),7, stat.dwAvailPhys/1024);
	DEBUGMSG(TRUE, (szMem));
	DEBUGMSG(TRUE, (TEXT("\r\n")));*/
	//theApp.WriteLog(szMem);
	// 
	return result;
}

/**********************************************************************************************//**
 * Closes menu view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::CloseMenuView()
{
	EnterCriticalSection(&csViewMenu);
	if(m_vwMenu)
	{
		delete m_vwMenu;
	}
	m_vwMenu=NULL;
	LeaveCriticalSection(&csViewMenu);

	return true;
}

/**********************************************************************************************//**
 * Opens service view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bRedraw	True to redraw.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::OpenServiceView(bool bRedraw)
{
	bool result = false;

	EnterCriticalSection(&csViewService);
	if(!m_vwService)
		m_vwService = new CWndWrapperService();

	if(m_vwService)
	{
		result = m_vwService->showView(bRedraw);
	}
	LeaveCriticalSection(&csViewService);

	return result;
}

/**********************************************************************************************//**
 * Closes service view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::CloseServiceView()
{
	EnterCriticalSection(&csViewService);
	if(m_vwService)
	{
		delete m_vwService;
	}
	m_vwService=NULL;
	LeaveCriticalSection(&csViewService);

	return true;
}



/**********************************************************************************************//**
 * Opens log view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::OpenLogView()
{
	bool result = false;

	EnterCriticalSection(&csViewLog);
	if(!m_vwLog)
		m_vwLog = new CWndWrapperAlarmLog();

	if(m_vwLog)
	{
		result = m_vwLog->showView();
	}
	LeaveCriticalSection(&csViewLog);

	return result;
}

/**********************************************************************************************//**
 * Closes log view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::CloseLogView()
{
	EnterCriticalSection(&csViewLog);
	if(m_vwLog)
	{
		delete m_vwLog;
	}
	m_vwLog=NULL;
	LeaveCriticalSection(&csViewLog);

	return true;
}

/**********************************************************************************************//**
 * Opens numeric view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::OpenNumericView()
{
	bool result = false;

	EnterCriticalSection(&csViewNumeric);
	if(!m_vwNumericWnd)
		m_vwNumericWnd = new CWndWrapperNumerics();

	if(m_vwNumericWnd)
	{
		result = m_vwNumericWnd->showView();
	}
	LeaveCriticalSection(&csViewNumeric);

	return result;
}

/**********************************************************************************************//**
 * Closes numeric view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::CloseNumericView()
{
	EnterCriticalSection(&csViewNumeric);
	if(m_vwNumericWnd)
	{
		delete m_vwNumericWnd;
	}
	m_vwNumericWnd=NULL;
	LeaveCriticalSection(&csViewNumeric);

	return true;
}

/**********************************************************************************************//**
 * Opens para button view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::OpenParaBtnView()
{
	bool result = false;

	EnterCriticalSection(&csViewParaBtn);
	if(!m_vwParaBtn)
		m_vwParaBtn = new CWndWrapperParaBtn();

	if(m_vwParaBtn)
	{
		result = m_vwParaBtn->showView();
	}
	LeaveCriticalSection(&csViewParaBtn);

	/*MEMORYSTATUS stat;
	GlobalMemoryStatus (&stat);
	DWORD dwMem=stat.dwAvailPhys/1024;

	CString szMem=_T("");
	szMem.Format(_T("OpenParaBtnView %*ld free "),7, stat.dwAvailPhys/1024);
	DEBUGMSG(TRUE, (szMem));
	DEBUGMSG(TRUE, (TEXT("\r\n")));*/
	//theApp.WriteLog(szMem);

	return result;
}

/**********************************************************************************************//**
 * Closes para button view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::CloseParaBtnView()
{
	EnterCriticalSection(&csViewParaBtn);
	if(m_vwParaBtn)
	{
		delete m_vwParaBtn;
	}
	m_vwParaBtn=NULL;
	LeaveCriticalSection(&csViewParaBtn);

	return true;
}

/**********************************************************************************************//**
 * Opens diagramm view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::OpenDiagrammView()
{
	bool result = false;

	EnterCriticalSection(&csViewDiagramm);
	if(!m_vwDiagramm)
		m_vwDiagramm = new CWndWrapperDiagramms();

	if(m_vwDiagramm)
	{
		result = m_vwDiagramm->showView();
	}
	LeaveCriticalSection(&csViewDiagramm);

	return result;
}

/**********************************************************************************************//**
 * Closes diagramm view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::CloseDiagrammView()
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vwDiagramm)
	{
		delete m_vwDiagramm;
	}
	m_vwDiagramm=NULL;
	LeaveCriticalSection(&csViewDiagramm);

	return true;
}

/**********************************************************************************************//**
 * Opens dtb field view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::OpenDTBFieldView()
{
	bool result = false;

	EnterCriticalSection(&csViewDTBField);
	if(!m_vwDTBField)
		m_vwDTBField = new CWndWrapperDTBField();

	if(m_vwDTBField)
	{
		result = m_vwDTBField->showView();
	}
	LeaveCriticalSection(&csViewDTBField);

	/*MEMORYSTATUS stat;
	GlobalMemoryStatus (&stat);
	DWORD dwMem=stat.dwAvailPhys/1024;

	CString szMem=_T("");
	szMem.Format(_T("OpenDTBFieldView %*ld free "),7, stat.dwAvailPhys/1024);
	DEBUGMSG(TRUE, (szMem));
	DEBUGMSG(TRUE, (TEXT("\r\n")));*/
	//theApp.WriteLog(szMem);
	// 
	return result;
}

/**********************************************************************************************//**
 * Closes dtb field view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::CloseDTBFieldView()
{
	EnterCriticalSection(&csViewDTBField);
	if(m_vwDTBField)
	{
		delete m_vwDTBField;
	}
	m_vwDTBField=NULL;
	LeaveCriticalSection(&csViewDTBField);

	return true;
}

/**********************************************************************************************//**
 * Opens alarm limits view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::OpenAlarmLimitsView()
{
	bool result = false;

	EnterCriticalSection(&csViewAlarmLimits);
	if(!m_vwAlarmLimits)
		m_vwAlarmLimits = new CWndWrapperAlarmLimits();

	if(m_vwAlarmLimits)
	{
		result = m_vwAlarmLimits->showView();
	}
	LeaveCriticalSection(&csViewAlarmLimits);

	return result;
}

/**********************************************************************************************//**
 * Closes alarm limits view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVViewHandler::CloseAlarmLimitsView()
{
	EnterCriticalSection(&csViewAlarmLimits);
	if(m_vwAlarmLimits)
	{
		delete m_vwAlarmLimits;
	}
	m_vwAlarmLimits=NULL;
	LeaveCriticalSection(&csViewAlarmLimits);

	return true;
}

/**********************************************************************************************//**
 * Draw views
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CMVViewHandler::DrawViews(bool bRedraw)
{
	switch(getViewState())
	{
	case VS_SHUTDOWN:
		{ 
			drawView_SHUTDOWN();
		}
		break;
	case VS_SYSFAIL:
		{
			drawView_SYSFAIL();
		}
		break;
	case VS_BATTERY:
		{
			drawView_BATTERY();
		}
		break;
	case VS_PATDATA:
		{
			drawView_PATDATA();
		}
		break;
	case VS_TREND:
		{
			drawView_TREND(bRedraw);
		}
		break;
	case VS_PARA:
		{
			drawView_PARA(true);
		}
		break;
	case VS_GRAPH:
		{
			drawView_GRAPH();
		}
		break;
	case VS_SERVICE:
		{
			drawView_SERVICE(bRedraw);
		}
		break;
	case VS_ALARM_SYS:
		{
			drawView_ALARM_SYS();
		}
		break;
	case VS_SETUP:
		{
			drawView_SETUP(bRedraw);
		}
		break;
	case VS_ALARM_LIMIT:
		{
			drawView_ALARM_LIMIT(true);
		}
		break;
	default:
		{
			SetPrimaryActiveView(NULL);
		}
		break;
	}
}

/**********************************************************************************************//**
 * Draw view shutdown
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::drawView_SHUTDOWN()
{
	//closeNebulizer();

	CloseSystemAlarmView();
	CloseStartupView();
	CloseDiagrammView();
	CloseParaBtnView();
	CloseNumericView();
	CloseMenuView();
	CloseServiceView();
	CloseAlarmLimitsView();
	CloseTrendView();
	CloseLogView();
	ClosePatDataView();
	CloseDTBFieldView();
	CloseFullscreenMsgView();

	if(AfxGetApp()->GetMainWnd())
		AfxGetApp()->GetMainWnd()->UpdateWindow();
	OpenShutdownView();

	SetPrimaryActiveView(m_vwShutdown);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();
}

/**********************************************************************************************//**
 * Draw view sysfail
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::drawView_SYSFAIL()
{
	//closeNebulizer();

	CloseSystemAlarmView();
	CloseStartupView();
	CloseDiagrammView();
	CloseParaBtnView();
	CloseNumericView();
	CloseMenuView();
	CloseServiceView();
	CloseAlarmLimitsView();
	CloseTrendView();
	CloseLogView();
	ClosePatDataView();

	if(AfxGetApp()->GetMainWnd())
		AfxGetApp()->GetMainWnd()->UpdateWindow();
	OpenDTBFieldView();
	OpenFullscreenMsgView();

	SetPrimaryActiveView(m_vwFullscreenMsg);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();
}

/**********************************************************************************************//**
 * Draw view battery
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::drawView_BATTERY()
{
	//closeNebulizer();

	CloseSystemAlarmView();
	CloseStartupView();
	CloseDiagrammView();
	CloseParaBtnView();
	CloseNumericView();
	CloseMenuView();
	CloseServiceView();
	CloseAlarmLimitsView();
	CloseTrendView();
	CloseLogView();
	ClosePatDataView();

	if(AfxGetApp()->GetMainWnd())
		AfxGetApp()->GetMainWnd()->UpdateWindow();
	OpenDTBFieldView();
	OpenFullscreenMsgView();

	SetPrimaryActiveView(m_vwFullscreenMsg);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();
}

/**********************************************************************************************//**
 * Draw view patdata
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::drawView_PATDATA()
{
	//closeNebulizer();

	CloseSystemAlarmView();
	CloseStartupView();
	CloseDiagrammView();
	CloseParaBtnView();
	CloseNumericView();
	CloseMenuView();
	CloseServiceView();
	CloseAlarmLimitsView();
	CloseTrendView();
	CloseLogView();
	CloseFullscreenMsgView();

	if(AfxGetApp()->GetMainWnd())
		AfxGetApp()->GetMainWnd()->UpdateWindow();
	OpenDTBFieldView();
	OpenPatDataView();

	getModel()->getALARMHANDLER()->setAutoSilent(true,true,false);

	SetPrimaryActiveView(m_vwPatData);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();
}

/**********************************************************************************************//**
 * Draw view trend
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CMVViewHandler::drawView_TREND(bool bRedraw)
{
	//closeNebulizer();

	CloseSystemAlarmView();
	CloseStartupView();
	CloseDiagrammView();
	CloseParaBtnView();
	CloseMenuView();
	CloseServiceView();
	CloseAlarmLimitsView();
	CloseLogView();
	ClosePatDataView();
	CloseFullscreenMsgView();

	if(AfxGetApp()->GetMainWnd())
		AfxGetApp()->GetMainWnd()->UpdateWindow();
	OpenDTBFieldView();
	OpenNumericView();
	OpenTrendView(bRedraw);
	
	SetPrimaryActiveView(m_vwTrend);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();

}

/**********************************************************************************************//**
 * Draw view para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CMVViewHandler::drawView_PARA(bool bRedraw)
{
	//closeNebulizer();

	CloseSystemAlarmView();
	CloseStartupView();
	CloseMenuView();
	CloseServiceView();
	CloseAlarmLimitsView();
	CloseTrendView();
	CloseLogView();
	ClosePatDataView();
	CloseFullscreenMsgView();

	if(AfxGetApp()->GetMainWnd())
		AfxGetApp()->GetMainWnd()->UpdateWindow();
	OpenParaBtnView();
	OpenDTBFieldView();
	OpenNumericView();
	OpenDiagrammView();
	//theApp.getLog()->Write(_T("#4"));

	switch(getPrevViewState())
	{
	case VS_STARTUP:
		{
			getModel()->getALARMHANDLER()->setAutoSilent(true,true,false);
		}
		break;
	case VS_PATDATA:
		{
			getModel()->getALARMHANDLER()->setAutoSilent(true,true,false);
		}
		break;
	case VS_TREND:
	case VS_ALARM_SYS:
	case VS_SERVICE:
	case VS_GRAPH:
	case VS_PARA:
	case VS_SETUP:
	case VS_ALARM_LIMIT:
	case VS_BATTERY:
	case VS_SYSFAIL:
	default:
		{

		}
		break;
	}

	SetPrimaryActiveView(m_vwParaBtn);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();
}

/**********************************************************************************************//**
 * Draw view graph
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::drawView_GRAPH()
{
	//closeNebulizer();

	CloseSystemAlarmView();
	CloseStartupView();
	CloseParaBtnView();
	CloseMenuView();
	CloseServiceView();
	CloseAlarmLimitsView();
	CloseTrendView();
	CloseLogView();
	ClosePatDataView();
	CloseFullscreenMsgView();
	
	if(AfxGetApp()->GetMainWnd())
		AfxGetApp()->GetMainWnd()->UpdateWindow();
	OpenDTBFieldView();
	OpenNumericView();
	OpenDiagrammView();

	SetPrimaryActiveView(m_vwDiagramm);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();
}

/**********************************************************************************************//**
 * Draw view service
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CMVViewHandler::drawView_SERVICE(bool bRedraw)
{
	//closeNebulizer();

	CloseSystemAlarmView();
	CloseStartupView();
	CloseDiagrammView();
	CloseParaBtnView();
	CloseNumericView();
	CloseMenuView();
	CloseAlarmLimitsView();
	CloseTrendView();
	CloseLogView();
	ClosePatDataView();
	CloseFullscreenMsgView();

	if(AfxGetApp()->GetMainWnd())
		AfxGetApp()->GetMainWnd()->UpdateWindow();
	OpenDTBFieldView();
	OpenServiceView(bRedraw);

	SetPrimaryActiveView(m_vwService);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_SETFOCUS_PIMARYVIEW);
}

/**********************************************************************************************//**
 * Draw view alarm system
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::drawView_ALARM_SYS()
{
	//closeNebulizer();

	CloseStartupView();
	CloseDiagrammView();
	CloseParaBtnView();
	CloseNumericView();
	CloseMenuView();
	CloseServiceView();
	CloseAlarmLimitsView();
	CloseTrendView();
	CloseLogView();
	ClosePatDataView();
	CloseFullscreenMsgView();

	if(AfxGetApp()->GetMainWnd())
		AfxGetApp()->GetMainWnd()->UpdateWindow();
	OpenDTBFieldView();
	OpenSystemAlarmView();

	SetPrimaryActiveView(m_vwSystemAlarm);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_SETFOCUS_PIMARYVIEW);
}

/**********************************************************************************************//**
 * Draw view setup
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CMVViewHandler::drawView_SETUP(bool bRedraw)
{
	//closeNebulizer();

	CloseSystemAlarmView();
	CloseStartupView();
	CloseDiagrammView();
	CloseParaBtnView();
	CloseNumericView();
	CloseServiceView();
	CloseAlarmLimitsView();
	CloseTrendView();
	CloseLogView();
	ClosePatDataView();
	CloseFullscreenMsgView();

	if(AfxGetApp()->GetMainWnd())
		AfxGetApp()->GetMainWnd()->UpdateWindow();
	OpenDTBFieldView();

	OpenMenuView(bRedraw);
	SetPrimaryActiveView(m_vwMenu);

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_SETFOCUS_PIMARYVIEW);
}

/**********************************************************************************************//**
 * Draw view alarm limit
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CMVViewHandler::drawView_ALARM_LIMIT(bool bRedraw)
{
	//closeNebulizer();

	CloseSystemAlarmView();
	CloseStartupView();
	CloseNumericView();
	CloseMenuView();
	CloseServiceView();
	CloseTrendView();
	ClosePatDataView();
	CloseFullscreenMsgView();

	if(getViewSubState()==VSS_ALIMIT_LOGLIST)
	{
		if(m_vwParaBtn)
			CloseParaBtnView();
		if(AfxGetApp()->GetMainWnd())
			AfxGetApp()->GetMainWnd()->UpdateWindow();
		OpenLogView();
	}
	else
	{
		CloseLogView();
		if(AfxGetApp()->GetMainWnd())
			AfxGetApp()->GetMainWnd()->UpdateWindow();
		OpenParaBtnView();
	}


	OpenDiagrammView();
	OpenDTBFieldView();
	OpenAlarmLimitsView();

	SetPrimaryActiveView(m_vwAlarmLimits);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_SETFOCUS_PIMARYVIEW);
}

/**********************************************************************************************//**
 * View state changed
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::viewStateChanged()
{
	DrawViews(false);
}

//void CMVViewHandler::VentModeChanged()
//{
//	//theApp.WriteLog(_T("CMVViewHandler::NotifyVentModeChanged"));
//	if(AfxGetApp()->GetMainWnd())
//		AfxGetApp()->GetMainWnd()->SetFocus();
//	//SetCurrentFocusedView(GetPrimaryActiveView());
//}

/**********************************************************************************************//**
 * Sets ppsv value
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iValPPSV	Zero-based index of the value ppsv.
 * \param	bSend   	True to send.
 **************************************************************************************************/

void CMVViewHandler::setPpsvValue(int iValPPSV,bool bSend)
{
	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vwParaBtn)
		m_vwParaBtn->setPpsvValue(iValPPSV,bSend);
	LeaveCriticalSection(&csViewParaBtn);
}

/**********************************************************************************************//**
 * Sets pee pvalue
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iValPEEP	Zero-based index of the value peep.
 * \param	bSend   	True to send.
 **************************************************************************************************/

void CMVViewHandler::setPEEPvalue(int iValPEEP,bool bSend)
{
	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vwParaBtn)
		m_vwParaBtn->setPEEPvalue(iValPEEP,bSend);
	LeaveCriticalSection(&csViewParaBtn);
}

/**********************************************************************************************//**
 * Sets pmean difference
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iValPmean	Zero-based index of the value pmean.
 * \param	bSend	 	True to send.
 **************************************************************************************************/

void CMVViewHandler::setPmeanDifference(int iValPmean,bool bSend)//PMAN1
{
	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vwParaBtn)
		m_vwParaBtn->setPmeanDiff(iValPmean,bSend);
	LeaveCriticalSection(&csViewParaBtn);
}

/**********************************************************************************************//**
 * Sets pmean record difference
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iValPmeanRec	Zero-based index of the value pmean record.
 * \param	bSend			True to send.
 **************************************************************************************************/

void CMVViewHandler::setPmeanRecDifference(int iValPmeanRec,bool bSend)//PMAN1
{
	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vwParaBtn)
		m_vwParaBtn->setPmeanRecDiff(iValPmeanRec,bSend);
	LeaveCriticalSection(&csViewParaBtn);
}

/**********************************************************************************************//**
 * Updates the co 2 information data described by resetAvailable
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	resetAvailable	True if reset available.
 **************************************************************************************************/

void CMVViewHandler::UpdateCO2InfoData(bool resetAvailable)
{
	//rku cs1
	if(resetAvailable)
	{
		EnterCriticalSection(&csViewDiagramm);
		if(m_vwDiagramm)
			m_vwDiagramm->PostMessage(WM_UPDATECO2INFODATA_TRUE);
		LeaveCriticalSection(&csViewDiagramm);
	}
	else
	{
		EnterCriticalSection(&csViewDiagramm);
		if(m_vwDiagramm)
			m_vwDiagramm->PostMessage(WM_UPDATECO2INFODATA_FALSE);
		LeaveCriticalSection(&csViewDiagramm);
	}
}

//void CMVViewHandler::CO2PumpOn()
//{
//	if(m_vwDiagramm)
//		m_vwDiagramm->CO2PumpOn();
//}

/**********************************************************************************************//**
 * Gets primary active view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the primary active view.
 **************************************************************************************************/

CWndWrapper* CMVViewHandler::GetPrimaryActiveView()
{
	return m_vPrimaryActiveView;
}

/**********************************************************************************************//**
 * Sets primary active view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pView	If non-null, the view.
 **************************************************************************************************/

void CMVViewHandler::SetPrimaryActiveView(CWndWrapper *pView)
{
	m_vPrimaryActiveView=pView;
}

/**********************************************************************************************//**
 * Gets current focused view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the current focused view.
 **************************************************************************************************/

CWndWrapper* CMVViewHandler::GetCurrentFocusedView()
{
	return m_vCurrentFocusedView;
}

/**********************************************************************************************//**
 * Sets current focused view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pView	If non-null, the view.
 **************************************************************************************************/

void CMVViewHandler::SetCurrentFocusedView(CWndWrapper *pView)
{
	//DEBUGMSG(TRUE, (TEXT("CMVViewHandler::SetCurrentFocusedView()\r\n")));
	m_vCurrentFocusedView=pView;

	if(m_vCurrentFocusedView!=NULL)
		getModel()->NotifyViewFocusChanged(m_vCurrentFocusedView->GetDlgCtrlID());
	else
		getModel()->NotifyViewFocusChanged(0);
}

/**********************************************************************************************//**
 * Updates the service view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::UpdateServiceView()
{
	if(GetPrimaryActiveView()!=NULL)
	{
		GetPrimaryActiveView()->PostMessage(WM_SETVIEWFOCUS);
		SetCurrentFocusedView(GetPrimaryActiveView());

		int iT = 0;
		if(GetCurrentFocusedView()!=NULL)
			iT = GetCurrentFocusedView()->GetDlgCtrlID();

		if(iT==IDC_VIEW_SERVICE)
		{
			//rku cs1
			EnterCriticalSection(&csViewService);
			if(m_vwService)
			{
				m_vwService->PostMessage(WM_UPDATE_VIEW);
			}
			LeaveCriticalSection(&csViewService);
		}
	}
}

/**********************************************************************************************//**
 * Sets focus to primary view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::SetFocusToPrimaryView()
{
	if(GetPrimaryActiveView()!=NULL)
	{
		GetPrimaryActiveView()->PostMessage(WM_SETVIEWFOCUS);
		SetCurrentFocusedView(GetPrimaryActiveView());

		int iT = 0;
		if(GetCurrentFocusedView()!=NULL)
			iT = GetCurrentFocusedView()->GetDlgCtrlID();

		if(iT==IDC_VIEW_PARABTN)
		{
			//rku cs1
			EnterCriticalSection(&csViewDiagramm);
			if(m_vwDiagramm)
			{
				m_vwDiagramm->PostMessage(WM_GRAPH_LOSTFOCUS);
			}
			LeaveCriticalSection(&csViewDiagramm);

		}
		else if(iT==IDC_VIEW_TREND)
		{
			//rku cs1
			EnterCriticalSection(&csViewTrend);
			if(m_vwTrend)
			{
				m_vwTrend->PostMessage(WM_TREND_LOSTFOCUS);
			}
			LeaveCriticalSection(&csViewTrend);
		}
	}
	else
	{
		switch(getViewState())
		{
		case VS_SHUTDOWN:
			{
				SetPrimaryActiveView(m_vwShutdown);
				m_vwShutdown->PostMessage(WM_SETVIEWFOCUS);
			}
			break;
		case VS_SYSFAIL:
			{
				//rku cs1
				EnterCriticalSection(&csViewFullscreenMsg);
				if(m_vwFullscreenMsg)
				{
					SetPrimaryActiveView(m_vwFullscreenMsg);
					m_vwFullscreenMsg->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewFullscreenMsg);
			}
			break;
		case VS_BATTERY:
			{
				//rku cs1
				EnterCriticalSection(&csViewFullscreenMsg);
				if(m_vwFullscreenMsg)
				{
					SetPrimaryActiveView(m_vwFullscreenMsg);
					m_vwFullscreenMsg->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewFullscreenMsg);
			}
			break;
		case VS_PATDATA:
			{
				if(m_vwPatData)
				{
					SetPrimaryActiveView(m_vwPatData);
					m_vwPatData->PostMessage(WM_SETVIEWFOCUS);
				}
			}
			break;
		case VS_TREND:
			{
				//rku cs1
				EnterCriticalSection(&csViewTrend);
				if(m_vwTrend)
				{
					SetPrimaryActiveView(m_vwTrend);
					m_vwTrend->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewTrend);
			}
			break;
		case VS_PARA:
			{
				//rku cs1
				EnterCriticalSection(&csViewParaBtn);
				if(m_vwParaBtn)
				{
					SetPrimaryActiveView(m_vwParaBtn);
					m_vwParaBtn->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewParaBtn);
			}
			break;
		case VS_GRAPH:
			{
				//rku cs1
				EnterCriticalSection(&csViewDiagramm);
				if(m_vwDiagramm)
				{
					SetPrimaryActiveView(m_vwDiagramm);
					m_vwDiagramm->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewDiagramm);
			}
			break;
		case VS_SERVICE:
			{
				//rku cs1
				EnterCriticalSection(&csViewService);
				if(m_vwService)
				{
					SetPrimaryActiveView(m_vwService);
					m_vwService->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewService);
			}
			break;
		case VS_ALARM_SYS:
			{
				//rku cs1
				EnterCriticalSection(&csViewSystemAlarm);
				if(m_vwSystemAlarm)
				{
					SetPrimaryActiveView(m_vwSystemAlarm);
					m_vwSystemAlarm->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewSystemAlarm);

				theApp.WriteLog(_T("*************Focus VS_ALARM_SYS"));
			}
			break;
		case VS_SETUP:
			{
				//rku cs1
				EnterCriticalSection(&csViewMenu);
				if(m_vwMenu)
				{
					SetPrimaryActiveView(m_vwMenu);
					m_vwMenu->SetViewFocus();
				}
				LeaveCriticalSection(&csViewMenu);

				theApp.WriteLog(_T("*************Focus VS_SETUP"));
			}
			break;
		/*case CMVModel::VS_SELNUM:
			{
			
				SetPrimaryActiveView(m_vSelectNumeric);
				m_vSelectNumeric->SetViewFocus();
			}
			break;*/
		case VS_ALARM_LIMIT:
			{
				//rku cs1
				EnterCriticalSection(&csViewAlarmLimits);
				if(m_vwAlarmLimits)
				{
					SetPrimaryActiveView(m_vwAlarmLimits);
					m_vwAlarmLimits->SetViewFocus();
				}
				LeaveCriticalSection(&csViewAlarmLimits);
			}
			break;
		default:
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->SetFocus();
				theApp.WriteLog(_T("*************Focus default"));
			}
			break;
		}
	}
}

/**********************************************************************************************//**
 * Sets view vkup
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::SetViewVKUP()
{
	switch(getViewState())
	{
	case VS_ALARM_LIMIT:
		{
			if(getViewSubState()==VSS_ALIMIT_LOGLIST)
			{
				//rku cs1
				EnterCriticalSection(&csViewAlarmLimits);
				if(m_vwAlarmLimits)
					m_vwAlarmLimits->PostMessage(WM_SET_ALIMITTIMER);
				LeaveCriticalSection(&csViewAlarmLimits);
			}
			else
			{
				if(getModel()->getCONFIG()->GraphIsAutoScale())
				{
					//rku cs1
					EnterCriticalSection(&csViewDiagramm);
					if(m_vwDiagramm)
						m_vwDiagramm->PostMessage(WM_GRAPH_INCREASE_SPEED);
					LeaveCriticalSection(&csViewDiagramm);
				}
			}
		}
		break;
	case VS_PARA:
	case VS_GRAPH:
		{
			if(getModel()->getCONFIG()->GraphIsAutoScale())
			{
				//rku cs1
				EnterCriticalSection(&csViewDiagramm);
				if(m_vwDiagramm)
					m_vwDiagramm->PostMessage(WM_GRAPH_INCREASE_SPEED);
				LeaveCriticalSection(&csViewDiagramm);
			}
			else if(getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
			{
				//rku cs1
				EnterCriticalSection(&csViewDiagramm);
				if(m_vwDiagramm)
					m_vwDiagramm->PostMessage(WM_GRAPH_INCREASE_SPEED);
				LeaveCriticalSection(&csViewDiagramm);
			}
			//todoFOT ????
		}
		break;
	case VS_TREND:
		{
			if(!getModel()->isSafeTickCountDelayExpired(m_dwLastTrendCntLeft, TREND_MOVECHANGE_DELAY))
			{
				break;
			}
			//rku cs1
			EnterCriticalSection(&csViewTrend);
			if(m_vwTrend)
				m_vwTrend->PostMessage(WM_TREND_MOVERIGHT,m_iTrendRight);
			LeaveCriticalSection(&csViewTrend);

			m_dwLastTrendCntRight=GetTickCount();
			m_iTrendRight=1;
		}
		break;
	default:
		{
		}
		break;
	}
}

/**********************************************************************************************//**
 * Sets view vkdown
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::SetViewVKDOWN()
{
	switch(getViewState())
	{
	case VS_ALARM_LIMIT:
		{
			if(getViewSubState()==VSS_ALIMIT_LOGLIST)
			{
				//rku cs1
				EnterCriticalSection(&csViewAlarmLimits);
				if(m_vwAlarmLimits)
					m_vwAlarmLimits->PostMessage(WM_SET_ALIMITTIMER);
				LeaveCriticalSection(&csViewAlarmLimits);
			}
			else
			{
				if(getModel()->getCONFIG()->GraphIsAutoScale())
				{
					//rku cs1
					EnterCriticalSection(&csViewDiagramm);
					if(m_vwDiagramm)
						m_vwDiagramm->PostMessage(WM_GRAPH_DECREASE_SPEED);
					LeaveCriticalSection(&csViewDiagramm);
				}
			}
		}
		break;
	case VS_PARA:
	case VS_GRAPH:
		{
			if(getModel()->getCONFIG()->GraphIsAutoScale())
			{
				//rku cs1
				EnterCriticalSection(&csViewDiagramm);
				if(m_vwDiagramm)
					m_vwDiagramm->PostMessage(WM_GRAPH_DECREASE_SPEED);
				LeaveCriticalSection(&csViewDiagramm);
			}
			else if(getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
			{
				//rku cs1
				EnterCriticalSection(&csViewDiagramm);
				if(m_vwDiagramm)
					m_vwDiagramm->PostMessage(WM_GRAPH_DECREASE_SPEED);
				LeaveCriticalSection(&csViewDiagramm);
			}
			//todoFOT ????
		}
		break;
	case VS_TREND:
		{
			if(!getModel()->isSafeTickCountDelayExpired(m_dwLastTrendCntRight, TREND_MOVECHANGE_DELAY))
			{
				break;
			}

			//rku cs1
			EnterCriticalSection(&csViewTrend);
			if(m_vwTrend)
				m_vwTrend->PostMessage(WM_TREND_MOVELEFT,m_iTrendLeft);
			LeaveCriticalSection(&csViewTrend);

			m_dwLastTrendCntLeft=GetTickCount();
			m_iTrendLeft=1;	
		}
		break;
	default:
		{
		}
		break;
	}
}

/**********************************************************************************************//**
 * Check current view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iViewID	Identifier for the view.
 **************************************************************************************************/

void CMVViewHandler::CheckCurrentView(int iViewID)
{
	switch(getViewState())
	{
	case VS_TREND:
		{

		}
		break;
	case VS_PARA:
		{
			switch(iViewID)
			{
			case IDC_VIEW_PARABTN:
				{
					SetFocusToPrimaryView();
				}
				break;
			case IDC_VIEW_GRAPH:
				{
					
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case VS_GRAPH:
		{
			switch(iViewID)
			{
			case IDC_VIEW_PARABTN:
				{
					//SetCurrentFocusedView(m_vwParaBtn);
				}
				break;
			case IDC_VIEW_GRAPH:
				{
					//rku cs1
					EnterCriticalSection(&csViewDiagramm);
					if(m_vwDiagramm)
						SetCurrentFocusedView(m_vwDiagramm);
					LeaveCriticalSection(&csViewDiagramm);
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case VS_ALARM_LIMIT:
		{
			switch(iViewID)
			{
			case IDC_VIEW_PARABTN:
				{
					//rku cs1
					EnterCriticalSection(&csViewParaBtn);
					if(m_vwParaBtn)
						SetCurrentFocusedView(m_vwParaBtn);
					LeaveCriticalSection(&csViewParaBtn);
				}
				break;
			case IDC_VIEW_ALARMLIMITS:
				{
					//rku cs1
					EnterCriticalSection(&csViewAlarmLimits);
					if(m_vwAlarmLimits)
						SetCurrentFocusedView(m_vwAlarmLimits);
					LeaveCriticalSection(&csViewAlarmLimits);
				}
				break;
			case IDC_VIEW_GRAPH:
				{
					//SetCurrentFocusedView(m_vGraph);
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	default:
		{
		}
		break;
	}
	
}

/**********************************************************************************************//**
 * Sets next focus
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::SetNextFocus()
{
	if(GetCurrentFocusedView()!=NULL)
	{
		GetCurrentFocusedView()->PostMessage(WM_SETNEXTFOCUS);
	}
}

/**********************************************************************************************//**
 * Sets previous focus
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::SetPrevFocus()
{
	if(GetCurrentFocusedView()!=NULL)
	{
		GetCurrentFocusedView()->PostMessage(WM_SETPREVFOCUS);
	}
}

/**********************************************************************************************//**
 * Sets focus to next view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::SetFocusToNextView()
{
	switch(getViewState())
	{
	case VS_TREND:
		{

		}
		break;
	case VS_PARA:
		{
			int iT = 0;
			if(GetCurrentFocusedView()!=NULL)
				iT = GetCurrentFocusedView()->GetDlgCtrlID();
			
			if(iT==IDC_VIEW_PARABTN)
			{
				//rku cs1
				EnterCriticalSection(&csViewDiagramm);
				if(m_vwDiagramm)
				{
					SetCurrentFocusedView(m_vwDiagramm);
					m_vwDiagramm->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewDiagramm);
			}
			else if(iT==IDC_VIEW_GRAPH)
			{
				//rku cs1
				EnterCriticalSection(&csViewParaBtn);
				if(m_vwParaBtn)
				{
					SetCurrentFocusedView(m_vwParaBtn);
					m_vwParaBtn->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewParaBtn);
			}
		}
		break;
	default:
		{
			SetPrimaryActiveView(NULL);
		}
		break;
	}
}

/**********************************************************************************************//**
 * Sets focus to previous view
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::SetFocusToPrevView()
{
	switch(getViewState())
	{
	case VS_TREND:
		{

		}
		break;
	case VS_PARA:
		{
			int iT = 0;
			if(GetCurrentFocusedView()!=NULL)
				iT = GetCurrentFocusedView()->GetDlgCtrlID();

			if(iT==IDC_VIEW_PARABTN)
			{
				EnterCriticalSection(&csViewDiagramm);
				if(m_vwDiagramm)
				{
					SetCurrentFocusedView(m_vwDiagramm);
					m_vwDiagramm->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewDiagramm);
			}
			else if(iT==IDC_VIEW_GRAPH)
			{
				EnterCriticalSection(&csViewParaBtn);
				if(m_vwParaBtn)
				{
					SetCurrentFocusedView(m_vwParaBtn);
					m_vwParaBtn->PostMessage(WM_SETNEXTFOCUS);
				}
				LeaveCriticalSection(&csViewParaBtn);
			}
		}
		break;
	default:
		{
			SetPrimaryActiveView(NULL);
		}
		break;
	}
}

/**********************************************************************************************//**
 * Sets oxy hourglass
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CMVViewHandler::SetOxyHourglass(bool state)
{
	if(state)
	{
		EnterCriticalSection(&csViewParaBtn);
		if(m_vwParaBtn)
			m_vwParaBtn->PostMessage(WM_SETOXYHOURGLASS_TRUE);
		LeaveCriticalSection(&csViewParaBtn);
	}
	else
	{
		EnterCriticalSection(&csViewParaBtn);
		if(m_vwParaBtn)
			m_vwParaBtn->PostMessage(WM_SETOXYHOURGLASS_FALSE);
		LeaveCriticalSection(&csViewParaBtn);
	}
}

/**********************************************************************************************//**
 * Stops a diagramm
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bStopFreeze	True to stop freeze.
 **************************************************************************************************/

void CMVViewHandler::StopDiagramm(bool bStopFreeze)
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vwDiagramm)
		m_vwDiagramm->StopDiagramm(bStopFreeze);
	LeaveCriticalSection(&csViewDiagramm);
}

/**********************************************************************************************//**
 * Sets a limittimer
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::SetALIMITTIMER()
{
	EnterCriticalSection(&csViewAlarmLimits);
	if(m_vwAlarmLimits)
		m_vwAlarmLimits->PostMessage(WM_SET_ALIMITTIMER);
	LeaveCriticalSection(&csViewAlarmLimits);
}

/**********************************************************************************************//**
 * Sets cpap backup
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CMVViewHandler::SetCPAPBackup(bool bState)
{
	if(bState)
	{
		EnterCriticalSection(&csViewDTBField);
		if(m_vwDTBField)
			m_vwDTBField->PostMessage(WM_SETCPAPBACKUP_TRUE);
		LeaveCriticalSection(&csViewDTBField);
	}
	else
	{
		EnterCriticalSection(&csViewDTBField);
		if(m_vwDTBField)
			m_vwDTBField->PostMessage(WM_SETCPAPBACKUP_FALSE);
		LeaveCriticalSection(&csViewDTBField);
	}
}

/**********************************************************************************************//**
 * Sets ps vapnoe
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CMVViewHandler::setPSVapnoe(bool bState)
{
	if(bState)
	{
		EnterCriticalSection(&csViewParaBtn);
		if(m_vwParaBtn)
			m_vwParaBtn->PostMessage(WM_SETPSVAPNOE_TRUE);
		LeaveCriticalSection(&csViewParaBtn);

		EnterCriticalSection(&csViewDTBField);
		if(m_vwDTBField)
			m_vwDTBField->PostMessage(WM_SETPSVAPNOE_TRUE);
		LeaveCriticalSection(&csViewDTBField);
	}
	else
	{
		EnterCriticalSection(&csViewParaBtn);
		if(m_vwParaBtn)
			m_vwParaBtn->PostMessage(WM_SETPSVAPNOE_FALSE);
		LeaveCriticalSection(&csViewParaBtn);

		EnterCriticalSection(&csViewDTBField);
		if(m_vwDTBField)
			m_vwDTBField->PostMessage(WM_SETPSVAPNOE_FALSE);
		LeaveCriticalSection(&csViewDTBField);
	}
}

/**********************************************************************************************//**
 * Sets maintenance flag
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CMVViewHandler::setMaintenanceFlag(bool bState)
{
	EnterCriticalSection(&csViewDTBField);
	if(bState)
	{
		if(m_vwDTBField)
			m_vwDTBField->PostMessage(WM_SETMAINTENANCE_TRUE);
	}
	else
	{
		if(m_vwDTBField)
			m_vwDTBField->PostMessage(WM_SETMAINTENANCE_FALSE);
	}
	LeaveCriticalSection(&csViewDTBField);
}

/**********************************************************************************************//**
 * Sets triggered breath
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::SetTriggeredBreath()
{
	EnterCriticalSection(&csViewParaBtn);
	if(m_vwParaBtn)
		m_vwParaBtn->PostMessage(WM_SETTRIGGEREDBREATH);
	LeaveCriticalSection(&csViewParaBtn);
}

/**********************************************************************************************//**
 * Sets pric orunning
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CMVViewHandler::setPRICOrunning(bool state)
{
	if(state)
	{
		EnterCriticalSection(&csViewDiagramm);
		if(m_vwDiagramm)
			m_vwDiagramm->PostMessage(WM_SETPRICO_TRUE);
		LeaveCriticalSection(&csViewDiagramm);
		EnterCriticalSection(&csViewDTBField);
		if(m_vwDTBField)
			m_vwDTBField->PostMessage(WM_SETPRICO_TRUE);
		LeaveCriticalSection(&csViewDTBField);
		EnterCriticalSection(&csViewParaBtn);
		if(m_vwParaBtn)
			m_vwParaBtn->PostMessage(WM_SETPRICO_TRUE);
		LeaveCriticalSection(&csViewParaBtn);
	}		
	else
	{
		EnterCriticalSection(&csViewDiagramm);
		if(m_vwDiagramm)
			m_vwDiagramm->PostMessage(WM_SETPRICO_FALSE);
		LeaveCriticalSection(&csViewDiagramm);
		EnterCriticalSection(&csViewDTBField);
		if(m_vwDTBField)
			m_vwDTBField->PostMessage(WM_SETPRICO_FALSE);
		LeaveCriticalSection(&csViewDTBField);
		EnterCriticalSection(&csViewParaBtn);
		if(m_vwParaBtn)
			m_vwParaBtn->PostMessage(WM_SETPRICO_FALSE);
		LeaveCriticalSection(&csViewParaBtn);
	}
}

/**********************************************************************************************//**
 * Sets SI qdata
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::setSIQdata()
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vwDiagramm)
		m_vwDiagramm->PostMessage(WM_DRAW_SIQSPO2);
	LeaveCriticalSection(&csViewDiagramm);
}

/**********************************************************************************************//**
 * Draw measured fi o 2 value
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::drawMeasuredFiO2Value()
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vwDiagramm)
		m_vwDiagramm->PostMessage(WM_DRAW_FIO2VALUE);
	LeaveCriticalSection(&csViewDiagramm);
}

/**********************************************************************************************//**
 * Draw fo tsteps
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::drawFOTsteps()
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vwDiagramm)
		m_vwDiagramm->PostMessage(WM_DRAW_FOT_STEP);
	LeaveCriticalSection(&csViewDiagramm);
}

/**********************************************************************************************//**
 * Updates the limit data
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::UpdateLimitData()
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vwDiagramm)
		m_vwDiagramm->PostMessage(WM_UPDATELIMITDATA);
	LeaveCriticalSection(&csViewDiagramm);
}

/**********************************************************************************************//**
 * 2 flush changed
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::O2FlushChanged()
{
	EnterCriticalSection(&csViewParaBtn);
	if(m_vwParaBtn)
		m_vwParaBtn->PostMessage(WM_O2FLUSH_CHANGED);
	LeaveCriticalSection(&csViewParaBtn);
}

/**********************************************************************************************//**
 * Hfpmeanrec flush changed
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CMVViewHandler::HFPMEANRECFlushChanged(int iVal)
{
	EnterCriticalSection(&csViewParaBtn);
	if(m_vwParaBtn)
		m_vwParaBtn->PostMessage(WM_PMEANREC_CHANGED);
	LeaveCriticalSection(&csViewParaBtn);
}

/**********************************************************************************************//**
 * Para data changed
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::paraDataChanged()
{
	EnterCriticalSection(&csViewParaBtn);
	if(m_vwParaBtn)
		m_vwParaBtn->PostMessage(WM_SHOWPARABTN);
	LeaveCriticalSection(&csViewParaBtn);
}


//void CMVViewHandler::lungRecStateChanged()
//{
//	/*if(m_vwParaBtn)
//	{
//		m_vwParaBtn->ShowParaBtn(false);
//		m_vwParaBtn->Show();
//	}*/
//}

/**********************************************************************************************//**
 * Itime changed
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::ITIMEChanged()
{
	EnterCriticalSection(&csViewParaBtn);
	if(m_vwParaBtn)
		m_vwParaBtn->PostMessage(WM_ITIME_CHANGED);
	LeaveCriticalSection(&csViewParaBtn);
}

/**********************************************************************************************//**
 * Etime changed
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::ETIMEChanged()
{
	EnterCriticalSection(&csViewParaBtn);
	if(m_vwParaBtn)
		m_vwParaBtn->PostMessage(WM_ETIME_CHANGED);
	LeaveCriticalSection(&csViewParaBtn);
}

/**********************************************************************************************//**
 * Bpm changed
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::BPMChanged()
{
	EnterCriticalSection(&csViewParaBtn);
	if(m_vwParaBtn)
		m_vwParaBtn->PostMessage(WM_BPM_CHANGED);
	LeaveCriticalSection(&csViewParaBtn);
}

/**********************************************************************************************//**
 * Refresh o 2 flush
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::refreshO2Flush()
{
	EnterCriticalSection(&csViewParaBtn);
	if(m_vwParaBtn)
		m_vwParaBtn->PostMessage(WM_REFRESHOXIFLUSH);
	LeaveCriticalSection(&csViewParaBtn);

	if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
	{
		EnterCriticalSection(&csViewDiagramm);
		if(m_vwDiagramm)
			m_vwDiagramm->PostMessage(WM_CHECK_PRICO_ALARMS);
		LeaveCriticalSection(&csViewDiagramm);
	}
}

/**********************************************************************************************//**
 * Sets fo trunning
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CMVViewHandler::setFOTrunning(bool state)
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vwDiagramm)
	{
		if(state)
			m_vwDiagramm->PostMessage(WM_FOT_STARTED);
		else
			m_vwDiagramm->PostMessage(WM_FOT_STOPPED);
	}
	LeaveCriticalSection(&csViewDiagramm);
}

/**********************************************************************************************//**
 * Redraw information
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::redrawINFO()
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vwDTBField)
		m_vwDTBField->PostMessage(WM_DTB_REFRESH_INFO);
	LeaveCriticalSection(&csViewDiagramm);
}

/**********************************************************************************************//**
 * Draw fo ttime
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iCountFOTimer	Zero-based index of the count fo timer.
 **************************************************************************************************/

void CMVViewHandler::drawFOTtime(BYTE iCountFOTimer)
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vwDiagramm)
		m_vwDiagramm->PostMessage(WM_DRAW_FOT_TIME,iCountFOTimer);
	LeaveCriticalSection(&csViewDiagramm);
}

/**********************************************************************************************//**
 * Redraw fo tmenu
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::redrawFOTmenu()
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vwDiagramm)
		m_vwDiagramm->PostMessage(WM_REDRAW_FOT_STATE);
	LeaveCriticalSection(&csViewDiagramm);
}

/**********************************************************************************************//**
 * Check fo talarms
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::checkFOTalarms()
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vwDiagramm)
		m_vwDiagramm->PostMessage(WM_CHECK_FOT_ALARMS);
	LeaveCriticalSection(&csViewDiagramm);
}

/**********************************************************************************************//**
 * Updates the fot pmean para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::updateFOTPmeanPara()
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vwDiagramm)
		m_vwDiagramm->PostMessage(WM_FOT_UPDATE_PMEAN);
	LeaveCriticalSection(&csViewDiagramm);
}

/**********************************************************************************************//**
 * Updates the fotpeep para
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::updateFOTPEEPPara()
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vwDiagramm)
		m_vwDiagramm->PostMessage(WM_FOT_UPDATE_PEEP);
	LeaveCriticalSection(&csViewDiagramm);
}

/**********************************************************************************************//**
 * Redraw graph
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::redrawGraph()
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vwDiagramm)
		m_vwDiagramm->PostMessage(WM_GRAPH_REDRAW);
	LeaveCriticalSection(&csViewDiagramm);
}

/**********************************************************************************************//**
 * Draw o 2 flush time
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iO2FlushTime	The o 2 flush time.
 **************************************************************************************************/

void CMVViewHandler::DrawO2FlushTime(int iO2FlushTime)
{
	if(VS_PARA==getViewState())
	{
		EnterCriticalSection(&csViewParaBtn);
		if(m_vwParaBtn)
			m_vwParaBtn->PostMessage(WM_OXIFLUSHTIME,iO2FlushTime);
		LeaveCriticalSection(&csViewParaBtn);

		EnterCriticalSection(&csViewDTBField);
		if(m_vwDTBField)
			m_vwDTBField->PostMessage(WM_STOPOXIFLUSH);
		LeaveCriticalSection(&csViewDTBField);
	}
	else if(VS_ALARM_LIMIT==getViewState())
	{
		EnterCriticalSection(&csViewParaBtn);
		if(m_vwParaBtn)
			m_vwParaBtn->PostMessage(WM_OXIFLUSHTIME,iO2FlushTime);
		LeaveCriticalSection(&csViewParaBtn);

		EnterCriticalSection(&csViewDTBField);
		if(m_vwDTBField)
			m_vwDTBField->PostMessage(WM_STOPOXIFLUSH);
		LeaveCriticalSection(&csViewDTBField);
	}
	else
	{
		if(iO2FlushTime>=0)
		{
			if(iO2FlushTime==getModel()->getDATAHANDLER()->GetCurO2FlushTime())
			{
				EnterCriticalSection(&csViewDTBField);
				if(m_vwDTBField)
					m_vwDTBField->PostMessage(WM_STOPOXIFLUSH);
				LeaveCriticalSection(&csViewDTBField);
			}
			else
			{
				EnterCriticalSection(&csViewDTBField);
				if(m_vwDTBField)
					m_vwDTBField->SetTimeO2Flush(iO2FlushTime);
				LeaveCriticalSection(&csViewDTBField);
			}

		}
		else
		{
			EnterCriticalSection(&csViewDTBField);
			if(m_vwDTBField)
				m_vwDTBField->PostMessage(WM_STOPOXIFLUSH);
			LeaveCriticalSection(&csViewDTBField);
		}
	}
}

/**********************************************************************************************//**
 * Draw count down start time
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iCountDown	Zero-based index of the count down.
 **************************************************************************************************/

void CMVViewHandler::DrawCountDownStartTime(int iCountDown)
{
	EnterCriticalSection(&csViewDTBField);
	if(m_vwDTBField)
		m_vwDTBField->SetTimeUntilStartVent(iCountDown);
	LeaveCriticalSection(&csViewDTBField);

}

/**********************************************************************************************//**
 * Stops count down start time
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::StopCountDownStartTime()
{
	EnterCriticalSection(&csViewDTBField);
	if(m_vwDTBField)
		m_vwDTBField->PostMessage(WM_STOPTIMEUNTILSTARTVENT);
	LeaveCriticalSection(&csViewDTBField);
}

/**********************************************************************************************//**
 * Draw count down alarm silent
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iCountDown	Zero-based index of the count down.
 **************************************************************************************************/

void CMVViewHandler::DrawCountDownAlarmSilent(int iCountDown)
{
	EnterCriticalSection(&csViewDTBField);
	if(m_vwDTBField)
		m_vwDTBField->SetTimeAlarmSilent(iCountDown);
	LeaveCriticalSection(&csViewDTBField);
}

/**********************************************************************************************//**
 * Stops count down alarm silent
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::StopCountDownAlarmSilent()
{
	EnterCriticalSection(&csViewDTBField);
	if(m_vwDTBField)
		m_vwDTBField->PostMessage(WM_STOPALARMSILENT);
	LeaveCriticalSection(&csViewDTBField);
}

/**********************************************************************************************//**
 * Draw count time until stop
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iCountDown	Zero-based index of the count down.
 **************************************************************************************************/

void CMVViewHandler::DrawCountTimeUntilStop(int iCountDown)
{
	EnterCriticalSection(&csViewDTBField);
	if(m_vwDTBField)
		m_vwDTBField->SetTimeUntilStopVent(iCountDown);
	LeaveCriticalSection(&csViewDTBField);
}

/**********************************************************************************************//**
 * Stops count time until stop
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::StopCountTimeUntilStop()
{
	EnterCriticalSection(&csViewDTBField);
	if(m_vwDTBField)
		m_vwDTBField->PostMessage(WM_STOPTIMEUNTILSTOPVENT);
	LeaveCriticalSection(&csViewDTBField);
}

/**********************************************************************************************//**
 * Draw count time until off
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iCountDown	Zero-based index of the count down.
 **************************************************************************************************/

void CMVViewHandler::DrawCountTimeUntilOff(int iCountDown)
{
	EnterCriticalSection(&csViewDTBField);
	if(m_vwDTBField)
		m_vwDTBField->SetTimeUntilTurnOff(iCountDown);
	LeaveCriticalSection(&csViewDTBField);
}

/**********************************************************************************************//**
 * Stops count time until off
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::StopCountTimeUntilOff()
{
	EnterCriticalSection(&csViewDTBField);
	if(m_vwDTBField)
		m_vwDTBField->PostMessage(WM_STOPTIMEUNTILTURNOFF);
	LeaveCriticalSection(&csViewDTBField);
}

/**********************************************************************************************//**
 * Stops a video
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::StopVideo()
{
	EnterCriticalSection(&csViewMenu);
	if(m_vwMenu)
		m_vwMenu->PostMessage(WM_DEACTIVATE_VIDEO);
	LeaveCriticalSection(&csViewMenu);
}

/**********************************************************************************************//**
 * Redraw current views
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::RedrawCurViews()
{
	DrawViews(true);
	
}

/**********************************************************************************************//**
 * Gets view state
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The view state.
 **************************************************************************************************/

eViewState CMVViewHandler::getViewState()
{
	eViewState state=VS_NONE;
	EnterCriticalSection(&csViewState);
	state=m_eViewState;
	LeaveCriticalSection(&csViewState);

	//DEBUGMSG(TRUE, (TEXT("GetViewState %d\r\n"),(int)m_eViewState));
	return state;
}

/**********************************************************************************************//**
 * Sets view state
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	state   	The state.
 * \param	substate	The substate.
 **************************************************************************************************/

void CMVViewHandler::setViewState(eViewState state,eViewSubState substate)
{
	setPrevViewState(state,substate);

	EnterCriticalSection(&csViewState);
	m_eViewState = state;
	//DEBUGMSG(TRUE, (TEXT("SetViewState m_eViewState %d\r\n"),(int)m_eViewState));
	m_eViewSubState = substate;
	//DEBUGMSG(TRUE, (TEXT("SetViewState m_eViewSubState %d\r\n"),(int)m_eViewSubState));
	LeaveCriticalSection(&csViewState);

	CString viewState=_T("");
	viewState.Format(_T("***VS %d.%d"),(int)state,(int)substate);
	theApp.WriteLog(viewState);
}

/**********************************************************************************************//**
 * Gets view sub state
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The view sub state.
 **************************************************************************************************/

eViewSubState CMVViewHandler::getViewSubState()
{
	eViewSubState state=VSS_NONE;
	EnterCriticalSection(&csViewState);
	state=m_eViewSubState;
	LeaveCriticalSection(&csViewState);

	return state;
}

/**********************************************************************************************//**
 * Gets the previous view state
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The previous view state.
 **************************************************************************************************/

eViewState CMVViewHandler::getPrevViewState()
{
	eViewState state=VS_NONE;
	EnterCriticalSection(&csViewState);
	state=m_ePrevViewState;
	LeaveCriticalSection(&csViewState);

	return state;
}

/**********************************************************************************************//**
 * Sets previous view state
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	state   	The state.
 * \param	substate	The substate.
 **************************************************************************************************/

void CMVViewHandler::setPrevViewState(eViewState state,eViewSubState substate)
{
	EnterCriticalSection(&csViewState);
	if(state!=m_eViewState)
	{
		//DEBUGMSG(TRUE, (TEXT("SetPrevViewState m_eViewState %d\r\n"),(int)m_eViewState));
		m_ePrevViewState = m_eViewState;
	}
	if(substate!=m_eViewSubState)
	{
		//DEBUGMSG(TRUE, (TEXT("SetPrevViewState m_ePrevViewSubState %d\r\n"),(int)m_eViewSubState));
		m_ePrevViewSubState = m_eViewSubState;
	}
	LeaveCriticalSection(&csViewState);
}

/**********************************************************************************************//**
 * Gets the previous view sub state
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The previous view sub state.
 **************************************************************************************************/

eViewSubState CMVViewHandler::getPrevViewSubState()
{
	eViewSubState state=VSS_NONE;
	EnterCriticalSection(&csViewState);
	state=m_ePrevViewSubState;
	LeaveCriticalSection(&csViewState);

	return state;
}

/**********************************************************************************************//**
 * Gets the previous graph state
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The previous graph state.
 **************************************************************************************************/

eViewState CMVViewHandler::getPrevGraphState()
{
	eViewState state=VS_NONE;
	EnterCriticalSection(&csViewState);
	state=m_ePrevGraphState;
	LeaveCriticalSection(&csViewState);

	return state;
}

/**********************************************************************************************//**
 * Sets previous graph state
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::setPrevGraphState()
{
	EnterCriticalSection(&csViewState);
	m_ePrevGraphState = m_eViewState;
	m_ePrevGraphSubState = m_eViewSubState;
	LeaveCriticalSection(&csViewState);
}

/**********************************************************************************************//**
 * Gets the previous graph sub state
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	The previous graph sub state.
 **************************************************************************************************/

eViewSubState CMVViewHandler::getPrevGraphSubState()
{
	eViewSubState state=VSS_NONE;
	EnterCriticalSection(&csViewState);
	state=m_ePrevGraphSubState;
	LeaveCriticalSection(&csViewState);

	return state;
}

/**********************************************************************************************//**
 * Change view state
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	state	   	The state.
 * \param	substate   	The substate.
 * \param	bStopFreeze	True to stop freeze.
 **************************************************************************************************/

void CMVViewHandler::changeViewState(eViewState state,eViewSubState substate,bool bStopFreeze)
{
	//DEBUGMSG(TRUE, (TEXT("xxxxx changeViewState\r\n")));
	StopDiagramm(bStopFreeze);
	Sleep(0);

	if(getModel()->getCONFIG()->CurModeIsPresetMode()==true  || getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		getModel()->getVMODEHANDLER()->changeToPrevMode();

	if(getViewState()==VS_GRAPH || getViewState()==VS_PARA || getViewState()==VS_TREND)
		setPrevGraphState();

	setViewState(state,substate);

	viewStateChanged();

	getModel()->notifyViewStateChanged();
	
}

/**********************************************************************************************//**
 * Change to previous view state
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVViewHandler::changeToPrevViewState()
{
	eViewState ePrevGraphState=VS_NONE;
	eViewSubState ePrevGraphSubState=VSS_NONE;
	EnterCriticalSection(&csViewState);
	ePrevGraphState=m_ePrevGraphState;
	ePrevGraphSubState=m_ePrevGraphSubState;
	LeaveCriticalSection(&csViewState);

	if(		(getModel()->getCONFIG()->getCO2module()==CO2MODULE_NONE) 
		&&	ePrevGraphState==VS_GRAPH
		&&	(eViewSubState)getModel()->getCONFIG()->GraphGetLastViewState()==VSS_GRAPH_CO2GRAPHS)
	{
		changeViewState(VS_GRAPH,VSS_GRAPH_MULTI_LINEGRAPHS);
		getModel()->getCONFIG()->GraphSetLastViewState(VSS_GRAPH_MULTI_LINEGRAPHS);
	}
	else if(		(getModel()->getCONFIG()->getSPO2module()==SPO2MODULE_NONE) 
		&&	ePrevGraphState==VS_GRAPH
		&&	(eViewSubState)getModel()->getCONFIG()->GraphGetLastViewState()==VSS_GRAPH_SPO2GRAPHS)
	{
		changeViewState(VS_GRAPH,VSS_GRAPH_MULTI_LINEGRAPHS);
		getModel()->getCONFIG()->GraphSetLastViewState(VSS_GRAPH_MULTI_LINEGRAPHS);
	}
	else if(		(getModel()->getDATAHANDLER()->isFOTLicenseAvailable()==false) 
		&&	ePrevGraphState==VS_GRAPH
		&&	(eViewSubState)getModel()->getCONFIG()->GraphGetLastViewState()==VSS_GRAPH_FOTGRAPHS)
	{
		changeViewState(VS_GRAPH,VSS_GRAPH_MULTI_LINEGRAPHS);
		getModel()->getCONFIG()->GraphSetLastViewState(VSS_GRAPH_MULTI_LINEGRAPHS);
	}
	else
	{
		EnterCriticalSection(&csViewState);
		ePrevGraphState=m_ePrevGraphState;
		ePrevGraphSubState=m_ePrevGraphSubState;
		LeaveCriticalSection(&csViewState);
		changeViewState(ePrevGraphState,ePrevGraphSubState);
	}
}

/**********************************************************************************************//**
 * Determine if we can change view state alarm dependend
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if we can change view state alarm dependend, false if not.
 **************************************************************************************************/

bool CMVViewHandler::canChangeViewStateAlarmDependend()
{
	bool bRes=true;
	eAlarm curAlarm=getModel()->getALARMHANDLER()->getActiveAlarm();
	eVentSilentState silentState = getModel()->getALARMHANDLER()->getAlarmSilentState();

	if(curAlarm!=AL_NONE)
	{
		eAlarmType type = getModel()->getALARMHANDLER()->getAlarmType(curAlarm);

		if(		type==AT_SYSFAIL
			&&	getViewState()==VS_SYSFAIL
			&&	silentState==ASTATE_ACTIVE)
		{

			bRes=false;
		}
		else if(	getModel()->getALARMHANDLER()->ALARM_Accu_Empty->getAlarmState()==AS_ACTIVE
			&&	getViewState()==VS_BATTERY
			&&	silentState==ASTATE_ACTIVE)
		{
			bRes=false;
		}
		else if(	getModel()->getALARMHANDLER()->ALARM_Accu_Defect->getAlarmState()==AS_ACTIVE
			&&	getViewState()==VS_BATTERY
			&&	silentState==ASTATE_ACTIVE)
		{
			bRes=false;
		}
		else if(	type==AT_SYSALARM 
			&&	VS_ALARM_SYS==getViewState()
			&&	silentState==ASTATE_ACTIVE)
		{
			bRes=false;
		}
	}

	return bRes;
}

