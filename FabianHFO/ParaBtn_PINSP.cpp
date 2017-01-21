#include "StdAfx.h"
#include "ParaBtn_PINSP.h"
#include "globDefs.h"
#include "MVViewHandler.h"

extern HFONT g_hf7AcuNorm;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf8AcuBold;


CParaBtn_PINSP::CParaBtn_PINSP(BTN btn, int nOffset,bool bScrollOver):
CParaBtn(btn,nOffset,bScrollOver)
{
}

CParaBtn_PINSP::~CParaBtn_PINSP(void)
{
	
}

BEGIN_MESSAGE_MAP(CParaBtn_PINSP, CButton)
	//{{AFX_MSG_MAP(CParaBtn_PINSP)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	/*ON_WM_SETFOCUS()*/
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CParaBtn_PINSP::SetValue(fVALUE v, bool bRedraw) 
{
	memcpy(&m_v,&v,sizeof(fVALUE));

	if(bRedraw)
	{
		//Invalidate();
		//UpdateWindow();
		RefreshBtn();
	}
}

// **************************************************************************
// 
// **************************************************************************
void CParaBtn_PINSP::Draw(int nState)
{
	if(!m_bDisableText)
	{
		//if(g_hf31AcuBold )
		{
			CDC* pDC=CDC::FromHandle(m_hDC);
			HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf31AcuBoldNum);
			int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
			int nTxtColor;
			COLORREF	crTxtInactive=0x00828282;

			CBmp* pcArrow=NULL;

			switch(nState)
			{
			case BTN_DW_FOCUS:
				if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn())
					nTxtColor=crTxtInactive;
				else
					nTxtColor=m_btncr.crTxtUp;
				break;
			case BTN_DW_UP:
				if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn())
					nTxtColor=crTxtInactive;
				else
					nTxtColor=m_btncr.crTxtUp;
				break;
			case BTN_DW_DW:
				nTxtColor=m_btncr.crTxtDown;
				break;
			case BTN_DW_DIS:
			default:
				nTxtColor=m_btncr.crTxtDisabled;
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
				wsprintf(psz,_T("%0.0f"), CTlsFloat::Round(((double)m_v.iValue)/10, 0));
			else
				wsprintf(psz,_T("%d"), 0);

			DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);
			
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

			if(m_bEndOfRange)
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_ENDRANGE),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
			else if(m_bLimited)
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_LIMITTED),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
			else
			{
				DrawText(m_hDC,m_pszUnitText,-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

				if(m_bDrawWarning && m_bHasFocus)
				{
					if(m_pcWarning_Fc)
						m_pcWarning_Fc->Draw(m_hDC,65,65);
				}
				else if(m_bDrawWarning && m_bDepressed)
				{
					if(m_pcWarning_Dw)
						m_pcWarning_Dw->Draw(m_hDC,65,65);
				}
				else if(m_bDrawWarning && m_bSignaled)
				{
					if(m_pcWarning_Red)
						m_pcWarning_Red->Draw(m_hDC,65,65);
				}
				else if(m_bDrawWarning)
				{
					if(m_pcWarning_Up)
						m_pcWarning_Up->Draw(m_hDC,65,65);
				}

				if(m_bDrawKey && m_bDepressed)
				{
					if(m_pcKey)
						m_pcKey->Draw(m_hDC,10,65);
				}
				else if(m_bDrawAlarmArrowDown && m_bHasFocus)
				{
					pcArrow=m_pcAlarmArrowDw_FC;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
				else if(m_bDrawAlarmArrowDown && m_bDepressed)
				{
					pcArrow=m_pcAlarmArrowDw_DW;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
				else if(m_bDrawAlarmArrowDown  && !m_bHasFocus && !m_bDepressed)
				{
					if(m_bSignaled)
						pcArrow=m_pcArrowDw_RED;
					else
						pcArrow=m_pcAlarmArrowDw_UP;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
				else if(m_bDrawAlarmArrowUp && m_bHasFocus)
				{
					pcArrow=m_pcAlarmArrowUp_FC;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
				else if(m_bDrawAlarmArrowUp && m_bDepressed)
				{
					pcArrow=m_pcAlarmArrowUp_DW;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
				else if(m_bDrawAlarmArrowUp  && !m_bHasFocus && !m_bDepressed /*&& !m_bAutoState*/)
				{
					if(m_bSignaled)
						pcArrow=m_pcArrowUp_RED;
					else
						pcArrow=m_pcAlarmArrowUp_UP;

					if(pcArrow)
						pcArrow->Draw(m_hDC,10,65);
				}
				else if(m_bDrawArrowDown && m_bHasFocus)
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
				}
				else if(m_bDrawArrowDown  && !m_bHasFocus && !m_bDepressed /*&& !m_bAutoState*/)
				{
					if(m_bSignaled)
						pcArrow=m_pcArrowDw_RED;
					else
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
				else if(m_bDrawArrowUp  && !m_bHasFocus && !m_bDepressed /*&& !m_bAutoState*/)
				{
					if(m_bSignaled)
						pcArrow=m_pcArrowUp_RED;
					else
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




void CParaBtn_PINSP::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( m_bLMouseButtonDown )
	{
		if(nChar==m_kDown)	
		{
			if(m_bDepressed)
			{
				if(m_v.iValue>m_v.iLowerLimit)
				{
					int dTemp = m_v.iValue;
					dTemp=dTemp-10;
					int fPEEP= 0;
					int fPPSV= 0;

					if(	getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					{
						if(getModel()->getCONFIG()->GetCurMode()==VM_IPPV)
						{
							fPEEP= getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_IPPV();
						}
						else
						{
							fPEEP= getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_TRIGGER();
						}
						
						fPPSV= getModel()->getDATAHANDLER()->PARADATA()->GetPpsvPara();
					}
					else
					{
						if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_IPPV)
						{
							fPEEP= getModel()->getDATAHANDLER()->PRESET()->GetPEEPPara_IPPV();
						}
						else
						{
							fPEEP= getModel()->getDATAHANDLER()->PRESET()->GetPEEPPara_TRIGGER();
						}
						
						fPPSV= getModel()->getDATAHANDLER()->PRESET()->GetPpsvPara();
					}

				
					if(		getModel()->getCONFIG()->GetCurMode()==VM_PSV
						||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_PSV
						||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
						||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV)//newVG
					{
						if(dTemp<fPPSV)
						{
							m_bLimited=true;
							
							if(dTemp<fPEEP+DIFF_PEEP_PINSP)
							{
								if(GetParent())
									GetParent()->PostMessage(WM_PINSP_LIMITED);
							}
							else
							{
								if(GetParent())
									GetParent()->PostMessage(WM_PINSP_LIMITEDPSV);
							}
						}
						else if(dTemp<fPEEP+DIFF_PEEP_PINSP)
						{
							m_bLimited=true;
							if(GetParent())
								GetParent()->PostMessage(WM_PINSP_LIMITED);
						}
						else
						{
							m_v.iValue=dTemp;

							if(dTemp<=getModel()->getDATAHANDLER()->GetCurrentPInspMaxKey())
							{
								m_bKeyValueAccepted=false;
								/*if(m_bDrawKey)
								{
									if(GetParent())
										GetParent()->PostMessage(WM_PINSP_CLEARKEY);
								}*/
								m_bDrawKey=false;
								m_bDrawWarning=false;
							}
						}
					}
					else
					{
						if(dTemp<fPEEP+DIFF_PEEP_PINSP)
						{
							m_bLimited=true;
							if(GetParent())
								GetParent()->PostMessage(WM_PINSP_LIMITED);
						}
						else
						{
							m_v.iValue=dTemp;

							if(dTemp<=getModel()->getDATAHANDLER()->GetCurrentPInspMaxKey())
							{
								m_bKeyValueAccepted=false;
								/*if(m_bDrawKey)
								{
									if(GetParent())
										GetParent()->PostMessage(WM_PINSP_CLEARKEY);
								}*/
								m_bDrawKey=false;
								m_bDrawWarning=false;
							}
						}
					}
					m_bWaitConfirm=false;
					m_bKeyBeep=TRUE;

					if(GetParent())
						GetParent()->PostMessage(WM_PINSP_CLEAR_PMAXI);//newVG
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
					int iTemp=m_v.iValue+10;

					//int iPmax=getModel()->getDATAHANDLER()->GetCurrentPmaxVolGPara();//newVG

					//if(iTemp>iPmax)//newVG
					//{
					//	m_bLimited=true;
					//	if(GetParent())
					//		GetParent()->PostMessage(WM_PINSP_LIMITED_PMAX);
					//}
					//else
					{
						if(iTemp>getModel()->getDATAHANDLER()->GetCurrentPInspMaxKey())
						{
							if(m_bKeyValueAccepted==false)
							{
								m_bWaitConfirm=true;
								m_bDrawKey=true;
								if(GetParent())
									GetParent()->PostMessage(WM_PINSP_SETKEY, m_bKeyBeep);
								m_bKeyBeep=FALSE;
							}
							else
							{
								m_v.iValue=iTemp;
								if(GetParent())
									GetParent()->PostMessage(WM_PINSP_CHANGED);
							}
							m_bDrawWarning=true;
						}
						else
						{
							m_v.iValue=iTemp;
							if(GetParent())
								GetParent()->PostMessage(WM_PINSP_CHANGED);
						}
					}
					//if(GetParent())
					//	GetParent()->PostMessage(WM_PINSP_CLEAR_PPSV);  //newVG
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
				GetParent()->PostMessage(WM_PINSP_CHANGED);
		}
		CParaBtn::OnKeyUp(nChar,nRepCnt,nFlags);
	}
	m_bLMouseButtonDown = false;
}

void CParaBtn_PINSP::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bLMouseButtonDown )
	{
		if(ChildWindowFromPoint(point)==FromHandle(this->m_hWnd))
		{
			if(m_bDepressed)
			{
				if(GetParent())
					GetParent()->PostMessage(WM_PINSP_CHANGED);
			}
		}
	}
	CParaBtn::OnLButtonUp(nFlags, point);
}

