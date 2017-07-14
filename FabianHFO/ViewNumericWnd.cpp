#include "StdAfx.h"
#include "ViewNumericWnd.h"
#include "MVViewHandler.h"

#define NUMAV_WND_ID			0x0124

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000

/**********************************************************************************************//**
 * @fn	CViewNumericWnd::CViewNumericWnd(int ViewID)
 *
 * @brief	Constructor.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @param	ViewID	Identifier for the view.
 **************************************************************************************************/
CViewNumericWnd::CViewNumericWnd(int ViewID):
CMVView(ViewID)
{
	InitializeCriticalSection(&csNumWnd);

	m_bExit=false;

	m_eNumBlock=NUMB_NONE;
	m_bBackState=false;
	
	m_iCurrWnd=0;

	m_nwa.nX=610;
	m_nwa.nY=41;

	m_nwa.nCx=190;
	m_nwa.nCy=559;

	m_pcWndNumConfig=NULL;

	m_pcMenuNumeric=NULL;
	m_pcNumeric_Up=NULL;
	m_pcNumeric_Dw=NULL;

	m_iCurNumericBlock=0;
	m_iCountNumericBlock=0;
}

/**********************************************************************************************//**
 * @fn	CViewNumericWnd::~CViewNumericWnd(void)
 *
 * @brief	Destructor.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
CViewNumericWnd::~CViewNumericWnd(void)
{
	if(m_pcMenuNumeric)
		delete m_pcMenuNumeric;
	m_pcMenuNumeric=NULL;
	if(m_pcNumeric_Up)
		delete m_pcNumeric_Up;
	m_pcNumeric_Up=NULL;
	if(m_pcNumeric_Dw)
		delete m_pcNumeric_Dw;
	m_pcNumeric_Dw=NULL;

	DeleteCriticalSection(&csNumWnd);
}


BEGIN_MESSAGE_MAP(CViewNumericWnd, CMVView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * @fn	bool CViewNumericWnd::CreateView()
 *
 * @brief	Creates the view.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @return	true if it succeeds, false if it fails.
 **************************************************************************************************/
bool CViewNumericWnd::CreateView()
{
	RECT rcLd={m_nwa.nX,m_nwa.nY,m_nwa.nX+m_nwa.nCx,m_nwa.nY+m_nwa.nCy};

	if (!Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_NUM))
	{
		return false;
	}
	Initialize();

	return true;
}

/**********************************************************************************************//**
 * @fn	BOOL CViewNumericWnd::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
 *
 * @brief	Creates the Window instance that will be represented by this object.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @param [in,out]	pParentWnd	If non-null, the parent window.
 * @param	rc				  	The rectangle.
 * @param	nID				  	The identifier.
 * @param [in,out]	pContext  	If non-null, the context.
 *
 * @return	true if it succeeds, false if it fails.
 **************************************************************************************************/
BOOL CViewNumericWnd::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

		CBrush cbrBack(BACKGND);
		//CBrush cbrBack(RGB(180,180,255));
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();//rkuNEWFIX

		return 1;
	}
	else
		return 0;
}

/**********************************************************************************************//**
 * @fn	bool CViewNumericWnd::Initialize()
 *
 * @brief	Initializes this object.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @return	true if it succeeds, false if it fails.
 **************************************************************************************************/
bool CViewNumericWnd::Initialize()
{
	CClientDC dc(this);

	BTN btn;

	m_pcNumeric_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUMERIC3_UP);
	m_pcNumeric_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUMERIC3_DW);

	btn.wID					= IDC_BTN_MENU_NUMERIC;	
	btn.poPosition.x		= 15;
	btn.poPosition.y		= 509;
	btn.pcBmpUp				= m_pcNumeric_Up;
	btn.pcBmpDown			= m_pcNumeric_Dw;
	btn.pcBmpFocus			= m_pcNumeric_Up;
	btn.pcBmpDisabled		= m_pcNumeric_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuNumeric=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuNumeric->Create(this,g_hf21AcuBold,0);
	m_pcMenuNumeric->SetText(_T(""));
	m_pcMenuNumeric->ShowWindow(SW_HIDE);

	Show();

	return true;
}

/**********************************************************************************************//**
 * @fn	void CViewNumericWnd::Open()
 *
 * @brief	Opens this view.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
void CViewNumericWnd::Open()
{
	Show();
}

/**********************************************************************************************//**
 * @fn	void CViewNumericWnd::Close()
 *
 * @brief	Closes this view.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
void CViewNumericWnd::Close()
{
	deleteWndNumConfig();
}

/**********************************************************************************************//**
 * @fn	void CViewNumericWnd::Show()
 *
 * @brief	Shows this view.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
void CViewNumericWnd::Show()
{
	SetNumericBlock(false);
}

/**********************************************************************************************//**
 * @fn	void CViewNumericWnd::Hide()
 *
 * @brief	Hides this view.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
void CViewNumericWnd::Hide()
{
	HideNumWnd();
}



/**********************************************************************************************//**
 * @fn	bool CViewNumericWnd::createWndNumConfig(UINT ID, NUMERICINI* pbufNumeric)
 *
 * @brief	Creates numeric window configuration.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 *
 * @param	ID				   	The identifier.
 * @param [in,out]	pbufNumeric	If non-null, the pbuf numeric.
 *
 * @return	true if it succeeds, false if it fails.
 **************************************************************************************************/
bool CViewNumericWnd::createWndNumConfig(UINT ID, NUMERICINI* pbufNumeric)
{
	if(m_pcWndNumConfig==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumConfig = new CWndNumConfig(pbufNumeric);

		RECT rcLd={0,0,210,560};
		if(m_pcWndNumConfig->Create(this,rcLd,ID))
		{
			m_pcWndNumConfig->init();
			m_iCurrWnd=ID;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

/**********************************************************************************************//**
 * @fn	void CViewNumericWnd::deleteWndNumConfig()
 *
 * @brief	Deletes the window numeric configuration.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
void CViewNumericWnd::deleteWndNumConfig()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumConfig)
	{
		m_pcWndNumConfig->DestroyWindow();
		delete m_pcWndNumConfig;
		m_pcWndNumConfig=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}

/**********************************************************************************************//**
 * @fn	void CViewNumericWnd::ShowWndNumHFO()
 *
 * @brief	Shows the window numeric hfo.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
void CViewNumericWnd::ShowWndNumHFO()
{
	if(getModel()->getDATAHANDLER()->getCountNumericHFO()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericHFO[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_HFO,&getModel()->getDATAHANDLER()->m_pbufNumericHFO[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}

void CViewNumericWnd::ShowWndNumFLOWOFFCONV()
{
	if(getModel()->getDATAHANDLER()->getCountNumericFLOWOFFCONV()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericFLOWOFFCONV[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_FLOWOFFCONV,&getModel()->getDATAHANDLER()->m_pbufNumericFLOWOFFCONV[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}
void CViewNumericWnd::ShowWndNumFLOWOFFCPAP()
{
	if(getModel()->getDATAHANDLER()->getCountNumericFLOWOFFCPAP()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericFLOWOFFCPAP[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_FLOWOFFCPAP,&getModel()->getDATAHANDLER()->m_pbufNumericFLOWOFFCPAP[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}
void CViewNumericWnd::ShowWndNumFLOWOFFHFO()
{
	if(getModel()->getDATAHANDLER()->getCountNumericFLOWOFFHFO()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericFLOWOFFHFO[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_FLOWOFFHFO,&getModel()->getDATAHANDLER()->m_pbufNumericFLOWOFFHFO[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}

/**********************************************************************************************//**
 * @fn	void CViewNumericWnd::ShowWndNumIPPV()
 *
 * @brief	Shows the window numeric ippv.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
void CViewNumericWnd::ShowWndNumIPPV()
{
	if(getModel()->getDATAHANDLER()->getCountNumericIPPV()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericIPPV[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_IPPV,&getModel()->getDATAHANDLER()->m_pbufNumericIPPV[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}

/**********************************************************************************************//**
 * @fn	void CViewNumericWnd::ShowWndNumSIPPV()
 *
 * @brief	Shows the window numeric sippv.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
void CViewNumericWnd::ShowWndNumSIPPV()
{
	if(getModel()->getDATAHANDLER()->getCountNumericSIPPV()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericSIPPV[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_SIPPV,&getModel()->getDATAHANDLER()->m_pbufNumericSIPPV[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}

/**********************************************************************************************//**
 * @fn	void CViewNumericWnd::ShowWndNumSIMV()
 *
 * @brief	Shows the window numeric simv.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
void CViewNumericWnd::ShowWndNumSIMV()
{
	if(getModel()->getDATAHANDLER()->getCountNumericSIMV()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericSIMV[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_SIMV,&getModel()->getDATAHANDLER()->m_pbufNumericSIMV[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}

/**********************************************************************************************//**
 * @fn	void CViewNumericWnd::ShowWndNumSIMVPSV()
 *
 * @brief	Shows the window numeric simvpsv.
 *
 * @author	Rainer Kuehner
 * @date	07.02.2017
 **************************************************************************************************/
void CViewNumericWnd::ShowWndNumSIMVPSV()
{
	if(getModel()->getDATAHANDLER()->getCountNumericSIMVPSV()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericSIMVPSV[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_SIMVPSV,&getModel()->getDATAHANDLER()->m_pbufNumericSIMVPSV[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumPSV()
{
	if(getModel()->getDATAHANDLER()->getCountNumericPSV()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericPSV[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_PSV,&getModel()->getDATAHANDLER()->m_pbufNumericPSV[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumCPAP()
{
	if(getModel()->getDATAHANDLER()->getCountNumericCPAP()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericCPAP[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_CPAP,&getModel()->getDATAHANDLER()->m_pbufNumericCPAP[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumDUOPAP()
{
	if(getModel()->getDATAHANDLER()->getCountNumericDUOPAP()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericDUOPAP[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_DUOPAP,&getModel()->getDATAHANDLER()->m_pbufNumericDUOPAP[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumNCPAP()
{
	if(getModel()->getDATAHANDLER()->getCountNumericNCPAP()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericNCPAP[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_NCPAP,&getModel()->getDATAHANDLER()->m_pbufNumericNCPAP[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumTHERAPY()
{
	if(getModel()->getDATAHANDLER()->getCountNumericTHERAPY()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericTHERAPY[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_THERAPY,&getModel()->getDATAHANDLER()->m_pbufNumericTHERAPY[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}


// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::HideNumWnd()
{
	deleteWndNumConfig();
}
#define DIV 1024
// #define DIV 1

const char *divisor = "K";
// char *divisor = "";
#define WIDTH 7

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowNumWnd(eNumBlock eBlock,bool bChangeCurNumBlock)
{
	if(m_eNumBlock!=eBlock || bChangeCurNumBlock)
		HideNumWnd();

	m_eNumBlock=eBlock;
	switch(eBlock)
	{
	case NUMB_IPPV: 
		{
			ShowWndNumIPPV();
		}
		break;
	case NUMB_SIPPV: 
		{
			ShowWndNumSIPPV();
		}
		break;
	case NUMB_SIMV: 
		{
			ShowWndNumSIMV();
		}
		break;
	case NUMB_SIMVPSV: 
		{
			ShowWndNumSIMVPSV();
		}
		break;
	case NUMB_PSV: 
		{
			ShowWndNumPSV();
		}
		break;
	case NUMB_CPAP:
		{
			ShowWndNumCPAP();
		}
		break;
	case NUMB_DUOPAP:
		{
			ShowWndNumDUOPAP();
		}
		break;
	case NUMB_NCPAP: 
		{
			ShowWndNumNCPAP();
		}
		break;
	case NUMB_THERAPY: 
		{
			ShowWndNumTHERAPY();
		}
		break;
	case NUMB_HFO: 
		{
			ShowWndNumHFO();
		}
		break;
	case NUMB_FLOWOFFCONV: 
		{
			ShowWndNumFLOWOFFCONV();
		}
		break;
	case NUMB_FLOWOFFCPAP: 
		{
			ShowWndNumFLOWOFFCPAP();
		}
		break;
	case NUMB_FLOWOFFHFO: 
		{
			ShowWndNumFLOWOFFHFO();
		}
		break;
	default:
		{
		}
		break;
	}
	
	if(getModel()->getVIEWHANDLER()->getViewState()==VS_TREND)
	{
		DeleteGraphCursor();
	}
}



// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::SetNumericBlock(bool bChangeCurNumBlock)
{
	eNumBlock eBlock=NUMB_IPPV;

	eVentMode eMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	switch(eMode)
	{
	case VM_IPPV:
		{
			if(getModel()->getDATAHANDLER()->GetFlowSensorState()!=FLOWSENSOR_ON)
			{
				eBlock=NUMB_FLOWOFFCONV;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericFLOWOFFCONV();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericFLOWOFFCONV();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericFLOWOFFCONV(m_iCurNumericBlock);
				}
			}
			else
			{
				eBlock=NUMB_IPPV;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericIPPV();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericIPPV();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericIPPV(m_iCurNumericBlock);
				}
			}
			
		}
		break;
	case VM_SIMV:
		{
			if(getModel()->getDATAHANDLER()->GetFlowSensorState()!=FLOWSENSOR_ON)
			{
				eBlock=NUMB_FLOWOFFCONV;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericFLOWOFFCONV();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericFLOWOFFCONV();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericFLOWOFFCONV(m_iCurNumericBlock);
				}
			}
			else
			{
				eBlock=NUMB_SIMV;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericSIMV();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericSIMV();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericSIMV(m_iCurNumericBlock);
				}
			}
			
		}
		break;
	case VM_SIMVPSV:
		{
			if(getModel()->getDATAHANDLER()->GetFlowSensorState()!=FLOWSENSOR_ON)
			{
				eBlock=NUMB_FLOWOFFCONV;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericFLOWOFFCONV();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericFLOWOFFCONV();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericFLOWOFFCONV(m_iCurNumericBlock);
				}
			}
			else
			{
				eBlock=NUMB_SIMVPSV;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericSIMVPSV();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericSIMVPSV();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericSIMVPSV(m_iCurNumericBlock);
				}
			}
			
		}
		break;
	case VM_SIPPV:
		{
			if(getModel()->getDATAHANDLER()->GetFlowSensorState()!=FLOWSENSOR_ON)
			{
				eBlock=NUMB_FLOWOFFCONV;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericFLOWOFFCONV();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericFLOWOFFCONV();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericFLOWOFFCONV(m_iCurNumericBlock);
				}
			}
			else
			{
				eBlock=NUMB_SIPPV;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericSIPPV();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericSIPPV();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericSIPPV(m_iCurNumericBlock);
				}
			}
			
		}
		break;
	case VM_PSV:
		{
			if(getModel()->getDATAHANDLER()->GetFlowSensorState()!=FLOWSENSOR_ON)
			{
				eBlock=NUMB_FLOWOFFCONV;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericFLOWOFFCONV();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericFLOWOFFCONV();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericFLOWOFFCONV(m_iCurNumericBlock);
				}
			}
			else
			{
				eBlock=NUMB_PSV;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericPSV();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericPSV();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericPSV(m_iCurNumericBlock);
				}
			}
			
		}
		break;
	case VM_CPAP:
		{
			if(getModel()->getDATAHANDLER()->GetFlowSensorState()!=FLOWSENSOR_ON)
			{
				eBlock=NUMB_FLOWOFFCPAP;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericFLOWOFFCPAP();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericFLOWOFFCPAP();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericFLOWOFFCPAP(m_iCurNumericBlock);
				}
			}
			else
			{
				eBlock=NUMB_CPAP;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericCPAP();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericCPAP();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericCPAP(m_iCurNumericBlock);
				}
			}
		}
		break;
	case VM_HFO:
		{
			if(getModel()->getDATAHANDLER()->GetFlowSensorState()!=FLOWSENSOR_ON)
			{
				eBlock=NUMB_FLOWOFFHFO;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericFLOWOFFHFO();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericFLOWOFFHFO();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericFLOWOFFHFO(m_iCurNumericBlock);
				}
			}
			else
			{
				eBlock=NUMB_HFO;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericHFO();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericHFO();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericHFO(m_iCurNumericBlock);
				}
			}
			
		}
		break;
	case VM_NCPAP:
		{
			eBlock=NUMB_NCPAP;
			m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericNCPAP();
			m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericNCPAP();

			if(m_iCurNumericBlock>m_iCountNumericBlock-1)
			{
				m_iCurNumericBlock=0;
				getModel()->getCONFIG()->setLastNumericNCPAP(m_iCurNumericBlock);
			}
		}
		break;
	case VM_DUOPAP:
		{
			eBlock=NUMB_DUOPAP;
			m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericDUOPAP();
			m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericDUOPAP();

			if(m_iCurNumericBlock>m_iCountNumericBlock-1)
			{
				m_iCurNumericBlock=0;
				getModel()->getCONFIG()->setLastNumericDUOPAP(m_iCurNumericBlock);
			}
		}
		break;
	case VM_THERAPIE:
		{
			eBlock=NUMB_THERAPY;
			m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericTHERAPY();
			m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericTHERAPY();

			if(m_iCurNumericBlock>m_iCountNumericBlock-1)
			{
				m_iCurNumericBlock=0;
				getModel()->getCONFIG()->setLastNumericTHERAPY(m_iCurNumericBlock);
			}
		}
		break;
	default:
		{
			return;
		}
		break;
	}

	ShowNumWnd(eBlock,bChangeCurNumBlock);
}


// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::NotifyEvent(CMVEvent* pEvent)
{
	if(m_bExit)
		return;

	switch(pEvent->GetET())
	{
	case CMVEvent::ET_CONTROL:
		{
			CMVEventControl* pCtrlEvent = (CMVEventControl*)pEvent;
			switch(pCtrlEvent->GetEventType())
			{
			case CMVEventControl::EV_CONTROL_FLOWSENSORSTATE:
			case CMVEventControl::EV_CONTROL_O2SENSORSTATE:
				{
					try
					{
						SetNumericBlock(false);
					}
					catch (...)
					{
						theApp.ReportException(_T("CViewNumericWnd::NotifyEvent"));
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case CMVEvent::ET_UIEVENT:
		{
			CMVEventUI* pUIEvent = (CMVEventUI*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventUI::EV_BN_NUMERIC:
				{
					ShowNextNumericWnd();
				}
				break;
			case CMVEventUI::EV_GRAPH_SETCURSOR:
				{
					DrawGraphCursor(pUIEvent->GetIData());
				}
				break;
			case CMVEventUI::EV_GRAPH_DELCURSOR:
				{
					DeleteGraphCursor();
				}
				break;
			default:
				break;
			}
		}
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowNextNumericWnd()
{

}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::DrawGraphCursor(int iDiagrmm)
{
	bool bMoveXscale=false;
	bool bMoveYscale=false;

	if(getModel()->getVIEWHANDLER()->getViewState()==VS_TREND)
	{

	}
	else
	{
		if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
		{
			switch(iDiagrmm)
			{
			case IDC_LINEDIAGRAM_PRESSURE:
				{
					bMoveXscale=true;
				}
				break;
			case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
				{
					bMoveYscale=true;
				}
				break;
			}
		}
	}

	try
	{
		EnterCriticalSection(&csNumWnd);
		if(m_pcWndNumConfig)
		{
			m_pcWndNumConfig->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		LeaveCriticalSection(&csNumWnd);
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewNumericWnd::DrawGraphCursor"));
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::DeleteGraphCursor()
{
	try
	{
		EnterCriticalSection(&csNumWnd);
		if(m_pcWndNumConfig)
		{
			m_pcWndNumConfig->SetGraphCursor(false,false,false);
		}
		LeaveCriticalSection(&csNumWnd);
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewNumericWnd::DeleteGraphCursor"));
	}
}


// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::NotifyParaBtnEvent(CMVEvent* pEvent)
{
	if(m_bExit)
		return;

	switch(pEvent->GetET())
	{
	case CMVEvent::ET_UIEVENT:
		{
			CMVEventUI* pUIEvent = (CMVEventUI*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventUI::EV_PARABN_PMITT:
			//case CMVEventUI::EV_PARABN_IERATIO:
			//case CMVEventUI::EV_PARABN_HFFREQREC:
				{
					bool bChanged=false;
					if(getModel()->getALARMHANDLER()->getAlimitState_MAPminLimit()==AL_AUTO)
					{
						bChanged=true;
					}
					if(getModel()->getALARMHANDLER()->getAlimitState_MAPmaxLimit()==AL_AUTO)
					{
						bChanged=true;
					}
					if(bChanged)
					{	
						PostMessage(WM_ALIMIT_CHANGED);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_HFAMPL:
				{
					bool bChanged=false;
					/*if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
					{
						bChanged=true;
					}*/
					if(true==getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
					{
						bChanged=true;
					}
					/*else if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
					{
						bChanged=true;
					}*/
					if(bChanged)
					{	
						PostMessage(WM_ALIMIT_CHANGED);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_PEEP:
				{
					if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
					{
						PostMessage(WM_ALIMIT_CHANGED);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_CPAP:
				{
					bool bLimitChanged=false;
					if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
					{
						bLimitChanged=true;
					}
					if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
					{
						bLimitChanged=true;
					}
					if(bLimitChanged)
					{
						PostMessage(WM_ALIMIT_CHANGED);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_PPSV:
			case CMVEventUI::EV_PARABN_PINSP:
			case CMVEventUI::EV_PARABN_PMAXVOLG:
				{
					bool bLimitChanged=false;
					if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
					{
						bLimitChanged=true;
					}
					if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
					{
						bLimitChanged=true;
					}
					if(bLimitChanged)
					{
						PostMessage(WM_ALIMIT_CHANGED);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_PDUO:
				{
					if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
					{
						PostMessage(WM_ALIMIT_CHANGED);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_PMAN:
				{
					if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
					{
						PostMessage(WM_ALIMIT_CHANGED);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_FOT:
				{
					bool bChanged=false;
					if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
					{
						bChanged=true;
					}
					if(true==getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
					{
						bChanged=true;
					}
					else if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
					{
						bChanged=true;
					}
					if(bChanged)
					{	
						PostMessage(WM_ALIMIT_CHANGED);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_IFLOW:	
			case CMVEventUI::EV_PARABN_EFLOW:
			case CMVEventUI::EV_PARABN_VLIMIT:
			case CMVEventUI::EV_PARABN_TRIGGER:
			case CMVEventUI::EV_PARABN_BPM:
			case CMVEventUI::EV_PARABN_ITIME:
			case CMVEventUI::EV_PARABN_ETIME:
			case CMVEventUI::EV_PARABN_BACKUP:
			case CMVEventUI::EV_PARABN_O2:
			case CMVEventUI::EV_PARABN_MINFLOW:
			case CMVEventUI::EV_PARABN_VGARANT:
			case CMVEventUI::EV_PARABN_O2FLUSH:
			case CMVEventUI::EV_PARABN_HFFREQ:
			//case CMVEventUI::EV_PARABN_HFAMPL:
			//case CMVEventUI::EV_PARABN_IERATIO:
			default:
				break;
			}
		}

	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::NotifyViewStateChanged()
{
	if(m_bExit)
		return;

	switch(getModel()->getVIEWHANDLER()->getViewState())
	{
	case VS_SETUP:
		{
			m_bBackState=true;
		}
		break;
	case VS_PARA:
		{
			if(getModel()->getVIEWHANDLER()->getPrevViewState()==VS_SETUP)
			{
				if(m_bBackState)
				{
					m_bBackState=false;
				}
				break;
			}

			if(m_bBackState)
			{
				m_bBackState=false;
			}
		}
		break;
	case VS_GRAPH:
	case VS_TREND:
		{
			
		}
		break;
	/*case VS_TREND:
		{
			
		}
		break;*/
	default:
		{
			if(m_bBackState)
			{
				m_bBackState=false;
			}
		}
		break;
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::NotifyVentModeChanged()
{
	if(m_bExit)
		return;

	switch(getModel()->getVIEWHANDLER()->getViewState())
	{
	case VS_PARA:
		{
			if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
			{
				try
				{
					SetNumericBlock(false);
				}
				catch (...)
				{
					theApp.ReportException(_T("CViewNumericWnd::NotifyVentModeChanged"));
				}
			}
		}
		break;
	default:
		{
		}
		break;
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::CalculateAlarmLimit()
{
	if(m_bExit)
		return;

	redrawNumerics(true,true,true,true,false);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::NotifyCalculateAlarmLimit()
{
	if(m_bExit)
		return;

	PostMessage(WM_CALCULATE_ALARMLIMIT);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::RedrawAlarmLimits()
{
	if(m_bExit)
		return;

	redrawNumerics(true,true,true,true,false);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::NotifyAlarmLimitChanged()
{
	if(m_bExit)
		return;
	
	PostMessage(WM_ALIMIT_CHANGED);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::NotifyDataChanged()
{
	if(m_bExit)
		return;
	
	PostMessage(WM_DATA_CHANGED);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::NotifyExspirationDataChanged()
{
	if(m_bExit)
		return;

	PostMessage(WM_EXSPIRATION);
}

// **************************************************************************
//
// 
//
// **************************************************************************
LRESULT CViewNumericWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_SETVIEWFOCUS:
		{
			//rku cs1
			SetViewFocus();
			return 1;
		}
		break;
	case WM_SETPREVFOCUS:
		{
			//rku cs1
			SetPrevFocus();
			return 1;
		}
		break;
	case WM_SETNEXTFOCUS:
		{
			//rku cs1
			SetNextFocus();
			return 1;
		}
		break;
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_MENU_NUMERIC:
				{
					BnMenuNUMERIC();
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
	case WM_CALCULATE_ALARMLIMIT:
		{
			CalculateAlarmLimit();
			return 1;
		}
		break;
	case WM_ALIMIT_CHANGED:
		{
			RedrawAlarmLimits();
			return 1;
		}
		break;
	case WM_EXSPIRATION:
		{
			redrawNumerics(true,false, false, false,false);
			return 1;
		}
		break;
	case WM_DATA_CHANGED:
		{
			redrawNumerics(true,false, false, false,true);
			return 1;
		}
		break;
	default:
		{
		}
		break;
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}

void CViewNumericWnd::redrawNumerics(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
{
	EnterCriticalSection(&csNumWnd);
	switch(m_iCurrWnd)
	{
	case IDC_VIEW_NUM_NCPAP:
	case IDC_VIEW_NUM_DUOPAP:
	case IDC_VIEW_NUM_THERAPY:
	case IDC_VIEW_NUM_HFO:
	case IDC_VIEW_NUM_CPAP:
	case IDC_VIEW_NUM_PSV:
	case IDC_VIEW_NUM_SIMVPSV:
	case IDC_VIEW_NUM_SIMV:
	case IDC_VIEW_NUM_SIPPV:
	case IDC_VIEW_NUM_IPPV:
	case IDC_VIEW_NUM_FLOWOFFCONV:
	case IDC_VIEW_NUM_FLOWOFFCPAP:
	case IDC_VIEW_NUM_FLOWOFFHFO:
		{
			if(m_pcWndNumConfig)
				m_pcWndNumConfig->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}


//************************************
// Method:    OnDestroy
// FullName:  CViewNumericWnd::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CViewNumericWnd::OnDestroy()
{
	m_bExit=true;
	
	deleteWndNumConfig();

	m_iCurrWnd=0;

	CMVView::OnDestroy();
}


void CViewNumericWnd::BnMenuNUMERIC()
{
	bool bChangeCurNumBlock=false;
	CString szText=_T("");
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_DEL_GRAPHCURSOR);

	switch(m_iCurrWnd)
	{
	case IDC_VIEW_NUM_IPPV:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericIPPV())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericIPPV(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_SIPPV:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericSIPPV())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericSIPPV(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_SIMV:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericSIMV())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericSIMV(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_SIMVPSV:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericSIMVPSV())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericSIMVPSV(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_PSV:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericPSV())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericPSV(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_CPAP:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericCPAP())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericCPAP(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_HFO:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericHFO())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericHFO(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_NCPAP:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericNCPAP())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericNCPAP(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_DUOPAP:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericDUOPAP())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericDUOPAP(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_THERAPY:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericTHERAPY())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericTHERAPY(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_FLOWOFFCONV:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericFLOWOFFCONV())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericFLOWOFFCONV(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_FLOWOFFCPAP:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericFLOWOFFCPAP())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericFLOWOFFCPAP(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_FLOWOFFHFO:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericFLOWOFFHFO())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericFLOWOFFHFO(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	}
	SetNumericBlock(bChangeCurNumBlock);

	AfxGetApp()->GetMainWnd()->SetFocus();
}

