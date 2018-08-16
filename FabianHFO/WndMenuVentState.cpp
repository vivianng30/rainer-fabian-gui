// WndMenuVentState.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndMenuVentState.h"
#include "LangAdmin.h"
#include "globDefs.h"

/**********************************************************************************************//**
 * A macro that defines color txtbtnup
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNUP				0x00000000

/**********************************************************************************************//**
 * A macro that defines color txtbtndw
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNDW				0x00FFFFFF

/**********************************************************************************************//**
 * A macro that defines color txtbtnfc
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNFC				0x00000000


//global font objects
//extern HFONT g_hf15AcuMed;

//extern CLangAdmin* g_pGlobalLanguageStrings;

/**********************************************************************************************//**
 * CWndMenuVentState
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndMenuVentState, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndMenuVentState class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	parentView	If non-null, the parent view.
 **************************************************************************************************/

CWndMenuVentState::CWndMenuVentState()
{
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
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndMenuVentState class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndMenuVentState::~CWndMenuVentState()
{
	delete m_pcMenu_Up;
	m_pcMenu_Up=NULL;
	delete m_pcMenu_Dw;
	m_pcMenu_Dw=NULL;
	delete m_pcMenu_Fc;
	m_pcMenu_Fc=NULL;
	delete m_pcMenu_Dis;
	m_pcMenu_Dis=NULL;
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * CWndMenuVentState message handlers
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	rc		  	The rectangle.
 * \param 		  	nID		  	The identifier.
 * \param [in,out]	pContext  	If non-null, the context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

		CPen penBack;
		penBack.CreatePen(PS_SOLID,1,BACKGND);

		CBrush cbrBack(BACKGND);
		//CBrush cbrBack(RGB(200,200,255));
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)penBack);	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		
		penBack.DeleteObject();

		return 1;
	}
	else
		return 0;
}

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuVentState::Init()
{
	CClientDC dc(this);

	DWORD dwStyleNoTab = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW;

	BTN btn;

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

	m_menuIPPV.Initialize(btn,COLOR_TXTBTNUP);
	m_menuIPPV.Create(this,g_hf14AcuMed,0);
	m_menuIPPV.SetText(getModel()->GetLanguageString(IDS_IPPV));
	m_menuIPPV.ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(&m_menuIPPV);



	btn.wID					= IDC_BTN_MENU_SIPPV;	
	btn.poPosition.x		= 120;
	//btn.poPosition.x		= 151;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_menuSIPPV.Initialize(btn,COLOR_TXTBTNUP);
	m_menuSIPPV.Create(this,g_hf14AcuMed,0);
	m_menuSIPPV.SetText(getModel()->GetLanguageString(IDS_SIPPV));
	m_menuSIPPV.ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(&m_menuSIPPV);

	btn.wID					= IDC_BTN_MENU_SIMV;	
	btn.poPosition.x		= 237;
	//btn.poPosition.x		= 299;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_menuSIMV.Initialize(btn,COLOR_TXTBTNUP);
	m_menuSIMV.Create(this,g_hf14AcuMed,0);
	m_menuSIMV.SetText(getModel()->GetLanguageString(IDS_SIMV));
	m_menuSIMV.ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(&m_menuSIMV);

	btn.wID					= IDC_BTN_MENU_SIMVPSV;	
	btn.poPosition.x		= 354;
	//btn.poPosition.x		= 299;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_menuSIMVPSV.Initialize(btn,COLOR_TXTBTNUP);
	m_menuSIMVPSV.Create(this,g_hf14AcuMed,0);
	m_menuSIMVPSV.SetText(getModel()->GetLanguageString(IDS_SIMVPSV));
	m_menuSIMVPSV.ShowWindow(SW_HIDE);
	//TESTVERSION PSV
	m_plMenuBtn.AddTail(&m_menuSIMVPSV);

	btn.wID					= IDC_BTN_MENU_PSV;	
	btn.poPosition.x		= 471;
	//btn.poPosition.x		= 447;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_menuPSV.Initialize(btn,COLOR_TXTBTNUP);
	m_menuPSV.Create(this,g_hf14AcuMed,0);
	m_menuPSV.SetText(getModel()->GetLanguageString(IDS_PSV));
	m_menuPSV.ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(&m_menuPSV);

	

	btn.wID					= IDC_BTN_MENU_CPAP;	
	btn.poPosition.x		= 3;
	btn.poPosition.y		= 50;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_menuCPAP.Initialize(btn,COLOR_TXTBTNUP);
	m_menuCPAP.Create(this,g_hf14AcuMed,0);
	m_menuCPAP.SetText(getModel()->GetLanguageString(IDS_CPAP));
	m_menuCPAP.ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(&m_menuCPAP);

	btn.wID					= IDC_BTN_MENU_NCPAP;	
	btn.poPosition.x		= 120;
	//btn.poPosition.x		= 151;
	btn.poPosition.y		= 50;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_menuNCPAP.Initialize(btn,COLOR_TXTBTNUP);
	m_menuNCPAP.Create(this,g_hf14AcuMed,0);
	m_menuNCPAP.SetText(getModel()->GetLanguageString(IDS_NCPAP));
	m_menuNCPAP.ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(&m_menuNCPAP);

	btn.wID					= IDC_BTN_MENU_DUOPAP;	
	btn.poPosition.x		= 237;
	//btn.poPosition.x		= 299;
	btn.poPosition.y		= 50;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_menuDUOPAP.Initialize(btn,COLOR_TXTBTNUP);
	m_menuDUOPAP.Create(this,g_hf14AcuMed,0);
	m_menuDUOPAP.SetText(getModel()->GetLanguageString(IDS_DUOPAP));
	m_menuDUOPAP.ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(&m_menuDUOPAP);

	btn.wID					= IDC_BTN_MENU_HFO;	
	btn.poPosition.x		= 354;
	//btn.poPosition.x		= 447;
	btn.poPosition.y		= 50;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_menuHFO.Initialize(btn,COLOR_TXTBTNUP);
	m_menuHFO.Create(this,g_hf14AcuMed,0);
	m_menuHFO.SetText(getModel()->GetLanguageString(IDS_HFO));

	if(		false==getModel()->getCONFIG()->isBiasFlowActive()
		&&	false==getModel()->getDATAHANDLER()->isHFOLicenseAvailable())
	{
		m_menuHFO.ShowWindow(SW_HIDE);
	}
	else
	{
		m_menuHFO.ShowWindow(SW_HIDE);
		m_plMenuBtn.AddTail(&m_menuHFO);
	}
	

	btn.wID					= IDC_BTN_MENU_THERAPIE;	
	btn.poPosition.x		= 471;
	btn.poPosition.y		= 50;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Fc;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_menuTherapy.Initialize(btn,COLOR_TXTBTNUP);
	m_menuTherapy.Create(this,g_hf14AcuMed,0);
	m_menuTherapy.SetText(getModel()->GetLanguageString(IDS_O2THERAPIE));
	m_menuTherapy.ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(&m_menuTherapy);


	ShowMenuBtn();

}

/**********************************************************************************************//**
 * Shows
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CWndMenuVentState::Show(BOOL bShow)
{	
	if(bShow==TRUE)
	{
		ShowMenuBtn();
		this->ShowWindow(SW_SHOW);
	}
	else
		this->ShowWindow(SW_HIDE);
}

/**********************************************************************************************//**
 * Shows the menu button
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuVentState::ShowMenuBtn()
{
	m_menuIPPV.ShowWindow(SW_SHOW);
	m_menuSIPPV.ShowWindow(SW_SHOW);
	m_menuSIMV.ShowWindow(SW_SHOW);
	m_menuSIMVPSV.ShowWindow(SW_SHOW);
	m_menuPSV.ShowWindow(SW_SHOW);

	//HFO
	if(		false==getModel()->getCONFIG()->isBiasFlowActive()
		&&	false==getModel()->getDATAHANDLER()->isHFOLicenseAvailable())
	{
		m_menuHFO.ShowWindow(SW_HIDE);
	}
	else if(true==getModel()->getDATAHANDLER()->isHFOLicenseAvailable())
	{
		m_menuHFO.EnableWindow(TRUE);
		m_menuHFO.ShowWindow(SW_SHOW);
	}
	else
	{
		m_menuHFO.EnableWindow(FALSE);
		m_menuHFO.ShowWindow(SW_SHOW);
	}


	m_menuCPAP.ShowWindow(SW_SHOW);

	if(		true==getModel()->getDATAHANDLER()->isNMODELicenseAvailable() 
		&&	getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		m_menuNCPAP.EnableWindow(TRUE);
		m_menuNCPAP.ShowWindow(SW_SHOW);
		m_menuDUOPAP.EnableWindow(TRUE);
		m_menuDUOPAP.ShowWindow(SW_SHOW);
	}
	else
	{
		m_menuNCPAP.EnableWindow(FALSE);
		m_menuNCPAP.ShowWindow(SW_SHOW);
		m_menuDUOPAP.EnableWindow(FALSE);
		m_menuDUOPAP.ShowWindow(SW_SHOW);
	}

	if(true==getModel()->getDATAHANDLER()->isTHERAPYLicenseAvailable())
	{
		m_menuTherapy.EnableWindow(TRUE);
		m_menuTherapy.ShowWindow(SW_SHOW);
	}
	else
	{
		m_menuTherapy.EnableWindow(FALSE);
		m_menuTherapy.ShowWindow(SW_SHOW);
	}

	if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF)
	{
		m_menuPSV.EnableWindow(FALSE);
		m_menuPSV.ShowWindow(SW_SHOW);
		m_menuSIMVPSV.EnableWindow(FALSE);
		m_menuSIMVPSV.ShowWindow(SW_SHOW);
	}
	else
	{
		m_menuPSV.EnableWindow(TRUE);
		m_menuPSV.ShowWindow(SW_SHOW);
		m_menuSIMVPSV.EnableWindow(TRUE);
		m_menuSIMVPSV.ShowWindow(SW_SHOW);
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

				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_IPPV))
					theApp.WriteLog(_T("#HFO:0276"));
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
			if(		true==getModel()->getDATAHANDLER()->isNMODELicenseAvailable())
			{
				SetOneButtonDepressed(IDC_BTN_MENU_NCPAP);
			}
			else
			{
				SetOneButtonDepressed(IDC_BTN_MENU_CPAP);

				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_CPAP))
					theApp.WriteLog(_T("#HFO:0277"));
			}
		}
		break;
	case VM_DUOPAP:
		{
			if(		true==getModel()->getDATAHANDLER()->isNMODELicenseAvailable())
			{
				SetOneButtonDepressed(IDC_BTN_MENU_DUOPAP);
			}
			else
			{
				SetOneButtonDepressed(IDC_BTN_MENU_CPAP);

				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_CPAP))
					theApp.WriteLog(_T("#HFO:0278"));
			}
		}
		break;
	case VM_THERAPIE:
		{
			if(		true==getModel()->getDATAHANDLER()->isTHERAPYLicenseAvailable())
			{
				SetOneButtonDepressed(IDC_BTN_MENU_THERAPIE);
			}
			else
			{
				SetOneButtonDepressed(IDC_BTN_MENU_CPAP);

				if(false==getModel()->getVMODEHANDLER()->changeVentMode(VM_CPAP))
					theApp.WriteLog(_T("#HFO:0279"));
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
 * Hides the menu button
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuVentState::HideMenuBtn()
{
	m_menuIPPV.ShowWindow(SW_HIDE);
	m_menuSIPPV.ShowWindow(SW_HIDE);
	m_menuSIMV.ShowWindow(SW_HIDE);
	m_menuSIMVPSV.ShowWindow(SW_HIDE);
	m_menuPSV.ShowWindow(SW_HIDE);
	m_menuHFO.ShowWindow(SW_HIDE);
	m_menuCPAP.ShowWindow(SW_HIDE);
	m_menuNCPAP.ShowWindow(SW_HIDE);
	m_menuDUOPAP.ShowWindow(SW_HIDE);
	m_menuTherapy.ShowWindow(SW_HIDE);
}

/**********************************************************************************************//**
 * Window proc
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	message	The message.
 * \param	wParam 	The wParam field of the message.
 * \param	lParam 	The lParam field of the message.
 *
 * \return	A LRESULT.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets one button depressed
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	btnID	Identifier for the button.
 **************************************************************************************************/

void CWndMenuVentState::SetOneButtonDepressed(int btnID)
{
	//DEBUGMSG(TRUE, (TEXT("CWndMenuVentState::SetOneButtonDepressed start\r\n")));
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
	//DEBUGMSG(TRUE, (TEXT("CWndMenuVentState::SetOneButtonDepressed end\r\n")));
	
}

/**********************************************************************************************//**
 * Sets one button preset
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	btnID	Identifier for the button.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Resets the button presetstate
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets button state
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	btnID	Identifier for the button.
 * \param	state	The state.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets all button unpressed
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuVentState::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);


	// CMVView::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndMenuVentState::OnDestroy() 
{
	CWnd::OnDestroy();

	m_plMenuBtn.RemoveAll();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}

/**********************************************************************************************//**
 * Translates all messages before they are processed by the main message loop
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pMsg	If non-null, the message.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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
