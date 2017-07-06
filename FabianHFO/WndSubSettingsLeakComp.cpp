// WndSubSettingsLeakComp.cpp : implementation file

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndSubSettingsLeakComp.h"



IMPLEMENT_DYNAMIC(CWndSubSettingsLeakComp, CWnd)

CWndSubSettingsLeakComp::CWndSubSettingsLeakComp(UINT viewFlag):
CWndSubSettings(viewFlag)
{
	m_eLeakCompensation=LC_MIDDLE;
}

CWndSubSettingsLeakComp::~CWndSubSettingsLeakComp()
{
}


BEGIN_MESSAGE_MAP(CWndSubSettingsLeakComp, CWnd)
	ON_WM_DESTROY()
	//ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
	ON_BN_CLICKED(IDC_BTN_SETUP_1, &CWndSubSettingsLeakComp::OnBnClicked1)
	ON_BN_CLICKED(IDC_BTN_SETUP_2, &CWndSubSettingsLeakComp::OnBnClicked2)
	ON_BN_CLICKED(IDC_BTN_SETUP_3, &CWndSubSettingsLeakComp::OnBnClicked3)
	//ON_BN_CLICKED(IDC_BTN_SETUP_4, &CWndSubSettingsLeakComp::OnBnClicked4)
	//ON_BN_CLICKED(IDC_BTN_SETUP_5, &CWndSubSettingsAutoOxyCal::OnBnClicked5)
	//ON_BN_CLICKED(IDC_BTN_SETUP_6, &CWndSubSettingsAutoOxyCal::OnBnClicked6)
	//ON_BN_CLICKED(IDC_BTN_SETUP_7, &CWndSubSettingsAutoOxyCal::OnBnClicked7)
END_MESSAGE_MAP()



// CWndSubSettingsLeakComp message handlers
// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsLeakComp::Initialize()
{
	CClientDC dc(this);

	m_eLeakCompensation=getModel()->getCONFIG()->getLeakCompensation();

	m_szSetting = getModel()->GetLanguageString(IDS_TXT_LEAKCOMP);
	m_iNumValues=3;

	//m_pbtn1->ShowWindow(SW_SHOW);
	//m_pbtn1->RefreshText(getModel()->GetLanguageString(IDS_TXT_DISABLED));

	m_pbtn1->ShowWindow(SW_SHOW);
	m_pbtn1->RefreshText(getModel()->GetLanguageString(IDS_TXT_SENS_LOW));
	
	m_pbtn2->ShowWindow(SW_SHOW);
	m_pbtn2->RefreshText(getModel()->GetLanguageString(IDS_TXT_SENS_MID));

	m_pbtn3->ShowWindow(SW_SHOW);
	m_pbtn3->RefreshText(getModel()->GetLanguageString(IDS_TXT_SENS_HIGH));

	m_plBtn.AddTail(m_pbtn1);
	m_plBtn.AddTail(m_pbtn2);
	m_plBtn.AddTail(m_pbtn3);
	//m_plBtn.AddTail(m_pbtn4);

	switch(m_eLeakCompensation)
	{
	/*case LC_OFF:
		{
			m_pbtn1->DrawOK(true);
		}
		break;*/
	case LC_LOW:
		{
			m_pbtn1->DrawOK(true);
		}
		break;
	case LC_MIDDLE:
		{
			m_pbtn2->DrawOK(true);
		}
		break;
	case LC_HIGH:
		{
			m_pbtn3->DrawOK(true);
		}
		break;
	}

}

// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsLeakComp::Draw()
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
	//RoundRect(hdcMem, 210, 70, 590, 272,20,20);
	RoundRect(hdcMem, 210, 70, 590, 226,20,20);

	MoveToEx(hdcMem, 210, 126, NULL);
	LineTo(hdcMem, 590, 126);

	MoveToEx(hdcMem, 210, 172, NULL);
	LineTo(hdcMem, 590, 172);

	//MoveToEx(hdcMem, 210, 218, NULL);
	//LineTo(hdcMem, 590, 218);

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
BOOL CWndSubSettingsLeakComp::PreTranslateMessage(MSG* pMsg) 
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
					setLeakCompOff(m_iCurPosInBlock);
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
void CWndSubSettingsLeakComp::setLeakCompOff(UINT btn)
{
	switch(btn)
	{
	/*case IDC_BTN_SETUP_1:
		{
			m_eLeakCompensation=LC_OFF;
		}
		break;*/
	case IDC_BTN_SETUP_1:
		{
			m_eLeakCompensation=LC_LOW;
		}
		break;
	case IDC_BTN_SETUP_2:
		{
			m_eLeakCompensation=LC_MIDDLE;
		}
		break;
	case IDC_BTN_SETUP_3:
		{
			m_eLeakCompensation=LC_HIGH;
		}
		break;
	}

	getModel()->getCONFIG()->setLeakCompensation(m_eLeakCompensation);
}

// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsLeakComp::OnBnClicked1()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_1);
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_1);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_1);
		setLeakCompOff(IDC_BTN_SETUP_1);
	}
}
// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsLeakComp::OnBnClicked2()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_2);
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_2);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_2);
		setLeakCompOff(IDC_BTN_SETUP_2);
	}
}
// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsLeakComp::OnBnClicked3()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_3);
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_3);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_3);
		setLeakCompOff(IDC_BTN_SETUP_3);
	}
}
// **************************************************************************
// 
// **************************************************************************
//void CWndSubSettingsLeakComp::OnBnClicked4()
//{
//	eBtnState eState = GetBtnState(IDC_BTN_SETUP_4);
//	if(eState==BS_DOWN)
//		SetOneButtonFocused(IDC_BTN_SETUP_4);
//	else
//	{
//		SetOneButtonDepressed(IDC_BTN_SETUP_4);
//		setLeakCompOff(IDC_BTN_SETUP_4);
//	}
//}
// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsLeakComp::OnDestroy()
{
	CWndSubSettings::OnDestroy();
}