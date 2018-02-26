// WndSubSettingsBTBforVT.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndSubSettingsBTBforVT.h"

/**********************************************************************************************//**
 * CWndSubSettingsBTBforVT
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndSubSettingsBTBforVT, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndSubSettingsBTBforVT class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	viewFlag	The view flag.
 **************************************************************************************************/

CWndSubSettingsBTBforVT::CWndSubSettingsBTBforVT(UINT viewFlag):
CWndSubSettings(viewFlag)
{
	m_bUseTveBTB=false;
	m_slider=NULL;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndSubSettingsBTBforVT class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

CWndSubSettingsBTBforVT::~CWndSubSettingsBTBforVT()
{
	if(m_slider)
		delete m_slider;
	m_slider=NULL;
}


BEGIN_MESSAGE_MAP(CWndSubSettingsBTBforVT, CWnd)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndSubSettingsBTBforVT message handlers
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsBTBforVT::Initialize()
{
	CClientDC dc(this);

	m_bUseTveBTB=getModel()->getCONFIG()->useTveBTB();

	m_slider = new CBitmapSlider();
	m_slider->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(430,80,579,120), 
		this,IDC_SLD_USETVEBTB);
	m_slider->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
	m_slider->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
	m_slider->SetRange( 0, 1 );
	if(m_bUseTveBTB)
	{
		m_slider->SetPos( 1 );
	}
	else
	{
		m_slider->SetPos( 0 );
	}
	m_slider->SetMargin( 5, 0, 6, 0 );
	m_slider->DrawFocusRect( TRUE );



	m_szSetting = getModel()->GetLanguageString(IDS_TXT_VTASBTB);

	m_iNumValues=0;
}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsBTBforVT::Draw()
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

	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	SelectObject(hdcMem,cbrRound);
	RoundRect(hdcMem, 210, 70, 590, 130,20,20);





	rc.left = 220;  
	rc.top = 90;  
	rc.right  = 600;  
	rc.bottom = 500;
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_VTASBTB),-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	CPen penBlack;
	penBlack.CreatePen(PS_SOLID,2,RGB(0,0,0));
	SelectObject(hdcMem,(HPEN)penBlack);
	MoveToEx(hdcMem, 200, 60, NULL);
	LineTo(hdcMem, 600, 60);

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

BOOL CWndSubSettingsBTBforVT::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				m_slider->SetFocus();

				if(GetParent())
					GetParent()->PostMessage(WM_SET_SETUPTIMER);
				//return 1;
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				if(m_slider->GetPos()!=0)
				{
					m_slider->SetPos( 0 );
					m_bUseTveBTB=false;
					getModel()->getCONFIG()->setUseTveBTB(m_bUseTveBTB);
				}

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
				if(m_slider->GetPos()!=1)
				{
					m_slider->SetPos( 1 );
					m_bUseTveBTB=true;
					getModel()->getCONFIG()->setUseTveBTB(m_bUseTveBTB);
				}
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
 * Executes the destroy action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndSubSettingsBTBforVT::OnDestroy()
{
	CWndSubSettings::OnDestroy();
}

/**********************************************************************************************//**
 * Executes my message action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	wParam	The wParam field of the message.
 * \param	lParam	The lParam field of the message.
 *
 * \return	A LRESULT.
 **************************************************************************************************/

LRESULT CWndSubSettingsBTBforVT::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case IDC_SLD_USETVEBTB:
		{
			if(lParam==0)
			{
				m_bUseTveBTB=false;
			}
			else
			{
				m_bUseTveBTB=true;
			}

			getModel()->getCONFIG()->setUseTveBTB(m_bUseTveBTB);
			Draw();

			if(GetParent())
				GetParent()->PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	}
	return 1;
}





