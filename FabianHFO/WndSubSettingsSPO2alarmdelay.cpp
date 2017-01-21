// WndSubSettingsSPO2alarmdelay.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndSubSettingsSPO2alarmdelay.h"


// CWndSubSettingsSPO2alarmdelay

IMPLEMENT_DYNAMIC(CWndSubSettingsSPO2alarmdelay, CWnd)

CWndSubSettingsSPO2alarmdelay::CWndSubSettingsSPO2alarmdelay(UINT viewFlag):
CWndSubSettings(viewFlag)
{
	m_iSPO2alarmdelay=SPO2_ALARMDELAY_0;
}

CWndSubSettingsSPO2alarmdelay::~CWndSubSettingsSPO2alarmdelay()
{
}


BEGIN_MESSAGE_MAP(CWndSubSettingsSPO2alarmdelay, CWnd)
	ON_WM_DESTROY()
	//ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
	ON_BN_CLICKED(IDC_BTN_SETUP_1, &CWndSubSettingsSPO2alarmdelay::OnBnClicked1)
	ON_BN_CLICKED(IDC_BTN_SETUP_2, &CWndSubSettingsSPO2alarmdelay::OnBnClicked2)
	ON_BN_CLICKED(IDC_BTN_SETUP_3, &CWndSubSettingsSPO2alarmdelay::OnBnClicked3)
	ON_BN_CLICKED(IDC_BTN_SETUP_4, &CWndSubSettingsSPO2alarmdelay::OnBnClicked4)
	//ON_BN_CLICKED(IDC_BTN_SETUP_5, &CWndSubSettingsAutoOxyCal::OnBnClicked5)
	//ON_BN_CLICKED(IDC_BTN_SETUP_6, &CWndSubSettingsAutoOxyCal::OnBnClicked6)
	//ON_BN_CLICKED(IDC_BTN_SETUP_7, &CWndSubSettingsAutoOxyCal::OnBnClicked7)
END_MESSAGE_MAP()



// CWndSubSettingsSPO2alarmdelay message handlers

// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsSPO2alarmdelay::Initialize()
{
	CClientDC dc(this);

	m_iSPO2alarmdelay=getModel()->getCONFIG()->getSPO2alarmdelay();

	//m_szSetting = getModel()->GetLanguageString(IDS_TXT_FLUSHTIME);
	m_szSetting = _T("Alarm delay:");
	m_iNumValues=4;

	m_pbtn1->ShowWindow(SW_SHOW);
	m_pbtn1->RefreshText(_T("0 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS));

	m_pbtn2->ShowWindow(SW_SHOW);
	m_pbtn2->RefreshText(_T("5 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS));

	m_pbtn3->ShowWindow(SW_SHOW);
	m_pbtn3->RefreshText(_T("10 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS));

	m_pbtn4->ShowWindow(SW_SHOW);
	m_pbtn4->RefreshText(_T("15 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS));


	m_plBtn.AddTail(m_pbtn1);
	m_plBtn.AddTail(m_pbtn2);
	m_plBtn.AddTail(m_pbtn3);
	m_plBtn.AddTail(m_pbtn4);

	switch(m_iSPO2alarmdelay)
	{
	case SPO2_ALARMDELAY_0:
		{
			m_pbtn1->DrawOK(true);
		}
		break;
	case SPO2_ALARMDELAY_5:
		{
			m_pbtn2->DrawOK(true);
		}
		break;
	case SPO2_ALARMDELAY_10:
		{
			m_pbtn3->DrawOK(true);
		}
		break;
	case SPO2_ALARMDELAY_15:
		{
			m_pbtn4->DrawOK(true);
		}
		break;
	}

	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
}

// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsSPO2alarmdelay::Draw()
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

	//CDC* pDCMem=CDC::FromHandle(hdcMem);




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
	RoundRect(hdcMem, 210, 70, 590, 272,20,20);



	MoveToEx(hdcMem, 210, 126, NULL);
	LineTo(hdcMem, 590, 126);

	MoveToEx(hdcMem, 210, 172, NULL);
	LineTo(hdcMem, 590, 172);

	MoveToEx(hdcMem, 210, 218, NULL);
	LineTo(hdcMem, 590, 218);


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

// **************************************************************************
// 
// **************************************************************************
BOOL CWndSubSettingsSPO2alarmdelay::PreTranslateMessage(MSG* pMsg) 
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
					SetAlarmDelay(m_iCurPosInBlock);
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

// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsSPO2alarmdelay::SetAlarmDelay(UINT btn)
{
	switch(btn)
	{
	case IDC_BTN_SETUP_1:
		{
			m_iSPO2alarmdelay=SPO2_ALARMDELAY_0;
		}
		break;
	case IDC_BTN_SETUP_2:
		{
			m_iSPO2alarmdelay=SPO2_ALARMDELAY_5;
		}
		break;
	case IDC_BTN_SETUP_3:
		{
			m_iSPO2alarmdelay=SPO2_ALARMDELAY_10;
		}
		break;
	case IDC_BTN_SETUP_4:
		{
			m_iSPO2alarmdelay=SPO2_ALARMDELAY_15;
		}
		break;
	}

	if(getModel()->getSPO2()!=NULL)
		getModel()->getSPO2()->set_SPO2alarmdelay(m_iSPO2alarmdelay);
}

// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsSPO2alarmdelay::OnBnClicked1()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_1);
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_1);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_1);
		SetAlarmDelay(IDC_BTN_SETUP_1);
	}
}
// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsSPO2alarmdelay::OnBnClicked2()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_2);
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_2);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_2);
		SetAlarmDelay(IDC_BTN_SETUP_2);
	}
}
// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsSPO2alarmdelay::OnBnClicked3()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_3);
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_3);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_3);
		SetAlarmDelay(IDC_BTN_SETUP_3);
	}
}
// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsSPO2alarmdelay::OnBnClicked4()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_4);
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_4);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_4);
		SetAlarmDelay(IDC_BTN_SETUP_4);
	}
}
//************************************
// Method:    OnDestroy
// FullName:  CWndSubSettingsSPO2alarmdelay::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndSubSettingsSPO2alarmdelay::OnDestroy()
{
	CWndSubSettings::OnDestroy();
}