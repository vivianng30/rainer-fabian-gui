// NumericFieldVTEMAND.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumericFieldVTEMAND.h"

/**********************************************************************************************//**
 * CNumericFieldVTEMAND
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CNumericFieldVTEMAND, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CNumericFieldVTEMAND class
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	size	The size.
 **************************************************************************************************/

CNumericFieldVTEMAND::CNumericFieldVTEMAND(eNumericSize size):
CNumericField(size)
{
	m_szName=getModel()->GetLanguageString(IDS_PARA_V);
	m_szNameNote=getModel()->GetLanguageString(IDS_PARA_TE) + _T(" ") + getModel()->GetLanguageString(IDS_PARA_RESP);
	m_szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MILLILITER)+_T("]");
}

/**********************************************************************************************//**
 * Finalizes an instance of the CNumericFieldVTEMAND class
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

CNumericFieldVTEMAND::~CNumericFieldVTEMAND()
{
}


BEGIN_MESSAGE_MAP(CNumericFieldVTEMAND, CWnd)
	//{{AFX_MSG_MAP(CNumericField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CNumericFieldVTEMAND message handlers
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CNumericFieldVTEMAND::OnDestroy() 
{
	CNumericField::OnDestroy();
}

/**********************************************************************************************//**
 * Draw data
 *
 * \author	Rainer K�hner
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

bool CNumericFieldVTEMAND::drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
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

	BitBlt(hdcMem, 0, 0, m_lX, m_lY,m_hdcStatic , 0, 0, SRCCOPY);

	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf34BoldNum);
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdcMem,RGB(0,0,255));

	TCHAR psz[MAX_PATH];

	//###########################################################################
	WORD iTVEresp=getModel()->getDATAHANDLER()->getMessureDataTVEresp();

	if(m_eSize==NUMERICSIZE_1)
	{
		SelectObject(hdcMem,g_hf33AcuBoldNum);
		rc.top = 0;
		rc.bottom = 64;
		rc.left = 15; 
		rc.right = 175;
	}
	else //if(m_eSize==NUMERICSIZE_2)
	{
		SelectObject(hdcMem,g_hf70BoldNum);
		rc.top = 0;
		rc.bottom = 119;
		rc.left = 15; 
		rc.right = 175;
	}

	if(iTVEresp==0)
		wsprintf(psz,_T("%d"), 0);
	else
	{
		if(iTVEresp>=1000)
			wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)iTVEresp)/10, 1));
		else
			wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)iTVEresp)/10, 1));
	}
	DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	BitBlt(m_hDC, 0, 0, m_lX, m_lY,hdcMem , 0, 0, SRCCOPY);
	BitBlt(dc.m_hDC, 0, 0, m_lX, m_lY, hdcMem, 0, 0, SRCCOPY);

	SetTextColor(hdcMem,nPrevTxtColor);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hPrevFont);
	SelectObject(hdcMem,hBmpMemPrev);

	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

	LeaveCriticalSection(&csDraw);

	return bReturn;
}

//bool CNumericFieldVTEMAND::drawFrames(CDC* pDC)
//{
//	HDC hdc = *pDC;
//	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));	
//	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
//
//	/*if(m_pcNumPara3er)
//		m_pcNumPara3er->Draw(hdc,0,5);*/
//
//	RECT rc;
//
//	
//
//	SelectObject(hdc, hPrevBrush);	
//	SelectObject(hdc, hPrevPen);
//
//	return true;
//}

/**********************************************************************************************//**
 * Draw static text
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CNumericFieldVTEMAND::drawStaticText(CDC* pDC)
{
	HDC hdc = *pDC;
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf13AcuBold);
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	RECT rc;

	if(m_eSize==NUMERICSIZE_1)
	{
		SelectObject(hdc,g_hf13AcuBold);
		rc.top = 9;
		rc.bottom = m_lY;
		rc.left = 15; 
		rc.right = 190;

		CSize sz1;

		pDC->DrawText(m_szName,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz1 = pDC->GetTextExtent(m_szName);

		SelectObject(hdc,g_hf7AcuBold);

		CSize szNote;

		rc.top = 13;
		rc.left = 17+sz1.cx;
		pDC->DrawText(m_szNameNote,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		szNote = pDC->GetTextExtent(m_szNameNote);

		SelectObject(hdc,g_hf7AcuNorm);

		rc.top = 10;
		rc.left = 25+sz1.cx+szNote.cx;
		pDC->DrawText(m_szUnit,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}
	else //if(m_eSize==NUMERICSIZE_2)
	{
		SelectObject(hdc,g_hf17AcuBold);
		rc.top = 9;
		rc.bottom = m_lY;
		rc.left = 15; 
		rc.right = 190;

		CSize sz1;

		pDC->DrawText(m_szName,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz1 = pDC->GetTextExtent(m_szName);

		SelectObject(hdc,g_hf8AcuBold);

		CSize szNote;

		rc.top = 17;
		rc.left = 17+sz1.cx;
		pDC->DrawText(m_szNameNote,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		szNote = pDC->GetTextExtent(m_szNameNote);

		SelectObject(hdc,g_hf15AcuMed);

		rc.top = 8;
		rc.left = 25+sz1.cx+szNote.cx;
		pDC->DrawText(m_szUnit,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}



	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}

/**********************************************************************************************//**
 * Draw limits
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CNumericFieldVTEMAND::drawLimits(CDC* pDC)
{
	
	return true;
}
