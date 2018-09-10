// WndDataCO2.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndDataCO2.h"

extern CEvent g_eventCO2Data;

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

/**********************************************************************************************//**
 * CWndDataCO2
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndDataCO2, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndDataCO2 class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndDataCO2::CWndDataCO2()
{
	InitializeCriticalSection(&csDoThread);
	InitializeCriticalSection(&csValues);

	m_pModel=NULL;
	m_pcO2ParaLeft=NULL;
	m_pcO2ParaRight=NULL;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	m_hdcStatic=NULL;
	m_hbmpStatic=NULL;

	

	EnterCriticalSection(&csValues);
	m_dwRemainCO2PumpTime=0;
	if(getModel()->getCONFIG()->GetTimePumpAutoOn()!=0)
	{
		m_bAutoonPump=true;
	}
	else
	{
		m_bAutoonPump=false;
	}
	if(getModel()->getETCO2()!=NULL)
	{
		m_bSystemDateOk=getModel()->getETCO2()->isSystemDateOk();
		m_bCalibrationIsDue=getModel()->getETCO2()->isCalibrationDue();
		m_bServiceIsDue=getModel()->getETCO2()->isServiceDue();
		m_byStateBytes=getModel()->getETCO2()->getStateBytes();
		m_byExtendedStateBytes=getModel()->getETCO2()->getExtendedStateBytes();
		m_bCO2ValueValid=getModel()->getETCO2()->isCO2ValueValid();
		m_bETCO2ValueValid=getModel()->getETCO2()->isETCO2ValueValid();
		m_bFreqValuevalid=getModel()->getETCO2()->isFreqValueValid();
		m_bPumpOn=getModel()->getETCO2()->isPumpOn();
	}
	else
	{
		m_bSystemDateOk=false;
		m_bCalibrationIsDue=false;
		m_bServiceIsDue=false;
		m_byStateBytes=0xFF;
		m_byExtendedStateBytes=0;
		m_bCO2ValueValid=false;
		m_bETCO2ValueValid=false;
		m_bFreqValuevalid=false;
		m_bPumpOn=false;
	}
	m_iETCO2=0;
	m_iRESPRATE=0;
	LeaveCriticalSection(&csValues);
	
	m_byCO2Module=getModel()->getCONFIG()->getCO2module();
	m_csUnit = _T("");
	m_eCO2unit = CO2_UNIT_MMHG;

	m_pcwtCO2DataThread=NULL;
	m_hThreadCO2Data=INVALID_HANDLE_VALUE;
	m_bDoThread=false;

	m_pcLimitHigh_Up=NULL;
	m_pcLimitLow_Up=NULL;

	m_pcResetTime_Up=NULL;
	m_pcResetTime_Dw=NULL;
	m_pcResetTime=NULL;

	

	m_sliderPassword=NULL;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndDataCO2 class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndDataCO2::~CWndDataCO2()
{
	if(m_pcResetTime)
		delete m_pcResetTime;
	m_pcResetTime=NULL;

	if(m_sliderPassword)
		delete m_sliderPassword;
	m_sliderPassword=NULL;

	if(m_pcResetTime_Up)
		delete m_pcResetTime_Up;
	m_pcResetTime_Up=NULL;

	if(m_pcResetTime_Dw)
		delete m_pcResetTime_Dw;
	m_pcResetTime_Dw=NULL;

	if(m_pcLimitHigh_Up)
		delete m_pcLimitHigh_Up;
	m_pcLimitHigh_Up=NULL;

	if(m_pcLimitLow_Up)
		delete m_pcLimitLow_Up;
	m_pcLimitLow_Up=NULL;

	if(m_pcO2ParaLeft)
		delete m_pcO2ParaLeft;
	m_pcO2ParaLeft=NULL;

	if(m_pcO2ParaRight)
		delete m_pcO2ParaRight;
	m_pcO2ParaRight=NULL;

	DeleteCriticalSection(&csDoThread);
	DeleteCriticalSection(&csValues);
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CWndDataCO2::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


BEGIN_MESSAGE_MAP(CWndDataCO2, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
	ON_BN_CLICKED(IDC_BTN_CO2_RESETTIME, &CWndDataCO2::OnBnClickedResetTimer)
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndDataCO2 message handlers
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

BOOL CWndDataCO2::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{
	bool bRes=false;
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD/*|WS_VISIBLE*/,rc,pParentWnd,nID))
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

		m_hdcStatic = CreateCompatibleDC(m_hDC);
		m_hbmpStatic = CreateCompatibleBitmap(m_hDC, m_lX, m_lY);
		SelectObject(m_hdcStatic, m_hbmpStatic);
		HPEN hpenprevStat=(HPEN)SelectObject(m_hdcStatic, (HPEN)GetStockObject(NULL_PEN));	
		HBRUSH hbrprevStat=(HBRUSH)SelectObject(m_hdcStatic,cbrBack); 
		Rectangle(m_hdcStatic, 0, 0, m_lX, m_lY);


		m_pcO2ParaLeft= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CO2PARA_LE);
		m_pcO2ParaRight= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CO2PARA_RI);

		m_pcLimitHigh_Up	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_LIMIT_HIGH);
		m_pcLimitLow_Up		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_LIMIT_LOW);

		m_pcResetTime_Up	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN6_YEL_UP);
		m_pcResetTime_Dw		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN6_YEL_DW);

		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		SelectObject(m_hdcStatic, hpenprevStat);
		SelectObject(m_hdcStatic,hbrprevStat);

		bRes=true;
	}
	else
		bRes=false;

	if(bRes)
		Init();
	

	return bRes;
}

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndDataCO2::Init()
{
	EnterCriticalSection(&csValues);
	if(m_byCO2Module==CO2MODULE_MICROPOD)
	{
		m_sliderPassword = new CBitmapSlider();
		m_sliderPassword->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(200,32,349,77), 
			this,IDC_SLD_CO2PUMP);
		m_sliderPassword->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
		m_sliderPassword->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
		m_sliderPassword->SetRange( 0, 1 );
		
		if(m_bPumpOn==false)
		{
			m_sliderPassword->SetPos( 0 );
		}
		else
		{
			m_dwRemainCO2PumpTime=0;
			m_sliderPassword->SetPos( 1 );
		}
		m_sliderPassword->SetMargin( 5, 0, 6, 0 );
		m_sliderPassword->DrawFocusRect( FALSE );
	}

	m_iRESPRATE=getModel()->getDATAHANDLER()->getAVGMessureDataBPMco2();
	m_iETCO2=getModel()->getDATAHANDLER()->getAVGMessureDataETCO2();
	m_eCO2unit=getModel()->getCONFIG()->GetCO2unit();
	switch(m_eCO2unit)
	{
	case CO2_UNIT_KPA:
		{
			m_csUnit =_T(" [")+getModel()->GetLanguageString(IDS_UNIT_KPA)+_T("]");
		}
		break;
	case CO2_UNIT_PERCENT:
		{
			m_csUnit =_T(" [")+getModel()->GetLanguageString(IDS_UNIT_VOLPERCENT)+_T("]");
		}
		break;
	case CO2_UNIT_MMHG:
	default:
		{
			m_csUnit =_T(" [")+getModel()->GetLanguageString(IDS_UNIT_MMHG)+_T("]");
		}
		break;
	}
	LeaveCriticalSection(&csValues);
	

	BTN btn;

	btn.wID					= IDC_BTN_CO2_RESETTIME;	
	btn.poPosition.x		= 440;
	btn.poPosition.y		= 95;
	btn.pcBmpUp				= m_pcResetTime_Up;
	btn.pcBmpDown			= m_pcResetTime_Dw;
	btn.pcBmpFocus			= m_pcResetTime_Up;
	btn.pcBmpDisabled		= m_pcResetTime_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcResetTime=new CKbrdBtn(btn,COLOR_TXTBTNUP,true);
	m_pcResetTime->Create(this,g_hf8AcuBold,0);
	m_pcResetTime->SetText(getModel()->GetLanguageString(IDS_TXT_CO2_RESTARTTIMER_TOP),getModel()->GetLanguageString(IDS_TXT_CO2_RESTARTTIMER_BOT));
	
	EnterCriticalSection(&csValues);
	if(m_bPumpOn)
	{
		m_dwRemainCO2PumpTime=0;
		m_pcResetTime->ShowWindow(SW_HIDE);
	}
	else
	{
		if(m_bAutoonPump)
			m_pcResetTime->ShowWindow(SW_SHOW);
		else
			m_pcResetTime->ShowWindow(SW_HIDE);
	}
	LeaveCriticalSection(&csValues);
	


	StartCO2DataThread();
}

/**********************************************************************************************//**
 * Shows
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CWndDataCO2::Show(bool bShow)
{
	if(bShow)
	{
		SetWindowPos(NULL,0,0,m_lX,m_lX,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
		Draw(true,true);
	}
	else
	{
		SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
	}
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

LRESULT CWndDataCO2::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_REDRAW_CO2WND_COMPLETE:
		{
			Draw(true,true);
			return 1;
		}
		break;
	case WM_REDRAW_CO2WND_DATA:
		{
			Draw(false,false);
			return 1;
		}
		break;
	case WM_REDRAW_CO2WND_STATIC:
		{
			Draw(true,false);
			return 1;
		}
		break;
	default:
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndDataCO2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// CWnd::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndDataCO2::OnDestroy()
{
	StopCO2DataThread();

	if(m_pcwtCO2DataThread!=NULL)
	{
		delete m_pcwtCO2DataThread;
		m_pcwtCO2DataThread=NULL;

		if(m_hThreadCO2Data!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadCO2Data);
			m_hThreadCO2Data=INVALID_HANDLE_VALUE;
		}
	}

	CWnd::OnDestroy();

	if(m_hdcStatic)
		DeleteDC(m_hdcStatic);

	if(m_hbmpStatic)
		DeleteObject(m_hbmpStatic);

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}

/**********************************************************************************************//**
 * Updates the messure data described by bLimits
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	bLimits	True to limits.
 **************************************************************************************************/

void CWndDataCO2::UpdateMessureData(bool bLimits)
{
	if(bLimits)
		PostMessage(WM_REDRAW_CO2WND_COMPLETE);//Draw(true,true);xxxx
	else
		PostMessage(WM_REDRAW_CO2WND_DATA);//Draw(false,false);
	//Draw(bLimits,bLimits);
}

/**********************************************************************************************//**
 * Updates the information data described by resetAvailable
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	resetAvailable	True if reset available.
 **************************************************************************************************/

void CWndDataCO2::UpdateInfoData(bool resetAvailable)
{
	
	EnterCriticalSection(&csValues);
	m_dwRemainCO2PumpTime=getModel()->getDATAHANDLER()->getRemainCO2PumpTime()/1000;

	if(resetAvailable)
	{
		if(m_bAutoonPump)
			m_pcResetTime->ShowWindow(SW_SHOW);
		else
			m_pcResetTime->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pcResetTime->ShowWindow(SW_HIDE);
	}
	LeaveCriticalSection(&csValues);
	
	PostMessage(WM_REDRAW_CO2WND_STATIC);
	//Draw(true,false);
}

/**********************************************************************************************//**
 * Draws
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	bStatic	True to static.
 * \param	bLimits	True to limits.
 **************************************************************************************************/

void CWndDataCO2::Draw(bool bStatic, bool bLimits)
{
	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	//CBrush cbrBack(RGB(210,210,255));
	CBrush cbrBack(BACKGND);
	//CBrush cbrYellow(RGB(252,252,170));
	//CBrush cbrBack(RGB(210,210,255));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf43AcuBold);
	//HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf53AcuBold);
	int tc=SetTextColor(hdcMem,RGB(0,0,255));

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	if(bStatic)
		DrawStatic();

	if(bLimits)
		DrawLimits();

	BitBlt(m_hDC, 0, 0, m_lX, m_lY, m_hdcStatic, 0, 0, SRCCOPY);
	BitBlt(hdcMem, 0, 0, m_lX, m_lY,m_hdcStatic , 0, 0, SRCCOPY);

	EnterCriticalSection(&csValues);
	bool bETCO2ValueValid=m_bETCO2ValueValid;
	bool bFreqValuevalid=m_bFreqValuevalid;
	BYTE byCO2Module=m_byCO2Module;
	SHORT iETCO2=m_iETCO2;
	SHORT iRESPRATE=m_iRESPRATE;
	LeaveCriticalSection(&csValues);
	
	CStringW cs = _T("");

	if(byCO2Module==CO2MODULE_MICROPOD)
	{
		rc.left = 15;  
		rc.top = 30;  
		rc.right  = 185;  
		rc.bottom = 200;
		if(bETCO2ValueValid)
		{
			cs.Format(_T("%0.1f"), (double)iETCO2/10);
		}
		else
		{
			cs=_T("--");
		}

		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		//*******************************RespRate*****************************//
	
		rc.left = 395;  
		rc.right  = 565;

		if(bFreqValuevalid)
		{
			cs.Format(_T("%d"), iRESPRATE);
		}
		else
		{
			cs=_T("--");
		}
		
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	else
	{
		rc.left = 30;  
		rc.top = 30;  
		rc.right  = 186;  
		rc.bottom = 200;
		if(bETCO2ValueValid)
		{
			//cs.Format(_T("%d"), m_iETCO2);
			cs.Format(_T("%0.1f"), (double)iETCO2/10);
		}
		else
		{
			cs=_T("--");
		}

		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		//*******************************RespRate*****************************//
	
		rc.left = 305;  
		////rc.top = 60;  
		rc.right  = 545;  
		////rc.bottom = 200;
		if(bFreqValuevalid)
		{
			cs.Format(_T("%d"), iRESPRATE);
		}
		else
		{
			cs=_T("--");
		}
		
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);
	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}

/**********************************************************************************************//**
 * Draw static
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndDataCO2::DrawStatic()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);

	//CBrush cbrBack(RGB(210,210,255));
	CBrush cbrBack(BACKGND);
	CBrush cbrYellow(RGB(252,252,170));
	CBrush cbrGrey(RGB(200,200,200));
	//CBrush cbrBack(RGB(210,210,255));
	HBRUSH hbrprev=(HBRUSH)SelectObject(m_hdcStatic,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(m_hdcStatic, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(m_hdcStatic,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(m_hdcStatic,g_hf14AcuMed);
	int tc=SetTextColor(m_hdcStatic,0x0000000);

	Rectangle(m_hdcStatic, 0, 0, m_lX, m_lY);



	m_pcO2ParaLeft->Draw(m_hdcStatic,0,0);
	m_pcO2ParaRight->Draw(m_hdcStatic,545,0);

	SelectObject(m_hdcStatic,cbrYellow);
	Rectangle(m_hdcStatic, 20, 00, 545, DIAGRAMM_DATAHEIGHT);

	SelectObject(m_hdcStatic,cbrBack);

	CStringW cs = _T("");
	
	EnterCriticalSection(&csValues);
	bool bPumpOn=m_bPumpOn;
	bool bAutoonPump=m_bAutoonPump;
	DWORD dwRemainCO2PumpTime=m_dwRemainCO2PumpTime;
	BYTE byStateBytes=m_byStateBytes;
	BYTE byExtendedStateBytes=m_byExtendedStateBytes;
	bool bSystemDateOk=m_bSystemDateOk;
	bool bCalibrationIsDue=m_bCalibrationIsDue;
	bool bServiceIsDue=m_bServiceIsDue;
	LeaveCriticalSection(&csValues);

	if(m_byCO2Module==CO2MODULE_MICROPOD)
	{
		Rectangle(m_hdcStatic, 0, 80, 565, 85);
		Rectangle(m_hdcStatic, 185, 00, 190, 80);
		Rectangle(m_hdcStatic, 375, 00, 380, 80);
		
		SelectObject(m_hdcStatic,cbrGrey);
		Rectangle(m_hdcStatic, 190, 00, 375, 80);

		rc.left = 184;  
		rc.top = 5;  
		rc.right  = 361;  
		rc.bottom = 200;
		//cs = _T("Pump [on/off]");
		cs = getModel()->GetLanguageString(IDS_TXT_SENSOR_PUMPONOFF);
		//cs = getModel()->GetLanguageString(IDS_PARA_CO2FREQ);
		DrawText(m_hdcStatic,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
		

		EnterCriticalSection(&csValues);
		int iPos=m_sliderPassword->GetPos();
		if(iPos==1 && bPumpOn==false)
		{
			m_sliderPassword->SetPos(0);
			if(bAutoonPump)
				m_pcResetTime->ShowWindow(SW_SHOW);
			else
				m_pcResetTime->ShowWindow(SW_HIDE);
		}
		else if(iPos==0 && bPumpOn==true)
		{
			m_dwRemainCO2PumpTime=0;
			dwRemainCO2PumpTime=0;
			
			m_sliderPassword->SetPos(1);
			m_pcResetTime->ShowWindow(SW_HIDE);
		}
		LeaveCriticalSection(&csValues);


		//*******************************ETCO2*****************************//
		rc.left = 15;  
		rc.top = 5;  
		rc.right  = 186;  
		rc.bottom = 200;
		cs = getModel()->GetLanguageString(IDS_PARA_ETCO);
		//cs = _T("etCO");
		CSize sz = pDCStatic->GetTextExtent(cs);
		cs += _T("    ");
		cs += m_csUnit;
		DrawText(m_hdcStatic,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 15+sz.cx;  
		rc.top = 10;  
		rc.right  = 186;  
		rc.bottom = 200;
		//cs = getModel()->GetLanguageString(IDS_PARA_ETCO2);
		cs = _T("2");
		DrawText(m_hdcStatic,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		//*******************************RespRate*****************************//
		rc.left = 395;//199;  
		rc.top = 5;  
		rc.right  = 565;//364;  
		rc.bottom = 200;
		//cs = _T("Resp Rate [BPM]");
		cs = getModel()->GetLanguageString(IDS_PARA_CO2FREQ);
		sz = pDCStatic->GetTextExtent(cs);
		//cs += _T("   [")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]");
		DrawText(m_hdcStatic,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


		rc.left = 399+sz.cx;//199;  
		rc.top = 5;  
		rc.right  = 565;//364;  
		rc.bottom = 200;
		//cs = _T("Resp Rate [BPM]");
		cs = _T("   [")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]");
		DrawText(m_hdcStatic,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		//SelectObject(m_hdcStatic,g_hf7AcuBold);
	}
	else
	{
		Rectangle(m_hdcStatic, 0, 80, 565, 85);
		Rectangle(m_hdcStatic, 280, 00, 285, 80);


		
		//*******************************ETCO2*****************************//
		rc.left = 15;  
		rc.top = 5;  
		rc.right  = 186;  
		rc.bottom = 200;
		cs = getModel()->GetLanguageString(IDS_PARA_ETCO);
		//cs = _T("etCO");
		CSize sz = pDCStatic->GetTextExtent(cs);
		cs += _T("    ");
		cs += m_csUnit;
		DrawText(m_hdcStatic,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 15+sz.cx;  
		rc.top = 10;  
		rc.right  = 186;  
		rc.bottom = 200;
		//cs = getModel()->GetLanguageString(IDS_PARA_ETCO2);
		cs = _T("2");
		DrawText(m_hdcStatic,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		//*******************************RespRate*****************************//
		rc.left = 290;//199;  
		rc.top = 5;  
		rc.right  = 545;//364;  
		rc.bottom = 200;
		//cs = _T("Resp Rate [BPM]");
		cs = getModel()->GetLanguageString(IDS_PARA_CO2FREQ);
		cs += _T("   [")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]");
		DrawText(m_hdcStatic,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		m_pcResetTime->ShowWindow(SW_HIDE);
	}

	//*******************************Sensor state*****************************//
	SelectObject(m_hdcStatic,g_hf10AcuBold);
	
	cs = getModel()->GetLanguageString(IDS_TXT_SENSOR_STATE);
	cs += _T(" ");
	
	bool bOk=true;
	bool bTimer=false;
	CString szTime=_T("");


	if(m_byCO2Module==CO2MODULE_CAPNOSTAT)
	{
		switch(byStateBytes)
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
				cs+=_T("standby");
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
				cs += _T(" / ");
				cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_CHECKAIRWAY);
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
		if(byStateBytes==0xFF)
		{
			cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_OFF);
			bOk=false;
		}
		else if(getModel()->getETCO2()->get_StandbyMode())
		{
			//cs+=_T("standby"); 
			cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_STANDBY);
			bOk=false;
		}
		else if(m_bPumpOn==false)
		{
			//cs+=_T("pump off");
			
			bOk=false;
			

			if(dwRemainCO2PumpTime>0)
			{
				bTimer=true;

				int iMin=dwRemainCO2PumpTime/60;
				int iSec=0;
				if(iMin>0)
				{
					iSec=dwRemainCO2PumpTime-(iMin*60);
				}
				else
				{
					iSec=dwRemainCO2PumpTime;
				}
				if(iSec<10)
				{
					szTime.Format(_T("%d%s 0%d%s"), iMin,getModel()->GetLanguageString(IDS_UNIT_MIN),iSec,getModel()->GetLanguageString(IDS_UNIT_SECONDS));
				}
				else
				{
					szTime.Format(_T("%d%s %d%s"), iMin,getModel()->GetLanguageString(IDS_UNIT_MIN),iSec,getModel()->GetLanguageString(IDS_UNIT_SECONDS));
				}
				cs.Format(_T("%s %s: "), getModel()->GetLanguageString(IDS_MENU_CO2), getModel()->GetLanguageString(IDS_TXT_CO2AUTOPUMP));
			}
			else
			{
				cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_PUMPOFF);
			}
		}
		else
		{
			if(byStateBytes & BIT7)
			{
				//fault bit -> check extended status
				if(byExtendedStateBytes & BIT0)
				{
					//Check calibration
					cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_CHECKCAL);
					bOk=false;
				}
				else if(byExtendedStateBytes & BIT1)
				{
					//Check flow
					cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_CHECKFLOW);
					bOk=false;
				}
				else if(byExtendedStateBytes & BIT2)
				{
					//Occlusion in gas input line
					cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_OCCLUSION);
					bOk=false;
				}
				else if(byExtendedStateBytes & BIT3)
				{
					//Temperature out of range
					cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_OVERTEMP);
					bOk=false;
				}
				else if(byExtendedStateBytes & BIT7)
				{
					//malfunction, service code is set
					cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_MALFUNC);
					bOk=false;
				}
			}
			else if(byStateBytes & BIT6)
			{
				//filter line not connected
				cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_LINENOTCON);
				bOk=false;
			}
			else if(byStateBytes & BIT5)
			{
				//purging in progress
				cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_PURGING);
				bOk=false;
			}
			else if(byStateBytes & BIT4)
			{
				//SFM in progress
				cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_SFM);
				bOk=false;
			}
			else if(byStateBytes & BIT2)
			{
				//CO2 value over-range
				cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_CO2RANGE);
				bOk=false;
			}
			else if(byStateBytes & BIT1)
			{
				//initialization
				cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_INIT);
				bOk=false;
			}
			else if(byStateBytes & BIT0)
			{
				//invalid CO2 value
				cs+=getModel()->GetLanguageString(IDS_TXT_SENSOR_INVALIDCO2);
				bOk=false;
			}
		}

		if(bOk)
		{
			cs += getModel()->GetLanguageString(IDS_TXT_SENSOR_OK);

			if(bSystemDateOk==false)
			{
				bOk=false;
				cs+=_T(" (");
				cs+=getModel()->GetLanguageString(IDS_TXT_WRONG_SYSTEMDATE);
				cs+=_T(")");
			}
			else if(bServiceIsDue)
			{
				bOk=false;
				cs += _T(" (");
				cs += getModel()->GetLanguageString(IDS_TXT_SERVICE_REQUIRE);
				cs += _T(")");
			}
			else if(bCalibrationIsDue)
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
		cs += getModel()->GetLanguageString(IDS_ERR_ERROR);
		SetTextColor(m_hdcStatic,RGB(255,0,0));
	}

	if(bOk==false)
	{
		SetTextColor(m_hdcStatic,RGB(255,0,0));
	}
	
	if(bTimer)
	{
		rc.left = 10;  
		rc.top = 85;  
		rc.right  = 280;  
		rc.bottom = DIAGRAMM_DATAHEIGHT;
		DrawText(m_hdcStatic,cs,-1,&rc,DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

		SelectObject(m_hdcStatic,g_hf17AcuBold);
		rc.left = 300;  
		rc.top = 85;  
		rc.right  = 505;  
		rc.bottom = DIAGRAMM_DATAHEIGHT;
		DrawText(m_hdcStatic,szTime,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}
	else
	{
		rc.left = 0;  
		rc.top = 85;  
		rc.right  = 545;  
		rc.bottom = DIAGRAMM_DATAHEIGHT;
		DrawText(m_hdcStatic,cs,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	

	SetTextColor(m_hdcStatic,tc);
	SetBkMode(m_hdcStatic,nBkMode);

	SelectObject(m_hdcStatic,hbrprev);
	SelectObject(m_hdcStatic,hpenprev);
	SelectObject(m_hdcStatic,hPrevFont);
}

/**********************************************************************************************//**
 * Determines if we can draw limits
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CWndDataCO2::DrawLimits()
{
	CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);

	int nBkMode=SetBkMode(m_hdcStatic,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(m_hdcStatic,g_hf10AcuBold);
	int nPrevTxtColor=SetTextColor(m_hdcStatic,RGB(200,0,0));

	RECT rc;	
	TCHAR psz[MAX_PATH];

	if(m_byCO2Module==CO2MODULE_MICROPOD)
	{
		//################ETCO2 Max + Min##################
		m_pcLimitHigh_Up->Draw(m_hdcStatic,120,35);//right-60, top+2
		m_pcLimitLow_Up->Draw(m_hdcStatic,120,60);//right-60, bottom-16

		rc.top = 33;
		rc.bottom = 75;
		rc.left=50;
		rc.right=175;
		eAlarmLimitState limitstateEtco2Max = getModel()->getALARMHANDLER()->getAlimitState_ETCO2maxLimit();
		eAlarmLimitState limitstateEtco2Min = getModel()->getALARMHANDLER()->getAlimitState_ETCO2minLimit();
		if(limitstateEtco2Max == AL_CALC)
		{
			pDCStatic->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
		else if(limitstateEtco2Max == AL_OFF)
		{
			pDCStatic->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
		else if(limitstateEtco2Max != AL_OFF)
		{
			wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitETCO2max())/10, 0));
			pDCStatic->DrawText(psz,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
		if(limitstateEtco2Min == AL_CALC)
		{
			pDCStatic->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}
		else if(limitstateEtco2Min == AL_OFF)
		{
			pDCStatic->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}
		else if(limitstateEtco2Min != AL_OFF)
		{
			wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitETCO2min())/10, 0));
			pDCStatic->DrawText(psz,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}
	}
	else
	{
		//################ETCO2 Max + Min##################
		m_pcLimitHigh_Up->Draw(m_hdcStatic,200,30);//right-60, top+2
		m_pcLimitLow_Up->Draw(m_hdcStatic,200,60);//right-60, bottom-16

		rc.top = 28;
		rc.bottom = 75;
		rc.left=120;
		rc.right=260;//175;
		eAlarmLimitState limitstateMax = getModel()->getALARMHANDLER()->getAlimitState_ETCO2maxLimit();
		eAlarmLimitState limitstateMin = getModel()->getALARMHANDLER()->getAlimitState_ETCO2minLimit();
		if(limitstateMax == AL_CALC)
		{
			pDCStatic->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
		else if(limitstateMax == AL_OFF)
		{
			pDCStatic->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
		else if(limitstateMax != AL_OFF)
		{
			wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitETCO2max())/10, 0));
			pDCStatic->DrawText(psz,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
		if(limitstateMin == AL_CALC)
		{
			pDCStatic->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}
		else if(limitstateMin == AL_OFF)
		{
			pDCStatic->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}
		else if(limitstateMin != AL_OFF)
		{
			wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitETCO2min())/10, 0));
			pDCStatic->DrawText(psz,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}
	}

	SetTextColor(m_hdcStatic,nPrevTxtColor);
	SetBkMode(m_hdcStatic,nBkMode);
	SelectObject(m_hdcStatic,hPrevFont);
	return true;
}

/**********************************************************************************************//**
 * Starts co 2 data thread
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndDataCO2::StartCO2DataThread( void )
{
	startThread();

	if(m_pcwtCO2DataThread!=NULL)
	{
		delete m_pcwtCO2DataThread;
		m_pcwtCO2DataThread=NULL;

		if(m_hThreadCO2Data!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadCO2Data);
			m_hThreadCO2Data=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtCO2DataThread=AfxBeginThread(CCO2DataThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadCO2Data=m_pcwtCO2DataThread->m_hThread;
	m_pcwtCO2DataThread->m_bAutoDelete = FALSE; 
	m_pcwtCO2DataThread->ResumeThread();
}

/**********************************************************************************************//**
 * Stops co 2 data thread
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndDataCO2::StopCO2DataThread( void )
{
	if(doThread())
	{
		stopThread();

		g_eventCO2Data.SetEvent();
		
		if (WaitForSingleObject(m_pcwtCO2DataThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			theApp.WriteLog(_T("#THR:033a"));
			if(!TerminateThread(m_pcwtCO2DataThread,0))
			{
				theApp.WriteLog(_T("#THR:033b"));
			}
		}
	}
}

/**********************************************************************************************//**
 * Executes the thread operation
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CWndDataCO2::doThread()
{
	bool bRes=false;
	EnterCriticalSection(&csDoThread);
	bRes=m_bDoThread;
	LeaveCriticalSection(&csDoThread);
	return bRes;
}

/**********************************************************************************************//**
 * Starts a thread
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndDataCO2::startThread()
{
	EnterCriticalSection(&csDoThread);
	m_bDoThread=true;
	LeaveCriticalSection(&csDoThread);
}

/**********************************************************************************************//**
 * Stops a thread
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndDataCO2::stopThread()
{
	EnterCriticalSection(&csDoThread);
	m_bDoThread=false;
	LeaveCriticalSection(&csDoThread);
}

/**********************************************************************************************//**
 * Cco 2 data thread
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	pc	The PC.
 *
 * \return	An UINT.
 **************************************************************************************************/

static UINT CCO2DataThread( LPVOID pc )
{
	try
	{
		((CWndDataCO2*)pc)->CO2Data();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CCO2DataThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CCO2DataThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CWndDataCO2*)pc)->CO2Data();
	return TRUE;
}

/**********************************************************************************************//**
 * Co 2 data
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	A DWORD.
 **************************************************************************************************/

DWORD CWndDataCO2::CO2Data(void) 
{
	CeSetThreadPriority(m_pcwtCO2DataThread->m_hThread,256);
	
	do
	{
		WaitForSingleObject(g_eventCO2Data, INFINITE);

		if(doThread())
		{
			bool bDraw=false;

			SHORT iTempETCO2=getModel()->getDATAHANDLER()->getAVGMessureDataETCO2();
			EnterCriticalSection(&csValues);
			if(m_iETCO2!=iTempETCO2)
			{
				m_iETCO2=iTempETCO2;
				bDraw=true;
			}
			LeaveCriticalSection(&csValues);

			BYTE byStateBytes=0xFF;
			BYTE byExtendedStateBytes=0;
			bool bCO2ValueValid=false;
			bool bETCO2ValueValid=false;
			bool bPumpStateOn=false;
			bool bFreqValuevalid=false;
			bool bCalibrationIsDue=false;
			bool bServiceIsDue=false;
			bool bSystemDateOk=false;

			if(getModel()->getETCO2()!=NULL)
			{
				byStateBytes=getModel()->getETCO2()->getStateBytes();
				byExtendedStateBytes=getModel()->getETCO2()->getExtendedStateBytes();
				bCO2ValueValid=getModel()->getETCO2()->isCO2ValueValid();
				bETCO2ValueValid=getModel()->getETCO2()->isETCO2ValueValid();
				bPumpStateOn=getModel()->getETCO2()->isPumpOn();
				bFreqValuevalid=getModel()->getETCO2()->isFreqValueValid();
				bCalibrationIsDue=getModel()->getETCO2()->isCalibrationDue();
				bServiceIsDue=getModel()->getETCO2()->isServiceDue();
				bSystemDateOk=getModel()->getETCO2()->isSystemDateOk();
			}
			SHORT iTemp=getModel()->getDATAHANDLER()->getAVGMessureDataBPMco2();

			EnterCriticalSection(&csValues);
			if(bSystemDateOk!=m_bSystemDateOk)
			{
				m_bSystemDateOk=bSystemDateOk;
				bDraw=true;
			}
			if(bCalibrationIsDue!=m_bCalibrationIsDue)
			{
				m_bCalibrationIsDue=bCalibrationIsDue;
				bDraw=true;
			}
			if(bServiceIsDue!=m_bServiceIsDue)
			{
				m_bServiceIsDue=bServiceIsDue;
				bDraw=true;
			}
			if(m_iRESPRATE!=iTemp)
			{
				m_iRESPRATE=iTemp;
				bDraw=true;
			}
			if(		byStateBytes!=m_byStateBytes
				||	byExtendedStateBytes!=m_byExtendedStateBytes
				||	bCO2ValueValid!=m_bCO2ValueValid
				||	bETCO2ValueValid!=m_bETCO2ValueValid
				||	bFreqValuevalid!=m_bFreqValuevalid
				||	bPumpStateOn!=m_bPumpOn)
			{
				m_bCalibrationIsDue=bCalibrationIsDue;
				m_byStateBytes=byStateBytes;
				m_byExtendedStateBytes=byExtendedStateBytes;
				m_bETCO2ValueValid=bETCO2ValueValid;
				m_bFreqValuevalid=bFreqValuevalid;
				m_bCO2ValueValid=bCO2ValueValid;
				m_bPumpOn=bPumpStateOn;

				bDraw=true;
			}
			else
			{
				bDraw=false;
			}
			bool bPumpOn=m_bPumpOn;

			if(m_byCO2Module==CO2MODULE_MICROPOD)
			{
				int iPos=m_sliderPassword->GetPos();
				if(iPos==1 && bPumpOn==false)
				{
					bDraw=true;
				}
				else if(iPos==0 && bPumpOn==true)
				{
					bDraw=true;
				}
			}
			LeaveCriticalSection(&csValues);

			
			if(bDraw)
				PostMessage(WM_REDRAW_CO2WND_COMPLETE);//Draw(true,true);xxxx
			else
				PostMessage(WM_REDRAW_CO2WND_DATA);//Draw(false,false);
		}
	}while(doThread());

	//theApp.WriteLog(_T("#THR:033"));

	return 0;
}

/**********************************************************************************************//**
 * Executes my message action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	wParam	The wParam field of the message.
 * \param	lParam	The lParam field of the message.
 *
 * \return	A LRESULT.
 **************************************************************************************************/

LRESULT CWndDataCO2::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case IDC_SLD_CO2PUMP:
		{
			if(lParam==1)
			{
				if(getModel()->getETCO2()!=NULL)
					getModel()->getETCO2()->set_PumpStateOn();
				EnterCriticalSection(&csValues);
				m_dwRemainCO2PumpTime=0;
				m_bPumpOn=true;
				LeaveCriticalSection(&csValues);

				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_KILL_CO2PUMPTIMER);
			}
			else
			{
				if(getModel()->getETCO2()!=NULL)
					getModel()->getETCO2()->set_PumpStateOff();
				EnterCriticalSection(&csValues);
				m_bPumpOn=false;
				LeaveCriticalSection(&csValues);
				
				if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_FILTERLINE_NOTCONNECTED->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_CO2_FILTERLINE_NOTCONNECTED, true);
				}
				if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_CHECKSAMPLINGLINE->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_CO2_CHECKSAMPLINGLINE, true);
				}
				if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_CHECKAIRWAYADAPTER->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_CO2_CHECKAIRWAYADAPTER, true);
				}
				if(getModel()->getALARMHANDLER()->ALARM_Sens_CO2_SENSORFAULTY->getAlarmState()!=AS_NONE)
				{
					getModel()->getALARMHANDLER()->deleteAlarm(AL_Sens_CO2_SENSORFAULTY, true);
				}
				if(getModel()->getALARMHANDLER()->ALARM_PatAl_ETCO2max->getAlarmState()!=AS_NONE)
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_ETCO2max);

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_ETCO2min->getAlarmState()!=AS_NONE)
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_ETCO2min);

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_FICO2max->getAlarmState()!=AS_NONE)
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_FICO2max);

				if(getModel()->getALARMHANDLER()->ALARM_PatAl_FICO2min->getAlarmState()!=AS_NONE)
					getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_FICO2min);

				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SET_CO2PUMPTIMER);
			}

			EnterCriticalSection(&csValues);
			if(m_bPumpOn)
			{
				m_pcResetTime->ShowWindow(SW_HIDE);
			}
			else
			{
				if(m_bAutoonPump)
					m_pcResetTime->ShowWindow(SW_SHOW);
				else
					m_pcResetTime->ShowWindow(SW_HIDE);
			}
			LeaveCriticalSection(&csValues);

			PostMessage(WM_REDRAW_CO2WND_COMPLETE);
		}
		break;
	}
	return 1;
}

/**********************************************************************************************//**
 * Executes the button clicked reset timer action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndDataCO2::OnBnClickedResetTimer()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_SET_CO2PUMPTIMER);
}
