//pro, created
// WndServiceMaintenance.cpp : implementation file
#include "stdafx.h"
#include "FabianHFO.h"
#include "WndServiceMaintenance.h"
#include "DlgMessageBox.h"
#include "TlsFile.h"
#include "NumInputDlg.h"

// CWndServiceMaintenance

#define TIMECHANGE	600

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000

#define COLOR_CALBTNUP				RGB(181,178,181)
#define COLOR_CALBTNDW				RGB(0,191,0)
#define COLOR_CALBTNFC				RGB(85,95,255)

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
extern HFONT g_hf70Bold;


IMPLEMENT_DYNAMIC(CWndServiceMaintenance, CWnd)

CWndServiceMaintenance::CWndServiceMaintenance():
CWndService()
{
	m_pDlg = NULL;

	m_bSetupAvailable=CTlsFile::Exists(_T("\\Hard Disk\\SETUP\\SetupFabian.exe"));

	m_pcAlarm_Up=NULL;
	m_pcAlarm_Dw=NULL;

	m_pcResetOpTime=NULL;
	m_pcResetAccuOpTime=NULL;
	m_pcResetHFOOpTime=NULL;
	m_pcInstaller=NULL;

	m_sliderPassword=NULL;
	m_bServicePswrdDisabled=false;

	m_sliderAlarm=NULL;
	m_bAlarmOn=false;


	//************************************************** 
	// from subviewdatetime
	m_iCounter=0;
	m_bBigSteps=false;
	m_eTimeChanger=TC_OFF;

	m_iCurrentField=0;

	m_pbtnOk=NULL;
	m_pbtnDay=NULL;
	m_pbtnMonth=NULL;
	m_pbtnYear=NULL;

	m_pcSmallUp_Up=NULL;
	m_pcSmallUp_Dw=NULL;
	m_pcSmallDw_Up=NULL;
	m_pcSmallDw_Dw=NULL;
	m_pcOkGreen_Up=NULL;
	m_pcOkGreen_Dw=NULL;
	m_pcOkDisabled=NULL;
	m_pcOkRed_Up=NULL;
	m_pcOkRed_Dw=NULL;

	m_pcStaticMonth_Up=NULL;
	m_pcStaticMonth_Dw=NULL;
	m_pcStaticYear_Up=NULL;
	m_pcStaticYear_Dw=NULL;
	m_pcStaticTime_Up=NULL;
	m_pcStaticTime_Dw=NULL;
	m_pcStaticMonth_Fc=NULL;
	m_pcStaticYear_Fc=NULL;
	m_pcStaticTime_Fc=NULL;

	m_pcDayNextUp=NULL;
	m_pcDayNextDwn=NULL;

	m_pcMonthNextUp=NULL;
	m_pcMonthNextDwn=NULL;

	m_pcYearNextUp=NULL;
	m_pcYearNextDwn=NULL;

	m_bDateValidChanged=false;
	m_bDateChanged=false;

	int iTest=m_lY;
	m_dwLastSetupTimer=0;

	m_iYear=0;
	m_iMonth=0;
	m_iDay=0;
}

CWndServiceMaintenance::~CWndServiceMaintenance()
{
	
}

BEGIN_MESSAGE_MAP(CWndServiceMaintenance, CWnd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_INSTALLER, &CWndServiceMaintenance::OnBnClickedInstaller)
	ON_BN_CLICKED(IDC_BTN_SERVICE_RESETOPTIME, &CWndServiceMaintenance::OnBnClickedResetOpTime)
	ON_BN_CLICKED(IDC_BTN_SERVICE_RESETOPTIMEBATTERY, &CWndServiceMaintenance::OnBnClickedResetOpTimeBattery)
	ON_BN_CLICKED(IDC_BTN_SERVICE_RESETOPTIMEHFO, &CWndServiceMaintenance::OnBnClickedResetOpTimeHFO)
	ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
	ON_BN_CLICKED(IDC_BTN_SETUP_DT_OK, &CWndServiceMaintenance::OnBnClickedSetDate)
	ON_BN_CLICKED(IDC_BTN_SETUP_MONTH, &CWndServiceMaintenance::OnBnClickedMonth)
	ON_BN_CLICKED(IDC_BTN_SETUP_YEAR, &CWndServiceMaintenance::OnBnClickedYear)
	ON_BN_CLICKED(IDC_BTN_SETUP_DAY, &CWndServiceMaintenance::OnBnClickedDay)
END_MESSAGE_MAP()


// CWndServiceMaintenance message handlers

void CWndServiceMaintenance::Init()
{
	CClientDC dc(this);

	m_pcAlarm_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_TODAY_UP);
	m_pcAlarm_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_TODAY_DW);


	BTN btn;

	btn.wID					= IDC_BTN_SERVICE_RESETOPTIME;	
	btn.poPosition.x		= 220;
	btn.poPosition.y		= 15;
	btn.pcBmpUp				= m_pcAlarm_Up;
	btn.pcBmpDown			= m_pcAlarm_Dw;
	btn.pcBmpFocus			= m_pcAlarm_Up;
	btn.pcBmpDisabled		= m_pcAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcResetOpTime=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcResetOpTime->Create(this,g_hf15AcuMed,0);
	m_pcResetOpTime->SetText(_T("device"));
	m_pcResetOpTime->ShowWindow(SW_SHOW);

	btn.wID					= IDC_BTN_SERVICE_RESETOPTIMEBATTERY;	
	btn.poPosition.x		= 340;
	btn.poPosition.y		= 15;
	btn.pcBmpUp				= m_pcAlarm_Up;
	btn.pcBmpDown			= m_pcAlarm_Dw;
	btn.pcBmpFocus			= m_pcAlarm_Up;
	btn.pcBmpDisabled		= m_pcAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcResetAccuOpTime=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcResetAccuOpTime->Create(this,g_hf15AcuMed,0);
	m_pcResetAccuOpTime->SetText(_T("battery"));
	m_pcResetAccuOpTime->ShowWindow(SW_SHOW);


	btn.wID					= IDC_BTN_SERVICE_RESETOPTIMEHFO;	
	btn.poPosition.x		= 460;
	btn.poPosition.y		= 15;
	btn.pcBmpUp				= m_pcAlarm_Up;
	btn.pcBmpDown			= m_pcAlarm_Dw;
	btn.pcBmpFocus			= m_pcAlarm_Up;
	btn.pcBmpDisabled		= m_pcAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcResetHFOOpTime=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcResetHFOOpTime->Create(this,g_hf15AcuMed,0);
	m_pcResetHFOOpTime->SetText(_T("hfo"));
	m_pcResetHFOOpTime->ShowWindow(SW_SHOW);

	btn.wID					= IDC_BTN_INSTALLER;	
	btn.poPosition.x		= 450;
	btn.poPosition.y		= 130;
	btn.pcBmpUp				= m_pcAlarm_Up;
	btn.pcBmpDown			= m_pcAlarm_Dw;
	btn.pcBmpFocus			= m_pcAlarm_Up;
	btn.pcBmpDisabled		= m_pcAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcInstaller=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcInstaller->Create(this,g_hf15AcuMed,0);
	m_pcInstaller->SetText(_T("start"));
	m_pcInstaller->ShowWindow(SW_SHOW);

	//Service-password slider
	m_bServicePswrdDisabled=getModel()->getCONFIG()->IsServicePasswordDisabled();

	m_sliderPassword = new CBitmapSlider();
	m_sliderPassword->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(220,80,369,120), 
		this,IDC_SLD_SERVICEPASSWRD);
	m_sliderPassword->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
	m_sliderPassword->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
	m_sliderPassword->SetRange( 0, 1 );
	if(m_bServicePswrdDisabled)
	{
		m_sliderPassword->SetPos( 0 );
	}
	else
	{
		m_sliderPassword->SetPos( 1 );
	}
	m_sliderPassword->SetMargin( 5, 0, 6, 0 );
	m_sliderPassword->DrawFocusRect( FALSE );	

	//Alarm-slider
	if(getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_SYSTEM_SILENT)
		m_bAlarmOn=false;
	else
		m_bAlarmOn=true;

	m_sliderAlarm = new CBitmapSlider();
	m_sliderAlarm->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(220,130,369,120), 
		this,IDC_SLD_SYSTEMALARM);
	m_sliderAlarm->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
	m_sliderAlarm->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
	m_sliderAlarm->SetRange( 0, 1 );
	if(m_bAlarmOn)
	{
		m_sliderAlarm->SetPos( 1 );
	}
	else
	{
		m_sliderAlarm->SetPos( 0 );
	}
	m_sliderAlarm->SetMargin( 5, 0, 6, 0 );
	m_sliderAlarm->DrawFocusRect( FALSE );


	// Service Date
	m_dtNextServiceDate = getModel()->getCONFIG()->getNextServiceDate();
	SYSTEMTIME st;
	GetLocalTime(&st);

	if(m_dtNextServiceDate.GetStatus() == COleDateTime::valid)
	{
		m_bServiceDateOn=true;
	}
	else
	{
		m_dtNextServiceDate=COleDateTime(st);
		m_bServiceDateOn=false;
	}

	m_iYear=m_dtNextServiceDate.GetYear();
	m_iMonth=m_dtNextServiceDate.GetMonth();
	m_iDay=m_dtNextServiceDate.GetDay();

// 	CStringW strMonDate;
// 	CStringW strYearDate;
// 	CStringW strDayDate;
// 
// 	strMonDate=_T(" ")+GetMonthString(m_dtNextServiceDate.GetMonth());
// 	strYearDate=m_dtNextServiceDate.Format(_T("%Y"));
// 	strDayDate.Format(_T("%.2d"),m_dtNextServiceDate.GetDay());
// 
// 	if(m_pbtnMonth)
// 		m_pbtnMonth->RefreshText(strMonDate);
// 
// 	if(m_pbtnYear)
// 		m_pbtnYear->RefreshText(strYearDate);
// 
// 	if(m_pbtnDay)
// 		m_pbtnDay->RefreshText(strDayDate);

	//Service-date slider
	m_sliderDate = new CBitmapSlider();
	m_sliderDate->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(220,190,369,120), 
		this,IDC_SLD_SERVICEDATE);
	m_sliderDate->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
	m_sliderDate->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
	m_sliderDate->SetRange( 0, 1 );
	if(m_bServiceDateOn)
	{
		m_sliderDate->SetPos( 1 );
	}
	else
	{
		m_sliderDate->SetPos( 0 );
	}
	m_sliderDate->SetMargin( 5, 0, 6, 0 );
	m_sliderDate->DrawFocusRect( FALSE );

	//service date buttons
	m_pcSmallUp_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP1_UPUP);
	m_pcSmallUp_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP1_UPDW);
	m_pcSmallDw_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP1_DWUP);
	m_pcSmallDw_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP1_DWDW);

	m_pcOkDisabled= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_OK_DIS);
	m_pcOkGreen_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_OKGREENUP);
	m_pcOkGreen_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_OKGREENDW);
	m_pcOkRed_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_OKREDUP);
	m_pcOkRed_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_OKREDDW);

	m_pcStaticMonth_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_MONTH_UP);
	m_pcStaticMonth_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_MONTH_DW);
	m_pcStaticMonth_Fc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_MONTH_FC);
	m_pcStaticYear_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_YEAR_UP);
	m_pcStaticYear_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VALUE_DW);
	m_pcStaticYear_Fc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VALUE_FC);
	m_pcStaticTime_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_TIME_UP);
	m_pcStaticTime_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_TIME_DW);
	m_pcStaticTime_Fc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_TIME_FC);


	CStringW cs=_T("");

	//CUpDwnBtn* m_pbtnYear;
	btn.wID					= IDC_BTN_SETUP_YEAR;	
	btn.poPosition.x		= 490;
	btn.poPosition.y		= 250;
	btn.pcBmpUp				= m_pcStaticYear_Up;
	btn.pcBmpDown			= m_pcStaticYear_Dw;
	btn.pcBmpFocus			= m_pcStaticYear_Fc;
	btn.pcBmpDisabled		= m_pcStaticYear_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pbtnYear=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
	m_pbtnYear->Create(this,g_hf33AcuBold,0);
	cs = m_dtNextServiceDate.Format(_T("%Y"));
	m_pbtnYear->SetText(cs);
	m_plStaticBtn.AddTail(m_pbtnYear);

	//CUpDwnBtn* m_pbtnDay;
	btn.wID					= IDC_BTN_SETUP_DAY;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 250;
	btn.pcBmpUp				= m_pcStaticTime_Up;
	btn.pcBmpDown			= m_pcStaticTime_Dw;
	btn.pcBmpFocus			= m_pcStaticTime_Fc;
	btn.pcBmpDisabled		= m_pcStaticTime_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pbtnDay=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
	m_pbtnDay->Create(this,g_hf31AcuBold,0);
	cs = m_dtNextServiceDate.Format(_T("%d"));
	m_pbtnDay->SetText(cs);
	m_plStaticBtn.AddTail(m_pbtnDay);

	//CUpDwnBtn* m_pbtnMonth;
	btn.wID					= IDC_BTN_SETUP_MONTH;	
	btn.poPosition.x		= 235;
	btn.poPosition.y		= 250;
	btn.pcBmpUp				= m_pcStaticMonth_Up;
	btn.pcBmpDown			= m_pcStaticMonth_Dw;
	btn.pcBmpFocus			= m_pcStaticMonth_Fc;
	btn.pcBmpDisabled		= m_pcStaticMonth_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pbtnMonth=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
	m_pbtnMonth->Create(this,g_hf27AcuBold,0);
	cs = GetMonthString(m_dtNextServiceDate.GetMonth());
	m_pbtnMonth->SetText(cs);
	m_plStaticBtn.AddTail(m_pbtnMonth);


	btn.wID					= IDC_BTN_SETUP_DT_OK;	
	btn.poPosition.x		= 650;
	btn.poPosition.y		= 320;
	btn.pcBmpUp				= m_pcOkGreen_Up;
	btn.pcBmpDown			= m_pcOkGreen_Dw;
	btn.pcBmpFocus			= m_pcOkGreen_Up;
	btn.pcBmpDisabled		= m_pcOkGreen_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pbtnOk=new CBtn(btn,COLOR_TXTBTNUP);
	m_pbtnOk->Create(this,g_hf21AcuBold,0);
	m_pbtnOk->SetText(_T(""));


	//*******************Days*****************************
	btn.wID					= IDC_BTN_SETUPDAY_NEXTUP;	
	btn.poPosition.x		= 40;
	btn.poPosition.y		= 239;
	btn.pcBmpUp				= m_pcSmallUp_Up;
	btn.pcBmpDown			= m_pcSmallUp_Dw;
	btn.pcBmpFocus			= m_pcSmallUp_Up;
	btn.pcBmpDisabled		= m_pcSmallUp_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcDayNextUp=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
	m_pcDayNextUp->Create(this,g_hf21AcuBold,0);
	m_pcDayNextUp->SetText(_T(""));

	btn.wID					= IDC_BTN_SETUPDAY_NEXTDW;	
	btn.poPosition.x		= 40;
	btn.poPosition.y		= 281;
	btn.pcBmpUp				= m_pcSmallDw_Up;
	btn.pcBmpDown			= m_pcSmallDw_Dw;
	btn.pcBmpFocus			= m_pcSmallDw_Up;
	btn.pcBmpDisabled		= m_pcSmallDw_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcDayNextDwn=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
	m_pcDayNextDwn->Create(this,g_hf21AcuBold,0);
	m_pcDayNextDwn->SetText(_T(""));


	//*******************Month*****************************
	btn.wID					= IDC_BTN_SETUPMONTH_NEXTUP;	
	btn.poPosition.x		= 175;
	btn.poPosition.y		= 239;
	btn.pcBmpUp				= m_pcSmallUp_Up;
	btn.pcBmpDown			= m_pcSmallUp_Dw;
	btn.pcBmpFocus			= m_pcSmallUp_Up;
	btn.pcBmpDisabled		= m_pcSmallUp_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcMonthNextUp=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
	m_pcMonthNextUp->Create(this,g_hf21AcuBold,0);
	m_pcMonthNextUp->SetText(_T(""));

	btn.wID					= IDC_BTN_SETUPMONTH_NEXTDW;	
	btn.poPosition.x		= 175;
	btn.poPosition.y		= 281;
	btn.pcBmpUp				= m_pcSmallDw_Up;
	btn.pcBmpDown			= m_pcSmallDw_Dw;
	btn.pcBmpFocus			= m_pcSmallDw_Up;
	btn.pcBmpDisabled		= m_pcSmallDw_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcMonthNextDwn=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
	m_pcMonthNextDwn->Create(this,g_hf21AcuBold,0);
	m_pcMonthNextDwn->SetText(_T(""));

	//*******************Year*****************************
	btn.wID					= IDC_BTN_SETUPYEAR_NEXTUP;	
	btn.poPosition.x		= 430;
	btn.poPosition.y		= 239;
	btn.pcBmpUp				= m_pcSmallUp_Up;
	btn.pcBmpDown			= m_pcSmallUp_Dw;
	btn.pcBmpFocus			= m_pcSmallUp_Up;
	btn.pcBmpDisabled		= m_pcSmallUp_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcYearNextUp=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
	m_pcYearNextUp->Create(this,g_hf21AcuBold,0);
	m_pcYearNextUp->SetText(_T(""));

	btn.wID					= IDC_BTN_SETUPYEAR_NEXTDW;	
	btn.poPosition.x		= 430;
	btn.poPosition.y		= 281;
	btn.pcBmpUp				= m_pcSmallDw_Up;
	btn.pcBmpDown			= m_pcSmallDw_Dw;
	btn.pcBmpFocus			= m_pcSmallDw_Up;
	btn.pcBmpDisabled		= m_pcSmallDw_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcYearNextDwn=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
	m_pcYearNextDwn->Create(this,g_hf21AcuBold,0);
	m_pcYearNextDwn->SetText(_T(""));

	m_pcMenuBack->ShowWindow(SW_SHOW);

	SetTimer(SERVICETIMER, 1000, NULL);
	SetTimer(CHANGETIMER, 1000, NULL); //?
}


void CWndServiceMaintenance::Draw()
{
	RECT rc;
	
	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	CBrush cbrBack(RGB(181,178,181));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	

	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf9AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	CStringW cs = _T("");

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);


	//************************************************************//
	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	SelectObject(hdcMem,cbrRound);

	RoundRect(hdcMem, 10, 5, 770, 60,20,20);

	rc.left = 20;  
	rc.top = 15;  
	rc.right  = 280;  
	rc.bottom = 180;
	cs = _T("Set operating");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 20;  
	rc.top = 30;  
	rc.right  = 280;  
	rc.bottom = 180;
	cs = _T("hours:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	RoundRect(hdcMem, 10, 65, 770, 180,20,20);

	rc.left = 450;  
	rc.top = 90;  
	rc.right  = 580;  
	rc.bottom = 180;
	cs = _T("Fabian Installer:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	if(CTlsFile::Exists(_T("\\Hard Disk\\SETUP\\SetupFabian.exe")))
	{
		m_pcInstaller->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pcInstaller->ShowWindow(SW_HIDE);

		rc.left = 450;  
		rc.top = 140;  
		rc.right  = 600;  
		rc.bottom = 480; 
		cs=_T("not available");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}

	////**************************Password**********************************//
	rc.left = 20;  
	rc.top = 90;  
	rc.right  = 210;  
	rc.bottom = 180;
	cs = _T("Service-Password:");
	//cs = getModel()->GetLanguageString(IDS_TXT_KERNELVERS)+_T(":");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	////**************************System Alarm Status**********************************//	
	rc.left = 20;  
	rc.top = 140;  
	rc.right  = 280;  
	rc.bottom = 180;
	cs = _T("System Alarm:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//************************************************************ 
	// datetime
	if (m_bServiceDateOn)
	{
		RoundRect(hdcMem, 10, 185, 770, 410,20,20);

		m_pcDayNextUp->ShowWindow(SW_SHOW);
		m_pcDayNextDwn->ShowWindow(SW_SHOW);
		m_pcMonthNextUp->ShowWindow(SW_SHOW);
		m_pcMonthNextDwn->ShowWindow(SW_SHOW);		
		m_pcYearNextUp->ShowWindow(SW_SHOW);
		m_pcYearNextDwn->ShowWindow(SW_SHOW);
		m_pbtnDay->ShowWindow(SW_SHOW);
		m_pbtnMonth->ShowWindow(SW_SHOW);
		m_pbtnYear->ShowWindow(SW_SHOW);
		m_pbtnOk->ShowWindow(SW_SHOW);
	} 
	else
	{
		RoundRect(hdcMem, 10, 185, 770, 235,20,20);

		m_pcDayNextUp->ShowWindow(SW_HIDE);
		m_pcDayNextDwn->ShowWindow(SW_HIDE);
		m_pcMonthNextUp->ShowWindow(SW_HIDE);
		m_pcMonthNextDwn->ShowWindow(SW_HIDE);		
		m_pcYearNextUp->ShowWindow(SW_HIDE);
		m_pcYearNextDwn->ShowWindow(SW_HIDE);
		m_pbtnDay->ShowWindow(SW_HIDE);
		m_pbtnMonth->ShowWindow(SW_HIDE);
		m_pbtnYear->ShowWindow(SW_HIDE);
		m_pbtnOk->ShowWindow(SW_HIDE);
	}

	rc.left = 20;  
	rc.top = 200;  
	rc.right  = 280;  
	rc.bottom = 420;
	cs = _T("Next service date:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

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


void CWndServiceMaintenance::OnDestroy()
{
	KillTimer(SERVICETIMER);
	KillTimer(CHANGETIMER);

	m_plStaticBtn.RemoveAll();

	if(m_sliderPassword)
		delete m_sliderPassword;
	m_sliderPassword=NULL;

	if(m_pcAlarm_Up)
		delete  m_pcAlarm_Up;
	m_pcAlarm_Up=NULL;

	if(m_pcAlarm_Dw)
		delete  m_pcAlarm_Dw;
	m_pcAlarm_Dw=NULL;

	if(m_pcResetOpTime)
		delete m_pcResetOpTime;
	m_pcResetOpTime=NULL;

	if(m_pcResetAccuOpTime)
		delete m_pcResetAccuOpTime;
	m_pcResetAccuOpTime=NULL;

	if(m_pcResetHFOOpTime)
		delete m_pcResetHFOOpTime;
	m_pcResetHFOOpTime=NULL;

	if(m_pcInstaller)
		delete m_pcInstaller;
	m_pcInstaller=NULL;

	if(m_sliderAlarm)
		delete m_sliderAlarm;
	m_sliderAlarm=NULL;

	if(m_sliderDate)
		delete m_sliderDate;
	m_sliderDate=NULL;

	if(m_pbtnOk)
		delete m_pbtnOk;
	m_pbtnOk=NULL;

	if(m_pbtnMonth)
		delete m_pbtnMonth;
	m_pbtnMonth=NULL;
	
	if(m_pbtnYear)
		delete m_pbtnYear;
	m_pbtnYear=NULL;
	
	if(m_pbtnDay)
		delete m_pbtnDay;
	m_pbtnDay=NULL;

	if(m_pcDayNextUp)
		delete m_pcDayNextUp;
	m_pcDayNextUp=NULL;

	if(m_pcDayNextDwn)
		delete m_pcDayNextDwn;
	m_pcDayNextDwn=NULL;

	if(m_pcMonthNextUp)
		delete m_pcMonthNextUp;
	m_pcMonthNextUp=NULL;

	if(m_pcMonthNextDwn)
		delete m_pcMonthNextDwn;
	m_pcMonthNextDwn=NULL;

	if(m_pcYearNextUp)
		delete m_pcYearNextUp;
	m_pcYearNextUp=NULL;

	if(m_pcYearNextDwn)
		delete m_pcYearNextDwn;
	m_pcYearNextDwn=NULL;

	if(m_pcOkDisabled)
		delete m_pcOkDisabled;
	m_pcOkDisabled=NULL;

	if(m_pcOkGreen_Up)
		delete m_pcOkGreen_Up;
	m_pcOkGreen_Up=NULL;

	if(m_pcOkGreen_Dw)
		delete m_pcOkGreen_Dw;
	m_pcOkGreen_Dw=NULL;

	if(m_pcOkRed_Up)
		delete m_pcOkRed_Up;
	m_pcOkRed_Up=NULL;

	if(m_pcOkRed_Dw)
		delete m_pcOkRed_Dw;
	m_pcOkRed_Dw=NULL;

	if(m_pcStaticMonth_Up)
		delete m_pcStaticMonth_Up;
	m_pcStaticMonth_Up=NULL;

	if(m_pcStaticMonth_Dw)
		delete m_pcStaticMonth_Dw;
	m_pcStaticMonth_Dw=NULL;

	if(m_pcStaticYear_Up)
		delete m_pcStaticYear_Up;
	m_pcStaticYear_Up=NULL;

	if(m_pcStaticYear_Dw)
		delete m_pcStaticYear_Dw;
	m_pcStaticYear_Dw=NULL;

	if(m_pcStaticTime_Up)
		delete m_pcStaticTime_Up;
	m_pcStaticTime_Up=NULL;

	if(m_pcStaticTime_Dw)
		delete m_pcStaticTime_Dw;
	m_pcStaticTime_Dw=NULL;

	if(m_pcStaticMonth_Fc)
		delete m_pcStaticMonth_Fc;
	m_pcStaticMonth_Fc=NULL;

	if(m_pcStaticYear_Fc)
		delete m_pcStaticYear_Fc;
	m_pcStaticYear_Fc=NULL;

	if(m_pcStaticTime_Fc)
		delete m_pcStaticTime_Fc;
	m_pcStaticTime_Fc=NULL;

	if(m_pcSmallUp_Up)
		delete m_pcSmallUp_Up;
	m_pcSmallUp_Up=NULL;

	if(m_pcSmallUp_Dw)
		delete m_pcSmallUp_Dw;
	m_pcSmallUp_Dw=NULL;

	if(m_pcSmallDw_Up)
		delete m_pcSmallDw_Up;
	m_pcSmallDw_Up=NULL;

	if(m_pcSmallDw_Dw)
		delete m_pcSmallDw_Dw;
	m_pcSmallDw_Dw=NULL;	
		
	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);

	if(m_pDlg)
	{
		m_pDlg->EndDialog(IDCLOSE);
	}
	m_pDlg = NULL;

	CWndService::OnDestroy();
}

void CWndServiceMaintenance::OnBnClickedResetOpTime()
{
	UINT iNum=0;
	
	CNumInputDlg pwDlg(this,OPTIME_DEVICE,iNum);
	m_pDlg = &pwDlg;
	int iRes=pwDlg.DoModal();
	m_pDlg = NULL;

	if(iRes==IDOK)
	{
		getModel()->getDATAHANDLER()->setOpTimeDeviceMin(iNum);
	}

	UpdateWindow();
	Draw();
}

void CWndServiceMaintenance::OnBnClickedResetOpTimeBattery()
{
	UINT iNum=0;

	CNumInputDlg pwDlg(this,OPTIME_BATTERY,iNum);
	m_pDlg = &pwDlg;
	int iRes=pwDlg.DoModal();
	m_pDlg = NULL;


	if(iRes==IDOK)
	{
		getModel()->getDATAHANDLER()->setOpTimeBatteryMin(iNum);
	}

	UpdateWindow();
	Draw();
}
void CWndServiceMaintenance::OnBnClickedResetOpTimeHFO()
{
	UINT iNum=0;

	CNumInputDlg pwDlg(this,OPTIME_HFO,iNum);
	m_pDlg = &pwDlg;
	int iRes=pwDlg.DoModal();
	m_pDlg = NULL;


	if(iRes==IDOK)
	{
		getModel()->getDATAHANDLER()->setOpTimeHFOMin(iNum);
	}

	UpdateWindow();
	Draw();
}
void CWndServiceMaintenance::OnBnClickedInstaller()
{
	//if(CTlsFile::Exists(_T("\\Hard Disk\\FabianInstaller.exe")))
	//{
	//	CDlgMessageBox box(this,_T("UPDATE"),_T("This will start the Fabian Update, Ventilation will be stopped! Do not disconnect the USB-Setup-Stick! Continue?"),MB_YESNO,IDB_MSG_CONFIG);
	//	m_pDlg = &box;
	//	int iRes=box.DoModal();
	//	m_pDlg = NULL;

	//	if(iRes==IDCLOSE)
	//	{
	//		return;
	//	}

	//	if(iRes==IDYES)
	//	{
	//		//getModel()->StartInstaller();

	//		theApp.getLog()->WriteLine(_T("#HFO:0284"));
	//		
	//		CString file=_T("\\Hard Disk\\FabianInstaller.exe");
	//		PROCESS_INFORMATION pi;
	//		CreateProcess(file, 0, NULL, NULL, NULL, 0, NULL, NULL, NULL, &pi);

	//		???????????????
	//		CDlgMessageBox box(this,_T("UPDATE"),_T("Update ready to install, please reboot device manual!"),MB_OK,IDB_MSG_CONFIG);
	//		m_pDlg = &box;
	//		int iRes=box.DoModal();
	//		m_pDlg = NULL;
	//	}
	//}

	//old installer version
	if(CTlsFile::Exists(_T("\\Hard Disk\\SETUP\\SetupFabian.exe")))
	{
		CDlgMessageBox box(this,_T("SETUP"),_T("This will start the FabianSetup, Ventilation will be stopped! Do not disconnect the USB-Setup-Stick! It will take a minute until the setup starts, continue?"),MB_YESNO,IDB_MSG_CONFIG);
		m_pDlg = &box;
		int iRes=box.DoModal();
		m_pDlg = NULL;

		if(iRes==IDCLOSE)
		{
			return;
		}

		if(iRes==IDYES)
		{
			getModel()->StartInstaller();

			theApp.getLog()->WriteLine(_T("#HFO:0284"));
			getModel()->SetAccuTurnoff();
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENT_TURNOFF);
		}
	}
}

//=============================================================================
/**
 * @brief
 **/
//=============================================================================
void CWndServiceMaintenance::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==SERVICETIMER)
	{
		bool bSetupAvailable=false;
		if(CTlsFile::Exists(_T("\\Hard Disk\\SETUP\\SetupFabian.exe")))
		{
			bSetupAvailable=true;
		}

		if(m_bSetupAvailable!=bSetupAvailable)
		{
			Draw();
			m_bSetupAvailable=bSetupAvailable;
		}
		Draw();
	}
	else if(nIDEvent==CHANGETIMER && m_bServiceDateOn)
	{
		CStringW strDate;

		switch(m_eTimeChanger)
		{
		case TC_YEAR_UP:
			{
				if((m_iCounter>5) && (m_bBigSteps))
				{
					if(m_iYear<2490)
						m_iYear=m_iYear+10;
					else
						m_iYear=1970;

					strDate.Format(_T("%.4d"),m_iYear);
					if(m_pbtnYear)
						m_pbtnYear->RefreshText(strDate);

					DateTimeChanged();
				}
				else if((m_iCounter>5) && (m_iYear%10==0) && (m_bBigSteps==false))
				{
					m_bBigSteps=true;
					if(m_iYear<2490)
						m_iYear=m_iYear+10;
					else
						m_iYear=1970;

					strDate.Format(_T("%.4d"),m_iYear);
					if(m_pbtnYear)
						m_pbtnYear->RefreshText(strDate);

					DateTimeChanged();
				}
				else
				{
					if(m_iYear<2500)
						m_iYear++;
					else
						m_iYear=1967;

					strDate.Format(_T("%.4d"),m_iYear);
					if(m_pbtnYear)
						m_pbtnYear->RefreshText(strDate);

					DateTimeChanged();
				}
			}
			break;
		case TC_YEAR_DW:
			{
				if((m_iCounter>5) && (m_bBigSteps))
				{
					if(m_iYear>1980)
						m_iYear=m_iYear-10;
					else
						m_iYear=2500;

					strDate.Format(_T("%.4d"),m_iYear);
					if(m_pbtnYear)
						m_pbtnYear->RefreshText(strDate);

					DateTimeChanged();
				}
				else if((m_iCounter>5) && (m_iYear%10==0) && (m_bBigSteps==false))
				{
					m_bBigSteps=true;
					if(m_iYear>1980)
						m_iYear=m_iYear-10;
					else
						m_iYear=2500;

					strDate.Format(_T("%.4d"),m_iYear);
					if(m_pbtnYear)
						m_pbtnYear->RefreshText(strDate);

					DateTimeChanged();
				}
				else
				{
					if(m_iYear>1967)
						m_iYear--;
					else
						m_iYear=2500;

					strDate.Format(_T("%.2d"),m_iYear);
					if(m_pbtnYear)
						m_pbtnYear->RefreshText(strDate);

					DateTimeChanged();
				}
			}
			break;
		case TC_MONTH_UP:
			{
				if(m_iMonth<12)
					m_iMonth++;
				else
					m_iMonth=1;

				if(m_pbtnMonth)
					m_pbtnMonth->RefreshText(GetMonthString(m_iMonth));

				DateTimeChanged();
			}
			break;
		case TC_MONTH_DW:
			{
				if(m_iMonth>1)
					m_iMonth--;
				else
					m_iMonth=12;

				if(m_pbtnMonth)
					m_pbtnMonth->RefreshText(GetMonthString(m_iMonth));

				DateTimeChanged();
			}
			break;
		case TC_DAY_UP:
			{
				if(m_iMonth==2)
				{
					if(m_iDay<29)
						m_iDay++;
					else
						m_iDay=1;

					//check if valid
					//COleDateTime m_dtNextServiceDate;

					if(m_dtNextServiceDate.SetDate(m_iYear, m_iMonth, m_iDay)==0)
					{
					}
					else	//not valid
					{
						if(m_dtNextServiceDate.GetStatus() == COleDateTime::valid)
						{

						}
						else if(m_dtNextServiceDate.GetStatus() == COleDateTime::invalid)
						{
							m_iDay=1;
						}
						else if(m_dtNextServiceDate.GetStatus() == COleDateTime::null)
						{

						}
						else if(m_dtNextServiceDate.GetStatus() == COleDateTime::error )
						{

						}
					}
				}
				else if(m_iMonth==4 || m_iMonth==6 || m_iMonth==9 || m_iMonth==11)
				{
					if(m_iDay<30)
						m_iDay++;
					else
						m_iDay=1;
				}
				else
				{
					if(m_iDay<31)
						m_iDay++;
					else
						m_iDay=1;
				}



				strDate.Format(_T("%.2d"),m_iDay);
				if(m_pbtnDay)
					m_pbtnDay->RefreshText(strDate);

				DateTimeChanged();
			}
			break;
		case TC_DAY_DW:
			{
				if(m_iMonth==2)
				{
					if(m_iDay>1)
						m_iDay--;
					else
						m_iDay=29;

					//check if valid
					//COleDateTime m_dtNextServiceDate;

					if(m_dtNextServiceDate.SetDate(m_iYear, m_iMonth, m_iDay)==0)
					{
					}
					else	//not valid
					{
						if(m_dtNextServiceDate.GetStatus() == COleDateTime::valid)
						{

						}
						else if(m_dtNextServiceDate.GetStatus() == COleDateTime::invalid)
						{
							m_iDay=28;
						}
						else if(m_dtNextServiceDate.GetStatus() == COleDateTime::null)
						{

						}
						else if(m_dtNextServiceDate.GetStatus() == COleDateTime::error )
						{

						}
					}
				}
				else if(m_iMonth==4 || m_iMonth==6 || m_iMonth==9 || m_iMonth==11)
				{
					if(m_iDay>1)
						m_iDay--;
					else
						m_iDay=30;
				}
				else
				{
					if(m_iDay>1)
						m_iDay--;
					else
						m_iDay=31;
				}

				strDate.Format(_T("%.2d"),m_iDay);
				if(m_pbtnDay)
					m_pbtnDay->RefreshText(strDate);

				DateTimeChanged();
			}
			break;
		default:
			{

			}
			break;
		}

		m_iCounter++;
	}
	


	CWnd::OnTimer(nIDEvent);
}

LRESULT CWndServiceMaintenance::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case IDC_SLD_SERVICEPASSWRD:
		{
			if(lParam==1)
			{
				m_bServicePswrdDisabled=false;
			}
			else
			{
				m_bServicePswrdDisabled=true;
			}

			getModel()->getCONFIG()->SetServicePasswordDisabled(m_bServicePswrdDisabled);

			Draw();
		}
		break;
	case IDC_SLD_SYSTEMALARM:
		{
			if(lParam==0 && m_bAlarmOn)
			{
				m_bAlarmOn=false;
				theApp.getLog()->WriteLine(_T("#EVO:0199"));
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SYSALARM_SILENT);
			}
			if(lParam==1 && !m_bAlarmOn)
			{
				m_bAlarmOn=true;
				theApp.getLog()->WriteLine(_T("#EVO:0200"));
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SYSALARM_SILENT);
			}
			Draw();
		}
		break;	
	case IDC_SLD_SERVICEDATE:
		{
			if(lParam==0 && m_bServiceDateOn)
			{
				m_bServiceDateOn=false;				
				getModel()->getCONFIG()->setNextServiceDate(0,0,0);

				CStringW szServiceOff=_T("***Service Date turned off***");
				theApp.getLog()->WriteLine(szServiceOff);

				Draw();
			}
			if(lParam==1 && !m_bServiceDateOn)
			{
				m_bServiceDateOn=true;
				m_bDateChanged=false;

				CStringW strMonDate;
				CStringW strYearDate;
				CStringW strDayDate;

				SYSTEMTIME st;
				GetLocalTime(&st);
				m_dtNextServiceDate=COleDateTime(st);

				m_iYear=m_dtNextServiceDate.GetYear();
				m_iMonth=m_dtNextServiceDate.GetMonth();
				m_iDay=m_dtNextServiceDate.GetDay();

				strMonDate=_T(" ")+GetMonthString(m_dtNextServiceDate.GetMonth());
				strYearDate=m_dtNextServiceDate.Format(_T("%Y"));
				strDayDate.Format(_T("%.2d"),m_dtNextServiceDate.GetDay());

				if(m_pbtnMonth)
					m_pbtnMonth->RefreshText(strMonDate);

				if(m_pbtnYear)
					m_pbtnYear->RefreshText(strYearDate);

				if(m_pbtnDay)
					m_pbtnDay->RefreshText(strDayDate);

				if(m_pbtnOk)
					m_pbtnOk->SetBitmaps(m_pcOkDisabled,m_pcOkDisabled,m_pcOkDisabled,m_pcOkDisabled);

// 				theApp.getLog()->WriteLine(_T("#EVO:0200"));
// 				if(AfxGetApp())
// 					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SYSALARM_SILENT);
				Draw();
			}
		}
		break;
	}
	return 1;
}

CStringW CWndServiceMaintenance::GetMonthString(int iMonth)
{
	switch(iMonth)
	{
	case 1:
		return getModel()->GetLanguageString(IDS_TXT_JANUARY);
		break;
	case 2:
		return getModel()->GetLanguageString(IDS_TXT_FEBRUARY);
		break;
	case 3:
		return getModel()->GetLanguageString(IDS_TXT_MARCH);
		break;
	case 4:
		return getModel()->GetLanguageString(IDS_TXT_APRIL);
		break;
	case 5:
		return getModel()->GetLanguageString(IDS_TXT_MAY);
		break;
	case 6:
		return getModel()->GetLanguageString(IDS_TXT_JUNE);
		break;
	case 7:
		return getModel()->GetLanguageString(IDS_TXT_JULY);
		break;
	case 8:
		return getModel()->GetLanguageString(IDS_TXT_AUGUST);
		break;
	case 9:
		return getModel()->GetLanguageString(IDS_TXT_SEPTEMBER);
		break;
	case 10:
		return getModel()->GetLanguageString(IDS_TXT_OCTOBER);
		break;
	case 11:
		return getModel()->GetLanguageString(IDS_TXT_NOVEMBER);
		break;
	case 12:
		return getModel()->GetLanguageString(IDS_TXT_DECEMBER);
		break;
	default:
		return _T("");
		break;
	}
}

//=============================================================================
/**
 * @brief
 **/
//=============================================================================
void CWndServiceMaintenance::OnBnClickedDay()
{
	if (m_bServiceDateOn)
	{
		if(GetBtnState(IDC_BTN_SETUP_DAY)==BS_UP)
			SetOneButtonDepressed(IDC_BTN_SETUP_DAY);
		else if(GetBtnState(IDC_BTN_SETUP_DAY)==BS_FOCUSED)
			SetOneButtonDepressed(IDC_BTN_SETUP_DAY);
		else if(GetBtnState(IDC_BTN_SETUP_DAY)==BS_DOWN)
			SetOneButtonFocused(IDC_BTN_SETUP_DAY);
	}	
}

//=============================================================================
/**
 * @brief
 **/
//=============================================================================
void CWndServiceMaintenance::OnBnClickedMonth()
{
	if (m_bServiceDateOn)
	{
		if(GetBtnState(IDC_BTN_SETUP_MONTH)==BS_UP)
			SetOneButtonDepressed(IDC_BTN_SETUP_MONTH);
		else if(GetBtnState(IDC_BTN_SETUP_MONTH)==BS_FOCUSED)
			SetOneButtonDepressed(IDC_BTN_SETUP_MONTH);
		else if(GetBtnState(IDC_BTN_SETUP_MONTH)==BS_DOWN)
			SetOneButtonFocused(IDC_BTN_SETUP_MONTH);
	}
}
//=============================================================================
/**
 * @brief
 **/
//=============================================================================
void CWndServiceMaintenance::OnBnClickedYear()
{
	if (m_bServiceDateOn)
	{
		//SetOneButtonDepressed(IDC_BTN_SETUP_YEAR);
		if(GetBtnState(IDC_BTN_SETUP_YEAR)==BS_UP)
			SetOneButtonDepressed(IDC_BTN_SETUP_YEAR);
		else if(GetBtnState(IDC_BTN_SETUP_YEAR)==BS_FOCUSED)
			SetOneButtonDepressed(IDC_BTN_SETUP_YEAR);
		else if(GetBtnState(IDC_BTN_SETUP_YEAR)==BS_DOWN)
			SetOneButtonFocused(IDC_BTN_SETUP_YEAR);
	}
}


//=============================================================================
/**
 * @brief
 **/
//=============================================================================
LRESULT CWndServiceMaintenance::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
		switch(message)
		{
		case WM_MENUBUTTONDOWN:
			{
				switch(wParam)
				{
				case IDC_BTN_SERVICE_RESETOPTIME:
				case IDC_BTN_SERVICE_RESETOPTIMEBATTERY:
				case IDC_BTN_SERVICE_RESETOPTIMEHFO:
				case IDC_BTN_INSTALLER:
				//case IDC_BTN_SETUP_DT_OK: rku ?????????????????? - pro, ok, as we call it below where you put it, it´s the same
				case IDC_SLD_SERVICEPASSWRD:
				case IDC_SLD_SERVICEDATE:
				case IDC_SLD_SYSTEMALARM:
					{
						SetAllButtonUnpressed();
					}
					break;
				case IDC_BTN_MENU_NUMERIC:
					{
						if(GetParent())
							GetParent()->PostMessage(WM_EV_BN_SERVICE_BACK);
						return 1;
					}
					break;
				default:
					break;
				}
			}
			break;
		case WM_TIMEBTN_DOWN:
			{
				switch(wParam)
				{
				case IDC_BTN_SETUPYEAR_NEXTUP:
					{
						m_iCounter=0;
						m_bBigSteps=false;
						m_eTimeChanger=TC_YEAR_UP;
						SetTimer(CHANGETIMER,TIMECHANGE,NULL);
						SetOneButtonDepressed(IDC_BTN_SETUP_YEAR);
						return 1;
					}
					break;
				case IDC_BTN_SETUPYEAR_NEXTDW:
					{
						m_iCounter=0;
						m_bBigSteps=false;
						m_eTimeChanger=TC_YEAR_DW;
						SetTimer(CHANGETIMER,TIMECHANGE,NULL);
						SetOneButtonDepressed(IDC_BTN_SETUP_YEAR);
						return 1;
					}
					break;
				case IDC_BTN_SETUPMONTH_NEXTUP:
					{
						m_iCounter=0;
						m_bBigSteps=false;
						m_eTimeChanger=TC_MONTH_UP;
						SetTimer(CHANGETIMER,TIMECHANGE,NULL);
						SetOneButtonDepressed(IDC_BTN_SETUP_MONTH);
						return 1;
					}
					break;
				case IDC_BTN_SETUPMONTH_NEXTDW:
					{
						m_iCounter=0;
						m_bBigSteps=false;
						m_eTimeChanger=TC_MONTH_DW;
						SetTimer(CHANGETIMER,TIMECHANGE,NULL);
						SetOneButtonDepressed(IDC_BTN_SETUP_MONTH);
						return 1;
					}
					break;
				case IDC_BTN_SETUPDAY_NEXTUP:
					{
						m_iCounter=0;
						m_bBigSteps=false;
						m_eTimeChanger=TC_DAY_UP;
						SetTimer(CHANGETIMER,TIMECHANGE,NULL);
						SetOneButtonDepressed(IDC_BTN_SETUP_DAY);
						return 1;
					}
					break;
				case IDC_BTN_SETUPDAY_NEXTDW:
					{
						m_iCounter=0;
						m_bBigSteps=false;
						m_eTimeChanger=TC_DAY_DW;
						SetTimer(CHANGETIMER,TIMECHANGE,NULL);
						SetOneButtonDepressed(IDC_BTN_SETUP_DAY);
						return 1;
					}
					break;
				default:
					break;
				}
			}
			break;
		case WM_TIMEBTN_UP:
			{
				switch(wParam)
				{
				case IDC_BTN_SETUPYEAR_NEXTUP:
					{
						if(m_iCounter==0)
						{
							if(m_iYear<2500)
								m_iYear++;
							else
								m_iYear=1967;

							CStringW strDate;

							strDate.Format(_T("%.4d"),m_iYear);
							if(m_pbtnYear)
								m_pbtnYear->RefreshText(strDate);

							DateTimeChanged();

						}
						m_iCounter=0;
						m_bBigSteps=false;
						m_eTimeChanger=TC_OFF;
						KillTimer(CHANGETIMER);

						if(GetParent())
							GetParent()->PostMessage(WM_SET_SETUPTIMER);
						return 1;
					}
					break;
				case IDC_BTN_SETUPYEAR_NEXTDW:
					{
						if(m_iCounter==0)
						{
							if(m_iYear>1967)
								m_iYear--;
							else
								m_iYear=2500;

							CStringW strDate;

							strDate.Format(_T("%.4d"),m_iYear);
							if(m_pbtnYear)
								m_pbtnYear->RefreshText(strDate);

							DateTimeChanged();
						}
						m_iCounter=0;
						m_bBigSteps=false;
						m_eTimeChanger=TC_OFF;
						KillTimer(CHANGETIMER);

						if(GetParent())
							GetParent()->PostMessage(WM_SET_SETUPTIMER);
						return 1;
					}
					break;
				case IDC_BTN_SETUPMONTH_NEXTUP:
					{
						if(m_iCounter==0)
						{
							if(m_iMonth<12)
								m_iMonth++;
							else
								m_iMonth=1;

							if(m_pbtnMonth)
								m_pbtnMonth->RefreshText(GetMonthString(m_iMonth));

							DateTimeChanged();

						}
						m_iCounter=0;
						m_bBigSteps=false;
						m_eTimeChanger=TC_OFF;
						KillTimer(CHANGETIMER);

						if(GetParent())
							GetParent()->PostMessage(WM_SET_SETUPTIMER);
						return 1;
					}
					break;
				case IDC_BTN_SETUPMONTH_NEXTDW:
					{
						if(m_iCounter==0)
						{
							if(m_iMonth>1)
								m_iMonth--;
							else
								m_iMonth=12;

							if(m_pbtnMonth)
								m_pbtnMonth->RefreshText(GetMonthString(m_iMonth));

							DateTimeChanged();
						}
						m_iCounter=0;
						m_bBigSteps=false;
						m_eTimeChanger=TC_OFF;
						KillTimer(CHANGETIMER);

						if(GetParent())
							GetParent()->PostMessage(WM_SET_SETUPTIMER);
						return 1;
					}
					break;
				case IDC_BTN_SETUPDAY_NEXTUP:
					{
						if(m_iCounter==0)
						{
							if(m_iMonth==2)
							{
								if(m_iDay<29)
									m_iDay++;
								else
									m_iDay=1;

								//check if valid
								//COleDateTime m_dtNextServiceDate;

								if(m_dtNextServiceDate.SetDate(m_iYear, m_iMonth, m_iDay)==0)
								{


								}
								else	//not valid
								{
									if(m_dtNextServiceDate.GetStatus() == COleDateTime::valid)
									{

									}
									else if(m_dtNextServiceDate.GetStatus() == COleDateTime::invalid)
									{
										m_iDay=1;
									}
									else if(m_dtNextServiceDate.GetStatus() == COleDateTime::null)
									{

									}
									else if(m_dtNextServiceDate.GetStatus() == COleDateTime::error )
									{

									}
								}
							}
							else if(m_iMonth==4 || m_iMonth==6 || m_iMonth==9 || m_iMonth==11)
							{
								if(m_iDay<30)
									m_iDay++;
								else
									m_iDay=1;
							}
							else
							{
								if(m_iDay<31)
									m_iDay++;
								else
									m_iDay=1;
							}

							CStringW strDate;

							strDate.Format(_T("%.2d"),m_iDay);
							if(m_pbtnDay)
								m_pbtnDay->RefreshText(strDate);

							DateTimeChanged();

						}
						m_iCounter=0;
						m_bBigSteps=false;
						m_eTimeChanger=TC_OFF;
						KillTimer(CHANGETIMER);

						if(GetParent())
							GetParent()->PostMessage(WM_SET_SETUPTIMER);
						return 1;
					}
					break;
				case IDC_BTN_SETUPDAY_NEXTDW:
					{
						if(m_iCounter==0)
						{
							if(m_iMonth==2)
							{
								if(m_iDay>1)
									m_iDay--;
								else
									m_iDay=29;

								//check if valid
								//COleDateTime m_dtNextServiceDate;

								if(m_dtNextServiceDate.SetDate(m_iYear, m_iMonth, m_iDay)==0)
								{


								}
								else	//not valid
								{
									if(m_dtNextServiceDate.GetStatus() == COleDateTime::valid)
									{

									}
									else if(m_dtNextServiceDate.GetStatus() == COleDateTime::invalid)
									{
										m_iDay=28;
									}
									else if(m_dtNextServiceDate.GetStatus() == COleDateTime::null)
									{

									}
									else if(m_dtNextServiceDate.GetStatus() == COleDateTime::error )
									{

									}
								}
							}
							else if(m_iMonth==4 || m_iMonth==6 || m_iMonth==9 || m_iMonth==11)
							{
								if(m_iDay>1)
									m_iDay--;
								else
									m_iDay=30;
							}
							else
							{
								if(m_iDay>1)
									m_iDay--;
								else
									m_iDay=31;
							}


							CStringW strDate;

							strDate.Format(_T("%.2d"),m_iDay);
							if(m_pbtnDay)
								m_pbtnDay->RefreshText(strDate);

							DateTimeChanged();
						}
						m_iCounter=0;
						m_bBigSteps=false;
						m_eTimeChanger=TC_OFF;
						KillTimer(CHANGETIMER);

						if(GetParent())
							GetParent()->PostMessage(WM_SET_SETUPTIMER);
						return 1;
					}
					break;
				default:
					break;
				}

			}
			break;
		}
	return CWnd::WindowProc(message, wParam, lParam);
}

//=============================================================================
/**
 * @brief
 **/
//=============================================================================
void CWndServiceMaintenance::OnBnClickedSetDate()
{
	if(m_bDateChanged==false)
		return;

	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	COleDateTime dtCurrentTime(stTime);

	bool bInvalid=true;

	if(m_dtNextServiceDate.SetDate(m_iYear, m_iMonth, m_iDay)==0 && m_bServiceDateOn)
	{
		if (m_dtNextServiceDate > dtCurrentTime)
		{
			bInvalid=false;
			m_bDateChanged=false;
			
			getModel()->getCONFIG()->setNextServiceDate(m_iYear,m_iMonth,m_iDay);

 			CStringW szNextDate=_T("***next Service Date: ");
			CStringW strDate;
			strDate.Format(_T("%.2d.%.2d.%.4d"),m_iDay,m_iMonth,m_iYear);
 			szNextDate+=strDate;
 			szNextDate+=_T("***");
 			theApp.getLog()->WriteLine(szNextDate);

			if(m_pbtnOk)
				m_pbtnOk->SetBitmaps(m_pcOkGreen_Up,m_pcOkGreen_Dw,m_pcOkGreen_Up,m_pcOkGreen_Up);

			Sleep(500);
		}
	}
	if(bInvalid)	//not valid
	{ 
		if(AfxGetApp())
		{
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ERR_DATE);

			AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
		}
	}

	SetAllButtonUnpressed();
}


//=============================================================================
/**
 * @brief
 **/
//=============================================================================
eBtnState CWndServiceMaintenance::GetCurrentBtnState()
{
	POSITION pos;

	pos = m_plStaticBtn.GetHeadPosition();

	do {
		CSelectSetupBtn* pBtn  = m_plStaticBtn.GetNext(pos);

		//int iT = pBtn->GetBtnId();

		if(m_iCurrentField == pBtn->GetBtnId())
		{
			return pBtn->GetState();
		}


	} while (pos != NULL);

	return BS_NONE;
}

//=============================================================================
/**
 * @brief
 **/
//=============================================================================
eBtnState CWndServiceMaintenance::GetBtnState(int iID)
{
	POSITION pos;

	pos = m_plStaticBtn.GetHeadPosition();

	do {
		CSelectSetupBtn* pBtn  = m_plStaticBtn.GetNext(pos);

		//int iT = pBtn->GetBtnId();

		if(iID == pBtn->GetBtnId())
		{
			return pBtn->GetState();
		}


	} while (pos != NULL);

	return BS_NONE;
}

//=============================================================================
/**
 * @brief
 **/
//=============================================================================
void CWndServiceMaintenance::SetOneButtonDepressed(int btnID)
{
	POSITION pos;

	try
	{
		for( pos = m_plStaticBtn.GetHeadPosition(); pos != NULL; )
		{
			CSelectSetupBtn* pBtn = m_plStaticBtn.GetNext( pos );

			//int iT = pBtn->GetBtnId();

			if(pBtn->GetBtnId() != btnID)
			{
				pBtn->SetState(BS_UP);
			}
			else
			{
				pBtn->SetState(BS_DOWN);
				m_iCurrentField=pBtn->GetBtnId();
			}
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CWndServiceMaintenance::SetOneButtonDepressed"));
	}
	
	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
}

//=============================================================================
/**
 * @brief
 **/
//=============================================================================
void CWndServiceMaintenance::SetAllButtonUnpressed()
{
	POSITION pos;

	try
	{
		for( pos = m_plStaticBtn.GetHeadPosition(); pos != NULL; )
		{
			CSelectSetupBtn* pBtn = m_plStaticBtn.GetNext( pos );

			//int iT = pBtn->GetBtnId();

			pBtn->SetState(BS_UP);

		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CWndServiceMaintenance::SetAllButtonUnpressed"));
	}
	
	m_iCurrentField=0;

	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
}

//=============================================================================
/**
 * @brief
 **/
//=============================================================================
void CWndServiceMaintenance::SetOneButtonFocused(int btnID)
{
		POSITION pos;

		try
		{
			for( pos = m_plStaticBtn.GetHeadPosition(); pos != NULL; )
			{
				CSelectSetupBtn* pBtn = m_plStaticBtn.GetNext( pos );

				//int iT = pBtn->GetBtnId();

				if(pBtn->GetBtnId() != btnID)
				{
					pBtn->SetState(BS_UP);
				}
				else
				{
					pBtn->SetState(BS_FOCUSED);

					m_iCurrentField=btnID;
				}
			}
		}
		catch (...)
		{
			theApp.ReportException(_T("CWndServiceMaintenance::SetOneButtonFocused"));
		}


		if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
		{
			m_dwLastSetupTimer=GetTickCount();
			if(GetParent())
				GetParent()->PostMessage(WM_SET_SETUPTIMER);
		}
}

//=============================================================================
/**
 * @brief
 **/
//=============================================================================
void CWndServiceMaintenance::SetNextButtonFocused()
{
	POSITION pos;

	pos = m_plStaticBtn.GetHeadPosition();

	do {
		CSelectSetupBtn* pBtn  = m_plStaticBtn.GetNext(pos);

		//int iT = pBtn->GetBtnId();



		if(m_iCurrentField == pBtn->GetBtnId())
		{
			
			pBtn->SetState(BS_UP);

			if(pos == NULL)
			{
				pos = m_plStaticBtn.GetHeadPosition();
				pBtn = m_plStaticBtn.GetNext( pos );

				pBtn->SetState(BS_FOCUSED);
				
				m_iCurrentField=pBtn->GetBtnId();
				//getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);
				break;
			}
			else
			{
				pBtn = m_plStaticBtn.GetNext( pos );
				pBtn->SetState(BS_FOCUSED);
				m_iCurrentField=pBtn->GetBtnId();
				//getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);
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

//=============================================================================
/**
 * @brief
 **/
//=============================================================================
void CWndServiceMaintenance::SetPrevButtonFocused()
{
	POSITION pos;

	pos = m_plStaticBtn.GetTailPosition();

	do {
		CSelectSetupBtn* pBtn  = m_plStaticBtn.GetPrev(pos);

		//int iT = pBtn->GetBtnId();

		pBtn->SetState(BS_UP);

		if(m_iCurrentField == pBtn->GetBtnId())
		{
			if(pos == NULL)
			{
				pos = m_plStaticBtn.GetTailPosition();
				pBtn = m_plStaticBtn.GetPrev( pos );

				pBtn->SetState(BS_FOCUSED);
				m_iCurrentField=pBtn->GetBtnId();
				//getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurrentField);

				break;
			}
			else
			{
				pBtn = m_plStaticBtn.GetPrev( pos );
				pBtn->SetState(BS_FOCUSED);
				m_iCurrentField=pBtn->GetBtnId();
				//getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);

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


//=============================================================================
/**
 * @brief
 **/
//=============================================================================
void CWndServiceMaintenance::DateTimeChanged()
{

	m_dtNextServiceDate.SetDate(m_iYear, m_iMonth, m_iDay);
	bool bDateValidChanged=false;

	if (m_dtNextServiceDate.GetStatus() == COleDateTime::valid && m_pbtnOk)
	{
		SYSTEMTIME stTime;
		GetLocalTime(&stTime);
		COleDateTime dtCurrentTime(stTime);
		if (m_dtNextServiceDate>dtCurrentTime)
		{
			m_pbtnOk->SetBitmaps(m_pcOkRed_Up,m_pcOkRed_Dw,m_pcOkRed_Up,m_pcOkRed_Up);
			bDateValidChanged=true;
		}
		else
		{
			m_pbtnOk->SetBitmaps(m_pcOkDisabled,m_pcOkDisabled,m_pcOkDisabled,m_pcOkDisabled);
			bDateValidChanged=false;
		}
	}
	else
	{
		if(m_pbtnOk)
			m_pbtnOk->SetBitmaps(m_pcOkDisabled,m_pcOkDisabled,m_pcOkDisabled,m_pcOkDisabled);
		bDateValidChanged=false;
	}
	if(bDateValidChanged!=m_bDateValidChanged)
	{
		Draw();
		m_bDateValidChanged=bDateValidChanged;	
	}
	m_bDateChanged=true;
}


//=============================================================================
/**
 * @brief
 **/
//=============================================================================
BOOL CWndServiceMaintenance::PreTranslateMessage(MSG* pMsg) 
{
	if (m_bServiceDateOn)
	{
		switch(pMsg->message)
		{
		case WM_KEYDOWN:
			if(pMsg->wParam==VK_SPACE)
			{
				eBtnState eState = GetCurrentBtnState();
				if(eState==BS_NONE)
				{
					SetOneButtonFocused(IDC_BTN_SETUP_YEAR);
				}
				else if(eState==BS_UP)
				{
					SetOneButtonDepressed(m_iCurrentField);
				}
				else if(eState==BS_FOCUSED)
				{
					SetOneButtonDepressed(m_iCurrentField);
				}
				else if(eState==BS_DOWN)
				{
					SetOneButtonFocused(m_iCurrentField);
				}
				return 1;
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				switch(m_iCurrentField)
				{
				case IDC_BTN_SETUP_MONTH:
					{
						if(GetBtnState(IDC_BTN_SETUP_MONTH)==BS_DOWN)
						{
							if(m_iMonth>1)
								m_iMonth--;
							else
								m_iMonth=12;

							if(m_pbtnMonth)
								m_pbtnMonth->RefreshText(GetMonthString(m_iMonth));

							DateTimeChanged();

						}
						else
							SetPrevButtonFocused();
					}
					break;
				case IDC_BTN_SETUP_YEAR:
					{
						if(GetBtnState(IDC_BTN_SETUP_YEAR)==BS_DOWN)
						{
							if(m_iYear>1967)
								m_iYear--;
							else
								m_iYear=2500;

							CStringW strDate;

							strDate.Format(_T("%.4d"),m_iYear);
							if(m_pbtnYear)
								m_pbtnYear->RefreshText(strDate);

							DateTimeChanged();
						}
						else
							SetPrevButtonFocused();
					}
					break;
				case IDC_BTN_SETUP_DAY:
					{
						if(GetBtnState(IDC_BTN_SETUP_DAY)==BS_DOWN)
						{
							if(m_iMonth==2)
							{
								if(m_iDay>1)
									m_iDay--;
								else
									m_iDay=29;

								//check if valid
								//COleDateTime m_dtNextServiceDate;

								if(m_dtNextServiceDate.SetDate(m_iYear, m_iMonth, m_iDay)==0)
								{


								}
								else	//not valid
								{
									if(m_dtNextServiceDate.GetStatus() == COleDateTime::valid)
									{

									}
									else if(m_dtNextServiceDate.GetStatus() == COleDateTime::invalid)
									{
										m_iDay=28;
									}
									else if(m_dtNextServiceDate.GetStatus() == COleDateTime::null)
									{

									}
									else if(m_dtNextServiceDate.GetStatus() == COleDateTime::error )
									{

									}
								}
							}
							else if(m_iMonth==4 || m_iMonth==6 || m_iMonth==9 || m_iMonth==11)
							{
								if(m_iDay>1)
									m_iDay--;
								else
									m_iDay=30;
							}
							else
							{
								if(m_iDay>1)
									m_iDay--;
								else
									m_iDay=31;
							}



							CStringW strDate;

							strDate.Format(_T("%.2d"),m_iDay);
							if(m_pbtnDay)
								m_pbtnDay->RefreshText(strDate);

							DateTimeChanged();
						}
						else
							SetPrevButtonFocused();
					}
					break;
				default:
					{

					}
					break;
				}
				return 1;
			}
			else if(pMsg->wParam==VK_UP)
			{
				switch(m_iCurrentField)
				{
				case IDC_BTN_SETUP_MONTH:
					{
						if(GetBtnState(IDC_BTN_SETUP_MONTH)==BS_DOWN)
						{
							if(m_iMonth<12)
								m_iMonth++;
							else
								m_iMonth=1;

							if(m_pbtnMonth)
								m_pbtnMonth->RefreshText(GetMonthString(m_iMonth));

							DateTimeChanged();
						}
						else
							SetNextButtonFocused();
					}
					break;
				case IDC_BTN_SETUP_YEAR:
					{
						if(GetBtnState(IDC_BTN_SETUP_YEAR)==BS_DOWN)
						{
							if(m_iYear<2500)
								m_iYear++;
							else
								m_iYear=1967;

							CStringW strDate;

							strDate.Format(_T("%.4d"),m_iYear);
							if(m_pbtnYear)
								m_pbtnYear->RefreshText(strDate);

							DateTimeChanged();
						}
						else
							SetNextButtonFocused();
					}
					break;
				case IDC_BTN_SETUP_DAY:
					{
						if(GetBtnState(IDC_BTN_SETUP_DAY)==BS_DOWN)
						{
							if(m_iMonth==2)
							{
								if(m_iDay<29)
									m_iDay++;
								else
									m_iDay=1;

								//check if valid
								//COleDateTime m_dtNextServiceDate;

								if(m_dtNextServiceDate.SetDate(m_iYear, m_iMonth, m_iDay)==0)
								{


								}
								else	//not valid
								{
									if(m_dtNextServiceDate.GetStatus() == COleDateTime::valid)
									{

									}
									else if(m_dtNextServiceDate.GetStatus() == COleDateTime::invalid)
									{
										m_iDay=1;
									}
									else if(m_dtNextServiceDate.GetStatus() == COleDateTime::null)
									{

									}
									else if(m_dtNextServiceDate.GetStatus() == COleDateTime::error )
									{

									}
								}
							}
							else if(m_iMonth==4 || m_iMonth==6 || m_iMonth==9 || m_iMonth==11)
							{
								if(m_iDay<30)
									m_iDay++;
								else
									m_iDay=1;
							}
							else
							{
								if(m_iDay<31)
									m_iDay++;
								else
									m_iDay=1;
							}


							CStringW strDate;

							strDate.Format(_T("%.2d"),m_iDay);
							if(m_pbtnDay)
								m_pbtnDay->RefreshText(strDate);

							DateTimeChanged();
						}
						else
							SetNextButtonFocused();
					}
					break;
				default:
					{

					}
					break;
				}
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
	}	
	return CWnd::PreTranslateMessage(pMsg);
}