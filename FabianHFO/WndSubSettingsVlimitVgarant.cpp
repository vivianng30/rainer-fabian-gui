// WndSubSettingsVlimitVgarant.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndSubSettingsVlimitVgarant.h"
#include "DlgMessageBox.h"

/**********************************************************************************************//**
 * CWndSubSettingsVlimitVgarant
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndSubSettingsVlimitVgarant, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndSubSettingsVlimitVgarant class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	viewFlag	The view flag.
 **************************************************************************************************/

CWndSubSettingsVlimitVgarant::CWndSubSettingsVlimitVgarant(UINT viewFlag):
CWndSubSettings(viewFlag)
{
	m_eVControl=VCTRL_VLIMIT;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndSubSettingsVlimitVgarant class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndSubSettingsVlimitVgarant::~CWndSubSettingsVlimitVgarant()
{
}


BEGIN_MESSAGE_MAP(CWndSubSettingsVlimitVgarant, CWnd)
	ON_WM_DESTROY()
	//ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
	ON_BN_CLICKED(IDC_BTN_SETUP_1, &CWndSubSettingsVlimitVgarant::OnBnClicked1)
	ON_BN_CLICKED(IDC_BTN_SETUP_2, &CWndSubSettingsVlimitVgarant::OnBnClicked2)
	//ON_BN_CLICKED(IDC_BTN_SETUP_3, &CWndSubSettingsAutoOxyCal::OnBnClicked3)
	//ON_BN_CLICKED(IDC_BTN_SETUP_4, &CWndSubSettingsAutoOxyCal::OnBnClicked4)
	//ON_BN_CLICKED(IDC_BTN_SETUP_5, &CWndSubSettingsAutoOxyCal::OnBnClicked5)
	//ON_BN_CLICKED(IDC_BTN_SETUP_6, &CWndSubSettingsAutoOxyCal::OnBnClicked6)
	//ON_BN_CLICKED(IDC_BTN_SETUP_7, &CWndSubSettingsAutoOxyCal::OnBnClicked7)
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndSubSettingsVlimitVgarant message handlers
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsVlimitVgarant::Initialize()
{
	CClientDC dc(this);

	m_eVControl=getModel()->getCONFIG()->GetVolumeControl();

	m_szSetting = getModel()->GetLanguageString(IDS_PARA_V)
		+getModel()->GetLanguageString(IDS_PARA_LIMIT)
		+_T(" / ")
		+getModel()->GetLanguageString(IDS_PARA_V)
		+getModel()->GetLanguageString(IDS_PARA_GARANT);
	m_iNumValues=2;

	m_pbtn1->ShowWindow(SW_SHOW);
	m_pbtn1->RefreshText(getModel()->GetLanguageString(IDS_PARA_V)
		+getModel()->GetLanguageString(IDS_PARA_LIMIT));

	m_pbtn2->ShowWindow(SW_SHOW);
	m_pbtn2->RefreshText(getModel()->GetLanguageString(IDS_PARA_V)
		+getModel()->GetLanguageString(IDS_PARA_GARANT));

	m_plBtn.AddTail(m_pbtn1);
	m_plBtn.AddTail(m_pbtn2);


	switch(m_eVControl)
	{
	case VCTRL_VLIMIT:
		{
			m_pbtn1->DrawOK(true);
		}
		break;
	case VCTRL_VGARANT:
		{
			m_pbtn2->DrawOK(true);
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

void CWndSubSettingsVlimitVgarant::Draw()
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

	Rectangle(hdcMem, 120, 30, 520, 375);

	Rectangle(hdcMem, 140, 10, 500, 30);
	Rectangle(hdcMem, 140, 375, 500, 395);

	m_pcRegionTopLeCorner->Draw(hdcMem,120,10);
	m_pcRegionTopRiCorner->Draw(hdcMem,500,10);
	m_pcRegionBotLeCorner->Draw(hdcMem,120,375);
	m_pcRegionBotRiCorner->Draw(hdcMem,500,375);

	rc.left = 140;  
	rc.top = 25;  
	rc.right  = 500;  
	rc.bottom = 500;
	DrawText(hdcMem,m_szSetting,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	CPen penBlack;
	penBlack.CreatePen(PS_SOLID,2,RGB(0,0,0));
	SelectObject(hdcMem,(HPEN)penBlack);
	MoveToEx(hdcMem, 120, 60, NULL);
	LineTo(hdcMem, 520, 60);

	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	SelectObject(hdcMem,cbrRound);
	RoundRect(hdcMem, 130, 70, 510, 180,20,20);



	MoveToEx(hdcMem, 130, 126, NULL);
	LineTo(hdcMem, 510, 126);


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

BOOL CWndSubSettingsVlimitVgarant::PreTranslateMessage(MSG* pMsg) 
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
					SetControl(m_iCurPosInBlock);
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
				//TRACE(_T("focus changed UDParaBtn\r"));
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
 * Sets a control
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	btn	The button.
 **************************************************************************************************/

void CWndSubSettingsVlimitVgarant::SetControl(UINT btn)
{
	/*switch(btn)
	{
	case IDC_BTN_SETUP_1:
		{
			m_eVControl=VCTRL_VLIMIT;
		}
		break;
	case IDC_BTN_SETUP_2:
		{
			m_eVControl=VCTRL_VGARANT;
		}
		break;
	}

	getModel()->getCONFIG()->SetVolumeControl(m_eVControl);

	

	if(m_eVControl==VCTRL_VLIMIT && getModel()->getCONFIG()->GetParaDataVGarantState()==true)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_KILL_SETUPTIMER);

		

		getModel()->getDATAHANDLER()->SetCurrentVGarantParam(0, false, true);

		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	else if(m_eVControl==VCTRL_VGARANT && getModel()->getCONFIG()->GetParaDataVLimitState()==true)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_KILL_SETUPTIMER);

		

		getModel()->getDATAHANDLER()->SetCurrentVLimitParam(0, false, true);

		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
		
	}*/
}

/**********************************************************************************************//**
 * Executes the button clicked 1 action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsVlimitVgarant::OnBnClicked1()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_1);
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_1);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_1);
		SetControl(IDC_BTN_SETUP_1);
	}
}

/**********************************************************************************************//**
 * Executes the button clicked 2 action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsVlimitVgarant::OnBnClicked2()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_2);
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_2);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_2);
		SetControl(IDC_BTN_SETUP_2);
	}
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsVlimitVgarant::OnDestroy()
{
	CWndSubSettings::OnDestroy();
}



