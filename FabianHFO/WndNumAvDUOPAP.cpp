// WndNumAvDUOPAP.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndNumAvDUOPAP.h"

//#include <WinUser.h>


// CWndNumAv

IMPLEMENT_DYNAMIC(CWndNumAvDUOPAP, CWnd)

CWndNumAvDUOPAP::CWndNumAvDUOPAP():
CWndNumeric()
{
	InitializeCriticalSection(&csDoThread);

	m_bDoThread=false;
	m_pcwtFlowThread=NULL;
	m_hThreadFlow=INVALID_HANDLE_VALUE;

	m_szName1=getModel()->GetLanguageString(IDS_PARA_FLOW);
	m_szName2=getModel()->GetLanguageString(IDS_PARA_EXPIRATION);
	m_szName3=getModel()->GetLanguageString(IDS_PARA_INSPIRATION);
	m_szName4=getModel()->GetLanguageString(IDS_PARA_P);
	m_szName5=getModel()->GetLanguageString(IDS_PARA_DUO_PEEP);
	
	m_szNameNote1=getModel()->GetLanguageString(IDS_PARA_DUO_MAX);
	m_szNameNote2=getModel()->GetLanguageString(IDS_PARA_MITT);

	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		m_szUnitPressure=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]");
	}
	else
	{
		m_szUnitPressure=_T("[")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]");
	}
	m_szUnitLMIN=_T("[")+getModel()->GetLanguageString(IDS_UNIT_LMIN)+_T("]");

#ifndef SIMULATION_ENTREK //rkuNEWFIX
	StartFlowThread();
#endif
}

CWndNumAvDUOPAP::~CWndNumAvDUOPAP()
{
	DeleteCriticalSection(&csDoThread);
}


BEGIN_MESSAGE_MAP(CWndNumAvDUOPAP, CWnd)
	//{{AFX_MSG_MAP(CWndNumAvDUOPAP)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CURSORBTN_UP, &CWndNumeric::OnBnClickedCursorUp)
	ON_BN_CLICKED(IDC_CURSORBTN_DOWN, &CWndNumeric::OnBnClickedCursorDwn)
	ON_BN_CLICKED(IDC_CURSORBTN_LEFT, &CWndNumeric::OnBnClickedCursorLeft)
	ON_BN_CLICKED(IDC_CURSORBTN_RIGHT, &CWndNumeric::OnBnClickedCursorRight)
	ON_BN_CLICKED(IDC_CURSORBTN_XUP, &CWndNumeric::OnBnClickedMoveXUp)
	ON_BN_CLICKED(IDC_CURSORBTN_XDOWN, &CWndNumeric::OnBnClickedMoveXDown)
	ON_BN_CLICKED(IDC_CURSORBTN_YLEFT, &CWndNumeric::OnBnClickedMoveYLeft)
	ON_BN_CLICKED(IDC_CURSORBTN_YRIGHT, &CWndNumeric::OnBnClickedMoveYRight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CWndNumAvDUOPAP message handlers
//************************************
// Method:    OnDestroy
// FullName:  CWndNumAvDUOPAP::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of thread
//************************************
void CWndNumAvDUOPAP::OnDestroy() 
{
	StopFlowThread();
	if(m_pcwtFlowThread!=NULL)
	{
		delete m_pcwtFlowThread;
		m_pcwtFlowThread=NULL;

		if(m_hThreadFlow!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadFlow);
			m_hThreadFlow=INVALID_HANDLE_VALUE;
		}
	}

	CWndNumeric::OnDestroy();
}


// **************************************************************************
// 
// **************************************************************************
bool CWndNumAvDUOPAP::DrawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
{
	EnterCriticalSection(&csDraw);
	bool bReturn = true;
	
	RECT rc={0,0,m_lX,m_lY};

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);


	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf33AcuBoldNum);
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdcMem,RGB(0,0,255));

	if(bFrames)
	{
		DrawFrames(pDCStatic);
	}

	if(bText)
	{
		DrawStaticText(pDCStatic);
	}

	if(bLimits)
	{
		DrawLimits(pDCStatic);
	}

	BitBlt(hdcMem, 0, 0, m_lX, m_lY,m_hdcStatic , 0, 0, SRCCOPY);

	if(bFlowmeter && false==m_bGraphCursor)
	{
		
		if(m_pcFlowmeterLeft)
			m_pcFlowmeterLeft->Draw(hdcMem,65,240);

		if(m_pcFlowmeterRight)
			m_pcFlowmeterRight->Draw(hdcMem,131,240);

		// 0 l/min == 417
		// 20 l/min == 253
		// => 160 pixel == 20 l/min
		// => 8 pixel == 1 l/min

		switch(m_iFlowKegelExsp)
		{
		case 0:
			{
				m_pcFlowKegelExpp=m_pcFlowKegel_0;
			}
			break;
		case 1:
			{
				m_pcFlowKegelExpp=m_pcFlowKegel_1;
			}
			break;
		case 2:
			{
				m_pcFlowKegelExpp=m_pcFlowKegel_2;
			}
			break;
		case 3:
			{
				m_pcFlowKegelExpp=m_pcFlowKegel_3;
			}
			break;
		case 4:
			{
				m_pcFlowKegelExpp=m_pcFlowKegel_4;
			}
			break;
		case 5:
			{
				m_pcFlowKegelExpp=m_pcFlowKegel_5;
			}
			break;
		case 6:
			{
				m_pcFlowKegelExpp=m_pcFlowKegel_6;
			}
			break;
		case 7:
			{
				m_pcFlowKegelExpp=m_pcFlowKegel_7;
			}
			break;
		case 8:
			{
				m_pcFlowKegelExpp=m_pcFlowKegel_8;
			}
			break;
		case 9:
			{
				m_pcFlowKegelExpp=m_pcFlowKegel_9;
			}
			break;
		case 10:
			{
				m_pcFlowKegelExpp=m_pcFlowKegel_0;
			}
			break;
		default:
			{
				m_pcFlowKegelExpp=NULL;
			}
			break;
		}
		
		//###########################################################################
		int iExpFlow=getModel()->getDATAHANDLER()->GetExpFlowData();
		m_valueFlowExsp=417-(iExpFlow/153);

		if(m_pcFlowKegelExpp)
			m_pcFlowKegelExpp->Draw(hdcMem,71,m_valueFlowExsp);


		switch(m_iFlowKegelInsp)
		{
		case 0:
			{
				m_pcFlowKegelInsp=m_pcFlowKegel_0;
			}
			break;
		case 1:
			{
				m_pcFlowKegelInsp=m_pcFlowKegel_1;
			}
			break;
		case 2:
			{
				m_pcFlowKegelInsp=m_pcFlowKegel_2;
			}
			break;
		case 3:
			{
				m_pcFlowKegelInsp=m_pcFlowKegel_3;
			}
			break;
		case 4:
			{
				m_pcFlowKegelInsp=m_pcFlowKegel_4;
			}
			break;
		case 5:
			{
				m_pcFlowKegelInsp=m_pcFlowKegel_5;
			}
			break;
		case 6:
			{
				m_pcFlowKegelInsp=m_pcFlowKegel_6;
			}
			break;
		case 7:
			{
				m_pcFlowKegelInsp=m_pcFlowKegel_7;
			}
			break;
		case 8:
			{
				m_pcFlowKegelInsp=m_pcFlowKegel_8;
			}
			break;
		case 9:
			{
				m_pcFlowKegelInsp=m_pcFlowKegel_9;
			}
			break;
		case 10:
			{
				m_pcFlowKegelInsp=m_pcFlowKegel_0;
			}
			break;
		default:
			{
				m_pcFlowKegelInsp=NULL;
			}
			break;
		}

		int iInspFlow=getModel()->getDATAHANDLER()->GetInspFlowData();
		m_valueFlowInsp=417-(iInspFlow/153);

		if(m_pcFlowKegelInsp)
			m_pcFlowKegelInsp->Draw(hdcMem,151,m_valueFlowInsp);
	}

	if(bData)
	{
		TCHAR psz[MAX_PATH];

		SHORT iPmax=getModel()->getDATAHANDLER()->getBTBMessureDataPINSP();
		rc.top = 5;
		rc.bottom = 64;
		rc.left = 15; 
		rc.right = 175;
		if(iPmax==0)
			wsprintf(psz,_T("%d"), 0);
		else
			wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)iPmax)/10, 0));
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		SHORT iPmitt=getModel()->getDATAHANDLER()->getAVGMessureDataPmitt();
		rc.top = 77;
		rc.bottom = 126;
		rc.left = 15; 
		rc.right = 175;
		if(iPmitt==0)
			wsprintf(psz,_T("%d"), 0);
		else
			wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iPmitt)/10, 1));
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		SHORT iPEEP=getModel()->getDATAHANDLER()->getAVGMessureDataPEEP();
		rc.top = 139;
		rc.bottom = 188;
		rc.left = 15; 
		rc.right = 175;
		if(iPEEP==0)
			wsprintf(psz,_T("%d"), 0);
		else
		{
			wsprintf(psz,_T("%0.1f"), ((double)iPEEP)/10);
		}
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
		
		if(false==m_bGraphCursor)
		{
			SelectObject(hdcMem,g_hf11AcuBoldNum);

			rc.top = 450;
			rc.bottom = 476;
			rc.left = 65; 
			rc.right = 105;

			int iExpFlow2=getModel()->getDATAHANDLER()->GetExpFlowData();
			int iTemp=iExpFlow2/100;
			int iFactor=CTlsFloat::Round(((double)iTemp)/5, 0);
			iTemp=iFactor*5;
			wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iTemp)/10, 1));
			DrawText(hdcMem,psz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);

			rc.top = 450;
			rc.bottom = 476;
			rc.left = 143; 
			rc.right = 183;

			int iInspFlow2=getModel()->getDATAHANDLER()->GetInspFlowData();
			iTemp=iInspFlow2/100;
			iFactor=CTlsFloat::Round(((double)iTemp)/5, 0);
			iTemp=iFactor*5;
			wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iTemp)/10, 1));
			DrawText(hdcMem,psz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);
		}
	}

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



// **************************************************************************
// 
// **************************************************************************
bool CWndNumAvDUOPAP::DrawFrames(CDC* pDC)
{
	HDC hdc = *pDC;
	HBRUSH hPrevBrush = (HBRUSH)SelectObject(m_hDC, (HBRUSH)GetStockObject(WHITE_BRUSH));	
	HPEN hPrevPen = (HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(WHITE_PEN));

	if(m_pcNumPara1er)
	{
		m_pcNumPara1er->Draw(hdc,0,5);
		m_pcNumPara1er->Draw(hdc,0,67);
		m_pcNumPara1er->Draw(hdc,0,129);
	}
	
	RECT rc;

	if(true==m_bGraphCursor)
	{
		rc.left = 0;  
		rc.top = 208;  
		rc.right  = 190;  
		rc.bottom = 480;

		CBrush br(BACKGND);
		FillRect(hdc,&rc,br);


		rc.left = 0;  
		rc.top = 208;  
		rc.right  = 190;  
		rc.bottom = 419;

		if(m_pcNumInfoTop)
			m_pcNumInfoTop->Draw(hdc,0,191);
		if(m_pcNumInfoBot)
			m_pcNumInfoBot->Draw(hdc,0,419);

		CBrush br2(RGB(176,176,177));
		FillRect(hdc,&rc,br2);

		if(m_pbtnCursorUp)
			m_pbtnCursorUp->MoveWindow(70,217,51,68);
		if(m_pbtnCursorDown)
			m_pbtnCursorDown->MoveWindow(70,338,51,68);
		if(m_pbtnCursorLeft)
			m_pbtnCursorLeft->MoveWindow(2,286,68,51);
		if(m_pbtnCursorRight)
			m_pbtnCursorRight->MoveWindow(121,286,68,51);
		if(m_pbtnCursorUp)
			m_pbtnCursorUp->ShowWindow(SW_SHOW);
		if(m_pbtnCursorDown)
			m_pbtnCursorDown->ShowWindow(SW_SHOW);
		if(m_pbtnCursorLeft)
			m_pbtnCursorLeft->ShowWindow(SW_SHOW);
		if(m_pbtnCursorRight)
			m_pbtnCursorRight->ShowWindow(SW_SHOW);
		if(m_pcGraphCursor_pic)
			m_pcGraphCursor_pic->Draw(hdc,74,289);
	}
	else
	{
		rc.left = 0;  
		rc.top = 208;  
		rc.right  = 190;  
		rc.bottom = 460;

		if(m_pcNumericTop)
			m_pcNumericTop->Draw(hdc,0,191);
		if(m_pcNumericBot)
			m_pcNumericBot->Draw(hdc,0,460);

		CBrush br1(RGB(252,252,170));
		FillRect(hdc,&rc,br1);

		if(m_pcFlowmeterLeft)
			m_pcFlowmeterLeft->Draw(hdc,65,240);

		if(m_pcFlowmeterRight)
			m_pcFlowmeterRight->Draw(hdc,131,240);

		if(m_pbtnCursorUp)
			m_pbtnCursorUp->ShowWindow(SW_HIDE);
		if(m_pbtnCursorDown)
			m_pbtnCursorDown->ShowWindow(SW_HIDE);
		if(m_pbtnCursorLeft)
			m_pbtnCursorLeft->ShowWindow(SW_HIDE);
		if(m_pbtnCursorRight)
			m_pbtnCursorRight->ShowWindow(SW_HIDE);

		if(m_pbtnXmoveUp)
			m_pbtnXmoveUp->ShowWindow(SW_HIDE);
		if(m_pbtnXmoveDown)
			m_pbtnXmoveDown->ShowWindow(SW_HIDE);
		if(m_pbtnYmoveLeft)
			m_pbtnYmoveLeft->ShowWindow(SW_HIDE);
		if(m_pbtnYmoveRight)
			m_pbtnYmoveRight->ShowWindow(SW_HIDE);
	}	

	SelectObject(hdc, hPrevBrush);	
	SelectObject(hdc, hPrevPen);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CWndNumAvDUOPAP::DrawStaticText(CDC* pDC)
{
	HDC hdc = *pDC;
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	RECT rc;

	rc.top = 10;
	rc.bottom = m_lY;
	rc.left = 0; 
	rc.right = 190;

	if(false==m_bGraphCursor)
	{
		rc.top = 195;
		rc.bottom = m_lY;
		rc.left = 15; 
		rc.right = 190;
		pDC->DrawText(m_szName1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

		SelectObject(hdc,g_hf8AcuNorm);

		rc.top = 215;
		rc.bottom = m_lY;
		rc.left = 15; 
		rc.right = 190;
		pDC->DrawText(m_szUnitLMIN,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		
		SelectObject(hdc,g_hf8AcuBold);

		rc.top = 220;
		rc.bottom = 436;
		rc.left = 50; 
		rc.right = 123;
		pDC->DrawText(m_szName2,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);

		rc.top = 220;
		rc.bottom = 436;
		rc.left = 128; 
		rc.right = 201;
		pDC->DrawText(m_szName3,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);
	}

	rc.top = 10;
	rc.bottom = m_lY;
	rc.left = 0; 
	rc.right = 190;

	SetTextColor(hdc,RGB(0,0,0));
	SelectObject(hdc,g_hf13AcuBold);

	CSize sz1,sz2,sz3,sz7;

	rc.top = 9;
	rc.left = 15;
	pDC->DrawText(m_szName4,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz1 = pDC->GetTextExtent(m_szName4);

	rc.top = 71;
	pDC->DrawText(m_szName4,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz2 = sz1;

	rc.top = 133;
	pDC->DrawText(m_szName5,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz3 = pDC->GetTextExtent(m_szName5);
	
	rc.top = 10;
	rc.bottom = m_lY;
	rc.left = 0; 
	rc.right = 190;
	
	SetTextColor(hdc,RGB(0,0,0));
	SelectObject(hdc,g_hf7AcuBold);

	CSize sz1Note,sz2Note,sz3Note,sz7Note;

	rc.top = 13;
	rc.left = 17+sz1.cx;
	pDC->DrawText(m_szNameNote1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz1Note = pDC->GetTextExtent(m_szNameNote1);

	rc.top = 75;
	rc.left = 17+sz2.cx;
	pDC->DrawText(m_szNameNote2,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	sz2Note = pDC->GetTextExtent(m_szNameNote2);

	SelectObject(hdc,g_hf8AcuNorm);

	rc.top = 10;
	rc.left = 25+sz1.cx+sz1Note.cx;
	pDC->DrawText(m_szUnitPressure,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 72;
	rc.left = 25+sz2.cx+sz2Note.cx;
	pDC->DrawText(m_szUnitPressure,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 132;
	rc.left = 25+sz3.cx;
	pDC->DrawText(m_szUnitPressure,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	
	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}


// **************************************************************************
// 
// **************************************************************************
bool CWndNumAvDUOPAP::DrawLimits(CDC* pDC)
{
	HDC hdc = *pDC;
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
	int nPrevTxtColor=SetTextColor(hdc,RGB(200,0,0));
	RECT rc;	
	TCHAR psz[MAX_PATH];

	//################Pmax Max##################
	if(m_pcLimitHigh_Up)
		m_pcLimitHigh_Up->Draw(hdc,120,11);//right-60, top+2

	rc.top = 9;
	rc.bottom = 52;
	rc.left = 0;
	rc.right = 187;
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
	else 
	{
		SelectObject(hdc,g_hf11AcuBoldNum);
		if(getModel()->getALARMHANDLER()->getAlimitPIPmax()==0)
			wsprintf(psz,_T("%d"), 0);
		else
			wsprintf(psz,_T("%0.0f"), CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitPIPmax())/10, 0));
		pDC->DrawText(psz,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);

		if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit() == AL_AUTO)
		{
			SelectObject(hdc,g_hf9AcuBold);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}
	}

	

	//#########################PEEP Min##########################
	if(m_pcLimitLow_Up)
		m_pcLimitLow_Up->Draw(hdc,120,167);//right-60, bottom-16

	rc.top = 133;
	rc.bottom = 183;
	if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit() == AL_CALC)
	{
		SelectObject(hdc,g_hf9AcuBold);
		pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
	}
	else if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit() == AL_OFF)
	{
		SelectObject(hdc,g_hf9AcuBold);
		pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_OFF),&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
	}
	else 
	{
		SelectObject(hdc,g_hf11AcuBoldNum);
		if(getModel()->getALARMHANDLER()->getAlimitPEEPmin()==0)
			wsprintf(psz,_T("%d"), 0);
		else
			wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitPEEPmin())/10, 1));
		pDC->DrawText(psz,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);

		if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit() == AL_AUTO)
		{
			SelectObject(hdc,g_hf9AcuBold);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_AUTO),&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
	}

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
	return true;
}

// **************************************************************************
// 
// **************************************************************************
void CWndNumAvDUOPAP::StartFlowThread(void)
{
	startThread();

	if(m_pcwtFlowThread!=NULL)
	{
		delete m_pcwtFlowThread;
		m_pcwtFlowThread=NULL;
		if(m_hThreadFlow!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadFlow);
			m_hThreadFlow=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtFlowThread=AfxBeginThread(CFlowThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadFlow=m_pcwtFlowThread->m_hThread;
	m_pcwtFlowThread->m_bAutoDelete = FALSE; 
	m_pcwtFlowThread->ResumeThread();
}
//************************************
// Method:    StopFlowThread
// FullName:  CWndNumAvDUOPAP::StopFlowThread
// Access:    protected 
// Returns:   void
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
void CWndNumAvDUOPAP::StopFlowThread( void )
{
	if(doThread())
	{
		stopThread();
		eventDoFlow.SetEvent();

		if (WaitForSingleObject(m_pcwtFlowThread->m_hThread,3000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:036a"));
			if(!TerminateThread(m_pcwtFlowThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:036b"));
			}
		}
	}
	
}

bool CWndNumAvDUOPAP::doThread()
{
	bool bRes=false;
	EnterCriticalSection(&csDoThread);
	bRes=m_bDoThread;
	LeaveCriticalSection(&csDoThread);
	return bRes;
}
void CWndNumAvDUOPAP::startThread()
{
	EnterCriticalSection(&csDoThread);
	m_bDoThread=true;
	LeaveCriticalSection(&csDoThread);
}
void CWndNumAvDUOPAP::stopThread()
{
	EnterCriticalSection(&csDoThread);
	m_bDoThread=false;
	LeaveCriticalSection(&csDoThread);
}
//************************************
// Method:    CFlowThread
// FullName:  CFlowThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID pc
//
// 2015/06/19: checked for correct closing of thread
//************************************
static UINT CFlowThread( LPVOID pc )
{
	((CWndNumAvDUOPAP*)pc)->DoFlow();
	return TRUE;
}
//************************************
// Method:    DoFlow
// FullName:  CWndNumAvDUOPAP::DoFlow
// Access:    protected 
// Returns:   DWORD
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
DWORD CWndNumAvDUOPAP::DoFlow(void) 
{
	CeSetThreadPriority(m_pcwtFlowThread->m_hThread,250);
	
	DWORD WAIT=150;
	DWORD dwWait=0;
	DWORD dwDiff = 0;
	DWORD dwStart=GetTickCount();
	DWORD dwEnd=0;

	do
	{
		dwEnd=GetTickCount();

		if(dwEnd>=dwStart)
			dwDiff=dwEnd-dwStart;
		else
			dwDiff=0;

		if(dwDiff<WAIT)
		{
			dwWait=WAIT-dwDiff;
		}
		else
		{
			dwWait=1;
		}


		DWORD dw = ::WaitForSingleObject(eventDoFlow, dwWait);

		if(doThread()==false)
		{
			continue;
		}

		dwStart = GetTickCount();

		switch(dw)
		{
		case WAIT_OBJECT_0:
			break;
		default:
			{
				if(false==m_bGraphCursor)
				{
					if(m_valueFlowInsp<417)
					{
						m_iFlowKegelInsp++;
						if(m_iFlowKegelInsp>10)
							m_iFlowKegelInsp=0;
					}

					if(m_valueFlowExsp<417)
					{
						m_iFlowKegelExsp++;
						if(m_iFlowKegelExsp>10)
							m_iFlowKegelExsp=0;
					}

					DrawData(true, false, false, false, true);
				}
			}
			break;
		}
	}while(doThread());

	theApp.getLog()->WriteLine(_T("#THR:036"));

	return 0;
}
