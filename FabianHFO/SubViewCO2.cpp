// SubViewCO2.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SubViewCO2.h"
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


// CSubViewCO2

IMPLEMENT_DYNAMIC(CSubViewCO2, CWnd)

CSubViewCO2::CSubViewCO2()
{
	m_pModel=NULL;

	m_bCO2disabled=false;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;
	m_bInprogress=false;
	//m_sliderAutoOn=NULL;

	m_pcRegionTopLeCorner=NULL;
	m_pcRegionTopRiCorner=NULL;
	m_pcRegionBotLeCorner=NULL;
	m_pcRegionBotRiCorner=NULL;

	m_pcSetupVent_Dis=NULL;
	m_pcSetupVent_Up=NULL;
	m_pcSetupVent_Dw=NULL;
	m_pcSetupVent_Fc=NULL;

	m_pcZero_Up=NULL;
	m_pcZero_Dw=NULL;
	m_pcZero_Fc=NULL;
	m_pcZero=NULL;

	m_iCurPara=IDC_BTN_SETUP_CO2MODULE;

	m_pDlg = NULL;

	m_kUp					= VK_UP;//up dw revers
	m_kDown					= VK_DOWN;
	m_kSpace				= VK_SPACE;

	m_dwLastSetupTimer=0;

	m_pcModuleEnabled=NULL;
	m_pcCO2units=NULL;
	m_pcBarometricP=NULL;
	m_pcAutoOnPump=NULL;
	//m_pcETCO2avg=NULL;


	m_byCO2Module=getModel()->getCONFIG()->GetCO2module();

	if(	getModel()->getCONFIG()->GetMainBoardVersion()<MAINBOARD_30
		&&
		(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_ASCII
		||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_WAVE
		||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_IVOI
		||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL
		||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_ETHERNET_WAVE))
	{
		if(m_byCO2Module!=CO2MODULE_NONE)//should never happen, catched in configuration
		{
			m_byCO2Module=CO2MODULE_NONE;
			getModel()->getCONFIG()->setTempCO2module(m_byCO2Module);
			getModel()->getCONFIG()->SetCO2module(m_byCO2Module);
		}
		

		m_bCO2disabled=true;
		m_byStateBytes=0xFF;
		m_byExtendedStateBytes=0;
		m_bPumpStateOn=false;
		m_szModuleVersion=_T("---");
		m_iMaxOpHRSCalibration=0;
		m_dtCalibrationDateTime.SetStatus(COleDateTime::null);
		m_iRemainServiceHrs=0;
		m_bSystemDateOk=false;
		m_bCalibrationIsDue=false;
		m_bServiceIsDue=false;
	}
	else if(m_byCO2Module!=CO2MODULE_NONE && getModel()->getETCO2())
	{
		m_bCO2disabled=false;
		m_byStateBytes=getModel()->getETCO2()->getStateBytes();
		m_byExtendedStateBytes=getModel()->getETCO2()->getExtendedStateBytes();
		m_bPumpStateOn=getModel()->getETCO2()->isPumpOn();
		m_szModuleVersion=getModel()->getETCO2()->get_ModuleVersion();
		m_iMaxOpHRSCalibration=getModel()->getETCO2()->get_MaxOpHRSCalibration();
		m_dtCalibrationDateTime=getModel()->getETCO2()->get_CalibrationDateTime();
		m_iRemainServiceHrs=getModel()->getETCO2()->get_RemainServiceHrs();
		m_bSystemDateOk=getModel()->getETCO2()->isSystemDateOk();
		m_bCalibrationIsDue=getModel()->getETCO2()->isCalibrationDue();
		m_bServiceIsDue=getModel()->getETCO2()->isServiceDue();
	}
	else
	{
		m_bCO2disabled=false;
		m_byStateBytes=0xFF;
		m_byExtendedStateBytes=0;
		m_bPumpStateOn=false;
		m_szModuleVersion=_T("---");
		m_iMaxOpHRSCalibration=0;
		m_dtCalibrationDateTime.SetStatus(COleDateTime::null);
		m_iRemainServiceHrs=0;
		m_bSystemDateOk=false;
		m_bCalibrationIsDue=false;
		m_bServiceIsDue=false;
	}
	m_eCO2units=getModel()->getCONFIG()->GetCO2unit();
	
	
	if(m_byCO2Module==CO2MODULE_MICROPOD)
	{
		m_iBarometricP=getModel()->getCONFIG()->GetCO2BaroPressure();
	}
	else
	{
		WORD iAltitude=getModel()->getI2C()->ReadConfigWord(ALTITUDE_16);
		m_iBarometricP=((double)1013*pow(((double)1-(double)0.000022558*iAltitude),(double)5.2561))*0.75;
	}

	m_iAutoOnPump=getModel()->getCONFIG()->GetTimePumpAutoOn();
}

CSubViewCO2::~CSubViewCO2()
{
	delete m_pcModuleEnabled;
	delete m_pcCO2units;
	delete m_pcBarometricP;
	delete m_pcAutoOnPump;
	//delete m_pcETCO2avg;

	m_pcModuleEnabled=NULL;
	m_pcCO2units=NULL;
	m_pcBarometricP=NULL;
	m_pcAutoOnPump=NULL;
	//m_pcETCO2avg=NULL;

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
	m_pcSetupVent_Up=NULL;
	m_pcSetupVent_Dw=NULL;
	m_pcSetupVent_Fc=NULL;
	m_pcSetupVent_Dis=NULL;

	delete m_pcZero_Up;
	delete m_pcZero_Dw;
	delete m_pcZero_Fc;
	delete m_pcZero;

	m_pcZero_Up=NULL;
	m_pcZero_Dw=NULL;
	m_pcZero_Fc=NULL;
	m_pcZero=NULL;

}


BEGIN_MESSAGE_MAP(CSubViewCO2, CWnd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SETUP_CO2MODULE, &CSubViewCO2::OnBnClickedCO2module)
	ON_BN_CLICKED(IDC_BTN_SETUP_CO2UNITS, &CSubViewCO2::OnBnClickedCO2units)
	ON_BN_CLICKED(IDC_BTN_SETUP_BAROPRESSURE, &CSubViewCO2::OnBnClickedBarometricPressure)
	ON_BN_CLICKED(IDC_BTN_SETUP_AUTOONPUMP, &CSubViewCO2::OnBnClickedAutoOnPump)
	ON_BN_CLICKED(IDC_BTN_SETUP_ETCO2AVG, &CSubViewCO2::OnBnClickedETCO2avg)
	ON_BN_CLICKED(IDC_BTN_SETUP_ZEROCAL, &CSubViewCO2::OnBnClickedZeroCal)
	//ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
END_MESSAGE_MAP()



// CSubViewCO2 message handlers



// **************************************************************************
// 
// **************************************************************************
CMVModel *CSubViewCO2::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}
// **************************************************************************
// 
// **************************************************************************
bool CSubViewCO2::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
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
BOOL CSubViewCO2::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

		m_pcRegionTopLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_TOP_CORNER);
		m_pcRegionTopRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_TOP_CORNER);
		m_pcRegionBotLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_BOT_CORNER);
		m_pcRegionBotRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_BOT_CORNER);

		m_pcZero_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_CALIBRATE_UP);
		m_pcZero_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_CALIBRATE_DW);
		m_pcZero_Fc		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_CALIBRATE_FC);
		

		CStringW sz=_T("");

		BTN btn;



		//################################### 1. CO2 Module ###########################
		btn.wID					= IDC_BTN_SETUP_CO2MODULE;	
		btn.poPosition.x		= 221;
		btn.poPosition.y		= 30;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Dis;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcModuleEnabled=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcModuleEnabled->Create(this,g_hf8AcuBold,0);
		//m_pcModuleEnabled->SetText(_T("CO2 module:"));

		CString szModule=getModel()->GetLanguageString(IDS_BTN_MNU_CO2);
		/*if(m_byCO2Module==CO2MODULE_MICROPOD)
		{
			szModule+=_T(" (MicroPod):");
		}
		else
		{
			szModule+=_T(" (Capnostat):");
		}*/
		m_pcModuleEnabled->SetText(szModule);

		switch(m_byCO2Module)
		{
		case CO2MODULE_NONE:
			{
				if(m_bCO2disabled)
				{
					CString szPDMS=getModel()->GetLanguageString(IDS_TXT_DISABLED);
					szPDMS+=_T(": ");
					szPDMS+=getModel()->GetLanguageString(IDS_BTN_MNU_PDMS);
					m_pcModuleEnabled->SetValueText(szPDMS);
				}
				else
				{
					m_pcModuleEnabled->SetValueText(getModel()->GetLanguageString(IDS_TXT_DISABLED));
				}
			}
			break;
		case CO2MODULE_CAPNOSTAT:
			{
				//m_pcModuleEnabled->SetValueText(getModel()->GetLanguageString(IDS_TXT_GENERICCO2));
				m_pcModuleEnabled->SetValueText(_T("Respironics (Philips)"));
			}
			break;
		case CO2MODULE_MICROPOD:
			{
				m_pcModuleEnabled->SetValueText(_T("MicroPod"));
			}
			break;
		}
		/*if(m_bCO2ModuleEnabled)
		{
			m_pcModuleEnabled->SetValueText(getModel()->GetLanguageString(IDS_TXT_ENABLED));
		}
		else
		{
			m_pcModuleEnabled->SetValueText(getModel()->GetLanguageString(IDS_TXT_DISABLED));
		}*/
		m_pcModuleEnabled->ShowWindow(SW_SHOW);
		m_plBtn.AddTail(m_pcModuleEnabled);

		

		//################################### 2. CO2 Units ###########################
		btn.wID					= IDC_BTN_SETUP_CO2UNITS;	
		btn.poPosition.x		= 221;
		btn.poPosition.y		= 100;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcCO2units=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcCO2units->Create(this,g_hf8AcuBold,0);
		m_pcCO2units->SetText(getModel()->GetLanguageString(IDS_TXT_CO2UNITS)+_T(":"));

		switch(m_eCO2units)
		{
		case CO2_UNIT_KPA:
			{
				m_pcCO2units->SetValueText(getModel()->GetLanguageString(IDS_UNIT_KPA));
			}
			break;
		case CO2_UNIT_PERCENT:
			{
				m_pcCO2units->SetValueText(getModel()->GetLanguageString(IDS_UNIT_VOLPERCENT));
			}
			break;
		case CO2_UNIT_MMHG:
		default:
			{
				m_pcCO2units->SetValueText(getModel()->GetLanguageString(IDS_UNIT_MMHG));
			}
			break;
		}
		
		
		if(m_byCO2Module!=CO2MODULE_NONE)
		{
			m_pcCO2units->ShowWindow(SW_SHOW);
			//m_pcCO2units->EnableWindow(TRUE);
			m_plBtn.AddTail(m_pcCO2units);
		}
		else
		{
			m_pcCO2units->ShowWindow(SW_HIDE);
			//m_pcCO2units->EnableWindow(FALSE);
		}

		//################################### 3. Barometric Pressure ###########################
		btn.wID					= IDC_BTN_SETUP_BAROPRESSURE;	
		btn.poPosition.x		= 221;
		btn.poPosition.y		= 141;
		/*if(m_byCO2Module==CO2MODULE_MICROPOD)
		{
			btn.pcBmpUp				= m_pcSetupVent_Dis;
		}
		else
		{
			btn.pcBmpUp				= m_pcSetupVent_Up;
		}*/
		btn.pcBmpUp				= m_pcSetupVent_Dis;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Dis;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcBarometricP=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcBarometricP->Create(this,g_hf8AcuBold,0);
		m_pcBarometricP->SetText(getModel()->GetLanguageString(IDS_TXT_BAROMETRICPRESSURE)+_T(":"));

		CStringW szUnit=getModel()->GetLanguageString(IDS_UNIT_MMHG);

		sz.Format(_T("%d %s"),m_iBarometricP,szUnit);
		m_pcBarometricP->SetValueText(sz);
		
		m_pcBarometricP->ShowWindow(SW_SHOW);
		
		if(m_byCO2Module!=CO2MODULE_NONE)
		{
			m_pcBarometricP->ShowWindow(SW_SHOW);
			m_pcBarometricP->EnableWindow(FALSE);
			/*if(m_byCO2Module==CO2MODULE_MICROPOD)
			{
				m_pcBarometricP->EnableWindow(FALSE);
			}
			else
			{
				m_pcBarometricP->EnableWindow(TRUE);
				m_plBtn.AddTail(m_pcBarometricP);
			}*/
			
		}
		else
		{
			m_pcBarometricP->ShowWindow(SW_HIDE);
			//m_pcBarometricP->EnableWindow(FALSE);
		}


		//################################### 4. Automatic Pump on ###########################
		btn.wID					= IDC_BTN_SETUP_AUTOONPUMP;	
		btn.poPosition.x		= 221;
		btn.poPosition.y		= 182;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Dis;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcAutoOnPump=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcAutoOnPump->Create(this,g_hf8AcuBold,0);
		//m_pcAutoOnPump->SetText(_T("Automatic pump on:"));
		m_pcAutoOnPump->SetText(getModel()->GetLanguageString(IDS_TXT_CO2AUTOPUMP)+_T(":"));
		

		if(m_byCO2Module==CO2MODULE_MICROPOD)
		{
			m_pcAutoOnPump->ShowWindow(SW_SHOW);
			m_plBtn.AddTail(m_pcAutoOnPump);

			/*if(m_iAutoOnPump!=0)
			{
				m_pcAutoOnPump->EnableWindow(TRUE);
			}
			else
			{
				m_pcAutoOnPump->EnableWindow(FALSE);
			}*/

			switch(m_iAutoOnPump)
			{
			case 0:
				{
					sz=getModel()->GetLanguageString(IDS_TXT_OFF);
				}
				break;
			case 5:
				{
					szUnit=getModel()->GetLanguageString(IDS_UNIT_MIN);
					sz.Format(_T("%d %s"),5,szUnit);
				}
				break;
			case 10:
				{
					szUnit=getModel()->GetLanguageString(IDS_UNIT_MIN);
					sz.Format(_T("%d %s"),10,szUnit);
				}
				break;
			case 15:
				{
					szUnit=getModel()->GetLanguageString(IDS_UNIT_MIN);
					sz.Format(_T("%d %s"),15,szUnit);
				}
				break;
			case 30:
				{
					szUnit=getModel()->GetLanguageString(IDS_UNIT_MIN);
					sz.Format(_T("%d %s"),30,szUnit);
				}
				break;
			}
			m_pcAutoOnPump->SetValueText(sz);

		}
		else
		{
			m_pcAutoOnPump->ShowWindow(SW_HIDE);
		}

		


		//###################################5. ETCO2 Average ###########################
		/*btn.wID					= IDC_BTN_SETUP_ETCO2AVG;	
		btn.poPosition.x		= 21;
		btn.poPosition.y		= 218;
		btn.pcBmpUp				= m_pcSetupVent_Up;
		btn.pcBmpDown			= m_pcSetupVent_Dw;
		btn.pcBmpFocus			= m_pcSetupVent_Fc;
		btn.pcBmpDisabled		= m_pcSetupVent_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcETCO2avg=new CSelectSetupBtn(btn,COLOR_TXTBTNUP,true);
		m_pcETCO2avg->Create(this,g_hf8AcuBold,0);
		m_pcETCO2avg->SetText(getModel()->GetLanguageString(IDS_TXT_AUTOCAL_OXY)+_T(":"));

		if(m_pcETCO2avg==CAL_21)
		{
			m_pcAutoOxyCal->SetValueText(getModel()->GetLanguageString(IDS_TXT_21PERC));
		}
		else
		{
			m_pcAutoOxyCal->SetValueText(getModel()->GetLanguageString(IDS_TXT_21_100PERC));
		}
		m_pcETCO2avg->ShowWindow(SW_SHOW);
		m_plBtn.AddTail(m_pcETCO2avg);*/


		//################################### Button Calibrate ###########################


		btn.wID					= IDC_BTN_SETUP_ZEROCAL;	
		btn.poPosition.x		= 415;
		btn.poPosition.y		= 248;//386;
		btn.pcBmpUp				= m_pcZero_Up;
		btn.pcBmpDown			= m_pcZero_Dw;
		btn.pcBmpFocus			= m_pcZero_Fc;
		btn.pcBmpDisabled		= m_pcZero_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcZero = new CSelectSetupBtn(btn,COLOR_TXTBTNUP,false);
		m_pcZero->Create(this,g_hf8AcuBold,40);
		m_pcZero->SetText(getModel()->GetLanguageString(IDS_BTN_CALIBRATION));
		//m_pcZero->SetText(_T("calibrate"));

		if(m_byCO2Module==CO2MODULE_CAPNOSTAT)
		{
			m_pcZero->ShowWindow(SW_SHOW);
			m_plBtn.AddTail(m_pcZero);
		}
		else
		{
			m_pcZero->ShowWindow(SW_HIDE);
		}

		if(m_bCO2disabled)
		{
			m_pcModuleEnabled->EnableWindow(FALSE);
		}
		else
		{
			m_bInprogress=getModel()->isCO2inprogress();
			if(m_bInprogress)
			{
				m_pcModuleEnabled->EnableWindow(FALSE);
				m_pcCO2units->EnableWindow(FALSE);
				m_pcBarometricP->EnableWindow(FALSE);
				m_pcAutoOnPump->EnableWindow(FALSE);

				m_pcZero->ShowWindow(SW_HIDE);
			}
			else
			{
				m_pcModuleEnabled->EnableWindow();
				m_pcCO2units->EnableWindow();
				m_pcBarometricP->EnableWindow();
				m_pcAutoOnPump->EnableWindow();

				if(m_byCO2Module==CO2MODULE_CAPNOSTAT)
				{
					m_pcZero->ShowWindow(SW_SHOW);
				}
				else
				{
					m_pcZero->ShowWindow(SW_HIDE);
				}
			}
		}
		

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
//void CSubViewCO2::Initialize()
//{
//	CClientDC dc(this);
//
//	
//
//
//	//SetTimer(SERVICETIMER, 30000, NULL);
//
//}

// **************************************************************************
// 
// **************************************************************************
void CSubViewCO2::Show(bool bShow)
{
	if(bShow)
	{
		ShowWindow(SW_SHOW);

		Draw();

		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);

		if(m_byCO2Module!=CO2MODULE_NONE)
		{
			SetOneButtonFocused(getModel()->getCONFIG()->GetLastSelectedSVCO2Btns());
		}
		else
		{
			SetOneButtonFocused(IDC_BTN_SETUP_CO2MODULE);
			getModel()->getCONFIG()->SetLastSelectedSVCO2Btns(IDC_BTN_SETUP_CO2MODULE);
		}
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}

//************************************
// Method:    OnDestroy
// FullName:  CSubViewCO2::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CSubViewCO2::OnDestroy()
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
void CSubViewCO2::Draw()
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

	if(m_byCO2Module!=CO2MODULE_NONE)
	{
		RoundRect(hdcMem, 220, 90, 585, 232,20,20);

		MoveToEx(hdcMem, 221, 140, NULL);
		LineTo(hdcMem, 584, 140);

		MoveToEx(hdcMem, 221, 181, NULL);
		LineTo(hdcMem, 584, 181);

		if(m_byCO2Module==CO2MODULE_CAPNOSTAT)
		{
			RoundRect(hdcMem, 220, 242, 585, 302,20,20);

			RoundRect(hdcMem, 220, 430, 585, 450,20,20);

			rc.left = 230;  
			rc.top = 242;  
			rc.right  = 585;  
			rc.bottom = 303;
			DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_CO2_ZEROCAL),-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
		else if(m_byCO2Module==CO2MODULE_MICROPOD)
		{
			RoundRect(hdcMem, 220, 360, 585, 450,20,20);

			
			rc.left = 220;  
			rc.top = 370;  
			rc.right  = 585;  
			rc.bottom = 390;
			CString szCalDate=_T("");
			if(m_dtCalibrationDateTime.GetStatus() != COleDateTime::null)
			{
				szCalDate.Format(_T("%02d.%02d.%04d"),
					m_dtCalibrationDateTime.GetDay(),
					m_dtCalibrationDateTime.GetMonth(),
					m_dtCalibrationDateTime.GetYear());
				cs=getModel()->GetLanguageString(IDS_TXT_DATELAST_CO2CAL);
				cs+=_T(": ");
				cs+=szCalDate;
			}
			if(m_bSystemDateOk==false)
			{
				cs+=_T(" (");
				cs+=getModel()->GetLanguageString(IDS_TXT_WRONG_SYSTEMDATE);
				cs+=_T(")");
			}
			DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

			rc.left = 220;  
			rc.top = 390;  
			rc.right  = 585;  
			rc.bottom = 410;
			if(m_iRemainServiceHrs>0)
			{
				CString csTemp=getModel()->GetLanguageString(IDS_TXT_MAXOPHRS_SERVICE);
				csTemp+=_T(": ");
				cs.Format(_T("%s %d"),csTemp, m_iRemainServiceHrs);
			}
			else if(m_iRemainServiceHrs==0)
			{
				cs=getModel()->GetLanguageString(IDS_TXT_MAXOPHRS_SERVICE);
				cs+=_T(": ");
				cs+=getModel()->GetLanguageString(IDS_TXT_SERVICE_REQUIRE);
			}
			else
			{
				cs=getModel()->GetLanguageString(IDS_TXT_MAXOPHRS_SERVICE);
				cs+=_T(": ");
				cs=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);
			}
			DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

			rc.left = 220;  
			rc.top = 410;  
			rc.right  = 585;  
			rc.bottom = 430;
			if(m_iMaxOpHRSCalibration>0)
			{
				CString csTemp=getModel()->GetLanguageString(IDS_TXT_MAXOPHRS_CALIBRATION);
				csTemp+=_T(": ");
				cs.Format(_T("%s %d"),csTemp, m_iMaxOpHRSCalibration);
				
			}
			else if(m_iMaxOpHRSCalibration==0)
			{
				cs=getModel()->GetLanguageString(IDS_TXT_MAXOPHRS_CALIBRATION);
				cs+=_T(": ");
				cs+=getModel()->GetLanguageString(IDS_TXT_CO2CAL_REQUIRE);
			}
			else
			{
				cs=getModel()->GetLanguageString(IDS_TXT_MAXOPHRS_CALIBRATION);
				cs+=_T(": ");
				cs=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);
			}

			DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}

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

	cs=getModel()->GetLanguageString(IDS_TXT_SENSOR_STATE);
	cs += _T(" ");

	bool bInprogress=getModel()->isCO2inprogress();

	bool bOk=true;
	if(m_byCO2Module==CO2MODULE_CAPNOSTAT)
	{
		switch(m_byStateBytes)
		{
		case 0xFF:
			{
				//cs = _T("Sensor state: Sensor off");
				cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_OFF);
				bOk=false;
			}
			break;
		case CO2_CAPNOSTAT_ERROR_PRIO_SENSOROVERTEMP:
			{
				//cs = _T("Sensor state: Sensor Over Temperature");
				cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_OVERTEMP);
				bOk=false;
			}
			break;
		case CO2_CAPNOSTAT_ERROR_PRIO_SENSORFAULTY:
			{
				//cs = _T("Sensor state: Sensor Faulty");
				cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_FAULTY);
				bOk=false;
			}
			break;
		case CO2_CAPNOSTAT_ERROR_PRIO_NOMESSAGE:
			{
				//cs = _T("Sensor state: No Message");
				cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_NOMESSAGE);
				bOk=false;
			}
			break;
		case CO2_CAPNOSTAT_ERROR_PRIO_SLEEPMODE:
			{
				//cs = _T("Sensor state: Capnostat in Sleep Mode");
				cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_SLEEP);
				bOk=false;
			}
			break;
		case CO2_CAPNOSTAT_ERROR_PRIO_ZEROINPROGRESS:
			{
				//cs = _T("Sensor state: Zero in Progress");
				cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_ZEROPROGRESS);
				bOk=false;
			}
			break;
		case CO2_CAPNOSTAT_ERROR_PRIO_WARMUP:
			{
				//cs = _T("Sensor state: Sensor Warm Up");
				cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_WARMUP);
				bOk=false;
			}
			break;
		case CO2_CAPNOSTAT_ERROR_PRIO_ZEROREQUIRED:
			{
				//cs = _T("Sensor state: Zero Required");
				cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_ZEROREQUIRED);
				bOk=false;
			}
			break;
		case CO2_CAPNOSTAT_ERROR_PRIO_CO2OUTOFRANGE:
			{
				//cs = _T("Sensor state: CO2 out of Range");
				cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_OUTOFRANGE);
				bOk=false;
			}
			break;
		case CO2_CAPNOSTAT_ERROR_PRIO_CHECKAIRWAY:
			{
				//cs = _T("Sensor state: Check Airway Adapter");
				cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_CHECKAIRWAY);
				bOk=false;
			}
			break;
		case CO2_CAPNOSTAT_ERROR_PRIO_CHECKSAMPLING:
			{
				//cs = _T("Sensor state: Check Sampling Line");
				cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_CHECKSAMPLING);
				bOk=false;
			}
			break;
		default:
			{
				//cs = _T("Sensor state: ok");
				cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_OK);
			}
			break;
		}
	}
	else if(m_byCO2Module==CO2MODULE_MICROPOD && getModel()->getETCO2())
	{
	
		if(m_byStateBytes==0xFF)
		{
			cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_OFF);
			bOk=false;
		}
		else if(getModel()->getETCO2()->get_StandbyMode())
		{
			cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_STANDBY);
			bOk=false;
		}
		else if(m_bPumpStateOn==false)
		{
			cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_PUMPOFF);
			bOk=false;
		}
		else
		{
			if(m_byStateBytes & BIT7)
			{
				//fault bit -> check extended status
				if(m_byExtendedStateBytes & BIT0)
				{
					//Check calibration
					cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_CHECKCAL);
					bOk=false;
				}
				else if(m_byExtendedStateBytes & BIT1)
				{
					//Check flow
					cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_CHECKFLOW);
					bOk=false;
				}
				else if(m_byExtendedStateBytes & BIT2)
				{
					//Occlusion in gas input line
					cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_OCCLUSION);
					bOk=false;
				}
				else if(m_byExtendedStateBytes & BIT3)
				{
					//Temperature out of range
					cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_OVERTEMP);
					bOk=false;
				}
				else if(m_byExtendedStateBytes & BIT7)
				{
					//malfunction, service code is set
					cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_MALFUNC);
					bOk=false;
				}
			}
			else if(m_byStateBytes & BIT6)
			{
				//filter line not connected
				cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_LINENOTCON);
				bOk=false;
			}
			else if(m_byStateBytes & BIT5)
			{
				//purging in progress
				cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_PURGING);
				bOk=false;
			}
			else if(m_byStateBytes & BIT4)
			{
				//SFM in progress
				cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_SFM);
				bOk=false;
			}
			else if(m_byStateBytes & BIT2)
			{
				//CO2 value over-range
				cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_CO2RANGE);
				bOk=false;
			}
			else if(m_byStateBytes & BIT1)
			{
				//initialization
				cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_INIT);
				bOk=false;
			}
			else if(m_byStateBytes & BIT0)
			{
				//invalid CO2 value
				cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_INVALIDCO2);
				bOk=false;
			}
			
			
			//if(m_byStateBytes & BIT3)
			//{
			//	//SFM request
			//	//cs+=_T("Sensor state: xxx");
			//}
		}

		if(bOk)
		{
			cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_OK);
			if(m_bServiceIsDue)
			{
				bOk=false;
				cs += _T(" (");
				cs += getModel()->GetLanguageString(IDS_TXT_SERVICE_REQUIRE);
				cs += _T(")");
			}
			else if(m_bCalibrationIsDue)
			{
				bOk=false;
				cs += _T(" (");
				cs += getModel()->GetLanguageString(IDS_TXT_CO2CAL_REQUIRE);
				cs += _T(")");
			}
		}

	}
	else
	{
		cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_OFF);
	}

	if(bInprogress)
	{
		SetTextColor(hdcMem,RGB(255,0,0));
		cs = getModel()->GetLanguageString(IDS_TXT_SENSOR_INIT);
	}
	else if(bOk==false)
	{
		SetTextColor(hdcMem,RGB(255,0,0));
	}

	rc.left = 230;  
	rc.top = 450;  
	rc.right  = 585;  
	rc.bottom = 480;
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	//m_byPrioritizedCO2Status
	

	/*MoveToEx(hdcMem, 221, 152, NULL);
	LineTo(hdcMem, 584, 152);*/

	//RoundRect(hdcMem, 20, 273, 385, 415,20,20);

	//MoveToEx(hdcMem, 21, 323, NULL);
	//LineTo(hdcMem, 384, 323);

	//MoveToEx(hdcMem, 21, 364, NULL);
	//LineTo(hdcMem, 384, 364);

	//RoundRect(hdcMem, 20, 420, 385, 480,20,20);

	//#####################################################



	/*RoundRect(hdcMem, 415, 20, 780, 162,20,20);

	MoveToEx(hdcMem, 416, 70, NULL);
	LineTo(hdcMem, 779, 70);

	MoveToEx(hdcMem, 416, 111, NULL);
	LineTo(hdcMem, 779, 111);

	RoundRect(hdcMem, 415, 420, 780, 480,20,20);

	SelectObject(hdcMem,g_hf8AcuBold);

	rc.left = 425;  
	rc.top = 420;  
	rc.right  = 780;  
	rc.bottom = 480;
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_BTN_DEFAULTS),-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);*/


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
void CSubViewCO2::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==TIMER_SENSORSTATE)
	{
		m_dtCalibrationDateTime.SetStatus(COleDateTime::null);
		BYTE byStateBytes=0xFF;
		BYTE byExtendedStateBytes=0;
		bool bPumpStateOn=false;
		int iMaxOpHRSCalibration=0;
		m_szModuleVersion=_T("---");
		int iRemainServiceHrs=0;
		bool bSystemDateOk=false;
		bool bCalibrationIsDue=false;
		bool bServiceIsDue=false;

		if(getModel()->getETCO2()!=NULL)
		{
			byStateBytes=getModel()->getETCO2()->getStateBytes();
			byExtendedStateBytes=getModel()->getETCO2()->getExtendedStateBytes();
			bPumpStateOn=getModel()->getETCO2()->isPumpOn();
			iMaxOpHRSCalibration=getModel()->getETCO2()->get_MaxOpHRSCalibration();
			m_szModuleVersion=getModel()->getETCO2()->get_ModuleVersion();
			m_dtCalibrationDateTime=getModel()->getETCO2()->get_CalibrationDateTime();
			iRemainServiceHrs=getModel()->getETCO2()->get_RemainServiceHrs();
			bSystemDateOk=getModel()->getETCO2()->isSystemDateOk();
			bCalibrationIsDue=getModel()->getETCO2()->isCalibrationDue();
			bServiceIsDue=getModel()->getETCO2()->isServiceDue();
		}
		
		//CO2RKU
		bool bInprogress=getModel()->isCO2inprogress();
		if(bInprogress || m_bCO2disabled)
		{
			m_pcModuleEnabled->EnableWindow(FALSE);
			m_pcCO2units->EnableWindow(FALSE);
			m_pcBarometricP->EnableWindow(FALSE);
			m_pcAutoOnPump->EnableWindow(FALSE);

			m_pcZero->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pcModuleEnabled->EnableWindow();
			m_pcCO2units->EnableWindow();
			m_pcBarometricP->EnableWindow();
			m_pcAutoOnPump->EnableWindow();

			if(m_byCO2Module==CO2MODULE_CAPNOSTAT)
			{
				m_pcZero->ShowWindow(SW_SHOW);
			}
			else
			{
				m_pcZero->ShowWindow(SW_HIDE);
			}
		}

		if(		byStateBytes!=m_byStateBytes
			||	byExtendedStateBytes!=m_byExtendedStateBytes
			||	bPumpStateOn!=m_bPumpStateOn
			||	iMaxOpHRSCalibration!=m_iMaxOpHRSCalibration
			||	iRemainServiceHrs!=m_iRemainServiceHrs
			||	bSystemDateOk!=m_bSystemDateOk
			||	bCalibrationIsDue!=m_bCalibrationIsDue
			||	bServiceIsDue!=m_bServiceIsDue
			||	bInprogress!=m_bInprogress)
		{
			m_bInprogress=bInprogress;
			m_bServiceIsDue=bServiceIsDue;
			m_bCalibrationIsDue=bCalibrationIsDue;
			m_bSystemDateOk=bSystemDateOk;
			m_iRemainServiceHrs=iRemainServiceHrs;
			m_byStateBytes=byStateBytes;
			m_byExtendedStateBytes=byExtendedStateBytes;
			m_bPumpStateOn=bPumpStateOn;
			m_iMaxOpHRSCalibration=iMaxOpHRSCalibration;
			Draw();
		}
	}

	CWnd::OnTimer(nIDEvent);
}

// **************************************************************************
// 
// **************************************************************************
LRESULT CSubViewCO2::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	//int iID = 0;
	switch(message)
	{
	case WM_OPEN_SETTING:
		{
			if(!m_bCO2disabled)
				OpenSettingsView();
			return 1;
		}
		break;
	case WM_MENUBUTTONDOWN:
		{
			SetOneButtonDepressed(wParam);
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
void CSubViewCO2::OnBnClickedCO2module()
{
	SetButtonClicked(IDC_BTN_SETUP_CO2MODULE);
}

void CSubViewCO2::OnBnClickedCO2units()
{
	SetButtonClicked(IDC_BTN_SETUP_CO2UNITS);
}
void CSubViewCO2::OnBnClickedBarometricPressure()
{
	if(m_byCO2Module==CO2MODULE_CAPNOSTAT)
	{
		SetButtonClicked(IDC_BTN_SETUP_BAROPRESSURE);
	}
	
}
void CSubViewCO2::OnBnClickedAutoOnPump()
{
	SetButtonClicked(IDC_BTN_SETUP_AUTOONPUMP);
}
void CSubViewCO2::OnBnClickedETCO2avg()
{
	SetButtonClicked(IDC_BTN_SETUP_ETCO2AVG);
}
void CSubViewCO2::OnBnClickedZeroCal()
{
	m_iCurPara=IDC_BTN_SETUP_ZEROCAL;
	//CDlgMessageBox box(this,_T("CO2 sensor calibration"),_T("This will start the zero calibration of the CO2 sensor. Continue?"),MB_YESNO,IDB_MSG_CONFIG);
	CDlgMessageBox box(this,getModel()->GetLanguageString(IDS_MSG_CAPTION_CO2CAL),getModel()->GetLanguageString(IDS_MSG_CO2CAL),MB_YESNO,IDB_MSG_CONFIG);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCLOSE)
	{
		return;
	}

	if(iRes==IDYES && getModel()->getETCO2()!=NULL)
	{
		getModel()->getETCO2()->zeroCalibration();
	}
	
	this->SetFocus();

	SetOneButtonFocused(IDC_BTN_SETUP_ZEROCAL);

	if(getModel()->getETCO2()!=NULL)
		m_byStateBytes=getModel()->getETCO2()->getStateBytes();
	UpdateWindow();
	Draw();
}
// **************************************************************************
// 
// **************************************************************************
void CSubViewCO2::SetButtonClicked(int btnID)
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
void CSubViewCO2::SetOneButtonDepressed(int btnID)
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
				getModel()->getCONFIG()->SetLastSelectedSVCO2Btns(m_iCurPara);


			}
		}
	}
	catch (...)
	{
		theApp.ReportErrorException(_T("CSubViewCO2::SetOneButtonDepressed"));
	}

	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewCO2::SetOneButtonFocused(int btnID)
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
				getModel()->getCONFIG()->SetLastSelectedSVCO2Btns(m_iCurPara);
			}
		}
	}
	catch (...)
	{
		theApp.ReportErrorException(_T("CSubViewCO2::SetOneButtonFocused"));
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
void CSubViewCO2::OpenSettingsView()
{
	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);

	if(GetParent())
		GetParent()->PostMessage(WM_OPEN_SETTING,m_iCurPara);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewCO2::SetNextButtonFocused()
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
				getModel()->getCONFIG()->SetLastSelectedSVCO2Btns(m_iCurPara);
				break;
			}
			else
			{
				pBtn = m_plBtn.GetNext( pos );
				pBtn->SetState(BS_FOCUSED);
				m_iCurPara=pBtn->GetBtnId();
				getModel()->getCONFIG()->SetLastSelectedSVCO2Btns(m_iCurPara);
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
void CSubViewCO2::SetPrevButtonFocused()
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
				getModel()->getCONFIG()->SetLastSelectedSVCO2Btns(m_iCurPara);

				break;
			}
			else
			{
				pBtn = m_plBtn.GetPrev( pos );
				pBtn->SetState(BS_FOCUSED);
				m_iCurPara=pBtn->GetBtnId();
				getModel()->getCONFIG()->SetLastSelectedSVCO2Btns(m_iCurPara);

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
eBtnState CSubViewCO2::GetCurrentBtnState()
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
BOOL CSubViewCO2::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				if(m_iCurPara==IDC_BTN_SETUP_ZEROCAL)
				{
					OnBnClickedZeroCal();
					return 1;
				}
				else if(GetCurrentBtnState()==BS_FOCUSED)
				{
					SetOneButtonDepressed(m_iCurPara);
				}
				else if(GetCurrentBtnState()==BS_DOWN)
				{
					SetOneButtonFocused(m_iCurPara);
				}
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				if(GetCurrentBtnState()==BS_FOCUSED)
					SetPrevButtonFocused();
			}
			else if(pMsg->wParam==VK_UP)
			{
				if(GetCurrentBtnState()==BS_FOCUSED)
					SetNextButtonFocused();
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
				if(m_iCurPara==IDC_BTN_SETUP_ZEROCAL)
				{
					return 1;
				}
				else if(GetCurrentBtnState()==BS_DOWN)
				{
					PostMessage(WM_OPEN_SETTING);
				}
				//return 1;
			}
			//else if(pMsg->wParam==VK_DOWN)
			//{
			//	return 1;
			//}
			//else if(pMsg->wParam==VK_UP)
			//{
			//	//return 1;
			//}
			//else
			//{
			//	//handle focus change manual, draw next focused button and return true
			//	//return 1;
			//}
			break;
		}
		break;
	default:
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}
