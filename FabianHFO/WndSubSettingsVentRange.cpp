// WndSubSettingsVentRange.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndSubSettingsVentRange.h"

/**********************************************************************************************//**
 * CWndSubSettingsVentRange
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndSubSettingsVentRange, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndSubSettingsVentRange class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	viewFlag	The view flag.
 **************************************************************************************************/

CWndSubSettingsVentRange::CWndSubSettingsVentRange(UINT viewFlag):
CWndSubSettings(viewFlag)
{
	m_iVentRange=0;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndSubSettingsVentRange class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

CWndSubSettingsVentRange::~CWndSubSettingsVentRange()
{
}


BEGIN_MESSAGE_MAP(CWndSubSettingsVentRange, CWnd)
	ON_WM_DESTROY()
	//ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
	ON_BN_CLICKED(IDC_BTN_SETUP_1, &CWndSubSettingsVentRange::OnBnClicked1)
	ON_BN_CLICKED(IDC_BTN_SETUP_2, &CWndSubSettingsVentRange::OnBnClicked2)
	//ON_BN_CLICKED(IDC_BTN_SETUP_3, &CWndSubSettingsAutoOxyCal::OnBnClicked3)
	//ON_BN_CLICKED(IDC_BTN_SETUP_4, &CWndSubSettingsAutoOxyCal::OnBnClicked4)
	//ON_BN_CLICKED(IDC_BTN_SETUP_5, &CWndSubSettingsAutoOxyCal::OnBnClicked5)
	//ON_BN_CLICKED(IDC_BTN_SETUP_6, &CWndSubSettingsAutoOxyCal::OnBnClicked6)
	//ON_BN_CLICKED(IDC_BTN_SETUP_7, &CWndSubSettingsAutoOxyCal::OnBnClicked7)
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndSubSettingsVentRange message handlers
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsVentRange::Initialize()
{
	CClientDC dc(this);

	m_iVentRange=getModel()->getCONFIG()->GetVentRange();

	m_szSetting = getModel()->GetLanguageString(IDS_TXT_VENTRANGE);
	//m_szSetting = _T("ventilator range:");
	m_iNumValues=2;
	
	m_pbtn1->ShowWindow(SW_SHOW);
	m_pbtn1->RefreshText(getModel()->GetLanguageString(IDS_TXT_NEONATAL));
	
	m_pbtn2->ShowWindow(SW_SHOW);
	m_pbtn2->RefreshText(getModel()->GetLanguageString(IDS_TXT_PEDIATRIC));
	
	m_plBtn.AddTail(m_pbtn1);
	m_plBtn.AddTail(m_pbtn2);

	switch(m_iVentRange)
	{
	case NEONATAL:
		{
			m_pbtn1->DrawOK(true);
		}
		break;
	case PEDIATRIC:
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

void CWndSubSettingsVentRange::Draw()
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

	/*MoveToEx(hdcMem, 205, 152, NULL);
	LineTo(hdcMem, 595, 152);

	MoveToEx(hdcMem, 205, 198, NULL);
	LineTo(hdcMem, 595, 198);

	MoveToEx(hdcMem, 205, 244, NULL);
	LineTo(hdcMem, 595, 244);

	MoveToEx(hdcMem, 205, 290, NULL);
	LineTo(hdcMem, 595, 290);*/

	/*MoveToEx(hdcMem, 205, 336, NULL);
	LineTo(hdcMem, 595, 336);

	MoveToEx(hdcMem, 205, 382, NULL);
	LineTo(hdcMem, 595, 382);

	MoveToEx(hdcMem, 205, 428, NULL);
	LineTo(hdcMem, 595, 428);

	MoveToEx(hdcMem, 205, 474, NULL);
	LineTo(hdcMem, 595, 474);*/


	if(m_pcWarning)
		m_pcWarning->Draw(hdcMem, 380,280);

	rc.left = 220;  
	rc.top = 330;    
	rc.right  = 580;  
	rc.bottom = 500;
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_DISCON_PAT),-1,&rc,DT_CENTER|DT_TOP|DT_WORDBREAK);

	rc.left = 220;  
	rc.top = 370;    
	rc.right  = 580;  
	rc.bottom = 500;
	//DrawText(hdcMem,_T("Flow sensor calibration is required after changing!"),-1,&rc,DT_CENTER|DT_TOP|DT_WORDBREAK);
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_RANGE_CALIBR),-1,&rc,DT_CENTER|DT_TOP|DT_WORDBREAK);


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

BOOL CWndSubSettingsVentRange::PreTranslateMessage(MSG* pMsg) 
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
					SetVentRange(m_iCurPosInBlock);
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
 * Sets vent range
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	btn	The button.
 **************************************************************************************************/

void CWndSubSettingsVentRange::SetVentRange(UINT btn)
{
	switch(btn)
	{
	case IDC_BTN_SETUP_1:
		{
			m_iVentRange=NEONATAL;
		}
		break;
	case IDC_BTN_SETUP_2:
		{
			m_iVentRange=PEDIATRIC;
		}
		break;
	}

	//getModel()->getCONFIG()->SetVentRange(m_iVentRange);
}

/**********************************************************************************************//**
 * Executes the button clicked 1 action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsVentRange::OnBnClicked1()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_1);
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_1);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_1);
		SetVentRange(IDC_BTN_SETUP_1);
	}
}

/**********************************************************************************************//**
 * Executes the button clicked 2 action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsVentRange::OnBnClicked2()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_2);
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_2);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_2);
		SetVentRange(IDC_BTN_SETUP_2);
	}
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsVentRange::OnDestroy()
{
	getModel()->getCONFIG()->SetVentRange(m_iVentRange);

	CWndSubSettings::OnDestroy();
}