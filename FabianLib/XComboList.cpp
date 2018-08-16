// XComboList.cpp
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This software is released into the public domain.
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XComboList.h"


#ifdef _DEBUG

/**********************************************************************************************//**
 * A macro that defines new
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__; ///< this file[]
#endif

UINT NEAR WM_XCOMBOLIST_VK_RETURN = ::RegisterWindowMessage(_T("WM_XCOMBOLIST_VK_RETURN")); ///< The windows message xcombolist vk return
UINT NEAR WM_XCOMBOLIST_VK_ESCAPE = ::RegisterWindowMessage(_T("WM_XCOMBOLIST_VK_ESCAPE")); ///< The windows message xcombolist vk escape
UINT NEAR WM_XCOMBOLIST_KEYDOWN   = ::RegisterWindowMessage(_T("WM_XCOMBOLIST_KEYDOWN"));   ///< The windows message xcombolist keydown
UINT NEAR WM_XCOMBOLIST_LBUTTONUP = ::RegisterWindowMessage(_T("WM_XCOMBOLIST_LBUTTONUP")); ///< The windows message xcombolist lbuttonup

BEGIN_MESSAGE_MAP(CXComboList, CWnd)
	//{{AFX_MSG_MAP(CXComboList)
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	ON_WM_VSCROLL()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * Initializes a new instance of the CXComboList class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pParent	If non-null, the parent.
 **************************************************************************************************/

CXComboList::CXComboList(CWnd *pParent)
{
	m_pParent = pParent;
	ASSERT(m_pParent);
	m_nCount = 0;
	m_bFirstTime = TRUE;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CXComboList class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CXComboList::~CXComboList()
{
}

/**********************************************************************************************//**
 * Sets the active
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CXComboList::SetActive(/*int nScrollBarWidth*/)
{
	if (!::IsWindow(m_ListBox.m_hWnd))
		return;

	int nScrollBarWidth=40;
	m_ListBox.SetFocus();

	//if (m_bFirstTime)
	{
		m_bFirstTime = FALSE;

		//const int nScrollBarWidth = 11;	// this looks good, maybe should be parameter?

		/*CRect rect;
		GetWindowRect(&rect);*/

		// set listbox size according to item height
		/*m_ListBox.SetItemHeight(0,20);*/
		int nItemHeight = m_ListBox.GetItemHeight(0);

		CRect lbrect;
		GetClientRect(&lbrect);
		/*lbrect.top   += 1;
		lbrect.bottom = lbrect.top + (rect.Height() / nItemHeight) * nItemHeight;
		lbrect.left  += 1;*/
		lbrect.right -= nScrollBarWidth;

		int nItemsInView = (lbrect.Height()) / nItemHeight;

		// set size of listbox wrapper (from size of listbox)
		/*rect.bottom = rect.top + lbrect.Height() + 4;
		MoveWindow(&rect);*/
		m_ListBox.MoveWindow(&lbrect);
		m_ListBox.BringWindowToTop();

		// set size and position for vertical scroll bar
		CRect sbrect;
		sbrect = lbrect;
		sbrect.left   = lbrect.right;
		sbrect.right += nScrollBarWidth;
		m_wndSBVert.MoveWindow(&sbrect);

		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		m_wndSBVert.GetScrollInfo(&si);

		// set info for scrollbar
		si.nMin = 0;
		si.nMax = m_ListBox.GetCount();
		if (si.nMax < 0)
			si.nMax = 1;
		si.nPage = nItemsInView;
		int nCurSel = m_ListBox.GetCurSel();
		if (nCurSel == LB_ERR || nCurSel < 0)
			nCurSel = 0;
		si.nPos = nCurSel;

		// set top index, to force selected item to be in view
		m_ListBox.SetTopIndex(nCurSel > 0 ? nCurSel - 1 : 0);

		if (si.nPos < 0)
			si.nPos = 0;
		m_wndSBVert.SetScrollInfo(&si);
		m_wndSBVert.SetScrollPos(si.nPos, TRUE);

		RedrawWindow();
	}
}

/**********************************************************************************************//**
 * Gets scroll bar control
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nBar	The bar.
 *
 * \return	Null if it fails, else the scroll bar control.
 **************************************************************************************************/

CScrollBar* CXComboList::GetScrollBarCtrl(int nBar)
{
	UNUSED_ALWAYS(nBar);
	return &m_wndSBVert;
}

/**********************************************************************************************//**
 * Sends a registered message
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nMsg  	The message.
 * \param	wParam	The wParam field of the message.
 * \param	lParam	The lParam field of the message.
 **************************************************************************************************/

void CXComboList::SendRegisteredMessage(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	CWnd *pWnd = m_pParent;
	if (pWnd)
		pWnd->SendMessage(nMsg, wParam, lParam);
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

void CXComboList::OnLButtonDown(UINT nFlags, CPoint point)
{
	SendRegisteredMessage(WM_XCOMBOLIST_LBUTTONUP, 0, 0);
	CWnd::OnLButtonUp(nFlags, point);
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

BOOL CXComboList::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
		case WM_KEYDOWN:
		{
			///////////////////////////////////////////////////////////////////
			// we need to trap all cursor keys & alpha keys to reposition the
			// scroll bar
			///////////////////////////////////////////////////////////////////

			SCROLLINFO si =
			{
				sizeof(SCROLLINFO),
				SIF_ALL | SIF_DISABLENOSCROLL,
			};
			m_wndSBVert.GetScrollInfo(&si);
			BOOL bSetScrollInfo = FALSE;
			int nIndex = 0;
			if (::IsWindow(m_ListBox.m_hWnd))
				nIndex = m_ListBox.GetCurSel();
			if (nIndex == LB_ERR || nIndex < 0)
				nIndex = 0;

			// use index from listbox, because scroll position cannot be relied
			// upon here

			switch (pMsg->wParam)
			{
				case VK_RETURN:
					SendRegisteredMessage(WM_XCOMBOLIST_VK_RETURN, 0, 0);
					break;

				case VK_ESCAPE:
					SendRegisteredMessage(WM_XCOMBOLIST_VK_ESCAPE, 0, 0);
					break;

				// handle scrolling messages
				case VK_DOWN:
					si.nPos = nIndex + 1;
					bSetScrollInfo = TRUE;
					break;

				case VK_END:
					si.nPos = si.nMax;
					bSetScrollInfo = TRUE;
					break;

				case VK_HOME:
					si.nPos = 0;
					bSetScrollInfo = TRUE;
					break;

				case VK_NEXT:			// PAGE DOWN
					si.nPos = nIndex + (si.nPage-1);
					bSetScrollInfo = TRUE;
					break;

				case VK_PRIOR:			// PAGE UP
					si.nPos = nIndex - (si.nPage - 1);
					bSetScrollInfo = TRUE;
					break;

				case VK_UP:
					si.nPos = nIndex - 1;
					bSetScrollInfo = TRUE;
					break;

				default:
					if (pMsg->wParam >= 0x41/*VK_A*/ && pMsg->wParam <= 0x5A/*VK_Z*/)
					{
						// this was an alpha key - try to find listbox index
						CStringW strAlpha;
						strAlpha = (_TCHAR) pMsg->wParam;
						int nIndex2 = 0;
						if (::IsWindow(m_ListBox.m_hWnd))
							nIndex2 = m_ListBox.FindString(nIndex, strAlpha);
						if (nIndex2 != LB_ERR)
						{
							si.nPos = nIndex2;
							bSetScrollInfo = TRUE;
						}
					}
					break;
			}

			if (bSetScrollInfo)
			{
				// let parent know the selection has changed
				SendRegisteredMessage(WM_XCOMBOLIST_KEYDOWN, 0, 0);

				// update scrollbar
				if (si.nPos < 0)
					si.nPos = 0;
				if (si.nPos > si.nMax)
					si.nPos = si.nMax;
				m_wndSBVert.SetScrollInfo(&si);
			}
			break;
		}

		case WM_LBUTTONUP:
			SendRegisteredMessage(WM_XCOMBOLIST_LBUTTONUP, 0, 0);
			break;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

/**********************************************************************************************//**
 * Executes the kill focus action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pNewWnd	If non-null, the new window.
 **************************************************************************************************/

void CXComboList::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	m_nCount++;
	if (m_nCount > 2)
	{
		SendRegisteredMessage(WM_XCOMBOLIST_VK_ESCAPE, 0, 0);
	}
}

/**********************************************************************************************//**
 * Called when this window is created
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	lpCreateStruct	The create structure.
 *
 * \return	An int.
 **************************************************************************************************/

int CXComboList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect(0,0,0,0);

	// create the listbox that we're wrapping
	VERIFY(m_ListBox.Create(WS_VISIBLE|WS_CHILD|LBS_OWNERDRAWFIXED|LBS_NOINTEGRALHEIGHT/*|WS_BORDER*/| LBS_HASSTRINGS | LBS_NOSEL,
		rect, this, 0));

	m_ListBox.SetItemHeight(24);

	// create the vertical scrollbar
	VERIFY(m_wndSBVert.Create(WS_VISIBLE|WS_CHILD|SBS_VERT,
		rect, this, 0));

	// set font from parent
	CFont *font = GetParent()->GetFont();
	if (font)
	{
		SetFont(font, FALSE);
		m_wndSBVert.SetFont(font, FALSE);
	}

	SetHorizontalExtent();

	return 0;
}

/**********************************************************************************************//**
 * Executes the v scroll action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param 		  	nSBCode   	The sb code.
 * \param 		  	nPos	  	The position.
 * \param [in,out]	parameter3	If non-null, the third parameter.
 **************************************************************************************************/

void CXComboList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar*)
{
	if (!::IsWindow(m_ListBox.m_hWnd))
		return;

	// forward scroll message to listbox
	const MSG* pMsg = GetCurrentMessage();
	m_ListBox.SendMessage(WM_VSCROLL, pMsg->wParam, pMsg->lParam);

    SCROLLINFO si =
	{
        sizeof(SCROLLINFO),
        SIF_ALL | SIF_DISABLENOSCROLL,
    };
	m_wndSBVert.GetScrollInfo(&si);

    switch (nSBCode)
	{
		case SB_BOTTOM:			// scroll to bottom
			si.nPos = si.nMax;
			break;
		case SB_TOP:			// scroll to top
			si.nPos = 0;
			break;
		case SB_PAGEDOWN:		// scroll one page down
			si.nPos += si.nPage;
			break;
		case SB_PAGEUP:			// scroll one page up
			si.nPos -= si.nPage;
			break;
		case SB_LINEDOWN:		// scroll one line up
			si.nPos += 1;
			break;
		case SB_LINEUP:			// scroll one line up
			si.nPos -= 1;
			break;
		case SB_THUMBTRACK:		// drag scroll box to specified position. The
								// current position is provided in nPos
		case SB_THUMBPOSITION:	// scroll to the absolute position. The current
								// position is provided in nPos
			si.nPos = nPos;
			break;
		case SB_ENDSCROLL:		// end scroll
			return;
		default:
			break;
	}

	if (si.nPos < 0)
		si.nPos = 0;
	if (si.nPos > si.nMax)
		si.nPos = si.nMax;
	m_wndSBVert.SetScrollInfo(&si);
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CXComboList::OnDestroy()
{
	if (::IsWindow(m_ListBox.m_hWnd))
		m_ListBox.DestroyWindow();
	CWnd::OnDestroy();
}

/**********************************************************************************************//**
 * Adds an item
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	lpszItem	The item.
 * \param	bg			The background.
 * \param	fg			The foreground.
 * \param	hal			The HAL.
 * \param	val			The value.
 * \param	active  	True to active.
 * \param	res			The resource.
 *
 * \return	An int.
 **************************************************************************************************/

int CXComboList::AddItem(CStringW lpszItem,
			COLORREF bg,	  COLORREF fg,
			UINT hal, UINT val,
			BOOL active, DWORD res)
{
	int iTemp = m_ListBox.AddItem(lpszItem,bg,fg,hal,val,active,res);

	return iTemp;
}

/**********************************************************************************************//**
 * Adds a head
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	lpszItem	The item.
 * \param	bg			The background.
 * \param	fg			The foreground.
 * \param	hal			The HAL.
 * \param	val			The value.
 * \param	active  	True to active.
 * \param	res			The resource.
 *
 * \return	An int.
 **************************************************************************************************/

int CXComboList::AddHead(LPCTSTR lpszItem,
			COLORREF bg,	  COLORREF fg,
			UINT hal, UINT val,
			BOOL active, DWORD res)
{
	//int iTemp = m_ListBox.AddString(lpszItem);

	int iTemp = m_ListBox.AddHead(lpszItem,bg,fg,hal,val,active,res);

	return iTemp;
}

/**********************************************************************************************//**
 * Sets horizontal extent
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CXComboList::SetHorizontalExtent(/*CListBox& ListBox*/)
{
	int nMaxTextWidth = 0;

	HDC hDC = ::GetDC(m_ListBox.m_hWnd);

	HGDIOBJ original = NULL;

	//Saving the original object
	original = SelectObject(hDC,m_ListBox.GetFont());

	CStringW Text;
	const int nItems = m_ListBox.GetCount();

	for (int i = 0; i < nItems; i++)
	{
		m_ListBox.GetText(i, Text);

		Text += "X";  // otherwise item may be clipped.
		SIZE sizeText;
		//Calculate the width of the text, by using the classic method
		GetTextExtentPoint32(hDC,Text,lstrlen(Text),&sizeText);

		if (sizeText.cx > nMaxTextWidth)
		{
			nMaxTextWidth = sizeText.cx;
		}
	}

	VERIFY(::ReleaseDC(m_ListBox.m_hWnd, hDC) != 0);

	m_ListBox.SetHorizontalExtent(nMaxTextWidth);
}

/**********************************************************************************************//**
 * Deletes the string described by idx
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	idx	Zero-based index of the.
 **************************************************************************************************/

void CXComboList::DeleteString(int idx)
{
	m_ListBox.DeleteString(idx);
};

/**********************************************************************************************//**
 * Invalidate list box
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CXComboList::InvalidateListBox()
{
	m_ListBox.Invalidate();
};