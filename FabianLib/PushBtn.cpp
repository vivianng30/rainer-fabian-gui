// GraphsBtn.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "PushBtn.h"
#include "FabianHFO.h"
#include "globDefs.h"

// CPushBtn

extern HFONT g_hf8AcuBold;

IMPLEMENT_DYNAMIC(CPushBtn, CButton)

CPushBtn::CPushBtn(BTN &btn,COLORREF cr, bool bTwoLine)
{
	memcpy(&m_btn,&btn,sizeof(BTN));
	// Default colors
	m_btncr.crTxtUp			= cr;
	m_btncr.crTxtDown		= cr;
	//m_btncr.crTxtDisabled	= 0x00c0c0c0;	
	m_btncr.crTxtDisabled	= 0x00777777;
	m_btncr.crTxtFocus		= cr;	
	//
	m_hBm=NULL;
	m_hBmPrev=NULL;
	m_hDC=NULL;
	//
	m_pszText[0]=0x0000;
	m_pszTextTop[0]=0x0000;
	m_pszTextBottom[0]=0x0000;
	//
	m_hFont=NULL;

	m_nXOffset=0;

	m_nNbr=-1;

	//m_bLMouseButtonDown=false;

	m_bTwoLine = bTwoLine;

	m_bButtonDown=false;

	_tcscpy_s(m_pszText,_countof(m_pszText),_T(""));
	_tcscpy_s(m_pszTextTop,_countof(m_pszTextTop),_T(""));
	_tcscpy_s(m_pszTextBottom,_countof(m_pszTextBottom),_T(""));

	m_uTimerId=0;
}

CPushBtn::~CPushBtn()
{

	if(m_hDC)
	{
		SelectObject(m_hDC,m_hBmPrev);
		DeleteObject(m_hBm);	 
		DeleteDC(m_hDC);
	}

	
}

BEGIN_MESSAGE_MAP(CPushBtn, CButton)
	//{{AFX_MSG_MAP(CPushBtn)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	//ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	//ON_WM_SETFOCUS()
	//ON_WM_KILLFOCUS()
	//ON_WM_MOUSEMOVE()
	//ON_WM_TIMER()
END_MESSAGE_MAP()


// CPushBtn-Meldungshandler
BOOL CPushBtn::Create(CWnd* pParentWnd, HFONT hFont, int nXOffset, DWORD	dwStyle) 
{
	SIZE sz;
	sz.cx=m_btn.pcBmpUp->Width();
	sz.cy=m_btn.pcBmpUp->Height();
	CRect cRect(m_btn.poPosition,sz);

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


void CPushBtn::SetText(TCHAR* pszTextTop,TCHAR* pszTextBottom,int nNbr)
{
	_tcscpy_s(m_pszText,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextTop,_countof(m_pszTextTop),pszTextTop);
	_tcscpy_s(m_pszTextBottom,_countof(m_pszTextBottom),pszTextBottom);
	m_nNbr=nNbr;
}
void CPushBtn::RefreshText(TCHAR* pszTextTop,TCHAR* pszTextBottom,int nNbr)
{
	SetText(pszTextTop,pszTextBottom,nNbr);
	Invalidate();
	UpdateWindow();
}
void CPushBtn::SetText(CStringW& pszTextTop,CStringW& pszTextBottom,int nNbr)
{
	_tcscpy_s(m_pszText,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextTop,_countof(m_pszTextTop),pszTextTop);
	_tcscpy_s(m_pszTextBottom,_countof(m_pszTextBottom),pszTextBottom);
	m_nNbr=nNbr;
}
void CPushBtn::RefreshText(CStringW& pszTextTop,CStringW& pszTextBottom,int nNbr)
{
	SetText(pszTextTop,pszTextBottom,nNbr);
	Invalidate();
	UpdateWindow();
}
void CPushBtn::SetText(TCHAR* pszText, int nNbr) 
{
	_tcscpy_s(m_pszText,_countof(m_pszText),pszText);
	m_nNbr=nNbr;
}
void CPushBtn::SetText(CStringW& szText, int nNbr) 
{
	_tcscpy_s(m_pszText,_countof(m_pszText),szText);
	m_nNbr=nNbr;
}
void CPushBtn::RefreshText(CStringW& szText, int nNbr) 
{
	SetText(szText,nNbr);
	Invalidate();
	UpdateWindow();
} 
void CPushBtn::RefreshText(TCHAR* pszText, int nNbr) 
{
	SetText(pszText,nNbr);
	Invalidate();
	UpdateWindow();
} 
void CPushBtn::SetChar(TCHAR t) 
{
	m_pszText[0]=t;
	m_pszText[1]=0x0000;
}

void CPushBtn::DrawDirectUp()
{
	CClientDC dc(this);

	m_bButtonDown=false;

	m_btn.pcBmpUp->Draw(m_hDC);
	Draw(BTN_DW_UP);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
}

// **************************************************************************
// Painting
// **************************************************************************
void CPushBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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
		// Button up
		if(lpDrawItemStruct->itemState & ODS_FOCUS /*&& !m_bButtonDown*/)
		{
			m_btn.pcBmpFocus->Draw(m_hDC);
			Draw(BTN_DW_FOCUS);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
		else if(m_bButtonDown)
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


	// TODO: handle additional BTN properties (text, color, ...)

	//if (lpDrawItemStruct->itemState & ODS_SELECTED)
	//{
	//	// Button is down
	//	m_btn.pcBmpDown->Draw(lpDrawItemStruct->hDC);

	//}
	//else if(lpDrawItemStruct->itemState & ODS_DISABLED)
	//{
	//	// Button is disabled
	//	m_btn.pcBmpDisabled->Draw(lpDrawItemStruct->hDC);
	//}
	//else	
	//{
	//	// Button is up ...
	//	if(lpDrawItemStruct->itemState & ODS_FOCUS)
	//	{
	//		// ... and has focus
	//		m_btn.pcBmpFocus->Draw(lpDrawItemStruct->hDC);
	//	}
	//	else
	//	{
	//		// ... and has'nt focus
	//		m_btn.pcBmpUp->Draw(lpDrawItemStruct->hDC);
	//	}
	//}
}

void CPushBtn::Draw(int nState/*,TCHAR* psz*/)
{
	if(m_hFont)
	{
		CClientDC dc(this);

		HFONT hPrevFont=(HFONT)SelectObject(m_hDC,m_hFont);
		int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
		int nTxtColor;

		RECT rc/*,rcOxy*/;
		memcpy(&rc,&m_rcClient,sizeof(RECT));
		

		

		if(m_btn.dwFormat&DT_RIGHT)
			rc.right-=m_nXOffset;
		else if(!(m_btn.dwFormat&DT_CENTER)) // left
			rc.left+=m_nXOffset;

		switch(nState)
		{
		case BTN_DW_FOCUS:
			nTxtColor=m_btncr.crTxtFocus;
			break;
		case BTN_DW_UP:
			nTxtColor=m_btncr.crTxtUp;
			//nTxtColor=RGB(255,255,255);
			break;
		case BTN_DW_DW:
			nTxtColor=m_btncr.crTxtDown;
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
				rc.top=9;
				rc.bottom=m_rcClient.bottom/2+2;
				DrawText(m_hDC,m_pszTextTop,-1,&rc,m_btn.dwFormat);
				rc.top=m_rcClient.bottom/2+1;
				rc.bottom=m_rcClient.bottom-5;
				DrawText(m_hDC,m_pszTextBottom,-1,&rc,m_btn.dwFormat);
			}
			else
			{
				rc.top=6;
				rc.bottom=m_rcClient.bottom/2;
				DrawText(m_hDC,m_pszTextTop,-1,&rc,m_btn.dwFormat);
				rc.top=m_rcClient.bottom/2;
				rc.bottom=m_rcClient.bottom-7;
				DrawText(m_hDC,m_pszTextBottom,-1,&rc,m_btn.dwFormat);
			}
			
		}
		else
		{
			if(nState==BTN_DW_DW)
			{
				rc.left+=2;
				rc.top+=2;
				DrawText(m_hDC,m_pszText,-1,&rc,m_btn.dwFormat);
			}
			else
			{
				DrawText(m_hDC,m_pszText,-1,&rc,m_btn.dwFormat);
			}

			//switch(m_btn.wID)
			//{
			//case IDC_BTN_CALOXY_CAL21:
			//	{
			//		memcpy(&rcOxy,&m_rcClient,sizeof(RECT));
			//		CSize szO = dc.GetTextExtent(_T("O"));
			//		if(nState==BTN_DW_DW)
			//		{
			//			rcOxy.left+=42;
			//			DrawText(m_hDC,_T("O"),-1,&rcOxy,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			//			rcOxy.left+=5+szO.cx;
			//			rcOxy.top+=12;
			//			DrawText(m_hDC,_T("2"),-1,&rcOxy,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			//			rcOxy.top-=12;
			//			rcOxy.right-=38;
			//			DrawText(m_hDC,_T("21%"),-1,&rcOxy,DT_VCENTER|DT_SINGLELINE|DT_RIGHT);
			//		}
			//		else
			//		{
			//			rcOxy.left+=40;
			//			DrawText(m_hDC,_T("O"),-1,&rcOxy,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			//			rcOxy.left+=5+szO.cx;
			//			rcOxy.top+=10;
			//			DrawText(m_hDC,_T("2"),-1,&rcOxy,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			//			rcOxy.top-=10;
			//			rcOxy.right-=40;
			//			DrawText(m_hDC,_T("21%"),-1,&rcOxy,DT_VCENTER|DT_SINGLELINE|DT_RIGHT);
			//		}


			//		/*DrawText(m_hDC,_T("O    21%"),-1,&rc,m_btn.dwFormat);
			//		rc.top=rc.top+10;
			//		SelectObject(m_hDC,g_hf8AcuBold);
			//		DrawText(m_hDC,_T("2        "),-1,&rc,m_btn.dwFormat);*/
			//	}
			//	break;
			//case IDC_BTN_CALOXY_CAL100:
			//	{
			//		memcpy(&rcOxy,&m_rcClient,sizeof(RECT));
			//		CSize szO = dc.GetTextExtent(_T("O"));
			//		if(nState==BTN_DW_DW)
			//		{
			//			rcOxy.left+=25;
			//			DrawText(m_hDC,_T("O"),-1,&rcOxy,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			//			rcOxy.left+=5+szO.cx;
			//			rcOxy.top+=12;
			//			DrawText(m_hDC,_T("2"),-1,&rcOxy,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			//			rcOxy.top-=12;
			//			rcOxy.right-=25;
			//			DrawText(m_hDC,_T("21+100%"),-1,&rcOxy,DT_VCENTER|DT_SINGLELINE|DT_RIGHT);
			//		}
			//		else
			//		{
			//			rcOxy.left+=23;
			//			DrawText(m_hDC,_T("O"),-1,&rcOxy,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			//			rcOxy.left+=5+szO.cx;
			//			rcOxy.top+=10;
			//			DrawText(m_hDC,_T("2"),-1,&rcOxy,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			//			rcOxy.top-=10;
			//			rcOxy.right-=27;
			//			DrawText(m_hDC,_T("21+100%"),-1,&rcOxy,DT_VCENTER|DT_SINGLELINE|DT_RIGHT);
			//		}
			//	}
			//	break;
			//default:
			//	{
			//		DrawText(m_hDC,m_pszText,-1,&rc,m_btn.dwFormat);
			//	}
			//	break;
			//}
			
		}

		SelectObject(m_hDC,m_hFont);
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

void CPushBtn::SetBitmaps(CBmp* pcBmpUp,CBmp* pcBmpDown,CBmp* pcBmpDisabled,CBmp* pcBmpFocus)
{
	if(m_btn.pcBmpUp!=pcBmpUp)
	{
		m_bButtonDown=false;

		m_btn.pcBmpUp=pcBmpUp;
		m_btn.pcBmpDown=pcBmpDown;
		m_btn.pcBmpDisabled=pcBmpDisabled;
		m_btn.pcBmpFocus=pcBmpFocus;
		Invalidate();
		UpdateWindow();


	}
}

void CPushBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( !m_bButtonDown )
	{
		if(GetParent())
			GetParent()->PostMessage(WM_MENUBUTTONDOWN,m_btn.wID,0);
			
		//DEBUGMSG(TRUE, (TEXT("OnLButtonDown\r\n")));

		m_bButtonDown = true;
 		CButton::OnLButtonDown(nFlags, point);

	}
}
//
//
void CPushBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bButtonDown )
	{
		if ( m_uTimerId != 0 ) 
		{   
			KillTimer( m_uTimerId ); 
			m_uTimerId = 0; 
		}

		//if(ChildWindowFromPoint(point)==FromHandle(this->m_hWnd))
		{
			if(GetParent())
				GetParent()->PostMessage(WM_MENUBUTTONUP,m_btn.wID,0);
		}
		//SetFocus();
		m_bButtonDown = false;

		//DEBUGMSG(TRUE, (TEXT("OnLButtonUp\r\n")));

		Invalidate();
		UpdateWindow();
		CButton::OnLButtonUp(nFlags, point);
	}
	m_bButtonDown = false;
}



