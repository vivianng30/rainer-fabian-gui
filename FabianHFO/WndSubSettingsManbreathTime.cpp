// WndSubSettingsManbreathTime.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndSubSettingsManbreathTime.h"

/**********************************************************************************************//**
 * A macro that defines color txtbtnup
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNUP				0x00000000

/**********************************************************************************************//**
 * A macro that defines color txtbtndw
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNDW				0x00FFFFFF

/**********************************************************************************************//**
 * A macro that defines color txtbtnfc
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNFC				0x00000000

/**********************************************************************************************//**
 * A macro that defines timechange
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

#define TIMECHANGE	600

/**********************************************************************************************//**
 * CWndSubSettingsManbreathTime
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndSubSettingsManbreathTime, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndSubSettingsManbreathTime class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	viewFlag	The view flag.
 **************************************************************************************************/

CWndSubSettingsManbreathTime::CWndSubSettingsManbreathTime(UINT viewFlag):
CWndSubSettings(viewFlag)
{
	m_pcUp_Up=NULL;
	m_pcUp_Dw=NULL;
	m_pcDw_Up=NULL;
	m_pcDw_Dw=NULL;

	m_pcStatic_Up=NULL;
	m_pcStatic_Dw=NULL;
	m_pcStatic_Fc=NULL;

	m_pbtnValue=NULL;

	m_pcNextUp=NULL;
	m_pcNextDwn=NULL;

	m_iValue=0;
	m_iCounter=0;
	m_eTimeChanger=TC_OFF;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndSubSettingsManbreathTime class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

CWndSubSettingsManbreathTime::~CWndSubSettingsManbreathTime()
{
}


BEGIN_MESSAGE_MAP(CWndSubSettingsManbreathTime, CWnd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SETUP_YEAR, &CWndSubSettingsManbreathTime::OnBnClickedValue)
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndSubSettingsManbreathTime message handlers
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsManbreathTime::Initialize()
{
	CClientDC dc(this);

	m_iValue=getModel()->getCONFIG()->GetManBreathTime();
	CStringW strVal;

	strVal.Format(_T("%d"),m_iValue);

	m_pcUp_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_UPUP);
	m_pcUp_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_UPDW);
	//m_pcUp_Fc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_UPFC);
	m_pcDw_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_DWUP);
	m_pcDw_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_DWDW);
	//m_pcDw_Fc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_DWFC);

	m_pcStatic_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VALUE_UP);
	m_pcStatic_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VALUE_DW);
	m_pcStatic_Fc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VALUE_FC);

	BTN btn;

	//*******************Days*****************************
	btn.wID					= IDC_BTN_SETUP_YEAR;	
	btn.poPosition.x		= 320;
	btn.poPosition.y		= 155;
	btn.pcBmpUp				= m_pcStatic_Up;
	btn.pcBmpDown			= m_pcStatic_Dw;
	btn.pcBmpFocus			= m_pcStatic_Fc;
	btn.pcBmpDisabled		= m_pcStatic_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pbtnValue=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
	m_pbtnValue->Create(this,g_hf33AcuBold,0);
	m_pbtnValue->SetText(strVal);
	m_pbtnValue->ShowWindow(SW_SHOW);


	btn.wID					= IDC_BTN_SETUP_NEXTUP;	
	btn.poPosition.x		= 435;
	btn.poPosition.y		= 143;
	btn.pcBmpUp				= m_pcUp_Up;
	btn.pcBmpDown			= m_pcUp_Dw;
	btn.pcBmpFocus			= m_pcUp_Up;
	btn.pcBmpDisabled		= m_pcUp_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcNextUp=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
	m_pcNextUp->Create(this,g_hf21AcuBold,0);
	m_pcNextUp->SetText(_T(""));
	m_pcNextUp->ShowWindow(SW_SHOW);

	btn.wID					= IDC_BTN_SETUP_NEXTDW;	
	btn.poPosition.x		= 435;
	btn.poPosition.y		= 185;
	btn.pcBmpUp				= m_pcDw_Up;
	btn.pcBmpDown			= m_pcDw_Dw;
	btn.pcBmpFocus			= m_pcDw_Up;
	btn.pcBmpDisabled		= m_pcDw_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcNextDwn=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
	m_pcNextDwn->Create(this,g_hf21AcuBold,0);
	m_pcNextDwn->SetText(_T(""));
	m_pcNextDwn->ShowWindow(SW_SHOW);

	m_szSetting = getModel()->GetLanguageString(IDS_TXT_MANBREATHTIME);
	m_iNumValues=0;
	

}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsManbreathTime::Draw()
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
	cbrFill.CreateSolidBrush(RGB(181,178,181));
	//cbrFill.CreateSolidBrush(RGB(150,150,150));
	SelectObject(hdcMem,cbrFill);

	Rectangle(hdcMem, 200, 30, 600, 465);

	Rectangle(hdcMem, 220, 10, 580, 30);
	Rectangle(hdcMem, 220, 465, 580, 485);

	m_pcRegionTopLeCorner->Draw(hdcMem,200,10);
	m_pcRegionTopRiCorner->Draw(hdcMem,580,10);
	m_pcRegionBotLeCorner->Draw(hdcMem,200,465);
	m_pcRegionBotRiCorner->Draw(hdcMem,580,465);

	
	rc.left = 220;  
	rc.top = 25;  
	rc.right  = 580;  
	rc.bottom = 500;
	DrawText(hdcMem,m_szSetting,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	CPen penBlack;
	penBlack.CreatePen(PS_SOLID,2,RGB(0,0,0));
	SelectObject(hdcMem,(HPEN)penBlack);
	MoveToEx(hdcMem, 200, 60, NULL);
	LineTo(hdcMem, 600, 60);

	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	SelectObject(hdcMem,cbrRound);
	RoundRect(hdcMem, 210, 70, 590, 300,20,20);

	MoveToEx(hdcMem, 300, 143, NULL);
	LineTo(hdcMem, 435, 143);

	MoveToEx(hdcMem, 300, 225, NULL);
	LineTo(hdcMem, 435, 225);

	rc.left = 500;  
	rc.top = 143;  
	rc.right  = 600;  
	rc.bottom = 225;
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_UNIT_SECONDS),-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);


	rc.left = 210;  
	rc.top = 225;  
	rc.right  = 590;  
	rc.bottom = 300;
	CStringW sz=_T("");
	sz.Format(_T("%s: %d - %d %s"),
		getModel()->GetLanguageString(IDS_TXT_RANGE),
		MIN_TIME_MANBREATH,
		MAX_TIME_MANBREATH,
		getModel()->GetLanguageString(IDS_UNIT_SECONDS));
	DrawText(hdcMem,sz,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);


	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	penBlack.DeleteObject();
	penLine.DeleteObject();

	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

}

/**********************************************************************************************//**
 * Translates all messages before they are processed by the main message loop
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pMsg	If non-null, the message.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CWndSubSettingsManbreathTime::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				eBtnState eState = m_pbtnValue->GetState();
				
				if(eState==BS_NONE)
				{
					m_pbtnValue->SetState(BS_FOCUSED);
				}
				else if(eState==BS_UP)
				{
					m_pbtnValue->SetState(BS_DOWN);
				}
				else if(eState==BS_FOCUSED)
				{
					m_pbtnValue->SetState(BS_DOWN);
				}
				else if(eState==BS_DOWN)
				{
					m_pbtnValue->SetState(BS_FOCUSED);
				}
				if(GetParent())
					GetParent()->PostMessage(WM_SET_SETUPTIMER);
				return 1;
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				if(m_pbtnValue->GetState()==BS_DOWN)
				{
					if(m_iValue>MIN_TIME_MANBREATH)
					{
						m_iValue-=STEP_TIME_MANBREATH;

						CStringW strVal;

						strVal.Format(_T("%d"),m_iValue);
						m_pbtnValue->RefreshText(strVal);
					}
					else
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
					}
				}
				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
			}
			else if(pMsg->wParam==VK_UP)
			{
				if(m_pbtnValue->GetState()==BS_DOWN)
				{
					if(m_iValue<MAX_TIME_MANBREATH)
					{
						m_iValue+=STEP_TIME_MANBREATH;

						CStringW strVal;

						strVal.Format(_T("%d"),m_iValue);
						m_pbtnValue->RefreshText(strVal);
					}
					else
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
					}
				}
				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
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

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsManbreathTime::OnDestroy()
{
	getModel()->getCONFIG()->SetManBreathTime(m_iValue);

	KillTimer(CHANGETIMER);
	
	delete m_pcUp_Up;
	delete m_pcUp_Dw;
	delete m_pcDw_Up;
	delete m_pcDw_Dw;

	m_pcUp_Up=NULL;
	m_pcUp_Dw=NULL;
	m_pcDw_Up=NULL;
	m_pcDw_Dw=NULL;

	delete m_pbtnValue;
	m_pbtnValue=NULL;

	delete m_pcNextUp;
	delete m_pcNextDwn;

	m_pcNextUp=NULL;
	m_pcNextDwn=NULL;

	CWndSubSettings::OnDestroy();

	delete m_pcStatic_Up;
	m_pcStatic_Up=NULL;
	delete m_pcStatic_Dw;
	m_pcStatic_Dw=NULL;
	delete m_pcStatic_Fc;
	m_pcStatic_Fc=NULL;
}

/**********************************************************************************************//**
 * Executes the button clicked value action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsManbreathTime::OnBnClickedValue()
{
	//SetOneButtonDepressed(IDC_BTN_SETUP_YEAR);
	if(m_pbtnValue->GetState()==BS_UP)
		m_pbtnValue->SetState(BS_DOWN);
	else if(m_pbtnValue->GetState()==BS_FOCUSED)
		m_pbtnValue->SetState(BS_DOWN);
	else if(m_pbtnValue->GetState()==BS_DOWN)
		m_pbtnValue->SetState(BS_FOCUSED);

	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
}

/**********************************************************************************************//**
 * Window proc
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	message	The message.
 * \param	wParam 	The wParam field of the message.
 * \param	lParam 	The lParam field of the message.
 *
 * \return	A LRESULT.
 **************************************************************************************************/

LRESULT CWndSubSettingsManbreathTime::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_TIMEBTN_DOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_SETUP_NEXTUP:
				{
					m_iCounter=0;
					m_eTimeChanger=TC_MANBREATHTIME_UP;
					SetTimer(CHANGETIMER,TIMECHANGE,NULL);
					m_pbtnValue->SetState(BS_DOWN);

					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
					return 1;
				}
				break;
			case IDC_BTN_SETUP_NEXTDW:
				{
					m_iCounter=0;
					m_eTimeChanger=TC_MANBREATHTIME_DW;
					SetTimer(CHANGETIMER,TIMECHANGE,NULL);
					m_pbtnValue->SetState(BS_DOWN);

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
	case WM_TIMEBTN_UP:
		{
			switch(wParam)
			{
			case IDC_BTN_SETUP_NEXTUP:
				{
					if(m_iCounter==0)
					{
						if(m_iValue<MAX_TIME_MANBREATH)
						{
							m_iValue+=STEP_TIME_MANBREATH;

							CStringW strVal;

							strVal.Format(_T("%d"),m_iValue);
							m_pbtnValue->RefreshText(strVal);
						}
						else
						{
							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
						}
					}
					m_iCounter=0;
					m_eTimeChanger=TC_OFF;
					KillTimer(CHANGETIMER);

					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
					return 1;
				}
				break;
			case IDC_BTN_SETUP_NEXTDW:
				{
					if(m_iCounter==0)
					{
						if(m_iValue>MIN_TIME_MANBREATH)
						{
							m_iValue-=STEP_TIME_MANBREATH;

							CStringW strVal;

							strVal.Format(_T("%d"),m_iValue);
							m_pbtnValue->RefreshText(strVal);
						}
						else
						{
							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
						}
					}
					m_iCounter=0;
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
	//case WM_MENUBUTTONDOWN:
	//	{
	//		switch(wParam)
	//		{
	//		case IDC_BTN_SETTING_BACK:
	//			{
	//				/*getModel()->getDATAHANDLER()->SetCurManBreathTime(m_iValue);*/
	//				GetParent()->PostMessage(WM_CLOSE_SETTING,m_viewFlag);
	//				return 1;
	//			}
	//			break;
	//		default:
	//			{

	//			}
	//			break;
	//		}
	//	}
	//	break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

/**********************************************************************************************//**
 * Executes the timer action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nIDEvent	The identifier event.
 **************************************************************************************************/

void CWndSubSettingsManbreathTime::OnTimer(UINT_PTR nIDEvent)
{
	CStringW strDate;

	if(nIDEvent==CHANGETIMER)
	{
		switch(m_eTimeChanger)
		{
		case TC_MANBREATHTIME_UP:
			{
				if(m_iValue<MAX_TIME_MANBREATH)
				{
					m_iValue+=STEP_TIME_MANBREATH;

					CStringW strVal;

					strVal.Format(_T("%d"),m_iValue);
					m_pbtnValue->RefreshText(strVal);
				}
				else
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
				}

				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
			}
			break;
		case TC_MANBREATHTIME_DW:
			{
				if(m_iValue>MIN_TIME_MANBREATH)
				{
					m_iValue-=STEP_TIME_MANBREATH;

					CStringW strVal;

					strVal.Format(_T("%d"),m_iValue);
					m_pbtnValue->RefreshText(strVal);
				}
				else
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
				}

				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
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