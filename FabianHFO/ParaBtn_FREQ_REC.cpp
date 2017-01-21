// ParaBtn_FREQ_REC.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ParaBtn_FREQ_REC.h"
#include "globDefs.h"

extern HFONT g_hf7AcuNorm;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf23AcuBold;

// CParaBtn_FREQ_REC

IMPLEMENT_DYNAMIC(CParaBtn_FREQ_REC, CWnd)

CParaBtn_FREQ_REC::CParaBtn_FREQ_REC(BTN btn, int nOffset,bool bScrollOver):
CParaBtn(btn,nOffset,bScrollOver)
{

}

CParaBtn_FREQ_REC::~CParaBtn_FREQ_REC()
{
}


BEGIN_MESSAGE_MAP(CParaBtn_FREQ_REC, CWnd)
	//{{AFX_MSG_MAP(CParaBtn_FREQ_REC)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	/*ON_WM_SETFOCUS()*/
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CParaBtn_FREQ_REC message handlers
// **************************************************************************
// 
// **************************************************************************
void CParaBtn_FREQ_REC::Draw(int nState)
{
	if(!m_bDisableText)
	{
		//if(g_hf31AcuBold )
		{
			CDC* pDC=CDC::FromHandle(m_hDC);

			HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf31AcuBoldNum);
			int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
			int nTxtColor;
			//int nTxtColorSubBtn;

			CBmp* pcArrow=NULL;

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

			/*if(m_v.iValue!=0)
				wsprintf(psz,_T("%0.0f"), CTlsFloat::Round(((double)m_v.iValue), 0));
			else
				wsprintf(psz,_T("%d"), 0);*/

			if(m_v.iValue==0)
			{
				SelectObject(m_hDC,g_hf23AcuBold);
				wsprintf(psz,_T("%s"),getModel()->GetLanguageString(IDS_TXT_OFF));
				//DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_OFF),-1,&rc,m_btn.dwFormat);
			}
			else
			{
				if(m_v.iValue<60)
				{
					wsprintf(psz,_T("%d"),m_v.iValue);
					//DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);
				}
				else if(m_v.iValue==60)
				{
					wsprintf(psz,_T("%d"),1);
					//DrawText(m_hDC,_T("1"),-1,&rc,m_btn.dwFormat);

				}
				else if(m_v.iValue==120)
				{
					wsprintf(psz,_T("%d"),2);
					//DrawText(m_hDC,_T("2"),-1,&rc,m_btn.dwFormat);

				}
				else if(m_v.iValue==180)
				{
					wsprintf(psz,_T("%d"),3);
					//DrawText(m_hDC,_T("3"),-1,&rc,m_btn.dwFormat);

				}
				else if(m_v.iValue==240)
				{
					wsprintf(psz,_T("%d"),4);
					//DrawText(m_hDC,_T("4"),-1,&rc,m_btn.dwFormat);

				}
				else
				{
					wsprintf(psz,_T("%d"),555);
					
				}
				
			}

			//test
			//int iT = m_btn.wID;
			DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);

			//RECT rcName;
			CRect rcName;


			SelectObject(m_hDC,g_hf14AcuBold);

			CSize TextSize1 = pDC->GetTextExtent(m_pszNameText);

			SelectObject(m_hDC,g_hf8AcuBold);

			CSize TextSize2 = pDC->GetTextExtent(m_pszNameNoteText);

			SelectObject(m_hDC,g_hf14AcuBold);

			int iDiff=(m_rcClient.right-TextSize1.cx-TextSize2.cx)/2;

			rcName.top = m_rcClient.top;
			rcName.bottom = 30;
			rcName.left = m_rcClient.left;
			rcName.right = rcName.left+iDiff+TextSize1.cx;

			DrawText(m_hDC,m_pszNameText,-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_RIGHT);

			SelectObject(m_hDC,g_hf8AcuBold);
			rcName.bottom = 35;
			rcName.left = rcName.right;
			rcName.right = m_rcClient.right;
			DrawText(m_hDC,m_pszNameNoteText,-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

			RECT rcUnit;
			rcUnit.top = m_rcClient.top;
			rcUnit.bottom = m_rcClient.bottom-5;
			rcUnit.left = m_rcClient.left;
			rcUnit.right = m_rcClient.right;

			SelectObject(m_hDC,g_hf7AcuNorm);
			//SetTextColor(m_hDC,RGB(0,0,0));

			if(m_bEndOfRange)
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_ENDRANGE),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
			else if(m_bLimited)
			{
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_LIMITTED)+_T(": ")+getModel()->GetLanguageString(IDS_PARA_ITIME),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
			}
			else
			{
				if(m_v.iValue>=60)
				{
					//m_pcPara_FREQ_REC->SetUnitText(_T("1/")+getModel()->GetLanguageString(IDS_UNIT_MIN));
					DrawText(m_hDC,_T("1/")+getModel()->GetLanguageString(IDS_UNIT_MIN),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
				}
				else
				{
					//m_pcPara_FREQ_REC->SetUnitText(_T("1/")+getModel()->GetLanguageString(IDS_UNIT_HRS));
					DrawText(m_hDC,_T("1/")+getModel()->GetLanguageString(IDS_UNIT_HRS),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
				}

				//DrawText(m_hDC,m_pszUnitText,-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

				if(m_bDrawWarning && m_bHasFocus)
				{
					if(m_pcWarning_Fc)
						m_pcWarning_Fc->Draw(m_hDC,60,65);
				}
				else if(m_bDrawWarning && m_bDepressed)
				{
					if(m_pcWarning_Dw)
						m_pcWarning_Dw->Draw(m_hDC,60,65);
				}
				else if(m_bDrawWarning)
				{
					if(m_pcWarning_Up)
						m_pcWarning_Up->Draw(m_hDC,60,65);
				}

				if(m_bDrawKey && m_bDepressed)
				{
					//if(m_bDepressed)
					{
						if(m_pcKey)
							m_pcKey->Draw(m_hDC,10,65);
					}
				}
				/*else if(m_bDrawArrowDown && m_bHasFocus)
				{
				pcArrow=m_pcArrowDw_FC;

				if(pcArrow)
				pcArrow->Draw(m_hDC,10,65);
				}
				else if(m_bDrawArrowDown && m_bDepressed)
				{
				pcArrow=m_pcArrowDw_DW;

				if(pcArrow)
				pcArrow->Draw(m_hDC,10,65);
				}*/
				else if(m_bDrawArrowDown  && !m_bHasFocus && !m_bDepressed)
				{
					pcArrow=m_pcArrowDw_UP;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
				else if(m_bDrawArrowUp && m_bHasFocus)
				{
					pcArrow=m_pcArrowUp_FC;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
				else if(m_bDrawArrowUp && m_bDepressed)
				{
					pcArrow=m_pcArrowUp_DW;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
				else if(m_bDrawArrowUp  && !m_bHasFocus && !m_bDepressed)
				{
					pcArrow=m_pcArrowUp_UP;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
			}

			m_bEndOfRange=false;
			m_bLimited=false;

		

			SetTextColor(m_hDC,nPrevTxtColor);
			SetBkMode(m_hDC,nBkMode);
			SelectObject(m_hDC,hPrevFont);

		}
	}
}


void CParaBtn_FREQ_REC::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//CParaBtn::OnKeyUp(nChar,nRepCnt,nFlags);
	//bool bLimitsOk=true;

	if( m_bLMouseButtonDown )
	{

		if(nChar==m_kDown)	
		{
			if(m_bDepressed)
			{

				if(m_v.iValue>m_v.iLowerLimit)
				{
					//m_v.iValue=m_v.iValue-1;

					if(m_v.iValue<=60)
					{
						m_v.iValue--;
					}
					else if(m_v.iValue<=120)
					{
						m_v.iValue=60;
					}
					else if(m_v.iValue<=180)
					{
						m_v.iValue=120;
					}
					else if(m_v.iValue<=240)
					{
						m_v.iValue=180;
					}
					

					if(GetParent())
						GetParent()->PostMessage(WM_BPMREC_CHANGED);

				}
				else if(m_bScrollOver)
					m_v.iValue=m_v.iUpperLimit;
				else
					m_bEndOfRange = true;

				CClientDC dc(this);
				m_btn.pcBmpDown->Draw(m_hDC);
				Draw(BTN_DW_DW);
				BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);

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
				{
					if(m_v.iValue<59)
					{
						int iTemp=m_v.iValue+1;
						if(getModel()->getDATAHANDLER()->CalculateHF_ETIME_REC(iTemp,getModel()->getDATAHANDLER()->GetCurrentITimeRecPara())==0)
						{
							/*if(GetParent() && !m_bLimited)
								GetParent()->PostMessage(WM_FREQREC_LIMITED);*/
							m_bLimited=true;

						}
						else
						{
							m_v.iValue++;
							if(GetParent())
								GetParent()->PostMessage(WM_BPMREC_CHANGED);
						}
						
					}
					else if(m_v.iValue<60)
					{
						if(getModel()->getDATAHANDLER()->CalculateHF_ETIME_REC(60,getModel()->getDATAHANDLER()->GetCurrentITimeRecPara())==0)
						{
							/*if(GetParent() && !m_bLimited)
								GetParent()->PostMessage(WM_FREQREC_LIMITED);*/
							m_bLimited=true;
						}
						else
						{
							m_v.iValue=60;
							if(GetParent())
								GetParent()->PostMessage(WM_BPMREC_CHANGED);
						}
					}
					else if(m_v.iValue<120)
					{
						if(getModel()->getDATAHANDLER()->CalculateHF_ETIME_REC(120,getModel()->getDATAHANDLER()->GetCurrentITimeRecPara())==0)
						{
							/*if(GetParent() && !m_bLimited)
								GetParent()->PostMessage(WM_FREQREC_LIMITED);*/
							m_bLimited=true;
						}
						else
						{
							m_v.iValue=120;
							if(GetParent())
								GetParent()->PostMessage(WM_BPMREC_CHANGED);
						}
					}
					else if(m_v.iValue<180)
					{
						if(getModel()->getDATAHANDLER()->CalculateHF_ETIME_REC(180,getModel()->getDATAHANDLER()->GetCurrentITimeRecPara())==0)
						{
							/*if(GetParent() && !m_bLimited)
								GetParent()->PostMessage(WM_FREQREC_LIMITED);*/
							m_bLimited=true;
						}
						else
						{
							m_v.iValue=180;
							if(GetParent())
								GetParent()->PostMessage(WM_BPMREC_CHANGED);
						}
					}
					else if(m_v.iValue<240)
					{
						if(getModel()->getDATAHANDLER()->CalculateHF_ETIME_REC(240,getModel()->getDATAHANDLER()->GetCurrentITimeRecPara())==0)
						{
							/*if(GetParent() && !m_bLimited)
								GetParent()->PostMessage(WM_FREQREC_LIMITED);*/
							m_bLimited=true;
						}
						else
						{
							m_v.iValue=240;
							if(GetParent())
								GetParent()->PostMessage(WM_BPMREC_CHANGED);
						}
						
					}

					/*if(m_v.iValue>getModel()->getDATAHANDLER()->HFO()->GetHFfreqMaxKey())
					{
						if(m_bKeyValueAccepted==false)
						{
							m_bDrawKey=true;
							GetParent()->PostMessage(WM_HFFREQ_SETKEY);
						}
						m_bDrawWarning=true;
					}*/
				}
				else if(m_bScrollOver)
					m_v.iValue=m_v.iLowerLimit;
				else
					m_bEndOfRange = true;

				CClientDC dc(this);
				m_btn.pcBmpDown->Draw(m_hDC);
				Draw(BTN_DW_DW);
				BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
			}
			else
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_FOCUS_VKUP);
			}
		}
		else
		{
			if(GetParent())
				GetParent()->PostMessage(WM_BPMREC_CHANGED);
		}

		CParaBtn::OnKeyUp(nChar,nRepCnt,nFlags);
	}
	m_bLMouseButtonDown = false;
}


void CParaBtn_FREQ_REC::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bLMouseButtonDown )
	{
		if(ChildWindowFromPoint(point)==FromHandle(this->m_hWnd))
		{
			if(m_bDepressed)
			{
				if(GetParent())
					GetParent()->PostMessage(WM_BPMREC_CHANGED);
			}
		}


	}
	CParaBtn::OnLButtonUp(nFlags, point);
}