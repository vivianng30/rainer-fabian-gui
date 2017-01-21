#include "StdAfx.h"
#include "ParaBtn_PEEP.h"
#include "globDefs.h"
#include "ViewParaBtn.h"
//#include "MVViewHandler.h"

extern HFONT g_hf7AcuNorm;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf8AcuBold;

CViewParaBtn* CParaBtn_PEEP::VIEWPARAPTN=0;

CParaBtn_PEEP::CParaBtn_PEEP(CViewParaBtn* parent, BTN btn, int nOffset,bool bScrollOver):
CParaBtn(btn,nOffset,bScrollOver)
{
	VIEWPARAPTN=parent;
}

CParaBtn_PEEP::~CParaBtn_PEEP(void)
{
	
}

BEGIN_MESSAGE_MAP(CParaBtn_PEEP, CButton)
	//{{AFX_MSG_MAP(CParaBtn_PEEP)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	/*ON_WM_SETFOCUS()*/
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




void CParaBtn_PEEP::Draw(int nState)
{
	if(!m_bDisableText)
	{
		//if(g_hf31AcuBold )
		{
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

			if(m_v.iValue!=0)
			{
				wsprintf(psz,_T("%0.1f"), (CTlsFloat::Round( (double)m_v.iValue/5, 0 )*5)/10);
			}
			else
				wsprintf(psz,_T("%d"), 0);

			DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);

			//RECT rcName;
			CRect rcName;


			rcName.top = m_rcClient.top;
			rcName.bottom = 30;
			rcName.left = m_rcClient.left;
			rcName.right = m_rcClient.right;

			SelectObject(m_hDC,g_hf14AcuBold);

			DrawText(m_hDC,m_pszNameText,-1,&rcName,m_btn.dwFormat);

			
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
				else if(m_bDrawWarning)
				{
					if(m_pcWarning_Up)
						m_pcWarning_Up->Draw(m_hDC,65,65);
				}

				if(m_bDrawKey && m_bDepressed)
				{
					//if(m_bDepressed)
					{
						if(m_pcKey)
							m_pcKey->Draw(m_hDC,10,65);
					}
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






void CParaBtn_PEEP::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
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
					int iTemp=m_v.iValue-5;

					if(		getModel()->getCONFIG()->GetCurMode()==VM_PSV
						||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_PSV
						||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
						||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV)
					{
						if(		iTemp<=getModel()->getDATAHANDLER()->GetCurrentPeepMaxKey()
							&&	iTemp>=getModel()->getDATAHANDLER()->GetCurrentPeepMinKey())
						{
							m_bKeyValueAccepted=false;
							/*if(m_bDrawKey)
							{
								if(GetParent())
									GetParent()->PostMessage(WM_PEEP_CLEARKEY);
							}*/
							m_bDrawKey=false;
							m_bDrawWarning=false;

							m_bWaitConfirm=false;
							m_bKeyBeep=TRUE;

							m_v.iValue=iTemp;
							
						}
						else if(iTemp<getModel()->getDATAHANDLER()->GetCurrentPeepMinKey())
						{
							if(m_bKeyValueAccepted==false)
							{
								m_bDrawKey=true;
								m_bWaitConfirm=true;
								if(GetParent())
									GetParent()->PostMessage(WM_PEEP_SETKEY_LOW,m_bKeyBeep);
								m_bKeyBeep=FALSE;
							}
							else
							{
								m_v.iValue=iTemp;
							}
							m_bDrawWarning=true;
						}
						else
						{
							m_bWaitConfirm=false;
							m_bKeyBeep=TRUE;

							m_v.iValue=iTemp;
						}

						VIEWPARAPTN->setPEEPvalue(m_v.iValue,false);
						
						if(GetParent())
							GetParent()->PostMessage(WM_PEEP_CHANGED,m_v.iValue);

					}
					else
					{

						if(iTemp<getModel()->getDATAHANDLER()->GetCurrentPeepMinKey())
						{
							if(m_bKeyValueAccepted==false)
							{
								m_bDrawKey=true;
								m_bWaitConfirm=true;
								if(GetParent())
									GetParent()->PostMessage(WM_PEEP_SETKEY_LOW,m_bKeyBeep);
								m_bKeyBeep=FALSE;
							}
							else
							{
								m_v.iValue=iTemp;
							}
							m_bDrawWarning=true;
						}
						else
						{
							if(iTemp<=getModel()->getDATAHANDLER()->GetCurrentPeepMaxKey())
							{
								m_bKeyValueAccepted=false;
								/*if(m_bDrawKey)
								{
									if(GetParent())
										GetParent()->PostMessage(WM_PEEP_CLEARKEY);
								}*/
								m_bDrawKey=false;
								m_bDrawWarning=false;
							}

							m_bWaitConfirm=false;
							m_bKeyBeep=TRUE;

							m_v.iValue=iTemp;
						}
						
						if(GetParent())
							GetParent()->PostMessage(WM_PEEP_CHANGED,m_v.iValue);
					}
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
					int dTemp = m_v.iValue+5;
					int fPInsp=(int)getModel()->getDATAHANDLER()->GetCurrentPINSPPara();
					int fPmax=(int)getModel()->getDATAHANDLER()->GetCurrentPmaxVolGPara();

					if(		getModel()->getCONFIG()->GetCurMode()==VM_PSV
						||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_PSV
						||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
						||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV)//newVG
					{
						int iPEEP_PPSVdiff=getModel()->getDATAHANDLER()->getPEEP_PpsvDifference();

						if(dTemp>fPInsp-DIFF_PEEP_PINSP)
						{
							m_bLimited=true;
							if(GetParent())
								GetParent()->PostMessage(WM_PEEP_LIMITED);
						}
						else if(dTemp>fPmax-DIFF_PEEP_PINSP && getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT
							&& getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON) //rku, PMAX check
						{
							m_bLimited=true;
							if(GetParent())
								GetParent()->PostMessage(WM_PEEP_LIMITED_PMAXI);
						}
						else if(TRUE==getModel()->getCONFIG()->isPpsvAsDeltaPEEPValue() && 
							dTemp+iPEEP_PPSVdiff>fPInsp)
						{
							m_bLimited=true;
							if(GetParent())
								GetParent()->PostMessage(WM_PEEP_LIMITEDPSV);
						}
						else
						{
							int fPPSV=VIEWPARAPTN->getPpsvDisplayedValue();

							if(FALSE==getModel()->getCONFIG()->isPpsvAsDeltaPEEPValue() 
								&& fPPSV==getModel()->getDATAHANDLER()->GetCurrentPpsvMaxKey()
								&& m_bKeyValueAccepted==false
								&& fPPSV!=fPInsp)//rku, VGVL check
							{
								m_bDrawKey=true;
								m_bWaitConfirm=true;
								if(GetParent())
									GetParent()->PostMessage(WM_PEEP_SETKEY_HIGH,m_bKeyBeep);
								m_bKeyBeep=FALSE;
							}
							else if(dTemp>getModel()->getDATAHANDLER()->GetCurrentPeepMaxKey())
							{
								if(m_bKeyValueAccepted==false)
								{
									m_bDrawKey=true;
									m_bWaitConfirm=true;
									if(GetParent())
										GetParent()->PostMessage(WM_PEEP_SETKEY_HIGH,m_bKeyBeep);
									m_bKeyBeep=FALSE;
								}
								else
								{
									m_v.iValue=dTemp;
								}
								VIEWPARAPTN->setPEEPvalue(m_v.iValue,false);
								m_bDrawWarning=true;
							}
							else
							{
								if(dTemp>=getModel()->getDATAHANDLER()->GetCurrentPeepMinKey())
								{
									m_bKeyValueAccepted=false;
									/*if(m_bDrawKey)
									{
										if(GetParent())
											GetParent()->PostMessage(WM_PEEP_CLEARKEY);
									}*/
									m_bDrawKey=false;
									m_bDrawWarning=false;
								}

								m_bWaitConfirm=false;
								m_bKeyBeep=TRUE;

								m_v.iValue=dTemp;
								VIEWPARAPTN->setPEEPvalue(m_v.iValue,false);
							}
						}
					}
					else
					{
						if(dTemp>fPInsp-DIFF_PEEP_PINSP)
						{
							m_bLimited=true;
							if(GetParent())
								GetParent()->PostMessage(WM_PEEP_LIMITED);

						}
						else if(dTemp>fPmax-DIFF_PEEP_PINSP && getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT
							&& getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON) //rku, PMAX check
						{
							m_bLimited=true;
							if(GetParent())
								GetParent()->PostMessage(WM_PEEP_LIMITED_PMAXI);
						}
						else
						{
							//m_v.iValue=dTemp;

							if(dTemp>getModel()->getDATAHANDLER()->GetCurrentPeepMaxKey())
							{
								if(m_bKeyValueAccepted==false)
								{
									m_bDrawKey=true;
									m_bWaitConfirm=true;
									if(GetParent())
										GetParent()->PostMessage(WM_PEEP_SETKEY_HIGH,m_bKeyBeep);
									m_bKeyBeep=FALSE;
								}
								else
								{
									m_v.iValue=dTemp;
								}
								m_bDrawWarning=true;
							}
							else
							{
								if(dTemp>=getModel()->getDATAHANDLER()->GetCurrentPeepMinKey())
								{
									m_bKeyValueAccepted=false;
									/*if(m_bDrawKey)
									{
										if(GetParent())
											GetParent()->PostMessage(WM_PEEP_CLEARKEY);
									}*/
									m_bDrawKey=false;
									m_bDrawWarning=false;
								}

								m_bWaitConfirm=false;
								m_bKeyBeep=TRUE;

								m_v.iValue=dTemp;
							}

							
						}
					}
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
			if(		getModel()->getCONFIG()->GetCurMode()==VM_PSV
				||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_PSV
				||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
				||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV)
			{
				//if(FALSE==getModel()->getCONFIG()->isPPSVasDeltaPEEPValue())
				VIEWPARAPTN->setPEEPvalue(m_v.iValue,false);
			}
			if(GetParent())
				GetParent()->PostMessage(WM_PEEP_CHANGED,m_v.iValue);
		}

		CParaBtn::OnKeyUp(nChar,nRepCnt,nFlags);
	}
	m_bLMouseButtonDown = false;
}


void CParaBtn_PEEP::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bLMouseButtonDown )
	{
		if(ChildWindowFromPoint(point)==FromHandle(this->m_hWnd))
		{
			if(m_bDepressed)
			{
				if(		getModel()->getCONFIG()->GetCurMode()==VM_PSV
					||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_PSV
					||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
					||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV)
				{
					VIEWPARAPTN->setPEEPvalue(m_v.iValue,false);
				}
				if(GetParent())
					GetParent()->PostMessage(WM_PEEP_CHANGED,m_v.iValue);
			}
		}
	}
	CParaBtn::OnLButtonUp(nFlags, point);
}
