// SelectGraphBtn.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "SelectGraphBtn.h"
#include "FabianHFO.h"
#include "globDefs.h"


// CSelectGraphBtn

IMPLEMENT_DYNAMIC(CSelectGraphBtn, CButton)

CSelectGraphBtn::CSelectGraphBtn(BTN btn,COLORREF crUp,COLORREF crDw,COLORREF crFc)
{
	memcpy(&m_btn,&btn,sizeof(BTN));
	// Default colors
	m_btncr.crTxtUp			= crUp;
	m_btncr.crTxtDown		= crDw;
	m_btncr.crTxtDisabled	= 0x00c0c0c0;		
	m_btncr.crTxtFocus		= crFc;	
	//
	m_hDC=NULL;
	m_hBm=NULL;
	m_hBmPrev=NULL;
	//
	m_pszText[0]=0x0000;
	//
	m_hFont=NULL;

	m_nXOffset=0;

	m_nNbr=-1;

	//m_bLMouseButtonDown=false;

	m_kUp					= VK_UP;
	m_kDown					= VK_DOWN;
	m_kSpace				= VK_SPACE;


	//m_pViewHandler = CMVViewHandler::GetInstance();
}

CSelectGraphBtn::~CSelectGraphBtn()
{
	//m_pViewHandler=NULL;

	if(m_hDC)
	{
		SelectObject(m_hDC,m_hBmPrev);
		DeleteObject(m_hBm);	 
		DeleteDC(m_hDC);
	}

	/*if(m_hFont)
	DeleteObject(m_hFont);*/
}

BEGIN_MESSAGE_MAP(CSelectGraphBtn, CButton)
	//{{AFX_MSG_MAP(CSelectGraphBtn)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
	//ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CSelectGraphBtn-Meldungshandler
BOOL CSelectGraphBtn::Create(CWnd* pParentWnd, HFONT hFont, int nXOffset, DWORD	dwStyle) 
{
	SIZE sz;
	sz.cx=m_btn.pcBmpUp->Width();
	sz.cy=m_btn.pcBmpUp->Height();
	CRect cRect(m_btn.poPosition,sz);

	//if(CButton::Create(_T("btn"),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW/*|WS_TABSTOP*/,cRect,pParentWnd,m_btn.wID))
	if(CButton::Create(_T("btn"),dwStyle,cRect,pParentWnd,m_btn.wID))
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
		return 1;
	}
	return 0;
}



void CSelectGraphBtn::SetText(TCHAR* pszText, int nNbr) 
{
	_tcscpy_s(m_pszText,_countof(m_pszText),pszText);
	m_nNbr=nNbr;
}
void CSelectGraphBtn::SetText(CStringW& szText, int nNbr) 
{
	_tcscpy_s(m_pszText,_countof(m_pszText),szText);
	m_nNbr=nNbr;
}
void CSelectGraphBtn::RefreshText(CStringW& szText, int nNbr) 
{
	SetText(szText,nNbr);
	Invalidate();
	UpdateWindow();
} 
void CSelectGraphBtn::RefreshText(TCHAR* pszText, int nNbr) 
{
	SetText(pszText,nNbr);
	Invalidate();
	UpdateWindow();
} 
void CSelectGraphBtn::SetChar(TCHAR t) 
{
	m_pszText[0]=t;
	m_pszText[1]=0x0000;
}

// **************************************************************************
// Painting
// **************************************************************************
void CSelectGraphBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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

void CSelectGraphBtn::Draw(int nState/*,TCHAR* psz*/)
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
			/*if(m_btn.wID==IDC_BTN_MENU_NUMERIC)
			{
			m_pViewHandler->BtnPushed(m_btn.wID,0);
			}*/
			break;
		case BTN_DW_DIS:
		default:
			nTxtColor=m_btncr.crTxtDisabled;
			break;
		}

		int nPrevTxtColor=SetTextColor(m_hDC,nTxtColor);

		RECT rc;
		memcpy(&rc,&m_rcClient,sizeof(RECT));

		

		//if(m_btn.dwFormat&DT_RIGHT)
		//	rc.right-=m_nXOffset;
		//else if(!(m_btn.dwFormat&DT_CENTER)) // left
		//	rc.left+=m_nXOffset;

		//		if(psz)
		//			DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat); not in use
		//		else

		CStringW sz(m_pszText);

		CDC* pDC=CDC::FromHandle(m_hDC);

		



		int iLen = sz.GetLength();

		int dyFont=pDC->GetTextExtent(_T("|")).cy-1;

		//int iX = 0;
		int iY = 0;

		RECT rc1;
		rc1.left=rc.left;
		rc1.right=rc.right-10;

		

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
		

		

		//DrawText(m_hDC,m_pszText,-1,&rc,m_btn.dwFormat);

		/*CDC* pDC=CDC::FromHandle(m_hDC);

		DrawRotatedText(pDC, m_pszText, &rc, 90, ETO_CLIPPED);*/

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

void CSelectGraphBtn::DrawRotatedText(CDC* pDC, const CStringW str, CRect rect, double angle, UINT nOptions)
{
	// convert angle to radian
	double pi = 3.141592654;
	double radian = pi * 2 / 360 * angle;

	// get the center of a not-rotated text
	CSize TextSize = pDC->GetTextExtent(str);
	CPoint center;
	center.x = TextSize.cx / 2;
	center.y = TextSize.cy / 2;

	// now calculate the center of the rotated text
	CPoint rcenter;
	rcenter.x = long(cos(radian) * center.x - sin(radian) * center.y);
	rcenter.y = long(sin(radian) * center.x + cos(radian) * center.y);

	// finally draw the text and move it to the center of the rectangle
	pDC->SetTextAlign(TA_BASELINE);
	pDC->SetBkMode(TRANSPARENT);
	pDC->ExtTextOut(rect.left + rect.Width() / 2 - rcenter.x, 
		rect.top + rect.Height() / 2 + rcenter.y,
		nOptions, rect, str, NULL);
}

void CSelectGraphBtn::SetBitmaps(CBmp* pcBmpUp,CBmp* pcBmpDown,CBmp* pcBmpDisabled,CBmp* pcBmpFocus)
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



void CSelectGraphBtn::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	{

		if(nChar==m_kSpace)	//VK_SPACE
		{
			if(IDC_BTN_PARAMENU==GetDlgCtrlID())
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETFOCUS_PIMARYVIEW);
				return;
			}
			else
			{
				if(GetParent())
					GetParent()->PostMessage(WM_SELECT_NEXTGRAPH,m_btn.wID,0);
			}
		}
		/*else if(nChar==m_kDown)	
		{
		}
		else if(nChar==m_kUp)	
		{
			
		}*/
		CButton::OnKeyUp(nChar,nRepCnt,nFlags);
	}
}