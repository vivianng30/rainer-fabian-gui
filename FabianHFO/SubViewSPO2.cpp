// SubViewSPO2.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SubViewSPO2.h"
#include "DlgMessageBox.h"

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
extern HFONT g_hf25AcuMed;
extern HFONT g_hf27AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf33AcuBold;
extern HFONT g_hf43AcuBold;
extern HFONT g_hf53AcuBold;


// CSubViewSPO2

IMPLEMENT_DYNAMIC(CSubViewSPO2, CWnd)

CSubViewSPO2::CSubViewSPO2()
{
	m_pModel=NULL;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	m_pcRegionTopLeCorner=NULL;
	m_pcRegionTopRiCorner=NULL;
	m_pcRegionBotLeCorner=NULL;
	m_pcRegionBotRiCorner=NULL;

	m_pcSetupVent_Dis=NULL;
	m_pcSetupVent_Up=NULL;
	m_pcSetupVent_Dw=NULL;
	m_pcSetupVent_Fc=NULL;
	m_pcSetupVentDis_Dw=NULL;
	m_pcSetupVentDis_Fc=NULL;

	m_sliderFastSAT=NULL;

	m_iCurPara=IDC_BTN_SETUP_SPO2MODULE;

	m_pDlg = NULL;

	m_kUp					= VK_UP;//up dw revers
	m_kDown					= VK_DOWN;
	m_kSpace				= VK_SPACE;

	m_dwLastSetupTimer=0;

	m_pcModuleEnabled=NULL;
	m_pcSensitivityMode=NULL;
	m_pcFastSAT=NULL;
	m_pcSensitivity=NULL;
	m_pcAalarmdelay=NULL;
	m_pcAveragingTime=NULL;
	//m_pcSIQlimit=NULL;

	//m_iSpO2SIQlimit=30;
	m_bFastSATon=false;
	m_eSensitivityMode=SPO2_SENSITIVITY_NORMAL;
	m_iSPO2alarmdelay=SPO2_ALARMDELAY_0;
	m_eSPO2averagingTime=SPO2_AVERAGING_8;

	m_iEXCEPTIONS2=0;
	m_iProductID=0;
	m_iVersionDSP=0;
	m_iVersionMCU=0;
	m_szModuleVersion=_T("---");

	m_bySPO2Module=getModel()->getCONFIG()->GetSPO2module();
	if(m_bySPO2Module!=SPO2MODULE_NONE && getModel()->getSPO2())
	{
		//m_iSpO2SIQlimit = getModel()->getCONFIG()->getSpO2SIQlimit();
		m_bFastSATon=getModel()->getCONFIG()->isFastSATon();
		m_eSensitivityMode=getModel()->getCONFIG()->getSensitivityMode();
		m_iSPO2alarmdelay=getModel()->getCONFIG()->getSPO2alarmdelay();
		m_eSPO2averagingTime=getModel()->getCONFIG()->getSPO2averagingTime();
		m_iEXCEPTIONS2=getModel()->getSPO2()->get_EXCEPTION2();

		m_iProductID=getModel()->getSPO2()->get_ProductID();
		m_iVersionMCU=getModel()->getSPO2()->get_VersionMCU();
		m_iVersionDSP=getModel()->getSPO2()->get_VersionDSP();

		
		WORD iVersionDSP = (m_iVersionDSP & 0xF000) >> 12;
		WORD iMajorDSP = (m_iVersionDSP & 0x0F00) >> 8;
		WORD iMinorDSP = (m_iVersionDSP & 0x00F0) >> 4;
		WORD iPatchDSP = (m_iVersionDSP & 0x000F);

		WORD iVersionMCU = (m_iVersionMCU & 0xF000) >> 12;
		WORD iMajorMCU = (m_iVersionMCU & 0x0F00) >> 8;
		WORD iMinorMCU = (m_iVersionMCU & 0x00F0) >> 4;
		WORD iPatchMCU = (m_iVersionMCU & 0x000F);

		m_szModuleVersion.Format(_T("DSP: V%d.%d.%d.%d MCU: V%d.%d.%d.%d PID: %d"),iVersionDSP, iMajorDSP,iMinorDSP,iPatchDSP,iVersionMCU,iMajorMCU,iMinorMCU,iPatchMCU,m_iProductID);
	}

	

	
}

CSubViewSPO2::~CSubViewSPO2()
{
	delete m_pcModuleEnabled;
	delete m_pcSensitivityMode;
	delete m_pcFastSAT;
	delete m_pcSensitivity;
	delete m_pcAalarmdelay;
	delete m_pcAveragingTime;
	//delete m_pcSIQlimit;

	m_pcModuleEnabled=NULL;
	m_pcSensitivityMode=NULL;
	m_pcFastSAT=NULL;
	m_pcSensitivity=NULL;
	m_pcAalarmdelay=NULL;
	m_pcAveragingTime=NULL;
	//m_pcSIQlimit=NULL;

	delete m_pcRegionTopLeCorner;
	m_pcRegionTopLeCorner=NULL;
	delete m_pcRegionTopRiCorner;
	m_pcRegionTopRiCorner=NULL;
	delete m_pcRegionBotLeCorner;
	m_pcRegionBotLeCorner=NULL;
	delete m_pcRegionBotRiCorner;
	m_pcRegionBotRiCorner=NULL;

	delete m_pcSetupVent_Up;
	delete m_pcSetupVent_Dw;
	delete m_pcSetupVent_Fc;
	delete m_pcSetupVent_Dis;
	delete m_pcSetupVentDis_Dw;
	delete m_pcSetupVentDis_Fc;
	m_pcSetupVent_Up=NULL;
	m_pcSetupVent_Dw=NULL;
	m_pcSetupVent_Fc=NULL;
	m_pcSetupVent_Dis=NULL;
	m_pcSetupVentDis_Dw=NULL;
	m_pcSetupVentDis_Fc=NULL;

	delete m_sliderFastSAT;
	m_sliderFastSAT=NULL;
}


BEGIN_MESSAGE_MAP(CSubViewSPO2, CWnd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SETUP_SPO2ALARMDELAY, &CSubViewSPO2::OnBnClickedSPO2alarmdelay)
	ON_BN_CLICKED(IDC_BTN_SETUP_SPO2AVERAGING, &CSubViewSPO2::OnBnClickedSPO2averaging)
	//ON_BN_CLICKED(IDC_BTN_SETUP_SPO2SIQLIMIT, &CSubViewSPO2::OnBnClickedSIQlimit)
	ON_BN_CLICKED(IDC_BTN_SETUP_SPO2MODULE, &CSubViewSPO2::OnBnClickedSPO2module)
	ON_BN_CLICKED(IDC_BTN_SETUP_SENSITIVITYMODE, &CSubViewSPO2::OnBnClickedSensitivityMode)
	ON_BN_CLICKED(IDC_BTN_SETUP_FASTSAT, &CSubViewSPO2::OnBnClickedFastSAT)
	ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
END_MESSAGE_MAP()



// CSubViewSPO2 message handlers


// **************************************************************************
// 
// **************************************************************************
CMVModel *CSubViewSPO2::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}
// **************************************************************************
// 
// **************************************************************************
bool CSubViewSPO2::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
{
	if (!Create(pParentWnd, rc, nID, pContext))
	{
		return false;
	}
	SetTimer(TIMER_SENSORSTATE, 1000, NULL);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CSubViewSPO2::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

		//CBrush cbrBack(RGB(210,210,255));
		CBrush cbrBack(BACKGND);
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		m_pcSetupVent_Dis		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUPVENT_DIS);
		m_pcSetupVent_Up		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUPVENT_UP);
		m_pcSetupVent_Dw		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUPVENT_DW);
		m_pcSetupVent_Fc		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUPVENT_FC);
		m_pcSetupVentDis_Dw		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUPVENTDIS_DW);
		m_pcSetupVentDis_Fc		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUPVENTDIS_FC);

		m_pcRegionTopLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_TOP_CORNER);
		m_pcRegionTopRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_TOP_CORNER);
		m_pcRegionBotLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_BOT_CORNER);
		m_pcRegionBotRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_BOT_CORNER);

		CStringW sz=_T("");

		m_sliderFastSAT = new CBitmapSlider();
		m_sliderFastSAT->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(365,141,514,161), 
			this,IDC_SLD_FASTSAT);
		m_sliderFastSAT->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
		m_sliderFastSAT->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
		m_sliderFastSAT->SetRange( 0, 1 );
		if(m_bFastSATon==true)
		{
			m_sliderFastSAT->SetPos( 1 );
		}
		else
		{
			m_sliderFastSAT->SetPos( 0 );
		}
		m_sliderFastSAT->SetMargin( 5, 0, 6, 0 );
		m_sliderFastSAT->DrawFocusRect( TRUE );

		if(m_bySPO2Module==SPO2MODULE_MASIMO)
			m_sliderFastSAT->ShowWindow(SW_SHOW);
		else
			m_sliderFastSAT->ShowWindow(SW_HIDE);


		BTN btn;

		//################################### 1. SPO2 Module ###########################
		btn.wID					= IDC_BTN_SETUP_SPO2MODULE;	
		btn.poPosition.x		= 221;
		btn.poPosition.y		= 30;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcModuleEnabled=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcModuleEnabled->Create(this,g_hf8AcuBold,0);

		CString szModule=getModel()->GetLanguageString(IDS_BTN_MNU_SPO2);
		
		m_pcModuleEnabled->SetText(szModule);

		switch(m_bySPO2Module)
		{
		case SPO2MODULE_NONE:
			{
				m_pcModuleEnabled->SetValueText(getModel()->GetLanguageString(IDS_TXT_DISABLED));
			}
			break;
		case SPO2MODULE_MASIMO:
			{
				m_pcModuleEnabled->SetValueText(_T("MASIMO: uSpO2 (USB)"));
			}
			break;
		}
		
		m_pcModuleEnabled->ShowWindow(SW_SHOW);
		m_plBtn.AddTail(m_pcModuleEnabled);

		//################################### 2. Sensitivity Mode ###########################
		btn.wID					= IDC_BTN_SETUP_SENSITIVITYMODE;	
		btn.poPosition.x		= 221;
		btn.poPosition.y		= 100;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcSensitivityMode=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcSensitivityMode->Create(this,g_hf8AcuBold,0);
		m_pcSensitivityMode->SetText(getModel()->GetLanguageString(IDS_TXT_SPO_SENSITIVITY)+_T(":"));
		//m_pcSensitivityMode->SetText(_T("Sensitivity mode:"));xxx

		switch(m_eSensitivityMode)
		{
		case SPO2_SENSITIVITY_NORMAL:
			{
				m_pcSensitivityMode->SetValueText(getModel()->GetLanguageString(IDS_TXT_SPO_SENSMODE_NORMAL));
			}
			break;
		case SPO2_SENSITIVITY_MAXIMUM:
			{
				m_pcSensitivityMode->SetValueText(getModel()->GetLanguageString(IDS_TXT_SPO_SENSMODE_MAX));
			}
			break;
		case SPO2_SENSITIVITY_APOD:
			{
				m_pcSensitivityMode->SetValueText(getModel()->GetLanguageString(IDS_TXT_SPO_SENSMODE_APOD));
			}
			break;
		}

		if(m_bySPO2Module==SPO2MODULE_MASIMO)
		{
			m_pcSensitivityMode->ShowWindow(SW_SHOW);
			m_plBtn.AddTail(m_pcSensitivityMode);
		}
		else
		{
			m_pcSensitivityMode->ShowWindow(SW_HIDE);
		}
		

		//################################### 3. Fast SAT on ###########################
		btn.wID					= IDC_BTN_SETUP_FASTSAT;	
		btn.poPosition.x		= 221;
		btn.poPosition.y		= 141;
		btn.pcBmpUp				= m_pcSetupVent_Dis;
		btn.pcBmpDown			= m_pcSetupVentDis_Dw;
		btn.pcBmpFocus			= m_pcSetupVentDis_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Dis;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;


		m_pcFastSAT=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcFastSAT->Create(this,g_hf8AcuBold,0);
		//m_pcFastSAT->SetText(_T("Fast SAT:"));xxx
		m_pcFastSAT->SetText(getModel()->GetLanguageString(IDS_TXT_SPO_FASTSAT)+_T(":"));


		if(m_bySPO2Module==SPO2MODULE_MASIMO)
		{
			m_pcFastSAT->ShowWindow(SW_SHOW);
			m_plBtn.AddTail(m_pcFastSAT);

			if(m_bFastSATon==true)
			{
				m_pcFastSAT->SetValueText(getModel()->GetLanguageString(IDS_TXT_ON));
			}
			else
			{
				m_pcFastSAT->SetValueText(getModel()->GetLanguageString(IDS_TXT_OFF));
			}
		}
		else
		{
			m_pcFastSAT->ShowWindow(SW_HIDE);
		}


		

		//################################### 5. Alarm delay ###########################
		btn.wID					= IDC_BTN_SETUP_SPO2ALARMDELAY;	
		btn.poPosition.x		= 221;
		btn.poPosition.y		= 182;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Dis;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcAalarmdelay=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcAalarmdelay->Create(this,g_hf8AcuBold,0);
		//m_pcAalarmdelay->SetText(_T("Alarm delay:"));xxx
		m_pcAalarmdelay->SetText(getModel()->GetLanguageString(IDS_TXT_SPO_ALARMDELAY)+_T(":"));


		if(m_bySPO2Module==SPO2MODULE_MASIMO)
		{
			m_pcAalarmdelay->ShowWindow(SW_SHOW);
			m_plBtn.AddTail(m_pcAalarmdelay);

			switch(m_iSPO2alarmdelay)
			{
			case SPO2_ALARMDELAY_0:
				{
					sz=_T("0 ");
				}
				break;
			case SPO2_ALARMDELAY_5:
				{
					sz=_T("5 ");
				}
				break;
			case SPO2_ALARMDELAY_10:
				{
					sz=_T("10 ");
				}
				break;
			case SPO2_ALARMDELAY_15:
				{
					sz=_T("15 ");
				}
				break;
			}
			sz+=getModel()->GetLanguageString(IDS_UNIT_SECONDS);
			m_pcAalarmdelay->SetValueText(sz);

		}
		else
		{
			m_pcAalarmdelay->ShowWindow(SW_HIDE);
		}

		//################################### 6. SpO2 averaging time ###########################
		btn.wID					= IDC_BTN_SETUP_SPO2AVERAGING;	
		btn.poPosition.x		= 221;
		btn.poPosition.y		= 223;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Dis;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcAveragingTime=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcAveragingTime->Create(this,g_hf8AcuBold,0);
		//m_pcAveragingTime->SetText(_T("SpO2 averaging time:"));xxx
		m_pcAveragingTime->SetText(getModel()->GetLanguageString(IDS_TXT_SPO_AVERAGING)+_T(":"));


		if(m_bySPO2Module==SPO2MODULE_MASIMO)
		{
			m_pcAveragingTime->ShowWindow(SW_SHOW);
			m_plBtn.AddTail(m_pcAveragingTime);

			switch(m_eSPO2averagingTime)
			{
			case SPO2_AVERAGING_2_4:
				{
					sz=_T("2-4 ");
				}
				break;
			case SPO2_AVERAGING_4_6:
				{
					sz=_T("4-6 ");
				}
				break;
			case SPO2_AVERAGING_8:
				{
					sz=_T("8 ");
				}
				break;
			case SPO2_AVERAGING_10:
				{
					sz=_T("10 ");
				}
				break;
			case SPO2_AVERAGING_12:
				{
					sz=_T("12 ");
				}
				break;
			case SPO2_AVERAGING_14:
				{
					sz=_T("14 ");
				}
				break;
			case SPO2_AVERAGING_16:
				{
					sz=sz=_T("16 ");
				}
				break;
			}
			sz+=getModel()->GetLanguageString(IDS_UNIT_SECONDS);
			m_pcAveragingTime->SetValueText(sz);

		}
		else
		{
			m_pcAveragingTime->ShowWindow(SW_HIDE);
		}

		//################################### 7. SpO2 SIQ alarm limits###########################
		//m_iSpO2SIQlimit = getModel()->getCONFIG()->readSpO2SIQ();
		//btn.wID					= IDC_BTN_SETUP_SPO2SIQLIMIT;	
		//btn.poPosition.x		= 221;
		//btn.poPosition.y		= 264;
		//btn.pcBmpUp				= m_pcSetupVent_Up;
		//btn.pcBmpDown			= m_pcSetupVent_Dw;
		//btn.pcBmpFocus			= m_pcSetupVent_Fc;
		//btn.pcBmpDisabled		= m_pcSetupVent_Dis;
		//btn.dwFormat			= DT_VCENTER|DT_CENTER;

		//m_pcSIQlimit=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		//m_pcSIQlimit->Create(this,g_hf8AcuBold,0);
		////m_pcSIQlimit->SetText(_T("SIQ alarm limit:"));
		//m_pcSIQlimit->SetText(getModel()->GetLanguageString(IDS_TXT_SIQLIMIT)+_T(":"));


		//if(m_bySPO2Module==SPO2MODULE_MASIMO)
		//{
		//	m_pcSIQlimit->ShowWindow(SW_SHOW);
		//	m_plBtn.AddTail(m_pcSIQlimit);

		//	if(m_iSpO2SIQlimit==0)
		//	{
		//		m_pcSIQlimit->SetValueText(getModel()->GetLanguageString(IDS_TXT_OFF));
		//	}
		//	else
		//	{
		//		sz.Format(_T("%d"),m_iSpO2SIQlimit);
		//		m_pcSIQlimit->SetValueText(sz);
		//	}
		//	

		//}
		//else
		//{
		//	m_pcSIQlimit->ShowWindow(SW_HIDE);
		//}

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
void CSubViewSPO2::Show(bool bShow)
{
	if(bShow)
	{
		ShowWindow(SW_SHOW);

		Draw();

		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);

		if(m_bySPO2Module!=SPO2MODULE_NONE)
		{
			SetOneButtonFocused(getModel()->getCONFIG()->GetLastSelectedSVSPO2Btns());
		}
		else
		{
			SetOneButtonFocused(IDC_BTN_SETUP_SPO2MODULE);
			getModel()->getCONFIG()->SetLastSelectedSVSPO2Btns(IDC_BTN_SETUP_SPO2MODULE);
		}
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}

//************************************
// Method:    OnDestroy
// FullName:  CSubViewSPO2::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CSubViewSPO2::OnDestroy()
{
	KillTimer(TIMER_SENSORSTATE);

	if(m_pDlg)
	{
		m_pDlg->EndDialog(IDCLOSE);
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_REDRAW_VIEW);
	}
	m_pDlg = NULL;

	m_plBtn.RemoveAll();

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
void CSubViewSPO2::Draw()
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
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf17AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	CBrush cbrFill;
	cbrFill.CreateSolidBrush(RGB(181,178,181));
	SelectObject(hdcMem,cbrFill);

	Rectangle(hdcMem, 210, 30, 595, 465);
	Rectangle(hdcMem, 230, 10, 575, 30);
	Rectangle(hdcMem, 230, 440, 575, 485);
	if(m_pcRegionTopLeCorner)
		m_pcRegionTopLeCorner->Draw(hdcMem,210,10);
	if(m_pcRegionTopRiCorner)
		m_pcRegionTopRiCorner->Draw(hdcMem,575,10);
	if(m_pcRegionBotLeCorner)
		m_pcRegionBotLeCorner->Draw(hdcMem,210,465);
	if(m_pcRegionBotRiCorner)
		m_pcRegionBotRiCorner->Draw(hdcMem,575,465);


	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	SelectObject(hdcMem,cbrRound);

	SelectObject(hdcMem,g_hf7AcuBold);

	RoundRect(hdcMem, 220, 20, 585, 80,20,20);

	CString cs=_T("");

	if(m_bySPO2Module!=SPO2MODULE_NONE)
	{
		//RoundRect(hdcMem, 220, 90, 585, 191,20,20);
		//RoundRect(hdcMem, 220, 90, 585, 232,20,20);
		RoundRect(hdcMem, 220, 90, 585, 273,20,20);
		//RoundRect(hdcMem, 220, 90, 585, 314,20,20);

		MoveToEx(hdcMem, 221, 140, NULL);
		LineTo(hdcMem, 584, 140);

		MoveToEx(hdcMem, 221, 181, NULL);
		LineTo(hdcMem, 584, 181);

		MoveToEx(hdcMem, 221, 222, NULL);
		LineTo(hdcMem, 584, 222);

		/*MoveToEx(hdcMem, 221, 263, NULL);
		LineTo(hdcMem, 584, 263);*/

		SelectObject(hdcMem,g_hf7AcuNorm);
		rc.left = 220;  
		rc.top = 430;  
		rc.right  = 585;  
		rc.bottom = 450;

		DrawText(hdcMem,m_szModuleVersion,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		
	}

	CBrush cbrInfo(RGB(252,252,170));
	SelectObject(hdcMem,cbrInfo);

	RoundRect(hdcMem, 220, 450, 585, 480,20,20);

	SelectObject(hdcMem,g_hf8AcuBold);

	bool bCheckSensor=false;
	bool bSensorNotConnected=false;
	bool bPulseSearch=false;
	bool bProcessingActive=false;
	bool bDemoMode=false;
	bool bOk=true;

	cs=getModel()->GetLanguageString(IDS_TXT_SENSOR_STATE);
	cs += _T(" ");

	if(m_bySPO2Module!=SPO2MODULE_NONE && getModel()->getSPO2())
	{
		if(getModel()->getSPO2()->isSensorConnectedToUSB()==false)
		{
			bSensorNotConnected=true;
			cs += _T(" ");
			cs += getModel()->GetLanguageString(IDS_TXT_SPO2_NOCONUSB);
			//cs += _T(" not connected to USB,");
			cs += _T(",");
			bOk=false;
		}
		else
		{
			if(m_iEXCEPTIONS2 & BIT0)	// no sensor connected
			{
				cs += _T(" ");
				bSensorNotConnected=true;
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_NOCONMODULE);
				//cs += _T(" not connected to module,");
				cs += _T(",");
				bOk=false;
			}

			if(m_iEXCEPTIONS2 & BIT1)	// defective sensor
			{
				bCheckSensor=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_DEFSENS);
				//cs += _T(" defective sensor,");
				cs += _T(",");
				bOk=false;
			}

			if(m_iEXCEPTIONS2 & BIT2)	// low perfusion
			{
				bCheckSensor=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_LOWPERF);
				//cs += _T(" low perfusion,");
				cs += _T(",");
				bOk=false;
			}

			if(m_iEXCEPTIONS2 & BIT3)	// pulse search
			{
				bPulseSearch=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_PULSESEARCH);
				//cs += _T(" pulse search,");
				cs += _T(",");
			}

			if(m_iEXCEPTIONS2 & BIT4)	// interference detected
			{
				bCheckSensor=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_INTERFERENCE);
				//cs += _T(" interference detected,");
				cs += _T(",");
				bOk=false;
			}

			if(m_iEXCEPTIONS2 & BIT5)	// sensor off patient
			{
				bCheckSensor=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_OFFPATIENT);
				//cs += _T(" sensor off patient,");
				cs += _T(",");
				bOk=false;
			}

			if(m_iEXCEPTIONS2 & BIT6)	// too much ambient light
			{
				bCheckSensor=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_AMBIENTLIGHT);
				//cs += _T(" too much ambient light,");
				cs += _T(",");
				bOk=false;
			}

			if(m_iEXCEPTIONS2 & BIT7)	// unrecognized sensor
			{
				bCheckSensor=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_UNRECSENS);
				//cs += _T(" unrecognized sensor,");
				cs += _T(",");
				bOk=false;
			}

			if(m_iEXCEPTIONS2 & BIT10)	// low signal IQ
			{
				bCheckSensor=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_LOWIQ);
				//cs += _T(" low signal IQ,");
				cs += _T(",");
				bOk=false;
			}

			if(m_iEXCEPTIONS2 & BIT11)	// processing active
			{
				bProcessingActive=true;
				//cs += _T(" processing active,");
			}

			if(m_iEXCEPTIONS2 & BIT12)	// no cable connected
			{
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_NOCABLE);
				//cs += _T(" no cable connected,");
				cs += _T(",");
				bCheckSensor=true;
				bOk=false;
			}

			if(m_iEXCEPTIONS2 & BIT13)	// no adhesive sensor connected
			{
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_NOADHESIVE);
				//cs += _T(" no adhesive sensor connected,");
				cs += _T(",");
				bCheckSensor=true;
				bOk=false;
			}

			if(m_iEXCEPTIONS2 & BIT14)	// demo mode
			{
				bDemoMode=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_DEMO);
				//cs += _T(" demo mode,");
				cs += _T(",");
			}
		}

		if(!bSensorNotConnected && !bCheckSensor && !bPulseSearch && !bDemoMode && bProcessingActive)	// processing active
		{
			cs += _T(" ");
			cs += getModel()->GetLanguageString(IDS_TXT_SPO2_ACTIVE);
			//cs += _T(" processing active,");
			cs += _T(",");
		}
		else if(!bSensorNotConnected && !bCheckSensor && !bPulseSearch && !bProcessingActive && !bDemoMode)
		{
			cs += _T(" ");
			cs += getModel()->GetLanguageString(IDS_TXT_CHCKSENSOR);
			//cs += _T(" checking");
			cs += _T(",");
			bOk=false;
		}
	}
	else
	{
		cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_OFF);
	}
	
	if(bOk==false)
	{
		SetTextColor(hdcMem,RGB(255,0,0));
	}

	rc.left = 230;  
	rc.top = 450;  
	rc.right  = 585;  
	rc.bottom = 480;
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	

	
	

	//dc.BitBlt(0,0,m_lX,m_lY,CDC::FromHandle(hdcMem),0,0,SRCCOPY);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	penLine.DeleteObject();


	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

}
// **************************************************************************
// 
// **************************************************************************
void CSubViewSPO2::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==TIMER_SENSORSTATE)
	{
		if(getModel()->getSPO2()!=NULL)
		{
			bool bDraw=false;
			WORD iTempProductID=getModel()->getSPO2()->get_ProductID();
			WORD iTempVersionMCU=getModel()->getSPO2()->get_VersionMCU();
			WORD iTempVersionDSP=getModel()->getSPO2()->get_VersionDSP();

			if(m_iProductID!=iTempProductID || m_iVersionDSP!=iTempVersionDSP || m_iVersionMCU!=iTempVersionMCU)
			{
				bDraw=true;

				m_iProductID=iTempProductID;
				m_iVersionDSP=iTempVersionDSP;
				m_iVersionMCU=iTempVersionMCU;

				WORD iVersionDSP = (m_iVersionDSP & 0xF000) >> 12;
				WORD iMajorDSP = (m_iVersionDSP & 0x0F00) >> 8;
				WORD iMinorDSP = (m_iVersionDSP & 0x00F0) >> 4;
				WORD iPatchDSP = (m_iVersionDSP & 0x000F);

				WORD iVersionMCU = (m_iVersionMCU & 0xF000) >> 12;
				WORD iMajorMCU = (m_iVersionMCU & 0x0F00) >> 8;
				WORD iMinorMCU = (m_iVersionMCU & 0x00F0) >> 4;
				WORD iPatchMCU = (m_iVersionMCU & 0x000F);

				m_szModuleVersion.Format(_T("DSP: V%d.%d.%d.%d MCU: V%d.%d.%d.%d PID: %d"),iVersionDSP, iMajorDSP,iMinorDSP,iPatchDSP,iVersionMCU,iMajorMCU,iMinorMCU,iPatchMCU,m_iProductID);
			}

			SHORT iEXCEPTIONS2=getModel()->getSPO2()->get_EXCEPTION2();
			if(m_iEXCEPTIONS2!=iEXCEPTIONS2)
			{
				bDraw=true;
				m_iEXCEPTIONS2=iEXCEPTIONS2;
			}

			if(bDraw)
				Draw();
		}
	}

	CWnd::OnTimer(nIDEvent);
}

// **************************************************************************
// 
// **************************************************************************
LRESULT CSubViewSPO2::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	//int iID = 0;
	switch(message)
	{
	case WM_OPEN_SETTING:
		{
			if(m_iCurPara!=IDC_BTN_SETUP_FASTSAT)
				OpenSettingsView();
			return 1;
		}
		break;
	case WM_MENUBUTTONDOWN:
		{
			if(wParam==IDC_BTN_SETUP_FASTSAT)
			{
				if(GetCurrentBtnState()==BS_FOCUSED)
				{
					SetOneButtonDepressed(wParam);
				}
				else if(GetCurrentBtnState()==BS_DOWN)
				{
					SetOneButtonFocused(wParam);
				}
			}
			else
			{
				SetOneButtonDepressed(wParam);
			}
			
			return 1;
		}
		break;
	case WM_BUTTON_VKUP:
		{
			m_iCurPara=wParam;
			return 1;
		}
		break;
	case WM_BUTTON_VKDOWN:
		{
			m_iCurPara=wParam;
			return 1;
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewSPO2::OnBnClickedSPO2module()
{
	SetButtonClicked(IDC_BTN_SETUP_SPO2MODULE);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewSPO2::OnBnClickedSPO2averaging()
{
	SetButtonClicked(IDC_BTN_SETUP_SPO2AVERAGING);
}

// **************************************************************************
// 
// **************************************************************************
//void CSubViewSPO2::OnBnClickedSIQlimit()
//{
//	SetButtonClicked(IDC_BTN_SETUP_SPO2SIQLIMIT);
//}
// **************************************************************************
// 
// **************************************************************************
void CSubViewSPO2::OnBnClickedSPO2alarmdelay()
{
	SetButtonClicked(IDC_BTN_SETUP_SPO2ALARMDELAY);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewSPO2::OnBnClickedSensitivityMode()
{
	SetButtonClicked(IDC_BTN_SETUP_SENSITIVITYMODE);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewSPO2::OnBnClickedFastSAT()
{
	if(getModel()->getSPO2()!=NULL)
		getModel()->getSPO2()->set_FastSATon(m_bFastSATon);
}


// **************************************************************************
// 
// **************************************************************************
void CSubViewSPO2::SetButtonClicked(int btnID)
{
	if(m_iCurPara==btnID)
	{
		eBtnState state = GetCurrentBtnState();


		if(state==BS_DOWN)
		{
			PostMessage(WM_OPEN_SETTING);
		}
	}
	else
	{
		SetOneButtonDepressed(btnID);
		PostMessage(WM_OPEN_SETTING);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewSPO2::SetOneButtonDepressed(int btnID)
{
	POSITION pos;

	try
	{
		for( pos = m_plBtn.GetHeadPosition(); pos != NULL; )
		{
			CSelectSetupBtn* pBtn = m_plBtn.GetNext( pos );

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
				//pBtn->DrawDirectDown();
				pBtn->SetState(BS_DOWN);
				m_iCurPara=btnID;
				getModel()->getCONFIG()->SetLastSelectedSVSPO2Btns(m_iCurPara);
			}
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CSubViewSPO2::SetOneButtonDepressed"));
	}

	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewSPO2::SetOneButtonFocused(int btnID)
{
	POSITION pos;

	try
	{
		for( pos = m_plBtn.GetHeadPosition(); pos != NULL; )
		{
			CSelectSetupBtn* pBtn = m_plBtn.GetNext( pos );

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
				m_iCurPara=btnID;
				getModel()->getCONFIG()->SetLastSelectedSVSPO2Btns(m_iCurPara);
			}
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CSubViewSPO2::SetOneButtonFocused"));
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
void CSubViewSPO2::OpenSettingsView()
{
	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);

	if(GetParent())
		GetParent()->PostMessage(WM_OPEN_SETTING,m_iCurPara);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewSPO2::SetNextButtonFocused()
{
	POSITION pos;

	pos = m_plBtn.GetHeadPosition();

	do {
		CSelectSetupBtn* pBtn  = m_plBtn.GetNext(pos);
		
		if(m_iCurPara == pBtn->GetBtnId())
		{
			pBtn->SetState(BS_UP);

			if(pos == NULL)
			{
				pos = m_plBtn.GetHeadPosition();
				pBtn = m_plBtn.GetNext( pos );

				pBtn->SetState(BS_FOCUSED);
				m_iCurPara=pBtn->GetBtnId();
				getModel()->getCONFIG()->SetLastSelectedSVSPO2Btns(m_iCurPara);
				break;
			}
			else
			{
				pBtn = m_plBtn.GetNext( pos );
				pBtn->SetState(BS_FOCUSED);
				m_iCurPara=pBtn->GetBtnId();
				getModel()->getCONFIG()->SetLastSelectedSVSPO2Btns(m_iCurPara);
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
void CSubViewSPO2::SetPrevButtonFocused()
{
	POSITION pos;

	pos = m_plBtn.GetTailPosition();

	do {
		CSelectSetupBtn* pBtn  = m_plBtn.GetPrev(pos);

		pBtn->SetState(BS_UP);

		if(m_iCurPara == pBtn->GetBtnId())
		{
			if(pos == NULL)
			{
				pos = m_plBtn.GetTailPosition();
				pBtn = m_plBtn.GetPrev( pos );

				pBtn->SetState(BS_FOCUSED);
				m_iCurPara=pBtn->GetBtnId();
				getModel()->getCONFIG()->SetLastSelectedSVSPO2Btns(m_iCurPara);

				break;
			}
			else
			{
				pBtn = m_plBtn.GetPrev( pos );
				pBtn->SetState(BS_FOCUSED);
				m_iCurPara=pBtn->GetBtnId();
				getModel()->getCONFIG()->SetLastSelectedSVSPO2Btns(m_iCurPara);

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
eBtnState CSubViewSPO2::GetCurrentBtnState()
{
	POSITION pos;

	pos = m_plBtn.GetHeadPosition();

	do {
		CSelectSetupBtn* pBtn  = m_plBtn.GetNext(pos);

		if(m_iCurPara == pBtn->GetBtnId())
		{
			return pBtn->GetState();
		}


	} while (pos != NULL);

	return BS_NONE;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CSubViewSPO2::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				//if(m_iCurPara==IDC_BTN_SETUP_FASTSAT)
				//{
				//	if(GetCurrentBtnState()==BS_FOCUSED)
				//	{
				//		SetOneButtonDepressed(m_iCurPara);
				//	}
				//	else if(GetCurrentBtnState()==BS_DOWN)
				//	{
				//		SetOneButtonFocused(m_iCurPara);
				//	}

				//	/*if(m_bFastSATon==true)
				//	{
				//		SetOneButtonDepressed(m_iCurPara);
				//	}
				//	else
				//	{
				//		m_sliderFastSAT->SetPos( 1 );
				//	}*/
				//}
				//else 
				
				if(GetCurrentBtnState()==BS_FOCUSED)
				{
					SetOneButtonDepressed(m_iCurPara);
				}
				else if(GetCurrentBtnState()==BS_DOWN)
				{
					SetOneButtonFocused(m_iCurPara);
					if(m_iCurPara==IDC_BTN_SETUP_FASTSAT)
					{
						if(getModel()->getSPO2()!=NULL)
							getModel()->getSPO2()->set_FastSATon(m_bFastSATon);
					}
				}
				return 1;
				
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				if(m_iCurPara==IDC_BTN_SETUP_FASTSAT)
				{
					if(GetCurrentBtnState()==BS_DOWN)
					{
						m_sliderFastSAT->SetPos( 0 );
						m_bFastSATon=false;
						//getModel()->getCONFIG()->setFastSATon(m_bFastSATon);

						m_pcFastSAT->RefreshValueText(getModel()->GetLanguageString(IDS_TXT_OFF));
					}
					else 
					{
						if(GetCurrentBtnState()==BS_FOCUSED)
							SetPrevButtonFocused();
					}
				}
				else
				{
					if(GetCurrentBtnState()==BS_FOCUSED)
						SetPrevButtonFocused();
				}
				
				return 1;
			}
			else if(pMsg->wParam==VK_UP)
			{
				if(m_iCurPara==IDC_BTN_SETUP_FASTSAT)
				{
					if(GetCurrentBtnState()==BS_DOWN)
					{
						m_sliderFastSAT->SetPos( 1 );
						m_bFastSATon=true;
						//getModel()->getCONFIG()->setFastSATon(m_bFastSATon);

						m_pcFastSAT->RefreshValueText(getModel()->GetLanguageString(IDS_TXT_ON));
						
					}
					else 
					{
						if(GetCurrentBtnState()==BS_FOCUSED)
							SetNextButtonFocused();
					}
				}
				else
				{
					if(GetCurrentBtnState()==BS_FOCUSED)
						SetNextButtonFocused();
				}
				
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
	case WM_KEYUP:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				if(GetCurrentBtnState()==BS_DOWN && m_iCurPara!=IDC_BTN_SETUP_FASTSAT)
				{
					PostMessage(WM_OPEN_SETTING);
				}
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
LRESULT CSubViewSPO2::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case IDC_SLD_FASTSAT:
		{
			if(lParam==0)
			{
				m_bFastSATon=false;
			}
			else
			{
				m_bFastSATon=true;
			}

			if(getModel()->getSPO2()!=NULL)
				getModel()->getSPO2()->set_FastSATon(m_bFastSATon);

			//Draw();
			/*if(m_bFastSATon==true)
			{
				m_pcFastSAT->EnableWindow(TRUE);
			}
			else
			{
				m_pcFastSAT->EnableWindow(FALSE);
			}*/

			CString sz=_T("");

			if(m_bFastSATon==true)
			{
				m_pcFastSAT->RefreshValueText(getModel()->GetLanguageString(IDS_TXT_ON));
			}
			else
			{
				m_pcFastSAT->RefreshValueText(getModel()->GetLanguageString(IDS_TXT_OFF));
			}

			if(GetParent())
				GetParent()->PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	}
	return 1;
}