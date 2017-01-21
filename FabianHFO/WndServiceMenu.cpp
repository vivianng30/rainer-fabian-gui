// WndServiceMenu.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndServiceMenu.h"

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000

//global font objects
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf8AcuNorm;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf13AcuBold;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf15AcuMed;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf19AcuMed;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf23AcuBold;
// CWndServiceMenu

IMPLEMENT_DYNAMIC(CWndServiceMenu, CWnd)

CWndServiceMenu::CWndServiceMenu():
CWndService()
{
	m_pcMenuSystem=NULL;
	m_pcMenuDisplay=NULL;
	m_pcMenuSettings=NULL;
	m_pcMenuCalibration=NULL;
	m_pcMenuFlowboard=NULL;
	m_pcMenuAccuboard=NULL;
	m_pcMenuValves=NULL;
	m_pcMenuMonitorPIC=NULL;
	m_pcMenuInterfaces=NULL;
	m_pcMenuNetwork=NULL;
	m_pcMenuLicensing=NULL;
	m_pcMenuMaintenance=NULL;

	m_pcMenuBACK=NULL;

	m_pcMenu_UP=NULL;
	m_pcMenu_DW=NULL;
	m_pcMenu_FC=NULL;
	m_pcMenu_DIS=NULL;

	m_pcMenu_BACK_UP=NULL;
	m_pcMenu_BACK_DW=NULL;
	//m_pcMenu_EXIT_FC=NULL;
}

CWndServiceMenu::~CWndServiceMenu()
{
	delete m_pcMenuSystem;
	delete m_pcMenuDisplay;
	delete m_pcMenuSettings;
	delete m_pcMenuCalibration;
	delete m_pcMenuFlowboard;
	delete m_pcMenuAccuboard;
	delete m_pcMenuValves;
	delete m_pcMenuMonitorPIC;
	delete m_pcMenuInterfaces;
	delete m_pcMenuNetwork;
	delete m_pcMenuLicensing;
	delete m_pcMenuMaintenance;
	delete m_pcMenuBACK;

	m_pcMenuSystem=NULL;
	m_pcMenuDisplay=NULL;
	m_pcMenuSettings=NULL;
	m_pcMenuCalibration=NULL;
	m_pcMenuFlowboard=NULL;
	m_pcMenuAccuboard=NULL;
	m_pcMenuValves=NULL;
	m_pcMenuMonitorPIC=NULL;
	m_pcMenuInterfaces=NULL;
	m_pcMenuNetwork=NULL;
	m_pcMenuLicensing=NULL;
	m_pcMenuMaintenance=NULL;
	m_pcMenuBACK=NULL;

	delete m_pcMenu_UP;
	delete m_pcMenu_DW;
	delete m_pcMenu_FC;
	delete m_pcMenu_DIS;
	delete m_pcMenu_BACK_UP;
	delete m_pcMenu_BACK_DW;
	//delete m_pcMenu_EXIT_FC;

	m_pcMenu_UP=NULL;
	m_pcMenu_DW=NULL;
	m_pcMenu_FC=NULL;
	m_pcMenu_DIS=NULL;
	m_pcMenu_BACK_UP=NULL;
	m_pcMenu_BACK_DW=NULL;
	//m_pcMenu_EXIT_FC=NULL;
}


BEGIN_MESSAGE_MAP(CWndServiceMenu, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SERVICE_SYSTEM, &CWndServiceMenu::OnBnClickedSystem)
	ON_BN_CLICKED(IDC_BTN_SERVICE_CAL, &CWndServiceMenu::OnBnClickedCalibration)
	ON_BN_CLICKED(IDC_BTN_SERVICE_FLOW, &CWndServiceMenu::OnBnClickedFlowboard)
	ON_BN_CLICKED(IDC_BTN_SERVICE_MONITOR, &CWndServiceMenu::OnBnClickedMonitorPIC)
	ON_BN_CLICKED(IDC_BTN_SERVICE_ACCU, &CWndServiceMenu::OnBnClickedAccuboard)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALVES, &CWndServiceMenu::OnBnClickedValves)
	ON_BN_CLICKED(IDC_BTN_SERVICE_DISPLAY, &CWndServiceMenu::OnBnClickedDisplay)
	ON_BN_CLICKED(IDC_BTN_SERVICE_SETTINGS, &CWndServiceMenu::OnBnClickedSettings)
	ON_BN_CLICKED(IDC_BTN_SERVICE_IF, &CWndServiceMenu::OnBnClickedInterfaces)
	ON_BN_CLICKED(IDC_BTN_SERVICE_NETWORK, &CWndServiceMenu::OnBnClickedNetwork)
	ON_BN_CLICKED(IDC_BTN_SERVICE_LICENSING, &CWndServiceMenu::OnBnClickedLicensing)
	ON_BN_CLICKED(IDC_BTN_SERVICE_MAINTENANCE, &CWndServiceMenu::OnBnClickedMaintenance)
	ON_BN_CLICKED(IDC_BTN_SERVICE_EXIT, &CWndServiceMenu::OnBnClickedBACK)
END_MESSAGE_MAP()



// CWndServiceMenu message handlers
void CWndServiceMenu::Init()
{
	CClientDC dc(this);

	m_pcMenu_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SELMENU_UP);
	m_pcMenu_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SELMENU_DW);
	//m_pcMenu_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SELMENU_FC);
	m_pcMenu_DIS= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SELMENU_DIS);

	/*m_pcMenu_EXIT_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SELMENU_EXIT_UP);
	m_pcMenu_EXIT_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SELMENU_EXIT_DW);
	m_pcMenu_EXIT_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SELMENU_EXIT_FC);*/

	m_pcMenu_BACK_UP		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUM_BACK_UP);
	m_pcMenu_BACK_DW		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUM_BACK_DW);

	CStringW cs=_T("");
	BTN btn;

	//Menu Buttons
	/********************************************************/
	btn.wID					= IDC_BTN_SERVICE_SYSTEM;	
	btn.poPosition.x		= 44;
	btn.poPosition.y		= 34;
	btn.pcBmpUp				= m_pcMenu_UP;
	btn.pcBmpDown			= m_pcMenu_DW;
	btn.pcBmpFocus			= m_pcMenu_FC;
	btn.pcBmpDisabled		= m_pcMenu_DIS;
	btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	m_pcMenuSystem=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuSystem->Create(this,g_hf13AcuBold,0,40);
	m_pcMenuSystem->SetText(_T("System"),_T("System"));
	//m_pcMenuxxx->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuSystem);

	/********************************************************/
	btn.wID					= IDC_BTN_SERVICE_CAL;	
	btn.poPosition.x		= 233;
	btn.poPosition.y		= 34;
	btn.pcBmpUp				= m_pcMenu_UP;
	btn.pcBmpDown			= m_pcMenu_DW;
	btn.pcBmpFocus			= m_pcMenu_FC;
	btn.pcBmpDisabled		= m_pcMenu_DIS;
	btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	m_pcMenuCalibration=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuCalibration->Create(this,g_hf13AcuBold,0,40);
	m_pcMenuCalibration->SetText(_T("Calibration"),_T("Calibration"));
	//m_pcMenuDisplay->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuCalibration);

	/********************************************************/
	btn.wID					= IDC_BTN_SERVICE_FLOW;	
	btn.poPosition.x		= 422;
	btn.poPosition.y		= 34;
	btn.pcBmpUp				= m_pcMenu_UP;
	btn.pcBmpDown			= m_pcMenu_DW;
	btn.pcBmpFocus			= m_pcMenu_FC;
	btn.pcBmpDisabled		= m_pcMenu_DIS;
	btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	m_pcMenuFlowboard=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuFlowboard->Create(this,g_hf13AcuBold,0,40);
	m_pcMenuFlowboard->SetText(_T("Blenderboard"),_T("Blenderboard"));
	//m_pcMenuVentilation->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuFlowboard);

	/********************************************************/
	btn.wID					= IDC_BTN_SERVICE_MONITOR;	
	btn.poPosition.x		= 611;
	btn.poPosition.y		= 34;
	btn.pcBmpUp				= m_pcMenu_UP;
	btn.pcBmpDown			= m_pcMenu_DW;
	btn.pcBmpFocus			= m_pcMenu_FC;
	btn.pcBmpDisabled		= m_pcMenu_DIS;
	btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	m_pcMenuMonitorPIC=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuMonitorPIC->Create(this,g_hf13AcuBold,0,40);
	m_pcMenuMonitorPIC->SetText(_T("Monitor PIC"),_T("Monitor PIC"));
	//m_pcMenuPatient->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuMonitorPIC);

	/********************************************************/
	btn.wID					= IDC_BTN_SERVICE_ACCU;	
	btn.poPosition.x		= 44;
	btn.poPosition.y		= 173;
	btn.pcBmpUp				= m_pcMenu_UP;
	btn.pcBmpDown			= m_pcMenu_DW;
	btn.pcBmpFocus			= m_pcMenu_FC;
	btn.pcBmpDisabled		= m_pcMenu_DIS;
	btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	m_pcMenuAccuboard=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuAccuboard->Create(this,g_hf13AcuBold,0,40);
	m_pcMenuAccuboard->SetText(_T("Accuboard"),_T("Accuboard"));
	//m_pcMenuLanguage->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuAccuboard);

	/********************************************************/
	btn.wID					= IDC_BTN_SERVICE_VALVES;	
	btn.poPosition.x		= 233;
	btn.poPosition.y		= 173;
	btn.pcBmpUp				= m_pcMenu_UP;
	btn.pcBmpDown			= m_pcMenu_DW;
	btn.pcBmpFocus			= m_pcMenu_FC;
	btn.pcBmpDisabled		= m_pcMenu_DIS;
	btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	m_pcMenuValves=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuValves->Create(this,g_hf13AcuBold,0,40);
	m_pcMenuValves->SetText(_T("Valves"),_T("Valves"));
	//m_pcMenuDate->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuValves);


	/********************************************************/
	btn.wID					= IDC_BTN_SERVICE_DISPLAY;	
	btn.poPosition.x		= 422;
	btn.poPosition.y		= 173;
	btn.pcBmpUp				= m_pcMenu_UP;
	btn.pcBmpDown			= m_pcMenu_DW;
	btn.pcBmpFocus			= m_pcMenu_FC;
	btn.pcBmpDisabled		= m_pcMenu_DIS;
	btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	m_pcMenuDisplay=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuDisplay->Create(this,g_hf13AcuBold,0,40);
	m_pcMenuDisplay->SetText(_T("Display"),_T("Display"));
	//m_pcMenuDisplay->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuDisplay);

	/********************************************************/
	btn.wID					= IDC_BTN_SERVICE_SETTINGS;	
	btn.poPosition.x		= 611;
	btn.poPosition.y		= 173;
	btn.pcBmpUp				= m_pcMenu_UP;
	btn.pcBmpDown			= m_pcMenu_DW;
	btn.pcBmpFocus			= m_pcMenu_FC;
	btn.pcBmpDisabled		= m_pcMenu_DIS;
	btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	m_pcMenuSettings=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuSettings->Create(this,g_hf13AcuBold,0,40);
	m_pcMenuSettings->SetText(_T("Settings"),_T("Settings"));
	//m_pcMenuInfo->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuSettings);



	/********************************************************/
	btn.wID					= IDC_BTN_SERVICE_IF;	
	btn.poPosition.x		= 44;
	btn.poPosition.y		= 312;
	btn.pcBmpUp				= m_pcMenu_UP;
	btn.pcBmpDown			= m_pcMenu_DW;
	btn.pcBmpFocus			= m_pcMenu_FC;
	btn.pcBmpDisabled		= m_pcMenu_DIS;
	btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	m_pcMenuInterfaces=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuInterfaces->Create(this,g_hf13AcuBold,0,40);
	m_pcMenuInterfaces->SetText(_T("Interfaces"),_T("Interfaces"));
	//m_pcMenuService->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuInterfaces);

	/********************************************************/
	btn.wID					= IDC_BTN_SERVICE_NETWORK;	
	btn.poPosition.x		= 233;
	btn.poPosition.y		= 312;
	btn.pcBmpUp				= m_pcMenu_UP;
	btn.pcBmpDown			= m_pcMenu_DW;
	btn.pcBmpFocus			= m_pcMenu_FC;
	btn.pcBmpDisabled		= m_pcMenu_DIS;
	btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	m_pcMenuNetwork=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuNetwork->Create(this,g_hf13AcuBold,0,40);
	m_pcMenuNetwork->SetText(_T("Network"),_T("Network"));
	//m_pcMenuNetwork->EnableWindow(FALSE);
	m_plMenuBtn.AddTail(m_pcMenuNetwork);

	/***********************000*********************************/
	btn.wID					= IDC_BTN_SERVICE_LICENSING;	
	btn.poPosition.x		= 422;
	btn.poPosition.y		= 312;
	btn.pcBmpUp				= m_pcMenu_UP;
	btn.pcBmpDown			= m_pcMenu_DW;
	btn.pcBmpFocus			= m_pcMenu_FC;
	btn.pcBmpDisabled		= m_pcMenu_DIS;
	btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	m_pcMenuLicensing=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuLicensing->Create(this,g_hf13AcuBold,0,40);
	m_pcMenuLicensing->SetText(_T("Licensing"),_T("Licensing"));
	//m_pcMenuLicensing->EnableWindow(FALSE);
	m_plMenuBtn.AddTail(m_pcMenuNetwork);

	/********************************************************/
	//pro, added
	btn.wID					= IDC_BTN_SERVICE_MAINTENANCE;
	btn.poPosition.x		= 611;
	btn.poPosition.y		= 312;
	btn.pcBmpUp				= m_pcMenu_UP;
	btn.pcBmpDown			= m_pcMenu_DW;
	btn.pcBmpFocus			= m_pcMenu_FC;
	btn.pcBmpDisabled		= m_pcMenu_DIS;
	btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	m_pcMenuMaintenance=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuMaintenance->Create(this,g_hf13AcuBold,0,40);
	m_pcMenuMaintenance->SetText(_T("Maintenance"),_T("Maintenance"));
	//m_pcMenuMaintenance->EnableWindow(FALSE);
	m_plMenuBtn.AddTail(m_pcMenuMaintenance);

	/*****************************HELP***************************/
	//btn.wID					= IDC_BTNMNU_HELP;	
	//btn.poPosition.x		= 422;
	//btn.poPosition.y		= 317;
	//btn.pcBmpUp				= m_pcMenuHelp_UP;
	//btn.pcBmpDown			= m_pcMenuHelp_DW;
	//btn.pcBmpFocus			= m_pcMenuHelp_FC;
	//btn.pcBmpDisabled		= m_pcMenuHelp_DIS;
	////btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;
	//btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	//m_pcMenuHelp=new CMenuBtn(btn,COLOR_TXTBTNUP);
	//m_pcMenuHelp->Create(this,g_hf13AcuBold,0,10);
	//m_pcMenuHelp->SetText(getModel()->GetLanguageString(IDS_BTN_MNU_HELP),getModel()->GetLanguageString(IDS_BTN_MNU_HELP));
	////m_pcMenuHelp->EnableWindow(FALSE);
	//m_pcMenuHelp->ShowWindow(SW_HIDE);
	////m_plMenuBtn.AddTail(m_pcMenuHelp);



	/********************************************************/
	btn.wID					= IDC_BTN_SERVICE_EXIT;	
	btn.poPosition.x		= 610;//611;
	btn.poPosition.y		= 470;//435;
	btn.pcBmpUp				= m_pcMenu_BACK_UP;
	btn.pcBmpDown			= m_pcMenu_BACK_DW;
	btn.pcBmpFocus			= m_pcMenu_BACK_UP;
	btn.pcBmpDisabled		= m_pcMenu_BACK_UP;
	btn.dwFormat			= DT_BOTTOM|DT_SINGLELINE|DT_CENTER;

	m_pcMenuBACK=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuBACK->Create(this,g_hf13AcuBold,0,10);
	m_pcMenuBACK->SetText(_T(""),_T(""));
	//m_pcMenuCO2->EnableWindow(FALSE);
	m_plMenuBtn.AddTail(m_pcMenuBACK);


	//SetTimer(SERVICETIMER, 500, NULL);

}




//************************************
// Method:    OnDestroy
// FullName:  CWndServiceMenu::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndServiceMenu::OnDestroy()
{
	m_plMenuBtn.RemoveAll();

	CWndService::OnDestroy();
}


// **************************************************************************
// 
// **************************************************************************
void CWndServiceMenu::SetOneButtonDepressed(int btnID)
{

	POSITION pos;
	try
	{
		for( pos = m_plMenuBtn.GetHeadPosition(); pos != NULL; )
		{
			CMenuBtn* pUDBtn = m_plMenuBtn.GetNext( pos );

			if(pUDBtn->GetBtnId() != btnID)
			{
				if(pUDBtn->IsDepressed())
				{
					pUDBtn->Depress(false);
				}
			}
			else
			{
				pUDBtn->Depress(true);
			}
		}
	}
	catch (...)
	{
		CFabianHFOApp::ReportException(_T("EXCEPTION: CWndServiceMenu::SetOneButtonDepressed"));
		//AfxMessageBox( _T("EXCEPTION: CMVModel::VentModeChanged()()") );

	}
	



}

// **************************************************************************
// 
// **************************************************************************
void CWndServiceMenu::SetAllButtonUnpressed()
{
	POSITION pos;

	try
	{
		for( pos = m_plMenuBtn.GetHeadPosition(); pos != NULL; )
		{
			CMenuBtn* pUDBtn = m_plMenuBtn.GetNext( pos );

			if(pUDBtn->IsDepressed())
			{
				pUDBtn->Depress(false);
			}
		}
	}
	catch (...)
	{
		CFabianHFOApp::ReportException(_T("EXCEPTION: CWndServiceMenu::SetAllButtonUnpressed"));
		//AfxMessageBox( _T("EXCEPTION: CMVModel::VentModeChanged()()") );

	}
	
}

// **************************************************************************
// 
// **************************************************************************
void CWndServiceMenu::SetOneButtonFocused(int btnID)
{
	//POSITION pos;

	//for( pos = m_plMenuBtn.GetHeadPosition(); pos != NULL; )
	//{
	//	CSelectSetupBtn* pBtn = m_plMenuBtn.GetNext( pos );

	//	int iT = pBtn->GetBtnId();

	//	if(pBtn->GetBtnId() != btnID)
	//	{
	//		pBtn->SetState(BS_UP);
	//	}
	//	else
	//	{
	//		pBtn->SetState(BS_FOCUSED);

	//		m_iCurrentField=btnID;
	//		//getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);
	//	}
	//}


}

// **************************************************************************
// 
// **************************************************************************
void CWndServiceMenu::SetNextButtonFocused()
{
	//POSITION pos;

	//pos = m_plMenuBtn.GetHeadPosition();

	//do {
	//	CSelectSetupBtn* pBtn  = m_plMenuBtn.GetNext(pos);

	//	int iT = pBtn->GetBtnId();



	//	if(m_iCurrentField == pBtn->GetBtnId())
	//	{

	//		pBtn->SetState(BS_UP);

	//		iT = pBtn->GetBtnId();
	//		if(pos == NULL)
	//		{
	//			pos = m_plMenuBtn.GetHeadPosition();
	//			pBtn = m_plMenuBtn.GetNext( pos );

	//			iT = pBtn->GetBtnId();

	//			pBtn->SetState(BS_FOCUSED);

	//			m_iCurrentField=pBtn->GetBtnId();
	//			//getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);
	//			break;
	//		}
	//		else
	//		{
	//			pBtn = m_plMenuBtn.GetNext( pos );
	//			pBtn->SetState(BS_FOCUSED);
	//			m_iCurrentField=pBtn->GetBtnId();
	//			//getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);
	//			break;
	//		}
	//	}


	//} while (pos != NULL);



}


// **************************************************************************
// 
// **************************************************************************
void CWndServiceMenu::SetPrevButtonFocused()
{
	//POSITION pos;

	//pos = m_plMenuBtn.GetTailPosition();

	//do {
	//	CSelectSetupBtn* pBtn  = m_plMenuBtn.GetPrev(pos);

	//	int iT = pBtn->GetBtnId();

	//	pBtn->SetState(BS_UP);

	//	if(m_iCurrentField == pBtn->GetBtnId())
	//	{
	//		iT = pBtn->GetBtnId();
	//		if(pos == NULL)
	//		{
	//			pos = m_plMenuBtn.GetTailPosition();
	//			pBtn = m_plMenuBtn.GetPrev( pos );

	//			iT = pBtn->GetBtnId();

	//			pBtn->SetState(BS_FOCUSED);
	//			m_iCurrentField=pBtn->GetBtnId();
	//			//getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurrentField);

	//			break;
	//		}
	//		else
	//		{
	//			pBtn = m_plMenuBtn.GetPrev( pos );
	//			pBtn->SetState(BS_FOCUSED);
	//			m_iCurrentField=pBtn->GetBtnId();
	//			//getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);

	//			break;
	//		}
	//	}


	//} while (pos != NULL);

	
}

//void CWndServiceMenu::Draw()
//{
//	//RECT rcCl;
//	//RECT rc;
//	//GetClientRect(&rcCl);
//
//	//CClientDC dc(this);
//	//HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
//	//HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
//	//HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);
//
//
//	//CBrush cbrBack(RGB(181,178,181));
//	//HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
//	//HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
//	//int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
//	//HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf10AcuBold);
//	//int tc=SetTextColor(hdcMem,0x0000000);
//
//	//Rectangle(hdcMem, 0, 0, m_lX, m_lY);
//
//	//CStringW cs = _T("");
//
//	////**************************Version**********************************//
//	//rc.left = 40;  
//	//rc.top = 40;  
//	//rc.right  = 280;  
//	//rc.bottom = 500;
//	//cs = _T("todo:");
//	//DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
//
//
//	//dc.BitBlt(0,0,m_lX,m_lY,CDC::FromHandle(hdcMem),0,0,SRCCOPY);
//	//BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);
//
//	//SetTextColor(hdcMem,tc);
//	//SetBkMode(hdcMem,nBkMode);
//
//	//SelectObject(hdcMem,hbrprev);
//	//SelectObject(hdcMem,hpenprev);
//
//
//	//SelectObject(hdcMem,hBmpMemPrev);
//	//SelectObject(hdcMem,hPrevFont);
//	//DeleteObject(hBmpMem);
//	//DeleteDC(hdcMem);
//}


// **************************************************************************
// 
// **************************************************************************
LRESULT CWndServiceMenu::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{
			SetOneButtonDepressed(wParam);
			return 1;
		}
		break;
	default:
		{
			
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

// **************************************************************************
// 
// **************************************************************************
BOOL CWndServiceMenu::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		if(pMsg->wParam==VK_SPACE)
		{
			return 1;
		}
		break;
	default:
		{

		}
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}


void CWndServiceMenu::OnBnClickedSystem()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SERVICE_SYSTEM);
}
void CWndServiceMenu::OnBnClickedCalibration()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SERVICE_CALIBR);
}
void CWndServiceMenu::OnBnClickedFlowboard()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SERVICE_FLOW);
}
void CWndServiceMenu::OnBnClickedMonitorPIC()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SERVICE_MONITOR);
}
void CWndServiceMenu::OnBnClickedAccuboard()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SERVICE_ACCU);
}
void CWndServiceMenu::OnBnClickedValves()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SERVICE_VALVES);
}
void CWndServiceMenu::OnBnClickedDisplay()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SERVICE_DISPLAY);
}
void CWndServiceMenu::OnBnClickedSettings()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SERVICE_SETTINGS);
}
void CWndServiceMenu::OnBnClickedInterfaces()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SERVICE_IF);
}
void CWndServiceMenu::OnBnClickedNetwork()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SERVICE_NETWORK);
}
void CWndServiceMenu::OnBnClickedLicensing()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SERVICE_LICENSING);
}
void CWndServiceMenu::OnBnClickedMaintenance()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SERVICE_MAINTENANCE);
}
void CWndServiceMenu::OnBnClickedBACK()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_NUMERIC);
}



void CWndServiceMenu::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	// CWnd::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
	// 
	CBrush cbrBack(BACKGND);
	HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

	Rectangle(m_hDC, 0, 0, m_lX, m_lY);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);

	SelectObject(m_hDC,hpenprev);
	SelectObject(m_hDC,hbrprev);
	//cbrBack.DeleteObject();//rkuNEWFIX
}