// SubViewSettings.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndSubSettings.h"
#include "DlgMessageBox.h"

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000

#define MAXVALUES	7


// CSubViewSettings

IMPLEMENT_DYNAMIC(CWndSubSettings, CWnd)

CWndSubSettings::CWndSubSettings(UINT viewFlag)
{
	m_pModel=NULL;
	//m_pDlg = NULL;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	/*m_pcBack_Up=NULL;
	m_pcBack_Dw=NULL;
	m_pcBack_Fc=NULL;
	m_pcBack=NULL;*/

	m_pcWarning= NULL;

	m_szBack=_T("");

	m_iCurPosInBlock=0;
	m_iCurBlockPos=0;
	m_iNumValues=0;

	m_viewFlag=viewFlag;
	m_szSetting=_T("");

	m_pcRegionTopLeCorner=NULL;
	m_pcRegionTopRiCorner=NULL;
	m_pcRegionBotLeCorner=NULL;
	m_pcRegionBotRiCorner=NULL;

	m_pcFocused=NULL;
	m_pcNotSel=NULL;
	m_pcSelected=NULL;

	m_pbtn1=NULL;
	m_pbtn2=NULL;
	m_pbtn3=NULL;
	m_pbtn4=NULL;
	m_pbtn5=NULL;
	m_pbtn6=NULL;
	m_pbtn7=NULL;
	m_pbtn8=NULL;
	//m_pbtn9=NULL;

	m_dwLastSetupTimer=0;
}

CWndSubSettings::~CWndSubSettings()
{
	/*m_pcBack_Up=NULL;
	m_pcBack_Dw=NULL;
	m_pcBack_Fc=NULL;
	m_pcBack=NULL;*/
	m_pcWarning= NULL;

}


BEGIN_MESSAGE_MAP(CWndSubSettings, CWnd)
	ON_WM_DESTROY()
	/*ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
	ON_BN_CLICKED(IDC_BTN_SETUP_1, &CWndSubSettings::OnBnClicked1)
	ON_BN_CLICKED(IDC_BTN_SETUP_2, &CWndSubSettings::OnBnClicked2)
	ON_BN_CLICKED(IDC_BTN_SETUP_3, &CWndSubSettings::OnBnClicked3)
	ON_BN_CLICKED(IDC_BTN_SETUP_4, &CWndSubSettings::OnBnClicked4)
	ON_BN_CLICKED(IDC_BTN_SETUP_5, &CWndSubSettings::OnBnClicked5)
	ON_BN_CLICKED(IDC_BTN_SETUP_6, &CWndSubSettings::OnBnClicked6)
	ON_BN_CLICKED(IDC_BTN_SETUP_7, &CWndSubSettings::OnBnClicked7)*/
END_MESSAGE_MAP()



// CWndSubSettings message handlers

// **************************************************************************
// 
// **************************************************************************
CMVModel *CWndSubSettings::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}
// **************************************************************************
// 
// **************************************************************************
bool CWndSubSettings::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
{
	if (!Create(pParentWnd, rc, nID, pContext))
	{
		return false;
	}

	Initialize();

	return true;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CWndSubSettings::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,/*WS_CHILD|*/WS_VISIBLE,rc,pParentWnd,nID))
	{
		CClientDC dc(this);
		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		//CBrush cbrBack(RGB(210,210,255));
		CBrush cbrBack;
		//cbrFill.CreateSolidBrush(RGB(181,178,181));
		cbrBack.CreateSolidBrush(BACKGND);
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);


		m_pcRegionTopLeCorner= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_SETUP_LE_TOP_CORNER);
		m_pcRegionTopRiCorner= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_SETUP_RI_TOP_CORNER);
		m_pcRegionBotLeCorner= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_SETUP_LE_BOT_CORNER);
		m_pcRegionBotRiCorner= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_SETUP_RI_BOT_CORNER);

		/*m_pcBack_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_BACKSET_UP);
		m_pcBack_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_BACKSET_DW);
		m_pcBack_Fc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_BACKSET_FC);*/

		m_pcNotSel= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_NOT);
		m_pcFocused= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_FOC);
		m_pcSelected= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_SEL);

		m_pcWarning			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_EXCLAMATION_40x35);


		switch(m_viewFlag)
		{
		case IDC_BTN_SCREENLOCKTIME:
			{
				m_szBack=getModel()->GetLanguageString(IDS_BTN_MNU_DIS);
			}
			break;
		case IDC_BTN_SETUP_RANGE:
		case IDC_BTN_SETUP_FUNCITIME:
		case IDC_BTN_SETUP_MANBREATHTIME:
		case IDC_BTN_SETUP_VOLUMETRIG:
		case IDC_BTN_SETUP_TUBESET:
		case IDC_BTN_SETUP_PRESSUREUNIT:
		case IDC_BTN_SETUP_EFLOW:
		case IDC_BTN_SETUP_AUTOOXYCAL:
		case IDC_BTN_SETUP_HFOMANBREATH:
		case IDC_BTN_SETUP_LEAKCOMP:
		case IDC_BTN_SETUP_PPSVABSOLUTE:
			{
				m_szBack=getModel()->GetLanguageString(IDS_BTN_MNU_VENT);
			}
			break;
		case  IDC_BTN_SETUP_CO2MODULE:
		case  IDC_BTN_SETUP_CO2UNITS:
		case  IDC_BTN_SETUP_BAROPRESSURE:
		case  IDC_BTN_SETUP_AUTOONPUMP:
		case  IDC_BTN_SETUP_ETCO2AVG:
			{
				m_szBack=getModel()->GetLanguageString(IDS_BTN_MNU_CO2);
			}
			break;
		case  IDC_BTN_SETUP_SPO2MODULE:
		case  IDC_BTN_SETUP_SPO2ALARMDELAY:
		case  IDC_BTN_SETUP_SPO2AVERAGING:
		case  IDC_BTN_SETUP_SENSITIVITYMODE:
		//case  IDC_BTN_SETUP_SPO2SIQLIMIT:
			{
				m_szBack=getModel()->GetLanguageString(IDS_BTN_MNU_SPO2);
			}
			break;

		}

		BTN btn;

		//m_pcBack
		/*btn.wID					= IDC_BTN_SETTING_BACK;	
		btn.poPosition.x		= 205;
		btn.poPosition.y		= 11;
		btn.pcBmpUp				= m_pcBack_Up;
		btn.pcBmpDown			= m_pcBack_Dw;
		btn.pcBmpFocus			= m_pcBack_Fc;
		btn.pcBmpDisabled		= m_pcBack_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcBack=new CPushBtn(btn,COLOR_TXTBTNUP);
		m_pcBack->Create(this,g_hf15AcuMed,0);
		m_pcBack->SetText(m_szBack);
		m_pcBack->ShowWindow(SW_SHOW);*/

		m_szSetting=_T("");




		btn.wID					= IDC_BTN_SETUP_1;	
		btn.poPosition.x		= 211;
		btn.poPosition.y		= 81;
		btn.pcBmpUp				= m_pcNotSel;
		btn.pcBmpDown			= m_pcSelected;
		btn.pcBmpFocus			= m_pcFocused;
		btn.pcBmpDisabled		= m_pcNotSel;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;

		m_pbtn1=new CSettingBtn(btn,COLOR_TXTBTNUP);
		m_pbtn1->Create(this,g_hf15AcuMed,80);
		m_pbtn1->SetText(_T(""));
		m_pbtn1->ShowWindow(SW_HIDE);
		//m_plBtn.AddTail(m_pbtn1);


		btn.wID					= IDC_BTN_SETUP_2;	
		btn.poPosition.x		= 211;
		btn.poPosition.y		= 127;
		btn.pcBmpUp				= m_pcNotSel;
		btn.pcBmpDown			= m_pcSelected;
		btn.pcBmpFocus			= m_pcFocused;
		btn.pcBmpDisabled		= m_pcNotSel;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;

		m_pbtn2=new CSettingBtn(btn,COLOR_TXTBTNUP);
		m_pbtn2->Create(this,g_hf15AcuMed,80);
		m_pbtn2->SetText(_T(""));
		m_pbtn2->ShowWindow(SW_HIDE);
		//m_plBtn.AddTail(m_pbtn2);

		btn.wID					= IDC_BTN_SETUP_3;	
		btn.poPosition.x		= 211;
		btn.poPosition.y		= 173;
		btn.pcBmpUp				= m_pcNotSel;
		btn.pcBmpDown			= m_pcSelected;
		btn.pcBmpFocus			= m_pcFocused;
		btn.pcBmpDisabled		= m_pcNotSel;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;

		m_pbtn3=new CSettingBtn(btn,COLOR_TXTBTNUP);
		m_pbtn3->Create(this,g_hf15AcuMed,80);
		m_pbtn3->SetText(_T(""));
		m_pbtn3->ShowWindow(SW_HIDE);
		//m_plBtn.AddTail(m_pbtn3);

		btn.wID					= IDC_BTN_SETUP_4;	
		btn.poPosition.x		= 211;
		btn.poPosition.y		= 219;
		btn.pcBmpUp				= m_pcNotSel;
		btn.pcBmpDown			= m_pcSelected;
		btn.pcBmpFocus			= m_pcFocused;
		btn.pcBmpDisabled		= m_pcNotSel;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;

		m_pbtn4=new CSettingBtn(btn,COLOR_TXTBTNUP);
		m_pbtn4->Create(this,g_hf15AcuMed,80);
		m_pbtn4->SetText(_T(""));
		m_pbtn4->ShowWindow(SW_HIDE);
		//m_plBtn.AddTail(m_pbtn4);

		btn.wID					= IDC_BTN_SETUP_5;	
		btn.poPosition.x		= 211;
		btn.poPosition.y		= 265;
		btn.pcBmpUp				= m_pcNotSel;
		btn.pcBmpDown			= m_pcSelected;
		btn.pcBmpFocus			= m_pcFocused;
		btn.pcBmpDisabled		= m_pcNotSel;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;

		m_pbtn5=new CSettingBtn(btn,COLOR_TXTBTNUP);
		m_pbtn5->Create(this,g_hf15AcuMed,80);
		m_pbtn5->SetText(_T(""));
		m_pbtn5->ShowWindow(SW_HIDE);
		//m_plBtn.AddTail(m_pbtn5);

		btn.wID					= IDC_BTN_SETUP_6;	
		btn.poPosition.x		= 211;
		btn.poPosition.y		= 311;
		btn.pcBmpUp				= m_pcNotSel;
		btn.pcBmpDown			= m_pcSelected;
		btn.pcBmpFocus			= m_pcFocused;
		btn.pcBmpDisabled		= m_pcNotSel;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;

		m_pbtn6=new CSettingBtn(btn,COLOR_TXTBTNUP);
		m_pbtn6->Create(this,g_hf15AcuMed,80);
		m_pbtn6->SetText(_T(""));
		m_pbtn6->ShowWindow(SW_HIDE);
		//m_plBtn.AddTail(m_pbtn6);

		btn.wID					= IDC_BTN_SETUP_7;	
		btn.poPosition.x		= 211;
		btn.poPosition.y		= 357;
		btn.pcBmpUp				= m_pcNotSel;
		btn.pcBmpDown			= m_pcSelected;
		btn.pcBmpFocus			= m_pcFocused;
		btn.pcBmpDisabled		= m_pcNotSel;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;

		m_pbtn7=new CSettingBtn(btn,COLOR_TXTBTNUP);
		m_pbtn7->Create(this,g_hf15AcuMed,80);
		m_pbtn7->SetText(_T(""));
		m_pbtn7->ShowWindow(SW_HIDE);
		//m_plBtn.AddTail(m_pbtn7);


		btn.wID					= IDC_BTN_SETUP_8;	
		btn.poPosition.x		= 211;
		btn.poPosition.y		= 403;
		btn.pcBmpUp				= m_pcNotSel;
		btn.pcBmpDown			= m_pcSelected;
		btn.pcBmpFocus			= m_pcFocused;
		btn.pcBmpDisabled		= m_pcNotSel;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;

		m_pbtn8=new CSettingBtn(btn,COLOR_TXTBTNUP);
		m_pbtn8->Create(this,g_hf15AcuMed,80);
		m_pbtn8->SetText(_T(""));
		m_pbtn8->ShowWindow(SW_HIDE);
		//m_plBtn.AddTail(m_pbtn8);


		/*btn.wID					= IDC_BTN_SETUP_9;	
		btn.poPosition.x		= 211;
		btn.poPosition.y		= 449;
		btn.pcBmpUp				= m_pcNotSel;
		btn.pcBmpDown			= m_pcSelected;
		btn.pcBmpFocus			= m_pcFocused;
		btn.pcBmpDisabled		= m_pcNotSel;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;

		m_pbtn9=new CSettingBtn(btn,COLOR_TXTBTNUP);
		m_pbtn9->Create(this,g_hf15AcuMed,80);
		m_pbtn9->SetText(_T(""));
		m_pbtn9->ShowWindow(SW_HIDE);
		m_plBtn.AddTail(m_pbtn9);*/


		SelectObject(m_hDC,hbrprev);
		SelectObject(m_hDC,hpenprev);

		return 1;
	}
	else
		return 0;
}

// **************************************************************************
// 
// **************************************************************************
void CWndSubSettings::Initialize()
{
	//CClientDC dc(this);

	


}

// **************************************************************************
// 
// **************************************************************************
void CWndSubSettings::Show(bool bShow)
{
	if(bShow)
	{
		ShowWindow(SW_SHOW);
		
		Draw();
	}
	else
	{
		ShowWindow(SW_SHOW);
	}
}

//************************************
// Method:    OnDestroy
// FullName:  CWndSubSettings::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndSubSettings::OnDestroy()
{
	m_plBtn.RemoveAll();

	if(m_pcWarning)
		delete m_pcWarning;
	m_pcWarning=NULL;

	delete m_pcRegionTopLeCorner;
	m_pcRegionTopLeCorner=NULL;
	delete m_pcRegionTopRiCorner;
	m_pcRegionTopRiCorner=NULL;
	delete m_pcRegionBotLeCorner;
	m_pcRegionBotLeCorner=NULL;
	delete m_pcRegionBotRiCorner;
	m_pcRegionBotRiCorner=NULL;

	delete m_pbtn1;
	m_pbtn1=NULL;
	delete m_pbtn2;
	m_pbtn2=NULL;
	delete m_pbtn3;
	m_pbtn3=NULL;
	delete m_pbtn4;
	m_pbtn4=NULL;
	delete m_pbtn5;
	m_pbtn5=NULL;
	delete m_pbtn6;
	m_pbtn6=NULL;
	delete m_pbtn7;
	m_pbtn7=NULL;
	delete m_pbtn8;
	m_pbtn8=NULL;
	//delete m_pbtn97;
	//m_pbtn9=NULL;

	delete m_pcFocused;
	m_pcFocused=NULL;
	delete m_pcSelected;
	m_pcSelected=NULL;
	delete m_pcNotSel;
	m_pcNotSel=NULL;

	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);

}

//// **************************************************************************
//// 
//// **************************************************************************
void CWndSubSettings::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);



	CBrush cbrBack(BACKGND);
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf11AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	CBrush cbrFill;
	//cbrFill.CreateSolidBrush(RGB(181,178,181));
	cbrFill.CreateSolidBrush(RGB(150,150,150));
	SelectObject(hdcMem,cbrFill);

	Rectangle(hdcMem, 200, 30, 600, 440);

	Rectangle(hdcMem, 220, 10, 580, 30);
	Rectangle(hdcMem, 220, 440, 580, 460);

	m_pcRegionTopLeCorner->Draw(hdcMem,200,10);
	m_pcRegionTopRiCorner->Draw(hdcMem,580,10);
	m_pcRegionBotLeCorner->Draw(hdcMem,200,440);
	m_pcRegionBotRiCorner->Draw(hdcMem,580,440);

	//CDC* pDCMem=CDC::FromHandle(hdcMem);

	


	rc.left = 220;  
	rc.top = 25;  
	rc.right  = 580;  
	rc.bottom = 500;
	DrawText(hdcMem,m_szSetting,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	CPen penBlack;
	penBlack.CreatePen(PS_SOLID,2,RGB(0,0,0));
	SelectObject(hdcMem,(HPEN)penBlack);
	MoveToEx(hdcMem, 200, 70, NULL);
	LineTo(hdcMem, 600, 70);


	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(200,200,200));
	SelectObject(hdcMem,(HPEN)penLine);

	MoveToEx(hdcMem, 205, 120, NULL);
	LineTo(hdcMem, 595, 120);

	MoveToEx(hdcMem, 205, 166, NULL);
	LineTo(hdcMem, 595, 166);

	MoveToEx(hdcMem, 205, 212, NULL);
	LineTo(hdcMem, 595, 212);

	MoveToEx(hdcMem, 205, 258, NULL);
	LineTo(hdcMem, 595, 258);

	MoveToEx(hdcMem, 205, 304, NULL);
	LineTo(hdcMem, 595, 304);

	MoveToEx(hdcMem, 205, 350, NULL);
	LineTo(hdcMem, 595, 350);

	MoveToEx(hdcMem, 205, 396, NULL);
	LineTo(hdcMem, 595, 396);
	


	//dc.BitBlt(0,0,m_lX,m_lY,pDCMem,0,0,SRCCOPY);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	//DeleteObject(cbrBack);
	penBlack.DeleteObject();
	penLine.DeleteObject();

	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

}

// **************************************************************************
// 
// **************************************************************************
LRESULT CWndSubSettings::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_CLOSE_SETTING:
		{
			if(GetParent())
				GetParent()->PostMessage(WM_CLOSE_SETTING,wParam);
			return 1;
		}
		break;
	/*case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_SETTING_BACK:
				{
					GetParent()->PostMessage(WM_CLOSE_SETTING,m_viewFlag);
					return 1;
				}
				break;
			default:
				{
					
				}
				break;
			}
		}
		break;*/
	}
	return CWnd::WindowProc(message, wParam, lParam);
}


//LRESULT CWndSubSettings::OnMyMessage(WPARAM wParam, LPARAM lParam)
//{
//	switch(wParam)
//	{
//	/*case IDC_SLD_AUTOLOCK:
//		{
//			if(lParam==0)
//			{
//				m_bAutoScreenLock=false;
//			}
//			else
//			{
//				m_bAutoScreenLock=true;
//			}
//
//			getModel()->getCONFIG()->SetAutoScreenlockActive(m_bAutoScreenLock);
//
//			Draw();
//		}
//		break;*/
//	}
//	return 1;
//}

// **************************************************************************
// 
// **************************************************************************
BOOL CWndSubSettings::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE && m_iNumValues>0)
			{
				eBtnState eState = GetCurrentBtnState();

				if(eState==BS_FOCUSED)
					SetOneButtonDepressed(m_iCurPosInBlock);
				else if(eState==BS_DOWN)
					SetOneButtonFocused(m_iCurPosInBlock);
				else
					SetOneButtonFocused(IDC_BTN_SETUP_1);
				return 1;
			}
			else if(pMsg->wParam==VK_DOWN && m_iNumValues>0)
			{
				eBtnState eState = GetCurrentBtnState();
				if(		eState==BS_FOCUSED
					||	eState==BS_DOWN)
					SetPrevButtonFocused();

				return 1;
			}
			else if(pMsg->wParam==VK_UP && m_iNumValues>0)
			{
				eBtnState eState = GetCurrentBtnState();
				if(		eState==BS_FOCUSED
					||	eState==BS_DOWN)
					SetNextButtonFocused();
				return 1;
			}
			else
			{
				//handle focus change manual, draw next focused button and return true
				//return 1;
			}
			break;
		}
		break;
	default:
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}


// **************************************************************************
// 
// **************************************************************************
void CWndSubSettings::SetOneButtonDepressed(int btnID)
{
	 if(m_iNumValues==0)
		 return;
	POSITION pos;

	try
	{
		for( pos = m_plBtn.GetHeadPosition(); pos != NULL; )
		{
			CSettingBtn* pBtn = m_plBtn.GetNext( pos );

			//int iT = pBtn->GetBtnId();

			if(pBtn->GetBtnId() != btnID)
			{
				//if(pBtn->GetState()==CPresetMenuBtn::DOWN)
				{
					pBtn->DrawOK(false);
					pBtn->SetState(BS_UP);

				}
			}
			else
			{
				pBtn->SetState(BS_DOWN);
				pBtn->DrawOK(true);
				m_iCurPosInBlock=btnID;
			}
		}
	}
	catch (...)
	{
		CFabianHFOApp::ReportException(_T("EXCEPTION: CWndSubSettings::SetOneButtonDepressed"));
	}
	
	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CWndSubSettings::SetOneButtonFocused(int btnID)
{
	if(m_iNumValues==0)
		return;
	POSITION pos;
	try
	{
		for( pos = m_plBtn.GetHeadPosition(); pos != NULL; )
		{
			CSettingBtn* pBtn = m_plBtn.GetNext( pos );

			//int iT = pBtn->GetBtnId();

			if(pBtn->GetBtnId() != btnID)
			{
				//if(pBtn->GetState()==CPresetMenuBtn::DOWN)
				{
					pBtn->SetState(BS_UP);
				}
			}
			else
			{
				pBtn->SetState(BS_FOCUSED);
				m_iCurPosInBlock=btnID;
			}
		}
	}
	catch (...)
	{
		CFabianHFOApp::ReportException(_T("EXCEPTION: CWndSubSettings::SetOneButtonFocused"));
	}
	
	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CWndSubSettings::SetNextButtonFocused()
{
	if(m_iNumValues==0)
		return;
	POSITION pos;

	pos = m_plBtn.GetHeadPosition();

	do {
		CSettingBtn* pBtn  = m_plBtn.GetNext(pos);

		//int iT = pBtn->GetBtnId();



		if(m_iCurPosInBlock == pBtn->GetBtnId())
		{
			/*pBtn->SetState(BS_UP);*/

			/*iT = pBtn->GetBtnId();

			int iTest = m_iCurBlockPos;
			iTest = m_iCurPosInBlock;
			iTest = m_iNumValues;*/

			if(pos == NULL )
			{
				if(m_iNumValues==m_iCurBlockPos)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);

					break;
				}
				else if(m_iNumValues>MAXVALUES && m_iCurPosInBlock==IDC_BTN_SETUP_7)
				{
					pBtn->SetState(BS_UP);
					//ReorganizeBtns(true);
					break;
				}
				else
				{
					pBtn->SetState(BS_UP);

					pos = m_plBtn.GetHeadPosition();
					pBtn = m_plBtn.GetNext( pos );

					pBtn->SetState(BS_FOCUSED);
					m_iCurPosInBlock=pBtn->GetBtnId();
					//getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);
					break;
				}
				
			}
			else
			{
				pBtn->SetState(BS_UP);

				pBtn = m_plBtn.GetNext( pos );
				pBtn->SetState(BS_FOCUSED);
				m_iCurPosInBlock=pBtn->GetBtnId();
				break;
			}
		}


	} while (pos != NULL);

	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
}


// **************************************************************************
// 
// **************************************************************************
void CWndSubSettings::SetPrevButtonFocused()
{
	if(m_iNumValues==0)
		return;
	POSITION pos;

	pos = m_plBtn.GetTailPosition();

	do {
		CSettingBtn* pBtn  = m_plBtn.GetPrev(pos);

		//int iT = pBtn->GetBtnId();

		/*pBtn->SetState(BS_UP);*/

		/*int iTest = m_iCurBlockPos;
		iTest = m_iCurPosInBlock;
		iTest = m_iNumValues;*/

		if(m_iCurPosInBlock == pBtn->GetBtnId())
		{
			if(pos == NULL)
			{
				if(MAXVALUES==m_iCurBlockPos && m_iCurPosInBlock==IDC_BTN_SETUP_1)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
					break;
				}
				else if(m_iNumValues>MAXVALUES  && m_iCurBlockPos>MAXVALUES)
				{
					pBtn->SetState(BS_UP);
					//ReorganizeBtns(false);
					break;
				}
				else
				{
					pBtn->SetState(BS_UP);
					pos = m_plBtn.GetTailPosition();
					pBtn = m_plBtn.GetPrev( pos );

					pBtn->SetState(BS_FOCUSED);
					m_iCurPosInBlock=pBtn->GetBtnId();
					//getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);

					break;
				}

				
			}
			else
			{
				pBtn->SetState(BS_UP);
				pBtn = m_plBtn.GetPrev( pos );
				pBtn->SetState(BS_FOCUSED);
				m_iCurPosInBlock=pBtn->GetBtnId();

				break;
			}
		}


	} while (pos != NULL);

	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
}

// **************************************************************************
// 
// **************************************************************************
eBtnState CWndSubSettings::GetCurrentBtnState()
{
	if(m_iNumValues==0)
		return BS_NONE;
	POSITION pos;

	pos = m_plBtn.GetHeadPosition();

	do {
		CSettingBtn* pBtn  = m_plBtn.GetNext(pos);

		//int iT = pBtn->GetBtnId();

		if(m_iCurPosInBlock == pBtn->GetBtnId())
		{
			return pBtn->GetState();
		}


	} while (pos != NULL);

	return BS_NONE;
}

// **************************************************************************
// 
// **************************************************************************
eBtnState CWndSubSettings::GetBtnState(int iID)
{
	if(m_iNumValues==0)
		return BS_NONE;
	POSITION pos;

	pos = m_plBtn.GetHeadPosition();

	do {
		CSettingBtn* pBtn  = m_plBtn.GetNext(pos);

		//int iT = pBtn->GetBtnId();

		if(iID == pBtn->GetBtnId())
		{
			return pBtn->GetState();
		}


	} while (pos != NULL);

	return BS_NONE;
}

