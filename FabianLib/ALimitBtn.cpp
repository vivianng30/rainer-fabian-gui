// ALimitBtn.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "ALimitBtn.h"
#include "globDefs.h"


// CALimitBtn

IMPLEMENT_DYNAMIC(CALimitBtn, CButton)

CALimitBtn::CALimitBtn(const BTN &btn):
m_pModel(NULL),m_pcClock_Up(NULL),m_pcClock_Dw(NULL),m_pcClock_Fc(NULL),m_pcClockInactive_Up(NULL),m_pcClockInactive_Dw(NULL),m_pcClockInactive_Fc(NULL),
m_pcLimitHigh_Up(NULL),m_pcLimitHigh_Dw(NULL),m_pcLimitHigh_Fc(NULL),m_pcLimitLow_Up(NULL),m_pcLimitLow_Dw(NULL),m_pcLimitLow_Fc(NULL),
m_hBm(NULL),m_hBmPrev(NULL),m_hDC(NULL),m_bAlarmLimitState(AL_ON),m_eAlarm(AS_NONE),m_ePrevstate(AL_ON),m_kUp(VK_UP),m_kDown(VK_DOWN),m_kSpace(VK_SPACE),
m_iLimitRange(NOLIMIT),m_iPrevValue(0),m_bHasFocus(false),m_bDepressed(false),m_bLMouseButtonDown(false),m_bLButtonDown(false),m_bEndOfRange(false),m_bLowOff(false),m_bHighOff(false)
{
	// Init state

	m_v.iAbsoluteLowerLimit=0;
	m_v.iAbsoluteUpperLimit=0;
	m_v.iCurrentLimit=0;


	m_dwPushDelta=0;
	
	memcpy(&m_btn,&btn,sizeof(BTN));
	// Default colors
	m_btncr.crTxtUp			= 0x00000000;
	m_btncr.crTxtDown		= 0x00000000;
	m_btncr.crTxtDisabled	= 0x00c0c0c0;;		
	m_btncr.crTxtFocus		= 0x00000000;	
}

CALimitBtn::~CALimitBtn()
{

	if(m_hDC)
	{
		SelectObject(m_hDC,m_hBmPrev);
		DeleteObject(m_hBm);
		DeleteDC(m_hDC);
	}

	delete m_pcClock_Up;
	m_pcClock_Up=NULL;
	delete m_pcClock_Dw;
	m_pcClock_Dw=NULL;
	delete m_pcClock_Fc;
	m_pcClock_Fc=NULL;

	delete m_pcClockInactive_Up;
	m_pcClockInactive_Up=NULL;
	delete m_pcClockInactive_Dw;
	m_pcClockInactive_Dw=NULL;
	delete m_pcClockInactive_Fc;
	m_pcClockInactive_Fc=NULL;

	delete m_pcLimitHigh_Up;
	m_pcLimitHigh_Up=NULL;
	delete m_pcLimitHigh_Dw;
	m_pcLimitHigh_Dw=NULL;
	delete m_pcLimitHigh_Fc;
	m_pcLimitHigh_Fc=NULL;
	delete m_pcLimitLow_Up;
	m_pcLimitLow_Up=NULL;
	delete m_pcLimitLow_Dw;
	m_pcLimitLow_Dw=NULL;
	delete m_pcLimitLow_Fc;
	m_pcLimitLow_Fc=NULL;

}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CALimitBtn::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


BEGIN_MESSAGE_MAP(CALimitBtn, CButton)
	//{{AFX_MSG_MAP(CALimitBtn)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CALimitBtn-Meldungshandler
BOOL CALimitBtn::Create(CWnd* pParentWnd,DWORD dwStyle, fALVALUE &v) 
{
	SIZE sz;
	sz.cx=m_btn.pcBmpUp->Width();
	sz.cy=m_btn.pcBmpUp->Height();
	CRect cRect(m_btn.poPosition,sz);

	//if(CButton::Create(_T("btn"),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW|WS_TABSTOP,cRect,pParentWnd,m_btn.wID))
	if(CButton::Create(_T("btn"),dwStyle,cRect,pParentWnd,m_btn.wID))
	{
		CClientDC dc(this);

		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBm=CreateCompatibleBitmap(dc.m_hDC,m_btn.pcBmpUp->Width(),m_btn.pcBmpUp->Height());
		m_hBmPrev=static_cast<HBITMAP>(SelectObject(m_hDC,m_hBm));

		m_rcClient.left=m_rcClient.top=0;
		m_rcClient.right=sz.cx;
		m_rcClient.bottom=sz.cy;

		m_pcClock_Up = new CBmp(theApp.m_hInstance,m_hDC,	IDB_ALARMCLOCK_UP);
		if(m_pcClock_Up == NULL)
			return FALSE;
		m_pcClock_Dw = new CBmp(theApp.m_hInstance,m_hDC,	IDB_ALARMCLOCK_DW);
		if(m_pcClock_Dw == NULL)
			return FALSE;
		m_pcClock_Fc = new CBmp(theApp.m_hInstance,m_hDC,	IDB_ALARMCLOCK_FC);
		if(m_pcClock_Fc == NULL)
			return FALSE;

		m_pcClockInactive_Up = new CBmp(theApp.m_hInstance,m_hDC,	IDB_CLOCK_INACT_UP);
		if(m_pcClockInactive_Up == NULL)
			return FALSE;
		m_pcClockInactive_Dw = new CBmp(theApp.m_hInstance,m_hDC,	IDB_CLOCK_INACT_DW);
		if(m_pcClockInactive_Dw == NULL)
			return FALSE;
		m_pcClockInactive_Fc = new CBmp(theApp.m_hInstance,m_hDC,	IDB_CLOCK_INACT_FC);
		if(m_pcClockInactive_Fc == NULL)
			return FALSE;

		m_pcLimitHigh_Up = new CBmp(theApp.m_hInstance,m_hDC,	IDB_LIMIT_HIGH_UP);
		if(m_pcLimitHigh_Up == NULL)
			return FALSE;
		m_pcLimitHigh_Dw = new CBmp(theApp.m_hInstance,m_hDC,	IDB_LIMIT_HIGH_DW);
		if(m_pcLimitHigh_Dw == NULL)
			return FALSE;
		m_pcLimitHigh_Fc = new CBmp(theApp.m_hInstance,m_hDC,	IDB_LIMIT_HIGH_FC);
		if(m_pcLimitHigh_Fc == NULL)
			return FALSE;
		m_pcLimitLow_Up	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_LIMIT_LOW_UP);
		if(m_pcLimitLow_Up == NULL)
			return FALSE;
		m_pcLimitLow_Dw	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_LIMIT_LOW_DW);
		if(m_pcLimitLow_Dw == NULL)
			return FALSE;
		m_pcLimitLow_Fc	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_LIMIT_LOW_FC);
		if(m_pcLimitLow_Fc == NULL)
			return FALSE;

		memcpy(&m_v,&v,sizeof(fALVALUE));

		return 1;
	}
	return 0;
}


// **************************************************************************
// 
// **************************************************************************
bool CALimitBtn::IsDepressed( void )
{
	// Return the buttons state
	return m_bDepressed;
}


void CALimitBtn::SetColors(COLORREF crTxtUp,COLORREF crTxtDown,COLORREF crSubTxtDown,COLORREF crTxtFocus, COLORREF crDisabel)
{
	m_btncr.crTxtUp			= crTxtUp;
	m_btncr.crTxtDown		= crTxtDown;
	m_btncr.crTxtDisabled	= crDisabel;		
	m_btncr.crTxtFocus		= crTxtFocus;
	m_btncr.crSubTxtDown	= crSubTxtDown;
}

void CALimitBtn::SetCurLimit(int val, bool bRedraw) 
{
	m_v.iCurrentLimit=val;

	if(bRedraw)
	{
		Invalidate();
		UpdateWindow();
	}
}

void CALimitBtn::Redraw() 
{
	CClientDC dc(this);

	if(m_bHasFocus)
	{
		m_btn.pcBmpFocus->Draw(m_hDC);
		Draw(BTN_DW_FOCUS);
	}
	else if(m_bDepressed)
	{
		m_btn.pcBmpDown->Draw(m_hDC);
		Draw(BTN_DW_DW);
	}
	else
	{
		m_btn.pcBmpUp->Draw(m_hDC);
		Draw(BTN_DW_UP);
	}
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
}


bool CALimitBtn::SetAbsoluteLimits(int nLower,int nUpper)
{
	m_v.iAbsoluteLowerLimit=nLower;
	m_v.iAbsoluteUpperLimit=nUpper;
	return true;
}


void CALimitBtn::setAlarm(eStateOfAlarm state, bool bRedraw)
{
	if(m_eAlarm==state)
		return;
	m_eAlarm=state;
	if(bRedraw)
	{
		Redraw();
	}
}

eStateOfAlarm CALimitBtn::GetAlarm()
{
	return m_eAlarm;
	
}


void CALimitBtn::SetAlarmLimitState(eAlarmLimitState state)
{
	int iBtn = GetBtnId();

	if(iBtn == IDC_BTN_ALARM_PEEP_LO || iBtn == IDC_BTN_ALARM_PIPMAX_HI || iBtn == IDC_BTN_ALARM_PIPMIN_LO)
	{
		if(m_bAlarmLimitState!=state)
		{
			if(state==AL_AUTO)
			{
				if(GetParent())
					GetParent()->PostMessage(WM_SETAUTOSTATE);
			}
			else if(m_bAlarmLimitState==AL_AUTO)
			{
				if(GetParent())
					GetParent()->PostMessage(WM_SETNOAUTOSTATE);
			}
		}
	}
	m_bAlarmLimitState=state;
}


void CALimitBtn::SetLimitRange(int iLimit)
{
	m_iLimitRange=iLimit;
}



void CALimitBtn::DrawDirectUp()
{
	CClientDC dc(this);

	m_bDepressed=false;

	m_btn.pcBmpUp->Draw(m_hDC);
	Draw(BTN_DW_UP);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
}

void CALimitBtn::DrawDirectDown()
{
	
	CClientDC dc(this);

	m_bDepressed=true;

	m_btn.pcBmpUp->Draw(m_hDC);
	Draw(BTN_DW_DW);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
}



// **************************************************************************
// Painting
// **************************************************************************
void CALimitBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (m_bDepressed && !(lpDrawItemStruct->itemState & ODS_DISABLED))
	{
		// Button down
		if(lpDrawItemStruct->itemState & ODS_FOCUS)	
		{
			m_bHasFocus=false;
			m_btn.pcBmpDown->Draw(m_hDC);
			Draw(BTN_DW_DW);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
		else
		{
			m_bHasFocus=false;
			m_btn.pcBmpUp->Draw(m_hDC);
			Draw(BTN_DW_UP);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
	}
	//else if(lpDrawItemStruct->itemState & ODS_DISABLED )
	//{
	//	// Button disabled
	//	if(!m_bDepressed)//up
	//	{
	//		m_btn.pcBmpUp->Draw(m_hDC);
	//		Draw(BTN_DW_UP);
	//		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	//	}
	//	else // down
	//	{
	//		m_btn.pcBmpUp->Draw(m_hDC);
	//		Draw(BTN_DW_DW);
	//		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	//	}
	//}
	else	
	{
		// Button up
		if(lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			m_bHasFocus=true;
			m_btn.pcBmpFocus->Draw(m_hDC);
			Draw(BTN_DW_FOCUS);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
		else
		{
			m_bHasFocus=false;
			m_btn.pcBmpUp->Draw(m_hDC);
			Draw(BTN_DW_UP);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
	}
}

void CALimitBtn::Draw(int nState)
{
	HFONT hPrevFont=static_cast<HFONT>(SelectObject(m_hDC,g_hf15AcuMed));
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nTxtColor;
	CBmp* pcLimit=NULL;
	CBmp* pcAlarm=m_pcClock_Up;

	switch(nState)
	{
	case BTN_DW_FOCUS:
		nTxtColor=m_btncr.crTxtFocus;
		if(m_iLimitRange==HIGHLIMIT)
		{
			pcLimit=m_pcLimitHigh_Fc;
		}
		else if(m_iLimitRange==LOWLIMIT)
		{
			pcLimit=m_pcLimitLow_Fc;
		}
		if(m_eAlarm==AS_ACTIVE)
		{
			pcAlarm=m_pcClock_Fc;
		}
		else if(m_eAlarm==AS_SIGNALED)
		{
			pcAlarm=m_pcClockInactive_Fc;
		}
		
		break;
	case BTN_DW_UP:
		nTxtColor=m_btncr.crTxtUp;
		if(m_iLimitRange==HIGHLIMIT)
		{
			pcLimit=m_pcLimitHigh_Up;
		}
		else if(m_iLimitRange==LOWLIMIT)
		{
			pcLimit=m_pcLimitLow_Up;
		}
		if(m_eAlarm==AS_ACTIVE)
		{
			pcAlarm=m_pcClock_Up;
		}
		else if(m_eAlarm==AS_SIGNALED)
		{
			pcAlarm=m_pcClockInactive_Up;
		}
		break;
	case BTN_DW_DW:
		nTxtColor=m_btncr.crTxtDown;
		if(m_iLimitRange==HIGHLIMIT)
		{
			pcLimit=m_pcLimitHigh_Dw;
		}
		else if(m_iLimitRange==LOWLIMIT)
		{
			pcLimit=m_pcLimitLow_Dw;
		}
		if(m_eAlarm==AS_ACTIVE)
		{
			pcAlarm=m_pcClock_Dw;
		}
		else if(m_eAlarm==AS_SIGNALED)
		{
			pcAlarm=m_pcClockInactive_Dw;
		}
		break;
	case BTN_DW_DIS:
	default:
		nTxtColor=m_btncr.crTxtDisabled;
		break;
	}
	int nPrevTxtColor=SetTextColor(m_hDC,nTxtColor);

	RECT rc;
	memcpy(&rc,&m_rcClient,sizeof(RECT));

	TCHAR psz[MAX_PATH];

	SelectObject(m_hDC,g_hf13AcuBoldNum);

	switch(GetBtnId())
	{
	case IDC_BTN_ALARM_MV_HI:
	case IDC_BTN_ALARM_MV_LO:
		{
			if(m_v.iCurrentLimit>=1000)
			{
				wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)m_v.iCurrentLimit)/1000, 1));
			}
			else
			{
				if(m_v.iCurrentLimit!=0)
					wsprintf(psz,_T("%0.2f"),CTlsFloat::Round(((double)m_v.iCurrentLimit)/1000, 2));
				else
					wsprintf(psz,_T("%d"),0);
			}
		}
		break;
	case IDC_BTN_ALARM_PIPMAX_HI:
		{
			if(m_v.iCurrentLimit!=0)
				wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)m_v.iCurrentLimit)/10, 0));
			else
				wsprintf(psz,_T("%d"),0);
		}
		break;
	case IDC_BTN_ALARM_PIPMIN_LO:
		{
			if(m_v.iCurrentLimit!=0)
				wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)m_v.iCurrentLimit)/10, 0));
			else
				wsprintf(psz,_T("%d"),0);
		}
		break;
	case IDC_BTN_ALARM_PEEP_LO:
		{
			if(m_v.iCurrentLimit!=0)
				wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)m_v.iCurrentLimit)/10, 1));
			else
				wsprintf(psz,_T("%d"),0);
		}
		break;
	case IDC_BTN_ALARM_SPO2_HI:
		{
			if(m_v.iCurrentLimit!=0)
			{
				//wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)m_v.iCurrentLimit)/10, 1));
				wsprintf(psz,_T("%d"),m_v.iCurrentLimit/10);
			}
			else
			{
				wsprintf(psz,_T("%d"),0);
			}
		}
		break;
	case IDC_BTN_ALARM_SPO2_LO:
		{
			if(m_v.iCurrentLimit!=0)
			{
				//wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)m_v.iCurrentLimit)/10, 1));
				wsprintf(psz,_T("%d"),m_v.iCurrentLimit/10);
			}
			else
			{
				wsprintf(psz,_T("%d"),0);
				
			}
		}
		break;
	case IDC_BTN_ALARM_SPO2_PI_LO:
		{
			if(m_v.iCurrentLimit!=0)
			{
				//wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)m_v.iCurrentLimit)/1000, 1));

				if(m_v.iCurrentLimit>=1000)
				{
					wsprintf(psz,_T("%d"),m_v.iCurrentLimit/1000);
				}
				else if(m_v.iCurrentLimit>=100)
				{
					wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)m_v.iCurrentLimit)/1000, 1));
				}
				else
				{
					wsprintf(psz,_T("%0.2f"),CTlsFloat::Round(((double)m_v.iCurrentLimit)/1000, 2));
				}
			}
			else
			{
				wsprintf(psz,_T("%d"),0);
				
			}
		}
		break;
	case IDC_BTN_ALARM_SPO2_SIQ:
		{
			wsprintf(psz,_T("%d"),m_v.iCurrentLimit);
		}
		break;
	case IDC_BTN_ALARM_PULSERATE_HI:
		{
			/*if(m_v.iCurrentLimit>m_v.iAbsoluteUpperLimit)
			{
				m_v.iCurrentLimit=m_v.iAbsoluteUpperLimit;
				wsprintf(psz,getModel()->GetLanguageString(IDS_TXT_OFF));
				if(m_bAlarmLimitState!=AL_OFF)
				{
					m_bAlarmLimitState=AL_OFF;
					getModel()->getALARMHANDLER()->setAlimitState_PulseRatemaxLimit(AL_OFF);
				}
			}
			else if(m_v.iCurrentLimit!=0)*/
			{
				wsprintf(psz,_T("%d"),m_v.iCurrentLimit);
			}
			/*else
			{
				m_v.iCurrentLimit=m_v.iAbsoluteUpperLimit;
				wsprintf(psz,getModel()->GetLanguageString(IDS_TXT_OFF));
				if(m_bAlarmLimitState!=AL_OFF)
				{
					m_bAlarmLimitState=AL_OFF;
					getModel()->getALARMHANDLER()->setAlimitState_PulseRatemaxLimit(AL_OFF);
				}
			}*/
		}
		break;
	case IDC_BTN_ALARM_PULSERATE_LO:
		{
			//if(m_v.iCurrentLimit<m_v.iAbsoluteLowerLimit)
			//{
			//	m_v.iCurrentLimit=m_v.iAbsoluteLowerLimit;
			//	wsprintf(psz,getModel()->GetLanguageString(IDS_TXT_OFF));
			//	if(m_bAlarmLimitState!=AL_OFF)
			//	{
			//		m_bAlarmLimitState=AL_OFF;
			//		getModel()->getALARMHANDLER()->setAlimitState_PulseRateminLimit(AL_OFF);
			//	}
			//}
			//else //if(m_v.iCurrentLimit!=0)
			{
				wsprintf(psz,_T("%d"),m_v.iCurrentLimit);
			}
		}
		break;
	case IDC_BTN_ALARM_BPM_HI:
	case IDC_BTN_ALARM_LEAK_HI:
		{
			wsprintf(psz,_T("%d"),m_v.iCurrentLimit);
		}
		break;
	case IDC_BTN_ALARM_APNOE:
		{
			wsprintf(psz,_T("%d"),m_v.iCurrentLimit);
		}
		break;
	case IDC_BTN_ALARM_DCO2_HI:
	case IDC_BTN_ALARM_DCO2_LO:
		{
			wsprintf(psz,_T("%d"),m_v.iCurrentLimit);
		}
		break;
	case IDC_BTN_ALARM_ETCO2_HI:
	case IDC_BTN_ALARM_ETCO2_LO:
		{
			if(m_v.iCurrentLimit!=0)
				wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)m_v.iCurrentLimit)/10, 0));
			else
				wsprintf(psz,_T("%d"),0);
		}
		break;
	case IDC_BTN_ALARM_FICO2_HI:
	case IDC_BTN_ALARM_FICO2_LO:
		{
			if(m_v.iCurrentLimit!=0)
				wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)m_v.iCurrentLimit)/10, 0));
			else
				wsprintf(psz,_T("%d"),0);
		}
		break;
	}

	rc.top = 0;
	rc.bottom = m_rcClient.bottom;
	rc.left = m_rcClient.left;
	rc.right = 87;

	SelectObject(m_hDC,g_hf15AcuMed);

	if(m_iLimitRange==HIGHLIMIT)
	{
		if(m_bAlarmLimitState==AL_CALC)
		{
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_AUTO),-1,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
		else if(m_bAlarmLimitState==AL_OFF)
		{
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_OFF),-1,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
		}
		else
		{
			DrawText(m_hDC,psz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);

			if(m_bAlarmLimitState==AL_AUTO)
			{
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_AUTO),-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
			}
		}

		if(m_eAlarm == AS_ACTIVE || m_eAlarm == AS_SIGNALED)
		{
			if(pcAlarm)
				pcAlarm->Draw(m_hDC,5,25);
		}
		if(pcLimit)
			pcLimit->Draw(m_hDC,5,5);
	}
	else if(m_iLimitRange==LOWLIMIT)
	{
		if(m_bAlarmLimitState==AL_CALC)
		{
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_AUTO),-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}
		else if(m_bAlarmLimitState==AL_OFF)
		{
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_OFF),-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
		}
		else
		{
			DrawText(m_hDC,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);

			if(m_bAlarmLimitState==AL_AUTO)
			{
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_AUTO),-1,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
			}
		}

		if(m_eAlarm == AS_ACTIVE || m_eAlarm == AS_SIGNALED)
		{
			if(pcAlarm)
				pcAlarm->Draw(m_hDC,5,5);
		}
		if(pcLimit)
			pcLimit->Draw(m_hDC,5,34);
	}
	else
	{
		if(m_bAlarmLimitState==AL_OFF)
		{
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_OFF),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_RIGHT);
		}
		else
		{
			DrawText(m_hDC,psz,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_RIGHT);
		}

		if(m_eAlarm == AS_ACTIVE || m_eAlarm == AS_SIGNALED)
		{
			if(pcAlarm)
				pcAlarm->Draw(m_hDC,0,20);
		}
		
	}

	SetTextColor(m_hDC,nPrevTxtColor);
	SetBkMode(m_hDC,nBkMode);
	SelectObject(m_hDC,hPrevFont);
}



// **************************************************************************
// 
// **************************************************************************
void CALimitBtn::SetUDKeys(WORD kUP, WORD kDown, WORD kSpace)
{
	m_kUp=kUP;
	m_kDown=kDown;
	m_kSpace=kSpace;
}

// **************************************************************************
// 
// **************************************************************************
int CALimitBtn::GetBtnId()
{
	return m_btn.wID;
}

// **************************************************************************
// Button is very slow - this code permforms speed up - but
//              use of double click is not possible any more
// **************************************************************************
void CALimitBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	DWORD lParam;
	WORD  po[2]={static_cast<WORD>(point.x),static_cast<WORD>(point.y)};
	memcpy(&lParam,po,sizeof(DWORD));

	SendMessage(WM_LBUTTONDOWN,nFlags,lParam);
	CButton::OnLButtonDblClk(nFlags, point);
}

void CALimitBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( !m_bLMouseButtonDown )
	{
		m_bLMouseButtonDown = true;
		CButton::OnLButtonDown(nFlags, point);

		getModel()->getCONFIG()->SetLastSelectedALBtns(m_btn.wID);
	}
}

void CALimitBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bLMouseButtonDown )
	{
		if(ChildWindowFromPoint(point)==FromHandle(this->m_hWnd))
		{
			if(m_bDepressed)
			{
				WriteCurrentValue();
			}
			else
			{
				m_ePrevstate = m_bAlarmLimitState;
				m_iPrevValue=m_v.iCurrentLimit;
			}
			m_bDepressed = !m_bDepressed;
		}
		CButton::OnLButtonUp(nFlags, point);
	}
	m_bLMouseButtonDown = false;
}
void CALimitBtn::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( !m_bLMouseButtonDown )
	{
		m_bLMouseButtonDown = true;
		if(nChar==32)	//VK_SPACE
		{
			if(!isSafeTickCountDelayExpired(m_dwPushDelta, BTN_DELAY))
			{
				return;
			}

			m_dwPushDelta=GetTickCount();

			if(m_bDepressed)
			{
				WriteCurrentValue();
			}
			else
			{
				m_ePrevstate = m_bAlarmLimitState;
				m_iPrevValue=m_v.iCurrentLimit;
			}
			m_bDepressed = !m_bDepressed;
		}
		CButton::OnKeyDown(nChar,nRepCnt,nFlags);
	}
}

void CALimitBtn::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( m_bLMouseButtonDown )
	{
		m_dwPushDelta=GetTickCount();

		int dTemp;
		if(nChar==m_kDown)	
		{
			if(m_bDepressed)
			{
				switch(GetBtnId())
				{
				case IDC_BTN_ALARM_MV_HI:
					{
						dTemp=getModel()->getALARMHANDLER()->getAlimitMinRangeMVmax();

						if(m_v.iCurrentLimit<=dTemp)
						{
							m_bEndOfRange = true;
						}
						else
						{
							m_v.iCurrentLimit = (m_v.iCurrentLimit/10)*10;

							if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
							{
								getModel()->getALARMHANDLER()->setAlimitState_MVmaxLimit(AL_ON);
								m_bAlarmLimitState=AL_ON;
							}

							if(m_v.iCurrentLimit<=260)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-10;
							}
							else if(m_v.iCurrentLimit<=500)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-20;
							}
							else if(m_v.iCurrentLimit<=1000)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-50;
							}
							else
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-100;
							}
						}
					}
					break;
				case IDC_BTN_ALARM_MV_LO:
					{
						//rku MVLOW
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_MVminLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						m_v.iCurrentLimit = (m_v.iCurrentLimit/10)*10;

						if(m_bAlarmLimitState==AL_OFF)
						{
							;
						}
						else if(m_v.iCurrentLimit>m_v.iAbsoluteLowerLimit)
						{
							if(m_v.iCurrentLimit<=260)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-10;
							}
							else if(m_v.iCurrentLimit<=500)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-20;
							}
							else if(m_v.iCurrentLimit<=1000)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-50;
							}
							else
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-100;
							}
						}
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_MVminLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;
						}
					}
					break;
				case IDC_BTN_ALARM_PIPMAX_HI:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_PIPmaxLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;

							if(GetParent())
								GetParent()->PostMessage(WM_SETNOAUTOSTATE);
						}

						m_v.iCurrentLimit = (m_v.iCurrentLimit/10)*10;

						if(m_v.iCurrentLimit>m_v.iAbsoluteLowerLimit)
							m_v.iCurrentLimit=m_v.iCurrentLimit-10;
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_PIPMIN_LO:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_PIPminLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;

							if(GetParent())
								GetParent()->PostMessage(WM_SETNOAUTOSTATE);
						}

						m_v.iCurrentLimit = (m_v.iCurrentLimit/10)*10;

						if(m_v.iCurrentLimit>m_v.iAbsoluteLowerLimit)
							m_v.iCurrentLimit=m_v.iCurrentLimit-10;
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_PEEP_LO:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_PEEPminLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;

							if(GetParent())
								GetParent()->PostMessage(WM_SETNOAUTOSTATE);
						}

						m_v.iCurrentLimit = (m_v.iCurrentLimit/5)*5;

						if(m_v.iCurrentLimit>m_v.iAbsoluteLowerLimit)
							m_v.iCurrentLimit=m_v.iCurrentLimit-5;
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_BPM_HI:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_BPMmaxLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							getModel()->getALARMHANDLER()->setAlimitState_BPMmaxLimit(AL_ON);

							m_bAlarmLimitState=AL_ON;

							int iVal=getModel()->getDATAHANDLER()->calculateAutoAlarmlimitBPMmax();
							if(iVal<m_v.iAbsoluteLowerLimit)
								iVal=m_v.iAbsoluteLowerLimit;
							else if(iVal>m_v.iAbsoluteUpperLimit)
								iVal=m_v.iAbsoluteUpperLimit;

							SetCurLimit(iVal, false);

						}
						else if(m_v.iCurrentLimit>m_v.iAbsoluteLowerLimit)
							m_v.iCurrentLimit--;
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_LEAK_HI:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_LeakmaxLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF && !m_bLowOff && m_bHighOff)
						{
							m_bHighOff=false;
							getModel()->getALARMHANDLER()->setAlimitState_LeakmaxLimit(AL_ON);

							m_bAlarmLimitState=AL_ON;

							m_v.iCurrentLimit=m_v.iAbsoluteUpperLimit;
						}
						else if(m_bAlarmLimitState==AL_OFF && !m_bLowOff && !m_bHighOff)
						{
							getModel()->getALARMHANDLER()->setAlimitState_LeakmaxLimit(AL_ON);

							m_bAlarmLimitState=AL_ON;

							m_v.iCurrentLimit=m_v.iAbsoluteUpperLimit;
						}
						else if(m_v.iCurrentLimit>m_v.iAbsoluteLowerLimit)
						{
							m_v.iCurrentLimit--;
						}
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_LeakmaxLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;
							m_bLowOff=true;
						}
					}
					break;
				case IDC_BTN_ALARM_APNOE:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_ApnoeLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							getModel()->getALARMHANDLER()->setAlimitState_ApnoeLimit(AL_ON);

							m_bAlarmLimitState=AL_ON;

							SetCurLimit(m_v.iAbsoluteUpperLimit, false);
						}
						else if(m_v.iCurrentLimit>m_v.iAbsoluteLowerLimit)
							m_v.iCurrentLimit--;
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_DCO2_HI:
					{
						dTemp=getModel()->getALARMHANDLER()->getAlimitMinRangeDCO2max();

						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_DCO2maxLimit(AL_ON);
							getModel()->getALARMHANDLER()->setAlimitState_DCO2minLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
							if(GetParent())
								GetParent()->PostMessage(WM_ALIMIT_STATE_DCO2_HI_CHANGED);
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							getModel()->getALARMHANDLER()->setAlimitState_DCO2maxLimit(AL_ON);
							getModel()->getALARMHANDLER()->setAlimitState_DCO2minLimit(AL_ON);

							m_bAlarmLimitState=AL_ON;

							if(GetParent())
								GetParent()->PostMessage(WM_ALIMIT_STATE_DCO2_HI_CHANGED);
							SetCurLimit(m_v.iAbsoluteUpperLimit, false);

						}
						else if(m_v.iCurrentLimit>dTemp)
						{
							if(m_v.iCurrentLimit<=200)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-1;
							}
							else if(m_v.iCurrentLimit<=500)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-5;
							}
							else if(m_v.iCurrentLimit<=1000)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-10;
							}
							else
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-50;
							}
						}
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_DCO2_LO:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_DCO2maxLimit(AL_ON);
							getModel()->getALARMHANDLER()->setAlimitState_DCO2minLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
							if(GetParent())
								GetParent()->PostMessage(WM_ALIMIT_STATE_DCO2_LO_CHANGED);
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							;
						}
						else if(m_v.iCurrentLimit>m_v.iAbsoluteLowerLimit)
						{
							if(m_v.iCurrentLimit<=200)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-1;
							}
							else if(m_v.iCurrentLimit<=500)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-5;
							}
							else if(m_v.iCurrentLimit<=1000)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-10;
							}
							else
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-50;
							}
						}
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_DCO2maxLimit(AL_OFF);
							getModel()->getALARMHANDLER()->setAlimitState_DCO2minLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;

							if(GetParent())
								GetParent()->PostMessage(WM_ALIMIT_STATE_DCO2_LO_CHANGED);
						}
					}
					break;
				case IDC_BTN_ALARM_ETCO2_HI:
					{
						dTemp=getModel()->getALARMHANDLER()->getAlimitMinRangeETCO2max();

						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_ETCO2maxLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							getModel()->getALARMHANDLER()->setAlimitState_ETCO2maxLimit(AL_ON);
							
							m_bAlarmLimitState=AL_ON;

							SetCurLimit(m_v.iAbsoluteUpperLimit, false);

						}
						else if(m_v.iCurrentLimit>dTemp)
						{
							m_v.iCurrentLimit=m_v.iCurrentLimit-10;
						}
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_ETCO2_LO:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_ETCO2minLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							;
						}
						else if(m_v.iCurrentLimit>m_v.iAbsoluteLowerLimit)
						{
							m_v.iCurrentLimit=m_v.iCurrentLimit-10;
						}
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_ETCO2minLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;
						}
					}
					break;
				case IDC_BTN_ALARM_FICO2_HI:
					{
						dTemp=getModel()->getALARMHANDLER()->getAlimitMinRangeFICO2max();

						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_FICO2maxLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							getModel()->getALARMHANDLER()->setAlimitState_FICO2maxLimit(AL_ON);

							m_bAlarmLimitState=AL_ON;

							SetCurLimit(m_v.iAbsoluteUpperLimit, false);

						}
						else if(m_v.iCurrentLimit>dTemp)
						{
							m_v.iCurrentLimit=m_v.iCurrentLimit-10;
						}
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_FICO2_LO:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_FICO2minLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							;
						}
						else if(m_v.iCurrentLimit>m_v.iAbsoluteLowerLimit)
						{
							m_v.iCurrentLimit=m_v.iCurrentLimit-10;
						}
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_FICO2minLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;
						}
					}
					break;
				case IDC_BTN_ALARM_SPO2_HI:
					{
						m_v.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangeSPO2max();

						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_SPO2maxLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							if(m_v.iCurrentLimit>m_v.iAbsoluteLowerLimit)
							{
								getModel()->getALARMHANDLER()->setAlimitState_SPO2maxLimit(AL_ON);

								m_bAlarmLimitState=AL_ON;
								m_v.iCurrentLimit=m_v.iAbsoluteUpperLimit-10;
							}
						}
						else if(m_v.iCurrentLimit>m_v.iAbsoluteLowerLimit)
						{
							dTemp=m_v.iCurrentLimit-10;
							if(dTemp<m_v.iAbsoluteLowerLimit)
							{
								m_bEndOfRange = true;
							}
							else
							{
								m_v.iCurrentLimit=dTemp;
							}
						}
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_SPO2_LO:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_SPO2minLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							;
						}
						else if(m_v.iCurrentLimit>m_v.iAbsoluteLowerLimit)
						{
							dTemp=m_v.iCurrentLimit-10;

							if(dTemp<=m_v.iAbsoluteLowerLimit)
							{
								getModel()->getALARMHANDLER()->setAlimitState_SPO2minLimit(AL_OFF);

								m_bEndOfRange = true;
								m_bAlarmLimitState=AL_OFF;
							}
							m_v.iCurrentLimit=dTemp;
						}
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_SPO2minLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;
						}
					}
					break;
				case IDC_BTN_ALARM_PULSERATE_HI:
					{
						dTemp=getModel()->getALARMHANDLER()->getAlimitMinRangePulseRatemax();

						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_PulseRatemaxLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							getModel()->getALARMHANDLER()->setAlimitState_PulseRatemaxLimit(AL_ON);

							m_bAlarmLimitState=AL_ON;

							SetCurLimit(m_v.iAbsoluteUpperLimit, false);
						}
						else if(m_v.iCurrentLimit>dTemp)
						{
							m_v.iCurrentLimit=m_v.iCurrentLimit-5;
						}
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_PULSERATE_LO:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_PulseRateminLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							;
						}
						else if(m_v.iCurrentLimit>m_v.iAbsoluteLowerLimit)
						{
							m_v.iCurrentLimit=m_v.iCurrentLimit-5;
						}
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_PulseRateminLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;
						}
					}
					break;
				case IDC_BTN_ALARM_SPO2_SIQ:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_SPO2_SIQminLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							;
						}
						else if(m_v.iCurrentLimit-5>m_v.iAbsoluteLowerLimit)
						{
							m_v.iCurrentLimit=m_v.iCurrentLimit-5;
						}
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_SPO2_SIQminLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;
							SetCurLimit(m_v.iAbsoluteLowerLimit, false);
						}
					}
					break;
				case IDC_BTN_ALARM_SPO2_PI_LO:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_SPO2_PIminLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							;
						}
						else if(m_v.iCurrentLimit>m_v.iAbsoluteLowerLimit)
						{
							if(m_v.iCurrentLimit>1000)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-1000;
							}
							else if(m_v.iCurrentLimit>100)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-100;
							}
							else
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit-10;
							}
						}
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_SPO2_PIminLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;
						}
					}
					break;
				}

			
				CClientDC dc(this);
				m_btn.pcBmpDown->Draw(m_hDC);
				Draw(BTN_DW_DW);
				BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
			}
			else
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_FOCUS_VKDOWN);
			}
		}
		else if(nChar==m_kUp)	
		{
			if(m_bDepressed)
			{
				switch(GetBtnId())
				{
				case IDC_BTN_ALARM_MV_HI:
					{
						m_v.iCurrentLimit = (m_v.iCurrentLimit/10)*10;

						if(m_v.iCurrentLimit>=m_v.iAbsoluteUpperLimit)
						{
							m_bEndOfRange = true;
						}
						else
						{
							if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
							{
								getModel()->getALARMHANDLER()->setAlimitState_MVmaxLimit(AL_ON);
								m_bAlarmLimitState=AL_ON;
							}

							if(m_v.iCurrentLimit>=1000)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+100;
							}
							else if(m_v.iCurrentLimit>=500)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+50;
							}
							else if(m_v.iCurrentLimit>=260)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+20;
							}
							else
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+10;
							}
						}
					}
					break;
				case IDC_BTN_ALARM_MV_LO:
					{
						//rku MVLOW
						dTemp=(int)CTlsFloat::Round(getModel()->getALARMHANDLER()->getAlimitMaxRangeMVmin(), 2);
						
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_MVminLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}
						if(m_bAlarmLimitState==AL_OFF)
						{
							getModel()->getALARMHANDLER()->setAlimitState_MVminLimit(AL_ON);

							m_bAlarmLimitState=AL_ON;

							SetCurLimit(m_v.iAbsoluteLowerLimit, false);
						}
						else if(m_v.iCurrentLimit<dTemp)
						{
							if(m_v.iCurrentLimit>=1000)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+100;
							}
							else if(m_v.iCurrentLimit>=500)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+50;
							}
							else if(m_v.iCurrentLimit>=260)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+20;
							}
							else
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+10;
							}
						}
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_PIPMAX_HI:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_PIPmaxLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;

							if(GetParent())
								GetParent()->PostMessage(WM_SETNOAUTOSTATE);
						}
						m_v.iCurrentLimit = (m_v.iCurrentLimit/10)*10;

						if(m_v.iCurrentLimit<m_v.iAbsoluteUpperLimit)
							m_v.iCurrentLimit=m_v.iCurrentLimit+10;
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_PIPMIN_LO:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_PIPminLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;

							if(GetParent())
								GetParent()->PostMessage(WM_SETNOAUTOSTATE);
						}
						m_v.iCurrentLimit = (m_v.iCurrentLimit/10)*10;

						if(m_v.iCurrentLimit<m_v.iAbsoluteUpperLimit)
							m_v.iCurrentLimit=m_v.iCurrentLimit+10;
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_PEEP_LO:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_PEEPminLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;

							if(GetParent())
								GetParent()->PostMessage(WM_SETNOAUTOSTATE);
						}

						m_v.iCurrentLimit = (m_v.iCurrentLimit/5)*5;

						if(m_v.iCurrentLimit<m_v.iAbsoluteUpperLimit)
							m_v.iCurrentLimit=m_v.iCurrentLimit+5;
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_BPM_HI:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_BPMmaxLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							;
						}
						else if(m_v.iCurrentLimit<m_v.iAbsoluteUpperLimit)
							m_v.iCurrentLimit++;
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_BPMmaxLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;
						}
					}
					break;
				case IDC_BTN_ALARM_LEAK_HI:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_LeakmaxLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF && m_bLowOff && !m_bHighOff)
						{
							m_bLowOff=false;
							getModel()->getALARMHANDLER()->setAlimitState_LeakmaxLimit(AL_ON);

							m_bAlarmLimitState=AL_ON;

							m_v.iCurrentLimit=m_v.iAbsoluteLowerLimit;
						}
						else if(m_bAlarmLimitState==AL_OFF && !m_bLowOff && !m_bHighOff)
						{
							getModel()->getALARMHANDLER()->setAlimitState_LeakmaxLimit(AL_ON);

							m_bAlarmLimitState=AL_ON;

							m_v.iCurrentLimit=m_v.iAbsoluteLowerLimit;
						}
						else if(m_v.iCurrentLimit<m_v.iAbsoluteUpperLimit)
						{
							m_v.iCurrentLimit++;
						}
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_LeakmaxLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;
							m_bHighOff=true;
						}
					}
					break;
				case IDC_BTN_ALARM_APNOE:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_ApnoeLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							;
						}
						else if(m_v.iCurrentLimit<m_v.iAbsoluteUpperLimit)
							m_v.iCurrentLimit++;
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_ApnoeLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;
						}
					}
					break;
				case IDC_BTN_ALARM_DCO2_HI:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_DCO2maxLimit(AL_ON);
							getModel()->getALARMHANDLER()->setAlimitState_DCO2minLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
							if(GetParent())
								GetParent()->PostMessage(WM_ALIMIT_STATE_DCO2_HI_CHANGED);
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							;
						}
						else if(m_v.iCurrentLimit<m_v.iAbsoluteUpperLimit)
						{
							if(m_v.iCurrentLimit>=1000)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+50;
							}
							else if(m_v.iCurrentLimit>=500)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+10;
							}
							else if(m_v.iCurrentLimit>=200)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+5;
							}
							else
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+1;
							}
						}
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_DCO2maxLimit(AL_OFF);
							getModel()->getALARMHANDLER()->setAlimitState_DCO2minLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;

							if(GetParent())
								GetParent()->PostMessage(WM_ALIMIT_STATE_DCO2_HI_CHANGED);
						}
					}
					break;
				case IDC_BTN_ALARM_DCO2_LO:
					{
						dTemp=getModel()->getALARMHANDLER()->getAlimitMaxRangeDCO2min();

						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_DCO2maxLimit(AL_ON);
							getModel()->getALARMHANDLER()->setAlimitState_DCO2minLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
							if(GetParent())
								GetParent()->PostMessage(WM_ALIMIT_STATE_DCO2_LO_CHANGED);
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							getModel()->getALARMHANDLER()->setAlimitState_DCO2maxLimit(AL_ON);
							getModel()->getALARMHANDLER()->setAlimitState_DCO2minLimit(AL_ON);

							m_bAlarmLimitState=AL_ON;
							if(GetParent())
								GetParent()->PostMessage(WM_ALIMIT_STATE_DCO2_LO_CHANGED);

							SetCurLimit(m_v.iAbsoluteLowerLimit, false);
						}
						else if(m_v.iCurrentLimit<dTemp)
						{
							if(m_v.iCurrentLimit>=1000)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+50;
							}
							else if(m_v.iCurrentLimit>=500)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+10;
							}
							else if(m_v.iCurrentLimit>=200)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+5;
							}
							else
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+1;
							}
						}
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_ETCO2_HI:
					{
					if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_ETCO2maxLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							;
						}
						else if(m_v.iCurrentLimit<m_v.iAbsoluteUpperLimit)
						{
							m_v.iCurrentLimit=m_v.iCurrentLimit+10;
						}
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_ETCO2maxLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;
						}
					}
					break;
				case IDC_BTN_ALARM_ETCO2_LO:
					{
						dTemp=getModel()->getALARMHANDLER()->getAlimitMaxRangeETCO2min();

						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_ETCO2minLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							getModel()->getALARMHANDLER()->setAlimitState_ETCO2minLimit(AL_ON);
							
							m_bAlarmLimitState=AL_ON;

							SetCurLimit(m_v.iAbsoluteLowerLimit, false);
						}
						else if(m_v.iCurrentLimit<dTemp)
						{
							m_v.iCurrentLimit=m_v.iCurrentLimit+10;
						}
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_FICO2_HI:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_FICO2maxLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							;
						}
						else if(m_v.iCurrentLimit<m_v.iAbsoluteUpperLimit)
						{
							m_v.iCurrentLimit=m_v.iCurrentLimit+10;
						}
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_FICO2maxLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;
						}
					}
					break;
				case IDC_BTN_ALARM_FICO2_LO:
					{
						dTemp=getModel()->getALARMHANDLER()->getAlimitMaxRangeFICO2min();

						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_FICO2minLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							getModel()->getALARMHANDLER()->setAlimitState_FICO2minLimit(AL_ON);

							m_bAlarmLimitState=AL_ON;

							SetCurLimit(m_v.iAbsoluteLowerLimit, false);
						}
						else if(m_v.iCurrentLimit<dTemp)
						{
							m_v.iCurrentLimit=m_v.iCurrentLimit+10;
						}
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_SPO2_HI:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_SPO2maxLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}
						dTemp=m_v.iCurrentLimit;

						if(m_bAlarmLimitState==AL_OFF)
						{
							;
						}
						else if(m_v.iCurrentLimit<m_v.iAbsoluteUpperLimit)
						{
							dTemp=m_v.iCurrentLimit+10;

							if(dTemp>=m_v.iAbsoluteUpperLimit)
							{
								getModel()->getALARMHANDLER()->setAlimitState_SPO2maxLimit(AL_OFF);

								m_bEndOfRange = true;
								m_bAlarmLimitState=AL_OFF;
							}
							m_v.iCurrentLimit=dTemp;
						}
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_SPO2maxLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;

						}
					}
					break;
				case IDC_BTN_ALARM_SPO2_LO:
					{
						m_v.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangeSPO2min();

						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_SPO2minLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							if(m_v.iCurrentLimit<m_v.iAbsoluteUpperLimit)
							{
								getModel()->getALARMHANDLER()->setAlimitState_SPO2minLimit(AL_ON);

								m_bAlarmLimitState=AL_ON;
								m_v.iCurrentLimit=10;
							}
						}
						else if(m_v.iCurrentLimit<m_v.iAbsoluteUpperLimit)
						{
							m_v.iCurrentLimit=m_v.iCurrentLimit+10;
						}
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_PULSERATE_HI:
					{
						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_PulseRatemaxLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							;
						}
						else if(m_v.iCurrentLimit<m_v.iAbsoluteUpperLimit)
						{
							m_v.iCurrentLimit=m_v.iCurrentLimit+5;
						}
						else
						{
							getModel()->getALARMHANDLER()->setAlimitState_PulseRatemaxLimit(AL_OFF);

							m_bEndOfRange = true;
							m_bAlarmLimitState=AL_OFF;
						}
					}
					break;
				case IDC_BTN_ALARM_PULSERATE_LO:
					{
						dTemp=getModel()->getALARMHANDLER()->getAlimitMaxRangePulseRatemin();

						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_PulseRateminLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							getModel()->getALARMHANDLER()->setAlimitState_PulseRateminLimit(AL_ON);

							m_bAlarmLimitState=AL_ON;
							
							SetCurLimit(m_v.iAbsoluteLowerLimit, false);
						}
						else if(m_v.iCurrentLimit<dTemp)
						{
							m_v.iCurrentLimit=m_v.iCurrentLimit+5;
						}
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_SPO2_SIQ:
					{
						dTemp=getModel()->getALARMHANDLER()->getAlimitMaxRangeSPO2_SIQmin();

						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_SPO2_SIQminLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							getModel()->getALARMHANDLER()->setAlimitState_SPO2_SIQminLimit(AL_ON);

							m_bAlarmLimitState=AL_ON;

							SetCurLimit(m_v.iAbsoluteLowerLimit+5, false);
						}
						else if(m_v.iCurrentLimit<dTemp)
						{
							m_v.iCurrentLimit=m_v.iCurrentLimit+5;
						}
						else
							m_bEndOfRange = true;
					}
					break;
				case IDC_BTN_ALARM_SPO2_PI_LO:
					{
						dTemp=getModel()->getALARMHANDLER()->getAlimitMaxRangeSPO2_PImin();

						if(m_bAlarmLimitState==AL_AUTO || m_bAlarmLimitState==AL_CALC)
						{
							getModel()->getALARMHANDLER()->setAlimitState_SPO2_PIminLimit(AL_ON);
							m_bAlarmLimitState=AL_ON;
						}

						if(m_bAlarmLimitState==AL_OFF)
						{
							getModel()->getALARMHANDLER()->setAlimitState_SPO2_PIminLimit(AL_ON);

							m_bAlarmLimitState=AL_ON;

							SetCurLimit(m_v.iAbsoluteLowerLimit, false);
						}
						else if(m_v.iCurrentLimit<dTemp)
						{
							if(m_v.iCurrentLimit<100)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+10;
							}
							else if(m_v.iCurrentLimit<1000)
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+100;
							}
							else
							{
								m_v.iCurrentLimit=m_v.iCurrentLimit+1000;
							}
						}
						else
							m_bEndOfRange = true;
					}
					break;
				}

				CClientDC dc(this);
				m_btn.pcBmpDown->Draw(m_hDC);
				Draw(BTN_DW_DW);
				BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
			}
			else
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_FOCUS_VKUP);
			}
		}
		CButton::OnKeyUp(nChar,nRepCnt,nFlags);
	}
	m_bLMouseButtonDown = false;
}

void CALimitBtn::OnSetFocus(CWnd* pOldWnd)
{
	CButton::OnSetFocus(pOldWnd);

	if(GetParent())
		GetParent()->PostMessage( WM_ALARMBTN_SETFOCUS, GetDlgCtrlID(), 0);
	
}

void CALimitBtn::OnKillFocus(CWnd* pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);
	//DEBUGMSG(TRUE, (TEXT("CALimitBtn::OnKillFocus()1 %d\r\n"),GetBtnId()));
	if(m_bDepressed)
	{
		//DEBUGMSG(TRUE, (TEXT("CALimitBtn::OnKillFocus()2 %d\r\n"),GetBtnId()));

		switch(GetBtnId())
		{
		case IDC_BTN_ALARM_MV_HI:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_MVmaxLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_MV_LO:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_MVminLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_PIPMAX_HI:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
				{
					if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
					{
						m_bAlarmLimitState=m_ePrevstate;
					}
					else if(m_ePrevstate==AL_CALC)
					{
						m_bAlarmLimitState=AL_ON;
					}
					else
					{
						m_bAlarmLimitState=m_ePrevstate;
					}
				}
				else
				{
					if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
					{
						m_bAlarmLimitState=m_ePrevstate;
					}
					else if(m_ePrevstate==AL_CALC)
					{
						m_bAlarmLimitState=AL_AUTO;
					}
					else
					{
						m_bAlarmLimitState=m_ePrevstate;
					}
				}
				getModel()->getALARMHANDLER()->setAlimitState_PIPmaxLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_PIPMIN_LO:
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
				{
					/*if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
					{
						m_bAlarmLimitState=m_ePrevstate;
					}
					else if(m_ePrevstate==AL_CALC)
					{
						m_bAlarmLimitState=AL_ON;
					}
					else
					{
						m_bAlarmLimitState=m_ePrevstate;
					}*/
				}
				else
				{
					if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
					{
						m_bAlarmLimitState=m_ePrevstate;
					}
					else if(m_ePrevstate==AL_CALC)
					{
						m_bAlarmLimitState=AL_AUTO;
					}
					else
					{
						m_bAlarmLimitState=m_ePrevstate;
					}
				}
				getModel()->getALARMHANDLER()->setAlimitState_PIPminLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_PEEP_LO:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_AUTO;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_PEEPminLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_BPM_HI:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_BPMmaxLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_LEAK_HI:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_LeakmaxLimit(m_bAlarmLimitState);
				m_bLowOff=false;
				m_bHighOff=false;
			}
			break;
		case IDC_BTN_ALARM_APNOE:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_ApnoeLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_DCO2_HI:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_DCO2maxLimit(m_bAlarmLimitState);
				//getModel()->getALARMHANDLER()->setAlimitState_DCO2minLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_DCO2_LO:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				//getModel()->getALARMHANDLER()->setAlimitState_DCO2maxLimit(m_bAlarmLimitState);
				getModel()->getALARMHANDLER()->setAlimitState_DCO2minLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_ETCO2_HI:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_ETCO2maxLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_ETCO2_LO:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_ETCO2minLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_FICO2_HI:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_FICO2maxLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_FICO2_LO:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_FICO2minLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_SPO2_HI:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_SPO2maxLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_SPO2_LO:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_SPO2minLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_PULSERATE_HI:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_PulseRatemaxLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_PULSERATE_LO:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_PulseRateminLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_SPO2_PI_LO:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_SPO2_PIminLimit(m_bAlarmLimitState);
			}
			break;
		case IDC_BTN_ALARM_SPO2_SIQ:
			{
				if(getModel()->CalculateAlarmlimitRunning() && m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				else if(m_ePrevstate==AL_CALC)
				{
					m_bAlarmLimitState=AL_ON;
				}
				else
				{
					m_bAlarmLimitState=m_ePrevstate;
				}
				getModel()->getALARMHANDLER()->setAlimitState_SPO2_SIQminLimit(m_bAlarmLimitState);
			}
			break;

		}

		if(m_bDepressed)
			m_v.iCurrentLimit=m_iPrevValue;
		WriteCurrentValue();

		m_bDepressed=false;
	}
	CWnd* pFocWnd=CWnd::GetFocus();
	if(pFocWnd!=0)
	{
		int iID=pFocWnd->GetDlgCtrlID();
	}
}

void CALimitBtn::WriteCurrentValue()
{
	int iID = GetBtnId();

	//int test = m_v.iCurrentLimit;

	switch(iID)
	{
	case IDC_BTN_ALARM_MV_HI:
		{
			getModel()->getALARMHANDLER()->setAlimitMVmax(m_v.iCurrentLimit);

			//if(getModel()->getDATAHANDLER()->ALARMS->GetStateOfAlarm(AL_Pat_MVmax)==AS_ACTIVE)
			{
				getModel()->getDATAHANDLER()->checkLimits();
			}
		}
		break;
	case IDC_BTN_ALARM_MV_LO:
		{
			getModel()->getALARMHANDLER()->setAlimitMVmin(m_v.iCurrentLimit);

			//if(getModel()->getDATAHANDLER()->ALARMS->GetStateOfAlarm(AL_Pat_MVmin)==AS_ACTIVE)
			{
				getModel()->getDATAHANDLER()->checkLimits();
			}

			//rku MVLOW
			if(getModel()->getALARMHANDLER()->ALARM_PatAl_MVmin->getAlarmState()!=AS_ACTIVE
				&&	getModel()->getALARMHANDLER()->getAlimitState_MVminLimit()==AL_OFF)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_MVmin);

			}
		}
		break;
	case IDC_BTN_ALARM_PIPMAX_HI:
		{
			getModel()->getALARMHANDLER()->setAlimitPIPmax(m_v.iCurrentLimit);

			if(GetParent())
				GetParent()->PostMessage(WM_PMAX_HI_CHANGED);
			

			//if(getModel()->getDATAHANDLER()->ALARMS->GetStateOfAlarm(AL_Pat_Pmax)==AS_ACTIVE)
			{
				getModel()->getDATAHANDLER()->checkLimits();
			}
		}
		break;
	case IDC_BTN_ALARM_PIPMIN_LO:
		{
			getModel()->getALARMHANDLER()->setAlimitPIPmin(m_v.iCurrentLimit);

			if(GetParent())
				GetParent()->PostMessage(WM_PMAX_LO_CHANGED);


			//if(getModel()->getDATAHANDLER()->ALARMS->GetStateOfAlarm(AL_Pat_Pmax)==AS_ACTIVE)
			{
				getModel()->getDATAHANDLER()->checkLimits();
			}
		}
		break;
	case IDC_BTN_ALARM_PEEP_LO:
		{
			getModel()->getALARMHANDLER()->setAlimitPEEPmin(m_v.iCurrentLimit);

			if(GetParent())
				GetParent()->PostMessage(WM_PEEP_LO_CHANGED);
			

			//if(getModel()->getDATAHANDLER()->ALARMS->GetStateOfAlarm(AL_Pat_Pmin)==AS_ACTIVE)
			{
				getModel()->getDATAHANDLER()->checkLimits();
			}
		}
		break;
	case IDC_BTN_ALARM_BPM_HI:
		{
			getModel()->getALARMHANDLER()->setAlimitBPMmax(m_v.iCurrentLimit);
			
			getModel()->getDATAHANDLER()->checkLimits();

			if(getModel()->getALARMHANDLER()->ALARM_PatAl_BPMmax->getAlarmState()!=AS_ACTIVE
				&&	getModel()->getALARMHANDLER()->getAlimitState_BPMmaxLimit()==AL_OFF)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_BPMmax);

			}

		}
		break;
	case IDC_BTN_ALARM_LEAK_HI:
		{
			getModel()->getALARMHANDLER()->setAlimitLeakmax(m_v.iCurrentLimit);

			getModel()->getDATAHANDLER()->checkLimits();

			if(getModel()->getALARMHANDLER()->ALARM_PatAl_Leakmax->getAlarmState()!=AS_ACTIVE
				&&	getModel()->getALARMHANDLER()->getAlimitState_LeakmaxLimit()==AL_OFF)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_Leakmax);
				m_bLowOff=false;
				m_bHighOff=false;
			}
		}
		break;
	case IDC_BTN_ALARM_APNOE:
		{
			eAlarmLimitState stateApnoe = getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit();
			if(stateApnoe==AL_ON || stateApnoe==AL_AUTO)
				getModel()->getALARMHANDLER()->setAlimitApnoe(m_v.iCurrentLimit);
			else
				getModel()->getALARMHANDLER()->setAlimitApnoe(0);

			getModel()->getDATAHANDLER()->checkLimits();

			if(getModel()->getALARMHANDLER()->ALARM_PatAl_Apnoe->getAlarmState()!=AS_ACTIVE
				&&	getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit()==AL_OFF)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_Apnoe);

			}
		}
		break;
	case IDC_BTN_ALARM_DCO2_HI:
		{
			getModel()->getALARMHANDLER()->setAlimitDCO2max(m_v.iCurrentLimit);

			getModel()->getDATAHANDLER()->checkLimits();

			if(getModel()->getALARMHANDLER()->ALARM_PatAl_DCO2max->getAlarmState()!=AS_ACTIVE
				&&	getModel()->getALARMHANDLER()->getAlimitState_DCO2maxLimit()==AL_OFF)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_DCO2max);

			}
		}
		break;
	case IDC_BTN_ALARM_DCO2_LO:
		{
			getModel()->getALARMHANDLER()->setAlimitDCO2min(m_v.iCurrentLimit);

			getModel()->getDATAHANDLER()->checkLimits();

			if(getModel()->getALARMHANDLER()->ALARM_PatAl_DCO2min->getAlarmState()!=AS_ACTIVE
				&&	getModel()->getALARMHANDLER()->getAlimitState_DCO2minLimit()==AL_OFF)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_DCO2min);

			}
		}
		break;
	case IDC_BTN_ALARM_ETCO2_HI:
		{
			getModel()->getALARMHANDLER()->setAlimitETCO2max(m_v.iCurrentLimit);

			getModel()->getDATAHANDLER()->checkLimits();

			if(getModel()->getALARMHANDLER()->ALARM_PatAl_ETCO2max->getAlarmState()!=AS_ACTIVE
				&&	getModel()->getALARMHANDLER()->getAlimitState_ETCO2maxLimit()==AL_OFF)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_ETCO2max);
			}
		}
		break;
	case IDC_BTN_ALARM_ETCO2_LO:
		{
			getModel()->getALARMHANDLER()->setAlimitETCO2min(m_v.iCurrentLimit);

			getModel()->getDATAHANDLER()->checkLimits();

			if(getModel()->getALARMHANDLER()->ALARM_PatAl_ETCO2min->getAlarmState()!=AS_ACTIVE
				&&	getModel()->getALARMHANDLER()->getAlimitState_ETCO2minLimit()==AL_OFF)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_ETCO2min);
			}
		}
		break;
	case IDC_BTN_ALARM_FICO2_HI:
		{
			getModel()->getALARMHANDLER()->setAlimitFICO2max(m_v.iCurrentLimit);

			getModel()->getDATAHANDLER()->checkLimits();

			if(getModel()->getALARMHANDLER()->ALARM_PatAl_FICO2max->getAlarmState()!=AS_ACTIVE
				&&	getModel()->getALARMHANDLER()->getAlimitState_FICO2maxLimit()==AL_OFF)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_FICO2max);
			}
		}
		break;
	case IDC_BTN_ALARM_FICO2_LO:
		{
			getModel()->getALARMHANDLER()->setAlimitFICO2min(m_v.iCurrentLimit);

			getModel()->getDATAHANDLER()->checkLimits();

			if(getModel()->getALARMHANDLER()->ALARM_PatAl_FICO2min->getAlarmState()!=AS_ACTIVE
				&&	getModel()->getALARMHANDLER()->getAlimitState_FICO2minLimit()==AL_OFF)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_FICO2min);
			}
		}
		break;
	case IDC_BTN_ALARM_SPO2_HI:
		{
			getModel()->getALARMHANDLER()->setAlimitSPO2max(m_v.iCurrentLimit);

			getModel()->getDATAHANDLER()->checkLimits();

			if(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2max->getAlarmState()!=AS_ACTIVE
				&&	getModel()->getALARMHANDLER()->getAlimitState_SPO2maxLimit()==AL_OFF)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_SPO2max);

			}
		}
		break;
	case IDC_BTN_ALARM_SPO2_LO:
		{
			getModel()->getALARMHANDLER()->setAlimitSPO2min(m_v.iCurrentLimit);

			getModel()->getDATAHANDLER()->checkLimits();

			if(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2min->getAlarmState()!=AS_ACTIVE
				&&	getModel()->getALARMHANDLER()->getAlimitState_SPO2minLimit()==AL_OFF)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_SPO2min);

			}
		}
		break;
	case IDC_BTN_ALARM_PULSERATE_HI:
		{
			getModel()->getALARMHANDLER()->setAlimitPulseRatemax(m_v.iCurrentLimit);

			getModel()->getDATAHANDLER()->checkLimits();

			if(getModel()->getALARMHANDLER()->ALARM_PatAl_PulseRatemax->getAlarmState()!=AS_ACTIVE
				&&	getModel()->getALARMHANDLER()->getAlimitState_PulseRatemaxLimit()==AL_OFF)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_PulseRatemax);

			}
		}
		break;
	case IDC_BTN_ALARM_PULSERATE_LO:
		{
			getModel()->getALARMHANDLER()->setAlimitPulseRatemin(m_v.iCurrentLimit);

			getModel()->getDATAHANDLER()->checkLimits();

			if(getModel()->getALARMHANDLER()->ALARM_PatAl_PulseRatemin->getAlarmState()!=AS_ACTIVE
				&&	getModel()->getALARMHANDLER()->getAlimitState_PulseRateminLimit()==AL_OFF)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_PulseRatemin);

			}
		}
		break;
	case IDC_BTN_ALARM_SPO2_PI_LO:
		{
			getModel()->getALARMHANDLER()->setAlimitSPO2_PImin(m_v.iCurrentLimit);

			getModel()->getDATAHANDLER()->checkLimits();

			if(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2_PImin->getAlarmState()!=AS_ACTIVE
				&&	getModel()->getALARMHANDLER()->getAlimitState_SPO2_PIminLimit()==AL_OFF)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_SPO2_PImin);
			}
		}
		break;
	case IDC_BTN_ALARM_SPO2_SIQ:
		{
			getModel()->getALARMHANDLER()->setAlimitSPO2_SIQmin(m_v.iCurrentLimit);

			getModel()->getDATAHANDLER()->checkLimits();

			if(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2_SIQmin->getAlarmState()!=AS_ACTIVE
				&&	getModel()->getALARMHANDLER()->getAlimitState_SPO2_SIQminLimit()==AL_OFF)
			{
				getModel()->getALARMHANDLER()->deleteAlarm(AL_PatAl_SPO2_SIQmin);
			}
		}
		break;
	default:
		break;
	}
}

// **************************************************************************
// Time measurement
// **************************************************************************
//rkuTICKCOUNT
bool CALimitBtn::isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay)////used to check if old tick count plus delay is still lower than actual tickCount, (dwLastTickCount+DELAY<getTickCount64())
{
	bool bExpired=false;
	DWORD curTickCount=GetTickCount();

	if(curTickCount<oldTickCount)//overrun
	{
		DWORD diff=0xFFFFFFFF-oldTickCount;

		if(diff+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	else
	{
		if(oldTickCount+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	return bExpired;
}