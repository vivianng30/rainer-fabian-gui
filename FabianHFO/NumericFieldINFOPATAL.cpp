// NumericFieldINFOPATAL.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumericFieldINFOPATAL.h"


// CNumericFieldINFOPATAL

IMPLEMENT_DYNAMIC(CNumericFieldINFOPATAL, CWnd)

CNumericFieldINFOPATAL::CNumericFieldINFOPATAL(eNumericSize size):
CNumericField(size)
{
	m_szName=_T("");
	m_szNameNote=_T("");
	m_szUnit=_T("");

	m_szName1=getModel()->GetLanguageString(IDS_TXT_THERAP_NOALARM1);
	m_szName2=getModel()->GetLanguageString(IDS_TXT_THERAP_NOALARM2);

	//m_pcNumInfoTop		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUM_INFO_TOP);
	//m_pcNumInfoBot		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUM_INFO_BOT);
}

CNumericFieldINFOPATAL::~CNumericFieldINFOPATAL()
{
}


BEGIN_MESSAGE_MAP(CNumericFieldINFOPATAL, CWnd)
	//{{AFX_MSG_MAP(CNumericField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CNumericFieldINFOPATAL message handlers
void CNumericFieldINFOPATAL::OnDestroy() 
{
	CNumericField::OnDestroy();
}

// **************************************************************************
// 
// **************************************************************************
bool CNumericFieldINFOPATAL::drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
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

	if(bText)
	{
		drawStaticText(pDCStatic);
	}

	/*if(bLimits)
	{
		drawLimits(pDCStatic);
	}*/

	
	BitBlt(m_hDC, 0, 0, m_lX, m_lY,m_hdcStatic , 0, 0, SRCCOPY);
	BitBlt(dc.m_hDC, 0, 0, m_lX, m_lY, m_hdcStatic, 0, 0, SRCCOPY);

	SelectObject(hdcMem,hBmpMemPrev);

	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

	LeaveCriticalSection(&csDraw);

	return bReturn;
}


// **************************************************************************
// 
// **************************************************************************
bool CNumericFieldINFOPATAL::drawFrames(CDC* pDC)
{
	HDC hdc = *pDC;
	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));	
	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

	if(m_pcNumInfoTop)
		m_pcNumInfoTop->Draw(hdc,0,12);
	if(m_pcNumInfoBot)
		m_pcNumInfoBot->Draw(hdc,0,45);

	RECT rc;
	rc.left = 0;  
	rc.top = 29;  
	rc.right  = 200;  
	rc.bottom = 45;

	CBrush br3(RGB(176,176,177));
	FillRect(hdc,&rc,br3);

	SelectObject(hdc, hPrevBrush);	
	SelectObject(hdc, hPrevPen);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CNumericFieldINFOPATAL::drawStaticText(CDC* pDC)
{
	HDC hdc = *pDC;
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdc,RGB(255,0,0));
	RECT rc;

	rc.left = 0;  
	rc.top = 20;  
	rc.right  = 190;  
	rc.bottom = 57;

	pDC->DrawText(m_szName1,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);
	pDC->DrawText(m_szName2,&rc,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}


// **************************************************************************
// 
// **************************************************************************
bool CNumericFieldINFOPATAL::drawLimits(CDC* pDC)
{
	return true;
}




