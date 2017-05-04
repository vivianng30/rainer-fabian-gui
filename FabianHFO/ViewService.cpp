// ViewService.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ViewService.h"
#include "LangAdmin.h"
#include "globDefs.h"

#include "TlsDevice.h"


// **************************************************************************
// 
// **************************************************************************
CViewService::CViewService(int iViewID):
CMVView(iViewID)
{
	InitializeCriticalSection(&csServiceWnd);

	m_pcWndSystem=NULL;
	m_pcWndDisplay=NULL;
	m_pcWndSettings=NULL;
	m_pcWndCalibration=NULL;
	m_pcWndFlowboard=NULL;
	m_pcWndAccuboard=NULL;
	m_pcWndValves=NULL;
	m_pcWndMonitorPIC=NULL;
	m_pcWndInterfaces=NULL;
	m_pcWndNetwork=NULL;
	m_pcWndLicensing=NULL;
	m_pcWndMaintenance=NULL;
	m_pWndServiceMenu=NULL;

	m_eCurServiceWnd=SERVICE_MENU;
}

// **************************************************************************
// 
// **************************************************************************
CViewService::~CViewService(void)
{
	DeleteCriticalSection(&csServiceWnd);
}



BEGIN_MESSAGE_MAP(CViewService, CMVView)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// **************************************************************************
// 
// **************************************************************************
bool CViewService::CreateView()
{
	//RECT rcLd={5,50,790,535};
	RECT rcLd={5,50,800,595};

	if (!Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_SERVICE))
	{
		return false;
	}

	Initialize();

	return true;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CViewService::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD|WS_VISIBLE,rc,pParentWnd,nID))
	{
		CClientDC dc(this);

		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		CBrush cbrBack(BACKGND);
		//CBrush cbrBack(RGB(255,255,255));
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev = (HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();//rkuNEWFIX

		return 1;
	}
	else
		return 0;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewService::Initialize()
{
	theApp.getLog()->WriteLine(_T("*** Service mode started ***"));

	bool result=CreateWndServiceMenu();
	if(result)
		ShowWndServiceMenu(true);

	return result;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewService::CreateWndServiceMenu()
{
	if(m_pWndServiceMenu==NULL)
	{
		m_pWndServiceMenu = new CWndServiceMenu();
		RECT rcLd={10,10,790,590};

		if(m_pWndServiceMenu->Create(this,rcLd,IDC_VIEW_SERVICE_MENU))
		{
			return true;
		}
	}

	return false;
}
void CViewService::ShowWndServiceMenu(bool bShow)
{
	if(m_pWndServiceMenu)
	{
		m_pWndServiceMenu->Show(bShow);
		m_pWndServiceMenu->SetFocus();
	}
	setCurServiceWnd(SERVICE_MENU);


}

bool CViewService::DestroyWndServiceMenu()
{
	if(m_pWndServiceMenu)
	{
		m_pWndServiceMenu->DestroyWindow();
	}
	
	delete m_pWndServiceMenu;
	m_pWndServiceMenu=NULL;

	return false;
}


// **************************************************************************
// 
// **************************************************************************
bool CViewService::CreateWndMaintenance()
{
	if(m_pcWndMaintenance==NULL)
	{
		m_pcWndMaintenance = new CWndServiceMaintenance();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={10,10,790,590};

		if(m_pcWndMaintenance->Create(this,rcLd,IDC_VIEW_SERVICE_MAINTENANCE))
		{
			return true;
		}
	}
	return false;
}
bool CViewService::DestroyWndMaintenance()
{
	if(m_pcWndMaintenance)
	{
		m_pcWndMaintenance->DestroyWindow();
		delete m_pcWndMaintenance;
		m_pcWndMaintenance=NULL;
	}
	return false;
}
void CViewService::ShowWndMaintenance(bool show)
{
	if(m_pcWndMaintenance)
	{
		m_pcWndMaintenance->Show(show);
		m_pcWndMaintenance->SetFocus();
	}
	setCurServiceWnd(SERVICE_MAINTENANCE);
}
// **************************************************************************
// 
// **************************************************************************
bool CViewService::CreateWndSystem()
{
	if(m_pcWndSystem==NULL)
	{
		m_pcWndSystem = new CWndServiceSystem();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={10,10,790,590};
		
		if(m_pcWndSystem->Create(this,rcLd,IDC_VIEW_SERVICE_SYS))
		{
			return true;
		}
	}
	return false;
}
bool CViewService::DestroyWndSystem()
{
	if(m_pcWndSystem)
	{
		m_pcWndSystem->DestroyWindow();
	}
	
	delete m_pcWndSystem;
	m_pcWndSystem=NULL;

	return false;
}
void CViewService::ShowWndSystem(bool show)
{
	if(m_pcWndSystem)
	{
		m_pcWndSystem->Show(show);
		m_pcWndSystem->SetFocus();
	}
	setCurServiceWnd(SERVICE_SYS);
}


// **************************************************************************
// 
// **************************************************************************
bool CViewService::CreateWndDisplay()
{
	if(m_pcWndDisplay==NULL)
	{
		m_pcWndDisplay = new CWndServiceDisplay();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={10,10,790,590};

		if(m_pcWndDisplay->Create(this,rcLd,IDC_VIEW_SERVICE_DISPL))
		{
			return true;
		}
	}
	return false;
}
bool CViewService::DestroyWndDisplay()
{
	if(m_pcWndDisplay)
	{
		m_pcWndDisplay->DestroyWindow();
	}
	
	delete m_pcWndDisplay;
	m_pcWndDisplay=NULL;

	return false;
}
void CViewService::ShowWndDisplay(bool show)
{
	if(m_pcWndDisplay)
	{
		m_pcWndDisplay->Show(show);
		m_pcWndDisplay->SetFocus();
	}
	setCurServiceWnd(SERVICE_DISPL);
}

// **************************************************************************
// 
// **************************************************************************
bool CViewService::CreateWndSettings()
{
	if(m_pcWndSettings==NULL)
	{
		m_pcWndSettings = new CWndServiceSettings();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={10,10,790,590};

		if(m_pcWndSettings->Create(this,rcLd,IDC_VIEW_SERVICE_SETTING))
		{
			return true;
		}
	}
	return false;
}
bool CViewService::DestroyWndSettings()
{
	if(m_pcWndSettings)
	{
		m_pcWndSettings->DestroyWindow();
	}
	
	delete m_pcWndSettings;
	m_pcWndSettings=NULL;

	return false;
}
void CViewService::ShowWndSettings(bool show)
{
	if(m_pcWndSettings)
	{
		m_pcWndSettings->Show(show);
		m_pcWndSettings->SetFocus();
	}
	setCurServiceWnd(SERVICE_SETTINGS);
}

// **************************************************************************
// 
// **************************************************************************
bool CViewService::CreateWndCalibration()
{
	if(m_pcWndCalibration==NULL)
	{
		m_pcWndCalibration = new CWndServiceCalibration();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={10,10,790,590};

		if(m_pcWndCalibration->Create(this,rcLd,IDC_VIEW_SERVICE_CAL))
		{
			return true;
		}
	}
	return false;
}
bool CViewService::DestroyWndCalibration()
{
	if(m_pcWndCalibration)
	{
		m_pcWndCalibration->DestroyWindow();
	}
	
	delete m_pcWndCalibration;
	m_pcWndCalibration=NULL;

	return false;
}
void CViewService::ShowWndCalibration(bool show)
{
	if(m_pcWndCalibration)
	{
		m_pcWndCalibration->Show(show);
		m_pcWndCalibration->SetFocus();
	}
	setCurServiceWnd(SERVICE_CAL);
}

// **************************************************************************
// 
// **************************************************************************
bool CViewService::CreateWndFlowboard()
{
	if(m_pcWndFlowboard==NULL)
	{
		m_pcWndFlowboard = new CWndServiceFlowboard();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={10,10,790,590};

		if(m_pcWndFlowboard->Create(this,rcLd,IDC_VIEW_SERVICE_FLOW))
		{
			return true;
		}
	}
	return false;
}
bool CViewService::DestroyWndFlowboard()
{
	if(m_pcWndFlowboard)
	{
		m_pcWndFlowboard->DestroyWindow();
	}
	
	delete m_pcWndFlowboard;
	m_pcWndFlowboard=NULL;

	return false;
}
void CViewService::ShowWndFlowboard(bool show)
{
	if(m_pcWndFlowboard)
	{
		m_pcWndFlowboard->Show(show);
		m_pcWndFlowboard->SetFocus();
	}
	setCurServiceWnd(SERVICE_FLOW);
}

// **************************************************************************
// 
// **************************************************************************
bool CViewService::CreateWndAccuboard()
{
	if(m_pcWndAccuboard==NULL)
	{
		m_pcWndAccuboard = new CWndServiceAccuboard();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={10,10,790,590};

		if(m_pcWndAccuboard->Create(this,rcLd,IDC_VIEW_SERVICE_ACCU))
		{
			return true;
		}
	}
	return false;
}
bool CViewService::DestroyWndAccuboard()
{
	if(m_pcWndAccuboard)
	{
		m_pcWndAccuboard->DestroyWindow();
	}
	
	delete m_pcWndAccuboard;
	m_pcWndAccuboard=NULL;

	return false;
}
void CViewService::ShowWndAccuboard(bool show)
{
	if(m_pcWndAccuboard)
	{
		m_pcWndAccuboard->Show(show);
		m_pcWndAccuboard->SetFocus();
	}
	setCurServiceWnd(SERVICE_ACCU);
}

// **************************************************************************
// 
// **************************************************************************
bool CViewService::CreateWndInterfaces()
{
	if(m_pcWndInterfaces==NULL)
	{
		m_pcWndInterfaces = new CWndServiceInterfaces();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={10,10,790,590};

		if(m_pcWndInterfaces->Create(this,rcLd,IDC_VIEW_SERVICE_IF))
		{
			return true;
		}
	}
	return false;
}
bool CViewService::DestroyWndInterfaces()
{
	if(m_pcWndInterfaces)
	{
		m_pcWndInterfaces->DestroyWindow();
	}
	
	delete m_pcWndInterfaces;
	m_pcWndInterfaces=NULL;

	return false;
}
void CViewService::ShowWndInterfaces(bool show)
{
	if(m_pcWndInterfaces)
	{
		m_pcWndInterfaces->Show(show);
		m_pcWndInterfaces->SetFocus();
	}
	setCurServiceWnd(SERVICE_IF);
}

// **************************************************************************
// 
// **************************************************************************
bool CViewService::CreateWndNetwork()
{
	if(m_pcWndNetwork==NULL)
	{
		m_pcWndNetwork = new CWndServiceNetwork();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={10,10,790,590};

		if(m_pcWndNetwork->Create(this,rcLd,IDC_VIEW_SERVICE_NETWORK))
		{
			return true;
		}
	}
	return false;
}
bool CViewService::DestroyWndNetwork()
{
	if(m_pcWndNetwork)
	{
		m_pcWndNetwork->DestroyWindow();
	}
	
	delete m_pcWndNetwork;
	m_pcWndNetwork=NULL;

	return false;
}
void CViewService::ShowWndNetwork(bool show)
{
	if(m_pcWndNetwork)
	{
		m_pcWndNetwork->Show(show);
		m_pcWndNetwork->SetFocus();
	}
	setCurServiceWnd(SERVICE_NETWORK);
}

// **************************************************************************
// 
// **************************************************************************
bool CViewService::CreateWndLicensing()
{
	if(m_pcWndLicensing==NULL)
	{
		m_pcWndLicensing = new CWndServiceLicensing();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={10,10,790,590};

		if(m_pcWndLicensing->Create(this,rcLd,IDC_VIEW_SERVICE_LICENSING))
		{
			return true;
		}
	}
	return false;
}
bool CViewService::DestroyWndLicensing()
{
	try
	{
		if(m_pcWndLicensing)
		{
			m_pcWndLicensing->DestroyWindow();
		}
	}
	catch (...)
	{
		CFabianHFOApp::ReportException(_T("CViewService::DestroyWndLicensing"));
		delete m_pcWndLicensing;
		throw;
	}
	
	delete m_pcWndLicensing;
	m_pcWndLicensing=NULL;

	return false;
}
void CViewService::ShowWndLicensing(bool show)
{
	if(m_pcWndLicensing)
	{
		m_pcWndLicensing->Show(show);
		m_pcWndLicensing->SetFocus();
	}
	setCurServiceWnd(SERVICE_LICENSING);
}


// **************************************************************************
// 
// **************************************************************************
bool CViewService::CreateWndValves()
{
	if(m_pcWndValves==NULL)
	{
		m_pcWndValves = new CWndServiceValves();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={10,10,790,590};

		if(m_pcWndValves->Create(this,rcLd,IDC_VIEW_SERVICE_VALV))
		{
			return true;
		}
	}
	return false;
}
bool CViewService::DestroyWndValves()
{
	if(m_pcWndValves)
	{
		m_pcWndValves->DestroyWindow();
	}
	
	delete m_pcWndValves;
	m_pcWndValves=NULL;

	return false;
}
void CViewService::ShowWndValves(bool show)
{
	if(m_pcWndValves)
	{
		m_pcWndValves->Show(show);
		m_pcWndValves->SetFocus();
	}
	setCurServiceWnd(SERVICE_VALV);
}

// **************************************************************************
// 
// **************************************************************************
bool CViewService::CreateWndMonitorPIC()
{
	if(m_pcWndMonitorPIC==NULL)
	{
		m_pcWndMonitorPIC = new CWndServiceMonitorPIC();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={10,10,790,590};

		if(m_pcWndMonitorPIC->Create(this,rcLd,IDC_VIEW_SERVICE_MON))
		{
			return true;
		}
	}
	return false;
}
bool CViewService::DestroyWndMonitorPIC()
{
	if(m_pcWndMonitorPIC)
	{
		m_pcWndMonitorPIC->DestroyWindow();
	}
	
	delete m_pcWndMonitorPIC;
	m_pcWndMonitorPIC=NULL;

	return false;
}
void CViewService::ShowWndMonitorPIC(bool show)
{
	if(m_pcWndMonitorPIC)
	{
		m_pcWndMonitorPIC->Show(show);
		m_pcWndMonitorPIC->SetFocus();
	}
	setCurServiceWnd(SERVICE_MON);
}

// **************************************************************************
// 
// **************************************************************************
void CViewService::Open()
{

}

// **************************************************************************
// 
// **************************************************************************
void CViewService::Close()
{


}

// **************************************************************************
// 
// **************************************************************************
void CViewService::Show(bool bRedraw)
{
	if(bRedraw)
	{
		switch(getCurServiceWnd())
		{
		case SERVICE_SYS:
			{
				ShowWndSystem(true);
			}
			break;
		case SERVICE_DISPL:
			{
				ShowWndDisplay(true);
			}
			break;
		case SERVICE_SETTINGS:
			{
				ShowWndSettings(true);
			}
			break;
		case SERVICE_CAL:
			{
				ShowWndCalibration(true);
			}
			break;
		case SERVICE_FLOW:
			{
				ShowWndFlowboard(true);
			}
			break;
		case SERVICE_ACCU:
			{
				ShowWndAccuboard(true);
			}
			break;
		case SERVICE_VALV:
			{
				ShowWndValves(true);
			}
			break;
		case SERVICE_MON:
			{
				ShowWndMonitorPIC(true);
			}
			break;
		case SERVICE_IF:
			{
				ShowWndInterfaces(true);
			}
			break;
		case SERVICE_NETWORK:
			{
				ShowWndNetwork(true);
			}
			break;
		case SERVICE_LICENSING:
			{
				ShowWndLicensing(true);
			}
			break;
		case SERVICE_MENU:
			{
				ShowWndServiceMenu(true);
			}
			break;
		default:
			{
				this->SetFocus();
			}
			break;
		}
	}
	else
	{
		CreateWndServiceMenu();
		ShowWndServiceMenu(true);
	}

	

	

	this->ShowWindow(SW_SHOW);
}


// **************************************************************************
// 
// **************************************************************************
void CViewService::Hide()
{
	this->ShowWindow(SW_HIDE);
}

// **************************************************************************
// 
// **************************************************************************
void CViewService::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);


	// CMVView::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
}

// **************************************************************************
// 
// **************************************************************************
void CViewService::SetViewFocus()
{
	switch(getCurServiceWnd())
	{
	case SERVICE_SYS:
		{
			if(m_pcWndSystem)
				m_pcWndSystem->SetFocus();
		}
		break;
	case SERVICE_DISPL:
		{
			if(m_pcWndDisplay)
				m_pcWndDisplay->SetFocus();
		}
		break;
	case SERVICE_SETTINGS:
		{
			if(m_pcWndSettings)
				m_pcWndSettings->SetFocus();
		}
		break;
	case SERVICE_CAL:
		{
			if(m_pcWndCalibration)
				m_pcWndCalibration->SetFocus();
		}
		break;
	case SERVICE_FLOW:
		{
			if(m_pcWndFlowboard)
				m_pcWndFlowboard->SetFocus();
		}
		break;
	case SERVICE_ACCU:
		{
			if(m_pcWndAccuboard)
				m_pcWndAccuboard->SetFocus();
		}
		break;
	case SERVICE_VALV:
		{
			if(m_pcWndValves)
				m_pcWndValves->SetFocus();
		}
		break;
	case SERVICE_MON:
		{
			if(m_pcWndMonitorPIC)
				m_pcWndMonitorPIC->SetFocus();
		}
		break;
	case SERVICE_IF:
		{
			if(m_pcWndInterfaces)
				m_pcWndInterfaces->SetFocus();
		}
		break;
	case SERVICE_NETWORK:
		{
			if(m_pcWndNetwork)
				m_pcWndNetwork->SetFocus();
		}
		break;
	case SERVICE_LICENSING:
		{
			if(m_pcWndLicensing)
				m_pcWndLicensing->SetFocus();
		}
		break;
	case SERVICE_MENU:
		{
			if(m_pWndServiceMenu)
				m_pWndServiceMenu->SetFocus();
		}
		break;
	case SERVICE_MAINTENANCE: 
		{
			if(m_pcWndMaintenance)
				m_pcWndMaintenance->SetFocus();
		}
		break;
	default:
		{
			this->SetFocus();
		}
		break;
	}

}

// **************************************************************************
// 
// **************************************************************************
void CViewService::NotifyEvent(CMVEvent* pEvent)
{
	switch(pEvent->GetET())
	{
	case CMVEvent::ET_UIEVENT:
		{
			CMVEventUI* pUIEvent = (CMVEventUI*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventUI::EV_BN_SERVICE_SYSTEM:
				{
					DestroyWndServiceMenu();

					UpdateWindow();
					Invalidate();

					CreateWndSystem();
					ShowWndSystem(true);
					m_pcWndSystem->SetFocus();

					PostMessage(WM_BN_SERVICE_SYSTEM);
				}
				break;
			case CMVEventUI::EV_BN_SERVICE_DISPLAY:
				{
					DestroyWndServiceMenu();

					UpdateWindow();
					Invalidate();

					CreateWndDisplay();
					ShowWndDisplay(true);
					m_pcWndDisplay->SetFocus();

					PostMessage(WM_BN_SERVICE_DISPLAY);
				}
				break;
			case CMVEventUI::EV_BN_SERVICE_SETTINGS:
				{
					DestroyWndServiceMenu();

					UpdateWindow();
					Invalidate();

					CreateWndSettings();
					ShowWndSettings(true);
					m_pcWndSettings->SetFocus();

					PostMessage(WM_BN_SERVICE_SETTINGS);
				}
				break;
			case CMVEventUI::EV_BN_SERVICE_CALIBR:
				{
					DestroyWndServiceMenu();

					UpdateWindow();
					Invalidate();

					CreateWndCalibration();
					ShowWndCalibration(true);
					m_pcWndCalibration->SetFocus();

					PostMessage(WM_BN_SERVICE_CALIBR);
				}
				break;
			case CMVEventUI::EV_BN_SERVICE_FLOW:
				{
					DestroyWndServiceMenu();

					UpdateWindow();
					Invalidate();

					CreateWndFlowboard();
					ShowWndFlowboard(true);
					m_pcWndFlowboard->SetFocus();

					PostMessage(WM_BN_SERVICE_FLOW);
				}
				break;
			case CMVEventUI::EV_BN_SERVICE_ACCU:
				{
					DestroyWndServiceMenu();

					UpdateWindow();
					Invalidate();

					CreateWndAccuboard();
					ShowWndAccuboard(true);
					m_pcWndAccuboard->SetFocus();

					PostMessage(WM_BN_SERVICE_ACCU);
				}
				break;
			case CMVEventUI::EV_BN_SERVICE_VALVES:
				{
					DestroyWndServiceMenu();

					UpdateWindow();
					Invalidate();

					CreateWndValves();
					ShowWndValves(true);
					m_pcWndValves->SetFocus();

					PostMessage(WM_BN_SERVICE_VALVES);
				}
				break;
			case CMVEventUI::EV_BN_SERVICE_MONITOR:
				{
					DestroyWndServiceMenu();

					UpdateWindow();
					Invalidate();

					CreateWndMonitorPIC();
					ShowWndMonitorPIC(true);
					m_pcWndMonitorPIC->SetFocus();

					PostMessage(WM_BN_SERVICE_MONITOR);
				}
				break;
			case CMVEventUI::EV_BN_SERVICE_IF:
				{
					DestroyWndServiceMenu();

					UpdateWindow();
					Invalidate();

					CreateWndInterfaces();
					ShowWndInterfaces(true);
					m_pcWndInterfaces->SetFocus();

					PostMessage(WM_BN_SERVICE_IF);
				}
				break;
			case CMVEventUI::EV_BN_SERVICE_LICENSING:
				{
					DestroyWndServiceMenu();

					UpdateWindow();
					Invalidate();

					CreateWndLicensing();
					ShowWndLicensing(true);
					m_pcWndLicensing->SetFocus();

					PostMessage(WM_BN_SERVICE_LICENSING);
				}
				break;
			case CMVEventUI::EV_BN_SERVICE_MAINTENANCE: 
				{
					DestroyWndServiceMenu();

					UpdateWindow();
					Invalidate();

					CreateWndMaintenance();
					ShowWndMaintenance(true);
					if(m_pcWndMaintenance)
						m_pcWndMaintenance->SetFocus();

					PostMessage(WM_BN_SERVICE_MAINTENANCE);
				}
				break;
			case CMVEventUI::EV_BN_SERVICE_NETWORK:
				{
					DestroyWndServiceMenu();

					UpdateWindow();
					Invalidate();

					CreateWndNetwork();
					ShowWndNetwork(true);
					m_pcWndNetwork->SetFocus();

					PostMessage(WM_BN_SERVICE_NETWORK);
				}
				break;
			case CMVEventUI::EV_BN_SERVICE_BACK:
				{
					DestroyWndSystem();
					DestroyWndDisplay();
					DestroyWndSettings();
					DestroyWndCalibration();
					DestroyWndFlowboard();
					DestroyWndAccuboard();
					DestroyWndValves();
					DestroyWndMonitorPIC();
					DestroyWndInterfaces();
					DestroyWndNetwork();
					DestroyWndLicensing();
					DestroyWndMaintenance();

					UpdateWindow();
					Invalidate();

					CreateWndServiceMenu();
					ShowWndServiceMenu(true);
					m_pWndServiceMenu->SetFocus();

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_CLEAR_TEXT);
				}
				break;
			default:
				break;
			}
		}
	}
}


//************************************
// Method:    OnDestroy
// FullName:  CViewService::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CViewService::OnDestroy()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_CLEAR_TEXT);

	theApp.getLog()->WriteLine(_T("*** Service mode finished ***"));

	KillTimer(SERVICETIMER);

	//
	DestroyWndSystem();
	DestroyWndDisplay();
	DestroyWndSettings();
	DestroyWndCalibration();
	DestroyWndFlowboard();
	DestroyWndAccuboard();
	DestroyWndValves();
	DestroyWndMonitorPIC();
	DestroyWndInterfaces();
	DestroyWndNetwork();
	DestroyWndLicensing();
	DestroyWndMaintenance();
	DestroyWndServiceMenu();

	CMVView::OnDestroy();
}

// **************************************************************************
// 
// **************************************************************************
bool CViewService::SetPrevFocus()
{
	return true;
}

bool CViewService::SetNextFocus()
{
	return true;
}

LRESULT CViewService::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_SETVIEWFOCUS:
		{
			//rku cs1
			SetViewFocus();
			return 1;
		}
		break;
	case WM_SETPREVFOCUS:
		{
			//rku cs1
			SetPrevFocus();
			return 1;
		}
		break;
	case WM_SETNEXTFOCUS:
		{
			//rku cs1
			SetNextFocus();
			return 1;
		}
		break;
	case WM_UPDATE_VIEW:
		{
			if(getCurServiceWnd()==SERVICE_DISPL)
			{
				m_pcWndDisplay->Show(true);

				m_pcWndDisplay->SetFocus();
			}

			return 1;
		}
		break;
	case WM_BN_SERVICE_SYSTEM:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TEXT_SERVICE_SYSTEM);
			return 1;
		}
		break;
	case WM_BN_SERVICE_DISPLAY:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TEXT_SERVICE_DISPLAY);
			return 1;
		}
		break;
	case WM_BN_SERVICE_SETTINGS:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TEXT_SERVICE_SETTINGS);
			return 1;
		}
		break;
	case WM_BN_SERVICE_CALIBR:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TEXT_SERVICE_CALIBRATION);
			return 1;
		}
		break;
	case WM_BN_SERVICE_FLOW:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TEXT_SERVICE_FLOWBOARD);
			return 1;
		}
		break;
	case WM_BN_SERVICE_ACCU:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TEXT_SERVICE_ACCUBOARD);
			return 1;
		}
		break;
	case WM_BN_SERVICE_VALVES:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TEXT_SERVICE_VALVES);
			return 1;
		}
		break;
	case WM_BN_SERVICE_MONITOR:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TEXT_SERVICE_MONITOR);
			return 1;
		}
		break;
	case WM_BN_SERVICE_IF:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TEXT_SERVICE_INTERFACES);
			return 1;
		}
		break;
	case WM_BN_SERVICE_NETWORK:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TEXT_SERVICE_NETWORK);
			return 1;
		}
		break;
	case WM_BN_SERVICE_LICENSING:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TEXT_SERVICE_LICENSING);
			return 1;
		}
		break;
	case WM_BN_SERVICE_MAINTENANCE: 
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TEXT_SERVICE_MAINTENANCE);
			return 1;
		}
		break;
	case WM_EV_BN_SERVICE_BACK:
		{
			DestroyWndSystem();
			DestroyWndDisplay();
			DestroyWndSettings();
			DestroyWndCalibration();
			DestroyWndFlowboard();
			DestroyWndAccuboard();
			DestroyWndValves();
			DestroyWndMonitorPIC();
			DestroyWndInterfaces();
			DestroyWndNetwork();
			DestroyWndLicensing();
			DestroyWndMaintenance();

			UpdateWindow();
			Invalidate();

			CreateWndServiceMenu();
			ShowWndServiceMenu(true);
			m_pWndServiceMenu->SetFocus();

			AfxGetMainWnd()->PostMessage(WM_EV_CLEAR_TEXT);
		}
		break;
	default:
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

void CViewService::setCurServiceWnd(eServiceView serviceWnd)
{
	EnterCriticalSection(&csServiceWnd);
	m_eCurServiceWnd=serviceWnd;
	LeaveCriticalSection(&csServiceWnd);
}

eServiceView CViewService::getCurServiceWnd()
{
	EnterCriticalSection(&csServiceWnd);
	eServiceView serviceWnd=m_eCurServiceWnd;
	LeaveCriticalSection(&csServiceWnd);
	return serviceWnd;
}