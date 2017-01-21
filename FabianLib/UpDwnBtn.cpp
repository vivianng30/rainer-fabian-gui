// UpDwnBtn.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "UpDwnBtn.h"
#include "globDefs.h"

// CUpDwnBtn

IMPLEMENT_DYNAMIC(CUpDwnBtn, CButton)

/////////////////////////////////////////////////////////////////////////////
// CMenuBtn
// **************************************************************************
// De-, construction
// **************************************************************************
CUpDwnBtn::CUpDwnBtn(BTN btn,COLORREF cr, bool bTwoLine, bool bVerticalTxt) : CBtn(btn,cr)
{
	m_pcBmpFocusDown=NULL;

	m_bLMouseButtonDown=false;
	m_bButtonIsActive=false;
	m_bTwoLine = bTwoLine;
	m_bVerticalTxt=bVerticalTxt;

	m_pszTextTop[0]=0x0000;
	m_pszTextBottom[0]=0x0000;
	m_pszTextBottomDown[0]=0x0000;
	m_pszTextTopDown[0]=0x0000;
	m_pszTextDown[0]=0x0000;
}

CUpDwnBtn::~CUpDwnBtn()
{
}


BEGIN_MESSAGE_MAP(CUpDwnBtn, CButton)
	//{{AFX_MSG_MAP(CUpDwnBtn)
	//ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	//ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuBtn message handlers
// **************************************************************************
// Painting
// **************************************************************************
void CUpDwnBtn::DrawDirectUp(void)
{
	m_bButtonIsActive=false;
	//m_bLMouseButtonDown = false;
	CClientDC dc(this);

	m_btn.pcBmpUp->Draw(m_hDC);
	Draw(BTN_DW_UP);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
}

void CUpDwnBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
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
		if(lpDrawItemStruct->itemState & ODS_FOCUS  && !m_bButtonIsActive)
		{
			m_btn.pcBmpFocus->Draw(m_hDC);
			Draw(BTN_DW_FOCUS);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
		else if(m_bButtonIsActive)//(down)
		{
			m_btn.pcBmpDown->Draw(m_hDC);
			Draw(BTN_DW_DW);
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

void CUpDwnBtn::Draw(int nState,int nAddInfo)
{
	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,m_hFont);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nTxtColor;
	//TCHAR* t;

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
		//t=m_pszText;
		break;
	case BTN_DW_UP:
		nTxtColor=m_btncr.crTxtUp;
		//t=m_pszText;
		break;
	case BTN_DW_DW:
		nTxtColor=m_btncr.crTxtDown;
		//t=m_pszTextDown;
		if(!m_bTwoLine)
		{
			rc.top+=3;
		}
		rc.left+=3;
		break;
	case BTN_DW_DIS:
	default:
		nTxtColor=m_btncr.crTxtDisabled;
		break;
	}
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColor);

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
	else if(m_bVerticalTxt)
	{
		CDC* pDC=CDC::FromHandle(m_hDC);

		CStringW sz(m_pszText);

		int iLen = sz.GetLength();
		int dyFont=pDC->GetTextExtent(_T("|")).cy-2;

		//int iX = 0;
		int iY = 0;

		RECT rc1;

		if(IDC_BTN_PARAMENUBCK==m_btn.wID)
		{
			rc1.left=rc.left+10;
			rc1.right=rc.right;
		}
		else
		{
			rc1.left=rc.left;
			rc1.right=rc.right-10;
		}
		

		if(iLen>0)
		{
			if(iLen%2==0)
			{
				iY = (rc.bottom/2)-(iLen/2)*dyFont;
			}
			else
			{
				if(iLen>1)
				{
					iY = (rc.bottom/2)-((((iLen-1)/2)*dyFont)+dyFont/2);
				}
				else
				{
					iY = (rc.bottom/2)-(dyFont/2);
				}
			}

			for(int i=1; i <= iLen; i++)
			{
				rc1.top=iY;
				rc1.bottom=iY+dyFont;

				CStringW sz1 = sz.Left(1);

				DrawText(m_hDC,sz1,-1,&rc1,DT_VCENTER|DT_SINGLELINE|DT_CENTER);

				sz = sz.Right(iLen-i);

				iY = rc1.bottom;
			}
		}
	}
	else
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
void CUpDwnBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( !m_bLMouseButtonDown )
	{
		if(GetParent())
			GetParent()->PostMessage(WM_MENUBUTTONDOWN,m_btn.wID,0);
		m_bLMouseButtonDown = true;
		CButton::OnLButtonDown(nFlags, point);
	}
}

void CUpDwnBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bLMouseButtonDown )
	{
		if(GetParent())
			GetParent()->PostMessage(WM_MENUBUTTONUP,m_btn.wID,0);
		CButton::OnLButtonUp(nFlags, point);
	}
	m_bLMouseButtonDown = false;
}


// **************************************************************************
// Operation methods
// **************************************************************************
bool CUpDwnBtn::IsDepressed( void )
{
	return m_bButtonIsActive;
}

bool CUpDwnBtn::Depress(bool bDown)
{
	//m_bLMouseButtonDown = false;
	// Set the buttons state
	if( bDown != m_bButtonIsActive )
	{
		m_bButtonIsActive = bDown;
		Invalidate(true);
		UpdateWindow();
		return !m_bButtonIsActive;
	}
	return m_bButtonIsActive;
}


// **************************************************************************
// 
// **************************************************************************
void CUpDwnBtn::SetText(TCHAR* pszText,TCHAR* pszTextDown,int nNbr) 
{
	_tcscpy(m_pszText,pszText);
	_tcscpy(m_pszTextDown,pszTextDown);
	m_nNbr=nNbr;
}

void CUpDwnBtn::RefreshText(TCHAR* pszText,TCHAR* pszTextDown,int nNbr) 
{
	SetText(pszText,pszTextDown,nNbr);
	Invalidate();
	UpdateWindow();
}
void CUpDwnBtn::SetText(CStringW& pszText,CStringW& pszTextDown,int nNbr) 
{
	_tcscpy(m_pszText,pszText);
	_tcscpy(m_pszTextDown,pszTextDown);
	m_nNbr=nNbr;
}

void CUpDwnBtn::RefreshText(CStringW& pszText,CStringW& pszTextDown,int nNbr) 
{
	SetText(pszText,pszTextDown,nNbr);
	Invalidate();
	UpdateWindow();
}

void CUpDwnBtn::SetText(CStringW& pszTextTop,CStringW& pszTextBottom,CStringW& pszTextTopDown,CStringW& pszTextBottomDown,int nNbr)

{
	_tcscpy_s(m_pszText,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextTop,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextBottom,_countof(m_pszText),pszTextBottom);
	_tcscpy_s(m_pszTextTopDown,_countof(m_pszText),pszTextTopDown);
	_tcscpy_s(m_pszTextBottomDown,_countof(m_pszText),pszTextBottomDown);
	m_nNbr=nNbr;
}
void CUpDwnBtn::RefreshText(CStringW& pszTextTop,CStringW& pszTextBottom,CStringW& pszTextTopDown,CStringW& pszTextBottomDown,int nNbr)
{
	SetText(pszTextTop,pszTextBottom,pszTextTopDown,pszTextBottomDown,nNbr);
	Invalidate();
	UpdateWindow();
}


// **************************************************************************
// 
// **************************************************************************
int CUpDwnBtn::GetBtnId()
{
	return m_btn.wID;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CUpDwnBtn::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_LBUTTONDOWN:
		m_bButtonIsActive = !m_bButtonIsActive;	
		break;
	default:
		break;
	}
	return CButton::PreTranslateMessage(pMsg);
}



void CUpDwnBtn::OnKillFocus(CWnd* pNewWnd)
{
	CBtn::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
	m_bLMouseButtonDown = false;
}
