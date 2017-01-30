// SubViewDateTime.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SubViewDateTime.h"

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



// CSubViewDateTime

IMPLEMENT_DYNAMIC(CSubViewDateTime, CWnd)

CSubViewDateTime::CSubViewDateTime()
{
	m_pModel=NULL;

	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	m_hDC = NULL;
	m_hBmp= NULL;
	m_hBmpPrev= NULL;

	m_iCounter=0;
	m_bBigSteps=false;
	m_eTimeChanger=TC_OFF;

	m_iCurrentField=0;

	m_iYear=0;
	m_iMonth=0;
	m_iDay=0;
	m_iHour=0;
	m_iMinute=0;

	m_pbtnOk=NULL;


	m_pcRegionTopLeCorner=NULL;
	m_pcRegionTopRiCorner=NULL;
	m_pcRegionBotLeCorner=NULL;
	m_pcRegionBotRiCorner=NULL;

	m_pbtnDay=NULL;
	m_pbtnMonth=NULL;
	m_pbtnYear=NULL;
	m_pbtnHour=NULL;
	m_pbtnMinute=NULL;

	m_pcSmallUp_Up=NULL;
	m_pcSmallUp_Dw=NULL;
	m_pcSmallDw_Up=NULL;
	m_pcSmallDw_Dw=NULL;
	m_pcOkGreen_Up=NULL;
	m_pcOkGreen_Dw=NULL;
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

	m_pcHourNextUp=NULL;
	m_pcHourNextDwn=NULL;

	m_pcMinNextUp=NULL;
	m_pcMinNextDwn=NULL;

	m_bDateTimeChanged=false;

	m_dwLastSetupTimer=0;
}

CSubViewDateTime::~CSubViewDateTime()
{
	delete m_pcDayNextUp;
	m_pcDayNextUp=NULL;
	delete m_pcDayNextDwn;
	m_pcDayNextDwn=NULL;

	delete m_pbtnOk;
	m_pbtnOk=NULL;

	delete m_pbtnMonth;
	m_pbtnMonth=NULL;
	delete m_pbtnYear;
	m_pbtnYear=NULL;
	delete m_pbtnDay;
	m_pbtnDay=NULL;
	delete m_pbtnHour;
	m_pbtnHour=NULL;
	delete m_pbtnMinute;
	m_pbtnMinute=NULL;

	
	delete m_pcMonthNextUp;
	m_pcMonthNextUp=NULL;
	delete m_pcMonthNextDwn;
	m_pcMonthNextDwn=NULL;
	delete m_pcYearNextUp;
	m_pcYearNextUp=NULL;
	delete m_pcYearNextDwn;
	m_pcYearNextDwn=NULL;
	delete m_pcHourNextUp;
	m_pcHourNextUp=NULL;
	delete m_pcHourNextDwn;
	m_pcHourNextDwn=NULL;
	delete m_pcMinNextUp;
	m_pcMinNextUp=NULL;
	delete m_pcMinNextDwn;
	m_pcMinNextDwn=NULL;
	/*delete m_pcLargeUp_Up;
	m_pcLargeUp_Up=NULL;
	delete m_pcLargeUp_Dw;
	m_pcLargeUp_Dw=NULL;
	delete m_pcLargeDw_Up;
	m_pcLargeDw_Up=NULL;
	delete m_pcLargeDw_Dw;
	m_pcLargeDw_Dw=NULL;*/
	delete m_pcOkGreen_Up;
	m_pcOkGreen_Up=NULL;
	delete m_pcOkGreen_Dw;
	m_pcOkGreen_Dw=NULL;
	delete m_pcOkRed_Up;
	m_pcOkRed_Up=NULL;
	delete m_pcOkRed_Dw;
	m_pcOkRed_Dw=NULL;

	delete m_pcStaticMonth_Up;
	m_pcStaticMonth_Up=NULL;
	delete m_pcStaticMonth_Dw;
	m_pcStaticMonth_Dw=NULL;
	delete m_pcStaticYear_Up;
	m_pcStaticYear_Up=NULL;
	delete m_pcStaticYear_Dw;
	m_pcStaticYear_Dw=NULL;
	delete m_pcStaticTime_Up;
	m_pcStaticTime_Up=NULL;
	delete m_pcStaticTime_Dw;
	m_pcStaticTime_Dw=NULL;
	delete m_pcStaticMonth_Fc;
	m_pcStaticMonth_Fc=NULL;
	delete m_pcStaticYear_Fc;
	m_pcStaticYear_Fc=NULL;
	delete m_pcStaticTime_Fc;
	m_pcStaticTime_Fc=NULL;
	delete m_pcSmallUp_Up;
	m_pcSmallUp_Up=NULL;
	delete m_pcSmallUp_Dw;
	m_pcSmallUp_Dw=NULL;
	delete m_pcSmallDw_Up;
	m_pcSmallDw_Up=NULL;
	delete m_pcSmallDw_Dw;
	m_pcSmallDw_Dw=NULL;
	delete m_pcRegionTopLeCorner;
	m_pcRegionTopLeCorner=NULL;
	delete m_pcRegionTopRiCorner;
	m_pcRegionTopRiCorner=NULL;
	delete m_pcRegionBotLeCorner;
	m_pcRegionBotLeCorner=NULL;
	delete m_pcRegionBotRiCorner;
	m_pcRegionBotRiCorner=NULL;
}


BEGIN_MESSAGE_MAP(CSubViewDateTime, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SETUP_DT_OK, &CSubViewDateTime::OnBnClickedSetDateTime)
	ON_BN_CLICKED(IDC_BTN_SETUP_MONTH, &CSubViewDateTime::OnBnClickedMonth)
	ON_BN_CLICKED(IDC_BTN_SETUP_YEAR, &CSubViewDateTime::OnBnClickedYear)
	ON_BN_CLICKED(IDC_BTN_SETUP_DAY, &CSubViewDateTime::OnBnClickedDay)
	ON_BN_CLICKED(IDC_BTN_SETUP_MINUTE, &CSubViewDateTime::OnBnClickedMinute)
	ON_BN_CLICKED(IDC_BTN_SETUP_HOUR, &CSubViewDateTime::OnBnClickedHour)
END_MESSAGE_MAP()



// CSubViewDateTime message handlers

// **************************************************************************
// 
// **************************************************************************
CMVModel *CSubViewDateTime::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}
// **************************************************************************
// 
// **************************************************************************
bool CSubViewDateTime::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
{
	if (!Create(pParentWnd, rc, nID, pContext))
	{
		return false;
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CSubViewDateTime::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

		//CBrush cbrBack(BACKGND);
		CBrush cbrBack;
		//cbrFill.CreateSolidBrush(RGB(181,178,181));
		cbrBack.CreateSolidBrush(BACKGND);
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		/*RECT rc;
		rc.left = 180;  
		rc.top = 20;  
		rc.right  = 620;  
		rc.bottom = 450;

		SelectObject(m_hDC,cbrFill);*/

		m_pcRegionTopLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_TOP_CORNER);
		m_pcRegionTopRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_TOP_CORNER);
		m_pcRegionBotLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_BOT_CORNER);
		m_pcRegionBotRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_BOT_CORNER);



		m_pcSmallUp_Up= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUP1_UPUP);
		m_pcSmallUp_Dw= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUP1_UPDW);
		m_pcSmallDw_Up= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUP1_DWUP);
		m_pcSmallDw_Dw= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUP1_DWDW);

		m_pcOkGreen_Up= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUP_OKGREENUP);
		m_pcOkGreen_Dw= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUP_OKGREENDW);
		m_pcOkRed_Up= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUP_OKREDUP);
		m_pcOkRed_Dw= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUP_OKREDDW);

		
		m_pcStaticMonth_Up= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_MONTH_UP);
		m_pcStaticMonth_Dw= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_MONTH_DW);
		m_pcStaticMonth_Fc= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_MONTH_FC);
		m_pcStaticYear_Up= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_YEAR_UP);
		m_pcStaticYear_Dw= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_VALUE_DW);
		m_pcStaticYear_Fc= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_VALUE_FC);
		m_pcStaticTime_Up= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_TIME_UP);
		m_pcStaticTime_Dw= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_TIME_DW);
		m_pcStaticTime_Fc= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_TIME_FC);

		SYSTEMTIME st;
		GetLocalTime(&st);
		m_dtDateTime=COleDateTime(st);
		//CStringW szTime = m_dtDateTime.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);

		m_iYear=m_dtDateTime.GetYear();
		m_iMonth=m_dtDateTime.GetMonth();
		m_iDay=m_dtDateTime.GetDay();
		m_iHour=m_dtDateTime.GetHour();
		m_iMinute=m_dtDateTime.GetMinute();
		
		
		BTN btn;

		//CUpDwnBtn* m_pbtnYear;
		btn.wID					= IDC_BTN_SETUP_YEAR;	
		btn.poPosition.x		= 330;
		btn.poPosition.y		= 55;
		btn.pcBmpUp				= m_pcStaticYear_Up;
		btn.pcBmpDown			= m_pcStaticYear_Dw;
		btn.pcBmpFocus			= m_pcStaticYear_Fc;
		btn.pcBmpDisabled		= m_pcStaticYear_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnYear=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
		m_pbtnYear->Create(this,g_hf33AcuBold,0);
		CStringW cs = m_dtDateTime.Format(_T("%Y"));
		m_pbtnYear->SetText(cs);
		m_pbtnYear->ShowWindow(SW_SHOW);
		m_plStaticBtn.AddTail(m_pbtnYear);

		//CUpDwnBtn* m_pbtnDay;
		btn.wID					= IDC_BTN_SETUP_DAY;	
		btn.poPosition.x		= 265;
		btn.poPosition.y		= 165;
		btn.pcBmpUp				= m_pcStaticTime_Up;
		btn.pcBmpDown			= m_pcStaticTime_Dw;
		btn.pcBmpFocus			= m_pcStaticTime_Fc;
		btn.pcBmpDisabled		= m_pcStaticTime_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnDay=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
		m_pbtnDay->Create(this,g_hf31AcuBold,0);
		cs = m_dtDateTime.Format(_T("%d"));
		m_pbtnDay->SetText(cs);
		m_pbtnDay->ShowWindow(SW_SHOW);
		m_plStaticBtn.AddTail(m_pbtnDay);

		//CUpDwnBtn* m_pbtnMonth;
		btn.wID					= IDC_BTN_SETUP_MONTH;	
		btn.poPosition.x		= 340;
		btn.poPosition.y		= 165;
		btn.pcBmpUp				= m_pcStaticMonth_Up;
		btn.pcBmpDown			= m_pcStaticMonth_Dw;
		btn.pcBmpFocus			= m_pcStaticMonth_Fc;
		btn.pcBmpDisabled		= m_pcStaticMonth_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnMonth=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
		m_pbtnMonth->Create(this,g_hf27AcuBold,0);
		cs = GetMonthString(m_dtDateTime.GetMonth());
		m_pbtnMonth->SetText(cs);
		m_pbtnMonth->ShowWindow(SW_SHOW);
		m_plStaticBtn.AddTail(m_pbtnMonth);


	

		//CUpDwnBtn* m_pbtnHour;
		btn.wID					= IDC_BTN_SETUP_HOUR;	
		btn.poPosition.x		= 320;
		btn.poPosition.y		= 275;
		btn.pcBmpUp				= m_pcStaticTime_Up;
		btn.pcBmpDown			= m_pcStaticTime_Dw;
		btn.pcBmpFocus			= m_pcStaticTime_Fc;
		btn.pcBmpDisabled		= m_pcStaticTime_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnHour=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
		m_pbtnHour->Create(this,g_hf33AcuBold,0);
		cs = m_dtDateTime.Format(_T("%H"));
		m_pbtnHour->SetText(cs);
		m_pbtnHour->ShowWindow(SW_SHOW);
		m_plStaticBtn.AddTail(m_pbtnHour);

		//CUpDwnBtn* m_pbtnMinute;
		btn.wID					= IDC_BTN_SETUP_MINUTE;	
		btn.poPosition.x		= 390;
		btn.poPosition.y		= 275;
		btn.pcBmpUp				= m_pcStaticTime_Up;
		btn.pcBmpDown			= m_pcStaticTime_Dw;
		btn.pcBmpFocus			= m_pcStaticTime_Fc;
		btn.pcBmpDisabled		= m_pcStaticTime_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnMinute=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
		m_pbtnMinute->Create(this,g_hf33AcuBold,0);
		cs = m_dtDateTime.Format(_T("%M"));
		m_pbtnMinute->SetText(cs);
		m_pbtnMinute->ShowWindow(SW_SHOW);
		m_plStaticBtn.AddTail(m_pbtnMinute);

		


		btn.wID					= IDC_BTN_SETUP_DT_OK;	
		btn.poPosition.x		= 540;
		btn.poPosition.y		= 365;
		btn.pcBmpUp				= m_pcOkGreen_Up;
		btn.pcBmpDown			= m_pcOkGreen_Dw;
		btn.pcBmpFocus			= m_pcOkGreen_Up;
		btn.pcBmpDisabled		= m_pcOkGreen_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnOk=new CBtn(btn,COLOR_TXTBTNUP);
		m_pbtnOk->Create(this,g_hf21AcuBold,0);
		m_pbtnOk->SetText(_T(""));
		m_pbtnOk->ShowWindow(SW_SHOW);




		//*******************Days*****************************
		btn.wID					= IDC_BTN_SETUPDAY_NEXTUP;	
		btn.poPosition.x		= 200;
		btn.poPosition.y		= 154;
		btn.pcBmpUp				= m_pcSmallUp_Up;
		btn.pcBmpDown			= m_pcSmallUp_Dw;
		btn.pcBmpFocus			= m_pcSmallUp_Up;
		btn.pcBmpDisabled		= m_pcSmallUp_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcDayNextUp=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
		m_pcDayNextUp->Create(this,g_hf21AcuBold,0);
		m_pcDayNextUp->SetText(_T(""));
		m_pcDayNextUp->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUPDAY_NEXTDW;	
		btn.poPosition.x		= 200;
		btn.poPosition.y		= 196;
		btn.pcBmpUp				= m_pcSmallDw_Up;
		btn.pcBmpDown			= m_pcSmallDw_Dw;
		btn.pcBmpFocus			= m_pcSmallDw_Up;
		btn.pcBmpDisabled		= m_pcSmallDw_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcDayNextDwn=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
		m_pcDayNextDwn->Create(this,g_hf21AcuBold,0);
		m_pcDayNextDwn->SetText(_T(""));
		m_pcDayNextDwn->ShowWindow(SW_SHOW);


		//*******************Month*****************************
		HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf21AcuBold);
		int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
		/*rc.top = 50;
		rc.bottom = 100;
		rc.left = 270;
		rc.right = 420;
		DrawText(m_hDC,strMonDate,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);*/

		btn.wID					= IDC_BTN_SETUPMONTH_NEXTUP;	
		btn.poPosition.x		= 530;
		btn.poPosition.y		= 154;
		btn.pcBmpUp				= m_pcSmallUp_Up;
		btn.pcBmpDown			= m_pcSmallUp_Dw;
		btn.pcBmpFocus			= m_pcSmallUp_Up;
		btn.pcBmpDisabled		= m_pcSmallUp_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcMonthNextUp=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
		m_pcMonthNextUp->Create(this,g_hf21AcuBold,0);
		m_pcMonthNextUp->SetText(_T(""));
		m_pcMonthNextUp->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUPMONTH_NEXTDW;	
		btn.poPosition.x		= 530;
		btn.poPosition.y		= 196;
		btn.pcBmpUp				= m_pcSmallDw_Up;
		btn.pcBmpDown			= m_pcSmallDw_Dw;
		btn.pcBmpFocus			= m_pcSmallDw_Up;
		btn.pcBmpDisabled		= m_pcSmallDw_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcMonthNextDwn=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
		m_pcMonthNextDwn->Create(this,g_hf21AcuBold,0);
		m_pcMonthNextDwn->SetText(_T(""));
		m_pcMonthNextDwn->ShowWindow(SW_SHOW);

		//*******************Year*****************************
		/*rc.top = 50;
		rc.bottom = 100;
		rc.left = 440;
		rc.right = 515;
		DrawText(m_hDC,strYearDate,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);*/

		btn.wID					= IDC_BTN_SETUPYEAR_NEXTUP;	
		btn.poPosition.x		= 445;
		btn.poPosition.y		= 43;
		btn.pcBmpUp				= m_pcSmallUp_Up;
		btn.pcBmpDown			= m_pcSmallUp_Dw;
		btn.pcBmpFocus			= m_pcSmallUp_Up;
		btn.pcBmpDisabled		= m_pcSmallUp_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcYearNextUp=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
		m_pcYearNextUp->Create(this,g_hf21AcuBold,0);
		m_pcYearNextUp->SetText(_T(""));
		m_pcYearNextUp->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUPYEAR_NEXTDW;	
		btn.poPosition.x		= 445;
		btn.poPosition.y		= 85;
		btn.pcBmpUp				= m_pcSmallDw_Up;
		btn.pcBmpDown			= m_pcSmallDw_Dw;
		btn.pcBmpFocus			= m_pcSmallDw_Up;
		btn.pcBmpDisabled		= m_pcSmallDw_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcYearNextDwn=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
		m_pcYearNextDwn->Create(this,g_hf21AcuBold,0);
		m_pcYearNextDwn->SetText(_T(""));
		m_pcYearNextDwn->ShowWindow(SW_SHOW);

		//*******************Hour*****************************
		/*SelectObject(m_hDC,g_hf33AcuBold);
		rc.top = 390;
		rc.bottom = 450;
		rc.left = 310;
		rc.right = 465;
		DrawText(m_hDC,_T("08 : 24"),-1,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);*/
		btn.wID					= IDC_BTN_SETUPHOUR_NEXTUP;	
		btn.poPosition.x		= 255;
		btn.poPosition.y		= 265;
		btn.pcBmpUp				= m_pcSmallUp_Up;
		btn.pcBmpDown			= m_pcSmallUp_Dw;
		btn.pcBmpFocus			= m_pcSmallUp_Up;
		btn.pcBmpDisabled		= m_pcSmallUp_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcHourNextUp=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
		m_pcHourNextUp->Create(this,g_hf21AcuBold,0);
		m_pcHourNextUp->SetText(_T(""));
		m_pcHourNextUp->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUPHOUR_NEXTDW;	
		btn.poPosition.x		= 255;
		btn.poPosition.y		= 307;
		btn.pcBmpUp				= m_pcSmallDw_Up;
		btn.pcBmpDown			= m_pcSmallDw_Dw;
		btn.pcBmpFocus			= m_pcSmallDw_Up;
		btn.pcBmpDisabled		= m_pcSmallDw_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcHourNextDwn=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
		m_pcHourNextDwn->Create(this,g_hf21AcuBold,0);
		m_pcHourNextDwn->SetText(_T(""));
		m_pcHourNextDwn->ShowWindow(SW_SHOW);

		//*******************Minute*****************************
		btn.wID					= IDC_BTN_SETUPMIN_NEXTUP;	
		btn.poPosition.x		= 465;
		btn.poPosition.y		= 265;
		btn.pcBmpUp				= m_pcSmallUp_Up;
		btn.pcBmpDown			= m_pcSmallUp_Dw;
		btn.pcBmpFocus			= m_pcSmallUp_Up;
		btn.pcBmpDisabled		= m_pcSmallUp_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcMinNextUp=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
		m_pcMinNextUp->Create(this,g_hf21AcuBold,0);
		m_pcMinNextUp->SetText(_T(""));
		m_pcMinNextUp->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUPMIN_NEXTDW;	
		btn.poPosition.x		= 465;
		btn.poPosition.y		= 307;
		btn.pcBmpUp				= m_pcSmallDw_Up;
		btn.pcBmpDown			= m_pcSmallDw_Dw;
		btn.pcBmpFocus			= m_pcSmallDw_Up;
		btn.pcBmpDisabled		= m_pcSmallDw_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcMinNextDwn=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
		m_pcMinNextDwn->Create(this,g_hf21AcuBold,0);
		m_pcMinNextDwn->SetText(_T(""));
		m_pcMinNextDwn->ShowWindow(SW_SHOW);

		Draw();

		SetBkMode(m_hDC,nBkMode);
		SelectObject(m_hDC,hPrevFont);
		SelectObject(m_hDC,hbrprev);
		SelectObject(m_hDC,hpenprev);
	
		

		return 1;
	}
	else
		return 0;
}

//#define TIME_REG _T("Time Zones")


//void CSubViewDateTime::TimeZoneInfoToTimeZoneInformation( TIME_ZONE_INFORMATION& TimeZoneInfo1 , const TIME_ZONE_INFO& TimeZoneInfo2 )
//{
//	TimeZoneInfo1.Bias = TimeZoneInfo2.Bias ;
//	TimeZoneInfo1.DaylightBias = TimeZoneInfo2.DaylightBias ;
//	TimeZoneInfo1.DaylightDate = TimeZoneInfo2.DaylightDate ;
//	TimeZoneInfo1.StandardBias = TimeZoneInfo2.StandardBias ;
//	TimeZoneInfo1.StandardDate = TimeZoneInfo2.StandardDate ;
//}

//test
//void TimeZoneInfoToTimeZoneInformation( TIME_ZONE_INFORMATION& TimeZoneInfo1 , const TIME_ZONE_INFO& TimeZoneInfo2 );

//HKEY hkTimeZones;
//
//if( RegOpenKeyEx(HKEY_LOCAL_MACHINE,TIME_REG,0,KEY_READ,&hkTimeZones) == ERROR_SUCCESS )
//{
//	DWORD dwIndex = 0;
//	TCHAR tcKeyName[512];
//	DWORD dwcbName = 512 * sizeof( TCHAR );
//	FILETIME ftLastWrite;
//
//	while( RegEnumKeyEx(hkTimeZones,dwIndex++,tcKeyName,&dwcbName,NULL,NULL,NULL,&ftLastWrite) != ERROR_NO_MORE_ITEMS )
//	{
//		HKEY hkTimeZone;
//		if( RegOpenKeyEx(hkTimeZones,tcKeyName,0,KEY_READ,&hkTimeZone) == ERROR_SUCCESS )
//		{
//
//			DWORD dwTimeZoneIndex;
//			WCHAR Name[ 32 ];
//			TIME_ZONE_INFO TZI;
//			TIME_ZONE_INFORMATION TZI2;
//			// Get Index
//			/*DWORD dwDataSize = sizeof( DWORD );
//			RegQueryValueEx(hkTimeZone,_T("Index"),NULL,NULL,(BYTE*)&dwTimeZoneIndex,&dwDataSize);*/
//
//			// Get TZI Upper Bytes
//			DWORD dwDataSize = sizeof( TIME_ZONE_INFO );
//			RegQueryValueEx(hkTimeZone,_T("TZI"),NULL,NULL,(BYTE*)&TZI,&dwDataSize);
//
//			TimeZoneInfoToTimeZoneInformation( TZI2 , TZI );
//
//			// Get Text Values
//			dwDataSize = 32 * sizeof( TCHAR );
//			RegQueryValueEx(hkTimeZone,_T("Dlt"),NULL,NULL,(BYTE*)TZI2.DaylightName,&dwDataSize);
//			dwDataSize = 32 * sizeof( TCHAR );
//			RegQueryValueEx(hkTimeZone,_T("Std"),NULL,NULL,(BYTE*)TZI2.StandardName,&dwDataSize);
//
//			dwDataSize = 32 * sizeof( TCHAR );
//			RegQueryValueEx(hkTimeZone,_T("Display"),NULL,NULL,(BYTE*)Name,&dwDataSize);
//
//
//			//if( bShow )
//			{
//				/*_tprintf( _T("Index: %d\n") , dwTimeZoneIndex );
//				_tprintf( _T("  STD: %s\n") , TZI2.StandardName );
//				_tprintf( _T("  DLT: %s\n\n") , TZI2.DaylightName );*/
//			}
//			/*else
//			{
//			if( dwTimeZoneIndex == dwIndexToFind )
//			if( SetTimeZoneInformation( &TZI2 ) )
//			iErr = 0;
//			}*/
//			RegCloseKey( hkTimeZone ); 
//		}
//		dwcbName = 512 * sizeof( TCHAR );
//	}
//	RegCloseKey( hkTimeZones );
//}
//
//return;

//void CSubViewDateTime::Test()
//{
//	HKEY hkTimeZones;
//	
//	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE,TIME_REG,0,KEY_READ,&hkTimeZones) == ERROR_SUCCESS )
//	{
//		DWORD dwIndex = 0;
//		TCHAR tcKeyName[512];
//		DWORD dwcbName = 512 * sizeof( TCHAR );
//		FILETIME ftLastWrite;
//	
//		while( RegEnumKeyEx(hkTimeZones,dwIndex++,tcKeyName,&dwcbName,NULL,NULL,NULL,&ftLastWrite) != ERROR_NO_MORE_ITEMS )
//		{
//			HKEY hkTimeZone;
//			if( RegOpenKeyEx(hkTimeZones,tcKeyName,0,KEY_READ,&hkTimeZone) == ERROR_SUCCESS )
//			{
//	
//				DWORD dwTimeZoneIndex;
//				WCHAR Name[ 32 ];
//				TIME_ZONE_INFO TZI;
//				TIME_ZONE_INFORMATION TZI2;
//				// Get Index
//				/*DWORD dwDataSize = sizeof( DWORD );
//				RegQueryValueEx(hkTimeZone,_T("Index"),NULL,NULL,(BYTE*)&dwTimeZoneIndex,&dwDataSize);*/
//	
//				// Get TZI Upper Bytes
//				DWORD dwDataSize = sizeof( TIME_ZONE_INFO );
//				RegQueryValueEx(hkTimeZone,_T("TZI"),NULL,NULL,(BYTE*)&TZI,&dwDataSize);
//	
//				TimeZoneInfoToTimeZoneInformation( TZI2 , TZI );
//	
//				// Get Text Values
//				dwDataSize = 32 * sizeof( TCHAR );
//				RegQueryValueEx(hkTimeZone,_T("Dlt"),NULL,NULL,(BYTE*)TZI2.DaylightName,&dwDataSize);
//				dwDataSize = 32 * sizeof( TCHAR );
//				RegQueryValueEx(hkTimeZone,_T("Std"),NULL,NULL,(BYTE*)TZI2.StandardName,&dwDataSize);
//	
//				dwDataSize = 32 * sizeof( TCHAR );
//				RegQueryValueEx(hkTimeZone,_T("Display"),NULL,NULL,(BYTE*)Name,&dwDataSize);
//	
//	
//				//if( bShow )
//				{
//					/*_tprintf( _T("Index: %d\n") , dwTimeZoneIndex );
//					_tprintf( _T("  STD: %s\n") , TZI2.StandardName );
//					_tprintf( _T("  DLT: %s\n\n") , TZI2.DaylightName );*/
//				}
//				/*else
//				{
//				if( dwTimeZoneIndex == dwIndexToFind )
//				if( SetTimeZoneInformation( &TZI2 ) )
//				iErr = 0;
//				}*/
//				RegCloseKey( hkTimeZone ); 
//			}
//			dwcbName = 512 * sizeof( TCHAR );
//		}
//		RegCloseKey( hkTimeZones );
//	}
//	
//	return;
//}

void CSubViewDateTime::Draw()
{
	//Test();

	SYSTEMTIME st;
	GetLocalTime(&st);
	m_dtDateTime=COleDateTime(st);
	//CStringW szTime = m_dtDateTime.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);

	m_iYear=m_dtDateTime.GetYear();
	m_iMonth=m_dtDateTime.GetMonth();
	m_iDay=m_dtDateTime.GetDay();
	m_iHour=m_dtDateTime.GetHour();
	m_iMinute=m_dtDateTime.GetMinute();

	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);

	if(m_pcRegionTopLeCorner)
		m_pcRegionTopLeCorner->Draw(m_hDC,122,10);
	if(m_pcRegionTopRiCorner)
		m_pcRegionTopRiCorner->Draw(m_hDC,642,10);
	if(m_pcRegionBotLeCorner)
		m_pcRegionBotLeCorner->Draw(m_hDC,122,440);
	if(m_pcRegionBotRiCorner)
		m_pcRegionBotRiCorner->Draw(m_hDC,642,440);

	CBrush cbrFill;
	cbrFill.CreateSolidBrush(RGB(181,178,181));

	
	HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));
	HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrFill);

	Rectangle(m_hDC, 122, 30, 662, 440);

	Rectangle(m_hDC, 142, 10, 642, 30);
	Rectangle(m_hDC, 142, 440, 642, 460);

	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf21AcuBold);


	int tc=SetTextColor(m_hDC,0x0000000);

	//COleDateTime date = m_wndCalendar.MakeDate();

	// Load up months
	CStringW strMonDate;
	CStringW strYearDate;
	CStringW strHourDate;
	CStringW strMinuteDate;
	//CStringW strTimeDate;

	strMonDate=_T(" ")+GetMonthString(m_dtDateTime.GetMonth());

	strYearDate=m_dtDateTime.Format(_T("%Y"));

	strHourDate =m_dtDateTime.Format(_T("%H"));
	strMinuteDate = m_dtDateTime.Format(_T("%M"));
	//strTimeDate.Format(_T("%.2d : %.2d"),m_iHour,m_iMinute);
	

	/*rc.top = 50;
	rc.bottom = 100;
	rc.left = 270;
	rc.right = 420;
	DrawText(m_hDC,strMonDate,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);*/
	if(m_pbtnMonth)
		m_pbtnMonth->RefreshText(strMonDate);


	/*rc.top = 50;
	rc.bottom = 100;
	rc.left = 440;
	rc.right = 515;
	DrawText(m_hDC,strYearDate,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);*/
	if(m_pbtnYear)
		m_pbtnYear->RefreshText(strYearDate);

	SelectObject(m_hDC,g_hf33AcuBold);
	rc.top = 280;
	rc.bottom = 345;
	rc.left = 310;
	rc.right = 460;
	DrawText(m_hDC,_T(":"),-1,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);
	if(m_pbtnHour)
		m_pbtnHour->RefreshText(strHourDate);
	if(m_pbtnMinute)
		m_pbtnMinute->RefreshText(strMinuteDate);

	
	//dot of day/month
	rc.top = 165;
	rc.bottom = 225;
	rc.left = 325;
	rc.right = 340;
	DrawText(m_hDC,_T("."),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);


	
	SelectObject(m_hDC,g_hf10AcuBold);
	SetTextColor(m_hDC,RGB(255,100,0));

	/*rc.top = 380;
	rc.bottom = 460;
	rc.left = 650;
	rc.right = 795;*/
	rc.top = 390;
	rc.bottom = 470;
	rc.left = 140;
	rc.right = 530;
	DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_DT_TREND),-1,&rc,DT_TOP|DT_WORDBREAK|DT_RIGHT);

	//CDC* pDC=CDC::FromHandle(m_hDC);
	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(200,200,200));

	SelectObject(m_hDC, (HPEN)penLine);

	MoveToEx(m_hDC, 310, 43, NULL);
	LineTo(m_hDC, 445, 43);
	MoveToEx(m_hDC, 310, 125, NULL);
	LineTo(m_hDC, 445, 125);

	MoveToEx(m_hDC, 250, 154, NULL);
	LineTo(m_hDC, 530, 154);
	MoveToEx(m_hDC, 250, 236, NULL);
	LineTo(m_hDC, 530, 236);

	MoveToEx(m_hDC, 305, 265, NULL);
	LineTo(m_hDC, 465, 265);
	MoveToEx(m_hDC, 305, 347, NULL);
	LineTo(m_hDC, 465, 347);


	

	SetTextColor(m_hDC,tc);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hbrprev);
	SelectObject(m_hDC,hPrevFont);
	SelectObject(m_hDC,hpenprev);

	penLine.DeleteObject();
	Invalidate();
	UpdateWindow();
	

}

// **************************************************************************
// 
// **************************************************************************
eBtnState CSubViewDateTime::GetCurrentBtnState()
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

// **************************************************************************
// 
// **************************************************************************
eBtnState CSubViewDateTime::GetBtnState(int iID)
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

// **************************************************************************
// 
// **************************************************************************
void CSubViewDateTime::SetOneButtonDepressed(int btnID)
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
		CFabianHFOApp::ReportException(_T("EXCEPTION: CSubViewDateTime::SetOneButtonDepressed"));
		//AfxMessageBox( _T("EXCEPTION: CMVModel::VentModeChanged()()") );

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
void CSubViewDateTime::SetAllButtonUnpressed()
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
		CFabianHFOApp::ReportException(_T("EXCEPTION: CSubViewDateTime::SetAllButtonUnpressed"));
		//AfxMessageBox( _T("EXCEPTION: CMVModel::VentModeChanged()()") );

	}
	
	m_iCurrentField=0;

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
void CSubViewDateTime::SetOneButtonFocused(int btnID)
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
				//getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);
			}
		}
	}
	catch (...)
	{
		CFabianHFOApp::ReportException(_T("EXCEPTION: CSubViewDateTime::SetOneButtonFocused"));
		//AfxMessageBox( _T("EXCEPTION: CMVModel::VentModeChanged()()") );

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
void CSubViewDateTime::SetNextButtonFocused()
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


// **************************************************************************
// 
// **************************************************************************
void CSubViewDateTime::SetPrevButtonFocused()
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

// **************************************************************************
// 
// **************************************************************************
CStringW CSubViewDateTime::GetMonthString(int iMonth)
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

// **************************************************************************
// 
// **************************************************************************
void CSubViewDateTime::OnBnClickedDay()
{
	if(GetBtnState(IDC_BTN_SETUP_DAY)==BS_UP)
		SetOneButtonDepressed(IDC_BTN_SETUP_DAY);
	else if(GetBtnState(IDC_BTN_SETUP_DAY)==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_DAY);
	else if(GetBtnState(IDC_BTN_SETUP_DAY)==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_DAY);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewDateTime::OnBnClickedMonth()
{
	if(GetBtnState(IDC_BTN_SETUP_MONTH)==BS_UP)
		SetOneButtonDepressed(IDC_BTN_SETUP_MONTH);
	else if(GetBtnState(IDC_BTN_SETUP_MONTH)==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_MONTH);
	else if(GetBtnState(IDC_BTN_SETUP_MONTH)==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_MONTH);
}
// **************************************************************************
// 
// **************************************************************************
void CSubViewDateTime::OnBnClickedYear()
{
	//SetOneButtonDepressed(IDC_BTN_SETUP_YEAR);
	if(GetBtnState(IDC_BTN_SETUP_YEAR)==BS_UP)
		SetOneButtonDepressed(IDC_BTN_SETUP_YEAR);
	else if(GetBtnState(IDC_BTN_SETUP_YEAR)==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_YEAR);
	else if(GetBtnState(IDC_BTN_SETUP_YEAR)==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_YEAR);
}
// **************************************************************************
// 
// **************************************************************************
void CSubViewDateTime::OnBnClickedMinute()
{
	//SetOneButtonDepressed(IDC_BTN_SETUP_MINUTE);
	if(GetBtnState(IDC_BTN_SETUP_MINUTE)==BS_UP)
		SetOneButtonDepressed(IDC_BTN_SETUP_MINUTE);
	else if(GetBtnState(IDC_BTN_SETUP_MINUTE)==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_MINUTE);
	else if(GetBtnState(IDC_BTN_SETUP_MINUTE)==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_MINUTE);
}
// **************************************************************************
// 
// **************************************************************************
void CSubViewDateTime::OnBnClickedHour()
{
	//SetOneButtonDepressed(IDC_BTN_SETUP_HOUR);
	if(GetBtnState(IDC_BTN_SETUP_HOUR)==BS_UP)
		SetOneButtonDepressed(IDC_BTN_SETUP_HOUR);
	else if(GetBtnState(IDC_BTN_SETUP_HOUR)==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_HOUR);
	else if(GetBtnState(IDC_BTN_SETUP_HOUR)==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_HOUR);
}


// **************************************************************************
// 
// **************************************************************************
LRESULT CSubViewDateTime::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
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
			case IDC_BTN_SETUPHOUR_NEXTUP:
				{
					m_iCounter=0;
					m_bBigSteps=false;
					m_eTimeChanger=TC_HOUR_UP;
					SetTimer(CHANGETIMER,TIMECHANGE,NULL);
					SetOneButtonDepressed(IDC_BTN_SETUP_HOUR);
					return 1;
				}
				break;
			case IDC_BTN_SETUPHOUR_NEXTDW:
				{
					m_iCounter=0;
					m_bBigSteps=false;
					m_eTimeChanger=TC_HOUR_DW;
					SetTimer(CHANGETIMER,TIMECHANGE,NULL);
					SetOneButtonDepressed(IDC_BTN_SETUP_HOUR);
					return 1;
				}
				break;
			case IDC_BTN_SETUPMIN_NEXTUP:
				{
					m_iCounter=0;
					m_bBigSteps=false;
					m_eTimeChanger=TC_MIN_UP;
					SetTimer(CHANGETIMER,TIMECHANGE,NULL);
					SetOneButtonDepressed(IDC_BTN_SETUP_MINUTE);
					return 1;
				}
				break;
			case IDC_BTN_SETUPMIN_NEXTDW:
				{
					m_iCounter=0;
					m_bBigSteps=false;
					m_eTimeChanger=TC_MIN_DW;
					SetTimer(CHANGETIMER,TIMECHANGE,NULL);
					SetOneButtonDepressed(IDC_BTN_SETUP_MINUTE);
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
							COleDateTime oletimeTime;

							if(oletimeTime.SetDateTime(m_iYear, m_iMonth, m_iDay, m_iHour, m_iMinute, 0)==0)
							{


							}
							else	//not valid
							{
								if(oletimeTime.GetStatus() == COleDateTime::valid)
								{

								}
								else if(oletimeTime.GetStatus() == COleDateTime::invalid)
								{
									m_iDay=1;
								}
								else if(oletimeTime.GetStatus() == COleDateTime::null)
								{

								}
								else if(oletimeTime.GetStatus() == COleDateTime::error )
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
							COleDateTime oletimeTime;

							if(oletimeTime.SetDateTime(m_iYear, m_iMonth, m_iDay, m_iHour, m_iMinute, 0)==0)
							{


							}
							else	//not valid
							{
								if(oletimeTime.GetStatus() == COleDateTime::valid)
								{

								}
								else if(oletimeTime.GetStatus() == COleDateTime::invalid)
								{
									m_iDay=28;
								}
								else if(oletimeTime.GetStatus() == COleDateTime::null)
								{

								}
								else if(oletimeTime.GetStatus() == COleDateTime::error )
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
			case IDC_BTN_SETUPHOUR_NEXTUP:
				{
					if(m_iCounter==0)
					{
						if(m_iHour<23)
							m_iHour++;
						else
							m_iHour=0;

						CStringW strDate;

						strDate.Format(_T("%.2d"),m_iHour);
						if(m_pbtnHour)
							m_pbtnHour->RefreshText(strDate);

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
			case IDC_BTN_SETUPHOUR_NEXTDW:
				{
					if(m_iCounter==0)
					{
						if(m_iHour>0)
							m_iHour--;
						else
							m_iHour=23;

						CStringW strDate;

						strDate.Format(_T("%.2d"),m_iHour);
						if(m_pbtnHour)
							m_pbtnHour->RefreshText(strDate);

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
			case IDC_BTN_SETUPMIN_NEXTUP:
				{
					if(m_iCounter==0)
					{
						if(m_iMinute<59)
							m_iMinute++;
						else
							m_iMinute=0;

						CStringW strDate;

						strDate.Format(_T("%.2d"),m_iMinute);
						if(m_pbtnMinute)
							m_pbtnMinute->RefreshText(strDate);

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
			case IDC_BTN_SETUPMIN_NEXTDW:
				{
					if(m_iCounter==0)
					{
						if(m_iMinute>0)
							m_iMinute--;
						else
							m_iMinute=59;

						CStringW strDate;

						strDate.Format(_T("%.2d"),m_iMinute);
						if(m_pbtnMinute)
							m_pbtnMinute->RefreshText(strDate);

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
	/*case WM_CALBTN_DOWN:
		{
			SetAllButtonUnpressed();
		}
		break;*/
	/*case WM_CHANGE_YEAR:
		{
			m_iYear=wParam;
			Draw();

			DateTimeChanged();
		}
		break;
	case WM_CHANGE_MONTH:
		{
			m_iMonth=wParam;
			Draw();

			DateTimeChanged();
		}
		break;*/
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewDateTime::OnBnClickedSetDateTime()
{
	SetAllButtonUnpressed();

	if(m_bDateTimeChanged==false)
		return;

	//BYTE test[5];

	SYSTEMTIME stTimeOld;
	SYSTEMTIME stTime;

	

	GetLocalTime(&stTimeOld);
	GetLocalTime(&stTime);

	COleDateTime dtTimeOld(stTimeOld);

	COleDateTime oletimeTime;

	bool bInvalid=true;

	if(oletimeTime.SetDateTime(m_iYear, m_iMonth, m_iDay, m_iHour, m_iMinute, 0)==0)
	{
		bInvalid=false;

		stTime.wYear	=oletimeTime.GetYear();
		stTime.wMonth	=oletimeTime.GetMonth();
		stTime.wDay		=oletimeTime.GetDay();
		stTime.wHour	= oletimeTime.GetHour();
		stTime.wMinute	= oletimeTime.GetMinute();
		stTime.wSecond	= 0;
		stTime.wMilliseconds = 0;
		/*try
		{*/
			SetLocalTime(&stTime);
			//SetSystemTime(&stTime);

			m_bDateTimeChanged=false;

			if(m_pbtnOk)
				m_pbtnOk->SetBitmaps(m_pcOkGreen_Up,m_pcOkGreen_Dw,m_pcOkGreen_Up,m_pcOkGreen_Up);

			
			theApp.getLog()->WriteLine(_T("CSubViewDateTime::OnBnClickedSetDateTime"));
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_DELETE_TRENDS);
			
			getModel()->getCONFIG()->SetLastCalOxy21(_T(""));
			getModel()->getCONFIG()->SetLastCalOxy100(_T(""));
			getModel()->getCONFIG()->SetLastCalFlow(_T(""));
			//getModel()->getCONFIG()->SetLastCal0mbar(_T(""));
			//getModel()->getCONFIG()->SetLastCal60mbar(_T(""));

			Sleep(500);

			GetLocalTime(&stTime);

			if(stTime.wHour != oletimeTime.GetHour())
			{
				stTime.wYear	=oletimeTime.GetYear();
				stTime.wMonth	=oletimeTime.GetMonth();
				stTime.wDay		=oletimeTime.GetDay();
				stTime.wHour	= oletimeTime.GetHour();
				stTime.wMinute	= oletimeTime.GetMinute();
				stTime.wSecond	= 0;
				stTime.wMilliseconds = 0;

				SetLocalTime(&stTime);

				theApp.getLog()->WriteLine(_T("HFO:0290"));
			}

			if(getModel()->getCONFIG()->GetCO2module()!=CO2MODULE_NONE && getModel()->getETCO2()!=NULL)
			{
				getModel()->getETCO2()->get_LastCalDate();
			}
			getModel()->isMaintenanceNeeded();
		//}
		//__except (EXCEPTION_EXECUTE_HANDLER) {  // Handle all exceptions
		//	// Respond (perhaps only partially) to exception
			//	getModel()->SaveSystemLog();
			//AfxMessageBox( _T("EXCEPTION: test"));
		//	throw;       // Pass exception to some other handler
		//}

	}
	if(bInvalid)	//not valid
	{
		/*if(oletimeTime.GetStatus() == COleDateTime::valid)
		{

		}
		else if(oletimeTime.GetStatus() == COleDateTime::invalid)
		{

		}
		else if(oletimeTime.GetStatus() == COleDateTime::null)
		{

		}
		else if(oletimeTime.GetStatus() == COleDateTime::error )
		{

		}*/
		/*CStringW sData = getModel()->GetLanguageString(IDS_ERR_DATE);
		CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT,  sData, 2000);
		getModel()->Trigger(&event);*/

		if(AfxGetApp())
		{
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ERR_DATE);
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
		}
		
	}
	else
	{
		if(getModel()->getDATAHANDLER()->isFOTLicenseAvailable()==true)
		{
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_STOPANDRESET_FOT);
		}
	}
	//VERIFY(pCtrl->SetTime(oletimeTime));


}

// **************************************************************************
// 
// **************************************************************************
void CSubViewDateTime::Show(bool bShow)
{
	if(bShow)
	{
		ShowWindow(SW_SHOW);
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}
// **************************************************************************
// 
// **************************************************************************
void CSubViewDateTime::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

//************************************
// Method:    OnDestroy
// FullName:  CSubViewDateTime::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CSubViewDateTime::OnDestroy()
{
	KillTimer(CHANGETIMER);

	m_plStaticBtn.RemoveAll();
	
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
void CSubViewDateTime::OnTimer(UINT_PTR nIDEvent)
{
	CStringW strDate;

	if(nIDEvent==CHANGETIMER)
	{
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
					COleDateTime oletimeTime;

					if(oletimeTime.SetDateTime(m_iYear, m_iMonth, m_iDay, m_iHour, m_iMinute, 0)==0)
					{


					}
					else	//not valid
					{
						if(oletimeTime.GetStatus() == COleDateTime::valid)
						{

						}
						else if(oletimeTime.GetStatus() == COleDateTime::invalid)
						{
							m_iDay=1;
						}
						else if(oletimeTime.GetStatus() == COleDateTime::null)
						{

						}
						else if(oletimeTime.GetStatus() == COleDateTime::error )
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
					COleDateTime oletimeTime;

					if(oletimeTime.SetDateTime(m_iYear, m_iMonth, m_iDay, m_iHour, m_iMinute, 0)==0)
					{


					}
					else	//not valid
					{
						if(oletimeTime.GetStatus() == COleDateTime::valid)
						{

						}
						else if(oletimeTime.GetStatus() == COleDateTime::invalid)
						{
							m_iDay=28;
						}
						else if(oletimeTime.GetStatus() == COleDateTime::null)
						{

						}
						else if(oletimeTime.GetStatus() == COleDateTime::error )
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

		case TC_HOUR_UP:
			{
				if(m_iHour<23)
					m_iHour++;
				else
					m_iHour=0;

				strDate.Format(_T("%.2d"),m_iHour);
				if(m_pbtnHour)
					m_pbtnHour->RefreshText(strDate);

				DateTimeChanged();
			}
			break;
		case TC_HOUR_DW:
			{
				if(m_iHour>0)
					m_iHour--;
				else
					m_iHour=23;

				strDate.Format(_T("%.2d"),m_iHour);
				if(m_pbtnHour)
					m_pbtnHour->RefreshText(strDate);

				DateTimeChanged();
			}
			break;
		case TC_MIN_UP:
			{
				if((m_iCounter>5) && (m_bBigSteps))
				{
					if(m_iMinute<55)
						m_iMinute=m_iMinute+5;
					else
						m_iMinute=0;

					strDate.Format(_T("%.2d"),m_iMinute);
					if(m_pbtnMinute)
						m_pbtnMinute->RefreshText(strDate);

					DateTimeChanged();
				}
				else if((m_iCounter>5) && (m_iMinute%10==0) && (m_bBigSteps==false))
				{
					m_bBigSteps=true;
					if(m_iMinute<55)
						m_iMinute=m_iMinute+5;
					else
						m_iMinute=0;

					strDate.Format(_T("%.2d"),m_iMinute);
					if(m_pbtnMinute)
						m_pbtnMinute->RefreshText(strDate);

					DateTimeChanged();
				}
				else
				{
					if(m_iMinute<59)
						m_iMinute++;
					else
						m_iMinute=0;

					strDate.Format(_T("%.2d"),m_iMinute);
					if(m_pbtnMinute)
						m_pbtnMinute->RefreshText(strDate);

					DateTimeChanged();
				}
			}
			break;
		case TC_MIN_DW:
			{
				if((m_iCounter>5) && (m_bBigSteps))
				{
					if(m_iMinute>5)
						m_iMinute=m_iMinute-5;
					else
						m_iMinute=55;

					strDate.Format(_T("%.2d"),m_iMinute);
					if(m_pbtnMinute)
						m_pbtnMinute->RefreshText(strDate);

					DateTimeChanged();
				}
				else if((m_iCounter>5) && (m_iMinute%10==0) && (m_bBigSteps==false))
				{
					m_bBigSteps=true;
					if(m_iMinute>5)
						m_iMinute=m_iMinute-5;
					else
						m_iMinute=55;

					strDate.Format(_T("%.2d"),m_iMinute);
					if(m_pbtnMinute)
						m_pbtnMinute->RefreshText(strDate);

					DateTimeChanged();
				}
				else
				{
					if(m_iMinute>0)
						m_iMinute--;
					else
						m_iMinute=59;

					strDate.Format(_T("%.2d"),m_iMinute);
					if(m_pbtnMinute)
						m_pbtnMinute->RefreshText(strDate);

					DateTimeChanged();
				}
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

// **************************************************************************
// 
// **************************************************************************
BOOL CSubViewDateTime::PreTranslateMessage(MSG* pMsg) 
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
							COleDateTime oletimeTime;

							if(oletimeTime.SetDateTime(m_iYear, m_iMonth, m_iDay, m_iHour, m_iMinute, 0)==0)
							{


							}
							else	//not valid
							{
								if(oletimeTime.GetStatus() == COleDateTime::valid)
								{

								}
								else if(oletimeTime.GetStatus() == COleDateTime::invalid)
								{
									m_iDay=28;
								}
								else if(oletimeTime.GetStatus() == COleDateTime::null)
								{

								}
								else if(oletimeTime.GetStatus() == COleDateTime::error )
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
			case IDC_BTN_SETUP_HOUR:
				{
					if(GetBtnState(IDC_BTN_SETUP_HOUR)==BS_DOWN)
					{
						if(m_iHour>0)
							m_iHour--;
						else
							m_iHour=23;

						CStringW strDate;

						strDate.Format(_T("%.2d"),m_iHour);
						if(m_pbtnHour)
							m_pbtnHour->RefreshText(strDate);

						DateTimeChanged();
					}
					else
						SetPrevButtonFocused();

					
				}
				break;
			case IDC_BTN_SETUP_MINUTE:
				{
					if(GetBtnState(IDC_BTN_SETUP_MINUTE)==BS_DOWN)
					{
						if(m_iMinute>0)
							m_iMinute--;
						else
							m_iMinute=59;

						CStringW strDate;

						strDate.Format(_T("%.2d"),m_iMinute);
						if(m_pbtnMinute)
							m_pbtnMinute->RefreshText(strDate);

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
							COleDateTime oletimeTime;

							if(oletimeTime.SetDateTime(m_iYear, m_iMonth, m_iDay, m_iHour, m_iMinute, 0)==0)
							{


							}
							else	//not valid
							{
								if(oletimeTime.GetStatus() == COleDateTime::valid)
								{

								}
								else if(oletimeTime.GetStatus() == COleDateTime::invalid)
								{
									m_iDay=1;
								}
								else if(oletimeTime.GetStatus() == COleDateTime::null)
								{

								}
								else if(oletimeTime.GetStatus() == COleDateTime::error )
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
			case IDC_BTN_SETUP_HOUR:
				{
					if(GetBtnState(IDC_BTN_SETUP_HOUR)==BS_DOWN)
					{
						if(m_iHour<23)
							m_iHour++;
						else
							m_iHour=0;

						CStringW strDate;

						strDate.Format(_T("%.2d"),m_iHour);
						if(m_pbtnHour)
							m_pbtnHour->RefreshText(strDate);

						DateTimeChanged();
					}
					else
						SetNextButtonFocused();
				}
				break;
			case IDC_BTN_SETUP_MINUTE:
				{
					if(GetBtnState(IDC_BTN_SETUP_MINUTE)==BS_DOWN)
					{
						if(m_iMinute<59)
							m_iMinute++;
						else
							m_iMinute=0;

						CStringW strDate;

						strDate.Format(_T("%.2d"),m_iMinute);
						if(m_pbtnMinute)
							m_pbtnMinute->RefreshText(strDate);

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
			/*m_wndCalendar.PostMessage(WM_KEYDOWN,VK_RIGHT,0);
			return 1;*/
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


// **************************************************************************
// 
// **************************************************************************
void CSubViewDateTime::DateTimeChanged()
{
	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}

	if(m_bDateTimeChanged==true)
		return;

	m_bDateTimeChanged=true;

	if(m_pbtnOk)
		m_pbtnOk->SetBitmaps(m_pcOkRed_Up,m_pcOkRed_Dw,m_pcOkRed_Up,m_pcOkRed_Up);
}