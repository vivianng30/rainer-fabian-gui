// UDBtn.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "UDBtn.h"

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTSUBBTNDW			0x00000000
#define COLOR_TXTBTNFC				0x00000000


// CUDBtn

IMPLEMENT_DYNAMIC(CUDBtn, CButton)

// **************************************************************************
// De-, constructio
// **************************************************************************
CUDBtn::CUDBtn(BTN btn, int nOffset, bool bScrollOver)
{
	m_kUp					= VK_UP;//up dw revers
	m_kDown					= VK_DOWN;
	m_kSpace				= VK_SPACE;

	memcpy(&m_btn,&btn,sizeof(BTN));
	// Default colors
	//m_btncr.crTxtUp			= 0x00000000;
	//m_btncr.crTxtDown		= 0x00000000;
	m_btncr.crTxtDisabled	= 0x00c0c0c0;;		
	//m_btncr.crTxtFocus		= 0x00000000;
	m_btncr.crTxtDown = COLOR_TXTBTNDW;
	m_btncr.crTxtUp = COLOR_TXTBTNUP;
	m_btncr.crTxtFocus = COLOR_TXTBTNFC;
	//
	m_hDC=NULL;
	m_hBm=NULL;
	m_hBmPrev=NULL;
	//
	m_pszText[0]=0x0000;
	//
	m_hFontText=NULL;
	m_hFontValue=NULL;

	m_v.nValue=0;
	m_v.nUpperLimit=0;
	m_v.nLowerLimit=0;

	

	m_bButtonIsActive=false;

	m_bTextOnly=false;

	m_nOffset=nOffset;

	//m_pl=NULL;

	m_bDisableText=false;

	m_bScrollOver=bScrollOver;
}

CUDBtn::~CUDBtn()
{
	if(m_hDC)
	{
		SelectObject(m_hDC,m_hBmPrev);
		DeleteObject(m_hBm);
		DeleteDC(m_hDC);
	}
}


BEGIN_MESSAGE_MAP(CUDBtn, CButton)
	//{{AFX_MSG_MAP(CUDBtn)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUDBtn message handlers
// **************************************************************************
// Creation
// **************************************************************************
BOOL CUDBtn::Create(CWnd* pParentWnd, HFONT hFontText, HFONT hFontValue,VALUE v) 
{
	SIZE sz;
	sz.cx=m_btn.pcBmpUp->Width();
	sz.cy=m_btn.pcBmpUp->Height();
	CRect cRect(m_btn.poPosition,sz);

	if(CButton::Create(_T("btn"),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW/*|WS_TABSTOP*/,cRect,pParentWnd,m_btn.wID))
	{
		CClientDC dc(this);
		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBm=CreateCompatibleBitmap(dc.m_hDC,m_btn.pcBmpUp->Width(),m_btn.pcBmpUp->Height());
		m_hBmPrev=(HBITMAP)SelectObject(m_hDC,m_hBm);

		m_rcClient.left=m_rcClient.top=0;
		m_rcClient.right=sz.cx;
		m_rcClient.bottom=sz.cy;

		m_hFontText=hFontText;
		m_hFontValue=hFontValue;
		memcpy(&m_v,&v,sizeof(VALUE));
		return 1;
	}
	return 0;
}
//BOOL CUDBtn::Create(CWnd* pParentWnd, HFONT hFont, LIST* pl, int nValue)
//{
//	m_pl=pl;
//	VALUE v;
//	v.nLowerLimit=pl->pli->nNbr;
//	v.nUpperLimit=pl->nSize-1;
//	v.nValue=nValue;
//	v.tText[0]=0x0000;
//	return Create(pParentWnd,hFont,v);
//}

void CUDBtn::SetColors(COLORREF cr, COLORREF crDisabel)
{
	m_btncr.crTxtUp			= cr;
	m_btncr.crTxtDown		= cr;
	m_btncr.crTxtDisabled	= crDisabel;		
	m_btncr.crTxtFocus		= cr;
}

void CUDBtn::SetValue(VALUE v, bool bRedraw) 
{
	memcpy(&m_v,&v,sizeof(VALUE));

	if(bRedraw)
	{
		Invalidate();
		UpdateWindow();
	}
}/*
 void CUDBtn::SetValue(int nValue, bool bRedraw) 
 {
 m_v.nValue=nValue;

 if(bRedraw)
 {
 Invalidate();
 UpdateWindow();
 }
 }*/

//void CUDBtn::SetList(LIST* pl, int nValue, bool bRedraw)
//{
//	m_pl=pl;
//
//	m_v.nLowerLimit=pl->pli->nNbr;
//	m_v.nUpperLimit=pl->nSize-1;
//	m_v.nValue=nValue;
//	m_v.tText[0]=0x0000;
//
//	if(bRedraw)
//	{
//		Invalidate();
//		UpdateWindow();
//	}	
//}

bool CUDBtn::DisableText(bool bDisable)
{
	m_bDisableText=bDisable;
	Invalidate();
	UpdateWindow();
	return true;
}

void CUDBtn::SetText(TCHAR* pszText,bool bTextOnly) 
{
	_tcscpy(m_pszText,pszText);
	m_bTextOnly=bTextOnly;
}
void CUDBtn::SetText(CStringW& szText,bool bTextOnly) 
{
	_tcscpy(m_pszText,szText);
	m_bTextOnly=bTextOnly;
}
void CUDBtn::DrawDirectDown(void)
{
	CClientDC dc(this);
	m_btn.pcBmpDown->Draw(m_hDC);
	Draw(BTN_DW_DW);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
}

void CUDBtn::DrawDirect(void)
{
	CClientDC dc(this);
	int i=GetState();
	if(i&0x0008)
	{
		m_btn.pcBmpDown->Draw(m_hDC);
		Draw(BTN_DW_DW);
		BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	}
	else if(i==0)
	{
		m_btn.pcBmpDisabled->Draw(m_hDC);
		Draw(BTN_DW_DIS);
		BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);

	}
	//	RETAILMSG(1, (_T("State %d\n\r"),i));
}


void CUDBtn::RefreshText(CStringW& szText,bool bTextOnly) 
{
	SetText(szText,bTextOnly);
	Invalidate();
	UpdateWindow();
}

bool CUDBtn::SetLimits(int nLower,int nUpper)
{
	m_v.nLowerLimit=nLower;
	m_v.nUpperLimit=nUpper;
	return true;
}

// **************************************************************************
// Painting
// **************************************************************************
void CUDBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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
		if(lpDrawItemStruct->itemState & ODS_FOCUS  && !m_bButtonIsActive)
		{
			m_btn.pcBmpFocus->Draw(m_hDC);
			Draw(BTN_DW_FOCUS);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
		else if(lpDrawItemStruct->itemState & ODS_FOCUS  && m_bButtonIsActive)//(down)
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

void CUDBtn::Draw(int nState)
{
	if(!m_bDisableText)
	{
		//if(m_hFont )
		{
			
			HFONT hPrevFont=(HFONT)SelectObject(m_hDC,m_hFontText);
			int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
			int nTxtColor;

			switch(nState)
			{
			case BTN_DW_FOCUS:
				//nTxtColor=m_btncr.crTxtFocus;
				nTxtColor=COLOR_TXTBTNFC;
				break;
			case BTN_DW_UP:
				//nTxtColor=m_btncr.crTxtUp;
				nTxtColor=COLOR_TXTBTNUP;
				break;
			case BTN_DW_DW:
				//nTxtColor=m_btncr.crTxtDown;
				nTxtColor=COLOR_TXTBTNDW;
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
				rc.right-=m_nOffset;
			else if(!(m_btn.dwFormat&DT_CENTER)) // left
				rc.left+=m_nOffset;

			TCHAR psz[MAX_PATH];

			
			if(_tcslen(m_pszText))
			{
				if(m_bTextOnly)
				{
					wsprintf(psz,_T("%s"),m_pszText);
					DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);
				}
				else
				{
					rc.top+=10;
					rc.bottom-=10;
					wsprintf(psz,_T("%s"),m_pszText);
					DrawText(m_hDC,psz,-1,&rc,DT_TOP|DT_CENTER);

					if(m_btn.wID==IDC_BTN_SERVICE_FLOWCORR_NEO)
					{
						double dbVal = (double)m_v.nValue/1000;
						//cs.Format(_T("%0.3f"), m_dbFCOR);
						SelectObject(m_hDC,m_hFontValue);
						wsprintf(psz,_T("%0.3f"),dbVal);
						DrawText(m_hDC,psz,-1,&rc,DT_BOTTOM|DT_CENTER);
					}
					else if(m_btn.wID==IDC_BTN_SERVICE_FLOWCORR_PED)
					{
						double dbVal = (double)m_v.nValue/1000;
						//cs.Format(_T("%0.3f"), m_dbFCOR);
						SelectObject(m_hDC,m_hFontValue);
						wsprintf(psz,_T("%0.3f"),dbVal);
						DrawText(m_hDC,psz,-1,&rc,DT_BOTTOM|DT_CENTER);
					}
					//pro, new - start
					else if(m_btn.wID==IDC_BTN_SERVICE_ALTITUDE)
					{ //TODO
						double dbVal = (double)m_v.nValue*100;
						//cs.Format(_T("%0.3f"), m_dbFCOR);
						SelectObject(m_hDC,m_hFontValue);
						wsprintf(psz,_T("%0.0f"),dbVal);
						DrawText(m_hDC,psz,-1,&rc,DT_BOTTOM|DT_CENTER);
					}
					//pro, new - end
					else
					{
						SelectObject(m_hDC,m_hFontValue);
						wsprintf(psz,_T("%d"),m_v.nValue);
						DrawText(m_hDC,psz,-1,&rc,DT_BOTTOM|DT_CENTER);
					}
					
				}
			}
			else
			{
				SelectObject(m_hDC,m_hFontValue);

				wsprintf(psz,_T("%d"),m_v.nValue);
				DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);
			}

			

			SetTextColor(m_hDC,nPrevTxtColor);
			SetBkMode(m_hDC,nBkMode);
			SelectObject(m_hDC,hPrevFont);
		}
	}
}

// **************************************************************************
// "Button" is very slow - this code permforms speed up - but
//              use of double click is not possible any more
// **************************************************************************
void CUDBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	DWORD lParam;
	WORD  po[2]={(WORD)point.x,(WORD)point.y};
	memcpy(&lParam,po,sizeof(DWORD));

	SendMessage(WM_LBUTTONDOWN,nFlags,lParam);
	CButton::OnLButtonDblClk(nFlags, point);
}

// **************************************************************************
// Button states
// **************************************************************************
BOOL CUDBtn::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_LBUTTONDOWN:
		m_bButtonIsActive = !m_bButtonIsActive;
		if(m_bButtonIsActive==false)
		{
			if(GetParent())
				GetParent()->SendMessage(WM_UPDATE_UDBTN,m_v.nValue,m_btn.wID);
		}
		break;
	case WM_RBUTTONDOWN:
		if(m_bButtonIsActive)
		{
			/////////////////////////////////////////////////////////////////
			if(m_v.nValue<m_v.nUpperLimit)
				m_v.nValue++;
			else if(m_bScrollOver)
				m_v.nValue=m_v.nLowerLimit;
			/////////////////////////////////////////////////////////////////
			if(m_bTextOnly)// notify parent by message
			{
				if(GetParent())
					GetParent()->SendMessage(WM_UPDATE_UDBTN,m_v.nValue,m_btn.wID);
			}
			else
			{
				CClientDC dc(this);
				m_btn.pcBmpDown->Draw(m_hDC);
				Draw(BTN_DW_DW);
				BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
			}
			return 1;
		}
		break;
	case WM_KEYDOWN:
		if(pMsg->wParam==m_kSpace)
		{
			m_bButtonIsActive = !m_bButtonIsActive;
			if(m_bButtonIsActive==false)
			{
				if(GetParent())
					GetParent()->SendMessage(WM_UPDATE_UDBTN,m_v.nValue,m_btn.wID);
			}
		}
		if(pMsg->wParam==m_kUp && m_bButtonIsActive)
		{
			/////////////////////////////////////////////////////////////////
			if(m_v.nValue<m_v.nUpperLimit)
			{
				m_v.nValue++;
			}
			else if(m_bScrollOver)
			{
				m_v.nValue=m_v.nLowerLimit;
			}
			/////////////////////////////////////////////////////////////////
			if(m_bTextOnly)// notify parent by message
			{
				//GetParent()->SendMessage(WM_UPDATE_UDBTN,m_v.nValue,m_btn.wID);
			}
			else
			{
				CClientDC dc(this);
				m_btn.pcBmpDown->Draw(m_hDC);
				Draw(BTN_DW_DW);
				BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
			}
			return 1;
		}
		if(pMsg->wParam==m_kDown && m_bButtonIsActive)
		{
			/////////////////////////////////////////////////////////////////
			if(m_v.nValue>m_v.nLowerLimit)
			{
				m_v.nValue--;
				//GetParent()->SendMessage(WM_UPDATE_UDBTN,m_v.nValue,m_btn.wID);
			}
			else if(m_bScrollOver)
			{
				m_v.nValue=m_v.nUpperLimit;
				//GetParent()->SendMessage(WM_UPDATE_UDBTN,m_v.nValue,m_btn.wID);
			}
			/////////////////////////////////////////////////////////////////
			if(m_bTextOnly)// notify parent by message
			{
				//GetParent()->SendMessage(WM_UPDATE_UDBTN,m_v.nValue,m_btn.wID);
			}
			else
			{
				CClientDC dc(this);
				m_btn.pcBmpDown->Draw(m_hDC);
				Draw(BTN_DW_DW);
				BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
			}
			return 1;
		}
		break;
	case WM_KEYUP:
		break;
	default:
		break;
	}
	return CButton::PreTranslateMessage(pMsg);
}

// **************************************************************************
// Get the button "value" and sta
// **************************************************************************
bool CUDBtn::GetButton(int* pnValue) 
{
	if(pnValue)
	{
		*pnValue=m_v.nValue;
	}
	return m_bButtonIsActive;
}

// **************************************************************************
// 
// **************************************************************************
void CUDBtn::SetUDKeys(WORD kUP, WORD kDown, WORD kSpace)
{
	m_kUp=kUP;
	m_kDown=kDown;
	m_kSpace=kSpace;
}

void CUDBtn::OnKillFocus(CWnd* pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);
	if(m_bButtonIsActive)
	{
		m_bButtonIsActive=false;
		if(GetParent())
			GetParent()->SendMessage(WM_UPDATE_UDBTN,m_v.nValue,m_btn.wID);
		//int iSt=0;
	}

	
}