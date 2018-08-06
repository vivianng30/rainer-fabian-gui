// WndSubSettingsScreenlocktime.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndSubSettingsScreenlocktime.h"

/**********************************************************************************************//**
 * CWndSubSettingsScreenlocktime
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndSubSettingsScreenlocktime, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndSubSettingsScreenlocktime class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	viewFlag	The view flag.
 **************************************************************************************************/

CWndSubSettingsScreenlocktime::CWndSubSettingsScreenlocktime(UINT viewFlag):
CWndSubSettings(viewFlag)
{
	m_eAutoSreenlock=ASL_60;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndSubSettingsScreenlocktime class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndSubSettingsScreenlocktime::~CWndSubSettingsScreenlocktime()
{
}


BEGIN_MESSAGE_MAP(CWndSubSettingsScreenlocktime, CWnd)
	ON_WM_DESTROY()
	//ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
	ON_BN_CLICKED(IDC_BTN_SETUP_1, &CWndSubSettingsScreenlocktime::OnBnClicked1)
	ON_BN_CLICKED(IDC_BTN_SETUP_2, &CWndSubSettingsScreenlocktime::OnBnClicked2)
	ON_BN_CLICKED(IDC_BTN_SETUP_3, &CWndSubSettingsScreenlocktime::OnBnClicked3)
	ON_BN_CLICKED(IDC_BTN_SETUP_4, &CWndSubSettingsScreenlocktime::OnBnClicked4)
	ON_BN_CLICKED(IDC_BTN_SETUP_5, &CWndSubSettingsScreenlocktime::OnBnClicked5)
	//ON_BN_CLICKED(IDC_BTN_SETUP_6, &CWndSubSettingsScreenlocktime::OnBnClicked6)
	//ON_BN_CLICKED(IDC_BTN_SETUP_7, &CWndSubSettingsScreenlocktime::OnBnClicked7)
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndSubSettingsScreenlocktime message handlers
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsScreenlocktime::Initialize()
{
	CClientDC dc(this);

	m_eAutoSreenlock=getModel()->getCONFIG()->GetAutoScreenlockTime();

	m_szSetting = getModel()->GetLanguageString(IDS_TXT_AUTOSCREENLOCK);
	m_iNumValues=5;
	
	m_pbtn1->ShowWindow(SW_SHOW);
	m_pbtn1->RefreshText(_T("20 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS));
	
	m_pbtn2->ShowWindow(SW_SHOW);
	m_pbtn2->RefreshText(_T("30 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS));
	
	m_pbtn3->ShowWindow(SW_SHOW);
	m_pbtn3->RefreshText(_T("40 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS));

	m_pbtn4->ShowWindow(SW_SHOW);
	m_pbtn4->RefreshText(_T("50 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS));

	m_pbtn5->ShowWindow(SW_SHOW);
	m_pbtn5->RefreshText(_T("60 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS));

	m_plBtn.AddTail(m_pbtn1);
	m_plBtn.AddTail(m_pbtn2);
	m_plBtn.AddTail(m_pbtn3);
	m_plBtn.AddTail(m_pbtn4);
	m_plBtn.AddTail(m_pbtn5);

	switch(m_eAutoSreenlock)
	{
	case ASL_20:
		{
			m_pbtn1->DrawOK(true);
		}
		break;
	case ASL_30:
		{
			m_pbtn2->DrawOK(true);
		}
		break;
	case ASL_40:
		{
			m_pbtn3->DrawOK(true);
		}
		break;
	case ASL_50:
		{
			m_pbtn4->DrawOK(true);
		}
		break;
	case ASL_60:
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

void CWndSubSettingsScreenlocktime::Draw()
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
	RoundRect(hdcMem, 210, 70, 590, 320,20,20);



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

BOOL CWndSubSettingsScreenlocktime::PreTranslateMessage(MSG* pMsg) 
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
					SetAutoScreenlockTime(m_iCurPosInBlock);
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
 * Sets automatic screenlock time
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	btn	The button.
 **************************************************************************************************/

void CWndSubSettingsScreenlocktime::SetAutoScreenlockTime(UINT btn)
{
	switch(btn)
	{
	case IDC_BTN_SETUP_1:
		{
			m_eAutoSreenlock=ASL_20;
		}
		break;
	case IDC_BTN_SETUP_2:
		{
			m_eAutoSreenlock=ASL_30;
		}
		break;
	case IDC_BTN_SETUP_3:
		{
			m_eAutoSreenlock=ASL_40;
		}
		break;
	case IDC_BTN_SETUP_4:
		{
			m_eAutoSreenlock=ASL_50;
		}
		break;
	case IDC_BTN_SETUP_5:
		{
			m_eAutoSreenlock=ASL_60;
		}
		break;
	}

	getModel()->getCONFIG()->SetAutoScreenlockTime(m_eAutoSreenlock);
}

/**********************************************************************************************//**
 * Executes the button clicked 1 action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsScreenlocktime::OnBnClicked1()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_1);
	/*if(eState==BS_UP)
		SetOneButtonFocused(IDC_BTN_SETUP_1);
	else if(eState==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_1);
	else if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_1);*/

	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_1);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_1);
		SetAutoScreenlockTime(IDC_BTN_SETUP_1);
	}
}

/**********************************************************************************************//**
 * Executes the button clicked 2 action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsScreenlocktime::OnBnClicked2()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_2);
	/*if(eState==BS_UP)
		SetOneButtonFocused(IDC_BTN_SETUP_2);
	else if(eState==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_2);
	else if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_2);*/

	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_2);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_2);
		SetAutoScreenlockTime(IDC_BTN_SETUP_2);
	}
}

/**********************************************************************************************//**
 * Executes the button clicked 3 action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsScreenlocktime::OnBnClicked3()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_3);
	/*if(eState==BS_UP)
		SetOneButtonFocused(IDC_BTN_SETUP_3);
	else if(eState==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_3);
	else if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_3);*/

	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_3);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_3);
		SetAutoScreenlockTime(IDC_BTN_SETUP_3);
	}
}

/**********************************************************************************************//**
 * Executes the button clicked 4 action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsScreenlocktime::OnBnClicked4()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_4);
	/*if(eState==BS_UP)
		SetOneButtonFocused(IDC_BTN_SETUP_4);
	else if(eState==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_4);
	else if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_4);*/

	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_4);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_4);
		SetAutoScreenlockTime(IDC_BTN_SETUP_4);
	}
}

/**********************************************************************************************//**
 * Executes the button clicked 5 action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsScreenlocktime::OnBnClicked5()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_5);
	/*if(eState==BS_UP)
		SetOneButtonFocused(IDC_BTN_SETUP_5);
	else if(eState==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_5);
	else if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_5);*/

	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_5);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_5);
		SetAutoScreenlockTime(IDC_BTN_SETUP_5);
	}
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsScreenlocktime::OnDestroy()
{
	CWndSubSettings::OnDestroy();
}