// Btn.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "Btn.h"

// CBtn

IMPLEMENT_DYNAMIC(CBtn, CButton)

/////////////////////////////////////////////////////////////////////////////
// CBtn
// **************************************************************************
// 
// **************************************************************************
CBtn::CBtn(BTN &btn,COLORREF crTxtUp, COLORREF crTxtDown, COLORREF crTxtDisabled, COLORREF crTxtFocus)
{
	memcpy(&m_btn,&btn,sizeof(BTN));
	// Default colors
	m_btncr.crTxtUp			= crTxtUp;
	m_btncr.crTxtDown		= crTxtDown;
	m_btncr.crTxtDisabled	= crTxtDisabled;
	m_btncr.crTxtFocus		= crTxtFocus;	
	//
	m_hDC=NULL;
	m_hBm=NULL;
	m_hBmPrev=NULL;
	//
	m_pszText[0]=0x0000;
	m_nXOffset=0;
	m_nYOffset=0;
	//
	m_hFont=NULL;

	m_bMoveDepressedText=false;

	m_nNbr=-1;
}

CBtn::~CBtn()
{
	if(m_hDC)
	{
		SelectObject(m_hDC,m_hBmPrev);
		DeleteObject(m_hBm);	 
		DeleteDC(m_hDC);
	}
}


BEGIN_MESSAGE_MAP(CBtn, CButton)
	//{{AFX_MSG_MAP(CBtn)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBtn message handlers
// **************************************************************************
// Creation
// **************************************************************************
BOOL CBtn::Create(CWnd* pParentWnd, HFONT hFont, int nXOffset, int nYOffset, bool bMoveDepressedText
				  ) 
{
	SIZE sz;
	sz.cx=m_btn.pcBmpUp->Width();
	sz.cy=m_btn.pcBmpUp->Height();
	CRect cRect(m_btn.poPosition,sz);

	m_bMoveDepressedText=bMoveDepressedText;

	if(CButton::Create(_T("btn"),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW/*|WS_TABSTOP*/,cRect,pParentWnd,m_btn.wID))
	{
		CClientDC dc(this);
		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBm=CreateCompatibleBitmap(dc.m_hDC,m_btn.pcBmpUp->Width(),m_btn.pcBmpUp->Height());
		m_hBmPrev=(HBITMAP)SelectObject(m_hDC,m_hBm);

		m_rcClient.left=m_rcClient.top=0;
		m_rcClient.right=sz.cx;
		m_rcClient.bottom=sz.cy;

		m_hFont=hFont;
		m_nXOffset=nXOffset;
		m_nYOffset=nYOffset;
		return 1;
	}
	return 0;
}
void CBtn::ReplaceFont(HFONT hFont) 
{
	m_hFont=hFont;
	Invalidate();
	UpdateWindow();
}
void CBtn::SetText(TCHAR* pszText, int nNbr) 
{
	_tcscpy(m_pszText,pszText);
	m_nNbr=nNbr;
}
void CBtn::SetText(CStringW& szText, int nNbr) 
{
	_tcscpy(m_pszText,szText);
	m_nNbr=nNbr;
}
void CBtn::RefreshText(CStringW& szText, int nNbr) 
{
	SetText(szText,nNbr);
	Invalidate();
	UpdateWindow();
} 
void CBtn::SetChar(TCHAR t) 
{
	m_pszText[0]=t;
	m_pszText[1]=0x0000;
}

// **************************************************************************
// Painting
// **************************************************************************
void CBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	//	Rectangle(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom);
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		// Button down
		m_btn.pcBmpDown->Draw(m_hDC);
		Draw(BTN_DW_DW);
		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	}
	else if(lpDrawItemStruct->itemState & ODS_DISABLED)
	{
		// Button disabled
		m_btn.pcBmpDisabled->Draw(m_hDC);
		Draw(BTN_DW_DIS);
		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	}
	else	
	{
		// Button up
		if(lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			m_btn.pcBmpFocus->Draw(m_hDC);
			Draw(BTN_DW_FOCUS);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
		else
		{
			m_btn.pcBmpUp->Draw(m_hDC);
			Draw(BTN_DW_UP);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
	}
}

void CBtn::Draw(int nState/*,TCHAR* psz*/)
{
	if(m_hFont)
	{
		HFONT hPrevFont=(HFONT)SelectObject(m_hDC,m_hFont);
		int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
		int nTxtColor;

		switch(nState)
		{
		case BTN_DW_FOCUS:
			nTxtColor=m_btncr.crTxtFocus;
			break;
		case BTN_DW_UP:
			nTxtColor=m_btncr.crTxtUp;
			break;
		case BTN_DW_DW:
			nTxtColor=m_btncr.crTxtDown;
			break;
		case BTN_DW_DIS:
		default:
			nTxtColor=m_btncr.crTxtDisabled;
			break;
		}

		int nPrevTxtColor=SetTextColor(m_hDC,nTxtColor);

		RECT rc;
		memcpy(&rc,&m_rcClient,sizeof(RECT));

		if(m_btn.dwFormat&DT_RIGHT)
			rc.right-=m_nXOffset;
		else if(!(m_btn.dwFormat&DT_CENTER)) // left
			rc.left+=m_nXOffset;

		//		if(psz)
		//			DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat); not in use
		//		else
		DrawText(m_hDC,m_pszText,-1,&rc,m_btn.dwFormat);

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
}




void CBtn::SetBitmaps(CBmp* pcBmpUp,CBmp* pcBmpDown,CBmp* pcBmpDisabled,CBmp* pcBmpFocus)
{
	if(m_btn.pcBmpUp!=pcBmpUp)
	{
		m_btn.pcBmpUp=pcBmpUp;
		m_btn.pcBmpDown=pcBmpDown;
		m_btn.pcBmpDisabled=pcBmpDisabled;
		m_btn.pcBmpFocus=pcBmpFocus;
		Invalidate();
		UpdateWindow();
	}
}

// **************************************************************************
// Button is very slow - this code permforms speed up - but
//              use of double click is not possible any more
// **************************************************************************
void CBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	DWORD lParam;
	WORD  po[2]={(WORD)point.x,(WORD)point.y};
	memcpy(&lParam,po,sizeof(DWORD));

	SendMessage(WM_LBUTTONDOWN,nFlags,lParam);
	CButton::OnLButtonDblClk(nFlags, point);
}


