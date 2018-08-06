// NumericFieldSPACE.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumericFieldSPACE.h"

/**********************************************************************************************//**
 * CNumericFieldSPACE
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CNumericFieldSPACE, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CNumericFieldSPACE class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	size	The size.
 **************************************************************************************************/

CNumericFieldSPACE::CNumericFieldSPACE(eNumericSize size):
CNumericField(size)
{
	m_szName=_T("");
	m_szNameNote=_T("");
	m_szUnit=_T("");
}

/**********************************************************************************************//**
 * Finalizes an instance of the CNumericFieldSPACE class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CNumericFieldSPACE::~CNumericFieldSPACE()
{
}


BEGIN_MESSAGE_MAP(CNumericFieldSPACE, CWnd)
	//{{AFX_MSG_MAP(CNumericField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CNumericFieldSPACE message handlers
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CNumericFieldSPACE::OnDestroy() 
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

bool CNumericFieldSPACE::drawStaticText(CDC* pDC)
{

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

bool CNumericFieldSPACE::drawLimits(CDC* pDC)
{
	
	return true;
}




