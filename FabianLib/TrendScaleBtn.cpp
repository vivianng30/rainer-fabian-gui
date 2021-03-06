// GraphsBtn.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "TrendScaleBtn.h"
#include "FabianHFO.h"
#include "globDefs.h"

// CTrendScaleBtn

extern HFONT g_hf8AcuBold;

/**********************************************************************************************//**
 * Initializes a new instance of the TrendScaleBtn class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CTrendScaleBtn, CButton)

/**********************************************************************************************//**
 * Initializes a new instance of the CTrendScaleBtn class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	btn			The button.
 * \param	cr			The carriage return.
 * \param	bTwoLine	True to two line.
 **************************************************************************************************/

CTrendScaleBtn::CTrendScaleBtn(BTN btn,COLORREF cr, bool bTwoLine)
{
	memcpy(&m_btn,&btn,sizeof(BTN));
	// Default colors
	m_btncr.crTxtUp			= cr;
	m_btncr.crTxtDown		= cr;
	//m_btncr.crTxtDisabled	= 0x00c0c0c0;	
	m_btncr.crTxtDisabled	= 0x00777777;
	m_btncr.crTxtFocus		= cr;	
	//
	m_hDC=NULL;
	m_hBm=NULL;
	m_hBmPrev=NULL;

	m_nXOffset=0;
	//
	m_pszText[0]=0x0000;
	m_pszTextTop[0]=0x0000;
	m_pszTextBottom[0]=0x0000;
	//
	m_hFont=NULL;

	m_nNbr=-1;

	m_bTwoLine = bTwoLine;

	m_bButtonDown=false;

	_tcscpy_s(m_pszText,_countof(m_pszText),_T(""));
	_tcscpy_s(m_pszTextTop,_countof(m_pszTextTop),_T(""));
	_tcscpy_s(m_pszTextBottom,_countof(m_pszTextBottom),_T(""));

}

/**********************************************************************************************//**
 * Finalizes an instance of the CTrendScaleBtn class
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

CTrendScaleBtn::~CTrendScaleBtn()
{

	if(m_hDC)
	{
		SelectObject(m_hDC,m_hBmPrev);
		DeleteObject(m_hBm);	 
		DeleteDC(m_hDC);
	}

	
}

BEGIN_MESSAGE_MAP(CTrendScaleBtn, CButton)
	//{{AFX_MSG_MAP(CTrendScaleBtn)
	//ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	//ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CTrendScaleBtn-Meldungshandler
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

BOOL CTrendScaleBtn::Create(CWnd* pParentWnd, HFONT hFont, int nXOffset, DWORD	dwStyle) 
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

void CTrendScaleBtn::SetText(TCHAR* pszTextTop,TCHAR* pszTextBottom,int nNbr)
{
	_tcscpy_s(m_pszText,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextTop,_countof(m_pszTextTop),pszTextTop);
	_tcscpy_s(m_pszTextBottom,_countof(m_pszTextBottom),pszTextBottom);
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

void CTrendScaleBtn::RefreshText(TCHAR* pszTextTop,TCHAR* pszTextBottom,int nNbr)
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
 * \param [in,out]	pszTextTop   	The text top.
 * \param [in,out]	pszTextBottom	The text bottom.
 * \param 		  	nNbr		 	Number of.
 **************************************************************************************************/

void CTrendScaleBtn::SetText(CStringW& pszTextTop,CStringW& pszTextBottom,int nNbr)
{
	_tcscpy_s(m_pszText,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextTop,_countof(m_pszTextTop),pszTextTop);
	_tcscpy_s(m_pszTextBottom,_countof(m_pszTextBottom),pszTextBottom);
	m_nNbr=nNbr;
}

/**********************************************************************************************//**
 * Refresh text
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param [in,out]	pszTextTop   	The text top.
 * \param [in,out]	pszTextBottom	The text bottom.
 * \param 		  	nNbr		 	Number of.
 **************************************************************************************************/

void CTrendScaleBtn::RefreshText(CStringW& pszTextTop,CStringW& pszTextBottom,int nNbr)
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

void CTrendScaleBtn::SetText(TCHAR* pszText, int nNbr) 
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

void CTrendScaleBtn::SetText(CStringW& szText, int nNbr) 
{
	_tcscpy_s(m_pszText,_countof(m_pszText),szText);
	m_nNbr=nNbr;
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

void CTrendScaleBtn::RefreshText(CStringW& szText, int nNbr) 
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

void CTrendScaleBtn::RefreshText(TCHAR* pszText, int nNbr) 
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

void CTrendScaleBtn::SetChar(TCHAR t) 
{
	m_pszText[0]=t;
	m_pszText[1]=0x0000;
}

/**********************************************************************************************//**
 * Draw direct up
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 **************************************************************************************************/

void CTrendScaleBtn::DrawDirectUp()
{
	CClientDC dc(this);

	m_bButtonDown=false;

	m_btn.pcBmpUp->Draw(m_hDC);
	Draw(BTN_DW_UP);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
}

/**********************************************************************************************//**
 * Draw item
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	lpDrawItemStruct	The draw item structure.
 **************************************************************************************************/

void CTrendScaleBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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

/**********************************************************************************************//**
 * Draws
 *
 * \author	Rainer K�hner
 * \date	26.02.2018
 *
 * \param	nState	The state.
 **************************************************************************************************/

void CTrendScaleBtn::Draw(int nState/*,TCHAR* psz*/)
{
	if(m_hFont)
	{
		HFONT hPrevFont=(HFONT)SelectObject(m_hDC,m_hFont);
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
			switch(m_btn.wID)
			{
			case IDC_BTN_CALOXY_CAL21:
				{
					DrawText(m_hDC,_T("O    21%"),-1,&rc,m_btn.dwFormat);
					rc.top=rc.top+10;
					SelectObject(m_hDC,g_hf8AcuBold);
					DrawText(m_hDC,_T("2        "),-1,&rc,m_btn.dwFormat);
				}
				break;
			case IDC_BTN_CALOXY_CAL100:
				{
					DrawText(m_hDC,_T("O   100%"),-1,&rc,m_btn.dwFormat);
					rc.top=rc.top+10;
					SelectObject(m_hDC,g_hf8AcuBold);
					DrawText(m_hDC,_T("2          "),-1,&rc,m_btn.dwFormat);
				}
				break;
			default:
				{
					DrawText(m_hDC,m_pszText,-1,&rc,m_btn.dwFormat);
				}
				break;
			}

			
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

void CTrendScaleBtn::SetBitmaps(CBmp* pcBmpUp,CBmp* pcBmpDown,CBmp* pcBmpDisabled,CBmp* pcBmpFocus)
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

BOOL CTrendScaleBtn::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	/*case WM_KILLFOCUS:
		{
			DEBUGMSG(TRUE, (TEXT("WM_KILLFOCUS\r\n")));
		}
		break;
	case WM_MOUSEMOVE:
		{
			return 1;
		}
		break;*/
	case WM_LBUTTONDOWN:
		{
			CClientDC dc(this);
			m_btn.pcBmpDown->Draw(m_hDC);
			Draw(BTN_DW_DW);
			BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);

			m_bButtonDown=true;

			if(GetParent())
				GetParent()->PostMessage(WM_MENUBUTTONDOWN,m_btn.wID,0);

			//DEBUGMSG(TRUE, (TEXT("WM_MENUBUTTONDOWN\r\n")));
			return 1;
		}
		break;
	case WM_LBUTTONUP:
		{
			m_bButtonDown=false;

			CClientDC dc(this);
			m_btn.pcBmpUp->Draw(m_hDC);
			Draw(BTN_DW_UP);
			BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);

			if(GetParent())
				GetParent()->PostMessage(WM_MENUBUTTONUP,m_btn.wID,0);

			//DEBUGMSG(TRUE, (TEXT("WM_LBUTTONUP\r\n")));
			return 1;
		}
		break;
	default:
		break;
	}
	return CButton::PreTranslateMessage(pMsg);
}
//void CTrendScaleBtn::OnSetFocus(CWnd* pOldWnd)
//{
//	CButton::OnSetFocus(pOldWnd);
//
//	// TODO: Add your message handler code here
//}

//void CTrendScaleBtn::OnKillFocus(CWnd* pNewWnd)
//{
//	CButton::OnKillFocus(pNewWnd);
//
//	DEBUGMSG(TRUE, (TEXT("CTrendScaleBtn::OnKillFocus\r\n")));
//
//	if(m_bButtonDown)
//	{
//		DEBUGMSG(TRUE, (TEXT("CTrendScaleBtn::OnKillFocus m_bButtonDown\r\n")));
//	}
//	
//}