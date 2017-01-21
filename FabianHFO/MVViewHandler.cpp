#include "StdAfx.h"
#include "MVViewHandler.h"
#include "globDefs.h"


CMVViewHandler* CMVViewHandler::theViewHandler=0;


// **************************************************************************
// 
// **************************************************************************
CMVViewHandler::CMVViewHandler()
{
	//getModel()->AttachObserver(this);
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
	
	m_pModel = NULL;

	m_vStartup=NULL;
	m_vDTBField=NULL;
	m_vDiagramm=NULL;
	m_vParaBtn=NULL;
	m_vNumericWnd=NULL;
	m_vService=NULL;
	m_vAlarmLimits=NULL;
	m_vSystemAlarm=NULL;
	m_vTrend=NULL;
	m_vLog=NULL;
	m_vPatData=NULL;
	m_vFullscreenMsg=NULL;
	//m_vSelectNumeric=NULL;
	m_vMenu=NULL;
	m_vShutdown=NULL;
	//m_vNebulizer=NULL;

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

// **************************************************************************
// 
// **************************************************************************
CMVViewHandler::~CMVViewHandler(void)
{
	//CloseNebulizerView();
	CloseFullscreenMsgView();
	CloseStartupView();
	CloseTrendView();
	CloseSystemAlarmView();
	//CloseSelectNumericView();
	CloseServiceView();
	CloseLogView();
	CloseNumericView();
	CloseParaBtnView();
	//CloseGraphView();
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

// **************************************************************************
// 
// **************************************************************************
CMVModel *CMVViewHandler::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::DestroyInstance()
{
	delete theViewHandler;
	theViewHandler = NULL;
}



// **************************************************************************
// 
// **************************************************************************
CMVViewHandler* CMVViewHandler::GetInstance()
{
	if(theViewHandler == 0)
	{
		theViewHandler = new CMVViewHandler;
	}
	return theViewHandler;
}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::Init()
{

	OpenStartupView();
	SetPrimaryActiveView(m_vStartup);
}



// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::OpenTrendView(bool bRedraw)
{
	bool result = false;

	//rku cs1
	EnterCriticalSection(&csViewTrend);
	if(m_vTrend)
	{
		m_vTrend->Show(bRedraw);
		LeaveCriticalSection(&csViewTrend);
		return false;
	}
	m_vTrend = new CViewTrend(IDC_VIEW_TREND);
	result = m_vTrend->CreateView();

	LeaveCriticalSection(&csViewTrend);

	return result;
}

// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::CloseTrendView()
{
	EnterCriticalSection(&csViewTrend);
	try
	{
		if(m_vTrend)
		{
			m_vTrend->DestroyWindow();
		}
	}
	catch (...)
	{
		delete m_vTrend;
		m_vTrend=NULL;
		LeaveCriticalSection(&csViewTrend);
		CFabianHFOApp::ReportException(_T("EXCEPTION: CloseTrendView"));
		throw;
	}
	delete m_vTrend;
	m_vTrend=NULL;
	LeaveCriticalSection(&csViewTrend);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::OpenPatDataView()
{
	bool result = false;

	//rku cs1
	EnterCriticalSection(&csViewPatData);
	if(m_vPatData)
	{
		m_vPatData->Show();
		LeaveCriticalSection(&csViewPatData);
		return false;
	}
	m_vPatData = new CViewPatData(IDC_VIEW_PATDATA);
	result = m_vPatData->CreateView();
	m_vPatData->Show();
	LeaveCriticalSection(&csViewPatData);

	return result;
}
// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::ClosePatDataView()
{

	EnterCriticalSection(&csViewPatData);
	try
	{
		if(m_vPatData)
		{
			m_vPatData->DestroyWindow();
		}
	}
	catch (...)
	{
		delete m_vPatData;
		m_vPatData=NULL;
		LeaveCriticalSection(&csViewPatData);
		CFabianHFOApp::ReportException(_T("EXCEPTION: ClosePatDataView"));
		throw;
	}
	delete m_vPatData;
	m_vPatData=NULL;
	LeaveCriticalSection(&csViewPatData);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::OpenFullscreenMsgView()
{
	bool result = false;

	//rku cs1
	EnterCriticalSection(&csViewFullscreenMsg);
	if(m_vFullscreenMsg)
	{
		m_vFullscreenMsg->Show();
		LeaveCriticalSection(&csViewFullscreenMsg);
		return false;
	}
	m_vFullscreenMsg = new CViewFullscreenMsg(IDC_VIEW_MESSAGE);
	result = m_vFullscreenMsg->CreateView();
	m_vFullscreenMsg->Show();
	LeaveCriticalSection(&csViewFullscreenMsg);

	return result;
}
bool CMVViewHandler::CloseFullscreenMsgView()
{
	EnterCriticalSection(&csViewFullscreenMsg);
	try
	{
		if(m_vFullscreenMsg)
		{
			m_vFullscreenMsg->DestroyWindow();
		}
	}
	catch (...)
	{
		delete m_vFullscreenMsg;
		m_vFullscreenMsg=NULL;
		LeaveCriticalSection(&csViewFullscreenMsg);
		CFabianHFOApp::ReportException(_T("EXCEPTION: CloseFullscreenMsgView"));
		throw;
	}
	delete m_vFullscreenMsg;
	m_vFullscreenMsg=NULL;
	LeaveCriticalSection(&csViewFullscreenMsg);

	return true;
}


// **************************************************************************
// 
// **************************************************************************
//bool CMVViewHandler::OpenSelectNumericView()
//{
//	bool result = false;
//
//	if(m_vSelectNumeric)
//	{
//		m_vSelectNumeric->Show();
//		return false;
//	}
//	m_vSelectNumeric = new CViewSelectNum(IDC_VIEW_SELECTNUM);
//
//	result = m_vSelectNumeric->CreateView();
//
//	m_vSelectNumeric->Show();
//
//	return result;
//}
//bool CMVViewHandler::CloseSelectNumericView()
//{
//	
//
//	try
//	{
//		if(m_vSelectNumeric)
//		{
//			m_vSelectNumeric->DestroyWindow();
//		}
//	}
//	catch (...)
//	{
//		CFabianHFOApp::ReportException(_T("EXCEPTION: CloseSelectNumericView"));
//		delete m_vSelectNumeric;
//		throw;
//	}
//	delete m_vSelectNumeric;
//	m_vSelectNumeric=NULL;
//
//	return true;
//}

// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::OpenStartupView()
{
	bool result = false;

	if(m_vStartup)
	{
		m_vStartup->Show();
		return false;
	}
	m_vStartup = new CViewStartup(IDC_VIEW_STARTUP);

	result = m_vStartup->CreateView();

	m_vStartup->Show();

	return result;
}

bool CMVViewHandler::CloseStartupView()
{
	try
	{
		if(m_vStartup)
		{
			m_vStartup->DestroyWindow();
		}
	}
	catch (...)
	{
		delete m_vStartup;
		m_vStartup=NULL;
		CFabianHFOApp::ReportException(_T("EXCEPTION: CloseStartupView"));
		throw;
	}
	delete m_vStartup;
	m_vStartup=NULL;

	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::OpenShutdownView()
{
	bool result = false;

	if(m_vShutdown)
	{
		m_vShutdown->Show();
		return false;
	}
	m_vShutdown = new CViewShutdown(IDC_VIEW_SHUTDOWN);

	result = m_vShutdown->CreateView();

	m_vShutdown->Show();

	return result;
}

bool CMVViewHandler::CloseShutdownView()
{
	try
	{
		if(m_vShutdown)
		{
			m_vShutdown->DestroyWindow();
		}
		
	}
	catch (...)
	{
		delete m_vShutdown;
		m_vShutdown=NULL;
		CFabianHFOApp::ReportException(_T("EXCEPTION: CloseShutdownView"));
		throw;
	}
	delete m_vShutdown;
	m_vShutdown=NULL;

	return true;
}


// **************************************************************************
// 
// **************************************************************************
//bool CMVViewHandler::OpenNebulizerView()
//{
//	bool result = false;
//
//	if(m_vNebulizer)
//	{
//		m_vNebulizer->Show();
//		return false;
//	}
//	m_vNebulizer = new CViewNebulizer(IDC_VIEW_NEBULIZER);
//
//	result = m_vNebulizer->CreateView();
//
//	m_vNebulizer->Show();
//
//	return result;
//}
//
//bool CMVViewHandler::CloseNebulizerView()
//{
//	
//	try
//	{
//		if(m_vNebulizer)
//		{
//			m_vNebulizer->DestroyWindow();
//		}
//		
//
//		if(AfxGetApp())
//			AfxGetApp()->GetMainWnd()->SetFocus();
//	}
//	catch (...)
//	{
//		CFabianHFOApp::ReportException(_T("EXCEPTION: CloseNebulizerView"));
//		delete m_vNebulizer;
//		throw;
//	}
//	delete m_vNebulizer;
//	m_vNebulizer=NULL;
//
//	return true;
//}

// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::OpenSystemAlarmView()
{
	bool result = false;

	//rku cs1
	EnterCriticalSection(&csViewSystemAlarm);
	if(m_vSystemAlarm)
	{
		m_vSystemAlarm->Show();
		LeaveCriticalSection(&csViewSystemAlarm);
		return false;
	}
	m_vSystemAlarm = new CViewSystemAlarm(IDC_VIEW_SYSALARM);
	result = m_vSystemAlarm->CreateView();
	LeaveCriticalSection(&csViewSystemAlarm);

	return result;
}

// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::CloseSystemAlarmView()
{
	//rku cs1
	EnterCriticalSection(&csViewSystemAlarm);
	try
	{
		if(m_vSystemAlarm)
		{
			m_vSystemAlarm->DestroyWindow();
		}
	}
	catch (...)
	{
		delete m_vSystemAlarm;
		m_vSystemAlarm=NULL;
		LeaveCriticalSection(&csViewSystemAlarm);
		CFabianHFOApp::ReportException(_T("EXCEPTION: CloseSystemAlarmView"));
		throw;
	}
	delete m_vSystemAlarm;
	m_vSystemAlarm=NULL;
	LeaveCriticalSection(&csViewSystemAlarm);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
//bool CMVViewHandler::OpenSetupView()
//{
//	bool result = false;
//
//	if(m_vSetup)
//	{
//		m_vSetup->Show();
//		return false;
//	}
//	m_vSetup = new CViewSetup(IDC_VIEW_SETUP);
//
//	result = m_vSetup->CreateView();
//
//	
//
//	return result;
//}
//
//bool CMVViewHandler::CloseSetupView()
//{
//	if(m_vSetup)
//	{
//		m_vSetup->Close();
//		m_vSetup->DestroyWindow();
//		delete m_vSetup;
//	}
//	m_vSetup=NULL;
//
//	return true;
//}

// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::OpenMenuView(bool bRedraw)
{
	bool result = false;

	//rku cs1
	EnterCriticalSection(&csViewMenu);
	if(m_vMenu)
	{
		m_vMenu->Show(bRedraw);
		LeaveCriticalSection(&csViewMenu);
		return false;
	}
	m_vMenu = new CViewMenu(IDC_VIEW_MENU);
	result = m_vMenu->CreateView();
	LeaveCriticalSection(&csViewMenu);

	return result;
}

bool CMVViewHandler::CloseMenuView()
{
	EnterCriticalSection(&csViewMenu);
	try
	{
		if(m_vMenu)
		{
			m_vMenu->Close();
			m_vMenu->DestroyWindow();
		}
	}
	catch (...)
	{
		delete m_vMenu;
		m_vMenu=NULL;
		LeaveCriticalSection(&csViewMenu);
		CFabianHFOApp::ReportException(_T("EXCEPTION: CloseMenuView"));
		throw;
	}
	delete m_vMenu;
	m_vMenu=NULL;
	LeaveCriticalSection(&csViewMenu);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::OpenServiceView(bool bRedraw)
{
	bool result = false;

	//rku cs1
	EnterCriticalSection(&csViewService);
	if(m_vService)
	{
		m_vService->Show(bRedraw);
		LeaveCriticalSection(&csViewService);
		return false;
	}
	m_vService = new CViewService(IDC_VIEW_SERVICE);
	result = m_vService->CreateView();
	LeaveCriticalSection(&csViewService);

	return result;
}

// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::CloseServiceView()
{
	//rku cs1
	EnterCriticalSection(&csViewService);
	try
	{
		if(m_vService)
		{
			m_vService->DestroyWindow();
		}
	}
	catch (...)
	{
		delete m_vService;
		m_vService=NULL;
		LeaveCriticalSection(&csViewService);
		CFabianHFOApp::ReportException(_T("EXCEPTION: CloseServiceView"));
		throw;
	}
	delete m_vService;
	m_vService=NULL;
	LeaveCriticalSection(&csViewService);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
//bool CMVViewHandler::OpenCalibrationView()
//{
//	bool result = false;
//
//	if(m_vCalibration)
//	{
//		return false;
//	}
//	m_vCalibration = new CViewCalibration(IDC_VIEW_CALIBRATION);
//
//	result = m_vCalibration->CreateView();
//	
//
//	return result;
//}
// **************************************************************************
// 
// **************************************************************************
//bool CMVViewHandler::CloseCalibrationView()
//{
//	if(m_vCalibration)
//	{
//		m_vCalibration->Close();
//		m_vCalibration->DestroyWindow();
//		delete m_vCalibration;
//	}
//	m_vCalibration=NULL;
//
//	return true;
//}

// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::OpenLogView()
{
	bool result = false;

	//rku cs1
	EnterCriticalSection(&csViewLog);
	if(m_vLog)
	{
		m_vLog->Show();
		LeaveCriticalSection(&csViewLog);
		return false;
	}
	m_vLog = new CViewLog(IDC_VIEW_LOGLIST);
	result = m_vLog->CreateView();
	LeaveCriticalSection(&csViewLog);
	return result;
}
// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::CloseLogView()
{
	//rku cs1
	EnterCriticalSection(&csViewLog);
	try
	{
		if(m_vLog)
		{
			m_vLog->DestroyWindow();
		}
	}
	catch (...)
	{
		delete m_vLog;
		m_vLog=NULL;
		LeaveCriticalSection(&csViewLog);
		CFabianHFOApp::ReportException(_T("EXCEPTION: CloseLogView"));
		throw;
	}
	delete m_vLog;
	m_vLog=NULL;
	LeaveCriticalSection(&csViewLog);
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::OpenNumericView()
{
	bool result = false;

	//rku cs1
	EnterCriticalSection(&csViewNumeric);
	if(m_vNumericWnd)
	{
		m_vNumericWnd->Show();
		LeaveCriticalSection(&csViewNumeric);
		return false;
	}
	m_vNumericWnd = new CViewNumericWnd(IDC_VIEW_NUMERIC);
	result = m_vNumericWnd->CreateView();

	LeaveCriticalSection(&csViewNumeric);
	return result;
}
// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::CloseNumericView()
{
	//rku cs1
	EnterCriticalSection(&csViewNumeric);
	try
	{
		if(m_vNumericWnd)
		{
			m_vNumericWnd->DestroyWindow();
		}
	}
	catch (...)
	{
		delete m_vNumericWnd;
		m_vNumericWnd=NULL;
		LeaveCriticalSection(&csViewNumeric);
		CFabianHFOApp::ReportException(_T("EXCEPTION: CloseNumericView"));
		throw;
	}

	delete m_vNumericWnd;
	m_vNumericWnd=NULL;
	LeaveCriticalSection(&csViewNumeric);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::OpenParaBtnView()
{
	bool result = false;

	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vParaBtn)
	{
		m_vParaBtn->Show();
		LeaveCriticalSection(&csViewParaBtn);
		return false;
	}
	m_vParaBtn = new CViewParaBtn(IDC_VIEW_PARABTN);
	result = m_vParaBtn->CreateView();
	LeaveCriticalSection(&csViewParaBtn);
	return result;
}
// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::CloseParaBtnView()
{
	EnterCriticalSection(&csViewParaBtn);
	try
	{
		if(m_vParaBtn)
		{
			m_vParaBtn->DestroyWindow();
		}
	}
	catch (...)
	{
		delete m_vParaBtn;
		m_vParaBtn=NULL;
		LeaveCriticalSection(&csViewParaBtn);
		CFabianHFOApp::ReportException(_T("EXCEPTION: CloseParaBtnView"));
		throw;
	}

	delete m_vParaBtn;
	m_vParaBtn=NULL;
	LeaveCriticalSection(&csViewParaBtn);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::OpenDiagrammView()
{
	bool result = false;

	//rku cs1
	EnterCriticalSection(&csViewDiagramm);
	if(m_vDiagramm)
	{
		m_vDiagramm->Show();
		LeaveCriticalSection(&csViewDiagramm);
		return false;
	}
	m_vDiagramm = new CViewDiagramm(IDC_VIEW_GRAPH);
	result = m_vDiagramm->CreateView();
	LeaveCriticalSection(&csViewDiagramm);

	return result;
}
bool CMVViewHandler::CloseDiagrammView()
{
	//rku cs1
	EnterCriticalSection(&csViewDiagramm);
	try
	{
		if(m_vDiagramm)
		{
			m_vDiagramm->DestroyWindow();
		}
	}
	catch (...)
	{
		delete m_vDiagramm;
		m_vDiagramm=NULL;
		LeaveCriticalSection(&csViewDiagramm);
		CFabianHFOApp::ReportException(_T("EXCEPTION: CloseDiagrammView"));
		throw;
	}

	delete m_vDiagramm;
	m_vDiagramm=NULL;
	LeaveCriticalSection(&csViewDiagramm);

	return true;
}
// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::OpenDTBFieldView()
{
	bool result = false;

	//rku cs1
	EnterCriticalSection(&csViewDTBField);
	if(m_vDTBField)
	{
		m_vDTBField->Show();
		LeaveCriticalSection(&csViewDTBField);
		return false;
	}
	m_vDTBField = new CViewDTBField(IDC_VIEW_DTBFIELD);
	result = m_vDTBField->CreateView();
	LeaveCriticalSection(&csViewDTBField);

	return result;
}
// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::CloseDTBFieldView()
{
	EnterCriticalSection(&csViewDTBField);
	try
	{
		if(m_vDTBField)
		{
			m_vDTBField->DestroyWindow();
		}
	}
	catch (...)
	{
		delete m_vDTBField;
		m_vDTBField=NULL;
		LeaveCriticalSection(&csViewDTBField);
		CFabianHFOApp::ReportException(_T("EXCEPTION: CloseDTBFieldView"));
		throw;
	}

	delete m_vDTBField;
	m_vDTBField=NULL;
	LeaveCriticalSection(&csViewDTBField);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::OpenAlarmLimitsView()
{
	bool result = false;

	//rku cs1
	EnterCriticalSection(&csViewAlarmLimits);
	if(m_vAlarmLimits)
	{
		m_vAlarmLimits->Show();
		LeaveCriticalSection(&csViewAlarmLimits);

		return false;
	}
	m_vAlarmLimits = new CViewAlarmLimit(IDC_VIEW_ALARMLIMITS);
	result = m_vAlarmLimits->CreateView();
	LeaveCriticalSection(&csViewAlarmLimits);

	return result;
}
// **************************************************************************
// 
// **************************************************************************
bool CMVViewHandler::CloseAlarmLimitsView()
{
	EnterCriticalSection(&csViewAlarmLimits);
	try
	{
		if(m_vAlarmLimits)
		{
			m_vAlarmLimits->DestroyWindow();
		}
	}
	catch (...)
	{
		delete m_vAlarmLimits;
		m_vAlarmLimits=NULL;
		LeaveCriticalSection(&csViewAlarmLimits);
		CFabianHFOApp::ReportException(_T("EXCEPTION: CloseAlarmLimitsView"));
		throw;
	}

	delete m_vAlarmLimits;
	m_vAlarmLimits=NULL;
	LeaveCriticalSection(&csViewAlarmLimits);

	return true;
}

/**=================================================================================================
 * \fn void CMVViewHandler::DrawViews(bool bRedraw)
 *
 * \brief Draw the different views
 * 
 * Depended on view to be drawn the previous views will be closed, depended views will be created.
 *
 * \author Rainer
 * \date 19 Nov 2015
 *
 * \param bRedraw true to redraw.
 *===============================================================================================**/
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

// **************************************************************************
// 
// **************************************************************************
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

	SetPrimaryActiveView(m_vShutdown);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();
}
// **************************************************************************
// 
// **************************************************************************
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

	SetPrimaryActiveView(m_vFullscreenMsg);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();
}
// **************************************************************************
// 
// **************************************************************************
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

	SetPrimaryActiveView(m_vFullscreenMsg);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();
}
// **************************************************************************
// 
// **************************************************************************
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

	SetPrimaryActiveView(m_vPatData);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();
}
// **************************************************************************
// 
// **************************************************************************
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
	
	SetPrimaryActiveView(m_vTrend);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();

}
// **************************************************************************
// 
// **************************************************************************
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

	SetPrimaryActiveView(m_vParaBtn);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();
}
// **************************************************************************
// 
// **************************************************************************
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

	SetPrimaryActiveView(m_vDiagramm);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();
}
// **************************************************************************
// 
// **************************************************************************
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

	SetPrimaryActiveView(m_vService);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_SETFOCUS_PIMARYVIEW);
}
// **************************************************************************
// 
// **************************************************************************
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

	SetPrimaryActiveView(m_vSystemAlarm);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_SETFOCUS_PIMARYVIEW);
}
// **************************************************************************
// 
// **************************************************************************
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
	SetPrimaryActiveView(m_vMenu);

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_SETFOCUS_PIMARYVIEW);
}
// **************************************************************************
// 
// **************************************************************************
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
		if(m_vParaBtn)
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

	SetPrimaryActiveView(m_vAlarmLimits);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_SETFOCUS_PIMARYVIEW);
}
// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::viewStateChanged()
{
	DrawViews(false);
}

// **************************************************************************
// 
// **************************************************************************
//void CMVViewHandler::VentModeChanged()
//{
//	//theApp.getLog()->WriteLine(_T("CMVViewHandler::NotifyVentModeChanged"));
//	if(AfxGetApp()->GetMainWnd())
//		AfxGetApp()->GetMainWnd()->SetFocus();
//	//SetCurrentFocusedView(GetPrimaryActiveView());
//}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::setPpsvValue(int iValPPSV,bool bSend)
{
	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vParaBtn)
		m_vParaBtn->setPpsvValue(iValPPSV,bSend);
	LeaveCriticalSection(&csViewParaBtn);
}
void CMVViewHandler::setPEEPvalue(int iValPEEP,bool bSend)
{
	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vParaBtn)
		m_vParaBtn->setPEEPvalue(iValPEEP,bSend);
	LeaveCriticalSection(&csViewParaBtn);
}

// **************************************************************************
// 
// **************************************************************************
//SHORT CMVViewHandler::getPpsvDisplayedValue()
//{
//	SHORT retVal=0;
//	//rku cs1
//	EnterCriticalSection(&csViewParaBtn);
//	if(m_vParaBtn)
//		retVal=m_vParaBtn->getPpsvDisplayedValue();
//	LeaveCriticalSection(&csViewParaBtn);
//
//	return retVal;
//}
//
//SHORT CMVViewHandler::getPEEPdisplayedValue()
//{
//	SHORT retVal=0;
//	//rku cs1
//	EnterCriticalSection(&csViewParaBtn);
//	if(m_vParaBtn)
//		retVal=m_vParaBtn->getPEEPdisplayedValue();
//	LeaveCriticalSection(&csViewParaBtn);
//
//	return retVal;
//}
// **************************************************************************
// 
// **************************************************************************
//void CMVViewHandler::setPmeanHFO(int iValPmean, bool bSendPmeanDif, bool bSendPmanual)//PMAN1
//{
//	if(m_vParaBtn)
//		m_vParaBtn->setPmeanHFO(iValPmean,bSendPmeanDif,bSendPmanual);
//}
void CMVViewHandler::setPmeanDifference(int iValPmean,bool bSend)//PMAN1
{
	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vParaBtn)
		m_vParaBtn->setPmeanDiff(iValPmean,bSend);
	LeaveCriticalSection(&csViewParaBtn);
}
void CMVViewHandler::setPmeanRecDifference(int iValPmeanRec,bool bSend)//PMAN1
{
	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vParaBtn)
		m_vParaBtn->setPmeanRecDiff(iValPmeanRec,bSend);
	LeaveCriticalSection(&csViewParaBtn);
}

// **************************************************************************
// 
// **************************************************************************
//void CMVViewHandler::setOxyDiffParaBTN(int iValOxy)//PRICO03
//{
//	//rku cs1
//	EnterCriticalSection(&csViewParaBtn);
//	if(m_vParaBtn)
//		m_vParaBtn->setOxyDiffParaBTN(iValOxy);
//	LeaveCriticalSection(&csViewParaBtn);
//}

//void CMVViewHandler::setOxyPPRICO(int iValOxy)//PRICO04
//{
//	if(m_vParaBtn)
//		m_vParaBtn->setOxyPPRICO(iValOxy);
//}
// **************************************************************************
// 
// **************************************************************************
//void CMVViewHandler::setPMAXVG(SHORT iValPMAXVG)//newVG
//{
//	if(m_vParaBtn)
//		m_vParaBtn->setPMAXVG(iValPMAXVG);
//}
// **************************************************************************
// 
// **************************************************************************
//void CMVViewHandler::RedrawDiagrammData(int iDiagrammID)
//{
//	//rku cs1
//	EnterCriticalSection(&csViewDiagramm);
//	if(m_vDiagramm)
//		m_vDiagramm->RedrawDiagrammData(iDiagrammID);
//	LeaveCriticalSection(&csViewDiagramm);
//}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::UpdateCO2InfoData(bool resetAvailable)
{
	//rku cs1
	if(resetAvailable)
	{
		EnterCriticalSection(&csViewDiagramm);
		if(m_vDiagramm)
			m_vDiagramm->PostMessage(WM_UPDATECO2INFODATA_TRUE);
		LeaveCriticalSection(&csViewDiagramm);
	}
	else
	{
		EnterCriticalSection(&csViewDiagramm);
		if(m_vDiagramm)
			m_vDiagramm->PostMessage(WM_UPDATECO2INFODATA_FALSE);
		LeaveCriticalSection(&csViewDiagramm);
	}
}

// **************************************************************************
// 
// **************************************************************************
//void CMVViewHandler::CO2PumpOn()
//{
//	if(m_vDiagramm)
//		m_vDiagramm->CO2PumpOn();
//}

// **************************************************************************
// 
// **************************************************************************
CMVView* CMVViewHandler::GetPrimaryActiveView()
{
	return m_vPrimaryActiveView;
}
void CMVViewHandler::SetPrimaryActiveView(CMVView *pView)
{
	m_vPrimaryActiveView=pView;
}

// **************************************************************************
// 
// **************************************************************************
CMVView* CMVViewHandler::GetCurrentFocusedView()
{
	return m_vCurrentFocusedView;
}
void CMVViewHandler::SetCurrentFocusedView(CMVView *pView)
{
	//DEBUGMSG(TRUE, (TEXT("CMVViewHandler::SetCurrentFocusedView()\r\n")));
	m_vCurrentFocusedView=pView;

	if(m_vCurrentFocusedView!=NULL)
		getModel()->NotifyViewFocusChanged(m_vCurrentFocusedView->GetDlgCtrlID());
	else
		getModel()->NotifyViewFocusChanged(0);
}


// **************************************************************************
// 
// **************************************************************************
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
			if(m_vService)
			{
				m_vService->PostMessage(WM_UPDATE_VIEW);
			}
			LeaveCriticalSection(&csViewService);
		}
	}
}

// **************************************************************************
// 
// **************************************************************************
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
			if(m_vDiagramm)
			{
				m_vDiagramm->PostMessage(WM_GRAPH_LOSTFOCUS);
			}
			LeaveCriticalSection(&csViewDiagramm);

		}
		else if(iT==IDC_VIEW_TREND)
		{
			//rku cs1
			EnterCriticalSection(&csViewTrend);
			if(m_vTrend)
			{
				m_vTrend->PostMessage(WM_TREND_LOSTFOCUS);
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
				SetPrimaryActiveView(m_vShutdown);
				m_vShutdown->PostMessage(WM_SETVIEWFOCUS);
			}
			break;
		case VS_SYSFAIL:
			{
				//rku cs1
				EnterCriticalSection(&csViewFullscreenMsg);
				if(m_vFullscreenMsg)
				{
					SetPrimaryActiveView(m_vFullscreenMsg);
					m_vFullscreenMsg->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewFullscreenMsg);
			}
			break;
		case VS_BATTERY:
			{
				//rku cs1
				EnterCriticalSection(&csViewFullscreenMsg);
				if(m_vFullscreenMsg)
				{
					SetPrimaryActiveView(m_vFullscreenMsg);
					m_vFullscreenMsg->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewFullscreenMsg);
			}
			break;
		case VS_PATDATA:
			{
				if(m_vPatData)
				{
					SetPrimaryActiveView(m_vPatData);
					m_vPatData->PostMessage(WM_SETVIEWFOCUS);
				}
			}
			break;
		case VS_TREND:
			{
				//rku cs1
				EnterCriticalSection(&csViewTrend);
				if(m_vTrend)
				{
					SetPrimaryActiveView(m_vTrend);
					m_vTrend->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewTrend);
			}
			break;
		case VS_PARA:
			{
				//rku cs1
				EnterCriticalSection(&csViewParaBtn);
				if(m_vParaBtn)
				{
					SetPrimaryActiveView(m_vParaBtn);
					m_vParaBtn->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewParaBtn);
			}
			break;
		case VS_GRAPH:
			{
				//rku cs1
				EnterCriticalSection(&csViewDiagramm);
				if(m_vDiagramm)
				{
					SetPrimaryActiveView(m_vDiagramm);
					m_vDiagramm->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewDiagramm);
			}
			break;
		case VS_SERVICE:
			{
				//rku cs1
				EnterCriticalSection(&csViewService);
				if(m_vService)
				{
					SetPrimaryActiveView(m_vService);
					m_vService->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewService);
			}
			break;
		case VS_ALARM_SYS:
			{
				//rku cs1
				EnterCriticalSection(&csViewSystemAlarm);
				if(m_vSystemAlarm)
				{
					SetPrimaryActiveView(m_vSystemAlarm);
					m_vSystemAlarm->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewSystemAlarm);

				theApp.getLog()->WriteLine(_T("*************Focus VS_ALARM_SYS"));
			}
			break;
		case VS_SETUP:
			{
				//rku cs1
				EnterCriticalSection(&csViewMenu);
				if(m_vMenu)
				{
					SetPrimaryActiveView(m_vMenu);
					m_vMenu->SetViewFocus();
				}
				LeaveCriticalSection(&csViewMenu);

				theApp.getLog()->WriteLine(_T("*************Focus VS_SETUP"));
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
				if(m_vAlarmLimits)
				{
					SetPrimaryActiveView(m_vAlarmLimits);
					m_vAlarmLimits->SetViewFocus();
				}
				LeaveCriticalSection(&csViewAlarmLimits);
			}
			break;
		default:
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->SetFocus();
				theApp.getLog()->WriteLine(_T("*************Focus default"));
			}
			break;
		}
	}
}

// **************************************************************************
// 
// **************************************************************************
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
				if(m_vAlarmLimits)
					m_vAlarmLimits->PostMessage(WM_SET_ALIMITTIMER);
				LeaveCriticalSection(&csViewAlarmLimits);
			}
			else
			{
				if(getModel()->getCONFIG()->GraphIsAutoScale())
				{
					//rku cs1
					EnterCriticalSection(&csViewDiagramm);
					if(m_vDiagramm)
						m_vDiagramm->PostMessage(WM_GRAPH_INCREASE_SPEED);
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
				if(m_vDiagramm)
					m_vDiagramm->PostMessage(WM_GRAPH_INCREASE_SPEED);
				LeaveCriticalSection(&csViewDiagramm);
			}
			else if(getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
			{
				//rku cs1
				EnterCriticalSection(&csViewDiagramm);
				if(m_vDiagramm)
					m_vDiagramm->PostMessage(WM_GRAPH_INCREASE_SPEED);
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
			if(m_vTrend)
				m_vTrend->PostMessage(WM_TREND_MOVERIGHT,m_iTrendRight);
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
// **************************************************************************
// 
// **************************************************************************
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
				if(m_vAlarmLimits)
					m_vAlarmLimits->PostMessage(WM_SET_ALIMITTIMER);
				LeaveCriticalSection(&csViewAlarmLimits);
			}
			else
			{
				if(getModel()->getCONFIG()->GraphIsAutoScale())
				{
					//rku cs1
					EnterCriticalSection(&csViewDiagramm);
					if(m_vDiagramm)
						m_vDiagramm->PostMessage(WM_GRAPH_DECREASE_SPEED);
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
				if(m_vDiagramm)
					m_vDiagramm->PostMessage(WM_GRAPH_DECREASE_SPEED);
				LeaveCriticalSection(&csViewDiagramm);
			}
			else if(getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
			{
				//rku cs1
				EnterCriticalSection(&csViewDiagramm);
				if(m_vDiagramm)
					m_vDiagramm->PostMessage(WM_GRAPH_DECREASE_SPEED);
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
			if(m_vTrend)
				m_vTrend->PostMessage(WM_TREND_MOVELEFT,m_iTrendLeft);
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

// **************************************************************************
// 
// **************************************************************************
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
					//SetCurrentFocusedView(m_vParaBtn);
				}
				break;
			case IDC_VIEW_GRAPH:
				{
					//rku cs1
					EnterCriticalSection(&csViewDiagramm);
					if(m_vDiagramm)
						SetCurrentFocusedView(m_vDiagramm);
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
					if(m_vParaBtn)
						SetCurrentFocusedView(m_vParaBtn);
					LeaveCriticalSection(&csViewParaBtn);
				}
				break;
			case IDC_VIEW_ALARMLIMITS:
				{
					//rku cs1
					EnterCriticalSection(&csViewAlarmLimits);
					if(m_vAlarmLimits)
						SetCurrentFocusedView(m_vAlarmLimits);
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

void CMVViewHandler::SetNextFocus()
{
	if(GetCurrentFocusedView()!=NULL)
	{
		GetCurrentFocusedView()->PostMessage(WM_SETNEXTFOCUS);
	}
}

void CMVViewHandler::SetPrevFocus()
{
	if(GetCurrentFocusedView()!=NULL)
	{
		GetCurrentFocusedView()->PostMessage(WM_SETPREVFOCUS);
	}
}

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
				if(m_vDiagramm)
				{
					SetCurrentFocusedView(m_vDiagramm);
					m_vDiagramm->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewDiagramm);
			}
			else if(iT==IDC_VIEW_GRAPH)
			{
				//rku cs1
				EnterCriticalSection(&csViewParaBtn);
				if(m_vParaBtn)
				{
					SetCurrentFocusedView(m_vParaBtn);
					m_vParaBtn->PostMessage(WM_SETVIEWFOCUS);
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
				//rku cs1
				EnterCriticalSection(&csViewDiagramm);
				if(m_vDiagramm)
				{
					SetCurrentFocusedView(m_vDiagramm);
					m_vDiagramm->PostMessage(WM_SETVIEWFOCUS);
				}
				LeaveCriticalSection(&csViewDiagramm);
			}
			else if(iT==IDC_VIEW_GRAPH)
			{
				//rku cs1
				EnterCriticalSection(&csViewParaBtn);
				if(m_vParaBtn)
				{
					SetCurrentFocusedView(m_vParaBtn);
					m_vParaBtn->PostMessage(WM_SETNEXTFOCUS);
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
// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::SetOxyHourglass(bool state)
{
	//rku cs1
	if(state)
	{
		EnterCriticalSection(&csViewParaBtn);
		if(m_vParaBtn)
			m_vParaBtn->PostMessage(WM_SETOXYHOURGLASS_TRUE);
		LeaveCriticalSection(&csViewParaBtn);
	}
	else
	{
		EnterCriticalSection(&csViewParaBtn);
		if(m_vParaBtn)
			m_vParaBtn->PostMessage(WM_SETOXYHOURGLASS_FALSE);
		LeaveCriticalSection(&csViewParaBtn);
	}
}



// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::StopDiagramm()
{
	//rku cs1
	EnterCriticalSection(&csViewDiagramm);
	if(m_vDiagramm)
		m_vDiagramm->StopDiagramm();
	LeaveCriticalSection(&csViewDiagramm);
}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::SetALIMITTIMER()
{
	//rku cs1
	EnterCriticalSection(&csViewAlarmLimits);
	if(m_vAlarmLimits)
		m_vAlarmLimits->PostMessage(WM_SET_ALIMITTIMER);
	LeaveCriticalSection(&csViewAlarmLimits);
}



// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::SetCPAPBackup(bool bState)
{
	//rku cs1
	if(bState)
	{
		EnterCriticalSection(&csViewDTBField);
		if(m_vDTBField)
			m_vDTBField->PostMessage(WM_SETCPAPBACKUP_TRUE);
		LeaveCriticalSection(&csViewDTBField);
	}
	else
	{
		EnterCriticalSection(&csViewDTBField);
		if(m_vDTBField)
			m_vDTBField->PostMessage(WM_SETCPAPBACKUP_FALSE);
		LeaveCriticalSection(&csViewDTBField);
	}
}
// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::setPSVapnoe(bool bState)
{
	//rku cs1
	if(bState)
	{
		EnterCriticalSection(&csViewParaBtn);
		if(m_vParaBtn)
			m_vParaBtn->PostMessage(WM_SETPSVAPNOE_TRUE);
		LeaveCriticalSection(&csViewParaBtn);

		EnterCriticalSection(&csViewDTBField);
		if(m_vDTBField)
			m_vDTBField->PostMessage(WM_SETPSVAPNOE_TRUE);
		LeaveCriticalSection(&csViewDTBField);
	}
	else
	{
		EnterCriticalSection(&csViewParaBtn);
		if(m_vParaBtn)
			m_vParaBtn->PostMessage(WM_SETPSVAPNOE_FALSE);
		LeaveCriticalSection(&csViewParaBtn);

		EnterCriticalSection(&csViewDTBField);
		if(m_vDTBField)
			m_vDTBField->PostMessage(WM_SETPSVAPNOE_FALSE);
		LeaveCriticalSection(&csViewDTBField);
	}
}

void CMVViewHandler::setMaintenanceFlag(bool bState)
{
	//rku cs1
	EnterCriticalSection(&csViewDTBField);
	if(bState)
	{
		if(m_vDTBField)
			m_vDTBField->PostMessage(WM_SETMAINTENANCE_TRUE);
	}
	else
	{
		if(m_vDTBField)
			m_vDTBField->PostMessage(WM_SETMAINTENANCE_FALSE);
	}
	LeaveCriticalSection(&csViewDTBField);
}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::SetTriggeredBreath()
{
	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vParaBtn)
		m_vParaBtn->PostMessage(WM_SETTRIGGEREDBREATH);
	LeaveCriticalSection(&csViewParaBtn);
}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::setPRICOrunning(bool state)
{
	if(state)
	{
		//rku cs1
		EnterCriticalSection(&csViewDTBField);
		if(m_vDTBField)
			m_vDTBField->PostMessage(WM_SETPRICO_TRUE);
		LeaveCriticalSection(&csViewDTBField);
		//rku cs1
		EnterCriticalSection(&csViewParaBtn);
		if(m_vParaBtn)
			m_vParaBtn->PostMessage(WM_SETPRICO_TRUE);
		LeaveCriticalSection(&csViewParaBtn);
		//rku cs1
		EnterCriticalSection(&csViewDiagramm);
		if(m_vDiagramm)
			m_vDiagramm->PostMessage(WM_SETPRICO_TRUE);
		LeaveCriticalSection(&csViewDiagramm);
	}
	else
	{
		//rku cs1
		EnterCriticalSection(&csViewDTBField);
		if(m_vDTBField)
			m_vDTBField->PostMessage(WM_SETPRICO_FALSE);
		LeaveCriticalSection(&csViewDTBField);
		//rku cs1
		EnterCriticalSection(&csViewParaBtn);
		if(m_vParaBtn)
			m_vParaBtn->PostMessage(WM_SETPRICO_FALSE);
		LeaveCriticalSection(&csViewParaBtn);
		//rku cs1
		EnterCriticalSection(&csViewDiagramm);
		if(m_vDiagramm)
			m_vDiagramm->PostMessage(WM_SETPRICO_FALSE);
		LeaveCriticalSection(&csViewDiagramm);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::setSIQdata()
{
	//rku cs1
	EnterCriticalSection(&csViewDiagramm);
	if(m_vDiagramm)
		m_vDiagramm->PostMessage(WM_DRAW_SIQSPO2);
	LeaveCriticalSection(&csViewDiagramm);
}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::drawMeasuredFiO2Value()
{
	//rku cs1
	EnterCriticalSection(&csViewDiagramm);
	if(m_vDiagramm)
		m_vDiagramm->PostMessage(WM_DRAW_FIO2VALUE);
	LeaveCriticalSection(&csViewDiagramm);
}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::drawFOTsteps()
{
	//rku cs1
	EnterCriticalSection(&csViewDiagramm);
	if(m_vDiagramm)
		m_vDiagramm->PostMessage(WM_DRAW_FOT_STEP);
	LeaveCriticalSection(&csViewDiagramm);
}


// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::UpdateLimitData()
{
	//rku cs1
	EnterCriticalSection(&csViewDiagramm);
	if(m_vDiagramm)
		m_vDiagramm->PostMessage(WM_UPDATELIMITDATA);
	LeaveCriticalSection(&csViewDiagramm);
}

void CMVViewHandler::O2FlushChanged()
{
	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vParaBtn)
		m_vParaBtn->PostMessage(WM_O2FLUSH_CHANGED);
	LeaveCriticalSection(&csViewParaBtn);
}

void CMVViewHandler::HFPMEANRECFlushChanged(int iVal)
{
	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vParaBtn)
		m_vParaBtn->PostMessage(WM_PMEANREC_CHANGED);
	LeaveCriticalSection(&csViewParaBtn);
}


void CMVViewHandler::paraDataChanged()
{
	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vParaBtn)
		m_vParaBtn->PostMessage(WM_SHOWPARABTN);
	LeaveCriticalSection(&csViewParaBtn);
}

void CMVViewHandler::lungRecStateChanged()
{
	/*if(m_vParaBtn)
	{
		m_vParaBtn->ShowParaBtn(false);
		m_vParaBtn->Show();
	}*/
}

void CMVViewHandler::ITIMEChanged()
{
	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vParaBtn)
		m_vParaBtn->PostMessage(WM_ITIME_CHANGED);
	LeaveCriticalSection(&csViewParaBtn);
}
void CMVViewHandler::ETIMEChanged()
{
	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vParaBtn)
		m_vParaBtn->PostMessage(WM_ETIME_CHANGED);
	LeaveCriticalSection(&csViewParaBtn);
}
void CMVViewHandler::BPMChanged()
{
	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vParaBtn)
		m_vParaBtn->PostMessage(WM_BPM_CHANGED);
	LeaveCriticalSection(&csViewParaBtn);
}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::refreshO2Flush()
{
	//rku cs1
	EnterCriticalSection(&csViewParaBtn);
	if(m_vParaBtn)
		m_vParaBtn->PostMessage(WM_REFRESHOXIFLUSH);
	LeaveCriticalSection(&csViewParaBtn);

	if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
	{
		EnterCriticalSection(&csViewDiagramm);
		if(m_vDiagramm)
			m_vDiagramm->PostMessage(WM_CHECK_PRICO_ALARMS);
		LeaveCriticalSection(&csViewDiagramm);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::setFOTrunning(bool state)
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vDiagramm)
	{
		if(state)
			m_vDiagramm->PostMessage(WM_FOT_STARTED);
		else
			m_vDiagramm->PostMessage(WM_FOT_STOPPED);
	}
	LeaveCriticalSection(&csViewDiagramm);
}
// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::drawFOTtime(BYTE iCountFOTimer)
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vDiagramm)
		m_vDiagramm->PostMessage(WM_DRAW_FOT_TIME,iCountFOTimer);
	LeaveCriticalSection(&csViewDiagramm);
}
//void CMVViewHandler::drawFOTCollectTime(BYTE iCountFOTimer)
//{
//	EnterCriticalSection(&csViewDiagramm);
//	if(m_vDiagramm)
//		m_vDiagramm->PostMessage(WM_COLLECTDATA_FOT_TIME,iCountFOTimer);
//	LeaveCriticalSection(&csViewDiagramm);
//}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::updateFOTPmeanPara()
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vDiagramm)
		m_vDiagramm->PostMessage(WM_FOT_UPDATE_PMEAN);
	LeaveCriticalSection(&csViewDiagramm);
}

void CMVViewHandler::updateFOTPEEPPara()
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vDiagramm)
		m_vDiagramm->PostMessage(WM_FOT_UPDATE_PEEP);
	LeaveCriticalSection(&csViewDiagramm);
}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::redrawGraph()
{
	EnterCriticalSection(&csViewDiagramm);
	if(m_vDiagramm)
		m_vDiagramm->PostMessage(WM_GRAPH_REDRAW);
	LeaveCriticalSection(&csViewDiagramm);
}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::DrawO2FlushTime(int iO2FlushTime)
{
	if(VS_PARA==getViewState())
	{
		EnterCriticalSection(&csViewParaBtn);
		if(m_vParaBtn)
			m_vParaBtn->PostMessage(WM_OXIFLUSHTIME,iO2FlushTime);
		LeaveCriticalSection(&csViewParaBtn);

		EnterCriticalSection(&csViewDTBField);
		if(m_vDTBField)
			m_vDTBField->PostMessage(WM_STOPOXIFLUSH);
		LeaveCriticalSection(&csViewDTBField);
	}
	else if(VS_ALARM_LIMIT==getViewState())
	{
		EnterCriticalSection(&csViewParaBtn);
		if(m_vParaBtn)
			m_vParaBtn->PostMessage(WM_OXIFLUSHTIME,iO2FlushTime);
		LeaveCriticalSection(&csViewParaBtn);

		EnterCriticalSection(&csViewDTBField);
		if(m_vDTBField)
			m_vDTBField->PostMessage(WM_STOPOXIFLUSH);
		LeaveCriticalSection(&csViewDTBField);
	}
	else
	{
		if(iO2FlushTime>=0)
		{
			if(iO2FlushTime==getModel()->getDATAHANDLER()->GetCurO2FlushTime())
			{
				//rku cs1
				EnterCriticalSection(&csViewDTBField);
				if(m_vDTBField)
					m_vDTBField->PostMessage(WM_STOPOXIFLUSH);
				LeaveCriticalSection(&csViewDTBField);
			}
			else
			{
				CStringW sz;
				sz.Format(_T("%s %d %s"),getModel()->GetLanguageString(IDS_TXT_FLUSHTIME), iO2FlushTime, getModel()->GetLanguageString(IDS_UNIT_SECONDS));
				//rku cs1
				EnterCriticalSection(&csViewDTBField);
				if(m_vDTBField)
					m_vDTBField->SetTimeO2Flush(sz);
				LeaveCriticalSection(&csViewDTBField);
			}

		}
		else
		{
			//rku cs1
			EnterCriticalSection(&csViewDTBField);
			if(m_vDTBField)
				m_vDTBField->PostMessage(WM_STOPOXIFLUSH);
			LeaveCriticalSection(&csViewDTBField);
		}
	}
}


// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::DrawCountDownStartTime(int iCountDown)
{
	//rku cs1
	EnterCriticalSection(&csViewDTBField);
	if(m_vDTBField)
		m_vDTBField->SetTimeUntilStartVent(iCountDown);
	LeaveCriticalSection(&csViewDTBField);

}
void CMVViewHandler::StopCountDownStartTime()
{
	//rku cs1
	EnterCriticalSection(&csViewDTBField);
	if(m_vDTBField)
		m_vDTBField->PostMessage(WM_STOPTIMEUNTILSTARTVENT);
	LeaveCriticalSection(&csViewDTBField);
}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::DrawCountDownAlarmSilent(int iCountDown)
{
	//rku cs1
	EnterCriticalSection(&csViewDTBField);
	if(m_vDTBField)
		m_vDTBField->SetTimeAlarmSilent(iCountDown);
	LeaveCriticalSection(&csViewDTBField);
}
void CMVViewHandler::StopCountDownAlarmSilent()
{
	//rku cs1
	EnterCriticalSection(&csViewDTBField);
	if(m_vDTBField)
		m_vDTBField->PostMessage(WM_STOPALARMSILENT);
	LeaveCriticalSection(&csViewDTBField);
}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::DrawCountTimeUntilStop(int iCountDown)
{
	//rku cs1
	EnterCriticalSection(&csViewDTBField);
	if(m_vDTBField)
		m_vDTBField->SetTimeUntilStopVent(iCountDown);
	LeaveCriticalSection(&csViewDTBField);
}
void CMVViewHandler::StopCountTimeUntilStop()
{
	//rku cs1
	EnterCriticalSection(&csViewDTBField);
	if(m_vDTBField)
		m_vDTBField->PostMessage(WM_STOPTIMEUNTILSTOPVENT);
	LeaveCriticalSection(&csViewDTBField);
}
// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::DrawCountTimeUntilOff(int iCountDown)
{
	//rku cs1
	EnterCriticalSection(&csViewDTBField);
	if(m_vDTBField)
		m_vDTBField->SetTimeUntilTurnOff(iCountDown);
	LeaveCriticalSection(&csViewDTBField);
}
void CMVViewHandler::StopCountTimeUntilOff()
{
	//rku cs1
	EnterCriticalSection(&csViewDTBField);
	if(m_vDTBField)
		m_vDTBField->PostMessage(WM_STOPTIMEUNTILTURNOFF);
	LeaveCriticalSection(&csViewDTBField);
}

// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::StopVideo()
{
	//rku cs1
	EnterCriticalSection(&csViewMenu);
	if(m_vMenu)
		m_vMenu->PostMessage(WM_DEACTIVATE_VIDEO);
	LeaveCriticalSection(&csViewMenu);
}



// **************************************************************************
// 
// **************************************************************************
void CMVViewHandler::RedrawCurViews()
{
	DrawViews(true);
	
}

// **************************************************************************
// 
// **************************************************************************
eViewState CMVViewHandler::getViewState()
{
	eViewState state=VS_NONE;
	//rku cs1
	EnterCriticalSection(&csViewState);
	state=m_eViewState;
	LeaveCriticalSection(&csViewState);

	//DEBUGMSG(TRUE, (TEXT("GetViewState %d\r\n"),(int)m_eViewState));
	return state;
}
void CMVViewHandler::setViewState(eViewState state,eViewSubState substate)
{
	setPrevViewState(state,substate);

	//rku cs1
	EnterCriticalSection(&csViewState);
	m_eViewState = state;
	//DEBUGMSG(TRUE, (TEXT("SetViewState m_eViewState %d\r\n"),(int)m_eViewState));
	m_eViewSubState = substate;
	//DEBUGMSG(TRUE, (TEXT("SetViewState m_eViewSubState %d\r\n"),(int)m_eViewSubState));
	LeaveCriticalSection(&csViewState);

	CString viewState=_T("");
	viewState.Format(_T("***VS %d.%d"),(int)state,(int)substate);
	theApp.getLog()->WriteLine(viewState);
}

eViewSubState CMVViewHandler::getViewSubState()
{
	eViewSubState state=VSS_NONE;
	//rku cs1
	EnterCriticalSection(&csViewState);
	state=m_eViewSubState;
	LeaveCriticalSection(&csViewState);

	return state;
}

eViewState CMVViewHandler::getPrevViewState()
{
	eViewState state=VS_NONE;
	//rku cs1
	EnterCriticalSection(&csViewState);
	state=m_ePrevViewState;
	LeaveCriticalSection(&csViewState);

	return state;
}

void CMVViewHandler::setPrevViewState(eViewState state,eViewSubState substate)
{
	//rku cs1
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

eViewSubState CMVViewHandler::getPrevViewSubState()
{
	eViewSubState state=VSS_NONE;
	//rku cs1
	EnterCriticalSection(&csViewState);
	state=m_ePrevViewSubState;
	LeaveCriticalSection(&csViewState);

	return state;
}

eViewState CMVViewHandler::getPrevGraphState()
{
	eViewState state=VS_NONE;
	//rku cs1
	EnterCriticalSection(&csViewState);
	state=m_ePrevGraphState;
	LeaveCriticalSection(&csViewState);

	return state;
}

void CMVViewHandler::setPrevGraphState()
{
	//rku cs1
	EnterCriticalSection(&csViewState);
	m_ePrevGraphState = m_eViewState;
	m_ePrevGraphSubState = m_eViewSubState;
	LeaveCriticalSection(&csViewState);
}

eViewSubState CMVViewHandler::getPrevGraphSubState()
{
	eViewSubState state=VSS_NONE;
	//rku cs1
	EnterCriticalSection(&csViewState);
	state=m_ePrevGraphSubState;
	LeaveCriticalSection(&csViewState);

	return state;
}

void CMVViewHandler::changeViewState(eViewState state,eViewSubState substate)
{
	StopDiagramm();

	if(getModel()->getCONFIG()->CurModeIsPresetMode()==true  || getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		getModel()->getVMODEHANDLER()->changeToPrevMode();

	if(getViewState()==VS_GRAPH || getViewState()==VS_PARA || getViewState()==VS_TREND)
		setPrevGraphState();

	setViewState(state,substate);

	viewStateChanged();

	getModel()->notifyViewStateChanged();
	
}

void CMVViewHandler::changeToPrevViewState()
{
	eViewState ePrevGraphState=VS_NONE;
	eViewSubState ePrevGraphSubState=VSS_NONE;
	//rku cs1
	EnterCriticalSection(&csViewState);
	ePrevGraphState=m_ePrevGraphState;
	ePrevGraphSubState=m_ePrevGraphSubState;
	LeaveCriticalSection(&csViewState);

	//DEBUGMSG(TRUE, (TEXT("changeToPrevViewState\r\n")));
	if(		(getModel()->getCONFIG()->GetCO2module()==CO2MODULE_NONE) 
		&&	ePrevGraphState==VS_GRAPH
		&&	(eViewSubState)getModel()->getCONFIG()->GraphGetLastViewState()==VSS_GRAPH_CO2GRAPHS)
	{
		changeViewState(VS_GRAPH,VSS_GRAPH_MULTI_LINEGRAPHS);
		getModel()->getCONFIG()->GraphSetLastViewState(VSS_GRAPH_MULTI_LINEGRAPHS);
	}
	else if(		(getModel()->getCONFIG()->GetSPO2module()==SPO2MODULE_NONE) 
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
		//rku cs1
		EnterCriticalSection(&csViewState);
		ePrevGraphState=m_ePrevGraphState;
		ePrevGraphSubState=m_ePrevGraphSubState;
		LeaveCriticalSection(&csViewState);
		changeViewState(ePrevGraphState,ePrevGraphSubState);
	}
}

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
			//theApp.getLog()->WriteLine(L"XXXXXXX #001");
		}
		else if(	getModel()->getALARMHANDLER()->ALARM_Accu_Empty->getAlarmState()==AS_ACTIVE
			&&	getViewState()==VS_BATTERY
			&&	silentState==ASTATE_ACTIVE)
		{
			bRes=false;
			//theApp.getLog()->WriteLine(L"XXXXXXX #002");
		}
		else if(	getModel()->getALARMHANDLER()->ALARM_Accu_Defect->getAlarmState()==AS_ACTIVE
			&&	getViewState()==VS_BATTERY
			&&	silentState==ASTATE_ACTIVE)
		{
			bRes=false;
			//theApp.getLog()->WriteLine(L"XXXXXXX #003");
		}
		else if(	type==AT_SYSALARM 
			&&	VS_ALARM_SYS==getViewState()
			&&	silentState==ASTATE_ACTIVE)
		{
			bRes=false;
			//theApp.getLog()->WriteLine(L"XXXXXXX #004");
		}
	}

	return bRes;
}

//void CMVViewHandler::startTest()
//{
//	if(m_vParaBtn)
//	{
//		//m_vParaBtn->ShowParaBtn();
//		HWND hWnd2=m_vParaBtn->GetSafeHwnd();
//		//CWnd* pWnd = CWnd::FromHandle(GetDlgItem(hWnd2,IDC_BTN_PARA_IFLOW));
//		CParaBtn* pWnd = (CParaBtn*)CWnd::FromHandle(GetDlgItem(hWnd2,IDC_BTN_PARA_IFLOW));
//		pWnd->SetParaValue(5000);
//		pWnd->PostMessage(WM_KEYDOWN);
//		//pWnd->WriteCurrentValue();
//		//pWnd->Depress();
//		int iStop=0;
//	}
//}