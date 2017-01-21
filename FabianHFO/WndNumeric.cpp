// WndNumeric.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndNumeric.h"


#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000

// CWndNumeric

IMPLEMENT_DYNAMIC(CWndNumeric, CWnd)

CWndNumeric::CWndNumeric()
{
	InitializeCriticalSection(&csDraw);
	
	m_pModel=NULL;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;

	m_hdcStatic=NULL;
	m_hbmpStatic=NULL;

	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;

	/*m_pcWait=NULL;
	m_pcWait1= NULL;
	m_pcWait2= NULL;
	m_pcWait3= NULL;
	m_pcWait4= NULL;
	m_pcWait5= NULL;
	m_pcWait6= NULL;
	m_pcWait7= NULL;
	m_pcWait8= NULL;
	m_iWaitCount=0;*/

	m_pcFlowKegelInsp=NULL;
	m_pcFlowKegelExpp=NULL;
	m_pcFlowKegel_0=NULL;
	m_pcFlowKegel_1=NULL;
	m_pcFlowKegel_2=NULL;
	m_pcFlowKegel_3=NULL;
	m_pcFlowKegel_4=NULL;
	m_pcFlowKegel_5=NULL;
	m_pcFlowKegel_6=NULL;
	m_pcFlowKegel_7=NULL;
	m_pcFlowKegel_8=NULL;
	m_pcFlowKegel_9=NULL;
	m_iFlowKegelInsp=9;
	m_iFlowKegelExsp=0;

	m_pcWarningYellow=NULL;

	m_pcNumPara1er=NULL;
	m_pcNumPara2er=NULL;
	m_pcNumPara3er=NULL;
	//m_pcNumHeader=NULL;
	m_pcLimitHigh_Up=NULL;
	m_pcLimitLow_Up=NULL;

	m_pcGraphCursor_left_up=NULL;
	m_pcGraphCursor_left_dw=NULL;
	m_pcGraphCursor_right_up=NULL;
	m_pcGraphCursor_right_dw=NULL;
	m_pcGraphCursor_up_up=NULL;
	m_pcGraphCursor_up_dw=NULL;
	m_pcGraphCursor_down_up=NULL;
	m_pcGraphCursor_down_dw=NULL;
	m_pcGraphCursor_pic=NULL;
	/*m_pcMediJet=NULL;
	m_pcInfantFlow=NULL;
	m_pcInfantFlowLP=NULL;*/

	m_pcXmoveDown_up=NULL;
	m_pcXmoveDown_dw=NULL;
	m_pcXmoveUp_up=NULL;
	m_pcXmoveUp_dw=NULL;
	m_pcYmoveLeft_up=NULL;
	m_pcYmoveLeft_dw=NULL;
	m_pcYmoveRight_up=NULL;
	m_pcYmoveRight_dw=NULL;
	m_pcMoveXscale_pic=NULL;
	m_pcMoveYscale_pic=NULL;


	m_pcNumInfoTop= NULL;
	m_pcNumInfoBot= NULL;
	m_pcNumericTop= NULL;
	m_pcNumericBot= NULL;

	m_pcFlowmeterTherapy=NULL;
	m_pcFlowmeterLeft= NULL;
	m_pcFlowmeterRight= NULL;


	m_bGraphCursor=false;
	m_bMoveXscale=false;
	m_bMoveYscale=false;
	
	

	m_pbtnCursorUp=NULL;
	m_pbtnCursorDown=NULL;
	m_pbtnCursorLeft=NULL;
	m_pbtnCursorRight=NULL;

	m_pbtnXmoveUp=NULL;
	m_pbtnXmoveDown=NULL;
	m_pbtnYmoveLeft=NULL;
	m_pbtnYmoveRight=NULL;

	m_valueFlowInsp=374;
	m_valueFlowExsp=374;
}

CWndNumeric::~CWndNumeric()
{
	if(m_pcWarningYellow)
		delete m_pcWarningYellow;
	m_pcWarningYellow=NULL;
	

	if(m_pcFlowKegelInsp)
		delete m_pcFlowKegelInsp;
	m_pcFlowKegelInsp=NULL;

	if(m_pcFlowKegelExpp)
		delete m_pcFlowKegelExpp;
	m_pcFlowKegelExpp=NULL;

	if(m_pcFlowKegel_0)
		delete m_pcFlowKegel_0;
	m_pcFlowKegel_0=NULL;
	if(m_pcFlowKegel_1)
		delete m_pcFlowKegel_1;
	m_pcFlowKegel_1=NULL;
	if(m_pcFlowKegel_2)
		delete m_pcFlowKegel_2;
	m_pcFlowKegel_2=NULL;
	if(m_pcFlowKegel_3)
		delete m_pcFlowKegel_3;
	m_pcFlowKegel_3=NULL;
	if(m_pcFlowKegel_4)
		delete m_pcFlowKegel_4;
	m_pcFlowKegel_4=NULL;
	if(m_pcFlowKegel_5)
		delete m_pcFlowKegel_5;
	m_pcFlowKegel_5=NULL;
	if(m_pcFlowKegel_6)
		delete m_pcFlowKegel_6;
	m_pcFlowKegel_6=NULL;

	if(m_pcFlowKegel_7)
		delete m_pcFlowKegel_7;
	m_pcFlowKegel_7=NULL;

	if(m_pcFlowKegel_8)
		delete m_pcFlowKegel_8;
	m_pcFlowKegel_8=NULL;

	if(m_pcFlowKegel_9)
		delete m_pcFlowKegel_9;
	m_pcFlowKegel_9=NULL;

	/*if(m_pcOxyWarning)
		delete m_pcOxyWarning;
	m_pcOxyWarning=NULL;*/

	if(m_pcNumPara1er)
		delete m_pcNumPara1er;
	m_pcNumPara1er=NULL;

	if(m_pcNumPara2er)
		delete m_pcNumPara2er;
	m_pcNumPara2er=NULL;

	if(m_pcNumPara3er)
		delete m_pcNumPara3er;
	m_pcNumPara3er=NULL;

	/*if(m_pcNumHeader)
		delete m_pcNumHeader;
	m_pcNumHeader=NULL;*/

	if(m_pcLimitHigh_Up)
		delete m_pcLimitHigh_Up;
	m_pcLimitHigh_Up=NULL;

	if(m_pcLimitLow_Up)
		delete m_pcLimitLow_Up;
	m_pcLimitLow_Up=NULL;

	if(m_pcNumInfoTop)
		delete m_pcNumInfoTop;
	m_pcNumInfoTop=NULL;

	if(m_pcNumInfoBot)
		delete m_pcNumInfoBot;
	m_pcNumInfoBot=NULL;

	if(m_pcNumericTop)
		delete m_pcNumericTop;
	m_pcNumericTop= NULL;
	if(m_pcNumericBot)
		delete m_pcNumericBot;
	m_pcNumericBot= NULL;

	
	if(m_pcFlowmeterTherapy)
		delete m_pcFlowmeterTherapy;
	m_pcFlowmeterTherapy= NULL;

	if(m_pcFlowmeterLeft)
		delete m_pcFlowmeterLeft;
	m_pcFlowmeterLeft= NULL;
	
	if(m_pcFlowmeterRight)
		delete m_pcFlowmeterRight;
	m_pcFlowmeterRight= NULL;

	if(m_pcGraphCursor_left_up)
		delete m_pcGraphCursor_left_up;
	m_pcGraphCursor_left_up=NULL;
	if(m_pcGraphCursor_left_dw)
		delete m_pcGraphCursor_left_dw;
	m_pcGraphCursor_left_dw=NULL;
	if(m_pcGraphCursor_right_up)
		delete m_pcGraphCursor_right_up;
	m_pcGraphCursor_right_up=NULL;
	if(m_pcGraphCursor_right_dw)
		delete m_pcGraphCursor_right_dw;
	m_pcGraphCursor_right_dw=NULL;
	if(m_pcGraphCursor_up_up)
		delete m_pcGraphCursor_up_up;
	m_pcGraphCursor_up_up=NULL;
	if(m_pcGraphCursor_up_dw)
		delete m_pcGraphCursor_up_dw;
	m_pcGraphCursor_up_dw=NULL;
	
	if(m_pcGraphCursor_down_up)
		delete m_pcGraphCursor_down_up;
	m_pcGraphCursor_down_up=NULL;
	
	if(m_pcGraphCursor_down_dw)
		delete m_pcGraphCursor_down_dw;
	m_pcGraphCursor_down_dw=NULL;
	
	if(m_pcGraphCursor_pic)
		delete m_pcGraphCursor_pic;
	m_pcGraphCursor_pic=NULL;




	if(m_pcXmoveDown_up)
		delete m_pcXmoveDown_up;
	m_pcXmoveDown_up=NULL;
	if(m_pcXmoveDown_dw)
		delete m_pcXmoveDown_dw;
	m_pcXmoveDown_dw=NULL;
	if(m_pcXmoveUp_up)
		delete m_pcXmoveUp_up;
	m_pcXmoveUp_up=NULL;
	if(m_pcXmoveUp_dw)
		delete m_pcXmoveUp_dw;
	m_pcXmoveUp_dw=NULL;
	if(m_pcYmoveLeft_up)
		delete m_pcYmoveLeft_up;
	m_pcYmoveLeft_up=NULL;
	if(m_pcYmoveLeft_dw)
		delete m_pcYmoveLeft_dw;
	m_pcYmoveLeft_dw=NULL;
	if(m_pcYmoveRight_up)
		delete m_pcYmoveRight_up;
	m_pcYmoveRight_up=NULL;
	if(m_pcYmoveRight_dw)
		delete m_pcYmoveRight_dw;
	m_pcYmoveRight_dw=NULL;
	if(m_pcMoveXscale_pic)
		delete m_pcMoveXscale_pic;
	m_pcMoveXscale_pic=NULL;
	if(m_pcMoveYscale_pic)
		delete m_pcMoveYscale_pic;
	m_pcMoveYscale_pic=NULL;

	

	delete m_pbtnCursorUp;
	m_pbtnCursorUp=NULL;
	delete m_pbtnCursorDown;
	m_pbtnCursorDown=NULL;
	delete m_pbtnCursorLeft;
	m_pbtnCursorLeft=NULL;
	delete m_pbtnCursorRight;
	m_pbtnCursorRight=NULL;

	delete m_pbtnXmoveUp;
	m_pbtnXmoveUp=NULL;
	delete m_pbtnXmoveDown;
	m_pbtnXmoveDown=NULL;
	delete m_pbtnYmoveLeft;
	m_pbtnYmoveLeft=NULL;
	delete m_pbtnYmoveRight;
	m_pbtnYmoveRight=NULL;

	DeleteCriticalSection(&csDraw);
}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CWndNumeric::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


BEGIN_MESSAGE_MAP(CWndNumeric, CWnd)
	//{{AFX_MSG_MAP(CWndNumeric)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//ON_WM_TIMER()
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






// CWndNumAv message handlers
//************************************
// Method:    OnDestroy
// FullName:  CWndNumeric::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndNumeric::OnDestroy() 
{
	CWnd::OnDestroy();
		
	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);

	if(m_hdcStatic)
		DeleteDC(m_hdcStatic);

	if(m_hbmpStatic)
		DeleteObject(m_hbmpStatic);
}

// **************************************************************************
// 
// **************************************************************************
BOOL CWndNumeric::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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
		//CBrush cbrBack(RGB(140,140,255));
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);


		m_hdcStatic = CreateCompatibleDC(m_hDC);
		m_hbmpStatic = CreateCompatibleBitmap(m_hDC, m_lX, m_lY);
		SelectObject(m_hdcStatic, m_hbmpStatic);

		HPEN hpenprevStatic=(HPEN)SelectObject(m_hdcStatic, (HPEN)GetStockObject(NULL_PEN));	
		HBRUSH hbrprevStatic=(HBRUSH)SelectObject(m_hdcStatic,cbrBack); 
		Rectangle(m_hdcStatic, 0, 0, m_lX, m_lY);


		m_pcNumPara1er		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUMPARA_1);
		m_pcNumPara2er		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUMPARA_2);
		m_pcNumPara3er		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUMPARA_3);
		//m_pcNumHeader		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_VENTSTATE);

		m_pcNumInfoTop		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUM_INFO_TOP);
		m_pcNumInfoBot		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUM_INFO_BOT);

		m_pcNumericTop		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUMERIC_TOP);
		m_pcNumericBot		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUMERIC_BOT);

		m_pcFlowmeterTherapy		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_FLOWMETER_THERAPY);
		m_pcFlowmeterLeft		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_FLOWMETER_LEFT);
		m_pcFlowmeterRight		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_FLOWMETER_RIGHT);

		m_pcFlowKegel_0		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_FLOWKEGEL_0);
		m_pcFlowKegel_1		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_FLOWKEGEL_1);
		m_pcFlowKegel_2		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_FLOWKEGEL_2);
		m_pcFlowKegel_3		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_FLOWKEGEL_3);
		m_pcFlowKegel_4		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_FLOWKEGEL_4);
		m_pcFlowKegel_5		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_FLOWKEGEL_5);
		m_pcFlowKegel_6		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_FLOWKEGEL_6);
		m_pcFlowKegel_7		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_FLOWKEGEL_7);
		m_pcFlowKegel_8		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_FLOWKEGEL_8);
		m_pcFlowKegel_9		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_FLOWKEGEL_9);
		
		m_pcWarningYellow = new CBmp(theApp.m_hInstance,m_hDC,IDB_PARA_WARN_SUB);

		m_pcLimitHigh_Up	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_LIMIT_HIGH);
		m_pcLimitLow_Up		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_LIMIT_LOW);


		m_pcGraphCursor_left_up		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURBTN_LEFT_UP);
		m_pcGraphCursor_left_dw		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURBTN_LEFT_DW);
		m_pcGraphCursor_right_up	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURBTN_RIGHT_UP);
		m_pcGraphCursor_right_dw	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURBTN_RIGHT_DW);
		m_pcGraphCursor_up_up		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURBTN_UP_UP);
		m_pcGraphCursor_up_dw		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURBTN_UP_DW);
		m_pcGraphCursor_down_up		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURBTN_DOWN_UP);
		m_pcGraphCursor_down_dw		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURBTN_DOWN_DW);
		m_pcGraphCursor_pic			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURSOR_GRAPH);


		m_pcXmoveDown_up			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURBTN_XDOWN_UP);
		m_pcXmoveDown_dw			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURBTN_XDOWN_DW);
		m_pcXmoveUp_up				= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURBTN_XUP_UP);
		m_pcXmoveUp_dw				= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURBTN_XUP_DW);
		m_pcYmoveLeft_up			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURBTN_YLEFT_UP);
		m_pcYmoveLeft_dw			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURBTN_YLEFT_DW);
		m_pcYmoveRight_up			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURBTN_YRIGHT_UP);
		m_pcYmoveRight_dw			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURBTN_YRIGHT_DW);
		m_pcMoveXscale_pic			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURSOR_MOVEXSCALE);
		m_pcMoveYscale_pic			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_CURSOR_MOVEYSCALE);
		

		/*m_pcMediJet		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_MEDIJET);
		m_pcInfantFlow		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_INFANTFLOW);
		m_pcInfantFlowLP		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_INFANTFLOW_LP);*/


		BTN btn;

		btn.wID					= IDC_CURSORBTN_UP;	
		btn.poPosition.x		= 70;
		btn.poPosition.y		= 195;//217;
		btn.pcBmpUp				= m_pcGraphCursor_up_up;
		btn.pcBmpDown			= m_pcGraphCursor_up_dw;
		btn.pcBmpFocus			= m_pcGraphCursor_up_up;
		btn.pcBmpDisabled		= m_pcGraphCursor_up_up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnCursorUp=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pbtnCursorUp->Create(this,g_hf21AcuBold,0);
		m_pbtnCursorUp->SetText(_T(""));
		m_pbtnCursorUp->ShowWindow(SW_HIDE);

		/************************************************/
		btn.wID					= IDC_CURSORBTN_DOWN;	
		btn.poPosition.x		= 70;
		btn.poPosition.y		= 316;//338;
		btn.pcBmpUp				= m_pcGraphCursor_down_up;
		btn.pcBmpDown			= m_pcGraphCursor_down_dw;
		btn.pcBmpFocus			= m_pcGraphCursor_down_up;
		btn.pcBmpDisabled		= m_pcGraphCursor_down_up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnCursorDown=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pbtnCursorDown->Create(this,g_hf21AcuBold,0);
		m_pbtnCursorDown->SetText(_T(""));
		m_pbtnCursorDown->ShowWindow(SW_HIDE);

		/************************************************/
		btn.wID					= IDC_CURSORBTN_LEFT;	
		btn.poPosition.x		= 2;
		btn.poPosition.y		= 264;//286;
		btn.pcBmpUp				= m_pcGraphCursor_left_up;
		btn.pcBmpDown			= m_pcGraphCursor_left_dw;
		btn.pcBmpFocus			= m_pcGraphCursor_left_up;
		btn.pcBmpDisabled		= m_pcGraphCursor_left_up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnCursorLeft=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pbtnCursorLeft->Create(this,g_hf21AcuBold,0);
		m_pbtnCursorLeft->SetText(_T(""));
		m_pbtnCursorLeft->ShowWindow(SW_HIDE);


		/************************************************/
		btn.wID					= IDC_CURSORBTN_RIGHT;	
		btn.poPosition.x		= 121;
		btn.poPosition.y		= 264;//286;
		btn.pcBmpUp				= m_pcGraphCursor_right_up;
		btn.pcBmpDown			= m_pcGraphCursor_right_dw;
		btn.pcBmpFocus			= m_pcGraphCursor_right_up;
		btn.pcBmpDisabled		= m_pcGraphCursor_right_up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnCursorRight=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pbtnCursorRight->Create(this,g_hf21AcuBold,0);
		m_pbtnCursorRight->SetText(_T(""));
		m_pbtnCursorRight->ShowWindow(SW_HIDE);




		/************************************************/
		//m_pbtnXmoveUp=NULL;
		btn.wID					= IDC_CURSORBTN_XUP;	
		btn.poPosition.x		= 10;
		btn.poPosition.y		= 392;
		btn.pcBmpUp				= m_pcXmoveUp_up;
		btn.pcBmpDown			= m_pcXmoveUp_dw;
		btn.pcBmpFocus			= m_pcXmoveUp_up;
		btn.pcBmpDisabled		= m_pcXmoveUp_up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnXmoveUp=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pbtnXmoveUp->Create(this,g_hf21AcuBold,0);
		m_pbtnXmoveUp->SetText(_T(""));
		m_pbtnXmoveUp->ShowWindow(SW_HIDE);
		/************************************************/
		//m_pbtnXmoveDown=NULL;
		btn.wID					= IDC_CURSORBTN_XDOWN;	
		btn.poPosition.x		= 127;
		btn.poPosition.y		= 392;
		btn.pcBmpUp				= m_pcXmoveDown_up;
		btn.pcBmpDown			= m_pcXmoveDown_dw;
		btn.pcBmpFocus			= m_pcXmoveDown_up;
		btn.pcBmpDisabled		= m_pcXmoveDown_up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnXmoveDown=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pbtnXmoveDown->Create(this,g_hf21AcuBold,0);
		m_pbtnXmoveDown->SetText(_T(""));
		m_pbtnXmoveDown->ShowWindow(SW_HIDE);
		/************************************************/
		//m_pbtnYmoveLeft=NULL;
		btn.wID					= IDC_CURSORBTN_YLEFT;	
		btn.poPosition.x		= 10;
		btn.poPosition.y		= 392;
		btn.pcBmpUp				= m_pcYmoveLeft_up;
		btn.pcBmpDown			= m_pcYmoveLeft_dw;
		btn.pcBmpFocus			= m_pcYmoveLeft_up;
		btn.pcBmpDisabled		= m_pcYmoveLeft_up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnYmoveLeft=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pbtnYmoveLeft->Create(this,g_hf21AcuBold,0);
		m_pbtnYmoveLeft->SetText(_T(""));
		m_pbtnYmoveLeft->ShowWindow(SW_HIDE);
		/************************************************/
		//m_pbtnYmoveRight=NULL;
		btn.wID					= IDC_CURSORBTN_YRIGHT;	
		btn.poPosition.x		= 127;
		btn.poPosition.y		= 392;
		btn.pcBmpUp				= m_pcYmoveRight_up;
		btn.pcBmpDown			= m_pcYmoveRight_dw;
		btn.pcBmpFocus			= m_pcYmoveRight_up;
		btn.pcBmpDisabled		= m_pcYmoveRight_up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnYmoveRight=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pbtnYmoveRight->Create(this,g_hf21AcuBold,0);
		m_pbtnYmoveRight->SetText(_T(""));
		m_pbtnYmoveRight->ShowWindow(SW_HIDE);

		SelectObject(m_hdcStatic, hpenprevStatic);	
		SelectObject(m_hdcStatic,hbrprevStatic);
		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();//rkuNEWFIX

		return 1;
	}
	else
		return 0;
}
//bool CWndNumeric::isGraphCursor()
//{
//	return getModel()->getDATAHANDLER()->isGraphCursor();
//}
void CWndNumeric::SetGraphCursor(bool bDraw, bool bMoveXscale, bool bMoveYscale)
{
	bool bRedraw=false;
	if(m_bGraphCursor!=bDraw)
		bRedraw=true;
	else if(m_bMoveXscale!=bMoveXscale)
		bRedraw=true;
	else if(m_bMoveYscale!=bMoveYscale)
		bRedraw=true;

	m_bGraphCursor=bDraw;
	m_bMoveXscale=bMoveXscale;
	m_bMoveYscale=bMoveYscale;

	if(bRedraw)
		DrawData(true,true, true, true,true);

}
void CWndNumeric::refreshLimits()
{
	DrawData(true,false, false, true,true);
}
//void CWndNumeric::SetLimits(WNDNUM_ALARMLIMITS* pwna, bool bRedraw)
//{
//	memcpy(&m_wna,pwna,sizeof(WNDNUM_ALARMLIMITS));
//
//	if(bRedraw)
//	{
//		DrawData(true,false, false, true,true);
//		//Invalidate();
//		//UpdateWindow();
//	}
//}
//void CWndNumeric::GetLimits(WNDNUM_ALARMLIMITS* pwna)
//{
//	memcpy(pwna,&m_wna,sizeof(WNDNUM_ALARMLIMITS));
//}



// **************************************************************************
// 
// **************************************************************************
//void CWndNumeric::SetLables(WNDNUM_LABELS* pwnl)
//{
//	////memcpy(&m_wnl,pwnl,sizeof(WNDNUM_LABELS));
//	//m_wnl.bVal1=pwnl->bVal1;
//	//m_wnl.szName1=pwnl->szName1;
//	//m_wnl.szNameNote1=pwnl->szNameNote1;
//	//m_wnl.szUnit1=pwnl->szUnit1;
//	//m_wnl.bVal2=pwnl->bVal2;
//	//m_wnl.szName2=pwnl->szName2;
//	//m_wnl.szNameNote2=pwnl->szNameNote2;
//	//m_wnl.szUnit2=pwnl->szUnit2;
//	//m_wnl.bVal3=pwnl->bVal3;
//	//m_wnl.szName3=pwnl->szName3;
//	//m_wnl.szNameNote3=pwnl->szNameNote3;
//	//m_wnl.szUnit3=pwnl->szUnit3;
//	//m_wnl.bVal4=pwnl->bVal4;
//	//m_wnl.szName4=pwnl->szName4;
//	//m_wnl.szNameNote4=pwnl->szNameNote4;
//	//m_wnl.szUnit4=pwnl->szUnit4;
//	//m_wnl.bVal5=pwnl->bVal5;
//	//m_wnl.szName5=pwnl->szName5;
//	//m_wnl.szNameNote5=pwnl->szNameNote5;
//	//m_wnl.szUnit5=pwnl->szUnit5;
//	//m_wnl.bVal6=pwnl->bVal6;
//	//m_wnl.szName6=pwnl->szName6;
//	//m_wnl.szNameNote6=pwnl->szNameNote6;
//	//m_wnl.szUnit6=pwnl->szUnit6;
//	//m_wnl.bVal7=pwnl->bVal7;
//	//m_wnl.szName7=pwnl->szName7;
//	//m_wnl.szNameNote7=pwnl->szNameNote7;
//	//m_wnl.szUnit7=pwnl->szUnit7;
//	//m_wnl.bVal8=pwnl->bVal8;
//	//m_wnl.szName8=pwnl->szName8;
//	//m_wnl.szNameNote8=pwnl->szNameNote8;
//	//m_wnl.szUnit8=pwnl->szUnit8;
//	//m_wnl.bVal9=pwnl->bVal9;
//	//m_wnl.szName9=pwnl->szName9;
//	//m_wnl.szNameNote9=pwnl->szNameNote9;
//	//m_wnl.szUnit9=pwnl->szUnit9;
//}
// **************************************************************************
// 
// **************************************************************************
//void CWndNumeric::SetValues(WNDNUM_VALUES* pwnv, bool bRedraw)
//{
//	memcpy(&m_wnv,pwnv,sizeof(WNDNUM_VALUES));
//
//	if(bRedraw)
//	{
//		DrawData(true,false, false, false,true);
//	}
//}

//void CWndNumeric::refreshMeasurements()
//{
//	DrawData(true,false, false, false,true);
//}
// **************************************************************************
// 
// **************************************************************************
//void CWndNumeric::SetPmax(int iPmax, bool bRedraw)
//{
//	m_wnv.iPmax=iPmax;
//	if(bRedraw)
//	{
//		DrawData(true,false, false, false,false);
//	}
//}

// **************************************************************************
// 
// **************************************************************************
//void CWndNumeric::GetValues(WNDNUM_VALUES* pwnv)
//{
//	memcpy(pwnv,&m_wnv,sizeof(WNDNUM_VALUES));
//}



// **************************************************************************
// 
// **************************************************************************
void CWndNumeric::OnPaint() 
{
	CPaintDC dc(this);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

// **************************************************************************
// 
// **************************************************************************
void CWndNumeric::Show(bool bShow) 
{
	/*if(this->IsWindowVisible() == bShow)
	{
	return;
	}*/

	if(bShow)
	{
		SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
		/*CStringW sz = m_wnl.szName4;
		int iTest = m_wnv.nPmax;*/
		//DrawData(false,true);

	}
	else
		SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
}


BOOL CWndNumeric::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		if(pMsg->wParam==VK_SPACE)
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETFOCUS_PIMARYVIEW);
			return 1;
		}
		break;
	default:
		{

		}
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}


void CWndNumeric::OnBnClickedCursorUp()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_CURSOR_UP);
}
void CWndNumeric::OnBnClickedCursorDwn()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_CURSOR_DOWN);
}
void CWndNumeric::OnBnClickedCursorLeft()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_CURSOR_LEFT);
}
void CWndNumeric::OnBnClickedCursorRight()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_CURSOR_RIGHT);
}

void CWndNumeric::OnBnClickedMoveXUp()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_CURSOR_MOVEXUP);
}
void CWndNumeric::OnBnClickedMoveXDown()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_CURSOR_MOVEXDOWN);
}
void CWndNumeric::OnBnClickedMoveYLeft()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_CURSOR_MOVEYLEFT);
}
void CWndNumeric::OnBnClickedMoveYRight()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_CURSOR_MOVEYRIGHT);
}

//rkuTICKCOUNT
bool CWndNumeric::isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay)////used to check if old tick count plus delay is still lower than actual tickCount, (dwLastTickCount+DELAY<getTickCount64())
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