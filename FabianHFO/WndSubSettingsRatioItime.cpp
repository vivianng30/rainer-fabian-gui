// WndSubSettingsRatioItime.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndSubSettingsRatioItime.h"

/**********************************************************************************************//**
 * CWndSubSettingsRatioItime
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndSubSettingsRatioItime, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndSubSettingsRatioItime class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	viewFlag	The view flag.
 **************************************************************************************************/

CWndSubSettingsRatioItime::CWndSubSettingsRatioItime(UINT viewFlag):
CWndSubSettings(viewFlag)
{
	m_eModeITime=RM_SET_ItoBPM;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndSubSettingsRatioItime class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

CWndSubSettingsRatioItime::~CWndSubSettingsRatioItime()
{
}


BEGIN_MESSAGE_MAP(CWndSubSettingsRatioItime, CWnd)
	ON_WM_DESTROY()
	//ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
	ON_BN_CLICKED(IDC_BTN_SETUP_1, &CWndSubSettingsRatioItime::OnBnClicked1)
	ON_BN_CLICKED(IDC_BTN_SETUP_2, &CWndSubSettingsRatioItime::OnBnClicked2)
	//ON_BN_CLICKED(IDC_BTN_SETUP_3, &CWndSubSettingsAutoOxyCal::OnBnClicked3)
	//ON_BN_CLICKED(IDC_BTN_SETUP_4, &CWndSubSettingsAutoOxyCal::OnBnClicked4)
	//ON_BN_CLICKED(IDC_BTN_SETUP_5, &CWndSubSettingsAutoOxyCal::OnBnClicked5)
	//ON_BN_CLICKED(IDC_BTN_SETUP_6, &CWndSubSettingsAutoOxyCal::OnBnClicked6)
	//ON_BN_CLICKED(IDC_BTN_SETUP_7, &CWndSubSettingsAutoOxyCal::OnBnClicked7)
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndSubSettingsRatioItime message handlers
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsRatioItime::Initialize()
{
	CClientDC dc(this);

	m_eModeITime=getModel()->getCONFIG()->GetIERelationMode();

	m_szSetting = getModel()->GetLanguageString(IDS_TXT_RATIOITIME);
	m_iNumValues=2;
	
	m_pbtn1->ShowWindow(SW_SHOW);
	m_pbtn1->RefreshText(getModel()->GetLanguageString(IDS_TXT_BPMITIME));
	
	m_pbtn2->ShowWindow(SW_SHOW);
	m_pbtn2->RefreshText(getModel()->GetLanguageString(IDS_TXT_ITIMEETIME));
	
	m_plBtn.AddTail(m_pbtn1);
	m_plBtn.AddTail(m_pbtn2);

	switch(m_eModeITime)
	{
	case RM_SET_ItoBPM:
		{
			m_pbtn1->DrawOK(true);
		}
		break;
	case RM_SET_ItoE:
		{
			m_pbtn2->DrawOK(true);
		}
		break;
	}

}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsRatioItime::Draw()
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
	RoundRect(hdcMem, 210, 70, 590, 180,20,20);



	MoveToEx(hdcMem, 210, 126, NULL);
	LineTo(hdcMem, 590, 126);



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

BOOL CWndSubSettingsRatioItime::PreTranslateMessage(MSG* pMsg) 
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
					SetIERatio(m_iCurPosInBlock);
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

				return 1;
			}
			else if(pMsg->wParam==VK_UP)
			{
				eBtnState eState = GetCurrentBtnState();
				if(		eState==BS_FOCUSED
					||	eState==BS_DOWN)
					SetNextButtonFocused();
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
 * Sets IE ratio
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	btn	The button.
 **************************************************************************************************/

void CWndSubSettingsRatioItime::SetIERatio(UINT btn)
{
	switch(btn)
	{
	case IDC_BTN_SETUP_1:
		{
			m_eModeITime=RM_SET_ItoBPM;
		}
		break;
	case IDC_BTN_SETUP_2:
		{
			m_eModeITime=RM_SET_ItoE;
		}
		break;
	}

	getModel()->getCONFIG()->SetIERelationMode(m_eModeITime);
}

/**********************************************************************************************//**
 * Executes the button clicked 1 action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsRatioItime::OnBnClicked1()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_1);
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_1);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_1);
		SetIERatio(IDC_BTN_SETUP_1);
	}
}

/**********************************************************************************************//**
 * Executes the button clicked 2 action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsRatioItime::OnBnClicked2()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_2);
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_2);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_2);
		SetIERatio(IDC_BTN_SETUP_2);
	}
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsRatioItime::OnDestroy()
{
	CWndSubSettings::OnDestroy();
}