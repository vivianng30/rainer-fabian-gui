// MenuBtn.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "MenuBtn.h"
#include "globDefs.h"

/**********************************************************************************************//**
 * CMenuBtn
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CMenuBtn, CButton)

/**********************************************************************************************//**
 * Initializes a new instance of the CMenuBtn class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	btn	The button.
 * \param	cr 	The carriage return.
 **************************************************************************************************/

CMenuBtn::CMenuBtn(BTN btn,COLORREF cr) : CBtn(btn,cr)
{
	// Init state
	
	m_bDepressed=false;
	m_bLMouseButtonDown=false;
	m_bLButtonDown=false;
	m_pcBmpFocusDown=NULL;
	m_dwLastMenudwn=0;

	m_pszTextDown[0]=0x0000;
	
}

/**********************************************************************************************//**
 * Finalizes an instance of the CMenuBtn class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CMenuBtn::~CMenuBtn()
{
}


BEGIN_MESSAGE_MAP(CMenuBtn, CButton)
	//{{AFX_MSG_MAP(CMenuBtn)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	//ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * ///////////////////////////////////////////////////////////////////////////
 *  CMenuBtn message handlers
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CMenuBtn::DrawDirectUp(void)
{
	m_bDepressed=false;
	CClientDC dc(this);

	m_btn.pcBmpUp->Draw(m_hDC);
	Draw(BTN_DW_UP);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
}

/**********************************************************************************************//**
 * Draw item
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	lpDrawItemStruct	The draw item structure.
 **************************************************************************************************/

void CMenuBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	//int iBtn = m_btn.wID;
	if (m_bDepressed && !(lpDrawItemStruct->itemState & ODS_DISABLED))
	{
		// Button down
		//if(lpDrawItemStruct->itemState & ODS_FOCUS)	
		{
			m_btn.pcBmpDown->Draw(m_hDC);
			Draw(BTN_DW_DW);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
			/*if(m_pcBmpFocusDown)
			{
				m_pcBmpFocusDown->Draw(m_hDC);
				Draw(BTN_DW_DW);
				BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
			}
			else
			{
				m_btn.pcBmpFocus->Draw(m_hDC);
				Draw(BTN_DW_DW);
				BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
			}*/
		}
		/*else
		{
			m_bDepressed = false;
			m_btn.pcBmpUp->Draw(m_hDC);
			Draw(BTN_DW_UP);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}*/
	}
	else if(lpDrawItemStruct->itemState & ODS_DISABLED )
	{
		m_btn.pcBmpDisabled->Draw(m_hDC);
		Draw(BTN_DW_DIS);
		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		
	}
	else	
	{
		// Button up
		if(lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			m_btn.pcBmpDown->Draw(m_hDC);
			Draw(BTN_DW_DW);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
		else
		{
			m_bDepressed = false;
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
 * \param	nState  	The state.
 * \param	nAddInfo	Information describing the add.
 **************************************************************************************************/

void CMenuBtn::Draw(int nState,int nAddInfo)
{
	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,m_hFont);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nTxtColor;
	TCHAR* pszText;

	RECT rc;
	memcpy(&rc,&m_rcClient,sizeof(RECT));

	if(m_btn.dwFormat&DT_RIGHT)
		rc.right-=m_nXOffset;
	else if(!(m_btn.dwFormat&DT_CENTER)) // left
		rc.left+=m_nXOffset;

	if(m_btn.dwFormat&DT_TOP)
		rc.top+=m_nYOffset;
	else if(!(m_btn.dwFormat&DT_VCENTER)) // bottom
		rc.bottom-=m_nYOffset;

	switch(nState)
	{
	case BTN_DW_FOCUS:
		//		nTxtColor=m_btncr.crTxtFocus;
		break;
	case BTN_DW_UP:
		nTxtColor=m_btncr.crTxtUp;
		//nTxtColor=RGB(255,255,255);
		pszText=m_pszText;
		break;
	case BTN_DW_DW:
		nTxtColor=m_btncr.crTxtDown;
		pszText=m_pszTextDown;
		if(m_bMoveDepressedText)
		{
			rc.top+=3;
			rc.left+=3;
		}
		
		break;
	case BTN_DW_DIS:
		nTxtColor=m_btncr.crTxtDisabled;
		pszText=m_pszText;
		break;
	default:
		nTxtColor=m_btncr.crTxtDisabled;
		if(nAddInfo==BTN_DW_UP)
			pszText=m_pszText;
		else if(nAddInfo==BTN_DW_DW)
			pszText=m_pszTextDown;
		break;
	}
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColor);

	CClientDC dc(this);

	switch(GetBtnId())
	{
	case IDC_BTN_GRAPH_CO2:
		{
			SelectObject(m_hDC,g_hf14AcuMed);
			CSize sz = dc.GetTextExtent(_T("etCO"));
			rc.left-=10;
			DrawText(m_hDC,_T("etCO"),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
			SelectObject(m_hDC,g_hf9AcuBold);
			rc.left+=24+sz.cx;
			rc.top+=7;
			DrawText(m_hDC,_T("2"),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
		break;
	case IDC_BTN_GRAPH_SPO2:
		{
			SelectObject(m_hDC,g_hf14AcuMed);
			CSize sz = dc.GetTextExtent(_T("SpO"));
			rc.left-=10;
			DrawText(m_hDC,_T("SpO"),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
			SelectObject(m_hDC,g_hf9AcuBold);
			rc.left+=24+sz.cx;
			rc.top+=7;
			DrawText(m_hDC,_T("2"),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
		break;
	default:
		{
			DrawText(m_hDC,pszText,-1,&rc,m_btn.dwFormat);
		}
		break;
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

/**********************************************************************************************//**
 * Executes the l button down action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nFlags	The flags.
 * \param	point 	The point.
 **************************************************************************************************/

void CMenuBtn::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CMenuBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bLMouseButtonDown )
	{
		if(GetParent())
			GetParent()->PostMessage(WM_MENUBUTTONUP,m_btn.wID,0);
		if(ChildWindowFromPoint(point)==FromHandle(this->m_hWnd))
			m_bDepressed = true;
		//SetFocus();
		CButton::OnLButtonUp(nFlags, point);

	}
	m_bLMouseButtonDown = false;
}
//void CMenuBtn::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
//{
//	if( !m_bLMouseButtonDown )
//	{
//		
//		m_bLMouseButtonDown = true;
//		CButton::OnKeyDown(nChar,nRepCnt,nFlags);
//	}
//}
//
//void CMenuBtn::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
//{
//	if( m_bLMouseButtonDown )
//	{
//		m_bDepressed = !m_bDepressed;
//		SetFocus();
//		CButton::OnKeyUp(nChar,nRepCnt,nFlags);
//	}
//	m_bLMouseButtonDown = false;
//}

/**********************************************************************************************//**
 * Query if this instance is depressed
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	True if depressed, false if not.
 **************************************************************************************************/

bool CMenuBtn::IsDepressed( void )
{
	// Return the buttons state
	return m_bDepressed;
}

/**********************************************************************************************//**
 * Depress
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	bDown	True to down.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMenuBtn::Depress(bool bDown)
{
	// Set the buttons state
	if( bDown != m_bDepressed )
	{
		m_bDepressed = bDown;
		Invalidate(true);
		UpdateWindow();
		return !m_bDepressed;
	}
	return m_bDepressed;
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

void CMenuBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	DWORD lParam;
	WORD  po[2]={(WORD)point.x,(WORD)point.y};
	memcpy(&lParam,po,sizeof(DWORD));

	SendMessage(WM_LBUTTONDOWN,nFlags,lParam);
	CButton::OnLButtonDblClk(nFlags, point);
}

/**********************************************************************************************//**
 * Sets a text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszText	   	If non-null, the text.
 * \param [in,out]	pszTextDown	If non-null, the text down.
 * \param 		  	nNbr	   	Number of.
 **************************************************************************************************/

void CMenuBtn::SetText(TCHAR* pszText,TCHAR* pszTextDown,int nNbr) 
{
	_tcscpy(m_pszText,pszText);
	_tcscpy(m_pszTextDown,pszTextDown);
	m_nNbr=nNbr;
}

/**********************************************************************************************//**
 * Refresh text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszText	   	If non-null, the text.
 * \param [in,out]	pszTextDown	If non-null, the text down.
 * \param 		  	nNbr	   	Number of.
 **************************************************************************************************/

void CMenuBtn::RefreshText(TCHAR* pszText,TCHAR* pszTextDown,int nNbr) 
{
	SetText(pszText,pszTextDown,nNbr);
	Invalidate();
	UpdateWindow();
}

/**********************************************************************************************//**
 * Sets a text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszText	   	The text.
 * \param [in,out]	pszTextDown	The text down.
 * \param 		  	nNbr	   	Number of.
 **************************************************************************************************/

void CMenuBtn::SetText(CStringW& pszText,CStringW& pszTextDown,int nNbr) 
{
	_tcscpy(m_pszText,pszText);
	_tcscpy(m_pszTextDown,pszTextDown);
	m_nNbr=nNbr;
}

/**********************************************************************************************//**
 * Refresh text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszText	   	The text.
 * \param [in,out]	pszTextDown	The text down.
 * \param 		  	nNbr	   	Number of.
 **************************************************************************************************/

void CMenuBtn::RefreshText(CStringW& pszText,CStringW& pszTextDown,int nNbr) 
{
	SetText(pszText,pszTextDown,nNbr);
	Invalidate();
	UpdateWindow();
}

/**********************************************************************************************//**
 * Gets button identifier
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The button identifier.
 **************************************************************************************************/

int CMenuBtn::GetBtnId()
{
	return m_btn.wID;
}

/**********************************************************************************************//**
 * Translates all messages before they are processed by the main message loop
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pMsg	If non-null, the message.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CMenuBtn::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_LBUTTONDOWN:
		{
			if(!isSafeTickCountDelayExpired(m_dwLastMenudwn, 1000))
			{
				return 1;
			}
			if(!m_bLButtonDown)
			{
				m_dwLastMenudwn=GetTickCount();
				m_bLButtonDown = true;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			m_bLButtonDown = false;
		}
		break;
	default:
		break;
	}
	return CButton::PreTranslateMessage(pMsg);
}

/**********************************************************************************************//**
 * Query if 'oldTickCount' is safe tick count delay expired
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	oldTickCount	Number of old ticks.
 * \param	delay			The delay.
 *
 * \return	True if safe tick count delay expired, false if not.
 **************************************************************************************************/

bool CMenuBtn::isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay)////used to check if old tick count plus delay is still lower than actual tickCount, (dwLastTickCount+DELAY<getTickCount64())
{
	bool bExpired=false;
	DWORD curTickCount=GetTickCount();

	if(curTickCount<oldTickCount)//overrun
	{
		DWORD diff=0xFFFFFFFF-oldTickCount;

		if(diff+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	else
	{
		if(oldTickCount+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	return bExpired;
}