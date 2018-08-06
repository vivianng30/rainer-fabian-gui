// NumericFieldVTEMANDSPONT.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumericFieldVTEMANDSPONT.h"

/**********************************************************************************************//**
 * CNumericFieldVTEMANDSPONT
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CNumericFieldVTEMANDSPONT, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CNumericFieldVTEMANDSPONT class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	size	The size.
 **************************************************************************************************/

CNumericFieldVTEMANDSPONT::CNumericFieldVTEMANDSPONT(eNumericSize size):
CNumericField(size)
{
	m_szName=getModel()->GetLanguageString(IDS_PARA_V);
	m_szNameNote=_T("");
	m_szNameNoteResp=getModel()->GetLanguageString(IDS_PARA_TE) + _T(" ") + getModel()->GetLanguageString(IDS_PARA_RESP);
	m_szNameNotePat=getModel()->GetLanguageString(IDS_PARA_TE) + _T(" ") + getModel()->GetLanguageString(IDS_PARA_PAT);
	m_szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MILLILITER)+_T("]");
}

/**********************************************************************************************//**
 * Finalizes an instance of the CNumericFieldVTEMANDSPONT class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CNumericFieldVTEMANDSPONT::~CNumericFieldVTEMANDSPONT()
{
}


BEGIN_MESSAGE_MAP(CNumericFieldVTEMANDSPONT, CWnd)
	//{{AFX_MSG_MAP(CNumericField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CNumericFieldVTEMANDSPONT message handlers
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CNumericFieldVTEMANDSPONT::OnDestroy() 
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

bool CNumericFieldVTEMANDSPONT::drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
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

	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf33AcuBoldNum);
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdcMem,RGB(0,0,255));

	TCHAR psz[MAX_PATH];

	//###########################################################################
	WORD iTVEresp=getModel()->getDATAHANDLER()->getMessureDataTVEresp();
	WORD iTVEpat=getModel()->getDATAHANDLER()->getMessureDataTVEpat();

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
		SelectObject(hdcMem,g_hf33AcuBoldNum);
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

	if(m_eSize==NUMERICSIZE_1)
	{
		SelectObject(hdcMem,g_hf33AcuBoldNum);
		rc.top = 0;
		rc.bottom = 64;
		rc.left = 105; 
		rc.right = 180;
	}
	else //if(m_eSize==NUMERICSIZE_2)
	{
		SelectObject(hdcMem,g_hf33AcuBoldNum);
		rc.top = 0;
		rc.bottom = 119;
		rc.left = 105; 
		rc.right = 180;
	}

	if(iTVEpat==0)
		wsprintf(psz,_T("%d"), 0);
	else
	{
		if(iTVEpat>=1000)
			wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)iTVEpat)/10, 1));
		else
			wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)iTVEpat)/10, 1));
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

//bool CNumericFieldVTEMANDSPONT::drawFrames(CDC* pDC)
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
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CNumericFieldVTEMANDSPONT::drawStaticText(CDC* pDC)
{
	HDC hdc = *pDC;
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf13AcuBold);
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	RECT rc;
	//CPen* pTempPen = NULL;
	//CPen origPen;
	//CPen penLine(PS_SOLID,3,RGB(200,200,200));
	//pTempPen = (CPen*)pDC->SelectObject(&penLine);
	//// Save original pen.
	//origPen.FromHandle((HPEN)pTempPen);

	CPen penLine(PS_SOLID,3,RGB(200,200,200));
	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)penLine);

	int iLeft = 0;
	int iBottom = 0;

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

		rc.top = 9;
		rc.bottom = m_lY;
		rc.left = 105; 
		rc.right = 190;

		pDC->DrawText(m_szName,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		
		rc.left = 15; 

		SelectObject(hdc,g_hf7AcuBold);

		CSize szNoteResp,szNotePat;

		rc.top = 13;
		rc.left = 17+sz1.cx;
		pDC->DrawText(m_szNameNoteResp,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		szNoteResp = pDC->GetTextExtent(m_szNameNoteResp);

		rc.top = 13;
		rc.left =105+sz1.cx;
		pDC->DrawText(m_szNameNotePat,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		szNotePat = pDC->GetTextExtent(m_szNameNotePat);

		SelectObject(hdc,g_hf7AcuNorm);

		rc.top = 10;
		rc.left = 25;
		rc.right = 185;
		pDC->DrawText(m_szUnit,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);

		
		iLeft = 25+sz1.cx+szNoteResp.cx+7;
		iBottom=62;
	}
	else //if(m_eSize==NUMERICSIZE_2)
	{
		SelectObject(hdc,g_hf13AcuBold);
		rc.top = 9;
		rc.bottom = m_lY;
		rc.left = 15; 
		rc.right = 190;

		CSize sz1;

		pDC->DrawText(m_szName,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		sz1 = pDC->GetTextExtent(m_szName);

		rc.top = 9;
		rc.bottom = m_lY;
		rc.left = 105; 
		rc.right = 190;

		pDC->DrawText(m_szName,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

		rc.left = 15; 

		SelectObject(hdc,g_hf7AcuBold);

		CSize szNoteResp,szNotePat;

		rc.top = 13;
		rc.left = 17+sz1.cx;
		pDC->DrawText(m_szNameNoteResp,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		szNoteResp = pDC->GetTextExtent(m_szNameNoteResp);

		rc.top = 13;
		rc.left =105+sz1.cx;
		pDC->DrawText(m_szNameNotePat,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		szNotePat = pDC->GetTextExtent(m_szNameNotePat);

		SelectObject(hdc,g_hf7AcuNorm);

		rc.top = 10;
		rc.left = 25;
		rc.right = 188;
		pDC->DrawText(m_szUnit,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);

		iLeft = 25+sz1.cx+szNoteResp.cx+7;
		iBottom=124;
	}

	/*MoveToEx(hdc, iLeft, 5, NULL);
	LineTo(hdc, iLeft, iBottom);*/
	MoveToEx(hdc, 98, 5, NULL);
	LineTo(hdc, 98, iBottom);

	// Reselect original brush into device context.
	/*pDC->SelectObject(&origPen);
	penLine.DeleteObject();*/

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	SelectObject(hdc, hPrevPen);
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

bool CNumericFieldVTEMANDSPONT::drawLimits(CDC* pDC)
{
	
	return true;
}

