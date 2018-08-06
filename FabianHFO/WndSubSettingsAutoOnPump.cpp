// WndSubSettingsAutoOnPump.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndSubSettingsAutoOnPump.h"

/**********************************************************************************************//**
 * CWndSubSettingsAutoOnPump
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndSubSettingsAutoOnPump, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndSubSettingsAutoOnPump class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	viewFlag	The view flag.
 **************************************************************************************************/

CWndSubSettingsAutoOnPump::CWndSubSettingsAutoOnPump(UINT viewFlag):
CWndSubSettings(viewFlag)
{
	m_iCO2TimePumpAutoOn=10;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndSubSettingsAutoOnPump class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndSubSettingsAutoOnPump::~CWndSubSettingsAutoOnPump()
{
}


BEGIN_MESSAGE_MAP(CWndSubSettingsAutoOnPump, CWnd)
	ON_WM_DESTROY()
	//ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
	ON_BN_CLICKED(IDC_BTN_SETUP_1, &CWndSubSettingsAutoOnPump::OnBnClicked1)
	ON_BN_CLICKED(IDC_BTN_SETUP_2, &CWndSubSettingsAutoOnPump::OnBnClicked2)
	ON_BN_CLICKED(IDC_BTN_SETUP_3, &CWndSubSettingsAutoOnPump::OnBnClicked3)
	ON_BN_CLICKED(IDC_BTN_SETUP_4, &CWndSubSettingsAutoOnPump::OnBnClicked4)
	ON_BN_CLICKED(IDC_BTN_SETUP_5, &CWndSubSettingsAutoOnPump::OnBnClicked5)
	//ON_BN_CLICKED(IDC_BTN_SETUP_6, &CWndSubSettingsScreenlocktime::OnBnClicked6)
	//ON_BN_CLICKED(IDC_BTN_SETUP_7, &CWndSubSettingsScreenlocktime::OnBnClicked7)
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndSubSettingsAutoOnPump message handlers
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsAutoOnPump::Initialize()
{
	CClientDC dc(this);

	m_iCO2TimePumpAutoOn=getModel()->getCONFIG()->GetTimePumpAutoOn();

	m_szSetting = getModel()->GetLanguageString(IDS_TXT_CO2AUTOPUMP);
	m_iNumValues=5;
	
	m_pbtn1->ShowWindow(SW_SHOW);
	m_pbtn1->RefreshText(getModel()->GetLanguageString(IDS_TXT_OFF));
	
	m_pbtn2->ShowWindow(SW_SHOW);
	m_pbtn2->RefreshText(_T("5 ")+getModel()->GetLanguageString(IDS_UNIT_MIN));
	
	m_pbtn3->ShowWindow(SW_SHOW);
	m_pbtn3->RefreshText(_T("10 ")+getModel()->GetLanguageString(IDS_UNIT_MIN));

	m_pbtn4->ShowWindow(SW_SHOW);
	m_pbtn4->RefreshText(_T("15 ")+getModel()->GetLanguageString(IDS_UNIT_MIN));

	m_pbtn5->ShowWindow(SW_SHOW);
	m_pbtn5->RefreshText(_T("30 ")+getModel()->GetLanguageString(IDS_UNIT_MIN));

	/*m_pbtn5->ShowWindow(SW_SHOW);
	m_pbtn5->RefreshText(_T("60 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS));*/

	m_plBtn.AddTail(m_pbtn1);
	m_plBtn.AddTail(m_pbtn2);
	m_plBtn.AddTail(m_pbtn3);
	m_plBtn.AddTail(m_pbtn4);
	m_plBtn.AddTail(m_pbtn5);

	switch(m_iCO2TimePumpAutoOn)
	{
	case 0:
		{
			m_pbtn1->DrawOK(true);
		}
		break;
	case 5:
		{
			m_pbtn2->DrawOK(true);
		}
		break;
	case 10:
		{
			m_pbtn3->DrawOK(true);
		}
		break;
	case 15:
		{
			m_pbtn4->DrawOK(true);
		}
		break;
	case 30:
		{
			m_pbtn5->DrawOK(true);
		}
		break;
	}

}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsAutoOnPump::Draw()
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
	RoundRect(hdcMem, 210, 70, 590, 318,20,20);



	MoveToEx(hdcMem, 210, 126, NULL);
	LineTo(hdcMem, 590, 126);

	MoveToEx(hdcMem, 210, 172, NULL);
	LineTo(hdcMem, 590, 172);

	MoveToEx(hdcMem, 210, 218, NULL);
	LineTo(hdcMem, 590, 218);

	MoveToEx(hdcMem, 210, 264, NULL);
	LineTo(hdcMem, 590, 264);

	/*MoveToEx(hdcMem, 210, 311, NULL);
	LineTo(hdcMem, 590, 311);*/

	/*MoveToEx(hdcMem, 205, 336, NULL);
	LineTo(hdcMem, 595, 336);

	MoveToEx(hdcMem, 205, 382, NULL);
	LineTo(hdcMem, 595, 382);

	MoveToEx(hdcMem, 205, 428, NULL);
	LineTo(hdcMem, 595, 428);

	MoveToEx(hdcMem, 205, 474, NULL);
	LineTo(hdcMem, 595, 474);*/



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
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pMsg	If non-null, the message.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CWndSubSettingsAutoOnPump::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				eBtnState eState = GetCurrentBtnState();

				if(eState==BS_FOCUSED)
				{
					SetOneButtonDepressed(m_iCurPosInBlock);
					SetTimePumpAutoOn(m_iCurPosInBlock);
				}
				else if(eState==BS_DOWN)
					SetOneButtonFocused(m_iCurPosInBlock);
				else
					SetOneButtonFocused(IDC_BTN_SETUP_1);
				return 1;
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				eBtnState eState = GetCurrentBtnState();
				if(		eState==BS_FOCUSED
					||	eState==BS_DOWN)
					SetPrevButtonFocused();

				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
				return 1;
			}
			else if(pMsg->wParam==VK_UP)
			{
				eBtnState eState = GetCurrentBtnState();
				if(		eState==BS_FOCUSED
					||	eState==BS_DOWN)
					SetNextButtonFocused();

				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
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
	default:
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

/**********************************************************************************************//**
 * Sets time pump automatic on
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	btn	The button.
 **************************************************************************************************/

void CWndSubSettingsAutoOnPump::SetTimePumpAutoOn(UINT btn)
{
	switch(btn)
	{
	case IDC_BTN_SETUP_1:
		{
			m_iCO2TimePumpAutoOn=0;
		}
		break;
	case IDC_BTN_SETUP_2:
		{
			m_iCO2TimePumpAutoOn=5;
		}
		break;
	case IDC_BTN_SETUP_3:
		{
			m_iCO2TimePumpAutoOn=10;
		}
		break;
	case IDC_BTN_SETUP_4:
		{
			m_iCO2TimePumpAutoOn=15;
		}
		break;
	case IDC_BTN_SETUP_5:
		{
			m_iCO2TimePumpAutoOn=30;
		}
		break;
	}

	getModel()->getCONFIG()->SetTimePumpAutoOn(m_iCO2TimePumpAutoOn);

	if(m_iCO2TimePumpAutoOn==0)
	{
		if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_KILL_CO2PUMPTIMER);
	}
	else if(getModel()->getETCO2())
	{
		if(getModel()->getETCO2()->isPumpOn()==false)
		{
			if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_SET_CO2PUMPTIMER);
		}
	}
}

/**********************************************************************************************//**
 * Executes the button clicked 1 action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsAutoOnPump::OnBnClicked1()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_1);
	
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_1);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_1);
		SetTimePumpAutoOn(IDC_BTN_SETUP_1);
	}
}

/**********************************************************************************************//**
 * Executes the button clicked 2 action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsAutoOnPump::OnBnClicked2()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_2);
	
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_2);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_2);
		SetTimePumpAutoOn(IDC_BTN_SETUP_2);
	}
}

/**********************************************************************************************//**
 * Executes the button clicked 3 action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsAutoOnPump::OnBnClicked3()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_3);
	
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_3);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_3);
		SetTimePumpAutoOn(IDC_BTN_SETUP_3);
	}
}

/**********************************************************************************************//**
 * Executes the button clicked 4 action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsAutoOnPump::OnBnClicked4()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_4);
	
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_4);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_4);
		SetTimePumpAutoOn(IDC_BTN_SETUP_4);
	}
}

/**********************************************************************************************//**
 * Executes the button clicked 5 action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsAutoOnPump::OnBnClicked5()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_5);

	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_5);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_5);
		SetTimePumpAutoOn(IDC_BTN_SETUP_5);
	}
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsAutoOnPump::OnDestroy()
{
	CWndSubSettings::OnDestroy();
}

