// NumericFieldPPEAK.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumericFieldPPEAK.h"

/**********************************************************************************************//**
 * CNumericFieldPPEAK
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CNumericFieldPPEAK, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CNumericFieldPPEAK class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	size	The size.
 **************************************************************************************************/

CNumericFieldPPEAK::CNumericFieldPPEAK(eNumericSize size):
CNumericField(size)
{
	m_szName=getModel()->GetLanguageString(IDS_PARA_PIP);
	m_szNameNote=_T("");
	//m_szNameNote=getModel()->GetLanguageString(IDS_PARA_MAX);
	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]");
	}
	else
	{
		m_szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]");
	}
}

/**********************************************************************************************//**
 * Finalizes an instance of the CNumericFieldPPEAK class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CNumericFieldPPEAK::~CNumericFieldPPEAK()
{
	
}


BEGIN_MESSAGE_MAP(CNumericFieldPPEAK, CWnd)
	//{{AFX_MSG_MAP(CNumericField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CNumericFieldPPEAK message handlers
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CNumericFieldPPEAK::OnDestroy() 
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

bool CNumericFieldPPEAK::drawData(bool, bool bFrames, bool bText, bool bLimits, bool)
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
	eVentMode eActiveVentMode=getModel()->getALARMHANDLER()->getSafeActiveVentMode(); //pro - added
	if(bLimits && eActiveVentMode!=VM_HFO)
	{
		drawLimits(pDCStatic);
	}

	BitBlt(hdcMem, 0, 0, m_lX, m_lY,m_hdcStatic , 0, 0, SRCCOPY);

	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf33AcuBoldNum);
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdcMem,RGB(0,0,255));

	TCHAR psz[MAX_PATH];

	//###########################################################################
	SHORT iPmax=getModel()->getDATAHANDLER()->getBTBMessureDataPINSP();

	//AmplitudeCorrectionFactor
	/*if(false==getModel()->getDATAHANDLER()->getFOToscillationState() && eActiveVentMode==VM_HFO && false==getModel()->isMANBREATHrunning())
	{
		double iPmitt=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
		double iAmpCorFactor = getModel()->getDATAHANDLER()->getAmpCorFactor(getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
		if(iAmpCorFactor==0)
			iAmpCorFactor=1;
		iPmax=(SHORT)(((double)iPmax-iPmitt)*iAmpCorFactor)+iPmitt;
	}*/


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
	
	if(iPmax==0)
		wsprintf(psz,_T("%d"), 0);
	else
	{
		wsprintf(psz,_T("%0.0f"), ((double)iPmax)/10);
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

//bool CNumericFieldPPEAK::drawFrames(CDC* pDC)
//{
//	HDC hdc = *pDC;
//	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));	
//	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
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

bool CNumericFieldPPEAK::drawStaticText(CDC* pDC)
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

		/*SelectObject(hdc,g_hf7AcuBold);

		CSize szNote;

		rc.top = 13;
		rc.left = 17+sz1.cx;
		pDC->DrawText(m_szNameNote,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		szNote = pDC->GetTextExtent(m_szNameNote);*/

		SelectObject(hdc,g_hf7AcuNorm);

		rc.top = 10;
		//rc.left = 25+sz1.cx+szNote.cx;
		rc.left = 25+sz1.cx;
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

		/*SelectObject(hdc,g_hf8AcuBold);

		CSize szNote;

		rc.top = 17;
		rc.left = 17+sz1.cx;
		pDC->DrawText(m_szNameNote,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		szNote = pDC->GetTextExtent(m_szNameNote);*/

		SelectObject(hdc,g_hf15AcuMed);

		rc.top = 8;
		//rc.left = 25+sz1.cx+szNote.cx;
		rc.left = 25+sz1.cx;
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
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CNumericFieldPPEAK::drawLimits(CDC* pDC)//rku PIPLOW DUOPAP
{
	HDC hdc = *pDC;
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nPrevTxtColor=SetTextColor(hdc,RGB(200,0,0));
	RECT rc;	
	TCHAR psz[MAX_PATH];

	eVentMode eActiveVentMode=getModel()->getALARMHANDLER()->getSafeActiveVentMode();

	if(m_eSize==NUMERICSIZE_1)
	{
		drawHighLimit(pDC);
		if(eActiveVentMode!=VM_HFO)
			drawLowLimit(pDC);

		rc.top = 9;
		rc.bottom = 59;
		rc.left = 0;
		rc.right = 187;
	}
	else //if(m_eSize==NUMERICSIZE_2)
	{
		rc.top = 58;
		rc.bottom = 110;
		rc.left = 0;
		rc.right = 187;
	}
	
	if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit() == AL_CALC)
	{
		SelectObject(hdc,g_hf9AcuBold);
		pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	}
	else if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit() == AL_OFF)
	{
		SelectObject(hdc,g_hf9AcuBold);
		pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	}
	else if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit() != AL_OFF)
	{
		SelectObject(hdc,g_hf11AcuBoldNum);
		if(getModel()->getALARMHANDLER()->getAlimitPIPmax()==0)
			wsprintf(psz,_T("%d"), 0);
		else
			wsprintf(psz,_T("%0.0f"), CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitPIPmax())/10, 0));
		pDC->DrawText(psz,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);

		/*if(eActiveVentMode!=VM_HFO)
		{
			if(getModel()->getALARMHANDLER()->getAlimitPIPmin()==0)
				wsprintf(psz,_T("%d"), 0);
			else
				wsprintf(psz,_T("%0.0f"), CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitPIPmin())/10, 0));
			pDC->DrawText(psz,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}*/
		

		if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit() == AL_AUTO)
		{
			if(m_eSize==NUMERICSIZE_1)
			{
				rc.bottom = 39;
			}
			else //if(m_eSize==NUMERICSIZE_2)
			{
				rc.bottom = 88;
			}
			SelectObject(hdc,g_hf9AcuBold);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}
	}

	if(m_eSize==NUMERICSIZE_1)
	{
		rc.top = 9;
		rc.bottom = 59;
		rc.left = 0;
		rc.right = 187;
	}
	else //if(m_eSize==NUMERICSIZE_2)
	{
		rc.top = 58;
		rc.bottom = 110;
		rc.left = 0;
		rc.right = 187;
	}

	if(eActiveVentMode!=VM_HFO)
	{
		if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit() == AL_CALC)
		{
			SelectObject(hdc,g_hf9AcuBold);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}
		else if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit() == AL_OFF)
		{
			SelectObject(hdc,g_hf9AcuBold);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}
		else if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit() != AL_OFF)
		{
			SelectObject(hdc,g_hf11AcuBoldNum);
			if(getModel()->getALARMHANDLER()->getAlimitPIPmin()==0)
				wsprintf(psz,_T("%d"), 0);
			else
				wsprintf(psz,_T("%0.0f"), CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitPIPmin())/10, 0));
			pDC->DrawText(psz,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);

			if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit() == AL_AUTO)
			{
				if(m_eSize==NUMERICSIZE_1)
				{
					rc.bottom = 39;
				}
				else //if(m_eSize==NUMERICSIZE_2)
				{
					rc.bottom = 88;
				}
				SelectObject(hdc,g_hf9AcuBold);
				pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
			}
		}
	}

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}


