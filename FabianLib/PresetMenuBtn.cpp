// PresetMenuBtn.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "PresetMenuBtn.h"
#include "globDefs.h"

// CPresetMenuBtn

IMPLEMENT_DYNAMIC(CPresetMenuBtn, CBtn)

/////////////////////////////////////////////////////////////////////////////
// CPresetMenuBtn
// **************************************************************************
// De-, construction
// **************************************************************************
CPresetMenuBtn::CPresetMenuBtn(BTN btn,COLORREF cr) : CBtn(btn,cr),m_eBtnState(UP)
{
	// Init state
	m_pszTextTop[0]=0x0000;
	m_pszTextBottom[0]=0x0000;
	//m_eBtnState=UP;
	//m_bDepressed=false;
	m_bLMouseButtonDown=false;
	m_bLButtonDown=false;
	m_pcBmpFocusDown=NULL;
	//m_pViewHandler = CMVViewHandler::GetInstance();

	m_bTwoLine=false;
}

CPresetMenuBtn::~CPresetMenuBtn()
{
	//m_pViewHandler=NULL;
}


BEGIN_MESSAGE_MAP(CPresetMenuBtn, CButton)
	//{{AFX_MSG_MAP(CPresetMenuBtn)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	//ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPresetMenuBtn message handlers
// **************************************************************************
// 
// **************************************************************************
void CPresetMenuBtn::DrawDirectUp(void)
{
	CClientDC dc(this);

	m_btn.pcBmpUp->Draw(m_hDC);
	Draw(BTN_DW_UP);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
}



void CPresetMenuBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	int iBtn = m_btn.wID;
	if (GetBtnState()==DOWN && !(lpDrawItemStruct->itemState & ODS_DISABLED))
	{
		// Button down
		m_btn.pcBmpDown->Draw(m_hDC);
		Draw(BTN_DW_DW);
		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	}
	else if(lpDrawItemStruct->itemState & ODS_DISABLED )
	{
		m_btn.pcBmpDisabled->Draw(m_hDC);
		Draw(BTN_DW_DIS);
		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);

		// Button disabled
		//if(GetBtnState()!=DOWN)//up
		//{
		//	m_btn.pcBmpUp->Draw(m_hDC);
		//	Draw(BTN_DW_DIS,BTN_DW_UP);
		//	BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		//}
		//else // down
		//{
		//	m_btn.pcBmpUp->Draw(m_hDC);
		//	Draw(BTN_DW_DIS,BTN_DW_DW);
		//	BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		//}
	}
	else	
	{
		// Button up
		if(lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			if(GetBtnState()==UP)//up
			{
				m_btn.pcBmpUp->Draw(m_hDC);
				Draw(BTN_DW_UP);
				BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
			}
			else if(GetBtnState()==PRESET)// down
			{
				m_btn.pcBmpFocus->Draw(m_hDC);
				Draw(BTN_DW_FOCUS);
				BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
			}
		}
		else
		{
			if(GetBtnState()==UP)//up
			{
				m_btn.pcBmpUp->Draw(m_hDC);
				Draw(BTN_DW_UP);
				BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
			}
			else if(GetBtnState()==PRESET)// down
			{
				m_btn.pcBmpFocus->Draw(m_hDC);
				Draw(BTN_DW_FOCUS);
				BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
			}
		}
	}
}

void CPresetMenuBtn::Draw(int nState,int nAddInfo)
{
	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,m_hFont);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nTxtColor;
	TCHAR* t;

	RECT rc;
	memcpy(&rc,&m_rcClient,sizeof(RECT));

	if(m_btn.dwFormat&DT_RIGHT)
		rc.right-=m_nXOffset;
	else if(!(m_btn.dwFormat&DT_CENTER)) // left
		rc.left+=m_nXOffset;

	switch(nState)
	{
	case BTN_DW_FOCUS:
		nTxtColor=m_btncr.crTxtUp;
		t=m_pszText;
		break;
	case BTN_DW_UP:
		nTxtColor=m_btncr.crTxtUp;
		//nTxtColor=RGB(255,255,255);
		t=m_pszText;
		break;
	case BTN_DW_DW:
		nTxtColor=m_btncr.crTxtDown;
		//t=m_pszTextDown;
		t=m_pszText;
		if(!m_bTwoLine)
		{
			rc.top+=3;
		}
		rc.left+=3;
		break;
	case BTN_DW_DIS:
	default:
		t=m_pszText;
		nTxtColor=m_btncr.crTxtDisabled;
		/*if(nAddInfo==BTN_DW_UP)
			t=m_pszText;
		else if(nAddInfo==BTN_DW_DW)
			t=m_pszTextDown;*/
		break;
	}
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColor);

	

	//	if(psz)
	//		DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat); not in use
	//	else
	if(m_bTwoLine)
	{
		if(nState==BTN_DW_DW)
		{
			rc.top=8;
			rc.bottom=m_rcClient.bottom/2+2;
			DrawText(m_hDC,m_pszTextTop,-1,&rc,m_btn.dwFormat);
			rc.top=m_rcClient.bottom/2+2;
			rc.bottom=m_rcClient.bottom-4;
			DrawText(m_hDC,m_pszTextBottom,-1,&rc,m_btn.dwFormat);
		}
		else
		{
			rc.top=6;
			rc.bottom=m_rcClient.bottom/2;
			DrawText(m_hDC,m_pszTextTop,-1,&rc,m_btn.dwFormat);
			rc.top=m_rcClient.bottom/2;
			rc.bottom=m_rcClient.bottom-6;
			DrawText(m_hDC,m_pszTextBottom,-1,&rc,m_btn.dwFormat);
		}
	}
	else
	{
		DrawText(m_hDC,t,-1,&rc,m_btn.dwFormat);
	}
	

	if(m_nNbr!=-1)
	{
		rc.left=0;
		rc.right=m_btn.pcBmpUp->Height(); // quaudrat!
		TCHAR pszNbr[8];
		wsprintf(pszNbr,_T("%d"),m_nNbr);
		DrawText(m_hDC,pszNbr,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);	
	}

	SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);

}
void CPresetMenuBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( !m_bLMouseButtonDown )
	{
		//DEBUGMSG(TRUE, (TEXT("xxxxxxxxxxxxOnLButtonDown is false %d\r\n"),(int)m_btn.wID));

		if(GetParent())
			GetParent()->PostMessage(WM_MENUBUTTONDOWN,m_btn.wID,GetBtnState());
		m_bLMouseButtonDown = true;
		CButton::OnLButtonDown(nFlags, point);
	}
	else
	{
		//DEBUGMSG(TRUE, (TEXT("xxxxxxxxxxxxOnLButtonDown is true %d\r\n"),(int)m_btn.wID));

	}
}

void CPresetMenuBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bLMouseButtonDown )
	{
		if(GetParent())
			GetParent()->PostMessage(WM_MENUBUTTONUP,m_btn.wID,0);
		CButton::OnLButtonUp(nFlags, point);
	}
	m_bLMouseButtonDown = false;

	//DEBUGMSG(TRUE, (TEXT("xxxxxxxxxxxxOnLButtonUp set false %d\r\n"),(int)m_btn.wID));

}



CPresetMenuBtn::eBtnState CPresetMenuBtn::GetBtnState()
{
	return m_eBtnState;
}
void CPresetMenuBtn::SetBtnState(eBtnState state, bool bReleaseButton)
{
	//if( state != m_eBtnState )
	{
		m_eBtnState = state;
		Invalidate(false);
		UpdateWindow();
	}
	if(bReleaseButton)
	{
		//int iStop=(int)m_btn.wID;
		m_bLMouseButtonDown = false;
		//DEBUGMSG(TRUE, (TEXT("xxxxxxxxxxxxSetBtnState set false %d\r\n"),(int)m_btn.wID));

	}
	//else
	//{
	//	int iStop=(int)m_btn.wID;
	//	//DEBUGMSG(TRUE, (TEXT("xxxxxxxxxxxxSetBtnState is false %d\r\n"),(int)m_btn.wID));

	//}
}

// **************************************************************************
// Button is very slow - this code permforms speed up - but
//              use of double click is not possible any more
// **************************************************************************
void CPresetMenuBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	DWORD lParam;
	WORD  po[2]={(WORD)point.x,(WORD)point.y};
	memcpy(&lParam,po,sizeof(DWORD));

	SendMessage(WM_LBUTTONDOWN,nFlags,lParam);
	CButton::OnLButtonDblClk(nFlags, point);
}

// **************************************************************************
// 
// **************************************************************************
void CPresetMenuBtn::SetText(TCHAR* pszText,/*TCHAR* pszTextDown,*/int nNbr) 
{
	_tcscpy(m_pszText,pszText);
	//_tcscpy(m_pszTextDown,pszTextDown);
	m_nNbr=nNbr;
}

void CPresetMenuBtn::RefreshText(TCHAR* pszText,/*TCHAR* pszTextDown,*/int nNbr) 
{
	SetText(pszText,/*pszTextDown,*/nNbr);
	Invalidate();
	UpdateWindow();
}

void CPresetMenuBtn::SetText(CStringW& pszText,/*CStringW& pszTextDown,*/int nNbr) 
{
	_tcscpy(m_pszText,pszText);
	//_tcscpy(m_pszTextDown,pszTextDown);
	m_nNbr=nNbr;
	m_bTwoLine=false;
}

void CPresetMenuBtn::RefreshText(CStringW& pszText,/*CStringW& pszTextDown,*/int nNbr) 
{
	SetText(pszText,/*pszTextDown,*/nNbr);
	Invalidate();
	UpdateWindow();
}

void CPresetMenuBtn::SetTextTwoLines(CStringW& pszTextTop,CStringW& pszTextBottom,int nNbr)
{
	_tcscpy_s(m_pszTextTop,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextBottom,_countof(m_pszText),pszTextBottom);
	m_nNbr=nNbr;
	m_bTwoLine=true;
}

void CPresetMenuBtn::RefreshTextTwoLines(CStringW& pszTextTop,CStringW& pszTextBottom,int nNbr)
{
	SetTextTwoLines(pszTextTop,pszTextBottom,nNbr);
	Invalidate();
	UpdateWindow();
}
// **************************************************************************
// 
// **************************************************************************
int CPresetMenuBtn::GetBtnId()
{
	return m_btn.wID;
}

