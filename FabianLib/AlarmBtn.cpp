// AlarmBtn.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "AlarmBtn.h"
#include "globDefs.h"

/**********************************************************************************************//**
 * CAlarmBtn
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CAlarmBtn, CButton)

/**********************************************************************************************//**
 * Initializes a new instance of the CAlarmBtn class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	btn			The button.
 * \param	cr			The carriage return.
 * \param	bTwoLine	True to two line.
 **************************************************************************************************/

CAlarmBtn::CAlarmBtn(const BTN &btn,COLORREF cr, bool bTwoLine):
m_hDC(NULL),m_hBmp(NULL),m_hBmpPrev(NULL),m_hFont(NULL),m_nNbr(-1),m_nXOffset(0),m_bTwoLine(false),m_bLMouseButtonDown(false)
{
	memcpy(&m_btn,&btn,sizeof(BTN));
	// Default colors
	m_btncr.crTxtUp			= cr;
	m_btncr.crTxtDown		= cr;
	m_btncr.crTxtDisabled	= 0x00c0c0c0;		
	m_btncr.crTxtFocus		= cr;	
	//
	m_pszText[0]=0x0000;
	m_pszTextTop[0]=0x0000;
	m_pszTextBottom[0]=0x0000;

	m_bTwoLine = bTwoLine;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CAlarmBtn class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CAlarmBtn::~CAlarmBtn()
{
	if(m_hDC)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);	 
		DeleteDC(m_hDC);
	}
}

BEGIN_MESSAGE_MAP(CAlarmBtn, CButton)
	//{{AFX_MSG_MAP(CAlarmBtn)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	//ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CAlarmBtn-Meldungshandler
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	hFont	  	The font.
 * \param 		  	nXOffset  	The x coordinate offset.
 * \param 		  	dwStyle   	The style.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CAlarmBtn::Create(CWnd* pParentWnd, HFONT hFont, int nXOffset, DWORD	dwStyle) 
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
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_btn.pcBmpUp->Width(),m_btn.pcBmpUp->Height());
		m_hBmpPrev=static_cast<HBITMAP>(SelectObject(m_hDC,m_hBmp));

		m_rcClient.left=m_rcClient.top=0;
		m_rcClient.right=sz.cx;
		m_rcClient.bottom=sz.cy;

		m_hFont=hFont;
		m_nXOffset=nXOffset;
		return 1;
	}
	return 0;
}

/**********************************************************************************************//**
 * Sets a text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszTextTop   	If non-null, the text top.
 * \param [in,out]	pszTextBottom	If non-null, the text bottom.
 * \param 		  	nNbr		 	Number of.
 * \param 		  	bRefresh	 	True to refresh.
 **************************************************************************************************/

void CAlarmBtn::SetText(TCHAR* pszTextTop,TCHAR* pszTextBottom,int nNbr,bool bRefresh)
{
	_tcscpy_s(m_pszText,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextTop,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextBottom,_countof(m_pszText),pszTextBottom);
	m_nNbr=nNbr;
	if(bRefresh)
	{
		Invalidate();
		UpdateWindow();
	}
}

/**********************************************************************************************//**
 * Refresh text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszTextTop   	If non-null, the text top.
 * \param [in,out]	pszTextBottom	If non-null, the text bottom.
 * \param 		  	nNbr		 	Number of.
 **************************************************************************************************/

void CAlarmBtn::RefreshText(TCHAR* pszTextTop,TCHAR* pszTextBottom,int nNbr)
{
	SetText(pszTextTop,pszTextBottom,nNbr);
	Invalidate();
	UpdateWindow();
}

/**********************************************************************************************//**
 * Sets a text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszText 	If non-null, the text.
 * \param 		  	nNbr		Number of.
 * \param 		  	bRefresh	True to refresh.
 **************************************************************************************************/

void CAlarmBtn::SetText(TCHAR* pszText, int nNbr,bool bRefresh) 
{
	_tcscpy_s(m_pszText,_countof(m_pszText),pszText);
	m_nNbr=nNbr;
	if(bRefresh)
	{
		Invalidate();
		UpdateWindow();
	}
}

/**********************************************************************************************//**
 * Sets a text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	pszTextTop   	The text top.
 * \param	pszTextBottom	The text bottom.
 * \param	nNbr		 	Number of.
 * \param	bRefresh	 	True to refresh.
 **************************************************************************************************/

void CAlarmBtn::SetText(const CStringW& pszTextTop,const CStringW& pszTextBottom,int nNbr,bool bRefresh)
{
	_tcscpy_s(m_pszText,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextTop,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextBottom,_countof(m_pszText),pszTextBottom);
	m_nNbr=nNbr;
	if(bRefresh)
	{
		Invalidate();
		UpdateWindow();
	}
}

/**********************************************************************************************//**
 * Refresh text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszTextTop   	The text top.
 * \param [in,out]	pszTextBottom	The text bottom.
 * \param 		  	nNbr		 	Number of.
 **************************************************************************************************/

void CAlarmBtn::RefreshText(CStringW& pszTextTop,CStringW& pszTextBottom,int nNbr)
{
	SetText(pszTextTop,pszTextBottom,nNbr);
	Invalidate();
	UpdateWindow();
}

/**********************************************************************************************//**
 * Sets a text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	szText  	The text.
 * \param	nNbr		Number of.
 * \param	bRefresh	True to refresh.
 **************************************************************************************************/

void CAlarmBtn::SetText(const CStringW& szText, int nNbr,bool bRefresh) 
{
	_tcscpy_s(m_pszText,_countof(m_pszText),szText);
	m_nNbr=nNbr;
	if(bRefresh)
	{
		Invalidate();
		UpdateWindow();
	}
}

/**********************************************************************************************//**
 * Refresh text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	szText	The text.
 * \param 		  	nNbr  	Number of.
 **************************************************************************************************/

void CAlarmBtn::RefreshText(CStringW& szText, int nNbr) 
{
	SetText(szText,nNbr);
	Invalidate();
	UpdateWindow();
} 

/**********************************************************************************************//**
 * Refresh text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszText	If non-null, the text.
 * \param 		  	nNbr   	Number of.
 **************************************************************************************************/

void CAlarmBtn::RefreshText(TCHAR* pszText, int nNbr) 
{
	SetText(pszText,nNbr);
	Invalidate();
	UpdateWindow();
} 

/**********************************************************************************************//**
 * Sets a character
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	t	A TCHAR to process.
 **************************************************************************************************/

void CAlarmBtn::SetChar(TCHAR t) 
{
	m_pszText[0]=t;
	m_pszText[1]=0x0000;
}

/**********************************************************************************************//**
 * Draw item
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	lpDrawItemStruct	The draw item structure.
 **************************************************************************************************/

void CAlarmBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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

/**********************************************************************************************//**
 * Draws
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nState	The state.
 **************************************************************************************************/

void CAlarmBtn::Draw(int nState/*,TCHAR* psz*/)
{
	if(m_hFont)
	{
		HFONT hPrevFont=static_cast<HFONT>(SelectObject(m_hDC,m_hFont));
		int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
		int nTxtColor;

		RECT rc;
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

/**********************************************************************************************//**
 * Sets the bitmaps
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pcBmpUp		 	If non-null, the PC bitmap up.
 * \param [in,out]	pcBmpDown	 	If non-null, the PC bitmap down.
 * \param [in,out]	pcBmpDisabled	If non-null, the PC bitmap disabled.
 * \param [in,out]	pcBmpFocus   	If non-null, the PC bitmap focus.
 **************************************************************************************************/

void CAlarmBtn::SetBitmaps(CBmp* pcBmpUp,CBmp* pcBmpDown,CBmp* pcBmpDisabled,CBmp* pcBmpFocus)
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

/**********************************************************************************************//**
 * Executes the l button double clock action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nFlags	The flags.
 * \param	point 	The point.
 **************************************************************************************************/

void CAlarmBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	DWORD lParam;
	WORD  po[2]={static_cast<WORD>(point.x),static_cast<WORD>(point.y)};
	memcpy(&lParam,po,sizeof(DWORD));

	PostMessage(WM_LBUTTONDOWN,nFlags,lParam);
	CButton::OnLButtonDblClk(nFlags, point);
}

/**********************************************************************************************//**
 * Executes the l button down action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nFlags	The flags.
 * \param	point 	The point.
 **************************************************************************************************/

void CAlarmBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( !m_bLMouseButtonDown )
	{
		if(GetParent())
			GetParent()->PostMessage(WM_MENUBUTTONDOWN,m_btn.wID,0);

		m_bLMouseButtonDown = true;
		CButton::OnLButtonDown(nFlags, point);
	}
}

/**********************************************************************************************//**
 * Executes the l button up action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nFlags	The flags.
 * \param	point 	The point.
 **************************************************************************************************/

void CAlarmBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bLMouseButtonDown )
	{
		if(GetParent())
			GetParent()->PostMessage(WM_MENUBUTTONUP,m_btn.wID,0);
		CButton::OnLButtonUp(nFlags, point);
	}
	m_bLMouseButtonDown = false;
}



//void CAlarmBtn::OnSetFocus(CWnd* pOldWnd)
//{
//	//CButton::OnSetFocus(pOldWnd);
//	m_pViewHandler->FocusReceived(m_btn.wID,m_iGroupId);
//
//	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
//}

