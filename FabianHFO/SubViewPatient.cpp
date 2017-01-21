// SubViewPatient.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SubViewPatient.h"
#include "DlgMessageBox.h"
#include "MVViewHandler.h"

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000

//global font objects
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf8AcuNorm;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf13AcuBold;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf15AcuMed;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf19AcuMed;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf23AcuBold;
extern HFONT g_hf25AcuMed;
extern HFONT g_hf27AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf33AcuBold;
extern HFONT g_hf43AcuBold;
extern HFONT g_hf53AcuBold;


// CSubViewPatient

IMPLEMENT_DYNAMIC(CSubViewPatient, CWnd)

CSubViewPatient::CSubViewPatient()
{
	m_pModel=NULL;

	m_hDC = NULL;
	m_hBmp= NULL;
	m_hBmpPrev= NULL;

	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	m_pcWndKeyboard=NULL;
	
	m_pDlg = NULL;

	m_bPatientDataChanged=false;

	m_pEditName=NULL;
	m_pEditFirstname=NULL;
	m_pEditBirthDay=NULL;
	m_pEditBirthMonth=NULL;
	m_pEditBirthYear=NULL;
	m_pEditPersID=NULL;
	m_pEditWeight=NULL;
	m_pEditRemark=NULL;

	m_pcToday_Up=NULL;
	m_pcToday_Dw=NULL;
	m_pcToday=NULL;

	m_pcPatientData=NULL;

	m_pc_Up=NULL;
	m_pc_Dw=NULL;
	m_pc_Red=NULL;
	m_pc_Dis=NULL;

	m_pcReset=NULL;
	m_pcChange=NULL;
	m_pcAply=NULL;
	
	m_bChangeMode=false;

	m_dwLastSetupTimer=0;
}

CSubViewPatient::~CSubViewPatient()
{
	delete m_pEditName;
	m_pEditName=NULL;
	delete m_pEditFirstname;
	m_pEditFirstname=NULL;
	delete m_pEditBirthDay;
	m_pEditBirthDay=NULL;
	delete m_pEditBirthMonth;
	m_pEditBirthMonth=NULL;
	delete m_pEditBirthYear;
	m_pEditBirthYear=NULL;
	delete m_pEditPersID;
	m_pEditPersID=NULL;
	delete m_pEditWeight;
	m_pEditWeight=NULL;
	delete m_pEditRemark;
	m_pEditRemark=NULL;

	delete m_pc_Up;
	m_pc_Up=NULL;
	delete m_pc_Dw;
	m_pc_Dw=NULL;
	delete m_pc_Red;
	m_pc_Red=NULL;
	delete m_pc_Dis;
	m_pc_Dis=NULL;

	delete  m_pcToday_Up;
	m_pcToday_Up=NULL;
	delete  m_pcToday_Dw;
	m_pcToday_Dw=NULL;
	delete  m_pcToday;
	m_pcToday=NULL;

	if(m_pcPatientData)
		delete m_pcPatientData;
	m_pcPatientData=NULL;


	delete m_pcReset;
	m_pcReset=NULL;
	delete m_pcChange;
	m_pcChange=NULL;
	delete m_pcAply;
	m_pcAply=NULL;
}


BEGIN_MESSAGE_MAP(CSubViewPatient, CWnd)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_BTN_SETUP_RESET, &CSubViewPatient::OnBnClickedReset)
	ON_BN_CLICKED(IDC_BTN_SETUP_APLY, &CSubViewPatient::OnBnClickedApply)
	ON_BN_CLICKED(IDC_BTN_SETUP_CHANGE, &CSubViewPatient::OnBnClickedChange)
	ON_BN_CLICKED(IDC_BTN_SETUP_TODAY, &CSubViewPatient::OnBnClickedToday)
	ON_EN_SETFOCUS(IDC_EDIT_SETUP_NAME, &CSubViewPatient::OnSetFocusEditName)
	ON_EN_SETFOCUS(IDC_EDIT_SETUP_FIRSTNAME, &CSubViewPatient::OnSetFocusEditFirtsName)
	ON_EN_SETFOCUS(IDC_EDIT_SETUP_PERSID, &CSubViewPatient::OnSetFocusEditPersID)
	ON_EN_SETFOCUS(IDC_EDIT_SETUP_BIRTHDAY, &CSubViewPatient::OnSetFocusEditBirthDay)
	ON_EN_SETFOCUS(IDC_EDIT_SETUP_BIRTHMONTH, &CSubViewPatient::OnSetFocusEditBirthMonth)
	ON_EN_SETFOCUS(IDC_EDIT_SETUP_BIRTHYEAR, &CSubViewPatient::OnSetFocusEditBirthYear)
	ON_EN_SETFOCUS(IDC_EDIT_SETUP_WEIGHT, &CSubViewPatient::OnSetFocusEditWeight)
	ON_EN_SETFOCUS(IDC_EDIT_SETUP_REMARK, &CSubViewPatient::OnSetFocusEditRemark)
END_MESSAGE_MAP()



// CSubViewPatient message handlers

// **************************************************************************
// 
// **************************************************************************
CMVModel *CSubViewPatient::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

// **************************************************************************
// 
// **************************************************************************
bool CSubViewPatient::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
{
	if (!Create(pParentWnd, rc, nID, pContext))
	{
		return false;
	}

	Initialize();

	return true;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CSubViewPatient::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD|WS_VISIBLE,rc,pParentWnd,nID))
	{
		CClientDC dc(this);

		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		//CBrush cbrBack(RGB(210,210,255));
		CBrush cbrBack(BACKGND);
		CBrush cbrFill;
		cbrFill.CreateSolidBrush(RGB(181,178,181));
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		RECT rcCl;
		GetClientRect(&rcCl);

		rcCl.left = 5; 
		rcCl.right = 795; 
		rcCl.top = 5; 
		rcCl.bottom = 500; 
		FillRect(m_hDC,&rcCl,cbrFill);

		

		SelectObject(m_hDC,hbrprev);
		SelectObject(m_hDC,hpenprev);

		return 1;
	}
	else
		return 0;
}

void CSubViewPatient::Initialize()
{
	CClientDC dc(this);

	m_pc_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_UP);
	m_pc_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_DW);
	m_pc_Red	= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_RED);
	m_pc_Dis	= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_DIS);

	m_pcToday_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_TODAY_UP);
	m_pcToday_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_TODAY_DW);

	m_pcPatientData		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PATIENTDATA_SETTING);


	m_pEditPersID = new CEdit;
	m_pEditPersID->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(175, 15, 430, 40), this, IDC_EDIT_SETUP_PERSID);
	m_pEditPersID->SetFont(CFont::FromHandle(g_hf11AcuBold));
	CStringW szPatID=_T("");
	szPatID.Format(_T("%d"), getModel()->getCONFIG()->GetPatientID());

	m_pEditPersID->SetWindowText(szPatID);
	m_pEditPersID->ShowWindow(SW_HIDE);

	m_pEditWeight = new CEdit;
	m_pEditWeight->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(575, 15, 750, 40), this, IDC_EDIT_SETUP_WEIGHT);
	m_pEditWeight->SetFont(CFont::FromHandle(g_hf11AcuBold));
	CStringW szWeight=_T("");
	szWeight.Format(_T("%d"), getModel()->getCONFIG()->GetPatientWeight());
	m_pEditWeight->SetWindowText(szWeight);
	m_pEditWeight->ShowWindow(SW_HIDE);
	
	m_pEditName = new CEdit;
	m_pEditName->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(175, 55, 430, 80), this, IDC_EDIT_SETUP_NAME);
	m_pEditName->SetFont(CFont::FromHandle(g_hf11AcuBold));
	m_pEditName->SetWindowText(getModel()->getCONFIG()->GetPatientName());
	m_pEditName->ShowWindow(SW_HIDE);

	m_pEditFirstname = new CEdit;
	m_pEditFirstname->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(575, 55, 750, 80), this, IDC_EDIT_SETUP_FIRSTNAME);
	m_pEditFirstname->SetFont(CFont::FromHandle(g_hf11AcuBold));
	m_pEditFirstname->SetWindowText(getModel()->getCONFIG()->GetPatientFirstname());
	m_pEditFirstname->ShowWindow(SW_HIDE);

	m_pEditBirthDay = new CEdit;
	m_pEditBirthDay->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(175, 95, 210, 120), this, IDC_EDIT_SETUP_BIRTHDAY);
	//CRect(560, 100, 595, 130), this, IDC_EDIT_SETUP_BIRTHDAY);
	m_pEditBirthDay->SetFont(CFont::FromHandle(g_hf11AcuBold));
	m_pEditBirthDay->ShowWindow(SW_HIDE);

	m_pEditBirthMonth = new CEdit;
	m_pEditBirthMonth->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(215, 95, 250, 120), this, IDC_EDIT_SETUP_BIRTHMONTH);
	//CRect(600, 100, 635, 130), this, IDC_EDIT_SETUP_BIRTHMONTH);
	m_pEditBirthMonth->SetFont(CFont::FromHandle(g_hf11AcuBold));
	m_pEditBirthMonth->ShowWindow(SW_HIDE);

	m_pEditBirthYear = new CEdit;
	m_pEditBirthYear->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		CRect(255, 95, 325, 120), this, IDC_EDIT_SETUP_BIRTHYEAR);
	//CRect(640, 100, 700, 130), this, IDC_EDIT_SETUP_BIRTHYEAR);
	m_pEditBirthYear->SetFont(CFont::FromHandle(g_hf11AcuBold));
	m_pEditBirthYear->ShowWindow(SW_HIDE);


	CStringW szTime = getModel()->getCONFIG()->GetPatientBirthday();
	COleDateTime dtBirthday;
	CStringW szDay=_T("");
	CStringW szMonth=_T("");
	CStringW szYear=_T("");

	SYSTEMTIME st;
	GetLocalTime(&st);

	if(szTime == _T(""))
	{
		dtBirthday=COleDateTime(st);
	}
	else
	{
		if(false==dtBirthday.ParseDateTime(szTime,VAR_DATEVALUEONLY))
		{
			/*int iRes=dtBirthday.SetDate(2009, 1, 1);
			iRes=0;*/
			dtBirthday=COleDateTime(st);
		}

	}


	szDay.Format(_T("%02d"),dtBirthday.GetDay());
	szMonth.Format(_T("%02d"),dtBirthday.GetMonth());
	szYear.Format(_T("%04d"),dtBirthday.GetYear());

	//m_pCtrl->SetTime(dtBirthday);
	m_pEditBirthDay->SetWindowText(szDay);
	m_pEditBirthMonth->SetWindowText(szMonth);
	m_pEditBirthYear->SetWindowText(szYear);
	m_pEditBirthYear->ShowWindow(SW_HIDE);


	m_pEditRemark = new CEdit;
	m_pEditRemark->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_MULTILINE,
		CRect(175, 145, 750, 220), this, IDC_EDIT_SETUP_REMARK);
	m_pEditRemark->SetFont(CFont::FromHandle(g_hf11AcuBold));
	m_pEditRemark->SetWindowText(getModel()->getCONFIG()->GetPatientRemark());
	m_pEditRemark->ShowWindow(SW_HIDE);


	BTN btn;
	
	btn.wID					= IDC_BTN_SETUP_TODAY;	
	btn.poPosition.x		= 360;//;586
	btn.poPosition.y		= 92;
	btn.pcBmpUp				= m_pcToday_Up;
	btn.pcBmpDown			= m_pcToday_Dw;
	btn.pcBmpFocus			= m_pcToday_Up;
	btn.pcBmpDisabled		= m_pcToday_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcToday=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcToday->Create(this,g_hf15AcuMed,0);
	m_pcToday->SetText(getModel()->GetLanguageString(IDS_TXT_TODAY));
	m_pcToday->ShowWindow(SW_HIDE);

	btn.wID					= IDC_BTN_SETUP_RESET;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 300;
	btn.pcBmpUp				= m_pc_Up;
	btn.pcBmpDown			= m_pc_Dw;
	btn.pcBmpFocus			= m_pc_Up;
	btn.pcBmpDisabled		= m_pc_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcReset=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcReset->Create(this,g_hf15AcuMed,0);
	m_pcReset->SetText(getModel()->GetLanguageString(IDS_TXT_RESET));
	m_pcReset->ShowWindow(SW_SHOW);


	btn.wID					= IDC_BTN_SETUP_APLY;	
	btn.poPosition.x		= 300;
	btn.poPosition.y		= 300;
	btn.pcBmpUp				= m_pc_Up;
	btn.pcBmpDown			= m_pc_Dw;
	btn.pcBmpFocus			= m_pc_Up;
	btn.pcBmpDisabled		= m_pc_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcAply=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcAply->Create(this,g_hf15AcuMed,0);
	

	if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
	{
		m_pcAply->SetText(getModel()->GetLanguageString(IDS_TXT_APPLY));
		m_pcAply->EnableWindow(FALSE);
	}
	else
	{
		m_pcAply->SetText(getModel()->GetLanguageString(IDS_TXT_STARTVENT));
	}




	btn.wID					= IDC_BTN_SETUP_CHANGE;	
	btn.poPosition.x		= 500;
	btn.poPosition.y		= 300;
	btn.pcBmpUp				= m_pc_Up;
	btn.pcBmpDown			= m_pc_Dw;
	btn.pcBmpFocus			= m_pc_Up;
	btn.pcBmpDisabled		= m_pc_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcChange=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcChange->Create(this,g_hf15AcuMed,0);
	m_pcChange->SetText(getModel()->GetLanguageString(IDS_TXT_CHANGE));
	m_pcChange->ShowWindow(SW_SHOW);


	//CreateWndHourglass();

}

void CSubViewPatient::OnSetFocusEditRemark()
{
	if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	if(m_pcWndKeyboard)
		m_pcWndKeyboard->SetEditWnd(m_pEditRemark,KR_NONE,IDC_EDIT_SETUP_REMARK);
}
void CSubViewPatient::OnSetFocusEditName()
{
	if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	if(m_pcWndKeyboard)
		m_pcWndKeyboard->SetEditWnd(m_pEditName,KR_NONE,IDC_EDIT_SETUP_NAME);
	//m_pEdit=m_pEditName;
}
void CSubViewPatient::OnSetFocusEditFirtsName()
{
	if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	if(m_pcWndKeyboard)
		m_pcWndKeyboard->SetEditWnd(m_pEditFirstname,KR_NONE,IDC_EDIT_SETUP_FIRSTNAME);
	//m_pEdit=m_pEditFirstname;
}
void CSubViewPatient::OnSetFocusEditPersID()
{
	if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	if(m_pcWndKeyboard)
		m_pcWndKeyboard->SetEditWnd(m_pEditPersID,KR_NUMBERS,IDC_EDIT_SETUP_PERSID);

	//m_pEdit=m_pEditPersID;
}
void CSubViewPatient::OnSetFocusEditBirthDay()
{
	if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}

	//m_pcWndKeyboard->SetEditWnd(NULL,KR_DAYS,IDC_EDIT_SETUP_BIRTHDAY);
	if(m_pcWndKeyboard)
		m_pcWndKeyboard->SetEditWnd(m_pEditBirthDay,KR_DAYS,IDC_EDIT_SETUP_BIRTHDAY);
}
void CSubViewPatient::OnSetFocusEditBirthMonth()
{
	if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	if(m_pcWndKeyboard)
		m_pcWndKeyboard->SetEditWnd(m_pEditBirthMonth,KR_MONTH,IDC_EDIT_SETUP_BIRTHMONTH);
}
void CSubViewPatient::OnSetFocusEditBirthYear()
{
	if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	if(m_pcWndKeyboard)
		m_pcWndKeyboard->SetEditWnd(m_pEditBirthYear,KR_YEAR,IDC_EDIT_SETUP_BIRTHYEAR);
}
void CSubViewPatient::OnSetFocusEditWeight()
{
	if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	if(m_pcWndKeyboard)
		m_pcWndKeyboard->SetEditWnd(m_pEditWeight,KR_NUMBERS,IDC_EDIT_SETUP_WEIGHT);
	//m_pEdit=m_pEditWeight;

}



void CSubViewPatient::Show(bool bShow)
{
	if(bShow)
	{
		ShowWindow(SW_SHOW);
		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);

		Draw();

		if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
		{
			if(GetParent())
				GetParent()->PostMessage(WM_SET_SETUPTIMER);
		}
		//m_pEditName->SetFocus();
	}
	else
	{
		ShowWindow(SW_HIDE);
		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);

		
	}

	
}

LRESULT CSubViewPatient::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			/*case IDC_BTN_SETUP_RESET:
				{
					BnClickedReset();
					return 1;
				}
				break;*/
			/*case IDC_BTN_SETUP_CHANGE:
				{
					BnClickedChange();
					return 1;
				}
				break;*/
			/*case IDC_BTN_SETUP_APLY:
				{
					BnClickedApply();
					return 1;
				}
				break;*/
			default:
				{

				}
				break;
			}
		}
		break;
	/*case WM_MENUBUTTONUP:
		{
			switch(wParam)
			{
			case IDC_BTN_SETUP_RESET:
				{
					BnClickedReset();
					return 1;
				}
				break;
			case IDC_BTN_SETUP_CHANGE:
				{
					BnClickedChange();
					return 1;
				}
				break;
			case IDC_BTN_SETUP_APLY:
				{
					BnClickedApply();
					return 1;
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;*/
	case WM_NO_PERSID:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			return 1;
		}
		break;
	case WM_INVALID_DATE:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			return 1;
		}
		break;
	case WM_INVALID_DAY:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			return 1;
		}
		break;
	case WM_INVALID_MONTH:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			return 1;
		}
		break;
	case WM_INVALID_YEAR:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			return 1;
		}
		break;
	case WM_PATIENT_FOCUS_LEFT:
		{
			if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
			{
				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
			}

			switch(wParam)
			{
			case IDC_EDIT_SETUP_REMARK:
				{
					if(m_pEditRemark)
					{
						DWORD dwSel = m_pEditRemark->GetSel();
						m_pEditRemark->SetSel(HIWORD(dwSel)-1, HIWORD(dwSel)-1,true);
						m_pEditRemark->SetFocus();
					}
					return 1;
					
				}
				break;
			case IDC_EDIT_SETUP_NAME:
				{
					if(m_pEditName)
					{
						DWORD dwSel = m_pEditName->GetSel();
						m_pEditName->SetSel(HIWORD(dwSel)-1, HIWORD(dwSel)-1,true);
						m_pEditName->SetFocus();
					}
					return 1;
					
				}
				break;
			case IDC_EDIT_SETUP_FIRSTNAME:
				{
					if(m_pEditFirstname)
					{
						DWORD dwSel = m_pEditFirstname->GetSel();
						m_pEditFirstname->SetSel(HIWORD(dwSel)-1, HIWORD(dwSel)-1,true);
						m_pEditFirstname->SetFocus();
					}
					return 1;
					
				}
				break;
			case IDC_EDIT_SETUP_PERSID:
				{
					if(m_pEditPersID)
					{
						DWORD dwSel = m_pEditPersID->GetSel();
						m_pEditPersID->SetSel(HIWORD(dwSel)-1, HIWORD(dwSel)-1,true);
						m_pEditPersID->SetFocus();
					}
					return 1;
					
				}
				break;
			case IDC_EDIT_SETUP_BIRTHDAY:
				{
					if(m_pEditBirthDay)
					{
						DWORD dwSel = m_pEditBirthDay->GetSel();
						m_pEditBirthDay->SetSel(HIWORD(dwSel)-1, HIWORD(dwSel)-1,true);
						m_pEditBirthDay->SetFocus();
					}
					return 1;
					
				}
				break;
			case IDC_EDIT_SETUP_BIRTHMONTH:
				{
					if(m_pEditBirthMonth)
					{
						DWORD dwSel = m_pEditBirthMonth->GetSel();
						m_pEditBirthMonth->SetSel(HIWORD(dwSel)-1, HIWORD(dwSel)-1,true);
						m_pEditBirthMonth->SetFocus();
					}
					return 1;
					
				}
				break;
			case IDC_EDIT_SETUP_BIRTHYEAR:
				{
					if(m_pEditBirthYear)
					{
						DWORD dwSel = m_pEditBirthYear->GetSel();
						m_pEditBirthYear->SetSel(HIWORD(dwSel)-1, HIWORD(dwSel)-1,true);
						m_pEditBirthYear->SetFocus();
					}
					return 1;
					
				}
				break;
				//case IDC_EDIT_SETUP_BIRTHDAY:
				//	{
				//		//DWORD dwSel = m_pEditFirstname->GetSel();
				//		//m_pEditFirstname->SetSel(HIWORD(dwSel)+1, HIWORD(dwSel)+1,true);
				//		m_pCtrl->PostMessage(WM_KEYDOWN,VK_LEFT,0);
				//		m_pCtrl->SetFocus();
				//		m_pcWndKeyboard->SetEditWnd(NULL,KR_DAYS,IDC_EDIT_SETUP_BIRTHDAY);
				//		//m_pCtrl->GetCaretPos()
				//		
				//	}
				//	break;
			case IDC_EDIT_SETUP_WEIGHT:
				{
					if(m_pEditWeight)
					{
						DWORD dwSel = m_pEditWeight->GetSel();
						m_pEditWeight->SetSel(HIWORD(dwSel)-1, HIWORD(dwSel)-1,true);
						m_pEditWeight->SetFocus();
					}
					return 1;
					
				}
				break;
			default:
				break;
			}
		}
		break;
	case WM_PATIENT_FOCUS_RIGHT:
		{
			if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
			{
				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
			}

			switch(wParam)
			{
			case IDC_EDIT_SETUP_REMARK:
				{
					if(m_pEditRemark)
					{
						DWORD dwSel = m_pEditRemark->GetSel();
						m_pEditRemark->SetSel(HIWORD(dwSel)+1, HIWORD(dwSel)+1,true);
						m_pEditRemark->SetFocus();
					}
					return 1;
					
				}
				break;
			case IDC_EDIT_SETUP_NAME:
				{
					if(m_pEditName)
					{
						DWORD dwSel = m_pEditName->GetSel();
						m_pEditName->SetSel(HIWORD(dwSel)+1, HIWORD(dwSel)+1,true);
						m_pEditName->SetFocus();
					}
					return 1;
					
				}
				break;
			case IDC_EDIT_SETUP_FIRSTNAME:
				{
					if(m_pEditFirstname)
					{
						DWORD dwSel = m_pEditFirstname->GetSel();
						m_pEditFirstname->SetSel(HIWORD(dwSel)+1, HIWORD(dwSel)+1,true);
						m_pEditFirstname->SetFocus();
					}
					return 1;
					
				}
				break;
			case IDC_EDIT_SETUP_PERSID:
				{
					if(m_pEditPersID)
					{
						DWORD dwSel = m_pEditPersID->GetSel();
						m_pEditPersID->SetSel(HIWORD(dwSel)+1, HIWORD(dwSel)+1,true);
						m_pEditPersID->SetFocus();
					}
					return 1;
					
				}
				break;
			case IDC_EDIT_SETUP_BIRTHDAY:
				{
					if(m_pEditBirthDay)
					{
						DWORD dwSel = m_pEditBirthDay->GetSel();
						m_pEditBirthDay->SetSel(HIWORD(dwSel)+1, HIWORD(dwSel)+1,true);
						m_pEditBirthDay->SetFocus();
					}
					return 1;
					
				}
				break;
			case IDC_EDIT_SETUP_BIRTHMONTH:
				{
					if(m_pEditBirthMonth)
					{
						DWORD dwSel = m_pEditBirthMonth->GetSel();
						m_pEditBirthMonth->SetSel(HIWORD(dwSel)+1, HIWORD(dwSel)+1,true);
						m_pEditBirthMonth->SetFocus();
					}
					return 1;
					
				}
				break;
			case IDC_EDIT_SETUP_BIRTHYEAR:
				{
					if(m_pEditBirthYear)
					{
						DWORD dwSel = m_pEditBirthYear->GetSel();
						m_pEditBirthYear->SetSel(HIWORD(dwSel)+1, HIWORD(dwSel)+1,true);
						m_pEditBirthYear->SetFocus();
					}
					return 1;
					
				}
				break;
				//case IDC_EDIT_SETUP_BIRTHDAY:
				//	{
				//		//DWORD dwSel = m_pEditFirstname->GetSel();
				//		//m_pEditFirstname->SetSel(HIWORD(dwSel)+1, HIWORD(dwSel)+1,true);
				//		//m_pCtrl->SetFocus();
				//		m_pCtrl->PostMessage(WM_KEYDOWN,VK_RIGHT,0);
				//		m_pCtrl->SetFocus();
				//		m_pcWndKeyboard->SetEditWnd(NULL,KR_DAYS,IDC_EDIT_SETUP_BIRTHDAY);
				//	}
				//	break;
			case IDC_EDIT_SETUP_WEIGHT:
				{
					if(m_pEditWeight)
					{
						DWORD dwSel = m_pEditWeight->GetSel();
						m_pEditWeight->SetSel(HIWORD(dwSel)+1, HIWORD(dwSel)+1,true);
						m_pEditWeight->SetFocus();
					}
					return 1;
					
				}
				break;
			default:
				break;
			}

		}
		break;
	case WM_PATIENT_FOCUS_UP:
		{
			if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
			{
				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
			}

			switch(wParam)
			{
			case IDC_EDIT_SETUP_NAME:
				{
					if(m_pEditWeight)
						m_pEditWeight->SetFocus();
					return 1;
					
				}
				break;
			case IDC_EDIT_SETUP_FIRSTNAME:
				{
					if(m_pEditName)
						m_pEditName->SetFocus();
					return 1;
				}
				break;
			case IDC_EDIT_SETUP_PERSID:
				{
					
					if(m_pEditRemark)
						m_pEditRemark->SetFocus();
					return 1;
				}
				break;
			case IDC_EDIT_SETUP_BIRTHDAY:
				{
					if(m_pEditFirstname)
						m_pEditFirstname->SetFocus();
					return 1;
				}
				break;
			case IDC_EDIT_SETUP_BIRTHMONTH:
				{
					if(m_pEditBirthDay)
						m_pEditBirthDay->SetFocus();
					return 1;
				}
				break;
			case IDC_EDIT_SETUP_BIRTHYEAR:
				{
					if(m_pEditBirthMonth)
						m_pEditBirthMonth->SetFocus();
					return 1;
				}
				break;
			case IDC_EDIT_SETUP_WEIGHT:
				{
					if(m_pEditPersID)
						m_pEditPersID->SetFocus();
					return 1;
				}
				break;
			case IDC_EDIT_SETUP_REMARK:
				{
					if(m_pEditBirthYear)
						m_pEditBirthYear->SetFocus();
					return 1;
				}
				break;
			default:
				break;
			}
		}
		break;
	case WM_PATIENT_FOCUS_DOWN:
		{
			if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
			{
				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
			}

			switch(wParam)
			{
			case IDC_EDIT_SETUP_NAME:
				{
					if(m_pEditFirstname)
						m_pEditFirstname->SetFocus();
					return 1;
				}
				break;
			case IDC_EDIT_SETUP_FIRSTNAME:
				{
					if(m_pEditBirthDay)
						m_pEditBirthDay->SetFocus();
					return 1;
				}
				break;
			case IDC_EDIT_SETUP_PERSID:
				{
					if(m_pEditWeight)
						m_pEditWeight->SetFocus();
					return 1;
				}
				break;
			case IDC_EDIT_SETUP_BIRTHDAY:
				{
					if(m_pEditBirthMonth)
						m_pEditBirthMonth->SetFocus();
					return 1;
				}
				break;
			case IDC_EDIT_SETUP_BIRTHMONTH:
				{
					if(m_pEditBirthYear)
						m_pEditBirthYear->SetFocus();
					return 1;
				}
				break;
			case IDC_EDIT_SETUP_BIRTHYEAR:
				{
					if(m_pEditRemark)
						m_pEditRemark->SetFocus();
					return 1;
				}
				break;
			case IDC_EDIT_SETUP_WEIGHT:
				{
					if(m_pEditName)
						m_pEditName->SetFocus();
					return 1;
				}
				break;
			case IDC_EDIT_SETUP_REMARK:
				{
					if(m_pEditPersID)
						m_pEditPersID->SetFocus();
					return 1;
				}
				break;
			default:
				break;
			}
		}
		break;
	case WM_PATIENTDATA:
		{
			PatientDataChanged();
			return 1;
		}
		break;
	case WM_SET_SETUPTIMER:
		{
			if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
			{
				if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
				{
					m_dwLastSetupTimer=GetTickCount();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}
			}
			return 1;
		}
		break;
	case WM_NOTIFY:
		{
			switch (((LPNMHDR)lParam)->code)
			{
			case NM_SETFOCUS:
				{
					if (((LPNMHDR)lParam)->idFrom == IDC_EDIT_SETUP_BIRTHDAY)
					{
						if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
						{
							if(GetParent())
								GetParent()->PostMessage(WM_SET_SETUPTIMER);
						}
						if(m_pcWndKeyboard)
							m_pcWndKeyboard->SetEditWnd(NULL,KR_DAYS,IDC_EDIT_SETUP_BIRTHDAY);
					}
					return 1;
				}
				break; 
			case DTN_DATETIMECHANGE:
				{
					if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
					{
						if(GetParent())
							GetParent()->PostMessage(WM_SET_SETUPTIMER);
					}
					return 1;

				}
				break;
			}
		}
		break;
	default:
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

void CSubViewPatient::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.

	

	if(m_pEditPersID)
	{
		if(m_pEditPersID->IsWindowVisible())
		{
			m_pEditPersID->SetFocus();
			if(m_pcWndKeyboard)
				m_pcWndKeyboard->SetEditWnd(m_pEditPersID,KR_NUMBERS,IDC_EDIT_SETUP_PERSID);
		}
	}
	
	
}


//void CSubViewPatient::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
//	// CWnd::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
//	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
//}

//************************************
// Method:    OnDestroy
// FullName:  CSubViewPatient::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CSubViewPatient::OnDestroy()
{
	if(m_pDlg)
	{
		m_pDlg->EndDialog(IDCLOSE);
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_REDRAW_VIEW);
	}
	m_pDlg = NULL;

	DestroyWndKeyboard();

	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}



void CSubViewPatient::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	
	int tc=SetTextColor(hdcMem,0x0000000);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf11AcuBold);
	CBrush cbrWhite(RGB(255,255,255));
	CBrush cbrBack(RGB(181,178,181));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);

	//Rectangle(hdcMem, 5, 5, m_lX, m_lY);
	BitBlt(hdcMem, 0, 0, m_lX, m_lY, m_hDC, 0, 0, SRCCOPY);

	if(m_pcPatientData)
		m_pcPatientData->Draw(hdcMem,5,5);

	CStringW cs = _T("");

	if(m_bChangeMode)
	{
		cs = getModel()->GetLanguageString(IDS_TXT_PERSID)+_T(":");//_T("Patienten-ID:");
		rc.left = 60;//450;  
		rc.top = 20;  
		rc.right  = 170;//580;  
		rc.bottom = 40;
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		cs = getModel()->GetLanguageString(IDS_TXT_WEIGHT)+_T(":");
		rc.left = 450;  
		rc.top = 20;//60;  
		rc.right  = 580;  
		rc.bottom = 40;//100;
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


		cs = getModel()->GetLanguageString(IDS_TXT_NAME)+_T(":");//_T("Name:");
		rc.left = 30;  
		rc.top = 60;//20;  
		rc.right  = 170;  
		rc.bottom = 100;//40
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		cs = getModel()->GetLanguageString(IDS_TXT_FIRSTNAME)+_T(":");//_T("Vorame:");
		rc.left = 450;//30;  
		rc.top = 60;  
		rc.right  = 580;//180;  
		rc.bottom = 100;//400;
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		
		
		cs = getModel()->GetLanguageString(IDS_TXT_BIRTH)+_T(":");
		rc.left = 30;  
		rc.top = 100;  
		rc.right  = 180;  
		rc.bottom = 400;
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		cs = getModel()->GetLanguageString(IDS_TXT_REMARK)+_T(":");
		rc.left = 30;  
		rc.top = 150;  
		rc.right  = 180;  
		rc.bottom = 400;
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		SelectObject(hdcMem,g_hf7AcuNorm);

		cs = getModel()->GetLanguageString(IDS_TXT_DDMMYY);//_T("[Tag.Monat.Jahr]");
		rc.left = 190;  
		rc.top = 120;  
		rc.right  = 800;  
		rc.bottom = 160;
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


		if(m_pEditName)
			m_pEditName->SetWindowText(getModel()->getCONFIG()->GetPatientName());
		if(m_pEditFirstname)
			m_pEditFirstname->SetWindowText(getModel()->getCONFIG()->GetPatientFirstname());
		
		CStringW szPatID=_T("");
		szPatID.Format(_T("%d"), getModel()->getCONFIG()->GetPatientID());
		if(m_pEditPersID)
			m_pEditPersID->SetWindowText(szPatID);

		CStringW szWeight=_T("");
		szWeight.Format(_T("%d"), getModel()->getCONFIG()->GetPatientWeight());
		if(m_pEditWeight)
			m_pEditWeight->SetWindowText(szWeight);

		CStringW szTime = getModel()->getCONFIG()->GetPatientBirthday();
		COleDateTime dtBirthday;
		CStringW szDay=_T("");
		CStringW szMonth=_T("");
		CStringW szYear=_T("");

		if(szTime != _T(""))
		{
			if(true==dtBirthday.ParseDateTime(szTime,VAR_DATEVALUEONLY))
			{
				szDay.Format(_T("%02d"),dtBirthday.GetDay());
				szMonth.Format(_T("%02d"),dtBirthday.GetMonth());
				szYear.Format(_T("%04d"),dtBirthday.GetYear());
			}
		}

		//m_pCtrl->SetTime(dtBirthday);
		if(m_pEditBirthDay)
			m_pEditBirthDay->SetWindowText(szDay);
		if(m_pEditBirthMonth)
			m_pEditBirthMonth->SetWindowText(szMonth);
		if(m_pEditBirthYear)
			m_pEditBirthYear->SetWindowText(szYear);

		if(m_pEditRemark)
			m_pEditRemark->SetWindowText(getModel()->getCONFIG()->GetPatientRemark());
	}
	else
	{
		SelectObject(hdcMem,cbrWhite);

		//****************************PATIENT-ID*********************************//
		cs = getModel()->GetLanguageString(IDS_TXT_PERSID)+_T(":");//_T("Patienten-ID:");
		rc.left = 60;//450;  
		rc.top = 20;  
		rc.right  = 170;//580;  
		rc.bottom = 40;
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 175;//575;  
		rc.top = 15;  
		rc.right  = 430;//750;  
		rc.bottom = 40;
		FillRect(hdcMem,&rc,cbrWhite);


		/*cs = getModel()->getCONFIG()->GetPatientID();
		if(cs==_T(""))
			cs=_T("---");*/
		cs.Format(_T("%d"),getModel()->getCONFIG()->GetPatientID());
		rc.left = 180;//580;  
		rc.top = 20;  
		rc.right  = 800;  
		rc.bottom = 40;
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		//****************************WEIGHT*********************************//
		cs = getModel()->GetLanguageString(IDS_TXT_WEIGHT)+_T(":");
		rc.left = 450;  
		rc.top = 20;//60;  
		rc.right  = 580;  
		rc.bottom = 40;//100;
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


		rc.left = 575;  
		rc.top = 15;//55;  
		rc.right  = 750;  
		rc.bottom = 40;//85;
		FillRect(hdcMem,&rc,cbrWhite);

		cs.Format(_T("%d"),getModel()->getCONFIG()->GetPatientWeight());
		rc.left = 580;  
		rc.top = 20;//60;  
		rc.right  = 800;  
		rc.bottom = 40;//100;
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		//****************************NAME*********************************//
		cs = getModel()->GetLanguageString(IDS_TXT_NAME)+_T(":");//_T("Name:");
		rc.left = 30;  
		rc.top = 60;//20;  
		rc.right  = 170;  
		rc.bottom = 100;//40
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 175;  
		rc.top = 55;//15;  
		rc.right  = 430;  
		rc.bottom = 80;//40;
		FillRect(hdcMem,&rc,cbrWhite);

		cs = getModel()->getCONFIG()->GetPatientName();
		if(cs==_T(""))
			cs=_T("---");
		rc.left = 180;  
		rc.top = 60;//20;  
		rc.right  = 800;  
		rc.bottom = 100;//40;
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		//****************************FIRTS NAME*********************************//
		cs = getModel()->GetLanguageString(IDS_TXT_FIRSTNAME)+_T(":");//_T("Vorame:");
		rc.left = 450;//30;  
		rc.top = 60;  
		rc.right  = 580;//180;  
		rc.bottom = 100;//400;
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 575;//175;  
		rc.top = 55;  
		rc.right  = 750;//430;  
		rc.bottom = 80;
		FillRect(hdcMem,&rc,cbrWhite);

		cs = getModel()->getCONFIG()->GetPatientFirstname();
		if(cs==_T(""))
			cs=_T("---");
		rc.left = 580;//180;  
		rc.top = 60;  
		rc.right  = 800;  
		rc.bottom = 100;//400;
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		//****************************BIRTH*********************************//
		cs = getModel()->GetLanguageString(IDS_TXT_BIRTH)+_T(":");
		rc.left = 30;  
		rc.top = 100;  
		rc.right  = 180;  
		rc.bottom = 400;
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		CStringW szTime = getModel()->getCONFIG()->GetPatientBirthday();
		COleDateTime dtBirthday;
		CStringW szBirthday=_T("");

		if(szTime != _T(""))
		{
			if(true==dtBirthday.ParseDateTime(szTime,VAR_DATEVALUEONLY))
			{
				szBirthday.Format(_T("%02d.%02d.%04d"),dtBirthday.GetDay(),dtBirthday.GetMonth(),dtBirthday.GetYear());
			}
		}

		if(szBirthday==_T(""))
			szBirthday=_T("-- : -- : ----");

		rc.left = 175;  
		rc.top = 95;  
		rc.right  = 300;  
		rc.bottom = 120;
		FillRect(hdcMem,&rc,cbrWhite);

		rc.left = 180;  
		rc.top = 100;  
		rc.right  = 800;  
		rc.bottom = 400;
		DrawText(hdcMem,szBirthday,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		
		//****************************REMARK*********************************//
		cs = getModel()->GetLanguageString(IDS_TXT_REMARK)+_T(":");
		rc.left = 30;  
		rc.top = 150;  
		rc.right  = 180;  
		rc.bottom = 400;
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


		rc.left = 175;  
		rc.top = 145;  
		rc.right  = 750;  
		rc.bottom = 220;
		FillRect(hdcMem,&rc,cbrWhite);

		cs = getModel()->getCONFIG()->GetPatientRemark();
		if(cs==_T(""))
			cs=_T("---");

		rc.left = 180;  
		rc.top = 150;  
		rc.right  = 750;  
		rc.bottom = 220;
		DrawText(hdcMem,cs,-1,&rc,DT_TOP|DT_WORDBREAK|DT_LEFT);


		//****************************BIRTH SUBTEXT*********************************//
		SelectObject(hdcMem,g_hf7AcuNorm);

		cs = getModel()->GetLanguageString(IDS_TXT_DDMMYY);//_T("[Tag.Monat.Jahr]");
		rc.left = 190;  
		rc.top = 120;  
		rc.right  = 800;  
		rc.bottom = 160;
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);



	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	//DeleteObject(cbrBack);
	//DeleteObject(cbrWhite);

	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

}

// **************************************************************************
// 
// **************************************************************************
bool CSubViewPatient::CreateWndKeyboard()
{
	if(m_pcWndKeyboard==NULL)
	{


		m_pcWndKeyboard = new CWndSetupKeyboard();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={5,290,795,470};
		//RECT rcLd={130,290,670,470};
		if(m_pcWndKeyboard->Create(this,rcLd,IDC_VIEW_KEYBOARD))
		{


			return true;
		}


	}
	return false;
}
bool CSubViewPatient::DestroyWndKeyboard()
{
	if(m_pcWndKeyboard)
	{
		m_pcWndKeyboard->DestroyWindow();
		delete m_pcWndKeyboard;
		m_pcWndKeyboard=NULL;
	}
	return false;
}
void CSubViewPatient::ShowWndKeyboard(bool show)
{
	if(m_pcWndKeyboard)
		m_pcWndKeyboard->Show(show);


}


void CSubViewPatient::OnBnClickedToday()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	COleDateTime dtBirthday(st);
	CStringW szDay=_T("");
	CStringW szMonth=_T("");
	CStringW szYear=_T("");

	szDay.Format(_T("%02d"),dtBirthday.GetDay());
	szMonth.Format(_T("%02d"),dtBirthday.GetMonth());
	szYear.Format(_T("%04d"),dtBirthday.GetYear());

	if(m_pEditBirthDay)
		m_pEditBirthDay->SetWindowText(szDay);
	if(m_pEditBirthMonth)
		m_pEditBirthMonth->SetWindowText(szMonth);
	if(m_pEditBirthYear)
		m_pEditBirthYear->SetWindowText(szYear);

	PatientDataChanged();
}

void CSubViewPatient::OnBnClickedReset()
{
	getModel()->getCONFIG()->SetPatientData(_T(""), _T(""), 0, 0, _T(""), _T(""));

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_PATDATA_CHANGED);

	Draw();

	if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	else
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_NUMERIC);
	}

	
}

void CSubViewPatient::OnBnClickedApply()
{
	CStringW name=_T("");
	CStringW firstName=_T("");
	CStringW persID=_T("");
	CStringW szWeight=_T("");
	//SYSTEMTIME systime;
	COleDateTime dtBirthday;
	CStringW strDay=_T("");
	CStringW strMonth=_T("");
	CStringW strYear=_T("");
	CStringW strTime=_T("");
	CStringW strRemark=_T("");

	if(m_bChangeMode)
	{
		if(m_pEditName)
			m_pEditName->GetWindowText(name);
		if(m_pEditFirstname)
			m_pEditFirstname->GetWindowText(firstName);
		if(m_pEditPersID)
			m_pEditPersID->GetWindowText(persID);
		int ipersID=_wtoi(persID);
		if(m_pEditWeight)
			m_pEditWeight->GetWindowText(szWeight);
		int iWeight=_wtoi(szWeight);
		if(m_pEditBirthDay)
			m_pEditBirthDay->GetWindowText(strDay);
		if(m_pEditBirthMonth)
			m_pEditBirthMonth->GetWindowText(strMonth);
		if(m_pEditBirthYear)
			m_pEditBirthYear->GetWindowText(strYear);
		if(m_pEditRemark)
			m_pEditRemark->GetWindowText(strRemark);

		

		CStringW szDay=_T("");
		CStringW szMonth=_T("");
		CStringW szYear=_T("");

		CStringW sData = _T("");

		if((strDay != _T("")) || (strMonth != _T("")) || (strYear != _T("")))
		{
			if(_wtoi(strYear)<1000)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ERR_YEAR);

				PostMessage(WM_INVALID_YEAR);

				if(m_pcAply)
					m_pcAply->SetBitmaps(m_pc_Up,m_pc_Dw,m_pc_Dis,m_pc_Up);
				m_bPatientDataChanged=false;

				if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
				{
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}

				return;
			}
			else if(dtBirthday.SetDate(_wtoi(strYear), _wtoi(strMonth), _wtoi(strDay))!=0)
			{
				bool bError=false;
				int iMonth=_wtoi(strMonth);
				int iDay=_wtoi(strDay);

				if(iMonth<1 || iMonth>12)
				{
					PostMessage(WM_INVALID_MONTH);
					bError=true;
				}

				if(iDay<1 || iDay>31)
				{
					PostMessage(WM_INVALID_DAY);
					bError=true;
				}
				if(!bError)
				{
					PostMessage(WM_INVALID_DATE);
				}

				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ERR_DATE);

				if(m_pcAply)
					m_pcAply->SetBitmaps(m_pc_Up,m_pc_Dw,m_pc_Dis,m_pc_Up);
				m_bPatientDataChanged=false;

				if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
				{
					if(GetParent())
						GetParent()->PostMessage(WM_SET_SETUPTIMER);
				}

				return;
			}
			strTime = dtBirthday.Format(VAR_DATEVALUEONLY);

			szDay.Format(_T("%02d"),dtBirthday.GetDay());
			szMonth.Format(_T("%02d"),dtBirthday.GetMonth());
			szYear.Format(_T("%04d"),dtBirthday.GetYear());

		}


		//m_pCtrl->SetTime(dtBirthday);
		if(m_pEditBirthDay)
			m_pEditBirthDay->SetWindowText(szDay);
		if(m_pEditBirthMonth)
			m_pEditBirthMonth->SetWindowText(szMonth);
		if(m_pEditBirthYear)
			m_pEditBirthYear->SetWindowText(szYear);

		//if((name==_T("")) && (persID==_T("")))
		if(ipersID==0)
		{
			PostMessage(WM_NO_PERSID);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ERR_PERSID);

			if(m_pcAply)
				m_pcAply->SetBitmaps(m_pc_Up,m_pc_Dw,m_pc_Dis,m_pc_Up);
			m_bPatientDataChanged=false;

			if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
			{
				if(GetParent())
					GetParent()->PostMessage(WM_SET_SETUPTIMER);
			}

			return;
		}


		getModel()->getCONFIG()->SetPatientData(name, firstName, ipersID, iWeight, strTime, strRemark);

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_PATDATA_CHANGED);

		m_bPatientDataChanged=false;
		if(m_pcAply)
			m_pcAply->SetBitmaps(m_pc_Up,m_pc_Dw,m_pc_Dis,m_pc_Up);


		if(m_pcToday)
			m_pcToday->ShowWindow(SW_HIDE);
		if(m_pEditName)
			m_pEditName->ShowWindow(SW_HIDE);
		if(m_pEditFirstname)
			m_pEditFirstname->ShowWindow(SW_HIDE);
		if(m_pEditBirthDay)
			m_pEditBirthDay->ShowWindow(SW_HIDE);
		if(m_pEditBirthMonth)
			m_pEditBirthMonth->ShowWindow(SW_HIDE);
		if(m_pEditBirthYear)
			m_pEditBirthYear->ShowWindow(SW_HIDE);
		if(m_pEditPersID)
			m_pEditPersID->ShowWindow(SW_HIDE);
		if(m_pEditWeight)
			m_pEditWeight->ShowWindow(SW_HIDE);
		if(m_pEditRemark)
			m_pEditRemark->ShowWindow(SW_HIDE);

		if(m_pcReset)
			m_pcReset->MoveWindow(100,300,160,48);
		if(m_pcChange)
			m_pcChange->MoveWindow(500,300,160,48);
		if(m_pcAply)
			m_pcAply->MoveWindow(300,300,160,48);

		//m_pcAply->ShowWindow(SW_HIDE);
		if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
		{
			if(m_pcAply)
				m_pcAply->EnableWindow(FALSE);
		}
		else
		{
			if(m_pcAply)
				m_pcAply->RefreshText(getModel()->GetLanguageString(IDS_TXT_STARTVENT));
		}

		if(m_pcChange)
			m_pcChange->RefreshText(getModel()->GetLanguageString(IDS_TXT_CHANGE));

		DestroyWndKeyboard();

		m_bChangeMode=false;

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_PATDATASAVE);

		Draw();

		if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
		{
			if(GetParent())
				GetParent()->PostMessage(WM_KILL_SETUPTIMER);
		}

		//CTlsMessageBox box(this,_T("Delete Trend data?"),_T("Do you want to delete the trend data of the previous patient?"),MB_YESNO);
		CDlgMessageBox box(this,getModel()->GetLanguageString(IDS_MSG_CAPTION_DELTRENDDATA),getModel()->GetLanguageString(IDS_MSG_DELTRENDDATA),MB_YESNO,IDB_MSG_DELTREND);
		m_pDlg = &box;
		int iRes=box.DoModal();
		m_pDlg = NULL;


		if(iRes==IDCLOSE)
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_DELETE_TRENDS);
			return;
		}
		else
		{
			UpdateWindow();
			Draw();

			if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
			{
				if(GetParent())
					GetParent()->PostMessage(WM_SET_SETUPTIMER);
			}

			if(iRes==IDYES)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_DELETE_TRENDS);
			}
		}

		

	}
	else
	{
		if(getModel()->getVIEWHANDLER()->getViewState()==VS_PATDATA)
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_NUMERIC);
		}

	}

	
}

void CSubViewPatient::PatientDataChanged()
{
	if(m_bPatientDataChanged==true)
		return;

	m_bPatientDataChanged=true;


	if(m_pcAply)
		m_pcAply->SetBitmaps(m_pc_Red,m_pc_Dw,m_pc_Dis,m_pc_Red);

	if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
	{
		if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
		{
			m_dwLastSetupTimer=GetTickCount();
			if(GetParent())
				GetParent()->PostMessage(WM_SET_SETUPTIMER);
		}
	}
}

void CSubViewPatient::OnBnClickedChange()
{
	if(m_bChangeMode)
	{
		DestroyWndKeyboard();

		m_bPatientDataChanged=false;
		if(m_pcAply)
			m_pcAply->SetBitmaps(m_pc_Up,m_pc_Dw,m_pc_Dis,m_pc_Up);

		if(m_pcToday)
			m_pcToday->ShowWindow(SW_HIDE);
		if(m_pEditName)
			m_pEditName->ShowWindow(SW_HIDE);
		if(m_pEditFirstname)
			m_pEditFirstname->ShowWindow(SW_HIDE);
		if(m_pEditBirthDay)
			m_pEditBirthDay->ShowWindow(SW_HIDE);
		if(m_pEditBirthMonth)
			m_pEditBirthMonth->ShowWindow(SW_HIDE);
		if(m_pEditBirthYear)
			m_pEditBirthYear->ShowWindow(SW_HIDE);
		if(m_pEditPersID)
			m_pEditPersID->ShowWindow(SW_HIDE);
		if(m_pEditWeight)
			m_pEditWeight->ShowWindow(SW_HIDE);
		if(m_pEditRemark)
			m_pEditRemark->ShowWindow(SW_HIDE);

		if(m_pcReset)
			m_pcReset->MoveWindow(100,300,160,48);
		if(m_pcChange)
			m_pcChange->MoveWindow(500,300,160,48);
		if(m_pcAply)
			m_pcAply->MoveWindow(300,300,160,48);

		if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
		{
			if(m_pcAply)
				m_pcAply->EnableWindow(FALSE);
		}
		else
		{
			if(m_pcAply)
				m_pcAply->RefreshText(getModel()->GetLanguageString(IDS_TXT_STARTVENT));
		}

		if(m_pcChange)
		{
			m_pcChange->RefreshText(getModel()->GetLanguageString(IDS_TXT_CHANGE));
			//m_pcChange->DrawDirectUp();
		}
		

		m_bChangeMode=false;
	}
	else
	{
		m_bChangeMode=true;
		
		CreateWndKeyboard();
		ShowWndKeyboard(true);

		if(m_pcToday)
			m_pcToday->ShowWindow(SW_SHOW);
		if(m_pEditName)
			m_pEditName->ShowWindow(SW_SHOW);
		if(m_pEditFirstname)
			m_pEditFirstname->ShowWindow(SW_SHOW);
		if(m_pEditBirthDay)
			m_pEditBirthDay->ShowWindow(SW_SHOW);
		if(m_pEditBirthMonth)
			m_pEditBirthMonth->ShowWindow(SW_SHOW);
		if(m_pEditBirthYear)
			m_pEditBirthYear->ShowWindow(SW_SHOW);
		if(m_pEditPersID)
			m_pEditPersID->ShowWindow(SW_SHOW);
		if(m_pEditWeight)
			m_pEditWeight->ShowWindow(SW_SHOW);
		if(m_pEditRemark)
			m_pEditRemark->ShowWindow(SW_SHOW);

		if(m_pcChange)
		{
			m_pcChange->RefreshText(getModel()->GetLanguageString(IDS_TXT_CANCEL));
			//m_pcChange->DrawDirectUp();
		}

		if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
		{
			if(m_pcAply)
				m_pcAply->EnableWindow(TRUE);
		}
		else
		{
			if(m_pcAply)
				m_pcAply->RefreshText(getModel()->GetLanguageString(IDS_TXT_APPLY));
		}

		if(m_pcReset)
			m_pcReset->MoveWindow(100,230,160,48);
		if(m_pcChange)
			m_pcChange->MoveWindow(500,230,160,48);
		if(m_pcAply)
			m_pcAply->MoveWindow(300,230,160,48);

		

		if(m_pEditPersID)
			m_pEditPersID->SetFocus();
	}
	

	Draw();

	if(getModel()->getVIEWHANDLER()->getViewState()!=VS_PATDATA)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}


}