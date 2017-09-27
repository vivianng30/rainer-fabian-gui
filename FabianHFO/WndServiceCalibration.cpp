// WndServiceCalibration.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndServiceCalibration.h"
#include "MessageBoxProxCal.h"
#include "DlgMessageBox.h"
#include "DlgProxPressure60.h"
#include <math.h>


// CWndServiceCalibration

IMPLEMENT_DYNAMIC(CWndServiceCalibration, CWnd)

CWndServiceCalibration::CWndServiceCalibration():
CWndService()
{
	m_pcFlowCorr_Up=NULL;
	m_pcFlowCorr_Dw=NULL;
	m_pcFlowCorrPED=NULL;
	m_pcFlowCorrNEO=NULL;
	m_pcAltitude=NULL;

	m_sliderEnableBTPS=NULL;

	m_iFCOR_PED=0;
	m_iFCOR_NEO=0;
	m_iAltitude=0;
	m_iCntCheckVentState=0;

	m_bUseBTPS=getModel()->getCONFIG()->getBTPS();

	m_bUpdateData=true;

	m_pDlg = NULL;

	m_pcDis=NULL;
	m_pcUp=NULL;
	m_pcDw=NULL;
	m_pcSysCal=NULL;
	m_pcPProx0=NULL;
	m_pcPProx60=NULL;
	m_pcExhCal=NULL;

	m_bProx0mbarCalibrated=false;
	
	m_pcWait=NULL;
	m_pcWait1= NULL;
	m_pcWait2= NULL;
	m_pcWait3= NULL;
	m_pcWait4= NULL;
	m_pcWait5= NULL;
	m_pcWait6= NULL;
	m_pcWait7= NULL;
	m_pcWait8= NULL;

	m_iADC0_I_Data=0;
	m_iADC0_I_Offs=0;
	m_iADC0_I_Scl=0;
	m_iADC0_I_Dac=0;
	m_iADC1_E_Data=0;
	m_iADC1_E_Offs=0;
	m_iADC1_E_Scl=0;
	m_iADC1_E_Dac=0;

	m_iPressure=0;
	m_iPProxADC=0;
	m_iPProxMonSCL=getModel()->getSPI()->Read_CAL_PRESS_SCALE();
	m_iPProxMonOFFS=getModel()->getSPI()->Read_CAL_PRESS_OFFSET();

	m_iPProxConSCL=getModel()->getSERIAL()->GetM_CAL_PRESS_SCALE();
	m_iPProxConOFFS=getModel()->getSERIAL()->GetM_CAL_PRESS_OFFSET();
	
	m_szLast0mbarCal=_T("");
	m_szLast60mbarCal=_T("");

	m_iWaitCount=0;
	m_bSysCalRunning=false;
	m_csSysCal=_T("");
	m_csProxCal=_T("");

	m_bPProx0CalRunning=false;
	m_bPProx60CalRunning=false;
}

CWndServiceCalibration::~CWndServiceCalibration()
{
	if(m_sliderEnableBTPS)
		delete m_sliderEnableBTPS;
	m_sliderEnableBTPS=NULL;

	delete  m_pcFlowCorr_Up;
	delete  m_pcFlowCorr_Dw;
	delete  m_pcFlowCorrNEO;
	delete  m_pcFlowCorrPED;
	delete  m_pcAltitude;


	m_pcFlowCorr_Up=NULL;
	m_pcFlowCorr_Dw=NULL;
	m_pcFlowCorrNEO=NULL;
	m_pcFlowCorrPED=NULL;
	m_pcAltitude=NULL;
	
	delete m_pcDis;
	m_pcDis=NULL;
	delete m_pcUp;
	m_pcUp=NULL;
	delete m_pcDw;
	m_pcDw=NULL;
	delete m_pcSysCal;
	m_pcSysCal=NULL;
	delete m_pcPProx0;
	m_pcPProx0=NULL;
	delete m_pcPProx60;
	m_pcPProx60=NULL;
	delete m_pcExhCal;
	m_pcExhCal=NULL;

	
	delete m_pcWait1;
	m_pcWait1=NULL;
	delete m_pcWait2;
	m_pcWait2=NULL;
	delete m_pcWait3;
	m_pcWait3=NULL;
	delete m_pcWait4;
	m_pcWait4=NULL;
	delete m_pcWait5;
	m_pcWait5=NULL;
	delete m_pcWait6;
	m_pcWait6=NULL;
	delete m_pcWait7;
	m_pcWait7=NULL;
	delete m_pcWait8;
	m_pcWait8=NULL;
	delete m_pcWait;
	m_pcWait=NULL;
}


BEGIN_MESSAGE_MAP(CWndServiceCalibration, CWnd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
	ON_BN_CLICKED(IDC_BTN_SERVICE_SYSCAL, &CWndServiceCalibration::OnBnClickedSysCal)
	ON_BN_CLICKED(IDC_BTN_SERVICE_PROX0, &CWndServiceCalibration::OnBnClickedProx0)
	ON_BN_CLICKED(IDC_BTN_SERVICE_PROX60, &CWndServiceCalibration::OnBnClickedProx60)
	ON_BN_CLICKED(IDC_BTN_SERVICE_EXHCAL, &CWndServiceCalibration::OnBnClickedExhCal)
END_MESSAGE_MAP()



// CWndServiceCalibration message handlers
void CWndServiceCalibration::Init()
{
	CClientDC dc(this);

	m_pcFlowCorr_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_FG_UP);
	m_pcFlowCorr_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_FG_DW);

	//m_iFCOR_NEO=getModel()->getSPI()->Read_FLOW_CORFACTOR_NEO();
	//m_iFCOR_PED=getModel()->getSPI()->Read_FLOW_CORFACTOR_PED();
	m_iFCOR_NEO=getModel()->getI2C()->ReadConfigWord(FLOW_CORFACTOR_NEO_16);
	m_iFCOR_PED=getModel()->getI2C()->ReadConfigWord(FLOW_CORFACTOR_PED_16);
	m_iAltitude=getModel()->getI2C()->ReadConfigWord(ALTITUDE_16);

	if(m_iFCOR_NEO<500 || m_iFCOR_NEO>1500)
		m_iFCOR_NEO=960;

	if(m_iFCOR_PED<500 || m_iFCOR_PED>1500)
		m_iFCOR_PED=960;

	m_pcUp= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_UP);
	m_pcDw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_DW);
	m_pcDis= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_DIS);

	m_pcWait=NULL;
	m_pcWait1= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY1);
	m_pcWait2= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY2);
	m_pcWait3= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY3);
	m_pcWait4= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY4);
	m_pcWait5= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY5);
	m_pcWait6= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY6);
	m_pcWait7= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY7);
	m_pcWait8= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY8);

	BTN btn;
	btn.wID					= IDC_BTN_SERVICE_SYSCAL;	
	btn.poPosition.x		= 20;
	btn.poPosition.y		= 50;
	btn.pcBmpUp				= m_pcUp;
	btn.pcBmpDown			= m_pcDw;
	btn.pcBmpFocus			= m_pcUp;
	btn.pcBmpDisabled		= m_pcDis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcSysCal=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcSysCal->Create(this,g_hf10AcuBold,0);
	m_pcSysCal->SetText(_T("Calibrate"));


	btn.wID					= IDC_BTN_SERVICE_PROX0;	
	btn.poPosition.x		= 20;
	btn.poPosition.y		= 175;
	btn.pcBmpUp				= m_pcUp;
	btn.pcBmpDown			= m_pcDw;
	btn.pcBmpFocus			= m_pcUp;
	btn.pcBmpDisabled		= m_pcDis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcPProx0=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcPProx0->Create(this,g_hf10AcuBold,0);
	m_pcPProx0->SetText(_T("0 mbar Cal."));
	m_pcPProx0->EnableWindow(TRUE);


	btn.wID					= IDC_BTN_SERVICE_PROX60;	
	btn.poPosition.x		= 20;
	btn.poPosition.y		= 230;
	btn.pcBmpUp				= m_pcUp;
	btn.pcBmpDown			= m_pcDw;
	btn.pcBmpFocus			= m_pcUp;
	btn.pcBmpDisabled		= m_pcDis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcPProx60=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcPProx60->Create(this,g_hf10AcuBold,0);
	m_pcPProx60->SetText(_T("60 mbar Cal."));
	m_pcPProx60->EnableWindow(FALSE);


	btn.wID					= IDC_BTN_SERVICE_EXHCAL;	
	btn.poPosition.x		= 20;
	btn.poPosition.y		= 445;
	btn.pcBmpUp				= m_pcUp;
	btn.pcBmpDown			= m_pcDw;
	btn.pcBmpFocus			= m_pcUp;
	btn.pcBmpDisabled		= m_pcDis;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcExhCal=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcExhCal->Create(this,g_hf10AcuBold,0);
	m_pcExhCal->SetText(_T("Calibrate"));

	if(getModel()->GetVentRunState()==VENT_RUNNING)
	{
		m_pcExhCal->EnableWindow(TRUE);
	}
	else
	{
		m_pcExhCal->EnableWindow(FALSE);
	}

	VALUE v;

	v.nLowerLimit=500;
	v.nUpperLimit=1500;

	v.nValue=m_iFCOR_NEO;

	btn.wID					= IDC_BTN_SERVICE_FLOWCORR_NEO;	
	btn.poPosition.x		= 650;//50;
	btn.poPosition.y		= 35;//300;
	btn.pcBmpUp				= m_pcFlowCorr_Up;
	btn.pcBmpDown			= m_pcFlowCorr_Dw;
	btn.pcBmpFocus			= m_pcFlowCorr_Up;
	btn.pcBmpDisabled		= m_pcFlowCorr_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcFlowCorrNEO=new CUDBtn(btn,COLOR_TXTBTNUP,false);
	m_pcFlowCorrNEO->Create(this,g_hf7AcuNorm,g_hf27AcuBold,v);
	m_pcFlowCorrNEO->SetText(_T("Flow Corr NEO"),false);

	v.nLowerLimit=500;
	v.nUpperLimit=1500;

	v.nValue=m_iFCOR_PED;

	btn.wID					= IDC_BTN_SERVICE_FLOWCORR_PED;	
	btn.poPosition.x		= 650;//50;
	btn.poPosition.y		= 130;//300;
	btn.pcBmpUp				= m_pcFlowCorr_Up;
	btn.pcBmpDown			= m_pcFlowCorr_Dw;
	btn.pcBmpFocus			= m_pcFlowCorr_Up;
	btn.pcBmpDisabled		= m_pcFlowCorr_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcFlowCorrPED=new CUDBtn(btn,COLOR_TXTBTNUP,false);
	m_pcFlowCorrPED->Create(this,g_hf7AcuNorm,g_hf27AcuBold,v);
	m_pcFlowCorrPED->SetText(_T("Flow Corr PED"),false);

	v.nLowerLimit=0;
	v.nUpperLimit=50;

	v.nValue=m_iAltitude/100;

	btn.wID					= IDC_BTN_SERVICE_ALTITUDE;	
	btn.poPosition.x		= 650;//50;
	btn.poPosition.y		= 225;//300;
	btn.pcBmpUp				= m_pcFlowCorr_Up;
	btn.pcBmpDown			= m_pcFlowCorr_Dw;
	btn.pcBmpFocus			= m_pcFlowCorr_Up;
	btn.pcBmpDisabled		= m_pcFlowCorr_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcAltitude=new CUDBtn(btn,COLOR_TXTBTNUP,false);
	m_pcAltitude->Create(this,g_hf7AcuNorm,g_hf27AcuBold,v);
	m_pcAltitude->SetText(_T("Altitude [m]"),false);

	m_pcMenuBack->ShowWindow(SW_SHOW);

	CStringW szTimeLastCal0mbar = getModel()->getCONFIG()->GetLastCal0mbar();
	CStringW szTimeLastCal60mbar = getModel()->getCONFIG()->GetLastCal60mbar();
	COleDateTime dtTimeLastCal0mbar;
	COleDateTime dtTimeLastCal60mbar;
	
	if(szTimeLastCal0mbar == _T(""))
	{
		m_szLast0mbarCal=_T("- unknown -");
	}
	else
	{
		if(false==dtTimeLastCal0mbar.ParseDateTime(szTimeLastCal0mbar,LOCALE_NOUSEROVERRIDE))
		{
			m_szLast0mbarCal=_T("- error -");
		}
		else
		{
			m_szLast0mbarCal.Format(_T("%02d.%02d.%04d %02d:%02d:%02d"),
				dtTimeLastCal0mbar.GetDay(),
				dtTimeLastCal0mbar.GetMonth(),
				dtTimeLastCal0mbar.GetYear(),
				dtTimeLastCal0mbar.GetHour(),
				dtTimeLastCal0mbar.GetMinute(),
				dtTimeLastCal0mbar.GetSecond());
		}
	}

	if(szTimeLastCal60mbar == _T(""))
	{
		m_szLast60mbarCal=_T("- unknown -");
	}
	else
	{
		if(false==dtTimeLastCal60mbar.ParseDateTime(szTimeLastCal60mbar,LOCALE_NOUSEROVERRIDE))
		{
			m_szLast60mbarCal=_T("- error -");
		}
		else
		{
			m_szLast60mbarCal.Format(_T("%02d.%02d.%04d %02d:%02d:%02d"),
				dtTimeLastCal60mbar.GetDay(),
				dtTimeLastCal60mbar.GetMonth(),
				dtTimeLastCal60mbar.GetYear(),
				dtTimeLastCal60mbar.GetHour(),
				dtTimeLastCal60mbar.GetMinute(),
				dtTimeLastCal60mbar.GetSecond());
		}
	}
	

	m_sliderEnableBTPS = new CBitmapSlider();
	m_sliderEnableBTPS->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(620,355,769,400), 
		this,IDC_SLD_BTPS);
	m_sliderEnableBTPS->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
	m_sliderEnableBTPS->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
	m_sliderEnableBTPS->SetRange( 0, 1 );
	if(m_bUseBTPS)
	{
		m_sliderEnableBTPS->SetPos( 1 );
	}
	else
	{
		m_sliderEnableBTPS->SetPos( 0 );
	}
	m_sliderEnableBTPS->SetMargin( 5, 0, 6, 0 );
	m_sliderEnableBTPS->DrawFocusRect( FALSE );

	SetTimer(SERVICETIMER, 150, NULL);

}

void CWndServiceCalibration::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	CBrush cbrWhite(RGB(255,255,255));
	CBrush cbrBack(RGB(181,178,181));
	CBrush cbrDark(RGB(140,140,140));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf8AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	
	//************************************************************//
	rc.left = 10;  
	rc.top = 15;  
	rc.right  = 570;  
	rc.bottom = 40;
	FillRect(hdcMem,&rc,cbrDark);

	rc.left = 20;  
	rc.top = 20;  
	rc.right  = 280;  
	rc.bottom = 500;
	CStringW cs = _T("System Calibration:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//**************************DATA,OFFS,SCL**********************************//
	rc.left = 200;  
	rc.top = 53;  
	rc.right  = 570;  
	rc.bottom = 72;
	FillRect(hdcMem,&rc,cbrWhite);


	rc.left = 300;  
	rc.top = 55;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs = _T("ADC");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 370;  
	cs = _T("OFFS");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 440;  
	cs = _T("SCL");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 510;  
	cs = _T("DAC");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************ADC0/I**********************************//
	rc.left = 210;  
	rc.top = 85;  
	cs = _T("I-Flow");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 300;  
	cs.Format(_T("%d"), m_iADC0_I_Data);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 370;  
	cs.Format(_T("%d"), m_iADC0_I_Offs);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 440;  
	cs.Format(_T("%d"), m_iADC0_I_Scl);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 510;  
	cs.Format(_T("%d"), m_iADC0_I_Dac);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//**************************ADC1/E**********************************//
	rc.left = 210;  
	rc.top = 105;  
	cs = _T("E-Flow");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 300;  
	cs.Format(_T("%d"), m_iADC1_E_Data);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 370;  
	cs.Format(_T("%d"), m_iADC1_E_Offs);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 440;  
	cs.Format(_T("%d"), m_iADC1_E_Scl);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 510;  
	cs.Format(_T("%d"), m_iADC1_E_Dac);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	if(m_bSysCalRunning)
	{
		m_csSysCal=_T("");

		//hourglass 
		if(m_iWaitCount>8)
			m_iWaitCount=1;

		switch(m_iWaitCount)
		{
		case 1:
			{
				m_pcWait=m_pcWait1;
			}
			break;
		case 2:
			{
				m_pcWait=m_pcWait2;
			}
			break;
		case 3:
			{
				m_pcWait=m_pcWait3;
			}
			break;
		case 4:
			{
				m_pcWait=m_pcWait4;
			}
			break;
		case 5:
			{
				m_pcWait=m_pcWait5;
			}
			break;
		case 6:
			{
				m_pcWait=m_pcWait6;
			}
			break;
		case 7:
			{
				m_pcWait=m_pcWait7;
			}
			break;
		case 8:
			{
				m_pcWait=m_pcWait8;
			}
			break;
		default:
			{
				m_pcWait=NULL;
			}
			break;
		}
	}
	
	
	

	rc.left = 90;  
	rc.top = 100;  
	rc.right  = 125;  
	rc.bottom = 135;
	FillRect(hdcMem,&rc,cbrBack);

	if(m_pcWait)
		m_pcWait->Draw(hdcMem,90,100);
	else
	{
		rc.left = 20;  
		rc.top = 110;  
		rc.right  = 180;  
		rc.bottom = 500;
		//cs = _T("<state>");
		DrawText(hdcMem,m_csSysCal,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
	}


	



	//************************************************************//
	rc.left = 10;  
	rc.top = 140;  
	rc.right  = 570;  
	rc.bottom = 165;
	FillRect(hdcMem,&rc,cbrDark);

	rc.left = 20;  
	rc.top = 145;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Proximal Pressure Calibration:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	

	rc.left = 210;
	rc.right  = 580;
	rc.top = 190;  
	cs = _T("Last 0 mbar calibration:  ");
	cs+= m_szLast0mbarCal;
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.top = 210;  
	cs = _T("Last 60 mbar calibration:  ");
	cs+= m_szLast60mbarCal;
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);




	//rc.left = 10;  
	rc.top = 240;  
	//rc.right  = 190;  
	rc.bottom = 500;
	DrawText(hdcMem,m_csProxCal,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);




	rc.left = 460;  
	rc.top = 315;  
	rc.right  = 520;  
	rc.bottom = 355;
	FillRect(hdcMem,&rc,cbrWhite);

	rc.left = 360;
	rc.right  = 600;
	rc.top = 317;  
	cs = _T("Pressure:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.right  = 510;
	cs.Format(_T("%0.1f"), CTlsFloat::Round(((double)m_iPressure)/10, 1));
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);

	rc.left = 530; 
	rc.right  = 600;
	cs = _T("mbar");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	
	
	
	rc.left = 360;  
	rc.top = 337;  
	cs = _T("PPROX ADC:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.right  = 510;
	cs.Format(_T("%d"), m_iPProxADC);
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);

	rc.left = 530; 
	rc.right  = 600;
	cs = _T("ADC");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	


	//tabelle Scale/Offset monitor/steuer pic
	//**************************OFFS,SCL**********************************//
	rc.left = 30;  
	rc.top = 285;  
	rc.right  = 300;  
	rc.bottom = 304;
	FillRect(hdcMem,&rc,cbrWhite);


	rc.top = 287;
	rc.left = 170;  
	cs = _T("OFFS");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 240;  
	cs = _T("SCL");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	


	rc.left = 40;  
	rc.top = 317;  
	rc.right  = 600;  
	rc.bottom = 500;
	cs = _T("MonitorPIC");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 170;  
	cs.Format(_T("%d"), m_iPProxMonOFFS);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 240;  
	cs.Format(_T("%d"), m_iPProxMonSCL);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 40;
	rc.top = 337;  
	cs = _T("ConductorPIC");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 170; 
	cs.Format(_T("%d"), m_iPProxConOFFS);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 240;  
	cs.Format(_T("%d"), m_iPProxConSCL);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//************************************************************//
	rc.left = 10;  
	rc.top = 400;  
	rc.right  = 570;  
	rc.bottom = 425;
	FillRect(hdcMem,&rc,cbrDark);

	rc.left = 20;  
	rc.top = 405;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Exhalation Valve Calibration:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 210;
	rc.right  = 570;
	rc.top = 465; 
	if(getModel()->GetVentRunState()==VENT_RUNNING)
		cs = _T("Before you start calibration connect test lung !");
	else
		cs = _T("ventilation is not running - calibration not possible !");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//******************BTPS******************************************//
	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	SelectObject(hdcMem,cbrRound);

	RoundRect(hdcMem, 608, 320, 778, 410,20,20);

	rc.left = 620;
	rc.right  = 770;
	rc.top = 330; 
	rc.bottom = 350;
	cs = _T("enable O2 comp.");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);


	//dc.BitBlt(0,0,m_lX,m_lY,CDC::FromHandle(hdcMem),0,0,SRCCOPY);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	penLine.DeleteObject();
	//DeleteObject(cbrWhite);
	//DeleteObject(cbrBack);
	//DeleteObject(cbrDark);

	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}


//************************************
// Method:    OnDestroy
// FullName:  CWndServiceCalibration::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndServiceCalibration::OnDestroy()
{
	KillTimer(SERVICETIMER);

	if(m_pDlg)
	{
		m_pDlg->EndDialog(IDCLOSE);
	}
	m_pDlg = NULL;

	if(m_bSysCalRunning)
	{
		getModel()->getSPI()->Send_KOMMANDO(0);
	}

	CWndService::OnDestroy();
}

// **************************************************************************
// 
// **************************************************************************
void CWndServiceCalibration::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==SERVICETIMER)
	{
		if(m_iWaitCount>0)
		{
			m_iWaitCount++;

			if(m_bUpdateData)
			{
				m_iPressure=getModel()->getDATAHANDLER()->getMessureDataPRESSURE();
				m_iPProxADC=getModel()->getSPI()->Read_P_PROX_ADC();

				m_iADC0_I_Data=getModel()->getSPI()->Read_FLOW_INSP_ADC();
				m_iADC0_I_Offs=getModel()->getSPI()->Read_SYST_OFFSET_FLOW_INSP();
				m_iADC0_I_Scl=getModel()->getSPI()->Read_SYST_SCALE_FLOW_INSP();
				m_iADC0_I_Dac=getModel()->getSPI()->Read_DAC_FLOW_INSP();

				m_iADC1_E_Data=getModel()->getSPI()->Read_FLOW_EXH_ADC();
				m_iADC1_E_Offs=getModel()->getSPI()->Read_SYST_OFFSET_FLOW_EXH();
				m_iADC1_E_Scl=getModel()->getSPI()->Read_SYST_SCALE_FLOW_EXH();
				m_iADC1_E_Dac=getModel()->getSPI()->Read_DAC_FLOW_EXH();

				int iCommand=getModel()->getSPI()->Read_FLOWSENS_CMND();

				if(0==iCommand)
				{
					m_iWaitCount=0;
					m_bSysCalRunning=false;
					m_pcWait=NULL;

					int iState=getModel()->getSPI()->Read_STATUS_FLOWSENSOR();
					if(iState & BIT7)
					{
						m_csSysCal=_T("- failed -");
					}
					else
					{
						m_csSysCal=_T("- calibrated -");
					}

					m_pcPProx0->EnableWindow(TRUE);
					if(m_bProx0mbarCalibrated)
						m_pcPProx60->EnableWindow(TRUE);
					else
						m_pcPProx60->EnableWindow(FALSE);
					m_pcExhCal->EnableWindow(TRUE);
				}
				else if(iCommand==-1)
				{
					m_iWaitCount=0;
					m_bSysCalRunning=false;
					m_pcWait=NULL;

					m_csSysCal=_T("- SPI-ERROR -");

					m_pcPProx0->EnableWindow(TRUE);
					if(m_bProx0mbarCalibrated)
						m_pcPProx60->EnableWindow(TRUE);
					else
						m_pcPProx60->EnableWindow(FALSE);
					m_pcExhCal->EnableWindow(TRUE);
				}
				else if(iCommand!=CMD_SYSCALIBRATION)
				{
					m_iWaitCount=0;
					m_bSysCalRunning=false;
					m_pcWait=NULL;

					m_csSysCal=_T("- SPI-ERROR -");

					m_pcPProx0->EnableWindow(TRUE);
					if(m_bProx0mbarCalibrated)
						m_pcPProx60->EnableWindow(TRUE);
					else
						m_pcPProx60->EnableWindow(FALSE);
					m_pcExhCal->EnableWindow(TRUE);
				}
			}

			Draw();
		}
		else
		{
			if(m_bUpdateData)
			{
				m_iPressure=getModel()->getDATAHANDLER()->getMessureDataPRESSURE();
				m_iPProxADC=getModel()->getSPI()->Read_P_PROX_ADC();

				m_iADC0_I_Data=getModel()->getSPI()->Read_FLOW_INSP_ADC();
				m_iADC0_I_Offs=getModel()->getSPI()->Read_SYST_OFFSET_FLOW_INSP();
				m_iADC0_I_Scl=getModel()->getSPI()->Read_SYST_SCALE_FLOW_INSP();
				m_iADC0_I_Dac=getModel()->getSPI()->Read_DAC_FLOW_INSP();

				m_iADC1_E_Data=getModel()->getSPI()->Read_FLOW_EXH_ADC();
				m_iADC1_E_Offs=getModel()->getSPI()->Read_SYST_OFFSET_FLOW_EXH();
				m_iADC1_E_Scl=getModel()->getSPI()->Read_SYST_SCALE_FLOW_EXH();
				m_iADC1_E_Dac=getModel()->getSPI()->Read_DAC_FLOW_EXH();

				Draw();
			}
		}

		m_bUpdateData=!m_bUpdateData;

		checkVentRunState();
	}

	CWnd::OnTimer(nIDEvent);
}

void CWndServiceCalibration::checkVentRunState()
{
	m_iCntCheckVentState++;

	if(m_iCntCheckVentState>=8)
	{
		m_iCntCheckVentState=0;
		if(getModel()->GetVentRunState()==VENT_RUNNING)
		{
			m_pcExhCal->EnableWindow(TRUE);
		}
		else
		{
			m_pcExhCal->EnableWindow(FALSE);
		}
	}
	
}


// **************************************************************************
// 
// **************************************************************************
void CWndServiceCalibration::OnBnClickedSysCal()
{
	m_bSysCalRunning=!m_bSysCalRunning;
	//SPI 35 / 36

	int iCommand=0;

	if(m_bSysCalRunning)
	{
		m_iWaitCount=1;
		iCommand=CMD_SYSCALIBRATION;

		if(m_pcPProx0)
			m_pcPProx0->EnableWindow(FALSE);
		if(m_pcPProx60)
			m_pcPProx60->EnableWindow(FALSE);
		if(m_pcExhCal)
			m_pcExhCal->EnableWindow(FALSE);

		theApp.getLog()->WriteLine(_T("*** Service mode CMD_SYSCALIBRATION ***"));
	}
	else
	{
		m_pcWait=NULL;
		m_iWaitCount=0;
		m_csSysCal=_T("- Cal. canceled -");

		if(m_pcPProx0)
			m_pcPProx0->EnableWindow(TRUE);
		if(m_bProx0mbarCalibrated)
		{
			if(m_pcPProx60)
				m_pcPProx60->EnableWindow(TRUE);
		}
		else
		{
			if(m_pcPProx60)
				m_pcPProx60->EnableWindow(FALSE);
		}
		if(m_pcExhCal)
			m_pcExhCal->EnableWindow(TRUE);
	}

	getModel()->getSPI()->Send_FLOWSENS_CMND(iCommand);

	Draw();
}


// **************************************************************************
// 
// **************************************************************************
void CWndServiceCalibration::OnBnClickedProx0()
{
	m_bPProx0CalRunning=true;

	if(m_pcExhCal)
		m_pcExhCal->EnableWindow(FALSE);
	if(m_pcPProx0)
		m_pcPProx0->EnableWindow(FALSE);
	if(m_pcPProx60)
		m_pcPProx60->EnableWindow(FALSE);
	if(m_pcSysCal)
		m_pcSysCal->EnableWindow(FALSE);

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);

	CMessageBoxProxCal box(this,_T("Proxymal Pressure Calibration"),_T("This will start the 0 mbar calibration. You have to disconnect the tube for pressure messurement first."),MB_CAL_0MBAR);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes==IDCANCEL)
	{
		if(m_pcExhCal)
			m_pcExhCal->EnableWindow(TRUE);
		if(m_pcPProx0)
			m_pcPProx0->EnableWindow(TRUE);
		if(m_pcPProx60)
			m_pcPProx60->EnableWindow(FALSE);
		if(m_pcSysCal)
			m_pcSysCal->EnableWindow(TRUE);
		m_csProxCal=_T("- canceled -");
		return;
	}
	else if(iRes==IDCLOSE)
	{
		return;
	}

	getModel()->getSPI()->Send_KOMMANDO(CMD_PROX0CALIBRATION);
	getModel()->getSERIAL()->Send_CAL_PRESS_PROX(0);

	int iSPIstate=getModel()->getSPI()->Read_KOMMANDO();

	bool bTimeout=false;
	BYTE iTimeout=0;

	while(bTimeout==false && iSPIstate == CMD_PROX0CALIBRATION)
	{
		if(iTimeout>100)
			bTimeout=true;
		Sleep(10);
		iSPIstate=getModel()->getSPI()->Read_KOMMANDO();
		iTimeout++;
	}

	if(bTimeout)
	{
		m_csProxCal=_T("- timeout 0mbar -");

		m_iPProxMonSCL=getModel()->getSPI()->Read_CAL_PRESS_SCALE();
		m_iPProxMonOFFS=getModel()->getSPI()->Read_CAL_PRESS_OFFSET();

		m_iPProxConSCL=getModel()->getSERIAL()->GetM_CAL_PRESS_SCALE();
		m_iPProxConOFFS=getModel()->getSERIAL()->GetM_CAL_PRESS_OFFSET();

		if(m_pcExhCal)
			m_pcExhCal->EnableWindow(TRUE);
		if(m_pcPProx0)
			m_pcPProx0->EnableWindow(TRUE);
		if(m_pcPProx60)
			m_pcPProx60->EnableWindow(FALSE);
		if(m_pcSysCal)
			m_pcSysCal->EnableWindow(TRUE);

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_DOUBLESIGNAL);

		return;
	}

	Sleep(100);

	bool bSERIALstate=getModel()->getSERIAL()->GetM_StatusByte();
	int iSERIALstate=-1;
	if(bSERIALstate)
	{
		iSERIALstate=getModel()->getDATAHANDLER()->GetMSTATUS_ConductorPIC();
		BOOL bPROXCAL0BIT=iSERIALstate & PROXCAL0BIT;
		if(bPROXCAL0BIT)
		{
			m_csProxCal=_T("- ConPIC 0mbar failed -");

			m_iPProxMonSCL=getModel()->getSPI()->Read_CAL_PRESS_SCALE();
			m_iPProxMonOFFS=getModel()->getSPI()->Read_CAL_PRESS_OFFSET();

			m_iPProxConSCL=getModel()->getSERIAL()->GetM_CAL_PRESS_SCALE();
			m_iPProxConOFFS=getModel()->getSERIAL()->GetM_CAL_PRESS_OFFSET();

			if(m_pcExhCal)
				m_pcExhCal->EnableWindow(TRUE);
			if(m_pcPProx0)
				m_pcPProx0->EnableWindow(TRUE);
			if(m_pcPProx60)
				m_pcPProx60->EnableWindow(FALSE);
			if(m_pcSysCal)
				m_pcSysCal->EnableWindow(TRUE);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_DOUBLESIGNAL);

			return;
		}
	}
	else
	{
		m_csProxCal=_T("- ConPIC failed -");

		m_iPProxMonSCL=getModel()->getSPI()->Read_CAL_PRESS_SCALE();
		m_iPProxMonOFFS=getModel()->getSPI()->Read_CAL_PRESS_OFFSET();

		m_iPProxConSCL=getModel()->getSERIAL()->GetM_CAL_PRESS_SCALE();
		m_iPProxConOFFS=getModel()->getSERIAL()->GetM_CAL_PRESS_OFFSET();

		if(m_pcExhCal)
			m_pcExhCal->EnableWindow(TRUE);
		if(m_pcPProx0)
			m_pcPProx0->EnableWindow(TRUE);
		if(m_pcPProx60)
			m_pcPProx60->EnableWindow(FALSE);
		if(m_pcSysCal)
			m_pcSysCal->EnableWindow(TRUE);

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_DOUBLESIGNAL);

		return;
	}

	BYTE byState=getModel()->getSPI()->Read_STATUS_PROXSENSOR();

	if(byState & BIT0)	// 0 mbar Kalibr. fehlgeschlagen
	{
		m_csProxCal=_T("- MonPIC 0mbar failed -");

		m_iPProxMonSCL=getModel()->getSPI()->Read_CAL_PRESS_SCALE();
		m_iPProxMonOFFS=getModel()->getSPI()->Read_CAL_PRESS_OFFSET();

		m_iPProxConSCL=getModel()->getSERIAL()->GetM_CAL_PRESS_SCALE();
		m_iPProxConOFFS=getModel()->getSERIAL()->GetM_CAL_PRESS_OFFSET();

		if(m_pcExhCal)
			m_pcExhCal->EnableWindow(TRUE);
		if(m_pcPProx0)
			m_pcPProx0->EnableWindow(TRUE);
		if(m_pcPProx60)
			m_pcPProx60->EnableWindow(FALSE);
		if(m_pcSysCal)
			m_pcSysCal->EnableWindow(TRUE);

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_DOUBLESIGNAL);

		return;
	}
	

	theApp.getLog()->WriteLine(_T("*** Service mode 0mbar calibrated ***"));
	m_csProxCal=_T("- 0mbar calibrated -");

	m_iPProxMonSCL=getModel()->getSPI()->Read_CAL_PRESS_SCALE();
	m_iPProxMonOFFS=getModel()->getSPI()->Read_CAL_PRESS_OFFSET();

	m_iPProxConSCL=getModel()->getSERIAL()->GetM_CAL_PRESS_SCALE();
	m_iPProxConOFFS=getModel()->getSERIAL()->GetM_CAL_PRESS_OFFSET();

	SYSTEMTIME st;
	GetLocalTime(&st);
	COleDateTime dtTimeLastCal0mbar(st);

	CStringW szTimeLastCal0mbar = dtTimeLastCal0mbar.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);

	getModel()->getCONFIG()->SetLastCal0mbar(szTimeLastCal0mbar);

	m_szLast0mbarCal=szTimeLastCal0mbar;

	Sleep(500);

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);

	if(m_pcPProx60)
		m_pcPProx60->EnableWindow(TRUE);

	if(m_pcExhCal)
		m_pcExhCal->EnableWindow(TRUE);
	if(m_pcPProx0)
		m_pcPProx0->EnableWindow(TRUE);
	if(m_pcSysCal)
		m_pcSysCal->EnableWindow(TRUE);
}
void CWndServiceCalibration::OnBnClickedProx60()
{
	m_bPProx0CalRunning=true;

	m_pcExhCal->EnableWindow(FALSE);
	m_pcPProx0->EnableWindow(FALSE);
	m_pcPProx60->EnableWindow(FALSE);
	m_pcSysCal->EnableWindow(FALSE);

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);

	CDlgMessageBox box(this,_T("Proxymal Pressure Calibration 60mbar"),_T("This will start the 60mbar calibration. You have to disconnect the patient first, PIP will be set to 60mbar! Continue?"),MB_YESNO,IDB_MSG_STOP);
	m_pDlg = &box;
	int iRes=box.DoModal();
	m_pDlg = NULL;

	if(iRes!=IDYES)
	{
		m_csProxCal=_T("- canceled -");
		m_pcExhCal->EnableWindow(TRUE);
		m_pcPProx0->EnableWindow(TRUE);
		m_pcPProx60->EnableWindow(TRUE);
		m_pcSysCal->EnableWindow(TRUE);
		return;
	}

	getModel()->getDATAHANDLER()->SetPINSPParadata_IPPV(600,true);
	getModel()->getDATAHANDLER()->enableProcPressureCal60();
	CDlgProxPressure60 box2(this);
	m_pDlg = &box2;
	iRes=box2.DoModal();
	m_pDlg = NULL;


	if(iRes==IDCANCEL)
	{
		getModel()->getDATAHANDLER()->disableProcPressureCal60();
		m_csProxCal=_T("- canceled -");
		m_pcExhCal->EnableWindow(TRUE);
		m_pcPProx0->EnableWindow(TRUE);
		m_pcPProx60->EnableWindow(TRUE);
		m_pcSysCal->EnableWindow(TRUE);
		getModel()->getDATAHANDLER()->SetPINSPParadata_IPPV(600,true);
		return;
	}
	else if(iRes==IDCLOSE)
	{
		getModel()->getDATAHANDLER()->disableProcPressureCal60();
		m_pcExhCal->EnableWindow(TRUE);
		m_pcPProx0->EnableWindow(TRUE);
		m_pcPProx60->EnableWindow(TRUE);
		m_pcSysCal->EnableWindow(TRUE);
		getModel()->getDATAHANDLER()->SetPINSPParadata_IPPV(600,true);
		return;
	}
	
	
	getModel()->getSPI()->Send_KOMMANDO(CMD_PROX60CALIBRATION);
	getModel()->getSERIAL()->Send_CAL_PRESS_PROX(600);

	int iSPIstate=getModel()->getSPI()->Read_KOMMANDO();
	bool bTimeout=false;
	BYTE iTimeout=0;

	while(bTimeout==false && iSPIstate == CMD_PROX60CALIBRATION)
	{
		if(iTimeout>100)
			bTimeout=true;
		Sleep(10);
		iSPIstate=getModel()->getSPI()->Read_KOMMANDO();
		iTimeout++;
	}

	if(bTimeout)
	{
		getModel()->getDATAHANDLER()->disableProcPressureCal60();
		m_csProxCal=_T("- timeout 60mbar -");

		m_iPProxMonSCL=getModel()->getSPI()->Read_CAL_PRESS_SCALE();
		m_iPProxMonOFFS=getModel()->getSPI()->Read_CAL_PRESS_OFFSET();

		m_iPProxConSCL=getModel()->getSERIAL()->GetM_CAL_PRESS_SCALE();
		m_iPProxConOFFS=getModel()->getSERIAL()->GetM_CAL_PRESS_OFFSET();

		m_pcExhCal->EnableWindow(TRUE);
		m_pcPProx0->EnableWindow(TRUE);
		m_pcPProx60->EnableWindow(TRUE);
		m_pcSysCal->EnableWindow(TRUE);

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_DOUBLESIGNAL);

		getModel()->getDATAHANDLER()->SetPINSPParadata_IPPV(600,true);
		return;
	}

	Sleep(100);

	bool bSERIALstate=getModel()->getSERIAL()->GetM_StatusByte();
	int iSERIALstate=-1;
	if(bSERIALstate)
	{
		iSERIALstate=getModel()->getDATAHANDLER()->GetMSTATUS_ConductorPIC();
		BOOL bPROXCAL60BIT=iSERIALstate & PROXCAL60BIT;
		if(bPROXCAL60BIT)
		{
			getModel()->getDATAHANDLER()->disableProcPressureCal60();
			m_csProxCal=_T("- ConPIC 60mbar failed -");

			m_iPProxMonSCL=getModel()->getSPI()->Read_CAL_PRESS_SCALE();
			m_iPProxMonOFFS=getModel()->getSPI()->Read_CAL_PRESS_OFFSET();

			m_iPProxConSCL=getModel()->getSERIAL()->GetM_CAL_PRESS_SCALE();
			m_iPProxConOFFS=getModel()->getSERIAL()->GetM_CAL_PRESS_OFFSET();

			m_pcExhCal->EnableWindow(TRUE);
			m_pcPProx0->EnableWindow(TRUE);
			m_pcPProx60->EnableWindow(TRUE);
			m_pcSysCal->EnableWindow(TRUE);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_DOUBLESIGNAL);

			getModel()->getDATAHANDLER()->SetPINSPParadata_IPPV(600,true);
			return;
		}

	}
	else
	{
		getModel()->getDATAHANDLER()->disableProcPressureCal60();
		m_csProxCal=_T("- ConPIC failed -");

		m_iPProxMonSCL=getModel()->getSPI()->Read_CAL_PRESS_SCALE();
		m_iPProxMonOFFS=getModel()->getSPI()->Read_CAL_PRESS_OFFSET();

		m_iPProxConSCL=getModel()->getSERIAL()->GetM_CAL_PRESS_SCALE();
		m_iPProxConOFFS=getModel()->getSERIAL()->GetM_CAL_PRESS_OFFSET();

		m_pcExhCal->EnableWindow(TRUE);
		m_pcPProx0->EnableWindow(TRUE);
		m_pcPProx60->EnableWindow(TRUE);
		m_pcSysCal->EnableWindow(TRUE);

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_DOUBLESIGNAL);

		getModel()->getDATAHANDLER()->SetPINSPParadata_IPPV(600,true);
		return;
	}


	BYTE byState=getModel()->getSPI()->Read_STATUS_PROXSENSOR();

	if(byState & BIT1)	// 60 mbar Kalibr. fehlgeschlagen
	{
		getModel()->getDATAHANDLER()->disableProcPressureCal60();
		m_csProxCal=_T("- MonPIC 60mbar failed -");

		m_iPProxMonSCL=getModel()->getSPI()->Read_CAL_PRESS_SCALE();
		m_iPProxMonOFFS=getModel()->getSPI()->Read_CAL_PRESS_OFFSET();

		m_iPProxConSCL=getModel()->getSERIAL()->GetM_CAL_PRESS_SCALE();
		m_iPProxConOFFS=getModel()->getSERIAL()->GetM_CAL_PRESS_OFFSET();

		m_pcExhCal->EnableWindow(TRUE);
		m_pcPProx0->EnableWindow(TRUE);
		m_pcPProx60->EnableWindow(TRUE);
		m_pcSysCal->EnableWindow(TRUE);

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_DOUBLESIGNAL);

		getModel()->getDATAHANDLER()->SetPINSPParadata_IPPV(600,true);
		return;
	}

	theApp.getLog()->WriteLine(_T("*** Service mode 60mbar calibrated ***"));
	m_csProxCal=_T("- 60mbar calibrated -");

	m_iPProxMonSCL=getModel()->getSPI()->Read_CAL_PRESS_SCALE();
	m_iPProxMonOFFS=getModel()->getSPI()->Read_CAL_PRESS_OFFSET();

	m_iPProxConSCL=getModel()->getSERIAL()->GetM_CAL_PRESS_SCALE();
	m_iPProxConOFFS=getModel()->getSERIAL()->GetM_CAL_PRESS_OFFSET();

	

	SYSTEMTIME st2;
	GetLocalTime(&st2);
	COleDateTime dtTimeLastCal60mbar(st2);

	CStringW szTimeLastCal60mbar = dtTimeLastCal60mbar.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);

	getModel()->getCONFIG()->SetLastCal60mbar(szTimeLastCal60mbar);

	m_szLast60mbarCal=szTimeLastCal60mbar;

	m_pcExhCal->EnableWindow(TRUE);
	m_pcPProx0->EnableWindow(TRUE);
	m_pcPProx60->EnableWindow(TRUE);
	m_pcSysCal->EnableWindow(TRUE);
	getModel()->getDATAHANDLER()->disableProcPressureCal60();

	getModel()->getDATAHANDLER()->SetPINSPParadata_IPPV(600,true);
	
}
// **************************************************************************
// 
// **************************************************************************
void CWndServiceCalibration::OnBnClickedExhCal()
{
	bool bPRICOrunning=false;
	if(getModel()->getPRICOThread())
	{
		if(getModel()->getPRICOThread()->isPRICOalgorithmRunning())
		{
			bPRICOrunning=true;
		}
	}

	if(bPRICOrunning)
	{
		CDlgMessageBox box(this,_T("Exhalation valve calibration not possible"),_T("PRICO algorithm is running, must be turned off before!"),MB_OK,IDB_MSG_STOP);
		m_pDlg = &box;
		int iRes=box.DoModal();
		m_pDlg = NULL;
	}
	else
	{
		CDlgMessageBox box(this,_T("Exhalation valve calibration"),_T("The actual mode will be changed to IPPV with default parameters. Please close Y-Piece at ET tube connector. Continue?"),MB_YESNO,IDB_MSG_STOP);
		m_pDlg = &box;
		int iRes=box.DoModal();
		m_pDlg = NULL;

		if(iRes==IDCLOSE)
		{
			return;
		}

		if(iRes==IDYES)
		{
			theApp.getLog()->WriteLine(_T("*** Service mode OnBnClickedExhCal() ***"));

			m_pcPProx0->EnableWindow(FALSE);
			m_pcPProx60->EnableWindow(FALSE);
			m_pcSysCal->EnableWindow(FALSE);

			getModel()->getDATAHANDLER()->SetExhalValvCalMode(true);
		
			getModel()->getDATAHANDLER()->SetCurrentO2Para(21);
			getModel()->getDATAHANDLER()->SetCurrentIFlowPara(10000);
			getModel()->getDATAHANDLER()->SetCurrentEFlowPara(5000);
			getModel()->getDATAHANDLER()->SetCurrentPEEPPara(50);
			getModel()->getDATAHANDLER()->SetCurrentITimePara(1000);
			getModel()->getDATAHANDLER()->SetCurrentBPMPara(30);
			getModel()->getDATAHANDLER()->SetCurrentETimePara(1000);
			getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_OFF);
			getModel()->getDATAHANDLER()->SetCurrentVLimitParam(getModel()->getDATAHANDLER()->GetCurrentVLimitPara(),false,true);
			getModel()->getDATAHANDLER()->SetCurrentPINSPPara(500);
		
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_CONTROL_SET_MODE_EXHALVALVCAL);
		}
	}
}


LRESULT CWndServiceCalibration::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_UPDATE_UDBTN:
		{
			switch(lParam)
			{
			case IDC_BTN_SERVICE_FLOWCORR_NEO:
				{
					int iVal=wParam;
					double iCorrFactor=CalcCorrectionFactor(m_iAltitude);
					getModel()->getI2C()->WriteConfigWord(FLOW_CORFACTOR_NEO_16,iVal);
					getModel()->getSPI()->Send_FLOW_CORFACTOR_NEO(iCorrFactor*iVal);
					return 1;
				}
				break;
			case IDC_BTN_SERVICE_FLOWCORR_PED:
				{
					int iVal=wParam;
					double iCorrFactor=CalcCorrectionFactor(m_iAltitude);
					getModel()->getI2C()->WriteConfigWord(FLOW_CORFACTOR_PED_16,iVal);
					getModel()->getSPI()->Send_FLOW_CORFACTOR_PED(iCorrFactor*iVal);
					return 1;
				}
				break;
			case IDC_BTN_SERVICE_ALTITUDE:
				{
					m_iAltitude=wParam*100;
					double iCorrFactor=CalcCorrectionFactor(m_iAltitude);
					getModel()->getI2C()->WriteConfigWord(ALTITUDE_16,m_iAltitude);
					CStringW szAlt=_T("");
					szAlt.Format(_T("### ALTITUDE %d"), m_iAltitude);
					theApp.getLog()->WriteLine(szAlt);
					getModel()->getSPI()->Send_FLOW_CORFACTOR_PED(iCorrFactor*m_iFCOR_PED);					
					getModel()->getSPI()->Send_FLOW_CORFACTOR_NEO(iCorrFactor*m_iFCOR_NEO);
					return 1;
				}
				break;
			}
		}
		break;
	}
	return CWndService::WindowProc(message, wParam, lParam);
}

LRESULT CWndServiceCalibration::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case IDC_SLD_BTPS:
		{
			if(lParam==1)
			{
				m_bUseBTPS=true;
			}
			else
			{
				m_bUseBTPS=false;
			}

			getModel()->getCONFIG()->setBTPS(m_bUseBTPS);

			Draw();
		}
		break;
	}
	return 1;
}

double CWndServiceCalibration::CalcCorrectionFactor(double iAltitude)
{
	double iCorrFactor=0;
	double iAmbientPressure=0;
	double iAmbientStandard=950;

	iAmbientPressure=(double)1013*pow(((double)1-(double)0.000022558*iAltitude),(double)5.2561); //do we need (doubles)?
	iCorrFactor=iAmbientStandard/iAmbientPressure;

	return iCorrFactor;
}
