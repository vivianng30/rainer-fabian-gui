// WndSubSettingsCO2Module.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndSubSettingsCO2Module.h"



// CWndSubSettingsCO2Module

IMPLEMENT_DYNAMIC(CWndSubSettingsCO2Module, CWnd)

CWndSubSettingsCO2Module::CWndSubSettingsCO2Module(UINT viewFlag):
CWndSubSettings(viewFlag)
{
	m_byCO2Module=CO2MODULE_NONE;
}

CWndSubSettingsCO2Module::~CWndSubSettingsCO2Module()
{
	//getModel()->getCONFIG()->setCO2module(m_byCO2Module);
}


BEGIN_MESSAGE_MAP(CWndSubSettingsCO2Module, CWnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SETUP_1, &CWndSubSettingsCO2Module::OnBnClicked1)
	ON_BN_CLICKED(IDC_BTN_SETUP_2, &CWndSubSettingsCO2Module::OnBnClicked2)
	ON_BN_CLICKED(IDC_BTN_SETUP_3, &CWndSubSettingsCO2Module::OnBnClicked3)
END_MESSAGE_MAP()



// CWndSubSettingsCO2Module message handlers
// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsCO2Module::Initialize()
{
	CClientDC dc(this);

	m_byCO2Module=getModel()->getCONFIG()->getCO2module();
	getModel()->getCONFIG()->setTempCO2module(m_byCO2Module);

	m_szSetting = getModel()->GetLanguageString(IDS_BTN_MNU_CO2);
	
	if(getModel()->getCONFIG()->GetMainBoardVersion()<MAINBOARD_30)
	{
		if(		getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_ASCII
			||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_WAVE
			||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_IVOI
			||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL_REMOTE
			||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL_WAVE
			||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_ETHERNET_WAVE)
		{
			m_iNumValues=1;

			m_pbtn1->ShowWindow(SW_SHOW);
			m_pbtn1->RefreshText(getModel()->GetLanguageString(IDS_TXT_DISABLED));

			m_plBtn.AddTail(m_pbtn1);

			/*m_pbtn2->ShowWindow(SW_SHOW);
			m_pbtn2->RefreshText(_T("Respironics (Philips)"));

			m_pbtn3->ShowWindow(SW_SHOW);
			m_pbtn3->RefreshText(_T("MicroPod (Covidien)"));*/


			m_pbtn1->EnableWindow(FALSE);
			/*m_pbtn2->EnableWindow(FALSE);
			m_pbtn3->EnableWindow(FALSE);*/

			if(m_byCO2Module!=CO2MODULE_NONE)
			{
				m_byCO2Module=CO2MODULE_NONE;
				getModel()->getCONFIG()->setTempCO2module(m_byCO2Module);
				getModel()->getCONFIG()->setCO2module(m_byCO2Module);
			}
		}
		else
		{
			m_iNumValues=3;

			m_pbtn1->ShowWindow(SW_SHOW);
			m_pbtn1->RefreshText(getModel()->GetLanguageString(IDS_TXT_DISABLED));

			m_pbtn2->ShowWindow(SW_SHOW);
			m_pbtn2->RefreshText(_T("Respironics (Philips)"));

			m_pbtn3->ShowWindow(SW_SHOW);
			m_pbtn3->RefreshText(_T("MicroPod (Covidien)"));


			m_plBtn.AddTail(m_pbtn1);
			m_plBtn.AddTail(m_pbtn2);
			m_plBtn.AddTail(m_pbtn3);
		}
	}
	else
	{
		m_iNumValues=3;

		m_pbtn1->ShowWindow(SW_SHOW);
		m_pbtn1->RefreshText(getModel()->GetLanguageString(IDS_TXT_DISABLED));

		m_pbtn2->ShowWindow(SW_SHOW);
		m_pbtn2->RefreshText(_T("Respironics (Philips)"));

		m_pbtn3->ShowWindow(SW_SHOW);
		m_pbtn3->RefreshText(_T("MicroPod (Covidien)"));


		m_plBtn.AddTail(m_pbtn1);
		m_plBtn.AddTail(m_pbtn2);
		m_plBtn.AddTail(m_pbtn3);
	}




	switch(m_byCO2Module)
	{
	case CO2MODULE_NONE:
		{
			m_pbtn1->DrawOK(true);
		}
		break;
	case CO2MODULE_CAPNOSTAT:
		{
			m_pbtn2->DrawOK(true);
		}
		break;
	case CO2MODULE_MICROPOD:
		{
			m_pbtn3->DrawOK(true);
		}
		break;
	}
}

// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsCO2Module::Draw()
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
	//RoundRect(hdcMem, 210, 70, 590, 180,20,20);
	RoundRect(hdcMem, 210, 70, 590, 226,20,20);



	MoveToEx(hdcMem, 210, 126, NULL);
	LineTo(hdcMem, 590, 126);

	MoveToEx(hdcMem, 210, 172, NULL);
	LineTo(hdcMem, 590, 172);


	if(getModel()->getCONFIG()->GetMainBoardVersion()<MAINBOARD_30
		&& (getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_ASCII
		||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_WAVE
		||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_IVOI
		||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL_REMOTE
		||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL_WAVE
		||  getModel()->getCONFIG()->GetPDMSprotocol()==ACL_ETHERNET_WAVE))
	{
		SetTextColor(hdcMem,RGB(255,0,0));

		rc.left = 220;  
		rc.top = 250;  
		rc.right  = 580;  
		rc.bottom = 300;
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_PDMSACTIVE),-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
	}

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
BOOL CWndSubSettingsCO2Module::PreTranslateMessage(MSG* pMsg) 
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
					SetModule(m_iCurPosInBlock);
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
void CWndSubSettingsCO2Module::SetModule(UINT btn)
{
	switch(btn)
	{
	case IDC_BTN_SETUP_1:
		{
			m_byCO2Module=CO2MODULE_NONE;
		}
		break;
	case IDC_BTN_SETUP_2:
		{
			m_byCO2Module=CO2MODULE_CAPNOSTAT;
		}
		break;
	case IDC_BTN_SETUP_3:
		{
			m_byCO2Module=CO2MODULE_MICROPOD;
		}
		break;
	}
	getModel()->getCONFIG()->setTempCO2module(m_byCO2Module);
	//getModel()->getCONFIG()->setCO2module(m_byCO2Module,false);
}

// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsCO2Module::OnBnClicked1()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_1);
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_1);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_1);
		SetModule(IDC_BTN_SETUP_1);
	}
}
// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsCO2Module::OnBnClicked2()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_2);
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_2);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_2);
		SetModule(IDC_BTN_SETUP_2);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsCO2Module::OnBnClicked3()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_3);
	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_3);
	else
	{
		SetOneButtonDepressed(IDC_BTN_SETUP_3);
		SetModule(IDC_BTN_SETUP_3);
	}
}



// **************************************************************************
// 
// **************************************************************************
//void CWndSubSettingsCO2Module::writeData()
//{
//	getModel()->getCONFIG()->setCO2module(m_byCO2Module,true);
//}

//************************************
// Method:    OnDestroy
// FullName:  CWndSubSettingsCO2Module::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndSubSettingsCO2Module::OnDestroy()
{
	getModel()->getCONFIG()->setTempCO2module(m_byCO2Module);
	getModel()->getCONFIG()->setCO2module(m_byCO2Module);

	CWndSubSettings::OnDestroy();
}
