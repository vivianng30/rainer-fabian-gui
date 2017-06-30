// SubParaBtn.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SubParaBtn.h"

extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf23AcuBold;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf13AcuBold;

// CSubParaBtn

IMPLEMENT_DYNAMIC(CSubParaBtn, CButton)

CSubParaBtn::CSubParaBtn(BTN btn, int nOffset,bool bScrollOver):
CParaBtn(btn,nOffset,bScrollOver)
{
	m_bTriggeredBreath=false;

	m_bTempValue=false;

	m_pcWait=NULL;
	m_pcWait1= NULL;
	m_pcWait2= NULL;
	m_pcWait3= NULL;
	m_pcWait4= NULL;
	m_pcWait5= NULL;
	m_pcWait6= NULL;
	m_pcWait7= NULL;
	m_pcWait8= NULL;
	m_iWaitCount=0;



	m_bCalRunning=false;
}

CSubParaBtn::~CSubParaBtn()
{
	KillTimer(TRIGGERTIMER1);
	KillTimer(AUTOOXYCALTIMER);

	delete m_pcWait;
	m_pcWait=NULL;
	
	delete m_pcWait1;
	m_pcWait1=NULL;
	
	delete m_pcWait2;
	m_pcWait2=NULL;
	
	delete m_pcWait3;
	m_pcWait3=NULL;
	
	delete m_pcWait4;
	m_pcWait4=NULL;
	
	delete m_pcWait5;
	m_pcWait5=NULL;
	
	delete m_pcWait6;
	m_pcWait6=NULL;

	delete m_pcWait7;
	m_pcWait7=NULL;

	delete m_pcWait8;
	m_pcWait8=NULL;


}





// CSubParaBtn-Meldungshandler
BEGIN_MESSAGE_MAP(CSubParaBtn, CButton)
	//{{AFX_MSG_MAP(CParaBtn_ITIME)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
	/*ON_WM_SETFOCUS()*/
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSubParaBtn::Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v) 
{
	SIZE sz;
	sz.cx=m_btn.pcBmpUp->Width();
	sz.cy=m_btn.pcBmpUp->Height();
	CRect cRect(m_btn.poPosition,sz);

	if(CButton::Create(_T("btn"),dwStyle,cRect,pParentWnd,m_btn.wID))
	{
		CClientDC dc(this);
		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBm=CreateCompatibleBitmap(dc.m_hDC,m_btn.pcBmpUp->Width(),m_btn.pcBmpUp->Height());
		m_hBmPrev=(HBITMAP)SelectObject(m_hDC,m_hBm);

		m_rcClient.left=m_rcClient.top=0;
		m_rcClient.right=sz.cx;
		m_rcClient.bottom=sz.cy;


		memcpy(&m_v,&v,sizeof(fVALUE));

		m_pcWait=NULL;
		m_pcWait1= new CBmp(theApp.m_hInstance,m_hDC,	IDB_WAIT30_YEL1);
		m_pcWait2= new CBmp(theApp.m_hInstance,m_hDC,	IDB_WAIT30_YEL2);
		m_pcWait3= new CBmp(theApp.m_hInstance,m_hDC,	IDB_WAIT30_YEL3);
		m_pcWait4= new CBmp(theApp.m_hInstance,m_hDC,	IDB_WAIT30_YEL4);
		m_pcWait5= new CBmp(theApp.m_hInstance,m_hDC,	IDB_WAIT30_YEL5);
		m_pcWait6= new CBmp(theApp.m_hInstance,m_hDC,	IDB_WAIT30_YEL6);
		m_pcWait7= new CBmp(theApp.m_hInstance,m_hDC,	IDB_WAIT30_YEL7);
		m_pcWait8= new CBmp(theApp.m_hInstance,m_hDC,	IDB_WAIT30_YEL8);

		m_pcWarning_Up		= new CBmp(theApp.m_hInstance,m_hDC,IDB_PARA_WARN_SUB);

		return 1;
	}
	return 0;
}

void CSubParaBtn::SetParaValue(int v,bool bRedraw)
{
	//bool bRedraw=false;
	if(m_v.iValue!=v)
		bRedraw=true;

	m_v.iValue=v;
	if(bRedraw)
	{
		RefreshBtn();
	}

}


void CSubParaBtn::RefreshBtn(bool bTempValue)
{
	m_bTempValue=bTempValue;

	CClientDC dc(this);

	//m_btn.pcBmpUp->Draw(m_hDC);
	Draw(BTN_DW_UP);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
}
// **************************************************************************
// Painting
// **************************************************************************
void CSubParaBtn::Draw(int nState)
{
	if(!m_bDisableText)
	{
		switch(GetBtnId())
		{
		case IDC_BTN_SUBPARA_I_E:
			{
				DrawSUBPARA_I_E(nState);
			}
			break;
		case IDC_BTN_SUBPARA_TRIGGER:
			{
				DrawSUBPARA_TRIGGER(nState);
			}
			break;
		case IDC_BTN_SUBPARA_O2REAL:
			{
				DrawSUBPARA_O2REAL(nState);
			}
			break;
		case IDC_BTN_SUBPARA_O2FLUSH:
			{
				DrawSUBPARA_O2FLUSH(nState);
			}
			break;
		case IDC_BTN_SUBPARA_BIASFLOW:
			{
				DrawSUBPARA_BIASFLOW(nState);
			}
			break;
		case IDC_BTN_SUBPARA_DEMFLOW:
			{
				DrawSUBPARA_DEMFLOW(nState);
			}
			break;
		case IDC_BTN_SUBPARA_PSVTIME:
			{
				DrawSUBPARA_PSVTIME(nState);
			}
			break;
		case IDC_BTN_SUBPARA_BPM:
			{
				DrawSUBPARA_BPM(nState);
			}
			break;
		case IDC_BTN_SUBPARA_ETIME:
			{
				DrawSUBPARA_ETIME(nState);
			}
			break;
		case IDC_BTN_SUBPARA_LEAKCOMP:
			{
				DrawSUBPARA_LEAKCOMP(nState);
			}
			break;
		default:
			{
				DrawDEFAULT(nState);
			}
			break;
		}
	}
}

void CSubParaBtn::DrawSUBPARA_I_E(int nState)
{
	int nTxtColorSubBtn=GetTextColorStateDependend(nState);
	int iTempIErelation = 0;
	double dTempVal=0;

	iTempIErelation =getModel()->getDATAHANDLER()->GetCurrentIERelation100();

	if(iTempIErelation<100)
	{
		m_btn.pcBmpDisabled->Draw(m_hDC);
	}
	else
	{
		m_btn.pcBmpUp->Draw(m_hDC);
	}

	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf10AcuBold);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColorSubBtn);

	//Sub-Button
	RECT rcSubBtn;
	rcSubBtn.top = m_rcClient.top+10;
	rcSubBtn.bottom = m_rcClient.bottom-10;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	TCHAR psz[MAX_PATH];

	if(		getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV)
	{
		SelectObject(m_hDC,g_hf7AcuBold);
		rcSubBtn.top = m_rcClient.top+3;
		rcSubBtn.bottom = m_rcClient.bottom-12;
		rcSubBtn.left = m_rcClient.left;
		rcSubBtn.right = m_rcClient.right;
		wsprintf(psz,_T("%s"),getModel()->GetLanguageString(IDS_PARA_PSV));
		DrawText(m_hDC,psz,-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);

		SelectObject(m_hDC,g_hf14AcuBold);
		rcSubBtn.top = m_rcClient.top+9;
		rcSubBtn.right = m_rcClient.right-25;
		wsprintf(psz,_T("%0.2f"),(float)((double)m_v.iValue/1000));
		DrawText(m_hDC,psz,-1,&rcSubBtn,DT_VCENTER|DT_SINGLELINE|DT_CENTER);

		SelectObject(m_hDC,g_hf7AcuBold);
		rcSubBtn.right = m_rcClient.right;
		rcSubBtn.left = m_rcClient.right-25;
		rcSubBtn.bottom = rcSubBtn.bottom-10;
		wsprintf(psz,_T("%s"),getModel()->GetLanguageString(IDS_UNIT_SECONDS));
		DrawText(m_hDC,psz,-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

		CDC* pDC=CDC::FromHandle(m_hDC);
		CPen pen, *poldpen;
		pen.CreatePen(PS_SOLID,1,BLACK_PEN);

		poldpen=pDC->SelectObject(&pen);

		rcSubBtn.left = m_rcClient.left;
		rcSubBtn.bottom = m_rcClient.bottom-12;
		pDC->MoveTo(rcSubBtn.left, rcSubBtn.bottom-8);
		pDC->LineTo(rcSubBtn.right,rcSubBtn.bottom-8);
		pDC->SelectObject(poldpen);

		rcSubBtn.top = m_rcClient.top+5;
		rcSubBtn.bottom = m_rcClient.bottom-3;
		rcSubBtn.left = m_rcClient.left;
		rcSubBtn.right = m_rcClient.right;

		SelectObject(m_hDC,g_hf7AcuBold);

		if(iTempIErelation<100)
		{
			iTempIErelation=getModel()->getDATAHANDLER()->GetCurrentEIRelation100();
			if(iTempIErelation<110)
				wsprintf(psz,_T("1.1 : 1"));
			else
				wsprintf(psz,_T("%0.1f : 1"),CTlsFloat::Round((double)iTempIErelation/100, 1));
		}
		else if(iTempIErelation==100)
			wsprintf(psz,_T("%s 1 : %0.0f"),m_pszNameText,CTlsFloat::Round((double)iTempIErelation/100, 1));
		else if(iTempIErelation>1000)
			wsprintf(psz,_T("%s < 1 : 10"),m_pszNameText);
		else
			wsprintf(psz,_T("%s 1 : %0.1f"),m_pszNameText,CTlsFloat::Round((double)iTempIErelation/100, 1));

		DrawText(m_hDC,psz,-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
		pen.DeleteObject();

	}
	else
	{
		if(iTempIErelation<100)
		{
			iTempIErelation = getModel()->getDATAHANDLER()->GetCurrentEIRelation100();
			if(iTempIErelation<110)
				wsprintf(psz,_T("1.1 : 1"));
			else
				wsprintf(psz,_T("%0.1f : 1"),CTlsFloat::Round((double)iTempIErelation/100, 1));
		}
		else if(iTempIErelation==100)
			wsprintf(psz,_T("1 : %0.0f"),CTlsFloat::Round((double)iTempIErelation/100, 1));
		else if(iTempIErelation>1000)
			wsprintf(psz,_T("< 1 : 10"));
		else
			wsprintf(psz,_T("1 : %0.1f"),CTlsFloat::Round((double)iTempIErelation/100, 1));

		rcSubBtn.top = m_rcClient.top+5;
		rcSubBtn.bottom = m_rcClient.bottom-5;
		rcSubBtn.left = m_rcClient.left;
		rcSubBtn.right = m_rcClient.right;

		SelectObject(m_hDC,g_hf7AcuBold);
		DrawText(m_hDC,m_pszNameText,-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);

		SelectObject(m_hDC,g_hf14AcuBold);
		rcSubBtn.top = m_rcClient.top+9;
		DrawText(m_hDC,psz,-1,&rcSubBtn,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
	}

	SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);

}
void CSubParaBtn::DrawSUBPARA_TRIGGER(int nState)
{
	if(		getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP)
	{
		DrawSUBPARA_TRIGGER_DUOPAP(nState);
	}
	else if(getModel()->getCONFIG()->GetCurMode()==VM_NCPAP
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_NCPAP)
	{
		DrawSUBPARA_TRIGGER_NCPAP(nState);
	}
	else if(getModel()->getCONFIG()->GetCurMode()==VM_CPAP
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_CPAP)
	{
		DrawSUBPARA_TRIGGER_CPAP(nState);
	}
	else
	{
		DrawSUBPARA_TRIGGER_DEFAULT(nState);
	}
}
void CSubParaBtn::DrawSUBPARA_TRIGGER_DUOPAP(int nState)
{
	int nTxtColorSubBtn=GetTextColorStateDependend(nState);
	int iTempIErelation = 0;
	double dTempVal=0;

	if(m_bTriggeredBreath)
	{
		m_btn.pcBmpDisabled->Draw(m_hDC);
	}
	else
	{
		m_btn.pcBmpUp->Draw(m_hDC);
	}

	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf10AcuBold);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColorSubBtn);

	//Sub-Button
	RECT rcSubBtn;
	rcSubBtn.top = m_rcClient.top+10;
	rcSubBtn.bottom = m_rcClient.bottom-10;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	//TCHAR psz[MAX_PATH];

	rcSubBtn.top = m_rcClient.top+5;
	rcSubBtn.bottom = m_rcClient.bottom-5;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	SelectObject(m_hDC,g_hf7AcuBold);

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true 
		&&	getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET)
	{
		rcSubBtn.top = m_rcClient.top+16;
		rcSubBtn.bottom = m_rcClient.bottom-16;

		DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_FLOWSENSOR),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
		DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_OFF),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
	}
	else
	{
		bool bTriggerOff=false;
		if(		getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
			&&	getModel()->getDATAHANDLER()->PARADATA()->GetITimeNMODEPara()>600)
		{
			bTriggerOff=true;
		}
		else if(		getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP
			&&	getModel()->getDATAHANDLER()->PRESET()->GetITimeNMODEPara()>600)
		{
			bTriggerOff=true;
		}

		if(getModel()->getDATAHANDLER()->getTriggerOptionNMODE()==TRIGGER_PRESSURE && false==bTriggerOff)
		{
			rcSubBtn.top = m_rcClient.top+16;
			rcSubBtn.bottom = m_rcClient.bottom-16;

			DrawText(m_hDC,getModel()->GetLanguageString(IDS_GRAPH_PRESSURE),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_TRIGGER),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE

			if(		getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
				&&	getModel()->getDATAHANDLER()->PARADATA()->GetITimeNMODEPara()>600)
			{
				rcSubBtn.top = m_rcClient.top+15;
				rcSubBtn.bottom = m_rcClient.bottom-3;

				SetTextColor(m_hDC,RGB(255,0,0));
				SelectObject(m_hDC,g_hf7AcuNorm);
				DrawText(m_hDC,_T("T-high"),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
			}
			else if(		getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP
				&&	getModel()->getDATAHANDLER()->PRESET()->GetITimeNMODEPara()>600)
			{
				rcSubBtn.top = m_rcClient.top+15;
				rcSubBtn.bottom = m_rcClient.bottom-3;

				SetTextColor(m_hDC,RGB(255,0,0));
				SelectObject(m_hDC,g_hf7AcuNorm);
				DrawText(m_hDC,_T("T-high"),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
			}
		}
		else if(getModel()->getDATAHANDLER()->getTriggerOptionNMODE()==TRIGGER_FLOW && false==bTriggerOff)
		{
			rcSubBtn.top = m_rcClient.top+16;
			rcSubBtn.bottom = m_rcClient.bottom-16;


			DrawText(m_hDC,getModel()->GetLanguageString(IDS_GRAPH_FLOW),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_TRIGGER),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
		}
		else//TRIGGER_OFF
		{
			rcSubBtn.top = m_rcClient.top+16;
			rcSubBtn.bottom = m_rcClient.bottom-16;

			if(bTriggerOff)
			{
				rcSubBtn.top = m_rcClient.top+15;
				rcSubBtn.bottom = m_rcClient.bottom-5;

				SetTextColor(m_hDC,RGB(255,0,0));
				DrawText(m_hDC,_T("T-high"),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE

				if(m_pcWarning_Up)
					m_pcWarning_Up->Draw(m_hDC,65,m_rcClient.bottom-20);

				rcSubBtn.top = m_rcClient.top+12;
				rcSubBtn.bottom = m_rcClient.bottom-20;
			}
			else
			{
				rcSubBtn.top = m_rcClient.top+16;
				rcSubBtn.bottom = m_rcClient.bottom-16;
			}

			DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_TRIGGER),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_NOTACTIVE),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
		}
	}

	SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);
}
void CSubParaBtn::DrawSUBPARA_TRIGGER_NCPAP(int nState)
{
	int nTxtColorSubBtn=GetTextColorStateDependend(nState);
	int iTempIErelation = 0;
	double dTempVal=0;

	if(m_bTriggeredBreath)
	{
		m_btn.pcBmpDisabled->Draw(m_hDC);
	}
	else
	{
		m_btn.pcBmpUp->Draw(m_hDC);
	}

	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf10AcuBold);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColorSubBtn);

	//Sub-Button
	RECT rcSubBtn;
	rcSubBtn.top = m_rcClient.top+10;
	rcSubBtn.bottom = m_rcClient.bottom-10;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	//TCHAR psz[MAX_PATH];

	rcSubBtn.top = m_rcClient.top+5;
	rcSubBtn.bottom = m_rcClient.bottom-5;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	SelectObject(m_hDC,g_hf7AcuBold);

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true 
		&&	getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET)
	{
		rcSubBtn.top = m_rcClient.top+16;
		rcSubBtn.bottom = m_rcClient.bottom-16;

		DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_FLOWSENSOR),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);
		DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_OFF),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
	}
	else
	{
		if(getModel()->getDATAHANDLER()->getTriggerOptionNMODE()==TRIGGER_PRESSURE)
		{
			rcSubBtn.top = m_rcClient.top+8;
			rcSubBtn.bottom = m_rcClient.bottom-8;

			DrawText(m_hDC,_T("Apnea"),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
			DrawText(m_hDC,_T("Detection"),-1,&rcSubBtn,DT_VCENTER|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_GRAPH_PRESSURE),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

		}
		else if(getModel()->getDATAHANDLER()->getTriggerOptionNMODE()==TRIGGER_FLOW)
		{
			rcSubBtn.top = m_rcClient.top+8;
			rcSubBtn.bottom = m_rcClient.bottom-8;

			DrawText(m_hDC,_T("Apnea"),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
			DrawText(m_hDC,_T("Detection"),-1,&rcSubBtn,DT_VCENTER|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_GRAPH_FLOW),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
		}
		else//TRIGGER_OFF
		{
			rcSubBtn.top = m_rcClient.top+16;
			rcSubBtn.bottom = m_rcClient.bottom-16;

			rcSubBtn.top = m_rcClient.top+16;
			rcSubBtn.bottom = m_rcClient.bottom-16;

			DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_TRIGGER),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_NOTACTIVE),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
		}
	}

	SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);
}
void CSubParaBtn::DrawSUBPARA_TRIGGER_CPAP(int nState)
{
	int nTxtColorSubBtn=GetTextColorStateDependend(nState);
	int iTempIErelation = 0;
	double dTempVal=0;

	if(m_bTriggeredBreath)
	{
		m_btn.pcBmpDisabled->Draw(m_hDC);
	}
	else
	{
		m_btn.pcBmpUp->Draw(m_hDC);
	}

	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf10AcuBold);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColorSubBtn);

	//Sub-Button
	RECT rcSubBtn;
	rcSubBtn.top = m_rcClient.top+10;
	rcSubBtn.bottom = m_rcClient.bottom-10;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	//TCHAR psz[MAX_PATH];

	rcSubBtn.top = m_rcClient.top+5;
	rcSubBtn.bottom = m_rcClient.bottom-5;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	SelectObject(m_hDC,g_hf7AcuBold);

	if(		getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true
		&& getModel()->getDATAHANDLER()->getTriggerOptionCONV()!=TRIGGER_PRESSURE
		&& getModel()->getCONFIG()->GetCurMode()==VM_PRE_CPAP)
	{
		rcSubBtn.top = m_rcClient.top+8;
		rcSubBtn.bottom = m_rcClient.bottom-8;

		DrawText(m_hDC,_T("Apnea"),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
		DrawText(m_hDC,_T("Detection"),-1,&rcSubBtn,DT_VCENTER|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
		DrawText(m_hDC,getModel()->GetLanguageString(IDS_GRAPH_PRESSURE),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
	}
	else if(		getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true
		&& getModel()->getDATAHANDLER()->getTriggerOptionCONV()!=TRIGGER_PRESSURE)
	{
		rcSubBtn.top = m_rcClient.top+16;
		rcSubBtn.bottom = m_rcClient.bottom-16;

		DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_FLOWSENSOR),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);
		DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_OFF),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
	}
	else
	{
		if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_PRESSURE)
		{
			rcSubBtn.top = m_rcClient.top+8;
			rcSubBtn.bottom = m_rcClient.bottom-8;

			DrawText(m_hDC,_T("Apnea"),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
			DrawText(m_hDC,_T("Detection"),-1,&rcSubBtn,DT_VCENTER|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_GRAPH_PRESSURE),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

		}
		else if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_FLOW)
		{
			rcSubBtn.top = m_rcClient.top+8;
			rcSubBtn.bottom = m_rcClient.bottom-8;

			DrawText(m_hDC,_T("Apnea"),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
			DrawText(m_hDC,_T("Detection"),-1,&rcSubBtn,DT_VCENTER|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_GRAPH_FLOW),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
		}
		else//TRIGGER_OFF
		{
			rcSubBtn.top = m_rcClient.top+16;
			rcSubBtn.bottom = m_rcClient.bottom-16;

			rcSubBtn.top = m_rcClient.top+16;
			rcSubBtn.bottom = m_rcClient.bottom-16;

			DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_TRIGGER),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_NOTACTIVE),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
		}
	}

	SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);
}
void CSubParaBtn::DrawSUBPARA_TRIGGER_DEFAULT(int nState)
{
	int nTxtColorSubBtn=GetTextColorStateDependend(nState);
	int iTempIErelation = 0;
	double dTempVal=0;

	if(m_bTriggeredBreath)
	{
		m_btn.pcBmpDisabled->Draw(m_hDC);
	}
	else
	{
		m_btn.pcBmpUp->Draw(m_hDC);
	}

	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf10AcuBold);
	RECT rcSubBtn;
	TCHAR psz[MAX_PATH];
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColorSubBtn);
	
	rcSubBtn.top = m_rcClient.top+5;
	rcSubBtn.bottom = m_rcClient.bottom-5;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	SelectObject(m_hDC,g_hf7AcuBold);

	if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_PRESSURE)
	{
		rcSubBtn.top = m_rcClient.top+15;
		rcSubBtn.bottom = m_rcClient.bottom-15;

		DrawText(m_hDC,getModel()->GetLanguageString(IDS_GRAPH_PRESSURE),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
		DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_TRIGGER),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);//todo NEWLANGUAGE
	}
	else
	{
		DrawText(m_hDC,m_pszNameText,-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);

		if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_VOLUME)
		{
			wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)m_v.iValue)/10, 1));
		}
		else if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_FLOW)
		{ 
			wsprintf(psz,_T("%0.2f"),CTlsFloat::Round((((double)m_v.iValue)/10)*0.06, 2));
		}

		SelectObject(m_hDC,g_hf14AcuBold);
		rcSubBtn.top = m_rcClient.top+9;
		DrawText(m_hDC,psz,-1,&rcSubBtn,DT_VCENTER|DT_SINGLELINE|DT_CENTER);

		SelectObject(m_hDC,g_hf7AcuNorm);
		DrawText(m_hDC,m_pszUnitText,-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
	}

	SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);
}

void CSubParaBtn::DrawSUBPARA_O2REAL(int nState)
{
	int nTxtColorSubBtn=GetTextColorStateDependend(nState);
	int iTempIErelation = 0;
	double dTempVal=0;

	bool bPRICOrunning=false;
	if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
	{
		if(getModel()->getDATAHANDLER()->getPRICOState())
		{
			bPRICOrunning=true;
		}
	}

	if(bPRICOrunning && !m_bCalRunning)
	{
		m_btn.pcBmpDisabled->Draw(m_hDC);
	}
	else
	{
		m_btn.pcBmpUp->Draw(m_hDC);
	}

	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf10AcuBold);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColorSubBtn);

	//Sub-Button
	RECT rcSubBtn;
	rcSubBtn.top = m_rcClient.top+10;
	rcSubBtn.bottom = m_rcClient.bottom-10;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	TCHAR psz[MAX_PATH];

	SelectObject(m_hDC,g_hf7AcuBold);
	CStringW szText=_T("");
	if(bPRICOrunning && !m_bCalRunning)
	{
		CStringW szText=_T("PRICO");

		rcSubBtn.top = m_rcClient.top+5;
		rcSubBtn.bottom = m_rcClient.bottom-5;
		rcSubBtn.left = m_rcClient.left;
		rcSubBtn.right = m_rcClient.right;

		DrawText(m_hDC,szText,-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);
	}
	else
	{
		CStringW szText=_T("O  ");

		rcSubBtn.top = m_rcClient.top+5;
		rcSubBtn.bottom = m_rcClient.bottom-5;
		rcSubBtn.left = m_rcClient.left;
		rcSubBtn.right = m_rcClient.right;

		DrawText(m_hDC,szText,-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);

		szText=_T("  2");

		rcSubBtn.top = m_rcClient.top+10;
		DrawText(m_hDC,szText,-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);
	}




	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;


	if(m_bCalRunning)
	{
		//hourglass 
		if(m_iWaitCount>8)
			m_iWaitCount=1;

		switch(m_iWaitCount)
		{
		case 1:
			{
				m_pcWait=m_pcWait1;
			}
			break;
		case 2:
			{
				m_pcWait=m_pcWait2;
			}
			break;
		case 3:
			{
				m_pcWait=m_pcWait3;
			}
			break;
		case 4:
			{
				m_pcWait=m_pcWait4;
			}
			break;
		case 5:
			{
				m_pcWait=m_pcWait5;
			}
			break;
		case 6:
			{
				m_pcWait=m_pcWait6;
			}
			break;
		case 7:
			{
				m_pcWait=m_pcWait7;
			}
			break;
		case 8:
			{
				m_pcWait=m_pcWait8;
			}
			break;
		default:
			{
				m_pcWait=NULL;
			}
			break;
		}

		if(m_pcWait)
			m_pcWait->Draw(m_hDC,27,27);
	}
	else
	{
		if(m_v.iValue==-1)
		{
			if(getModel()->getDATAHANDLER()->GetOxySensorCalState()==OS_DEFECT)
				wsprintf(psz,getModel()->GetLanguageString(IDS_TXT_DEFECT));
			else
				wsprintf(psz,getModel()->GetLanguageString(IDS_TXT_OFF));
		}
		else
		{
			int iOxy=CTlsFloat::Round(((double)m_v.iValue)/10, 0);
			if(iOxy<19)
			{
				wsprintf(psz,_T("--"));
			}
			else
			{
				//wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)m_v.iValue)/10, 1));
				wsprintf(psz,_T("%d"),iOxy);
			}
		}

		if(		getModel()->getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF
			||	getModel()->getDATAHANDLER()->GetOxySensorCalState()==OS_REPLACE)
		{
			if(bPRICOrunning)
			{
				if(m_pcWarning_Dw)
					m_pcWarning_Dw->Draw(m_hDC,65,m_rcClient.top+5);
			}
			else
			{
				if(m_pcWarning_Up)
					m_pcWarning_Up->Draw(m_hDC,65,m_rcClient.top+5);
			}
		}


		SelectObject(m_hDC,g_hf14AcuBold);

		rcSubBtn.top = m_rcClient.top+9;
		DrawText(m_hDC,psz,-1,&rcSubBtn,DT_VCENTER|DT_SINGLELINE|DT_CENTER);

		SelectObject(m_hDC,g_hf7AcuNorm);

		DrawText(m_hDC,m_pszUnitText,-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
	}

	SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);

}
void CSubParaBtn::DrawSUBPARA_O2FLUSH(int nState)
{
	int nTxtColorSubBtn=GetTextColorStateDependend(nState);
	int iTempIErelation = 0;
	double dTempVal=0;

	if(getModel()->isO2FlushActive())
	{
		m_btn.pcBmpDisabled->Draw(m_hDC);
	}
	else
	{
		m_btn.pcBmpUp->Draw(m_hDC);
	}

	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf10AcuBold);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColorSubBtn);

	//Sub-Button
	RECT rcSubBtn;
	rcSubBtn.top = m_rcClient.top+10;
	rcSubBtn.bottom = m_rcClient.bottom-10;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	TCHAR psz[MAX_PATH];

	SelectObject(m_hDC,g_hf7AcuBold);
	CDC* pDC=CDC::FromHandle(m_hDC);
	rcSubBtn.top = m_rcClient.top+5;
	rcSubBtn.bottom = m_rcClient.bottom-5;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	CSize szO2 = pDC->GetTextExtent(getModel()->GetLanguageString(IDS_PARA_O2));
	CSize szFlush = pDC->GetTextExtent(getModel()->GetLanguageString(IDS_PARA_FLUSH));
	LONG szComplete=szO2.cx + szFlush.cx;
	LONG paraBtnX=rcSubBtn.right-rcSubBtn.left;

	if(szComplete>paraBtnX)
	{
		if(getModel()->GetLanguageString(IDS_PARA_O2)==_T("O2"))
		{
			DrawText(m_hDC,_T("O"),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_LEFT);
			rcSubBtn.left=rcSubBtn.left+3+pDC->GetTextExtent(_T("O")).cx;

			rcSubBtn.top = m_rcClient.top+10;
			DrawText(m_hDC,_T("2"),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_LEFT);

			rcSubBtn.top = m_rcClient.top+5;
			rcSubBtn.left=rcSubBtn.left+1+pDC->GetTextExtent(_T("2")).cx;
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_FLUSH),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_O2),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_LEFT);
			rcSubBtn.left=rcSubBtn.left+1+szO2.cx;
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_FLUSH),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
	}
	else
	{
		rcSubBtn.left=((paraBtnX-szComplete)/2);

		if(getModel()->GetLanguageString(IDS_PARA_O2)==_T("O2"))
		{
			DrawText(m_hDC,_T("O"),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_LEFT);
			rcSubBtn.left=rcSubBtn.left+3+pDC->GetTextExtent(_T("0")).cx;
			rcSubBtn.top = m_rcClient.top+10;
			DrawText(m_hDC,_T("2"),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_LEFT);

			rcSubBtn.top = m_rcClient.top+5;
			rcSubBtn.left=rcSubBtn.left+1+pDC->GetTextExtent(_T("2")).cx;
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_FLUSH),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_O2),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_LEFT);
			rcSubBtn.left=rcSubBtn.left+1+szO2.cx;
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_FLUSH),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
	}

	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;
	rcSubBtn.top = m_rcClient.top+9;

	SelectObject(m_hDC,g_hf14AcuBold);

	if(m_v.iValue==0)
	{
		DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_OFF),-1,&rcSubBtn,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
	}
	else
	{
		wsprintf(psz,_T("%d"),m_v.iValue);
		DrawText(m_hDC,psz,-1,&rcSubBtn,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
	}

	if(m_v.iValue!=0)
	{
		SelectObject(m_hDC,g_hf7AcuNorm);
		DrawText(m_hDC,m_pszUnitText,-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
	}

	SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);

}

void CSubParaBtn::DrawSUBPARA_BIASFLOW(int nState)
{
	int nTxtColorSubBtn=GetTextColorStateDependend(nState);
	int iTempIErelation = 0;
	double dTempVal=0;

	m_btn.pcBmpUp->Draw(m_hDC);

	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf10AcuBold);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColorSubBtn);

	//Sub-Button
	RECT rcSubBtn;
	rcSubBtn.top = m_rcClient.top+10;
	rcSubBtn.bottom = m_rcClient.bottom-10;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	//TCHAR psz[MAX_PATH];

	SelectObject(m_hDC,g_hf7AcuBold);
	if(getModel()->getCONFIG()->getFreshgasExt())
	{
		rcSubBtn.top = m_rcClient.top+15;
		rcSubBtn.bottom = m_rcClient.bottom-15;
		rcSubBtn.left = m_rcClient.left;
		rcSubBtn.right = m_rcClient.right;

		DrawText(m_hDC,getModel()->GetLanguageString(IDS_BTN_BIAS_EXT1),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);
		DrawText(m_hDC,getModel()->GetLanguageString(IDS_BTN_BIAS_EXT2),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
	}
	else
	{
		rcSubBtn.top = m_rcClient.top+15;
		rcSubBtn.bottom = m_rcClient.bottom-15;
		rcSubBtn.left = m_rcClient.left;
		rcSubBtn.right = m_rcClient.right;

		DrawText(m_hDC,getModel()->GetLanguageString(IDS_BTN_BIAS_INT1),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);
		DrawText(m_hDC,getModel()->GetLanguageString(IDS_BTN_BIAS_INT2),-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
	}

	SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);

}
void CSubParaBtn::DrawSUBPARA_PSVTIME(int nState)
{
	int nTxtColorSubBtn=GetTextColorStateDependend(nState);
	int iTempIErelation = 0;
	double dTempVal=0;

	m_btn.pcBmpUp->Draw(m_hDC);

	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf10AcuBold);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColorSubBtn);

	//Sub-Button
	RECT rcSubBtn;
	rcSubBtn.top = m_rcClient.top+10;
	rcSubBtn.bottom = m_rcClient.bottom-10;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	TCHAR psz[MAX_PATH];

	rcSubBtn.top = m_rcClient.top+5;
	rcSubBtn.bottom = m_rcClient.bottom-5;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	SelectObject(m_hDC,g_hf7AcuBold);

	DrawText(m_hDC,m_pszNameText,-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);

	SelectObject(m_hDC,g_hf14AcuBold);

	rcSubBtn.top = m_rcClient.top+5;
	rcSubBtn.bottom = m_rcClient.bottom-3;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	SelectObject(m_hDC,g_hf14AcuBold);

	wsprintf(psz,_T("%0.2f"),(float)((double)m_v.iValue/1000));

	DrawText(m_hDC,psz,-1,&rcSubBtn,DT_VCENTER|DT_SINGLELINE|DT_CENTER);

	SelectObject(m_hDC,g_hf7AcuNorm);

	DrawText(m_hDC,m_pszUnitText,-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

	SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);

}
void CSubParaBtn::DrawSUBPARA_DEMFLOW(int nState)
{
	int nTxtColorSubBtn=GetTextColorStateDependend(nState);
	int iTempIErelation = 0;
	double dTempVal=0;

	m_btn.pcBmpUp->Draw(m_hDC);

	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf10AcuBold);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColorSubBtn);

	//Sub-Button
	RECT rcSubBtn;
	rcSubBtn.top = m_rcClient.top+10;
	rcSubBtn.bottom = m_rcClient.bottom-10;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	TCHAR psz[MAX_PATH];

	rcSubBtn.top = m_rcClient.top+5;
	rcSubBtn.bottom = m_rcClient.bottom-5;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	SelectObject(m_hDC,g_hf7AcuBold);

	DrawText(m_hDC,m_pszNameText,-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);

	wsprintf(psz,_T("%0.1f"),(float)((double)m_v.iValue/1000));

	SelectObject(m_hDC,g_hf14AcuBold);

	rcSubBtn.top = m_rcClient.top+9;
	DrawText(m_hDC,psz,-1,&rcSubBtn,DT_VCENTER|DT_SINGLELINE|DT_CENTER);

	SelectObject(m_hDC,g_hf7AcuNorm);

	DrawText(m_hDC,m_pszUnitText,-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

	SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);
}
void CSubParaBtn::DrawSUBPARA_BPM(int nState)
{
	int nTxtColorSubBtn=GetTextColorStateDependend(nState);
	int iTempIErelation = 0;
	double dTempVal=0;

	m_btn.pcBmpUp->Draw(m_hDC);

	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf10AcuBold);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColorSubBtn);

	//Sub-Button
	RECT rcSubBtn;
	rcSubBtn.top = m_rcClient.top+10;
	rcSubBtn.bottom = m_rcClient.bottom-10;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	TCHAR psz[MAX_PATH];

	rcSubBtn.top = m_rcClient.top+5;
	rcSubBtn.bottom = m_rcClient.bottom-5;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	SelectObject(m_hDC,g_hf7AcuBold);

	DrawText(m_hDC,m_pszNameText,-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);

	wsprintf(psz,_T("%d"),getModel()->getDATAHANDLER()->GetCurrentBPMPara());
	
	SelectObject(m_hDC,g_hf14AcuBold);

	rcSubBtn.top = m_rcClient.top+9;
	DrawText(m_hDC,psz,-1,&rcSubBtn,DT_VCENTER|DT_SINGLELINE|DT_CENTER);

	SelectObject(m_hDC,g_hf7AcuNorm);

	DrawText(m_hDC,m_pszUnitText,-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

	SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);

}
void CSubParaBtn::DrawSUBPARA_ETIME(int nState)
{
	int nTxtColorSubBtn=GetTextColorStateDependend(nState);
	int iTempIErelation = 0;
	double dTempVal=0;

	m_btn.pcBmpUp->Draw(m_hDC);

	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf10AcuBold);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColorSubBtn);

	//Sub-Button
	RECT rcSubBtn;
	rcSubBtn.top = m_rcClient.top+10;
	rcSubBtn.bottom = m_rcClient.bottom-10;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	TCHAR psz[MAX_PATH];

	rcSubBtn.top = m_rcClient.top+5;
	rcSubBtn.bottom = m_rcClient.bottom-5;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	SelectObject(m_hDC,g_hf7AcuBold);

	if(		getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
		||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP)
	{
		if(getModel()->getDATAHANDLER()->GetCurrentTempITime()>600)
		{
			CDC* pDC=CDC::FromHandle(m_hDC);
			CSize TextSize1 = pDC->GetTextExtent(_T("T"));

			//CSize TextSize2 = pDC->GetTextExtent(getModel()->GetLanguageString(IDS_PARA_ITIMEHIGH));
			CSize TextSize2 = pDC->GetTextExtent(_T("low"));

			int iDiff=(m_rcClient.right-TextSize1.cx-TextSize2.cx)/2;
			//rcName.top = m_rcClient.top;
			//rcName.bottom = 30;
			rcSubBtn.left = m_rcClient.left;
			rcSubBtn.right = rcSubBtn.left+iDiff+TextSize1.cx;
			DrawText(m_hDC,_T("T"),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_RIGHT);

			rcSubBtn.top = m_rcClient.top+7;
			rcSubBtn.left = rcSubBtn.right+2;
			rcSubBtn.right = m_rcClient.right;
			//DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_ITIMEHIGH),-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			DrawText(m_hDC,_T("low"),-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			DrawText(m_hDC,m_pszNameText,-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);
		}
	}
	else
	{
		DrawText(m_hDC,m_pszNameText,-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);
	}


	if(m_bTempValue)
	{
		//m_bTempValue=false;
		if(getModel()->getDATAHANDLER()->GetCurrentTempETime()!=0)
			dTempVal = CTlsFloat::Round((double)getModel()->getDATAHANDLER()->GetCurrentTempETime()/1000, 2);

	}
	else
	{
		if(getModel()->getDATAHANDLER()->GetCurrentETimePara()!=0)
			dTempVal = CTlsFloat::Round((double)getModel()->getDATAHANDLER()->GetCurrentETimePara()/1000, 2);

	}

	wsprintf(psz,_T("%0.2f"),dTempVal);

	SelectObject(m_hDC,g_hf14AcuBold);

	rcSubBtn.bottom = m_rcClient.bottom-5;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;
	rcSubBtn.top = m_rcClient.top+9;
	DrawText(m_hDC,psz,-1,&rcSubBtn,DT_VCENTER|DT_SINGLELINE|DT_CENTER);

	SelectObject(m_hDC,g_hf7AcuNorm);

	DrawText(m_hDC,m_pszUnitText,-1,&rcSubBtn,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

	SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);

}
void CSubParaBtn::DrawSUBPARA_LEAKCOMP(int nState)
{
	int nTxtColorSubBtn=GetTextColorStateDependend(nState);
	int iTempIErelation = 0;
	double dTempVal=0;

	m_btn.pcBmpUp->Draw(m_hDC);

	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf10AcuBold);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColorSubBtn);

	//Sub-Button
	RECT rcSubBtn;
	rcSubBtn.top = m_rcClient.top+10;
	rcSubBtn.bottom = m_rcClient.bottom-10;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	TCHAR psz[MAX_PATH];

	SelectObject(m_hDC,g_hf7AcuBold);

	CStringW szText=getModel()->GetLanguageString(IDS_PARA_LEAKCOMP);

	rcSubBtn.top = m_rcClient.top+5;
	rcSubBtn.bottom = m_rcClient.bottom-5;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	DrawText(m_hDC,szText,-1,&rcSubBtn,DT_TOP|DT_SINGLELINE|DT_CENTER);

	eLeakCompensation eLeakCompOff=getModel()->getCONFIG()->getLeakCompOff();
	switch(eLeakCompOff)
	{
	case LC_OFF:
		{
			SelectObject(m_hDC,g_hf14AcuBold);
			wsprintf(psz,getModel()->GetLanguageString(IDS_TXT_OFF));
			if(m_pcWarning_Up)
				m_pcWarning_Up->Draw(m_hDC,65,m_rcClient.bottom-20);
		}
		break;
	case LC_LOW:
		{
			wsprintf(psz,getModel()->GetLanguageString(IDS_TXT_SENS_LOW));
		}
		break;
	case LC_MIDDLE:
		{
			wsprintf(psz,getModel()->GetLanguageString(IDS_TXT_SENS_MID));
		}
		break;
	case LC_HIGH:
		{
			wsprintf(psz,getModel()->GetLanguageString(IDS_TXT_SENS_HIGH));
		}
		break;
	}

	SelectObject(m_hDC,g_hf10AcuBold);

	rcSubBtn.top = m_rcClient.top+9;
	DrawText(m_hDC,psz,-1,&rcSubBtn,DT_VCENTER|DT_SINGLELINE|DT_CENTER);

	SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);

}

void CSubParaBtn::DrawDEFAULT(int nState)
{
	int nTxtColorSubBtn=GetTextColorStateDependend(nState);
	int iTempIErelation = 0;
	double dTempVal=0;

	m_btn.pcBmpUp->Draw(m_hDC);

	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf10AcuBold);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColorSubBtn);

	//Sub-Button
	RECT rcSubBtn;
	rcSubBtn.top = m_rcClient.top+10;
	rcSubBtn.bottom = m_rcClient.bottom-10;
	rcSubBtn.left = m_rcClient.left;
	rcSubBtn.right = m_rcClient.right;

	//TCHAR psz[MAX_PATH];

	SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);

}

int CSubParaBtn::GetTextColorStateDependend(int nState)
{
	switch(nState)
	{
	case BTN_DW_FOCUS:
		return (int)m_btncr.crTxtFocus;
		break;
	case BTN_DW_UP:
		return (int)m_btncr.crTxtUp;
		break;
	case BTN_DW_DW:
		return (int)m_btncr.crSubTxtDown;
		break;
	case BTN_DW_DIS:
	default:
		return (int)m_btncr.crTxtDisabled;
		break;
	}
}
// **************************************************************************
// "Button" is very slow - this code permforms speed up - but
//              use of double click is not possible any more
// **************************************************************************
void CSubParaBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CParaBtn::OnLButtonDblClk(nFlags, point);
}

void CSubParaBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CParaBtn::OnLButtonDown(nFlags, point);
}

void CSubParaBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CParaBtn::OnLButtonUp(nFlags, point);
}
void CSubParaBtn::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CParaBtn::OnKeyDown(nChar,nRepCnt,nFlags);
}

void CSubParaBtn::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CParaBtn::OnKeyUp(nChar,nRepCnt,nFlags);
}

void CSubParaBtn::OnKillFocus(CWnd* pNewWnd)
{
	CParaBtn::OnKillFocus(pNewWnd);
}

void CSubParaBtn::TriggerBreath()
{
	SetTimer(TRIGGERTIMER1,300, NULL);
	m_bTriggeredBreath=true;

	RefreshBtn();
}

void CSubParaBtn::SetTimerRunning(bool state)
{
	m_bCalRunning=state;
	if(state)
	{
		m_iWaitCount=1;
		SetTimer(AUTOOXYCALTIMER, 150, NULL);
	}
	else
	{
		m_iWaitCount=0;
		KillTimer(AUTOOXYCALTIMER);

		RefreshBtn();
	}

	
}

void CSubParaBtn::OnTimer(UINT_PTR nIDEvent)
{


	if(nIDEvent==TRIGGERTIMER1)
	{
		KillTimer(TRIGGERTIMER1);
		m_bTriggeredBreath=false;
		RefreshBtn();
	
	}
	else if(nIDEvent==AUTOOXYCALTIMER)
	{
		if(m_iWaitCount>0)
		{
			m_iWaitCount++;

		}
		RefreshBtn();
	}
}

// **************************************************************************
// Button states
// **************************************************************************
BOOL CSubParaBtn::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
		{
			return 1;
		}
		break;
	default:
		{

		}
		break;
	}
	return CButton::PreTranslateMessage(pMsg);
}