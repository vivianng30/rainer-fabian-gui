
#include "stdafx.h"
#include "ColorListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HFONT g_hf14AcuMed;

/////////////////////////////////////////////////////////////////////////////
// CListBoxEBX

//-------------------------------------------------------------------
//
CColorListBox::CColorListBox()
{
	m_pSuper = (CListBox*)this;
	m_iItemHeight = 0;
}

CColorListBox::~CColorListBox(){}

BEGIN_MESSAGE_MAP(CColorListBox, CListBox)
	//{{AFX_MSG_MAP(CColorListBox)
	ON_WM_DESTROY()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorListBox message handlers

//-------------------------------------------------------------------
//

void CColorListBox::OnSysColorChange()
{
	CListBox::OnSysColorChange();
	this->RedrawWindow();
}

//************************************
// Method:    OnDestroy
// FullName:  CColorListBox::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CColorListBox::OnDestroy()
{
	//Clean up items and their strings
	ResetContent();

	CListBox::OnDestroy();
}

void CColorListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	if ((int)lpDIS->itemID < 0)
		return;

	int		iPenSize	= 3;
	CDC		*pDC		= CDC::FromHandle(lpDIS->hDC);

	LItem	*pItem		= Item((int)lpDIS->itemID);		// pointer to data item


	COLORREF bg			= pItem->cBg;
	COLORREF fg			= pItem->cFg;

	if(bg == DEF_COL) bg = ::GetSysColor(COLOR_WINDOW);
	if(fg == DEF_COL) fg = ::GetSysColor(COLOR_WINDOWTEXT);


	// Draw background
	// -------------------------------------------------------------------
	CBrush brush(bg);
	pDC->FillRect(&lpDIS->rcItem, &brush);

	// Draw text
	//--------------------------------------------------------------------
	UINT nFormat = DT_SINGLELINE | pItem->uVal | pItem->uHal;

	int		 nOldBkMode		= pDC->SetBkMode(TRANSPARENT);
	COLORREF cOldFg			= pDC->SetTextColor(fg);
	HFONT hPrevFont=(HFONT)pDC->SelectObject(g_hf14AcuMed);
	CStringW	 sItemText;

	CRect rc(lpDIS->rcItem);
	rc.DeflateRect(iPenSize, iPenSize);

	GetText((int)lpDIS->itemID, sItemText);
	pDC->DrawText(sItemText, -1, &rc, nFormat);

	pDC->SetTextColor(cOldFg);
	pDC->SetBkMode(nOldBkMode);
	pDC->SelectObject(hPrevFont);

	// Draw disabled
	//--------------------------------------------------------------------
	if(pItem->bActive == FALSE)
	{
		CRect rc(lpDIS->rcItem);

		rc.DeflateRect(iPenSize, iPenSize);

		CDC memDC0;

		memDC0.CreateCompatibleDC(pDC);

		AlphaBlendU(*pDC, rc.left, rc.top, rc.Width(), rc.Height(),
			memDC0, rc.left, rc.top, rc.Width(), rc.Height(), 50);

		memDC0.DeleteDC();
	}

	// if selected no 3D border
	//--------------------------------------------------------------------
	if((lpDIS->itemAction & ODA_FOCUS) && (lpDIS->itemState & ODS_FOCUS))
	{
		CRect rc(lpDIS->rcItem);
		for(int i = 0; i < iPenSize; i++)
		{
			pDC->DrawFocusRect(rc);
			rc.DeflateRect(1, 1);
		}
	}
}

void CColorListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	if(m_iItemHeight == 0)
	{
		CPaintDC dc(this);
		CRect r;
		dc.DrawText(_T("M"), -1, &r, DT_CALCRECT);
		lpMIS->itemHeight = r.Height() + 6;
	}
	else
		lpMIS->itemHeight = m_iItemHeight + 6;

}


// private
int CColorListBox::Ix(int ix)
{
	if(GetCount() == 0)					// not populated yet
		return -1;

	if(ix == -1)						// asking for the selected item
		return GetCurSel();
	else if(ix < 0 || ix >= GetCount())	// out of range
		return -1;
	else
		return ix;
}

// private
CColorListBox::LItem* CColorListBox::Item(int ix)
{
	int i =Ix(ix);

	if(i == -1) return NULL;

	return (LItem *)m_pSuper->GetItemData(i);
}

// Member functions
// public
int CColorListBox::AddItem
(LPCTSTR lpszItem, COLORREF bg, COLORREF fg, UINT hal, UINT val, BOOL active, DWORD res)
{
	// Add to the end
	return InsertItem(lpszItem, GetCount(), bg, fg, hal, val, active, res);
}

int CColorListBox::AddHead
(LPCTSTR lpszItem, COLORREF bg, COLORREF fg, UINT hal, UINT val, BOOL active, DWORD res)
{
	// Add to the head
	return InsertItem(lpszItem, 0, bg, fg, hal, val, active, res);
}


int CColorListBox::InsertItem
(LPCTSTR lpszItem, int ix, COLORREF bg, COLORREF fg, UINT hal, UINT val, BOOL active, DWORD res)
{
	if(ix < 0 || ix > GetCount()) //out of range thing
		return LB_ERR;


	int iRet = m_pSuper->InsertString(ix, lpszItem);

	if(iRet == LB_ERR)
		return iRet;

	LItem *lbi = new LItem;

	lbi->cBg	= bg;
	lbi->cFg	= fg;
	lbi->uHal	= hal;
	lbi->uVal	= val;
	lbi->bActive= active;
	lbi->dw		= res;

	if(m_pSuper->SetItemData(ix, (DWORD)lbi) == LB_ERR)
		return LB_ERR;

	return iRet;
}

BOOL CColorListBox::RemoveAt(int ix)
{
	int i =Ix(ix);

	if(i == -1) return FALSE;

	LItem *plbi = (LItem*)m_pSuper->GetItemData(i);

	if(m_pSuper->DeleteString(i) == LB_ERR || plbi == NULL)
		return FALSE;

	delete plbi;

	return TRUE;
}

void CColorListBox::ResetContent()
{
	while(GetCount()) RemoveAt(0);
}


BOOL CColorListBox::SetBg(COLORREF color, int ix)
{
	LItem *plbi = Item(ix);

	if(!plbi) return FALSE;

	plbi->cBg = color;

	RedrawWindow();

	return TRUE;
}

BOOL CColorListBox::SetFg(COLORREF color, int ix)
{
	LItem *plbi = Item(ix);

	if(!plbi) return FALSE;

	plbi->cFg = color;

	RedrawWindow();

	return TRUE;
}

BOOL CColorListBox::SetVal(UINT align, int ix)
{
	LItem *plbi = Item(ix);

	if(!plbi) return FALSE;

	plbi->uVal = align;

	RedrawWindow();

	return TRUE;
}

BOOL CColorListBox::SetHal(UINT align, int ix)
{
	LItem *plbi = Item(ix);

	if(!plbi) return FALSE;

	plbi->uHal = align;

	RedrawWindow();

	return TRUE;
}

BOOL CColorListBox::SetActive(BOOL bActive, int ix)
{
	LItem *plbi = Item(ix);

	if(!plbi) return FALSE;

	plbi->bActive = bActive;

	RedrawWindow();

	return TRUE;
}

BOOL CColorListBox::SetData(DWORD data, int ix)
{
	LItem *plbi = Item(ix);

	if(!plbi) return FALSE;

	plbi->dw = data;

	RedrawWindow();

	return TRUE;
}


// private
bool CColorListBox::AlphaBlendU
(
 HDC dcDest, int x, int y, int cx, int cy,
 HDC dcSrc, int sx, int sy, int scx, int scy, int alpha
 )
{
	BITMAPINFOHEADER BMI;
	// Fill in the header info.
	BMI.biSize = sizeof(BITMAPINFOHEADER);
	BMI.biWidth = cx;
	BMI.biHeight = cy;
	BMI.biPlanes = 1;
	BMI.biBitCount = 32;
	BMI.biCompression = BI_RGB;   // No compression
	BMI.biSizeImage = 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed = 0;           // Always use the whole palette.
	BMI.biClrImportant = 0;

	BYTE * pSrcBits;
	HBITMAP hbmSrc;
	// Create DIB section in shared memory
	hbmSrc = CreateDIBSection (dcSrc, (BITMAPINFO *)&BMI,
		DIB_RGB_COLORS, (void **)&pSrcBits, 0, 0l);

	BYTE * pDestBits;
	HBITMAP hbmDest;
	// Create DIB section in shared memory
	hbmDest = CreateDIBSection (dcDest, (BITMAPINFO *)&BMI,
		DIB_RGB_COLORS, (void **)&pDestBits, 0, 0l);


	// Copy our source and destination bitmaps onto our DIBSections,
	// so we can get access to their bits using the BYTE *'s we passed into CreateDIBSection

	HDC dc = CreateCompatibleDC(NULL);

	HBITMAP dcOld = (HBITMAP) SelectObject(dc, hbmSrc);

	if (!StretchBlt(dc, 0, 0, cx, cy, dcSrc, sx, sy, scx, scy, SRCCOPY))
		return false;

	SelectObject(dc, hbmDest);
	if (!StretchBlt(dc, 0, 0, cx, cy, dcDest, x, y, cx, cy, SRCCOPY))
		return false;

	SelectObject(dc, dcOld);
	DeleteDC(dc);

	for (int j = 0; j < cy; ++j)
	{
		LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[j * cx];
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[j * cx];

		for (int i = 0; i < cx; ++i)
		{
			pbSrcRGB[0]=(pbDestRGB[0] * (255-alpha) + pbSrcRGB[0] * alpha)>>8;
			pbSrcRGB[1]=(pbDestRGB[1] * (255-alpha) + pbSrcRGB[1] * alpha)>>8;
			pbSrcRGB[2]=(pbDestRGB[2] * (255-alpha) + pbSrcRGB[2] * alpha)>>8;
			pbSrcRGB += 4;
			pbDestRGB += 4;
		}
	}

	dc = CreateCompatibleDC(NULL);

	dcOld = (HBITMAP) SelectObject(dc, hbmSrc);
	if (!BitBlt(dcDest, x, y, cx, cy, dc, 0, 0, SRCCOPY))
		return false;
	SelectObject(dc, dcOld);
	DeleteDC(dc);

	DeleteObject(hbmSrc);
	DeleteObject(hbmDest);

	return true;
}

