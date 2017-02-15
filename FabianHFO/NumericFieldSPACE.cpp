// NumericFieldSPACE.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumericFieldSPACE.h"


// CNumericFieldSPACE

IMPLEMENT_DYNAMIC(CNumericFieldSPACE, CWnd)

CNumericFieldSPACE::CNumericFieldSPACE(eNumericSize size):
CNumericField(size)
{
	m_szName=_T("");
	m_szNameNote=_T("");
	m_szUnit=_T("");
}

CNumericFieldSPACE::~CNumericFieldSPACE()
{
}


BEGIN_MESSAGE_MAP(CNumericFieldSPACE, CWnd)
	//{{AFX_MSG_MAP(CNumericField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CNumericFieldSPACE message handlers
void CNumericFieldSPACE::OnDestroy() 
{
	CNumericField::OnDestroy();
}

// **************************************************************************
// 
// **************************************************************************
bool CNumericFieldSPACE::drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
{
	EnterCriticalSection(&csDraw);
	bool bReturn = true;

	RECT rc={0,0,m_lX,m_lY};

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);

	if(bFrames)
	{
		drawFrames(pDCStatic);
	}

	

	BitBlt(hdcMem, 0, 0, m_lX, m_lY,m_hdcStatic , 0, 0, SRCCOPY);


	BitBlt(m_hDC, 0, 0, m_lX, m_lY,hdcMem , 0, 0, SRCCOPY);
	BitBlt(dc.m_hDC, 0, 0, m_lX, m_lY, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem,hBmpMemPrev);

	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

	LeaveCriticalSection(&csDraw);

	return bReturn;
}


// **************************************************************************
// 
// **************************************************************************
bool CNumericFieldSPACE::drawFrames(CDC* pDC)
{
	HDC hdc = *pDC;
	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));	
	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

	switch(m_eSize)
	{
	case NUMERICSIZE_1:
		{
			RECT rc;
			rc.left = 0;  
			rc.top = 5;  
			rc.right  = 200;  
			rc.bottom = 62;

			CBrush br3(BACKGND);
			FillRect(hdc,&rc,br3);
		}
		break;
	case NUMERICSIZE_2:
		{
			RECT rc;
			rc.left = 0;  
			rc.top = 5;  
			rc.right  = 200;  
			rc.bottom = 124;

			CBrush br3(BACKGND);
			FillRect(hdc,&rc,br3);
		}
		break;
	case NUMERICSIZE_3:
		{
			RECT rc;
			rc.left = 0;  
			rc.top = 5;  
			rc.right  = 200;  
			rc.bottom = 186;

			CBrush br3(BACKGND);
			FillRect(hdc,&rc,br3);
		}
		break;
	}


	SelectObject(hdc, hPrevBrush);	
	SelectObject(hdc, hPrevPen);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CNumericFieldSPACE::drawStaticText(CDC* pDC)
{

	return true;
}


// **************************************************************************
// 
// **************************************************************************
bool CNumericFieldSPACE::drawLimits(CDC* pDC)
{
	
	return true;
}




