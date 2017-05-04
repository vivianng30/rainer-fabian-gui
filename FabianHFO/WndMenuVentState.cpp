// WndMenuVentState.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndMenuVentState.h"
#include "LangAdmin.h"
#include "globDefs.h"



#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000


//global font objects
//extern HFONT g_hf15AcuMed;

//extern CLangAdmin* g_pGlobalLanguageStrings;



// CWndMenuVentState

IMPLEMENT_DYNAMIC(CWndMenuVentState, CWnd)

CWndMenuVentState::CWndMenuVentState(CMVView *parentView)
{
	//m_iCurrentPressedBtn=0;
	m_parentView=parentView;

	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;

	m_pcMenu_Up=NULL;
	m_pcMenu_Dw=NULL;
	m_pcMenu_Fc=NULL;
	m_pcMenu_Dis=NULL;

	m_pcMenuIPPV=NULL;
	m_pcMenuSIPPV=NULL;
	m_pcMenuSIMV=NULL;
	m_pcMenuSIMVPSV=NULL;
	m_pcMenuPSV=NULL;
	m_pcMenuHFO=NULL;
	m_pcMenuCPAP=NULL;
	m_pcMenuNCPAP=NULL;
	m_pcMenuDUOPAP=NULL;
	m_pcMenuTherapy=NULL;

	m_pModel = NULL;
}

CWndMenuVentState::~CWndMenuVentState()
{
	delete m_pcMenuIPPV;
	m_pcMenuIPPV=NULL;
	delete m_pcMenuSIPPV;
	m_pcMenuSIPPV=NULL;
	delete m_pcMenuSIMV;
	m_pcMenuSIMV=NULL;
	delete m_pcMenuSIMVPSV;
	m_pcMenuSIMVPSV=NULL;
	delete m_pcMenuPSV;
	m_pcMenuPSV=NULL;
	delete m_pcMenuHFO;
	m_pcMenuHFO=NULL;
	delete m_pcMenuCPAP;
	m_pcMenuCPAP=NULL;
	delete m_pcMenuNCPAP;
	m_pcMenuNCPAP=NULL;
	delete m_pcMenuDUOPAP;
	m_pcMenuDUOPAP=NULL;
	delete m_pcMenuTherapy;
	m_pcMenuTherapy=NULL;

	delete m_pcMenu_Up;
	m_pcMenu_Up=NULL;
	delete m_pcMenu_Dw;
	m_pcMenu_Dw=NULL;
	delete m_pcMenu_Fc;
	m_pcMenu_Fc=NULL;
	delete m_pcMenu_Dis;
	m_pcMenu_Dis=NULL;
}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CWndMenuVentState::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


BEGIN_MESSAGE_MAP(CWndMenuVentState, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CWndMenuVentState message handlers
// **************************************************************************
// 
// **************************************************************************
BOOL CWndMenuVentState::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD/*|WS_VISIBLE*/,rc,pParentWnd,nID))
	{
		CClientDC dc(this);

		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		CBrush cbrBack(BACKGND);
		//CBrush cbrBack(RGB(200,200,255));
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

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
void CWndMenuVentState::Init()
{
	CClientDC dc(this);

	DWORD dwStyleNoTab = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW;
	//DWORD dwStyleNoTab = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW|WS_TABSTOP;

	BTN btn;

	/*m_pcMenu_Up		= new CBmp(theApp.m_hInstance,hdc,	IDB_BTN_5MENU_UP);
	m_pcMenu_Dw		= new CBmp(theApp.m_hInstance,hdc,	IDB_BTN_5MENU_DW);
	m_pcMenu_Fc		= new CBmp(theApp.m_hInstance,hdc,	IDB_BTN_5MENU_FC);*/

	m_pcMenu_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_UP);
	m_pcMenu_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_DW);
	m_pcMenu_Fc		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_FC);
	m_pcMenu_Dis	= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_DIS);

	//Menu Buttons
	btn.wID					= IDC_BTN_MENU_IPPV;	
	btn.poPosition.x		= 3;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuIPPV=new CPresetMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuIPPV->Create(this,g_hf14AcuMed,0);
	m_pcMenuIPPV->SetText(getModel()->GetLanguageString(IDS_IPPV));
	m_pcMenuIPPV->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuIPPV);



	btn.wID					= IDC_BTN_MENU_SIPPV;	
	btn.poPosition.x		= 120;
	//btn.poPosition.x		= 151;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuSIPPV=new CPresetMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuSIPPV->Create(this,g_hf14AcuMed,0);
	m_pcMenuSIPPV->SetText(getModel()->GetLanguageString(IDS_SIPPV));
	m_pcMenuSIPPV->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuSIPPV);

	btn.wID					= IDC_BTN_MENU_SIMV;	
	btn.poPosition.x		= 237;
	//btn.poPosition.x		= 299;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuSIMV=new CPresetMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuSIMV->Create(this,g_hf14AcuMed,0);
	m_pcMenuSIMV->SetText(getModel()->GetLanguageString(IDS_SIMV));
	m_pcMenuSIMV->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuSIMV);

	btn.wID					= IDC_BTN_MENU_SIMVPSV;	
	btn.poPosition.x		= 354;
	//btn.poPosition.x		= 299;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuSIMVPSV=new CPresetMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuSIMVPSV->Create(this,g_hf14AcuMed,0);
	m_pcMenuSIMVPSV->SetText(getModel()->GetLanguageString(IDS_SIMVPSV));
	m_pcMenuSIMVPSV->ShowWindow(SW_HIDE);
	//TESTVERSION PSV
	m_plMenuBtn.AddTail(m_pcMenuSIMVPSV);

	btn.wID					= IDC_BTN_MENU_PSV;	
	btn.poPosition.x		= 471;
	//btn.poPosition.x		= 447;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuPSV=new CPresetMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuPSV->Create(this,g_hf14AcuMed,0);
	m_pcMenuPSV->SetText(getModel()->GetLanguageString(IDS_PSV));
	m_pcMenuPSV->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuPSV);

	

	btn.wID					= IDC_BTN_MENU_CPAP;	
	btn.poPosition.x		= 3;
	btn.poPosition.y		= 50;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuCPAP=new CPresetMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuCPAP->Create(this,g_hf14AcuMed,0);
	m_pcMenuCPAP->SetText(getModel()->GetLanguageString(IDS_CPAP));
	m_pcMenuCPAP->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuCPAP);

	btn.wID					= IDC_BTN_MENU_NCPAP;	
	btn.poPosition.x		= 120;
	//btn.poPosition.x		= 151;
	btn.poPosition.y		= 50;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuNCPAP=new CPresetMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuNCPAP->Create(this,g_hf14AcuMed,0);
	m_pcMenuNCPAP->SetText(getModel()->GetLanguageString(IDS_NCPAP));
	m_pcMenuNCPAP->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuNCPAP);

	btn.wID					= IDC_BTN_MENU_DUOPAP;	
	btn.poPosition.x		= 237;
	//btn.poPosition.x		= 299;
	btn.poPosition.y		= 50;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuDUOPAP=new CPresetMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuDUOPAP->Create(this,g_hf14AcuMed,0);
	m_pcMenuDUOPAP->SetText(getModel()->GetLanguageString(IDS_DUOPAP));
	m_pcMenuDUOPAP->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuDUOPAP);

	btn.wID					= IDC_BTN_MENU_HFO;	
	btn.poPosition.x		= 354;
	//btn.poPosition.x		= 447;
	btn.poPosition.y		= 50;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuHFO=new CPresetMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuHFO->Create(this,g_hf14AcuMed,0);
	m_pcMenuHFO->SetText(getModel()->GetLanguageString(IDS_HFO));

	if(		false==getModel()->getCONFIG()->isBiasFlowActive()
		&&	false==getModel()->getDATAHANDLER()->isHFOLicenseAvailable())
	{
		m_pcMenuHFO->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pcMenuHFO->ShowWindow(SW_HIDE);
		m_plMenuBtn.AddTail(m_pcMenuHFO);
	}
	

	btn.wID					= IDC_BTN_MENU_THERAPIE;	
	btn.poPosition.x		= 471;
	btn.poPosition.y		= 50;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuTherapy=new CPresetMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuTherapy->Create(this,g_hf14AcuMed,0);
	m_pcMenuTherapy->SetText(getModel()->GetLanguageString(IDS_O2THERAPIE));
	m_pcMenuTherapy->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuTherapy);

	//btn.wID					= IDC_BTN_MENU_NUMERIC;	
	//btn.poPosition.x		= 602;
	//btn.poPosition.y		= 48;//553;
	//btn.pcBmpUp				= m_pcNumeric_Up;
	//btn.pcBmpDown			= m_pcNumeric_Dw;
	//btn.pcBmpFocus			= m_pcNumeric_Up;
	//btn.pcBmpDisabled		= m_pcNumeric_Up;
	//btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	//m_pcMenuNumeric=new CNumBtn(btn,COLOR_TXTBTNUP,GRP_NUMERIC_BTN);
	//m_pcMenuNumeric->Create(this,g_hf14AcuMed,0);
	//m_pcMenuNumeric->SetText(_T("Numerics"));
	//m_pcMenuNumeric->ShowWindow(SW_HIDE);
	

	ShowMenuBtn();

}

// **************************************************************************
// 
// **************************************************************************
void CWndMenuVentState::Show(BOOL bShow)
{
	if(bShow==TRUE)
	{
		//if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF)
		//{
		//	if(m_pcMenuPSV)
		//		m_pcMenuPSV->ShowWindow(SW_HIDE);
		//	if(m_pcMenuSIMVPSV)
		//		m_pcMenuSIMVPSV->ShowWindow(SW_HIDE);
		//	/*if(m_pcMenuSIMV)
		//		m_pcMenuSIMV->ShowWindow(SW_HIDE);
		//	if(m_pcMenuSIPPV)
		//		m_pcMenuSIPPV->ShowWindow(SW_HIDE);*/
		//}
		//else
		//{
		//	if(m_pcMenuPSV)
		//		m_pcMenuPSV->ShowWindow(SW_SHOW);
		//	//TESTVERSION PSV
		//	if(m_pcMenuSIMVPSV)
		//		m_pcMenuSIMVPSV->ShowWindow(SW_SHOW);
		//	/*if(m_pcMenuSIMV)
		//		m_pcMenuSIMV->ShowWindow(SW_SHOW);
		//	if(m_pcMenuSIPPV)
		//		m_pcMenuSIPPV->ShowWindow(SW_SHOW);*/
		//}

		//if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		//{

		//}
		//else
		//{

		//}
		ShowMenuBtn();
		this->ShowWindow(SW_SHOW);
	}
	else
		this->ShowWindow(SW_HIDE);
}

// **************************************************************************
// 
// **************************************************************************
void CWndMenuVentState::ShowMenuBtn()
{
	//m_pcMenuNumeric->ShowWindow(SW_SHOW);

	if(m_pcMenuIPPV)
		m_pcMenuIPPV->ShowWindow(SW_SHOW);
	if(m_pcMenuSIPPV)
		m_pcMenuSIPPV->ShowWindow(SW_SHOW);
	if(m_pcMenuSIMV)
		m_pcMenuSIMV->ShowWindow(SW_SHOW);
	if(m_pcMenuSIMVPSV)
		m_pcMenuSIMVPSV->ShowWindow(SW_SHOW);
	if(m_pcMenuPSV)
		m_pcMenuPSV->ShowWindow(SW_SHOW);

	//HFO
	if(		false==getModel()->getCONFIG()->isBiasFlowActive()
		&&	false==getModel()->getDATAHANDLER()->isHFOLicenseAvailable())
	{
		if(m_pcMenuHFO)
			m_pcMenuHFO->ShowWindow(SW_HIDE);
	}
	else if(true==getModel()->getDATAHANDLER()->isHFOLicenseAvailable())
	{
		if(m_pcMenuHFO)
			m_pcMenuHFO->EnableWindow(TRUE);
		if(m_pcMenuHFO)
			m_pcMenuHFO->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuHFO)
			m_pcMenuHFO->EnableWindow(FALSE);
		if(m_pcMenuHFO)
			m_pcMenuHFO->ShowWindow(SW_SHOW);
		/*if(m_pcMenuHFO)
			m_pcMenuHFO->ShowWindow(SW_HIDE);*/
	}


	if(m_pcMenuCPAP)
		m_pcMenuCPAP->ShowWindow(SW_SHOW);

	if(		true==getModel()->getDATAHANDLER()->isNMODELicenseAvailable() 
		&&	getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		if(m_pcMenuNCPAP)
			m_pcMenuNCPAP->EnableWindow(TRUE);
		if(m_pcMenuNCPAP)
			m_pcMenuNCPAP->ShowWindow(SW_SHOW);
		if(m_pcMenuDUOPAP)
			m_pcMenuDUOPAP->EnableWindow(TRUE);
		if(m_pcMenuDUOPAP)
			m_pcMenuDUOPAP->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNCPAP)
			m_pcMenuNCPAP->EnableWindow(FALSE);
		if(m_pcMenuNCPAP)
			m_pcMenuNCPAP->ShowWindow(SW_SHOW);
		if(m_pcMenuDUOPAP)
			m_pcMenuDUOPAP->EnableWindow(FALSE);
		if(m_pcMenuDUOPAP)
			m_pcMenuDUOPAP->ShowWindow(SW_SHOW);
	}

	if(true==getModel()->getDATAHANDLER()->isTHERAPYLicenseAvailable())
	{
		if(m_pcMenuTherapy)
			m_pcMenuTherapy->EnableWindow(TRUE);
		if(m_pcMenuTherapy)
			m_pcMenuTherapy->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuTherapy)
			m_pcMenuTherapy->EnableWindow(FALSE);
		if(m_pcMenuTherapy)
			m_pcMenuTherapy->ShowWindow(SW_SHOW);
	}

	if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF)
	{
		if(m_pcMenuPSV)
			m_pcMenuPSV->EnableWindow(FALSE);
		if(m_pcMenuPSV)
			m_pcMenuPSV->ShowWindow(SW_SHOW);
		if(m_pcMenuSIMVPSV)
			m_pcMenuSIMVPSV->EnableWindow(FALSE);
		if(m_pcMenuSIMVPSV)
			m_pcMenuSIMVPSV->ShowWindow(SW_SHOW);
		
	}
	else
	{
		if(m_pcMenuPSV)
			m_pcMenuPSV->EnableWindow(TRUE);
		if(m_pcMenuPSV)
			m_pcMenuPSV->ShowWindow(SW_SHOW);
		if(m_pcMenuSIMVPSV)
			m_pcMenuSIMVPSV->EnableWindow(TRUE);
		if(m_pcMenuSIMVPSV)
			m_pcMenuSIMVPSV->ShowWindow(SW_SHOW);
	}

		
	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_IPPV:
		{
			SetOneButtonDepressed(IDC_BTN_MENU_IPPV);
		}
		break;
	case VM_PSV: 
		{
			SetOneButtonDepressed(IDC_BTN_MENU_PSV);
		}
		break;
	case VM_SIMV:
		{
			SetOneButtonDepressed(IDC_BTN_MENU_SIMV);
		}
		break;
	case VM_SIMVPSV:
		{
			SetOneButtonDepressed(IDC_BTN_MENU_SIMVPSV);
		}
		break;
	case VM_SIPPV:
		{
			SetOneButtonDepressed(IDC_BTN_MENU_SIPPV);
		}
		break;
	case VM_HFO:
		{
			if(true==getModel()->getDATAHANDLER()->isHFOLicenseAvailable())
				SetOneButtonDepressed(IDC_BTN_MENU_HFO);
			else
			{
				SetOneButtonDepressed(IDC_BTN_MENU_IPPV);
				//getModel()->getDATAHANDLER()->IPPV()->SetDataFromMode(VM_PRE_IPPV);

				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_IPPV))
					theApp.getLog()->WriteLine(_T("#HFO:0276"));
			}

		}
		break;
	case VM_CPAP:
		{
			SetOneButtonDepressed(IDC_BTN_MENU_CPAP);
		}
		break;
	case VM_NCPAP:
		{
			if(		true==getModel()->getDATAHANDLER()->isNMODELicenseAvailable()
				 /*&& getModel()->getCONFIG()->GetVentRange()==NEONATAL*/)
			{
				SetOneButtonDepressed(IDC_BTN_MENU_NCPAP);
			}
			else
			{
				SetOneButtonDepressed(IDC_BTN_MENU_CPAP);

				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_CPAP))
					theApp.getLog()->WriteLine(_T("#HFO:0277"));
			}
		}
		break;
	case VM_DUOPAP:
		{
			if(		true==getModel()->getDATAHANDLER()->isNMODELicenseAvailable()
				 /*&& getModel()->getCONFIG()->GetVentRange()==NEONATAL*/)
			{
				SetOneButtonDepressed(IDC_BTN_MENU_DUOPAP);
			}
			else
			{
				SetOneButtonDepressed(IDC_BTN_MENU_CPAP);

				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_CPAP))
					theApp.getLog()->WriteLine(_T("#HFO:0278"));
			}
		}
		break;
	case VM_THERAPIE:
		{
			if(		true==getModel()->getDATAHANDLER()->isTHERAPYLicenseAvailable()
				/*&& getModel()->getCONFIG()->GetVentRange()==NEONATAL*/)
			{
				SetOneButtonDepressed(IDC_BTN_MENU_THERAPIE);
			}
			else
			{
				SetOneButtonDepressed(IDC_BTN_MENU_CPAP);

				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_CPAP))
					theApp.getLog()->WriteLine(_T("#HFO:0279"));
			}
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
void CWndMenuVentState::HideMenuBtn()
{
	if(m_pcMenuIPPV)
		m_pcMenuIPPV->ShowWindow(SW_HIDE);
	if(m_pcMenuSIPPV)
		m_pcMenuSIPPV->ShowWindow(SW_HIDE);
	if(m_pcMenuSIMV)
		m_pcMenuSIMV->ShowWindow(SW_HIDE);
	if(m_pcMenuSIMVPSV)
		m_pcMenuSIMVPSV->ShowWindow(SW_HIDE);
	if(m_pcMenuPSV)
		m_pcMenuPSV->ShowWindow(SW_HIDE);
	if(m_pcMenuHFO)
		m_pcMenuHFO->ShowWindow(SW_HIDE);
	if(m_pcMenuCPAP)
		m_pcMenuCPAP->ShowWindow(SW_HIDE);
	if(m_pcMenuNCPAP)
		m_pcMenuNCPAP->ShowWindow(SW_HIDE);
	if(m_pcMenuDUOPAP)
		m_pcMenuDUOPAP->ShowWindow(SW_HIDE);
	if(m_pcMenuTherapy)
		m_pcMenuTherapy->ShowWindow(SW_HIDE);
	//m_pcMenuVolume->ShowWindow(SW_HIDE);
	/*m_pcMenu10->ShowWindow(SW_HIDE);*/
}

// **************************************************************************
// 
// **************************************************************************
LRESULT CWndMenuVentState::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{
			//SetButton(wParam);

			if(lParam==CPresetMenuBtn::UP)
				SetOneButtonPreset(wParam);
			else if(lParam==CPresetMenuBtn::PRESET)
				SetOneButtonDepressed(wParam);
			/*else if(lParam==CPresetMenuBtn::DOWN)
				SetOneButtonDepressed(wParam);*/

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_MENUBUTTONDOWN,wParam);
			return 1;
		}
		break;
	//case WM_MENUBUTTONPRESET:
	//	{
	//		//SetButton(wParam);
	//		SetButtonState(wParam, CPresetMenuBtn::PRESET);
	//		switch(wParam)
	//		{
	//		case IDC_BTN_MENU_IPPV:
	//			{
	//				OnBnClickedMenuIPPV();
	//			}
	//			break;
	//		case IDC_BTN_MENU_SIPPV:
	//			{
	//				OnBnClickedMenuSIPPV();
	//			}
	//			break;
	//		case IDC_BTN_MENU_SIMV:
	//			{
	//				OnBnClickedMenuSIMV();
	//			}
	//			break;
	//		case IDC_BTN_MENU_SIMVPSV:
	//			{
	//				OnBnClickedMenuSIMVPSV();
	//			}
	//			break;
	//		case IDC_BTN_MENU_PSV:
	//			{
	//				OnBnClickedMenuPSV();
	//			}
	//			break;
	//		case IDC_BTN_MENU_HFO:
	//			{
	//				OnBnClickedMenuHFO();
	//			}
	//			break;
	//		case IDC_BTN_MENU_CPAP:
	//			{
	//				OnBnClickedMenuCPAP();
	//			}
	//			break;
	//		case IDC_BTN_MENU_NCPAP:
	//			{
	//				OnBnClickedMenuNCPAP();
	//			}
	//			break;
	//		case IDC_BTN_MENU_DUOPAP:
	//			{
	//				OnBnClickedMenuDUOPAP();
	//			}
	//			break;
	//		}
	//	}
	//	break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}


// **************************************************************************
// 
// **************************************************************************
void CWndMenuVentState::SetOneButtonDepressed(int btnID)
{
	POSITION pos;

	try
	{
		for( pos = m_plMenuBtn.GetHeadPosition(); pos != NULL; )
		{
			CPresetMenuBtn* pUDBtn = m_plMenuBtn.GetNext( pos );

			//int iT = pUDBtn->GetBtnId();

			if(pUDBtn->GetBtnId() != btnID)
			{
				if(pUDBtn->GetBtnState()!=CPresetMenuBtn::UP)
				{
					pUDBtn->SetBtnState(CPresetMenuBtn::UP);
				}
			}
			else
			{
				//SetCurrentPressedBtn(pUDBtn->GetBtnId());
				pUDBtn->SetBtnState(CPresetMenuBtn::DOWN);
			}
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CWndMenuTrendtype::SetOneButtonDepressed"));
	}
	
}

void CWndMenuVentState::SetOneButtonPreset(int btnID)
{
	POSITION pos;

	try
	{
		for( pos = m_plMenuBtn.GetHeadPosition(); pos != NULL; )
		{
			CPresetMenuBtn* pUDBtn = m_plMenuBtn.GetNext( pos );

			//int iT = pUDBtn->GetBtnId();

			if(pUDBtn->GetBtnId() != btnID)
			{
				if(pUDBtn->GetBtnState()==CPresetMenuBtn::PRESET)
				{
					pUDBtn->SetBtnState(CPresetMenuBtn::UP);
				}
			}
			else
			{
				//SetCurrentPressedBtn(pUDBtn->GetBtnId());
				pUDBtn->SetBtnState(CPresetMenuBtn::PRESET);
			}
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CWndMenuTrendtype::SetOneButtonPreset"));
	}
	
}

// **************************************************************************
// 
// **************************************************************************
void CWndMenuVentState::ResetButtonPresetstate()
{
	POSITION pos;

	try
	{
		for( pos = m_plMenuBtn.GetHeadPosition(); pos != NULL; )
		{
			CPresetMenuBtn* pUDBtn = m_plMenuBtn.GetNext( pos );

			//int iT = pUDBtn->GetBtnId();

			if(pUDBtn->GetBtnState()==CPresetMenuBtn::PRESET)
			{
				pUDBtn->SetBtnState(CPresetMenuBtn::UP);
			}
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CWndMenuTrendtype::ResetButtonPresetstate"));
	}
	
}

// **************************************************************************
// 
// **************************************************************************
//void CWndMenuVentState::SetButton(int btnID)
//{
//	POSITION pos;
//
//	for( pos = m_plMenuBtn.GetHeadPosition(); pos != NULL; )
//	{
//		CPresetMenuBtn* pUDBtn = m_plMenuBtn.GetNext( pos );
//
//		int iT = pUDBtn->GetBtnId();
//
//		if(pUDBtn->GetBtnId() == btnID)
//		{
//			if(pUDBtn->GetBtnState()==CPresetMenuBtn::UP)
//			{
//				SetButtonState(btnID, CPresetMenuBtn::PRESET);
//			}
//			else if(pUDBtn->GetBtnState()==CPresetMenuBtn::PRESET)
//			{
//				//SetCurrentPressedBtn(pUDBtn->GetBtnId());
//				SetButtonState(btnID, CPresetMenuBtn::DOWN);
//			}
//			else
//			{
//				SetButtonState(btnID, CPresetMenuBtn::DOWN);
//			}
//			break;
//		}
//	}
//}

// **************************************************************************
// 
// **************************************************************************
void CWndMenuVentState::SetButtonState(int btnID, CPresetMenuBtn::eBtnState state)
{
	POSITION pos;
	try
	{
		for( pos = m_plMenuBtn.GetHeadPosition(); pos != NULL; )
		{
			CPresetMenuBtn* pUDBtn = m_plMenuBtn.GetNext( pos );

			//int iT = pUDBtn->GetBtnId();
			if(pUDBtn->GetBtnId() != btnID)
			{
				
				if(pUDBtn->GetBtnState()==state)
				{
					pUDBtn->SetBtnState(CPresetMenuBtn::UP);
				}
			}
			else
			{
				pUDBtn->SetBtnState(state);
			}
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CWndMenuTrendtype::SetButtonState"));
	}
	
}

// **************************************************************************
// 
// **************************************************************************
void CWndMenuVentState::SetAllButtonUnpressed()
{
	POSITION pos;

	try
	{
		for( pos = m_plMenuBtn.GetHeadPosition(); pos != NULL; )
		{
			CPresetMenuBtn* pUDBtn = m_plMenuBtn.GetNext( pos );

			if(pUDBtn->GetBtnState()==CPresetMenuBtn::DOWN)
			{
				pUDBtn->SetBtnState(CPresetMenuBtn::UP);
			}
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CWndMenuTrendtype::SetAllButtonUnpressed"));
	}
	
}

// **************************************************************************
// 
// **************************************************************************
void CWndMenuVentState::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);


	// CMVView::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
}

//************************************
// Method:    OnDestroy
// FullName:  CWndMenuVentState::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndMenuVentState::OnDestroy() 
{
	m_plMenuBtn.RemoveAll();

	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}

// **************************************************************************
// 
// **************************************************************************
BOOL CWndMenuVentState::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		if(pMsg->wParam==VK_SPACE)
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_KEYDOWN,pMsg->wParam,pMsg->lParam);
			return 1;
		}
		else if(pMsg->wParam==VK_UP)
		{
			return 1;
		}
		else if(pMsg->wParam==VK_DOWN)
		{
			return 1;
		}
		else
		{
			//handle focus change manual, draw next focused button and return true
			//return 1;
		}
		break;
	default:
		{

		}
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

//int CWndMenuVentState::GetCurrentPressedBtn()
//{
//	return m_iCurrentPressedBtn;
//}
//void CWndMenuVentState::SetCurrentPressedBtn(int btn)
//{
//	m_iCurrentPressedBtn=btn;
//}
