// SubViewCalOxySensor.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SubViewCalOxySensor.h"
#include "globDefs.h"

/**********************************************************************************************//**
 * A macro that defines color txtbtnup
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNUP				0x00000000

/**********************************************************************************************//**
 * A macro that defines color txtbtndw
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNDW				0x00FFFFFF

/**********************************************************************************************//**
 * A macro that defines color txtbtnfc
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

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
extern HFONT g_hf14AcuMed;
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
extern HFONT g_hf70BoldNum;

extern CEvent g_eventCheckSensor;

/**********************************************************************************************//**
 * CSubViewCalOxySensor
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CSubViewCalOxySensor, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CSubViewCalOxySensor class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CSubViewCalOxySensor::CSubViewCalOxySensor()
{
	InitializeCriticalSection(&csOxy);

	m_pModel=NULL;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;

	m_hdcStatic=NULL;
	m_hdcTmp=NULL;
	m_hbmpStatic=NULL;
	m_hbmpTmp=NULL;

	m_pszTextO2SensorBot[0]=0x0000;
	m_pszTextO2SensorTop[0]=0x0000;

	m_eOxyCalState=OS_UNKNOWN;
	m_iO2Value=0;
	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;

	m_pcRegionTopLeCorner=NULL;
	m_pcRegionTopRiCorner=NULL;
	m_pcRegionBotLeCorner=NULL;
	m_pcRegionBotRiCorner=NULL;

	m_pcExclamation=NULL;
	m_pcOk=NULL;
	m_pcWait=NULL;
	m_pcWait1= NULL;
	m_pcWait2= NULL;
	m_pcWait3= NULL;
	m_pcWait4= NULL;
	m_pcWait5= NULL;
	m_pcWait6= NULL;
	m_pcWait7= NULL;
	m_pcWait8= NULL;
	m_iWaitCount=0;

	m_bCalRunning=false;
	//m_bAutoCalRunning=false;

	m_szLast21Cal=_T("");
	m_szLast100Cal=_T("");

	m_pcMenu_Dis=NULL;
	m_pcMenu_Up=NULL;
	m_pcMenu_Dw=NULL;


	m_bO21Cal=false;
	m_bO100Cal=false;
	m_bCheckActive=false;
	//m_bCalActive=false;

	m_bDoO21Check=false;
	m_bDoO100Check=false;

	m_bDoO21Cal=false;
	m_bDoO100Cal=false;

	m_bExit=false;

	//m_pcwtCheckO21=NULL;
	//m_pcwtCheckO100=NULL;

	m_pcO21Cal=NULL;
	m_pcO100Cal=NULL;
	m_pcOxyOnOff=NULL;

	m_pcwtCheckOxyThread=NULL;
	m_hThreadCheckOxy=INVALID_HANDLE_VALUE;
	m_bDoCheckThread=false;

	m_bCheckO21=false;
	m_bCheckO100=false;

	m_bTwoLines=false;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CSubViewCalOxySensor class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CSubViewCalOxySensor::~CSubViewCalOxySensor()
{
	delete m_pcExclamation;
	m_pcExclamation=NULL;

	delete m_pcOk;
	m_pcOk=NULL;

	delete m_pcRegionTopLeCorner;
	m_pcRegionTopLeCorner=NULL;
	delete m_pcRegionTopRiCorner;
	m_pcRegionTopRiCorner=NULL;
	delete m_pcRegionBotLeCorner;
	m_pcRegionBotLeCorner=NULL;
	delete m_pcRegionBotRiCorner;
	m_pcRegionBotRiCorner=NULL;

	delete m_pcWait;
	m_pcWait=NULL;
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

	delete m_pcMenu_Up;
	m_pcMenu_Up=NULL;
	delete m_pcMenu_Dw;
	m_pcMenu_Dw=NULL;
	delete m_pcMenu_Dis;
	m_pcMenu_Dis=NULL;

	delete m_pcO21Cal;
	m_pcO21Cal=NULL;
	delete m_pcO100Cal;
	m_pcO100Cal=NULL;
	delete m_pcOxyOnOff;
	m_pcOxyOnOff=NULL;

	DeleteCriticalSection(&csOxy);
}


BEGIN_MESSAGE_MAP(CSubViewCalOxySensor, CWnd)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CSubViewCalOxySensor message handlers
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CSubViewCalOxySensor::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Creates a window
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	rc		  	The rectangle.
 * \param 		  	nID		  	The identifier.
 * \param [in,out]	pContext  	If non-null, the context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CSubViewCalOxySensor::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
{
	if (!Create(pParentWnd, rc, nID, pContext))
	{
		return false;
	}

	return true;
}

/**********************************************************************************************//**
 * Creates the Window instance that will be represented by this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	rc		  	The rectangle.
 * \param 		  	nID		  	The identifier.
 * \param [in,out]	pContext  	If non-null, the context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CSubViewCalOxySensor::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

		//CBrush cbrBack(RGB(230,230,255));
		CBrush cbrBack(BACKGND);
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));
		Rectangle(m_hDC, 0, 0, m_lX, m_lY);


		m_pcRegionTopLeCorner		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_SETUP_LE_TOP_CORNER);
		m_pcRegionTopRiCorner		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_SETUP_RI_TOP_CORNER);
		m_pcRegionBotLeCorner		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_SETUP_LE_BOT_CORNER);
		m_pcRegionBotRiCorner		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_SETUP_RI_BOT_CORNER);

		//CBrush cbrBack(RGB(0,0,200));
		CBrush cbrFill;
		cbrFill.CreateSolidBrush(RGB(181,178,181));

		SelectObject(m_hDC,cbrFill);
		Rectangle(m_hDC, 210, 30, 595, 465);
		Rectangle(m_hDC, 230, 10, 575, 30);
		Rectangle(m_hDC, 230, 440, 575, 485);
		if(m_pcRegionTopLeCorner)
			m_pcRegionTopLeCorner->Draw(m_hDC,210,10);
		if(m_pcRegionTopRiCorner)
			m_pcRegionTopRiCorner->Draw(m_hDC,575,10);
		if(m_pcRegionBotLeCorner)
			m_pcRegionBotLeCorner->Draw(m_hDC,210,465);
		if(m_pcRegionBotRiCorner)
			m_pcRegionBotRiCorner->Draw(m_hDC,575,465);

		//grey area for percentage oxy
		CBrush cbrRound(RGB(200,200,200));
		SelectObject(m_hDC,cbrRound);
		RoundRect(m_hDC, 220, 20, 585, 120,20,20);

		//grey area for buttons
		RoundRect(m_hDC, 220, 260, 397, 400,20,20);
		RoundRect(m_hDC, 408, 260, 585, 400,20,20);

		//yellow area for last calibration date
		CBrush cbrInfo(RGB(252,252,170));
		SelectObject(m_hDC,cbrInfo);
		RoundRect(m_hDC, 220, 420, 585, 480,20,20);


		m_hdcTmp = CreateCompatibleDC(m_hDC);
		m_hbmpTmp = CreateCompatibleBitmap(m_hDC,m_lX,m_lY);
		SelectObject(m_hdcTmp, m_hbmpTmp);

		m_hdcStatic = CreateCompatibleDC(m_hDC);
		m_hbmpStatic = CreateCompatibleBitmap(m_hDC, m_lX, m_lY);
		SelectObject(m_hdcStatic, m_hbmpStatic);

		BitBlt(m_hdcTmp,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
		BitBlt(m_hdcStatic,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);



		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();//rkuNEWFIX


		return 1;
	}
	else
		return 0;
}

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewCalOxySensor::Init()
{
	CClientDC dc(this);

	m_pcMenu_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN160_LG_UP);
	m_pcMenu_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN160_LG_DW);
	m_pcMenu_Dis		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN160_LG_DIS);

	m_pcExclamation = new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_EXCLAMATION);
	m_pcOk = new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_OK_UP);
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

	btn.wID					= IDC_BTN_CALOXY_ONOFF;	
	btn.poPosition.x		= 228;//620;
	btn.poPosition.y		= 306;//180;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcOxyOnOff=new CPushBtn(btn,COLOR_TXTBTNUP,true);
	m_pcOxyOnOff->Create(this,g_hf10AcuBold,0);
	if(getModel()->getDATAHANDLER()->GetO2SensorState()==OXYSENS_ON)
	{
		m_pcOxyOnOff->SetText(getModel()->GetLanguageString(IDS_MENU_CAL5),getModel()->GetLanguageString(IDS_MENU_CAL6));
	}
	else
	{
		m_pcOxyOnOff->SetText(getModel()->GetLanguageString(IDS_MENU_CAL7),getModel()->GetLanguageString(IDS_MENU_CAL8));
	}
	m_pcOxyOnOff->ShowWindow(SW_HIDE);

	btn.wID					= IDC_BTN_CALOXY_CAL21;	
	btn.poPosition.x		= 416;//620;
	btn.poPosition.y		= 274;//30;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcO21Cal=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcO21Cal->Create(this,g_hf14AcuMed,0);
	m_pcO21Cal->SetText(_T("21%"));
	m_pcO21Cal->ShowWindow(SW_HIDE);


	btn.wID					= IDC_BTN_CALOXY_CAL100;	
	btn.poPosition.x		= 416;//620;
	btn.poPosition.y		= 336;//90;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcO100Cal=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcO100Cal->Create(this,g_hf14AcuMed,0);
	m_pcO100Cal->SetText(_T("21 + 100%"));
	m_pcO100Cal->ShowWindow(SW_HIDE);


	if(getModel()->getDATAHANDLER()->GetOxyCalRunning()==true)
	{
		CString szBot=_T("");
		m_iWaitCount=1;
		m_bCalRunning=true;
		m_eOxyCalState=OS_CALIBRATING;

		SetO2SensorText(getModel()->GetLanguageString(IDS_TXT_CALRUNNING),szBot,false);
		EnableOnOFF(FALSE);
	}
	else
	{
		CheckOxySensorState();
	}

	CStringW szTimeLast21Cal = getModel()->getCONFIG()->GetLastCalOxy21();
	CStringW szTimeLast100Cal = getModel()->getCONFIG()->GetLastCalOxy100();
	COleDateTime dtTimeLast21Cal;
	COleDateTime dtTimeLast100Cal;

	if(szTimeLast21Cal == _T(""))
	{
		m_szLast21Cal=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);
	}
	else
	{
		if(false==dtTimeLast21Cal.ParseDateTime(szTimeLast21Cal,LOCALE_NOUSEROVERRIDE))
		{
			//m_szLast21Cal=_T("- error -");
			m_szLast21Cal=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);
		}
		else
		{
			m_szLast21Cal.Format(_T("%02d.%02d.%04d %02d:%02d:%02d"),
				dtTimeLast21Cal.GetDay(),
				dtTimeLast21Cal.GetMonth(),
				dtTimeLast21Cal.GetYear(),
				dtTimeLast21Cal.GetHour(),
				dtTimeLast21Cal.GetMinute(),
				dtTimeLast21Cal.GetSecond());
		}
	}

	if(szTimeLast100Cal == _T(""))
	{
		m_szLast100Cal=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);
	}
	else
	{
		if(false==dtTimeLast100Cal.ParseDateTime(szTimeLast100Cal,LOCALE_NOUSEROVERRIDE))
		{
			m_szLast100Cal=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);
		}
		else
		{
			m_szLast100Cal.Format(_T("%02d.%02d.%04d %02d:%02d:%02d"),
				dtTimeLast100Cal.GetDay(),
				dtTimeLast100Cal.GetMonth(),
				dtTimeLast100Cal.GetYear(),
				dtTimeLast100Cal.GetHour(),
				dtTimeLast100Cal.GetMinute(),
				dtTimeLast100Cal.GetSecond());
		}
	}

	int iVal=getModel()->getSPI()->Read_OXY_ACTUAL();
	SetO2SensorValue(iVal);

	DrawO2Sensor(true,true);

	if(getModel()->getDATAHANDLER()->IsFlowSensorCalibrating()==true)//flow cal running
		ShowOxyBtns(true);
	else
		ShowOxyBtns(false);

	StartCheckOxyThread();

	SetTimer(OXYSENS_VALUE, 150, NULL);

}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewCalOxySensor::OnPaint()
{
	CPaintDC dc(this);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewCalOxySensor::OnDestroy()
{
	m_bExit=true;
	StopCheckOxyThread();
	KillTimer(OXYSENS_VALUE);

	if(m_pcwtCheckOxyThread!=NULL)
	{
		delete m_pcwtCheckOxyThread;
		m_pcwtCheckOxyThread=NULL;

		if(m_hThreadCheckOxy!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadCheckOxy);
			m_hThreadCheckOxy=INVALID_HANDLE_VALUE;
		}
	}

	// **********************************************************************
	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);

	if(m_hdcStatic)
		DeleteDC(m_hdcStatic);

	if(m_hdcTmp)
		DeleteDC(m_hdcTmp);

	if(m_hbmpStatic)
		DeleteObject(m_hbmpStatic);

	if(m_hbmpTmp)
		DeleteObject(m_hbmpTmp);
}

/**********************************************************************************************//**
 * Shows
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CSubViewCalOxySensor::Show(BOOL bShow) 
{
	if(this->IsWindowVisible() == bShow)
	{
		return;
	}

	if(bShow)
	{
		ShowWindow(SW_SHOW);
		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
	}
	else
	{
		ShowWindow(SW_HIDE);
		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
	}
}

/**********************************************************************************************//**
 * Sets o2 sensor text
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pszTextTop	The text top.
 * \param [in,out]	pszTextBot	The text bottom.
 * \param 		  	bTwoLines 	True to two lines.
 **************************************************************************************************/

void CSubViewCalOxySensor::SetO2SensorText(CStringW& pszTextTop,CStringW& pszTextBot,bool bTwoLines)
{
	EnterCriticalSection(&csOxy);
	_tcscpy_s(m_pszTextO2SensorTop,_countof(m_pszTextO2SensorTop),pszTextTop);
	_tcscpy_s(m_pszTextO2SensorBot,_countof(m_pszTextO2SensorBot),pszTextBot);
	m_bTwoLines=bTwoLines;
	LeaveCriticalSection(&csOxy);
}

/**********************************************************************************************//**
 * Sets o2 sensor value
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	val	The value.
 **************************************************************************************************/

void CSubViewCalOxySensor::SetO2SensorValue(int val)
{
	EnterCriticalSection(&csOxy);
	m_iO2Value = val;
	LeaveCriticalSection(&csOxy);
}

/**********************************************************************************************//**
 * Refresh o2 sensor value
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	val	The value.
 **************************************************************************************************/

void CSubViewCalOxySensor::RefreshO2SensorValue(int val)
{
	EnterCriticalSection(&csOxy);
	m_iO2Value = val;
	LeaveCriticalSection(&csOxy);
	
	DrawO2Sensor(false,true);
	
}

/**********************************************************************************************//**
 * Draw o2 sensor
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bStatic	True to static.
 * \param	bText  	True to text.
 **************************************************************************************************/

void CSubViewCalOxySensor::DrawO2Sensor(bool bStatic, bool bText)
{
	EnterCriticalSection(&csOxy);
	CClientDC dc(this);

	if(bStatic)
		DrawStatic();

	//BitBlt(m_hDC, 0, 0, m_lX, m_lY, m_hdcStatic, 0, 0, SRCCOPY);
	BitBlt(m_hdcTmp, 0, 0, m_lX, m_lY,m_hdcStatic , 0, 0, SRCCOPY);

	

	if(bText)
		DrawCalText();

	CDC* pDCTemp=CDC::FromHandle(m_hdcTmp);

	HFONT hPrevFont=(HFONT)SelectObject(m_hdcTmp,g_hf70BoldNum);
	int nBkMode=SetBkMode(m_hdcTmp,TRANSPARENT);
	//int nTxtColor=m_btncr.crTxtUp;

	int nPrevTxtColor=SetTextColor(m_hdcTmp,RGB(239,181,0));
	//int nPrevTxtColor=SetTextColor(m_hdcTmp,RGB(239,181,0));

	RECT rc;
	TCHAR psz[MAX_PATH];

	rc.top = 20;
	rc.bottom = 120;
	rc.left = 220; 
	rc.right = 585;

	if(m_iO2Value==-1)
	{
		wsprintf(psz,_T("--"));
	}
	else
	{
		//wsprintf(psz,_T("%0.1f"),(double)m_iO2Value/10);
		//wsprintf(psz,_T("%0.0f"), CTlsFloat::Round(((double)m_iO2Value)/10, 0));
		wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)m_iO2Value)/10, 1));
	}
	
	pDCTemp->DrawText(psz,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
	
	HPEN hpenprev=(HPEN)SelectObject(m_hdcTmp,(HPEN)GetStockObject(NULL_PEN));

	if(m_bCalRunning/* || m_bAutoCalRunning*/)
	{
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

		if(m_pcWait)
			m_pcWait->Draw(m_hdcTmp,250,176);
	}
	else
	{
		if(m_eOxyCalState==OS_OK || m_eOxyCalState==OS_21CALIBRATED || m_eOxyCalState==OS_100CALIBRATED /*|| m_eOxyCalState==OS_CALIBRATED*/)
		{
			if(m_pcOk)
				m_pcOk->Draw(m_hdcTmp,250,168);
		}
		else
		{
			if(m_pcExclamation)
				m_pcExclamation->Draw(m_hdcTmp,250,162);
		}
	}

	BitBlt(dc.m_hDC, 0, 0, m_lX, m_lY, m_hdcTmp, 0, 0, SRCCOPY);
	BitBlt(m_hDC, 0, 0, m_lX, m_lY, m_hdcTmp, 0, 0, SRCCOPY);

	SelectObject(m_hdcTmp,hpenprev);
	SetTextColor(m_hdcTmp,nPrevTxtColor);
	SetBkMode(m_hdcTmp,nBkMode);
	SelectObject(m_hdcTmp,hPrevFont);

	LeaveCriticalSection(&csOxy);
}

/**********************************************************************************************//**
 * Draw static
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewCalOxySensor::DrawStatic(/*CDC* pDC*/)
{
	CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);

	HFONT hPrevFont=(HFONT)SelectObject(m_hdcStatic,g_hf25AcuMed);
	int nBkMode=SetBkMode(m_hdcStatic,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hdcStatic,RGB(0,0,0));

	RECT rc;
	//memcpy(&rc,&m_rcClient,sizeof(RECT));

	/*rc.top = 20;
	rc.bottom = m_lY;
	rc.left = 20; 
	rc.right = m_lX;

	pDCStatic->DrawText(getModel()->GetLanguageString(IDS_TXT_O2SENSOR),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);*/

	//SelectObject(m_hdcStatic,g_hf13AcuBold);
	
	
	rc.top = 20;
	rc.bottom = 120;
	rc.left = 490; 
	rc.right = 585;

	SelectObject(m_hdcStatic,g_hf43AcuBold);
	SetTextColor(m_hdcStatic,RGB(0,0,0));

	pDCStatic->DrawText(_T("%"),&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

		

	SelectObject(m_hdcStatic,hPrevFont);
	SetBkMode(m_hdcStatic,nBkMode);
	SetTextColor(m_hdcStatic,nPrevTxtColor);
}

/**********************************************************************************************//**
 * Draw calibration text
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewCalOxySensor::DrawCalText()
{
	CDC* pDCTemp=CDC::FromHandle(m_hdcTmp);

	HFONT hPrevFont=(HFONT)SelectObject(m_hdcTmp,g_hf19AcuMed);
	int nBkMode=SetBkMode(m_hdcTmp,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hdcTmp,RGB(239,181,0));

	if(		m_bCalRunning 
		//||	m_bAutoCalRunning
		||	m_eOxyCalState==OS_UNKNOWN
		||	m_eOxyCalState==OS_OFF
		||	m_eOxyCalState==OS_DEFECT
		||	m_eOxyCalState==OS_REPLACE
		||	m_eOxyCalState==OS_NOT_CALIBRATED
		||	m_eOxyCalState==OS_NOT_CALIBRATED_AIR
		||	m_eOxyCalState==OS_NOT_CALIBRATED_OXY
		||	m_eOxyCalState==OS_CHECKING
		||	m_eOxyCalState==OS_CAL_CNCL
		//||	m_eOxyCalState==OS_AUTOCALIBRATING
		||	m_eOxyCalState==OS_CALIBRATING)
	{
		pDCTemp->SetTextColor(RGB(255,0,0));
	}
	else
		pDCTemp->SetTextColor(RGB(0,255,0));

	RECT rc;
	//memcpy(&rc,&m_rcClient,sizeof(RECT));

	//test
	//m_bTwoLines=true;
	//_tcscpy_s(m_pszTextO2SensorTop,_countof(m_pszTextO2SensorTop),_T("Top line"));
	//_tcscpy_s(m_pszTextO2SensorBot,_countof(m_pszTextO2SensorBot),_T("Bottom line"));


	rc.top = 160;
	rc.bottom = 230;
	rc.left = 320;//220; 
	rc.right = 585;

	if(m_bTwoLines)
	{
		pDCTemp->DrawText(m_pszTextO2SensorTop,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		pDCTemp->DrawText(m_pszTextO2SensorBot,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
	}
	else
	{
		pDCTemp->DrawText(m_pszTextO2SensorTop,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
	}

	SelectObject(m_hdcTmp,g_hf8AcuBold);
	SetTextColor(m_hdcTmp,RGB(0,0,0));

	rc.top = 430;
	rc.bottom = 470;
	rc.left = 235; 
	rc.right = 585;

	CStringW sz=getModel()->GetLanguageString(IDS_TXT_LAST21CAL);
	//CStringW sz=_T("Last 21% calibration: ");
	sz+=_T(" ");
	sz+=m_szLast21Cal;
	pDCTemp->DrawText(sz,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 430;
	rc.bottom = 470;
	rc.left = 235; 
	rc.right = 585;

	sz=getModel()->GetLanguageString(IDS_TXT_LAST100CAL);
	//sz=_T("Last 100% calibration:");
	sz+=_T(" ");
	sz+=m_szLast100Cal;
	pDCTemp->DrawText(sz,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);


	SelectObject(m_hdcTmp,hPrevFont);
	SetBkMode(m_hdcTmp,nBkMode);
	SetTextColor(m_hdcTmp,nPrevTxtColor);
	//pDC->SetTextAlign (iPrevTextAlign);

	/*SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);*/
}

/**********************************************************************************************//**
 * Sets calibration 21 ok
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bCalibrated	True if calibrated.
 **************************************************************************************************/

void CSubViewCalOxySensor::SetCalibration21Ok(bool bCalibrated)
{
	EnterCriticalSection(&csOxy);
	if(bCalibrated)
		m_eOxyCalState=OS_21CALIBRATED;
	else
		m_eOxyCalState=OS_OK;

	SYSTEMTIME st;
	GetLocalTime(&st);
	COleDateTime dtTimeLast21Cal(st);

	CStringW szTimeLast21Cal = dtTimeLast21Cal.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);

	getModel()->getCONFIG()->SetLastCalOxy21(szTimeLast21Cal);

	m_szLast21Cal=szTimeLast21Cal;
	LeaveCriticalSection(&csOxy);

	DrawO2Sensor(false,true);
}

/**********************************************************************************************//**
 * Sets calibration 100 ok
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bCalibrated	True if calibrated.
 **************************************************************************************************/

void CSubViewCalOxySensor::SetCalibration100Ok(bool bCalibrated)
{
	EnterCriticalSection(&csOxy);
	if(bCalibrated)
		m_eOxyCalState=OS_100CALIBRATED;
	else
		m_eOxyCalState=OS_OK;

	SYSTEMTIME st;
	GetLocalTime(&st);
	COleDateTime dtTimeLast100Cal(st);

	CStringW szTimeLast100Cal = dtTimeLast100Cal.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);

	getModel()->getCONFIG()->SetLastCalOxy21(szTimeLast100Cal);

	m_szLast100Cal=szTimeLast100Cal;

	LeaveCriticalSection(&csOxy);

	DrawO2Sensor(false,true);
}

/**********************************************************************************************//**
 * Sets calibration state
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	state  	The state.
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CSubViewCalOxySensor::SetCalState(eOxySensCalibrationstate state,bool bRedraw)
{
	EnterCriticalSection(&csOxy);
	m_eOxyCalState=state;
	getModel()->getDATAHANDLER()->SetOxySensorCalState(state);

	bool bTwoLines=false;
	CStringW csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);
	CStringW csSensorTextBot=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);

	switch(m_eOxyCalState)
	{
	case OS_OK:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_SENSORON);
		}
		break;
	case OS_OFF:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_SENSOROFF);
		}
		break;
	/*case OS_AUTOCALIBRATING:
		{
			csSensorText=getModel()->GetLanguageString(IDS_TXT_O2CAL);
		}
		break;*/
	case OS_CALIBRATING:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_CALRUNNING);
		}
		break;
	case OS_DEFECT:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_DEFECTSENS_TOP);
			csSensorTextTop+=_T(" ");
			csSensorTextTop+=getModel()->GetLanguageString(IDS_TXT_DEFECTSENS_BOT);
		}
		break;
	case OS_REPLACE:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_REPLACESOON);
		}
		break;
	case OS_CAL_CNCL:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_CALCNCL);
		}
		break;
	case OS_21CALIBRATED:
		{
			csSensorTextTop=_T("21% ");
			csSensorTextTop+=getModel()->GetLanguageString(IDS_TXT_CALIBRATED);
		}
		break;
	case OS_100CALIBRATED:
		{
			csSensorTextTop=_T("100% ");
			csSensorTextTop+=getModel()->GetLanguageString(IDS_TXT_CALIBRATED);
		}
		break;
	/*case OS_CALIBRATED:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_CALIBRATED);
		}
		break;*/
	case OS_NOT_CALIBRATED:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_NOTCALIBRATED);
		}
		break;
	case OS_NOT_CALIBRATED_AIR:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_NOTCALIBRATED);
			csSensorTextBot=getModel()->GetLanguageString(IDS_ALARM_PAIRIN);
			bTwoLines=true;
		}
		break;
	case OS_NOT_CALIBRATED_OXY:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_NOTCALIBRATED);
			csSensorTextBot=getModel()->GetLanguageString(IDS_ALARM_PO2IN);
			bTwoLines=true;
		}
		break;
	case OS_CHECKING:
		{
			csSensorTextTop=getModel()->GetLanguageString(IDS_TXT_CHCKSENSOR);
		}
		break;
	case OS_UNKNOWN:
	default:
		break;
	}
	LeaveCriticalSection(&csOxy);

	SetO2SensorText(csSensorTextTop,csSensorTextBot,bTwoLines);

	if(bRedraw)
	{
		DrawO2Sensor(false,true);
	}
}

/**********************************************************************************************//**
 * Check oxy sensor state
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bRedraw			True to redraw.
 * \param	bCalibrating	True to calibrating.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CSubViewCalOxySensor::CheckOxySensorState(bool bRedraw, bool bCalibrating)
{
	bool bRes=false;
	eOxySensCalibrationstate eOxyCalState=OS_CHECKING;

	if(getModel()->getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF)
	{
		eOxyCalState=OS_OFF;
	}
	else if(getModel()->getDATAHANDLER()->GetOxyCalRunning()==true)
	{
		/*if(bCalibrating)
		{
			int iState=getModel()->getSPI()->Read_STATUS_2();

			if(iState & BIT0)
			{
				eOxyCalState=OS_REPLACE;
			}
			else if(iState & BIT1)
			{
				eOxyCalState=OS_DEFECT;
			}
			else if(iState & BIT2)
			{
				if(iState & BIT4)
				{
					eOxyCalState=OS_NOT_CALIBRATED_AIR;
				}
				else if(iState & BIT5)
				{
					eOxyCalState=OS_NOT_CALIBRATED_OXY;
				}
				else
				{
					eOxyCalState=OS_NOT_CALIBRATED;
				}
			}
			else
			{
				bRes=true;
				if(bCalibrating)
				{
					eOxyCalState=OS_CALIBRATED;
				}
				else
				{
					eOxyCalState=OS_OK;
				}
			}
		}
		else*/
		{
			eOxyCalState=OS_CALIBRATING;
			DEBUGMSG(TRUE, (TEXT("OS_CALIBRATING\r\n")));
		}
	}
	else
	{
		int iState=getModel()->getSPI()->Read_STATUS_2();

		DEBUGMSG(TRUE, (TEXT("Read_STATUS_2\r\n")));
		if(iState & BIT0)
		{
			eOxyCalState=OS_REPLACE;
		}
		else if(iState & BIT1)
		{
			eOxyCalState=OS_DEFECT;
		}
		else if(iState & BIT2)
		{
			if(iState & BIT4)
			{
				eOxyCalState=OS_NOT_CALIBRATED_AIR;
			}
			else if(iState & BIT5)
			{
				eOxyCalState=OS_NOT_CALIBRATED_OXY;
			}
			else
			{
				eOxyCalState=OS_NOT_CALIBRATED;
			}
		}
		else
		{
			bRes=true;
			/*if(bCalibrating)
			{
				eOxyCalState=OS_CALIBRATED;
			}
			else*/
			{
				eOxyCalState=OS_OK;
			}
		}
	}



	SetCalState(eOxyCalState,bRedraw);
	

	return bRes;
}

/**********************************************************************************************//**
 * Sets calibration running
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CSubViewCalOxySensor::SetCalRunning(eOxySensorCalRunState state)
{
	EnterCriticalSection(&csOxy);
	switch(state)
	{
	case OXYCAL_RUNNING:
		{
			if(m_bCalRunning==false)
			{
				
				m_iWaitCount=1;
				m_bCalRunning=true;

			}
			
			//m_bAutoCalRunning=false;
		}
		break;
	/*case OXYCAL_AUTO:
		{
			m_iWaitCount=1;
			m_bCalRunning=false;
			m_bAutoCalRunning=true;
		}
		break;*/
	case OXYCAL_OFF:
	default:
		{
			m_iWaitCount=0;
			m_bCalRunning=false;

		}
		break;
	}

	LeaveCriticalSection(&csOxy);

}

/**********************************************************************************************//**
 * Executes the timer action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	nIDEvent	The identifier event.
 **************************************************************************************************/

void CSubViewCalOxySensor::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==OXYSENS_VALUE)
	{
		EnterCriticalSection(&csOxy);
		if(m_iWaitCount>0)
		{
			m_iWaitCount++;
			if(m_iWaitCount>8)
				m_iWaitCount=1;

		}
		LeaveCriticalSection(&csOxy);

		int iVal=0;

		if(m_bCalRunning)
		{
			iVal=getModel()->getSPI()->Read_OXY_DEFAULT();
			SetO2SensorValue(iVal);
		}
		else
		{
			iVal=getModel()->getDATAHANDLER()->getMessureDataO2();
			SetO2SensorValue(iVal);
		}

		DrawO2Sensor(false,true);
	}

	CWnd::OnTimer(nIDEvent);
}

/**********************************************************************************************//**
 * Updates the calibration time described by bRedraw
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CSubViewCalOxySensor::UpdateCalTime(bool bRedraw)
{
	EnterCriticalSection(&csOxy);
	CStringW szTimeLast21Cal = getModel()->getCONFIG()->GetLastCalOxy21();
	CStringW szTimeLast100Cal = getModel()->getCONFIG()->GetLastCalOxy100();
	COleDateTime dtTimeLast21Cal;
	COleDateTime dtTimeLast100Cal;

	if(szTimeLast21Cal == _T(""))
	{
		m_szLast21Cal=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);
	}
	else
	{
		if(false==dtTimeLast21Cal.ParseDateTime(szTimeLast21Cal,LOCALE_NOUSEROVERRIDE))
		{
			//m_szLast21Cal=_T("- error -");
			m_szLast21Cal=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);
		}
		else
		{
			m_szLast21Cal.Format(_T("%02d.%02d.%04d %02d:%02d:%02d"),
				dtTimeLast21Cal.GetDay(),
				dtTimeLast21Cal.GetMonth(),
				dtTimeLast21Cal.GetYear(),
				dtTimeLast21Cal.GetHour(),
				dtTimeLast21Cal.GetMinute(),
				dtTimeLast21Cal.GetSecond());
		}
	}

	if(szTimeLast100Cal == _T(""))
	{
		m_szLast100Cal=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);
	}
	else
	{
		if(false==dtTimeLast100Cal.ParseDateTime(szTimeLast100Cal,LOCALE_NOUSEROVERRIDE))
		{
			m_szLast100Cal=getModel()->GetLanguageString(IDS_TXT_UNKNOWN);
		}
		else
		{
			m_szLast100Cal.Format(_T("%02d.%02d.%04d %02d:%02d:%02d"),
				dtTimeLast100Cal.GetDay(),
				dtTimeLast100Cal.GetMonth(),
				dtTimeLast100Cal.GetYear(),
				dtTimeLast100Cal.GetHour(),
				dtTimeLast100Cal.GetMinute(),
				dtTimeLast100Cal.GetSecond());
		}
	}

	LeaveCriticalSection(&csOxy);

	if(bRedraw)
	{
		DrawO2Sensor(false,true);
	}
}

/**********************************************************************************************//**
 * Window proc
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	message	The message.
 * \param	wParam 	The wParam field of the message.
 * \param	lParam 	The lParam field of the message.
 *
 * \return	A LRESULT.
 **************************************************************************************************/

LRESULT CSubViewCalOxySensor::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_CHECKO21_DONE:
		{
			if(m_bO21Cal)
			{
				m_bO21Cal=false;

				getModel()->getDATAHANDLER()->SetO21SensorCalState(true);
				getModel()->Send_MODE_OPTION1(false,true);

				SetCalState(OS_CALIBRATING,true);

				if(AfxGetApp())
				{
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
					AfxGetApp()->GetMainWnd()->PostMessage(WM_OXICAL_START21);
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_CLEAR_TEXT);
				}
				
			}
			else if(!m_bExit)
			{
				SetCalRunning(OXYCAL_OFF);
				SetCalState(OS_CAL_CNCL,true);

				if(GetParent())
				{
					GetParent()->PostMessage(WM_SET_SETUPTIMER);
					GetParent()->PostMessage(WM_OXYCAL_STOP);
				}
				EnableOnOFF(TRUE);

				getModel()->getDATAHANDLER()->SetO21SensorCalState(false);
				getModel()->Send_MODE_OPTION1();

			}
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_CLEAR_TEXT);

			m_bDoO21Check=false;
			m_bCheckActive=false;

			return 1;
		}
		break;
	case WM_CHECKO100_DONE:
		{
			if(m_bO100Cal)
			{
				m_bO100Cal=false;

				getModel()->getDATAHANDLER()->SetO100SensorCalState(true);
				getModel()->Send_MODE_OPTION1(false,true);

				SetCalState(OS_CALIBRATING,true);

				if(AfxGetApp())
				{
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
					AfxGetApp()->GetMainWnd()->PostMessage(WM_OXICAL_START100);
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_CLEAR_TEXT);
				}
				
			}
			else if(!m_bExit)
			{
				SetCalRunning(OXYCAL_OFF);
				SetCalState(OS_CAL_CNCL,true);

				if(GetParent())
				{
					GetParent()->PostMessage(WM_SET_SETUPTIMER);
					GetParent()->PostMessage(WM_OXYCAL_STOP);
				}

				getModel()->getDATAHANDLER()->SetO100SensorCalState(false);
				getModel()->Send_MODE_OPTION1();
			}
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_CLEAR_TEXT);

			m_bDoO100Check=false;
			m_bCheckActive=false;

			return 1;
		}
		break;
	case WM_ALARMSTATE_CHANGED:
		{
			if(getModel()->getDATAHANDLER()->IsFlowSensorCalibrating()==true)//flow cal running
				ShowOxyBtns(true);
			else
				ShowOxyBtns(false);

			return 1;
		}
		break;
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_CALOXY_ONOFF:
				{
					if(getModel()->getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF)
					{
						getModel()->getDATAHANDLER()->SetO2SensorState(OXYSENS_ON);
						theApp.WriteLog(_T("Oxi-Sensor enabled"));
						if(m_pcOxyOnOff)
							m_pcOxyOnOff->RefreshText(getModel()->GetLanguageString(IDS_MENU_CAL5),getModel()->GetLanguageString(IDS_MENU_CAL6));
					}
					else
					{
						getModel()->getDATAHANDLER()->SetO2SensorState(OXYSENS_OFF);
						theApp.WriteLog(_T("Oxi-Sensor disabled"));
						if(m_pcOxyOnOff)
							m_pcOxyOnOff->RefreshText(getModel()->GetLanguageString(IDS_MENU_CAL7),getModel()->GetLanguageString(IDS_MENU_CAL8));

						if(getModel()->getPRICOThread())
						{
							if(getModel()->getPRICOThread()->isPRICOalgorithmRunning())
							{
								//getModel()->getALARMHANDLER()->setPRICOAutoTurnedOff();//rku AUTOPRICO
								getModel()->getDATAHANDLER()->setPRICOoff();

								if(getModel()->getALARMHANDLER()->isPRICOAutoTurnedOff())//rku AUTOPRICO
								{
									getModel()->getALARMHANDLER()->resetPRICOAutoTurnedOff();
								}

								CMVEventInfotext event2(CMVEventInfotext::EV_TIMETEXT,  getModel()->GetLanguageString(IDS_TXT_PRICOOFF), 3000);
								getModel()->triggerEvent(&event2);
								getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
							}
						}
					}


					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGER_O2SENSORSTATE);

					if(getModel()->getDATAHANDLER()->IsFlowSensorCalibrating()==true)//flow cal running
						ShowOxyBtns(true);
					else
						ShowOxyBtns(false);

					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
				break;
			case IDC_BTN_CALOXY_CAL21:
				{
					if(		m_bDoO21Check==false
						&&	getModel()->getDATAHANDLER()->GetOxyCalRunning()==false
						&&	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating()==false)
					{
						StartO21SensorCheckRoutine();
					}
					else
					{
						m_bDoO21Cal=false;
						m_bDoO100Cal=false;
						StopO21SensorCheckRoutine();
					}
				}
				break;
			case IDC_BTN_CALOXY_CAL100:
				{
					if(		m_bDoO100Check==false
						&&	getModel()->getDATAHANDLER()->GetOxyCalRunning()==false
						&&	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating()==false)
					{
						//TRACE(_T("StartO21SensorCheckRoutine 1\n"));
						StartO100SensorCheckRoutine();
					}
					else
					{
						m_bDoO21Cal=false;
						m_bDoO100Cal=false;
						StopO100SensorCheckRoutine();
					}
				}
				break;
			}
			return 1;
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

/**********************************************************************************************//**
 * Enables the on off
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CSubViewCalOxySensor::EnableOnOFF(BOOL state)
{
	if(m_pcOxyOnOff)
		m_pcOxyOnOff->EnableWindow(state);
}

/**********************************************************************************************//**
 * Shows the oxy btns
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	disable	True to disable, false to enable.
 **************************************************************************************************/

void CSubViewCalOxySensor::ShowOxyBtns(bool disable)
{
	if(disable)
	{
		if(m_pcOxyOnOff)
			m_pcOxyOnOff->ShowWindow(SW_SHOW);
		if(m_pcO21Cal)
			m_pcO21Cal->ShowWindow(SW_SHOW);
		if(m_pcO100Cal)
			m_pcO100Cal->ShowWindow(SW_SHOW);

		if(m_pcO21Cal)
			m_pcO21Cal->EnableWindow(FALSE);
		if(m_pcO100Cal)
			m_pcO100Cal->EnableWindow(FALSE);
	}
	else
	{
		if(getModel()->getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF)
		{
			if(m_pcOxyOnOff)
				m_pcOxyOnOff->ShowWindow(SW_SHOW);
			if(m_pcO21Cal)
				m_pcO21Cal->ShowWindow(SW_SHOW);
			if(m_pcO100Cal)
				m_pcO100Cal->ShowWindow(SW_SHOW);
			
			if(m_pcO21Cal)
				m_pcO21Cal->EnableWindow(FALSE);
			if(m_pcO100Cal)
				m_pcO100Cal->EnableWindow(FALSE);
		}
		else
		{
			if(m_pcOxyOnOff)
				m_pcOxyOnOff->ShowWindow(SW_SHOW);
			if(m_pcO21Cal)
				m_pcO21Cal->ShowWindow(SW_SHOW);
			if(m_pcO100Cal)
				m_pcO100Cal->ShowWindow(SW_SHOW);
			
			if(m_pcO21Cal)
				m_pcO21Cal->EnableWindow(TRUE);
			if(m_pcO100Cal)
				m_pcO100Cal->EnableWindow(TRUE);
		}
	}
	

}

/**********************************************************************************************//**
 * Starts o21 sensor check routine
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewCalOxySensor::StartO21SensorCheckRoutine()
{
	if(m_bCheckActive)
	{
		m_bDoO21Check=false;
		m_bDoO100Check=false;
		return;
	}

	m_bCheckActive=true;
	m_bDoO21Check=true;

	if(GetParent())
		GetParent()->PostMessage(WM_KILL_SETUPTIMER);

	m_bCheckO21=true;
	g_eventCheckSensor.SetEvent();
}

/**********************************************************************************************//**
 * Stops o21 sensor check routine
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewCalOxySensor::StopO21SensorCheckRoutine()
{
	//StopO21SensorCheckThread();

	m_bDoO21Check=false;
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_OXICAL_CANCEL);
	
	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
}

/**********************************************************************************************//**
 * Starts o100 sensor check routine
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewCalOxySensor::StartO100SensorCheckRoutine()
{
	if(m_bCheckActive)
	{
		m_bDoO21Check=false;
		m_bDoO100Check=false;
		return;
	}
	m_bCheckActive=true;
	m_bDoO100Check=true;

	if(GetParent())
		GetParent()->PostMessage(WM_KILL_SETUPTIMER);
	m_bCheckO100=true;
	g_eventCheckSensor.SetEvent();
}

/**********************************************************************************************//**
 * Stops o100 sensor check routine
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewCalOxySensor::StopO100SensorCheckRoutine()
{
	//StopO100SensorCheckThread();
	m_bDoO100Check=false;

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_OXICAL_CANCEL);
	
	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
}

/**********************************************************************************************//**
 * Starts check oxy thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewCalOxySensor::StartCheckOxyThread( void )
{
	m_bDoCheckThread=true;

	if(m_pcwtCheckOxyThread!=NULL)
	{
		delete m_pcwtCheckOxyThread;
		m_pcwtCheckOxyThread=NULL;

		if(m_hThreadCheckOxy!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadCheckOxy);
			m_hThreadCheckOxy=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtCheckOxyThread=AfxBeginThread(CCheckOxyThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadCheckOxy=m_pcwtCheckOxyThread->m_hThread;
	m_pcwtCheckOxyThread->m_bAutoDelete = FALSE; 
	m_pcwtCheckOxyThread->ResumeThread();
}

/**********************************************************************************************//**
 * Stops check oxy thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewCalOxySensor::StopCheckOxyThread( void )
{
	if(m_bDoCheckThread)
	{
		m_bDoCheckThread=false;
		m_bCheckO21=false;
		m_bCheckO100=false;

		g_eventCheckSensor.SetEvent();

		if (WaitForSingleObject(m_pcwtCheckOxyThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			theApp.WriteLog(_T("#THR:028a"));
			if(!TerminateThread(m_pcwtCheckOxyThread,0))
			{
				theApp.WriteLog(_T("#THR:028b"));
			}
		}
	}
}

/**********************************************************************************************//**
 * Check oxy thread
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

static UINT CCheckOxyThread( LPVOID pc )
{
	try
	{
		((CSubViewCalOxySensor*)pc)->CheckOxySensor();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CCheckOxyThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CCheckOxyThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CSubViewCalOxySensor*)pc)->CheckOxySensor();
	return TRUE;
}

/**********************************************************************************************//**
 * Check oxy sensor
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CSubViewCalOxySensor::CheckOxySensor()
{
	do
	{
		WaitForSingleObject(g_eventCheckSensor, INFINITE);

		if(!m_bDoCheckThread)
			continue;

		if(m_bCheckO21)
		{
			m_bDoO21Cal=true;
			m_bCheckO21=false;

			if(GetParent())
				GetParent()->PostMessage(WM_OXYCAL_START);
			EnableOnOFF(FALSE);

			SetCalState(OS_CHECKING,true);

			getModel()->getDATAHANDLER()->SetO21SensorCalState(true);
			getModel()->Send_MODE_OPTION1();

			CStringW csO2SensorText=_T("");
			int idefVal=0;

			BYTE iCheckStart=0;

			m_bO21Cal=false;

			SetCalRunning(OXYCAL_RUNNING);

			do
			{	
				if(iCheckStart>20 && m_bDoO21Check)
				{
					m_bO21Cal=true;
				}
				else if(m_bDoO21Check)
				{
					idefVal=getModel()->getSPI()->Read_OXY_DEFAULT();
					RefreshO2SensorValue(idefVal);
				}

				Sleep(100);
				iCheckStart++;

			}while(m_bDoCheckThread && m_bDoO21Check && !m_bO21Cal);

			if(m_bDoO21Cal)
			{
				m_bO21Cal=false;

				getModel()->getDATAHANDLER()->SetO21SensorCalState(true);
				getModel()->Send_MODE_OPTION1(false,true);

				if(!m_bExit)
					SetCalState(OS_CALIBRATING,true);

				if(AfxGetApp())
				{
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
					AfxGetApp()->GetMainWnd()->PostMessage(WM_OXICAL_START21);
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_CLEAR_TEXT);
				}
			}
			else
			{
				getModel()->getDATAHANDLER()->SetO21SensorCalState(false);
				getModel()->Send_MODE_OPTION1();

				if(!m_bExit)
				{
					SetCalRunning(OXYCAL_OFF);
					SetCalState(OS_CAL_CNCL,true);
					if(GetParent())
					{
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
						GetParent()->PostMessage(WM_OXYCAL_STOP);
					}
					EnableOnOFF(TRUE);
				}
			}
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_CLEAR_TEXT);

			m_bDoO21Check=false;
			m_bCheckActive=false;
		}
		else if(m_bCheckO100)
		{
			//############## 21% calibration

			m_bDoO100Cal=true;
			//m_bDoO21Cal=true;
			m_bCheckO100=false;

			if(GetParent())
				GetParent()->PostMessage(WM_OXYCAL_START);
			EnableOnOFF(FALSE);

			SetCalState(OS_CHECKING,true);

			getModel()->getDATAHANDLER()->SetO21SensorCalState(true);
			getModel()->Send_MODE_OPTION1();

			CStringW csO2SensorText=_T("");
			int idefVal=0;

			BYTE iCheckStart=0;

			m_bO100Cal=false;

			SetCalRunning(OXYCAL_RUNNING);

			do
			{	
				if(iCheckStart>20 && m_bDoO100Check)
				{
					m_bO100Cal=true;
				}
				else if(m_bDoO21Check)
				{
					idefVal=getModel()->getSPI()->Read_OXY_DEFAULT();
					RefreshO2SensorValue(idefVal);
				}

				//Sleep(200);
				Sleep(100);
				iCheckStart++;

			}while(m_bDoCheckThread && m_bDoO100Check && !m_bO100Cal);


			if(m_bDoO100Cal)
			{
				m_bO100Cal=false;
				m_bO21Cal=false;

				getModel()->getDATAHANDLER()->SetO21SensorCalState(true);
				getModel()->Send_MODE_OPTION1(false,true);

				if(!m_bExit)
					SetCalState(OS_CALIBRATING,true);

				if(AfxGetApp())
				{
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
					AfxGetApp()->GetMainWnd()->PostMessage(WM_OXICAL_START100);
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_CLEAR_TEXT);
				}

			}
			else
			{
				getModel()->getDATAHANDLER()->SetO21SensorCalState(false);
				getModel()->Send_MODE_OPTION1();

				if(!m_bExit)
				{
					SetCalRunning(OXYCAL_OFF);
					SetCalState(OS_CAL_CNCL,true);
					if(GetParent())
					{
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
						GetParent()->PostMessage(WM_OXYCAL_STOP);
					}
					/*EnableOnOFF(TRUE);*/
				}



			}
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_CLEAR_TEXT);

			m_bDoO100Check=false;
			m_bCheckActive=false;
		}
		m_bCheckActive=false;

	}while(m_bDoCheckThread);

	//theApp.WriteLog(_T("#THR:028"));

	return 0;

}