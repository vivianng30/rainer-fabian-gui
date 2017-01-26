#include "StdAfx.h"
#include "ParaBtn_TRIGGER.h"
#include "globDefs.h"

extern HFONT g_hf7AcuNorm;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf8AcuBold;

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTSUBBTNDW			0x00000000
#define COLOR_TXTBTNFC				0x00000000

CParaBtn_TRIGGER::CParaBtn_TRIGGER(BTN btn, int nOffset,bool bScrollOver):
CParaBtn(btn,nOffset,bScrollOver)
{
	
}

CParaBtn_TRIGGER::~CParaBtn_TRIGGER(void)
{
	KillTimer(TRIGGERTIMER2);	
}

BEGIN_MESSAGE_MAP(CParaBtn_TRIGGER, CButton)
	//{{AFX_MSG_MAP(CParaBtn_TRIGGER)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// **************************************************************************
// 
// **************************************************************************
void CParaBtn_TRIGGER::Draw(int nState)
{
	if(!m_bDisableText)
	{
		//if(g_hf31AcuBold )
		{
			HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf31AcuBoldNum);
			int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
			int nTxtColor;
			//int nTxtColorSubBtn;

			switch(nState)
			{
			case BTN_DW_FOCUS:
				nTxtColor=m_btncr.crTxtFocus;
				//nTxtColorSubBtn=m_btncr.crTxtFocus;
				break;
			case BTN_DW_UP:
				nTxtColor=m_btncr.crTxtUp;
				//nTxtColorSubBtn=m_btncr.crTxtUp;
				break;
			case BTN_DW_DW:
				nTxtColor=m_btncr.crTxtDown;
				//nTxtColorSubBtn=m_btncr.crSubTxtDown;
				break;
			case BTN_DW_DIS:
			default:
				nTxtColor=m_btncr.crTxtDisabled;
				//nTxtColorSubBtn=m_btncr.crTxtDisabled;
				break;
			}
			int nPrevTxtColor=SetTextColor(m_hDC,nTxtColor);

			RECT rc;
			memcpy(&rc,&m_rcClient,sizeof(RECT));

			rc.top = 5;

			if(m_btn.dwFormat&DT_RIGHT)
				rc.right-=m_nOffset;
			else if(!(m_btn.dwFormat&DT_CENTER)) // left
				rc.left+=m_nOffset;

			TCHAR psz[MAX_PATH];

			if(m_v.iValue!=0)
				wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)m_v.iValue)/10, 0));
			else
				wsprintf(psz,_T("%df"),0);

			DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);

			CRect rcName;
			rcName.top = m_rcClient.top;
			rcName.bottom = 30;
			rcName.left = m_rcClient.left;
			rcName.right = m_rcClient.right;

			SelectObject(m_hDC,g_hf14AcuBold);

			if(getModel()->getCONFIG()->GetCurMode()==VM_CPAP
				||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_CPAP)
			{
				SelectObject(m_hDC,g_hf9AcuBold);
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_SENSITIVITY),-1,&rcName,m_btn.dwFormat);

				RECT rcUnit;
				rcUnit.top = m_rcClient.top;
				rcUnit.left = m_rcClient.left;
				rcUnit.right = m_rcClient.right;

				SelectObject(m_hDC,g_hf7AcuNorm);
				rcUnit.bottom = m_rcClient.bottom-5;

				if(m_bEndOfRange)
				{
					DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_ENDRANGE),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
				}
				else
				{
					if(m_v.iValue<=40)
						DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_SENS_HIGH),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
					else if(m_v.iValue<=70)
						DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_SENS_MID),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
					else
						DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_SENS_LOW),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
				}
			}
			else
			{
				DrawText(m_hDC,m_pszNameText,-1,&rcName,m_btn.dwFormat);

				RECT rcUnit;
				rcUnit.top = m_rcClient.top;
				rcUnit.left = m_rcClient.left;
				rcUnit.right = m_rcClient.right;

				SelectObject(m_hDC,g_hf7AcuNorm);

				if(m_bEndOfRange)
				{
					rcUnit.bottom = m_rcClient.bottom-5;
					DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_ENDRANGE),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
				}
				else
				{
					rcUnit.bottom = m_rcClient.bottom-13;
					DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_SENSITIVITY),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

					rcUnit.bottom = m_rcClient.bottom-1;
					if(m_v.iValue<=40)
						DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_SENS_HIGH),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
					else if(m_v.iValue<=70)
						DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_SENS_MID),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
					else
						DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_SENS_LOW),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

				}
			}
			
			m_bEndOfRange=false;

			SetTextColor(m_hDC,nPrevTxtColor);
			SetBkMode(m_hDC,nBkMode);
			SelectObject(m_hDC,hPrevFont);
		}
	}
}



void CParaBtn_TRIGGER::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( m_bLMouseButtonDown )
	{

		if(nChar==m_kDown)	
		{
			if(m_bDepressed)
			{
				
				if(m_v.iValue>m_v.iLowerLimit)
					m_v.iValue=m_v.iValue-10;
				else if(m_bScrollOver)
					m_v.iValue=m_v.iUpperLimit;
				else
					m_bEndOfRange = true;

				if(m_bTextOnly)// notify parent by message
				{
					//GetParent()->PostMessage(WM_UPDATE_UDBTN,m_v.nValue,m_btn.wID);
				}
				else
				{
					CClientDC dc(this);
					m_btn.pcBmpDown->Draw(m_hDC);
					Draw(BTN_DW_DW);
					BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
				}


			}
			else
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_FOCUS_VKDOWN);
			}
		}
		else if(nChar==m_kUp)	
		{
			if(m_bDepressed)
			{
				if(m_v.iValue<m_v.iUpperLimit)
					m_v.iValue=m_v.iValue+10;
				else if(m_bScrollOver)
					m_v.iValue=m_v.iLowerLimit;
				else
					m_bEndOfRange = true;

				if(m_bTextOnly)// notify parent by message
				{
					//GetParent()->PostMessage(WM_UPDATE_UDBTN,m_v.nValue,m_btn.wID);
				}
				else
				{
					CClientDC dc(this);
					m_btn.pcBmpDown->Draw(m_hDC);
					Draw(BTN_DW_DW);
					BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
				}

				//WriteCurrentValue();
			}
			else
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_FOCUS_VKUP);
			}
		}
		CParaBtn::OnKeyUp(nChar,nRepCnt,nFlags);
	}
	m_bLMouseButtonDown = false;
}

void CParaBtn_TRIGGER::Trigger()
{

	if(!IsDepressed())
	{
		SetColors(RGB(0,191,0),RGB(0,191,0),COLOR_TXTSUBBTNDW,RGB(0,191,0));
		Invalidate();
		UpdateWindow();
		SetTimer(TRIGGERTIMER2,400, NULL);
	}
	
}

void CParaBtn_TRIGGER::OnTimer(UINT_PTR nIDEvent)
{


	if(nIDEvent==TRIGGERTIMER2)
	{
		KillTimer(TRIGGERTIMER2);
		SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
		//RefreshBtn();
		Invalidate();
		UpdateWindow();
	}
}
