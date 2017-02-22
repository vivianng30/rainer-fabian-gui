// DTUpDwnBtn.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "DTUpDwnBtn.h"
#include "globDefs.h"

// CDTUpDwnBtn

IMPLEMENT_DYNAMIC(CDTUpDwnBtn, CButton)

// **************************************************************************
// De-, construction
// **************************************************************************
CDTUpDwnBtn::CDTUpDwnBtn(BTN &btn,COLORREF cr) : CBtn(btn,cr)
{
	// Init state
	m_bDepressed=false;
	m_bLMouseButtonDown=false;
	//m_bLButtonDown=false;
	m_pcBmpFocusDown=NULL;
	//m_pViewHandler = CMVViewHandler::GetInstance();
}

CDTUpDwnBtn::~CDTUpDwnBtn()
{
	//m_pViewHandler=NULL;
}


BEGIN_MESSAGE_MAP(CDTUpDwnBtn, CButton)
	//{{AFX_MSG_MAP(CDTUpDwnBtn)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	//ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDTUpDwnBtn message handlers
// **************************************************************************
// 
// **************************************************************************
void CDTUpDwnBtn::DrawDirectUp(void)
{
	CClientDC dc(this);

	m_btn.pcBmpUp->Draw(m_hDC);
	Draw(BTN_DW_UP);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
}

void CDTUpDwnBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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

void CDTUpDwnBtn::Draw(int nState,int nAddInfo)
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
void CDTUpDwnBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( !m_bLMouseButtonDown )
	{
		if(GetParent())
			GetParent()->PostMessage(WM_MENUBUTTONDOWN,m_btn.wID,0);
		m_bLMouseButtonDown = true;
		CButton::OnLButtonDown(nFlags, point);
	}
}

void CDTUpDwnBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bLMouseButtonDown )
	{
		if(ChildWindowFromPoint(point)==FromHandle(this->m_hWnd))
		{
			//m_bDepressed = true;
			m_bDepressed = !m_bDepressed;
		}
		SetFocus();
		CButton::OnLButtonUp(nFlags, point);
	}
	m_bLMouseButtonDown = false;
}
//void CDTUpDwnBtn::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
//{
//	if( !m_bLMouseButtonDown )
//	{
//		
//		m_bLMouseButtonDown = true;
//		CButton::OnKeyDown(nChar,nRepCnt,nFlags);
//	}
//}
//
//void CDTUpDwnBtn::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
//{
//	if( m_bLMouseButtonDown )
//	{
//		m_bDepressed = !m_bDepressed;
//		SetFocus();
//		CButton::OnKeyUp(nChar,nRepCnt,nFlags);
//	}
//	m_bLMouseButtonDown = false;
//}

// **************************************************************************
// Operation methods
// **************************************************************************
bool CDTUpDwnBtn::IsDepressed( void )
{
	// Return the buttons state
	return m_bDepressed;
}

bool CDTUpDwnBtn::Depress(bool bDown)
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

// **************************************************************************
// Button is very slow - this code permforms speed up - but
//              use of double click is not possible any more
// **************************************************************************
void CDTUpDwnBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	DWORD lParam;
	WORD  po[2]={(WORD)point.x,(WORD)point.y};
	memcpy(&lParam,po,sizeof(DWORD));

	SendMessage(WM_LBUTTONDOWN,nFlags,lParam);
	CButton::OnLButtonDblClk(nFlags, point);
}

// **************************************************************************
// 
// **************************************************************************
int CDTUpDwnBtn::GetBtnId()
{
	return m_btn.wID;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CDTUpDwnBtn::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_LBUTTONDOWN:
		{
			if(		m_btn.wID == IDC_BTN_SETUPHOUR_NEXTUP
				||	m_btn.wID == IDC_BTN_SETUPHOUR_NEXTDW
				||	m_btn.wID == IDC_BTN_SETUPMIN_NEXTUP
				||	m_btn.wID == IDC_BTN_SETUPMIN_NEXTDW
				||	m_btn.wID == IDC_BTN_SETUPYEAR_NEXTUP
				||	m_btn.wID == IDC_BTN_SETUPYEAR_NEXTDW
				||	m_btn.wID == IDC_BTN_SETUPMONTH_NEXTUP
				||	m_btn.wID == IDC_BTN_SETUPMONTH_NEXTDW
				||	m_btn.wID == IDC_BTN_SETUPDAY_NEXTUP
				||	m_btn.wID == IDC_BTN_SETUPDAY_NEXTDW
				||	m_btn.wID == IDC_BTN_SETUP_NEXTUP
				||	m_btn.wID == IDC_BTN_SETUP_NEXTDW
				||	m_btn.wID == IDC_BTN_NEBULIZER_TIMEUP
				||	m_btn.wID == IDC_BTN_NEBULIZER_TIMEDWN
				||	m_btn.wID == IDC_BTN_WAVE1_NEXTUP
				||	m_btn.wID == IDC_BTN_WAVE1_NEXTDWN
				||	m_btn.wID == IDC_BTN_WAVE2_NEXTUP
				||	m_btn.wID == IDC_BTN_WAVE2_NEXTDWN
				||	m_btn.wID == IDC_BTN_SETUP_BODYWEIGHT)
			{
				if(GetParent())
					GetParent()->PostMessage(WM_TIMEBTN_DOWN,m_btn.wID,0);
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			if(		m_btn.wID == IDC_BTN_SETUPHOUR_NEXTUP
				||	m_btn.wID == IDC_BTN_SETUPHOUR_NEXTDW
				||	m_btn.wID == IDC_BTN_SETUPMIN_NEXTUP
				||	m_btn.wID == IDC_BTN_SETUPMIN_NEXTDW
				||	m_btn.wID == IDC_BTN_SETUPYEAR_NEXTUP
				||	m_btn.wID == IDC_BTN_SETUPYEAR_NEXTDW
				||	m_btn.wID == IDC_BTN_SETUPMONTH_NEXTUP
				||	m_btn.wID == IDC_BTN_SETUPMONTH_NEXTDW
				||	m_btn.wID == IDC_BTN_SETUPDAY_NEXTUP
				||	m_btn.wID == IDC_BTN_SETUPDAY_NEXTDW
				||	m_btn.wID == IDC_BTN_SETUP_NEXTUP
				||	m_btn.wID == IDC_BTN_SETUP_NEXTDW
				||	m_btn.wID == IDC_BTN_NEBULIZER_TIMEUP
				||	m_btn.wID == IDC_BTN_NEBULIZER_TIMEDWN
				||	m_btn.wID == IDC_BTN_WAVE1_NEXTUP
				||	m_btn.wID == IDC_BTN_WAVE1_NEXTDWN
				||	m_btn.wID == IDC_BTN_WAVE2_NEXTUP
				||	m_btn.wID == IDC_BTN_WAVE2_NEXTDWN
				||	m_btn.wID == IDC_BTN_SETUP_BODYWEIGHT)
			{
				if(GetParent())
					GetParent()->PostMessage(WM_TIMEBTN_UP,m_btn.wID,0);
			}
		}
		break;
	default:
		break;
	}
	return CButton::PreTranslateMessage(pMsg);
}


