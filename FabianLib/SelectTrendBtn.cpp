// SelectTrendBtn.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "SelectTrendBtn.h"
#include "FabianHFO.h"
#include "globDefs.h"

/**********************************************************************************************//**
 * CSelectTrendBtn
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CSelectTrendBtn, CButton)

/**********************************************************************************************//**
 * Initializes a new instance of the CSelectTrendBtn class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	btn	The button.
 * \param	cr 	The carriage return.
 **************************************************************************************************/

CSelectTrendBtn::CSelectTrendBtn(BTN btn,COLORREF cr) : CBtn(btn,cr)
{
	// Init state
	m_pszTextDown[0]=0x0000;
	m_bLMouseButtonDown=false;
	m_bLButtonDown=false;
	m_pcBmpFocusDown=NULL;
	//m_pViewHandler = CMVViewHandler::GetInstance();

	m_bDepressed=false;
	m_bLoadDataRunning=false;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CSelectTrendBtn class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CSelectTrendBtn::~CSelectTrendBtn()
{
	
}

BEGIN_MESSAGE_MAP(CSelectTrendBtn, CButton)
	//{{AFX_MSG_MAP(CSelectTrendBtn)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	//ON_WM_SETFOCUS()
	//ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CSelectTrendBtn-Meldungshandler
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CSelectTrendBtn::DrawDirectUp(void)
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

void CSelectTrendBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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
		// Button disabled
		//if(!m_bDepressed)//up
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
	
	//if (lpDrawItemStruct->itemState & ODS_SELECTED)
	//{
	//	// Button down
	//	m_btn.pcBmpDown->Draw(m_hDC);
	//	Draw(BTN_DW_DW);
	//	BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	//}
	//else if(lpDrawItemStruct->itemState & ODS_DISABLED)
	//{
	//	// Button disabled
	//	m_btn.pcBmpDisabled->Draw(m_hDC);
	//	Draw(BTN_DW_DIS);
	//	BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	//}
	//else	
	//{
	//	if(lpDrawItemStruct->itemState & ODS_FOCUS  && !m_bButtonIsActive)
	//	{
	//		m_btn.pcBmpFocus->Draw(m_hDC);
	//		Draw(BTN_DW_FOCUS);
	//		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	//	}
	//	else if(m_bButtonIsActive)//(down)
	//	{
	//		m_btn.pcBmpDown->Draw(m_hDC);
	//		Draw(BTN_DW_DW);
	//		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	//	}
	//	else
	//	{
	//		m_btn.pcBmpUp->Draw(m_hDC);
	//		Draw(BTN_DW_UP);
	//		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	//	}
	//}
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

void CSelectTrendBtn::Draw(int nState, int)
{
	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,m_hFont);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nTxtColor;
	//TCHAR* t;

	RECT rc;
	memcpy(&rc,&m_rcClient,sizeof(RECT));

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
		rc.top+=3;
		rc.left+=3;
		break;
	case BTN_DW_DIS:
	default:
		nTxtColor=m_btncr.crTxtDisabled;
		/*if(nAddInfo==BTN_DW_UP)
			t=m_pszText;
		else if(nAddInfo==BTN_DW_DW)
			t=m_pszTextDown;*/
		break;
	}
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColor);



	//if(m_btn.dwFormat&DT_RIGHT)
	//	rc.right-=m_nXOffset;
	//else if(!(m_btn.dwFormat&DT_CENTER)) // left
	//	rc.left+=m_nXOffset;

	////	if(psz)
	////		DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat); not in use
	////	else
	//DrawText(m_hDC,t,-1,&rc,m_btn.dwFormat);

	CStringW sz(m_pszText);

	CDC* pDC=CDC::FromHandle(m_hDC);

	int iLen = sz.GetLength();

	int dyFont=pDC->GetTextExtent(_T("|")).cy-1;

	//int iX = 0;
	int iY=0;

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

void CSelectTrendBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( !m_bLMouseButtonDown && !m_bLoadDataRunning)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_MENUBUTTONDOWN,m_btn.wID,0);
		m_bLMouseButtonDown = true;
		CButton::OnLButtonDown(nFlags, point);
	}
	else if(m_bLoadDataRunning)
	{
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
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

void CSelectTrendBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bLMouseButtonDown && !m_bLoadDataRunning)
	{
		if(ChildWindowFromPoint(point)==FromHandle(this->m_hWnd))
			m_bDepressed = true;
		CButton::OnLButtonUp(nFlags, point);
	}
	m_bLMouseButtonDown = false;
}

/**********************************************************************************************//**
 * Query if this instance is depressed
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	True if depressed, false if not.
 **************************************************************************************************/

bool CSelectTrendBtn::IsDepressed( void )
{
	// Return the buttons state
	//return m_bDepressed;
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

bool CSelectTrendBtn::Depress(bool bDown)
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

void CSelectTrendBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
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

void CSelectTrendBtn::SetText(TCHAR* pszText,TCHAR* pszTextDown,int nNbr) 
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

void CSelectTrendBtn::RefreshText(TCHAR* pszText,TCHAR* pszTextDown,int nNbr) 
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

void CSelectTrendBtn::SetText(CStringW& pszText,CStringW& pszTextDown,int nNbr) 
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

void CSelectTrendBtn::RefreshText(CStringW& pszText,CStringW& pszTextDown,int nNbr) 
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

int CSelectTrendBtn::GetBtnId()
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

BOOL CSelectTrendBtn::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_LBUTTONDOWN:
		{
			if(!m_bLButtonDown)
			{
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
 * Loads data running
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CSelectTrendBtn::LoadDataRunning(bool state)
{
	m_bLoadDataRunning=state;
}

