// DwnBtn.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "DwnBtn.h"
#include "globDefs.h"

// CDwnBtn

IMPLEMENT_DYNAMIC(CDwnBtn, CButton)

CDwnBtn::CDwnBtn(BTN btn,COLORREF cr, bool bTwoLine, bool bVerticalTxt) : CBtn(btn,cr)
{
	// Init state
	//m_bDepressed=false;
	m_bLMouseButtonDown=false;
	//m_bLButtonDown=false;
	m_pcBmpFocusDown=NULL;
	//m_pViewHandler = CMVViewHandler::GetInstance();

	m_bButtonIsActive=false;

	m_bTwoLine = bTwoLine;
	m_bVerticalTxt=bVerticalTxt;

	m_pszTextBottom[0]=0x0000;
	m_pszTextTop[0]=0x0000;
	m_pszTextBottomDown[0]=0x0000;
	m_pszTextTopDown[0]=0x0000;
	m_pszTextDown[0]=0x0000;
}

CDwnBtn::~CDwnBtn()
{
	//m_pViewHandler=NULL;
}


BEGIN_MESSAGE_MAP(CDwnBtn, CButton)
	//{{AFX_MSG_MAP(CDwnBtn)
	//ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	//ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuBtn message handlers
// **************************************************************************
// Painting
// **************************************************************************
void CDwnBtn::DrawDirectUp(void)
{
	m_bButtonIsActive=false;
	CClientDC dc(this);

	m_btn.pcBmpUp->Draw(m_hDC);
	Draw(BTN_DW_UP);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
}

void CDwnBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	//int iBtn = m_btn.wID;
	//if (m_bDepressed && !(lpDrawItemStruct->itemState & ODS_DISABLED))
	//{
	//	// Button down
	//	//if(lpDrawItemStruct->itemState & ODS_FOCUS)	
	//	{
	//		m_btn.pcBmpDown->Draw(m_hDC);
	//		Draw(BTN_DW_DW);
	//		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	//		/*if(m_pcBmpFocusDown)
	//		{
	//		m_pcBmpFocusDown->Draw(m_hDC);
	//		Draw(BTN_DW_DW);
	//		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	//		}
	//		else
	//		{
	//		m_btn.pcBmpFocus->Draw(m_hDC);
	//		Draw(BTN_DW_DW);
	//		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	//		}*/
	//	}
	//	/*else
	//	{
	//	m_bDepressed = false;
	//	m_btn.pcBmpUp->Draw(m_hDC);
	//	Draw(BTN_DW_UP);
	//	BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	//	}*/
	//}
	//else if(lpDrawItemStruct->itemState & ODS_DISABLED )
	//{
	//	// Button disabled
	//	if(!m_bDepressed)//up
	//	{
	//		m_btn.pcBmpUp->Draw(m_hDC);
	//		Draw(BTN_DW_DIS,BTN_DW_UP);
	//		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	//	}
	//	else // down
	//	{
	//		m_btn.pcBmpUp->Draw(m_hDC);
	//		Draw(BTN_DW_DIS,BTN_DW_DW);
	//		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	//	}
	//}
	//else	
	//{
	//	// Button up
	//	/*if(lpDrawItemStruct->itemState & ODS_FOCUS)
	//	{
	//		m_btn.pcBmpDown->Draw(m_hDC);
	//		Draw(BTN_DW_DW);
	//		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	//	}
	//	else*/
	//	{
	//		m_bDepressed = false;
	//		m_btn.pcBmpUp->Draw(m_hDC);
	//		Draw(BTN_DW_UP);
	//		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	//	}
	//}
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

void CDwnBtn::Draw(int nState,int nAddInfo)
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
	//DrawText(m_hDC,t,-1,&rc,m_btn.dwFormat);

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
void CDwnBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( !m_bLMouseButtonDown )
	{
		/*if(m_bButtonIsActive &&
			(	m_btn.wID == IDC_BTN_SIMVPSV_ON
			||	m_btn.wID == IDC_BTN_SIMVPSV_OFF
			||	m_btn.wID == IDC_BTN_FRESHGAS_INT
			||	m_btn.wID == IDC_BTN_FRESHGAS_EXT
			||	m_btn.wID == IDC_BTN_SHOW_VOLUMEGUARANTEE
			||	m_btn.wID == IDC_BTN_SHOW_VOLUMELIMIT))
		{
			return;
		}
		else*/
		{
			if(GetParent())
				GetParent()->PostMessage(WM_MENUBUTTONDOWN,m_btn.wID,0);
			m_bLMouseButtonDown = true;
			CButton::OnLButtonDown(nFlags, point);
			m_bButtonIsActive = !m_bButtonIsActive;
		}
		
	}
}

void CDwnBtn::OnLButtonUp(UINT nFlags, CPoint point) 
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
bool CDwnBtn::IsDepressed( void )
{
	// Return the buttons state
	//return m_bDepressed;
	return m_bButtonIsActive;
}

bool CDwnBtn::Depress(bool bDown)
{
	int iID=m_btn.wID;
	if(iID==IDC_BTN_FRESHGAS_INT)
	{
		int iStop=0;
	}
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
void CDwnBtn::SetText(TCHAR* pszText,TCHAR* pszTextDown,int nNbr) 
{
	_tcscpy(m_pszText,pszText);
	_tcscpy(m_pszTextDown,pszTextDown);
	m_nNbr=nNbr;
}

void CDwnBtn::RefreshText(TCHAR* pszText,TCHAR* pszTextDown,int nNbr) 
{
	SetText(pszText,pszTextDown,nNbr);
	Invalidate();
	UpdateWindow();
}
void CDwnBtn::SetText(CStringW& pszText,CStringW& pszTextDown,int nNbr) 
{
	_tcscpy(m_pszText,pszText);
	_tcscpy(m_pszTextDown,pszTextDown);
	m_nNbr=nNbr;
}

void CDwnBtn::RefreshText(CStringW& pszText,CStringW& pszTextDown,int nNbr) 
{
	SetText(pszText,pszTextDown,nNbr);
	Invalidate();
	UpdateWindow();
}

void CDwnBtn::SetText(CStringW& pszTextTop,CStringW& pszTextBottom,CStringW& pszTextTopDown,CStringW& pszTextBottomDown,int nNbr)

{
	_tcscpy_s(m_pszText,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextTop,_countof(m_pszText),pszTextTop);
	_tcscpy_s(m_pszTextBottom,_countof(m_pszText),pszTextBottom);
	_tcscpy_s(m_pszTextTopDown,_countof(m_pszText),pszTextTopDown);
	_tcscpy_s(m_pszTextBottomDown,_countof(m_pszText),pszTextBottomDown);
	m_nNbr=nNbr;
}
void CDwnBtn::RefreshText(CStringW& pszTextTop,CStringW& pszTextBottom,CStringW& pszTextTopDown,CStringW& pszTextBottomDown,int nNbr)
{
	SetText(pszTextTop,pszTextBottom,pszTextTopDown,pszTextBottomDown,nNbr);
	Invalidate();
	UpdateWindow();
}


// **************************************************************************
// 
// **************************************************************************
int CDwnBtn::GetBtnId()
{
	return m_btn.wID;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CDwnBtn::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_LBUTTONDOWN:

		/*if(m_bButtonIsActive &&
			(	m_btn.wID == IDC_BTN_SIMVPSV_ON
			||	m_btn.wID == IDC_BTN_SIMVPSV_OFF
			||	m_btn.wID == IDC_BTN_FRESHGAS_INT
			||	m_btn.wID == IDC_BTN_FRESHGAS_EXT
			||	m_btn.wID == IDC_BTN_SHOW_VOLUMEGUARANTEE
			||	m_btn.wID == IDC_BTN_SHOW_VOLUMELIMIT))
		{
			return 1;
		}
		else
			m_bButtonIsActive = !m_bButtonIsActive;*/
		break;
	default:
		break;
	}
	return CButton::PreTranslateMessage(pMsg);
}


