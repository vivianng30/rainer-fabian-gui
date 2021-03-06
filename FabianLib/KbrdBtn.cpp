// KbrdBtn.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "KbrdBtn.h"
#include "FabianHFO.h"
#include "globDefs.h"

/**********************************************************************************************//**
 * CKbrdBtn
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CKbrdBtn, CBtn)

/**********************************************************************************************//**
 * Initializes a new instance of the CKbrdBtn class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	btn			The button.
 * \param 		  	cr			The carriage return.
 * \param 		  	bTwoLine	True to two line.
 **************************************************************************************************/

CKbrdBtn::CKbrdBtn(BTN &btn,COLORREF cr, bool bTwoLine) : CBtn(btn,cr)
{
	memcpy(&m_btn,&btn,sizeof(BTN));
	// Default colors
	m_btncr.crTxtUp			= cr;
	m_btncr.crTxtDown		= cr;
	//m_btncr.crTxtDisabled	= 0x00c0c0c0;
	m_btncr.crTxtDisabled	= 0x00777777;
	m_btncr.crTxtFocus		= cr;

	m_hBm=NULL;
	m_hBmPrev=NULL;
	m_pszTextBottom[0]=0x0000;
	m_pszTextTop[0]=0x0000;

	//
	m_hDC=NULL;
	//
	m_pszText[0]=0x0000;
	//
	m_hFont=NULL;

	m_nXOffset=0;

	m_nNbr=-1;

	m_bLMouseButtonDown=false;

	m_bTwoLine = bTwoLine;

}

/**********************************************************************************************//**
 * Finalizes an instance of the CKbrdBtn class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

CKbrdBtn::~CKbrdBtn()
{

	if(m_hDC)
	{
		SelectObject(m_hDC,m_hBmPrev);
		DeleteObject(m_hBm);	 
		DeleteDC(m_hDC);
	}

	/*if(m_hFont)
	DeleteObject(m_hFont);*/
}

BEGIN_MESSAGE_MAP(CKbrdBtn, CBtn)
	//{{AFX_MSG_MAP(CKbrdBtn)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	//ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CKbrdBtn-Meldungshandler
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	hFont	  	The font.
 * \param 		  	nXOffset  	The x coordinate offset.
 * \param 		  	dwStyle   	The style.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CKbrdBtn::Create(CWnd* pParentWnd, HFONT hFont, int nXOffset, DWORD	dwStyle) 
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

/**********************************************************************************************//**
 * Sets a text
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pszTextTop   	The text top.
 * \param [in,out]	pszTextBottom	The text bottom.
 * \param 		  	nNbr		 	Number of.
 **************************************************************************************************/

void CKbrdBtn::SetText(CStringW& pszTextTop,CStringW& pszTextBottom,int nNbr)
{
	_tcscpy_s(m_pszText,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextTop,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextBottom,_countof(m_pszText),pszTextBottom);
	m_nNbr=nNbr;
}

/**********************************************************************************************//**
 * Sets a text
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pszTextTop   	If non-null, the text top.
 * \param [in,out]	pszTextBottom	If non-null, the text bottom.
 * \param 		  	nNbr		 	Number of.
 **************************************************************************************************/

void CKbrdBtn::SetText(TCHAR* pszTextTop,TCHAR* pszTextBottom,int nNbr)
{
	_tcscpy_s(m_pszText,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextTop,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextBottom,_countof(m_pszText),pszTextBottom);
	m_nNbr=nNbr;
}

/**********************************************************************************************//**
 * Refresh text
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pszTextTop   	If non-null, the text top.
 * \param [in,out]	pszTextBottom	If non-null, the text bottom.
 * \param 		  	nNbr		 	Number of.
 **************************************************************************************************/

void CKbrdBtn::RefreshText(TCHAR* pszTextTop,TCHAR* pszTextBottom,int nNbr)
{
	SetText(pszTextTop,pszTextBottom,nNbr);
	Invalidate();
	UpdateWindow();
}

/**********************************************************************************************//**
 * Sets a text
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pszText	If non-null, the text.
 * \param 		  	nNbr   	Number of.
 **************************************************************************************************/

void CKbrdBtn::SetText(TCHAR* pszText, int nNbr) 
{
	_tcscpy_s(m_pszText,_countof(m_pszText),pszText);
	m_nNbr=nNbr;
}

/**********************************************************************************************//**
 * Sets a text
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	szText	The text.
 * \param 		  	nNbr  	Number of.
 **************************************************************************************************/

void CKbrdBtn::SetText(CStringW& szText, int nNbr) 
{
	_tcscpy_s(m_pszText,_countof(m_pszText),szText);
	m_nNbr=nNbr;
}

/**********************************************************************************************//**
 * Gets a text
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	szText	If non-null, the text.
 **************************************************************************************************/

void CKbrdBtn::GetText(TCHAR* szText)
{
	_tcscpy_s(szText,_countof(m_pszText),m_pszText);
}

/**********************************************************************************************//**
 * Gets a text
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pszTextTop   	If non-null, the text top.
 * \param [in,out]	pszTextBottom	If non-null, the text bottom.
 **************************************************************************************************/

void CKbrdBtn::GetText(TCHAR* pszTextTop,TCHAR* pszTextBottom)
{
	_tcscpy_s(pszTextTop,_countof(m_pszText),m_pszTextTop);
	_tcscpy_s(pszTextBottom,_countof(m_pszText),m_pszTextBottom);
}

/**********************************************************************************************//**
 * Refresh text
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	szText	The text.
 * \param 		  	nNbr  	Number of.
 **************************************************************************************************/

void CKbrdBtn::RefreshText(CStringW& szText, int nNbr) 
{
	SetText(szText,nNbr);
	Invalidate();
	UpdateWindow();
} 

/**********************************************************************************************//**
 * Refresh text
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pszText	If non-null, the text.
 * \param 		  	nNbr   	Number of.
 **************************************************************************************************/

void CKbrdBtn::RefreshText(TCHAR* pszText, int nNbr) 
{
	SetText(pszText,nNbr);
	Invalidate();
	UpdateWindow();
} 

/**********************************************************************************************//**
 * Sets a character
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	t	A TCHAR to process.
 **************************************************************************************************/

void CKbrdBtn::SetChar(TCHAR t) 
{
	m_pszText[0]=t;
	m_pszText[1]=0x0000;
}

/**********************************************************************************************//**
 * Draw item
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	lpDrawItemStruct	The draw item structure.
 **************************************************************************************************/

void CKbrdBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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

/**********************************************************************************************//**
 * Draws
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nState	The state.
 **************************************************************************************************/

void CKbrdBtn::Draw(int nState/*,TCHAR* psz*/)
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
			nTxtColor=m_btncr.crTxtDisabled;
			break;
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

		if(m_bTwoLine)
		{
			rc.top=4;
			rc.bottom=m_rcClient.bottom/2;
			DrawText(m_hDC,m_pszTextTop,-1,&rc,m_btn.dwFormat);
			rc.top=m_rcClient.bottom/2;
			rc.bottom=m_rcClient.bottom-4;
			DrawText(m_hDC,m_pszTextBottom,-1,&rc,m_btn.dwFormat);
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
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pcBmpUp		 	If non-null, the PC bitmap up.
 * \param [in,out]	pcBmpDown	 	If non-null, the PC bitmap down.
 * \param [in,out]	pcBmpDisabled	If non-null, the PC bitmap disabled.
 * \param [in,out]	pcBmpFocus   	If non-null, the PC bitmap focus.
 **************************************************************************************************/

void CKbrdBtn::SetBitmaps(CBmp* pcBmpUp,CBmp* pcBmpDown,CBmp* pcBmpDisabled,CBmp* pcBmpFocus)
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
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nFlags	The flags.
 * \param	point 	The point.
 **************************************************************************************************/

void CKbrdBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	DWORD lParam;
	WORD  po[2]={(WORD)point.x,(WORD)point.y};
	memcpy(&lParam,po,sizeof(DWORD));

	SendMessage(WM_LBUTTONDOWN,nFlags,lParam);
	CButton::OnLButtonDblClk(nFlags, point);
}

/**********************************************************************************************//**
 * Executes the l button down action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nFlags	The flags.
 * \param	point 	The point.
 **************************************************************************************************/

void CKbrdBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( !m_bLMouseButtonDown )
	{
		m_bLMouseButtonDown = true;
		CButton::OnLButtonDown(nFlags, point);

	}
}

/**********************************************************************************************//**
 * Executes the l button up action
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nFlags	The flags.
 * \param	point 	The point.
 **************************************************************************************************/

void CKbrdBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bLMouseButtonDown )
	{
		CButton::OnLButtonUp(nFlags, point);

	}
	m_bLMouseButtonDown = false;
}



//void CKbrdBtn::OnSetFocus(CWnd* pOldWnd)
//{
//	//CButton::OnSetFocus(pOldWnd);
//	m_pViewHandler->FocusReceived(m_btn.wID,m_iGroupId);
//
//	// TODO: F�gen Sie hier Ihren Meldungsbehandlungscode ein.
//}

