// WndSubSettingsEFlow.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndSubSettingsEFlow.h"

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000

#define TIMECHANGE	600



// CWndSubSettingsEFlow

IMPLEMENT_DYNAMIC(CWndSubSettingsEFlow, CWnd)

CWndSubSettingsEFlow::CWndSubSettingsEFlow(UINT viewFlag):
CWndSubSettings(viewFlag)
{
	m_pcUp_Up=NULL;
	m_pcUp_Dw=NULL;
	m_pcDw_Up=NULL;
	m_pcDw_Dw=NULL;

	m_pcStatic_Up=NULL;
	m_pcStatic_Dw=NULL;
	m_pcStatic_Fc=NULL;

	m_pcPara_Select=NULL;
	m_pcPara_Select_yes=NULL;
	m_pcPara_Select_no=NULL;

	m_pbtnValue=NULL;

	m_pcNextUp=NULL;
	m_pcNextDwn=NULL;

	m_iValue=0;
	m_iCounter=0;
	m_eTimeChanger=TC_OFF;

	
	if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
	{
		m_iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMinPara_IPPV();
		m_iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMaxPara_IPPV();
	}
	else
	{
		m_iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMinPara_TRIGGER();
		m_iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMaxPara_TRIGGER();
	}
}

CWndSubSettingsEFlow::~CWndSubSettingsEFlow()
{
	if(m_pcPara_Select)
		delete m_pcPara_Select;
	m_pcPara_Select=NULL;
	if(m_pcPara_Select_yes)
		delete m_pcPara_Select_yes;
	m_pcPara_Select_yes=NULL;
	if(m_pcPara_Select_no)
		delete m_pcPara_Select_no;
	m_pcPara_Select_no=NULL;

	delete m_pcStatic_Up;
	m_pcStatic_Up=NULL;
	delete m_pcStatic_Dw;
	m_pcStatic_Dw=NULL;
	delete m_pcStatic_Fc;
	m_pcStatic_Fc=NULL;

	delete m_pcUp_Up;
	delete m_pcUp_Dw;
	delete m_pcDw_Up;
	delete m_pcDw_Dw;

	m_pcUp_Up=NULL;
	m_pcUp_Dw=NULL;
	m_pcDw_Up=NULL;
	m_pcDw_Dw=NULL;

	delete m_pbtnValue;
	m_pbtnValue=NULL;

	delete m_pcNextUp;
	delete m_pcNextDwn;

	m_pcNextUp=NULL;
	m_pcNextDwn=NULL;
}


BEGIN_MESSAGE_MAP(CWndSubSettingsEFlow, CWnd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SETUP_YEAR, &CWndSubSettingsEFlow::OnBnClickedValue)
END_MESSAGE_MAP()



// CWndSubSettingsEFlow message handlers
// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsEFlow::Initialize()
{
	CClientDC dc(this);

	/*if(getModel()->getDATAHANDLER()->activeModeIsIPPV())
	{
		m_iValue=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWPara_IPPV();
	}
	else*/
	{
		m_iValue=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWPara_TRIGGER();
	}
	CStringW strVal;

	strVal.Format(_T("%0.0f"),CTlsFloat::Round(((double)m_iValue)/1000, 0));

	m_pcUp_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_UPUP);
	m_pcUp_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_UPDW);
	//m_pcUp_Fc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_UPFC);
	m_pcDw_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_DWUP);
	m_pcDw_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_DWDW);
	//m_pcDw_Fc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_DWFC);

	m_pcStatic_Up= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_VALUE_UP);
	m_pcStatic_Dw= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_VALUE_DW);
	m_pcStatic_Fc= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_VALUE_FC);

	m_pcPara_Select=NULL;
	m_pcPara_Select_yes= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SELECT_YES);
	m_pcPara_Select_no= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SELECT_NO);

	BTN btn;

	//*******************Days*****************************
	btn.wID					= IDC_BTN_SETUP_YEAR;	
	btn.poPosition.x		= 320;
	btn.poPosition.y		= 155;
	btn.pcBmpUp				= m_pcStatic_Up;
	btn.pcBmpDown			= m_pcStatic_Dw;
	btn.pcBmpFocus			= m_pcStatic_Fc;
	btn.pcBmpDisabled		= m_pcStatic_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pbtnValue=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
	m_pbtnValue->Create(this,g_hf33AcuBold,0);
	m_pbtnValue->SetText(strVal);
	m_pbtnValue->ShowWindow(SW_SHOW);


	btn.wID					= IDC_BTN_SETUP_NEXTUP;	
	btn.poPosition.x		= 435;
	btn.poPosition.y		= 143;
	btn.pcBmpUp				= m_pcUp_Up;
	btn.pcBmpDown			= m_pcUp_Dw;
	btn.pcBmpFocus			= m_pcUp_Up;
	btn.pcBmpDisabled		= m_pcUp_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcNextUp=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
	m_pcNextUp->Create(this,g_hf21AcuBold,0);
	m_pcNextUp->SetText(_T(""));
	m_pcNextUp->ShowWindow(SW_SHOW);

	btn.wID					= IDC_BTN_SETUP_NEXTDW;	
	btn.poPosition.x		= 435;
	btn.poPosition.y		= 185;
	btn.pcBmpUp				= m_pcDw_Up;
	btn.pcBmpDown			= m_pcDw_Dw;
	btn.pcBmpFocus			= m_pcDw_Up;
	btn.pcBmpDisabled		= m_pcDw_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcNextDwn=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
	m_pcNextDwn->Create(this,g_hf21AcuBold,0);
	m_pcNextDwn->SetText(_T(""));
	m_pcNextDwn->ShowWindow(SW_SHOW);


	btn.wID					= IDC_BTN_SELECT_EFLOW;	
	btn.poPosition.x		= 230;
	btn.poPosition.y		= 300;
	btn.pcBmpUp				= m_pcPara_Select_no;
	btn.pcBmpDown			= m_pcPara_Select_yes;
	btn.pcBmpFocus			= m_pcPara_Select_no;
	btn.pcBmpDisabled		= m_pcPara_Select_no;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	m_pcPara_Select=new CUpDwnBtn(btn,COLOR_TXTBTNUP,false,false);
	m_pcPara_Select->Create(this,g_hf21AcuBold,0);
	m_pcPara_Select->SetText(_T(""),_T(""));
	m_pcPara_Select->ShowWindow(SW_SHOW);

	if(getModel()->getCONFIG()->getShowEFlowInSettings()==true)
	{
		m_pcPara_Select->Depress(true);
	}
	else
	{
		m_pcPara_Select->Depress(false);
	}

	m_szSetting = getModel()->GetLanguageString(IDS_PARA_EFLOW);
	m_iNumValues=0;
	
	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);

}

// **************************************************************************
// 
// **************************************************************************
void CWndSubSettingsEFlow::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);



	CBrush cbrBack(BACKGND);
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf11AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	CBrush cbrFill;
	cbrFill.CreateSolidBrush(RGB(181,178,181));
	//cbrFill.CreateSolidBrush(RGB(150,150,150));
	SelectObject(hdcMem,cbrFill);

	Rectangle(hdcMem, 200, 30, 600, 465);

	Rectangle(hdcMem, 220, 10, 580, 30);
	Rectangle(hdcMem, 220, 465, 580, 485);

	m_pcRegionTopLeCorner->Draw(hdcMem,200,10);
	m_pcRegionTopRiCorner->Draw(hdcMem,580,10);
	m_pcRegionBotLeCorner->Draw(hdcMem,200,465);
	m_pcRegionBotRiCorner->Draw(hdcMem,580,465);

	//CDC* pDCMem=CDC::FromHandle(hdcMem);


	rc.left = 220;  
	rc.top = 25;  
	rc.right  = 580;  
	rc.bottom = 500;
	DrawText(hdcMem,m_szSetting,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	CPen penBlack;
	penBlack.CreatePen(PS_SOLID,2,RGB(0,0,0));
	SelectObject(hdcMem,(HPEN)penBlack);
	MoveToEx(hdcMem, 200, 60, NULL);
	LineTo(hdcMem, 600, 60);

	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	SelectObject(hdcMem,cbrRound);
	RoundRect(hdcMem, 210, 70, 590, 400,20,20);

	MoveToEx(hdcMem, 300, 143, NULL);
	LineTo(hdcMem, 435, 143);

	MoveToEx(hdcMem, 300, 225, NULL);
	LineTo(hdcMem, 435, 225);

	rc.left = 500;  
	rc.top = 143;  
	rc.right  = 600;  
	rc.bottom = 225;
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_UNIT_LMIN),-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);


	rc.left = 210;  
	rc.top = 240;//225;  
	rc.right  = 590;  
	rc.bottom = 300;

	if(getModel()->getVMODEHANDLER()->activeModeIsIPPV())
	{
		m_iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMinPara_IPPV();
		m_iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMaxPara_IPPV();
	}
	else
	{
		m_iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMinPara_TRIGGER();
		m_iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetEFLOWMaxPara_TRIGGER();
	}

	CStringW sz=_T("");
	sz.Format(_T("%s: %0.0f - %0.0f %s"),
		getModel()->GetLanguageString(IDS_TXT_RANGE),
		CTlsFloat::Round(((double)m_iLowerLimit)/1000, 0),
		CTlsFloat::Round(((double)m_iUpperLimit)/1000, 0),
		getModel()->GetLanguageString(IDS_UNIT_LMIN));
	DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_CENTER|DT_SINGLELINE);


	rc.left = 310;  
	rc.top = 300;  
	rc.right  = 590;  
	rc.bottom = 350;
	//DrawText(hdcMem,_T("shortcut in settings menu"),-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_EFLOW_SHORTCUT),-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);


	SelectObject(hdcMem,g_hf10AcuBold);
	rc.left = 210;  
	rc.top = 263;//225;  
	rc.right  = 590;  
	rc.bottom = 300;
	DrawText(hdcMem,_T("(default: 6 l/min)"),-1,&rc,DT_TOP|DT_CENTER|DT_SINGLELINE);


	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	penBlack.DeleteObject();
	penLine.DeleteObject();

	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

}

// **************************************************************************
// 
// **************************************************************************
BOOL CWndSubSettingsEFlow::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				/*int iID=0;
				CWnd* pFocWnd=NULL;
				pFocWnd = CWnd::GetFocus();
				if(pFocWnd==NULL)
					return 1;
				iID=pFocWnd->GetDlgCtrlID();*/

				eBtnState eState = m_pbtnValue->GetState();
				
				if(eState==BS_NONE)
				{
					m_pbtnValue->SetState(BS_FOCUSED);
				}
				else if(eState==BS_UP)
				{
					m_pbtnValue->SetState(BS_DOWN);
				}
				else if(eState==BS_FOCUSED)
				{
					m_pbtnValue->SetState(BS_DOWN);
				}
				else if(eState==BS_DOWN)
				{
					m_pbtnValue->SetState(BS_FOCUSED);
				}
				/*else if(iID==IDC_BTN_SETTING_BACK)
				{
					PostMessage(WM_MENUBUTTONDOWN,IDC_BTN_SETTING_BACK);
				}*/

				if(GetParent())
					GetParent()->PostMessage(WM_SET_SETUPTIMER);
				return 1;
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				if(m_pbtnValue->GetState()==BS_DOWN)
				{
					if(m_iValue>m_iLowerLimit)
					{
						m_iValue-=STEP_EFLOW;

						CStringW strVal;

						//strVal.Format(_T("%d"),m_iValue);
						strVal.Format(_T("%0.0f"),CTlsFloat::Round(((double)m_iValue)/1000, 0));
						m_pbtnValue->RefreshText(strVal);
					}
					else
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
					}
				}
				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
			}
			else if(pMsg->wParam==VK_UP)
			{
				if(m_pbtnValue->GetState()==BS_DOWN)
				{
					if(m_iValue<m_iUpperLimit)
					{
						m_iValue+=STEP_EFLOW;

						CStringW strVal;

						//strVal.Format(_T("%d"),m_iValue);
						strVal.Format(_T("%0.0f"),CTlsFloat::Round(((double)m_iValue)/1000, 0));
						m_pbtnValue->RefreshText(strVal);
					}
					else
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
					}
				}
				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
			}
			else
			{
				//handle focus change manual, draw next focused button and return true
				//return 1;
			}
			break;
		}
		break;
	default:
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}


//************************************
// Method:    OnDestroy
// FullName:  CWndSubSettingsEFlow::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndSubSettingsEFlow::OnDestroy()
{
	getModel()->getDATAHANDLER()->SetEFlowParaData(m_iValue);

	KillTimer(CHANGETIMER);
	CWndSubSettings::OnDestroy();
}

void CWndSubSettingsEFlow::OnBnClickedValue()
{
	//SetOneButtonDepressed(IDC_BTN_SETUP_YEAR);
	if(m_pbtnValue->GetState()==BS_UP)
		m_pbtnValue->SetState(BS_DOWN);
	else if(m_pbtnValue->GetState()==BS_FOCUSED)
		m_pbtnValue->SetState(BS_DOWN);
	else if(m_pbtnValue->GetState()==BS_DOWN)
		m_pbtnValue->SetState(BS_FOCUSED);
}

// **************************************************************************
// 
// **************************************************************************
LRESULT CWndSubSettingsEFlow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_TIMEBTN_DOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_SETUP_NEXTUP:
				{
					m_iCounter=0;
					m_eTimeChanger=TC_EFLOW_UP;
					SetTimer(CHANGETIMER,TIMECHANGE,NULL);
					m_pbtnValue->SetState(BS_DOWN);

					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
					return 1;
				}
				break;
			case IDC_BTN_SETUP_NEXTDW:
				{
					m_iCounter=0;
					m_eTimeChanger=TC_EFLOW_DW;
					SetTimer(CHANGETIMER,TIMECHANGE,NULL);
					m_pbtnValue->SetState(BS_DOWN);

					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
					return 1;
				}
				break;
			default:
				break;
			}
		}
		break;
	case WM_TIMEBTN_UP:
		{
			switch(wParam)
			{
			case IDC_BTN_SETUP_NEXTUP:
				{
					if(m_iCounter==0)
					{
						if(m_iValue<m_iUpperLimit)
						{
							m_iValue+=STEP_EFLOW;

							CStringW strVal;

							//strVal.Format(_T("%d"),m_iValue);
							strVal.Format(_T("%0.0f"),CTlsFloat::Round(((double)m_iValue)/1000, 0));
							m_pbtnValue->RefreshText(strVal);
						}
						else
						{
							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
						}
					}
					m_iCounter=0;
					m_eTimeChanger=TC_OFF;
					KillTimer(CHANGETIMER);

					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
					return 1;
				}
				break;
			case IDC_BTN_SETUP_NEXTDW:
				{
					if(m_iCounter==0)
					{
						if(m_iValue>m_iLowerLimit)
						{
							m_iValue-=STEP_EFLOW;

							CStringW strVal;

							//strVal.Format(_T("%d"),m_iValue);
							strVal.Format(_T("%0.0f"),CTlsFloat::Round(((double)m_iValue)/1000, 0));
							m_pbtnValue->RefreshText(strVal);
						}
						else
						{
							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
						}
					}
					m_iCounter=0;
					m_eTimeChanger=TC_OFF;
					KillTimer(CHANGETIMER);

					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
					return 1;
				}
				break;
			default:
				break;
			}
		}
		break;
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_SELECT_EFLOW:
				{
					if(getModel()->getCONFIG()->getShowEFlowInSettings()==true)
					{
						getModel()->getCONFIG()->setShowEFlowInSettings(false);
					}
					else
					{
						getModel()->getCONFIG()->setShowEFlowInSettings(true);
					}
					return 1;
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}


void CWndSubSettingsEFlow::OnTimer(UINT_PTR nIDEvent)
{
	CStringW strDate;

	if(nIDEvent==CHANGETIMER)
	{
		switch(m_eTimeChanger)
		{
		case TC_EFLOW_UP:
			{
				if(m_iValue<m_iUpperLimit)
				{
					m_iValue+=STEP_EFLOW;

					CStringW strVal;

					//strVal.Format(_T("%d"),m_iValue);
					strVal.Format(_T("%0.0f"),CTlsFloat::Round(((double)m_iValue)/1000, 0));
					m_pbtnValue->RefreshText(strVal);
				}
				else
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
				}

				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
			}
			break;
		case TC_EFLOW_DW:
			{
				if(m_iValue>m_iLowerLimit)
				{
					m_iValue-=STEP_EFLOW;

					CStringW strVal;

					//strVal.Format(_T("%d"),m_iValue);
					strVal.Format(_T("%0.0f"),CTlsFloat::Round(((double)m_iValue)/1000, 0));
					m_pbtnValue->RefreshText(strVal);
				}
				else
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
				}

				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
			}
			break;
		default:
			{

			}
			break;
		}

		m_iCounter++;
	}


	CWnd::OnTimer(nIDEvent);
}