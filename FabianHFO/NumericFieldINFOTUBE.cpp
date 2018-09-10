// NumericFieldINFOTUBE.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumericFieldINFOTUBE.h"

/**********************************************************************************************//**
 * CNumericFieldINFOTUBE
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CNumericFieldINFOTUBE, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CNumericFieldINFOTUBE class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	size	The size.
 **************************************************************************************************/

CNumericFieldINFOTUBE::CNumericFieldINFOTUBE(eNumericSize size):
CNumericField(size)
{
	m_szName=_T("");
	m_szNameNote=_T("");
	m_szUnit=_T("");

	//m_pcNumInfoTop		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUM_INFO_TOP);
	//m_pcNumInfoBot		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUM_INFO_BOT);
}

/**********************************************************************************************//**
 * Finalizes an instance of the CNumericFieldINFOTUBE class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CNumericFieldINFOTUBE::~CNumericFieldINFOTUBE()
{
}


BEGIN_MESSAGE_MAP(CNumericFieldINFOTUBE, CWnd)
	//{{AFX_MSG_MAP(CNumericField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CNumericFieldINFOTUBE message handlers
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CNumericFieldINFOTUBE::OnDestroy() 
{
	CNumericField::OnDestroy();
}

/**********************************************************************************************//**
 * Draw data
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bData	  	True to data.
 * \param	bFrames   	True to frames.
 * \param	bText	  	True to text.
 * \param	bLimits   	True to limits.
 * \param	bFlowmeter	True to flowmeter.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CNumericFieldINFOTUBE::drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
{
	EnterCriticalSection(&csDraw);
	bool bReturn = true;

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

/**********************************************************************************************//**
 * Draw frames
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CNumericFieldINFOTUBE::drawFrames(CDC* pDC)
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

/**********************************************************************************************//**
 * Draw static text
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CNumericFieldINFOTUBE::drawStaticText(CDC* pDC)
{
	HDC hdc = *pDC;
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	RECT rc;

	rc.left = 0;  
	rc.top = 20;  
	rc.right  = 190;  
	rc.bottom = 57;

	pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_TUBESET),&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);

	eTubeSet eTube=getModel()->getDATAHANDLER()->GetTubeSet();
	if(eTube==TUBE_MEDIJET)
	{
		pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_MEDIJET),&rc,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
	}
	else if(eTube==TUBE_INFANTFLOW)
	{
		pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_INFANTFLOW),&rc,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
	}
	else //TUBE_INFANTFLOW_LP
	{
		pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_INFANTFLOWLP),&rc,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
	}
	

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}

/**********************************************************************************************//**
 * Draw limits
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CNumericFieldINFOTUBE::drawLimits(CDC* pDC)
{
	return true;
}




