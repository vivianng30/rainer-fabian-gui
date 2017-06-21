// SubViewService.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SubViewService.h"

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

// CSubViewService

IMPLEMENT_DYNAMIC(CSubViewService, CWnd)

CSubViewService::CSubViewService()
{
	m_pModel=NULL;

	m_pEditPwd=NULL;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	m_pcKbrdAlphanum_Up=NULL;
	m_pcKbrdAlphanum_Dw=NULL;
	m_pcKbrdDel_Up=NULL;
	m_pcKbrdDel_Dw=NULL;

	m_pcKbrd_Del=NULL;

	m_pcKbrd_1=NULL;
	m_pcKbrd_2=NULL;
	m_pcKbrd_3=NULL;
	m_pcKbrd_4=NULL;
	m_pcKbrd_5=NULL;
	m_pcKbrd_6=NULL;
	m_pcKbrd_7=NULL;
	m_pcKbrd_8=NULL;
	m_pcKbrd_9=NULL;
	m_pcKbrd_0=NULL;

	m_pcKbrd_a=NULL;
	m_pcKbrd_b=NULL;
	m_pcKbrd_c=NULL;
	m_pcKbrd_d=NULL;
	m_pcKbrd_e=NULL;
	m_pcKbrd_f=NULL;

	m_pc_Up=NULL;
	m_pc_Dw=NULL;

	m_pcOk=NULL;
	//m_pcCancel=NULL;

	m_szPSSW=getModel()->GetUniqueID();
	m_szPSSW=m_szPSSW.Right(5);

	m_szInfo=getModel()->GetLanguageString(IDS_TXT_SETPWD);
	m_bInfoClean=false;
}

CSubViewService::~CSubViewService()
{
	delete m_pcKbrdAlphanum_Up;
	m_pcKbrdAlphanum_Up=NULL;
	delete m_pcKbrdAlphanum_Dw;
	m_pcKbrdAlphanum_Dw=NULL;
	delete m_pcKbrdDel_Up;
	m_pcKbrdDel_Up=NULL;
	delete m_pcKbrdDel_Dw;
	m_pcKbrdDel_Dw=NULL;
	
	delete m_pcKbrd_Del;
	m_pcKbrd_Del=NULL;
	
	delete m_pEditPwd;
	m_pEditPwd=NULL;


	delete m_pcKbrd_1;
	m_pcKbrd_1=NULL;
	delete m_pcKbrd_2;
	m_pcKbrd_2=NULL;
	delete m_pcKbrd_3;
	m_pcKbrd_3=NULL;
	delete m_pcKbrd_4;
	m_pcKbrd_4=NULL;
	delete m_pcKbrd_5;
	m_pcKbrd_5=NULL;
	delete m_pcKbrd_6;
	m_pcKbrd_6=NULL;
	delete m_pcKbrd_7;
	m_pcKbrd_7=NULL;
	delete m_pcKbrd_8;
	m_pcKbrd_8=NULL;
	delete m_pcKbrd_9;
	m_pcKbrd_9=NULL;
	delete m_pcKbrd_0;
	m_pcKbrd_0=NULL;

	delete m_pc_Up;
	m_pc_Up=NULL;
	delete m_pc_Dw;
	m_pc_Dw=NULL;

	delete m_pcOk;
	m_pcOk=NULL;
	/*delete m_pcCancel;
	m_pcCancel=NULL;*/


	delete m_pcKbrd_e;
	m_pcKbrd_e=NULL;
	delete m_pcKbrd_a;
	m_pcKbrd_a=NULL;
	delete m_pcKbrd_d;
	m_pcKbrd_d=NULL;
	delete m_pcKbrd_f;
	m_pcKbrd_f=NULL;
	delete m_pcKbrd_c;
	m_pcKbrd_c=NULL;
	delete m_pcKbrd_b;
	m_pcKbrd_b=NULL;
}


BEGIN_MESSAGE_MAP(CSubViewService, CWnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_DEL, &CSubViewService::OnBnClickedDel)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_1, &CSubViewService::OnBnClicked_1)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_2, &CSubViewService::OnBnClicked_2)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_3, &CSubViewService::OnBnClicked_3)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_4, &CSubViewService::OnBnClicked_4)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_5, &CSubViewService::OnBnClicked_5)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_6, &CSubViewService::OnBnClicked_6)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_7, &CSubViewService::OnBnClicked_7)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_8, &CSubViewService::OnBnClicked_8)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_9, &CSubViewService::OnBnClicked_9)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_0, &CSubViewService::OnBnClicked_0)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_a, &CSubViewService::OnBnClicked_a)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_b, &CSubViewService::OnBnClicked_b)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_c, &CSubViewService::OnBnClicked_c)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_d, &CSubViewService::OnBnClicked_d)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_e, &CSubViewService::OnBnClicked_e)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_f, &CSubViewService::OnBnClicked_f)
END_MESSAGE_MAP()



// CSubViewService message handlers
// **************************************************************************
// 
// **************************************************************************
CMVModel *CSubViewService::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}
// **************************************************************************
// 
// **************************************************************************
bool CSubViewService::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
{
	if (!Create(pParentWnd, rc, nID, pContext))
	{
		return false;
	}

	return true;
}
// **************************************************************************
// 
// **************************************************************************
BOOL CSubViewService::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

		//CBrush cbrBack(RGB(200,200,200));
		CBrush cbrBack(BACKGND);
		//CBrush cbrBack(RGB(100,100,100));
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

		m_pcKbrdAlphanum_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_ALPHANUM_UP);
		m_pcKbrdAlphanum_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_ALPHANUM_DW);
		m_pcKbrdDel_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_DEL_UP);
		m_pcKbrdDel_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_DEL_DW);

		m_pc_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_UP);
		m_pc_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PAT_DW);
	

		m_pEditPwd = new CEdit;
		m_pEditPwd->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
			CRect(280, 50, 505, 80), this, IDC_EDIT_PASSWORD);
		m_pEditPwd->SetFont(CFont::FromHandle(g_hf17AcuBold));
		m_pEditPwd->SetWindowText(_T(""));
	

		BTN btn;

		btn.wID					= IDC_BTN_SETUP_KBRD_DEL;	
		btn.poPosition.x		= 415;
		btn.poPosition.y		= 285;
		btn.pcBmpUp				= m_pcKbrdDel_Up;
		btn.pcBmpDown			= m_pcKbrdDel_Dw;
		btn.pcBmpFocus			= m_pcKbrdDel_Up;
		btn.pcBmpDisabled		= m_pcKbrdDel_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_Del=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_Del->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_Del->SetText(_T(""));
		m_pcKbrd_Del->ShowWindow(SW_SHOW);

	
	

		//***********************************************************
		btn.wID					= IDC_BTN_SETUP_KBRD_0;	
		btn.poPosition.x		= 280;
		btn.poPosition.y		= 150;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_0=new CKbrdBtn(btn,COLOR_TXTBTNUP,0);
		m_pcKbrd_0->Create(this,g_hf17AcuBold,10);
		m_pcKbrd_0->SetText(_T("0"));
		m_pcKbrd_0->ShowWindow(SW_SHOW);



		btn.wID					= IDC_BTN_SETUP_KBRD_1;	
		btn.poPosition.x		= 325;
		btn.poPosition.y		= 150;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_1=new CKbrdBtn(btn,COLOR_TXTBTNUP,0);
		m_pcKbrd_1->Create(this,g_hf17AcuBold,10);
		m_pcKbrd_1->SetText(_T("1"));
		m_pcKbrd_1->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_2;	
		btn.poPosition.x		= 370;
		btn.poPosition.y		= 150;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_2=new CKbrdBtn(btn,COLOR_TXTBTNUP,0);
		m_pcKbrd_2->Create(this,g_hf17AcuBold,10);
		m_pcKbrd_2->SetText(_T("2"));
		m_pcKbrd_2->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_3;	
		btn.poPosition.x		= 415;
		btn.poPosition.y		= 150;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_3=new CKbrdBtn(btn,COLOR_TXTBTNUP,0);
		m_pcKbrd_3->Create(this,g_hf17AcuBold,10);
		m_pcKbrd_3->SetText(_T("3"));
		m_pcKbrd_3->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_4;	
		btn.poPosition.x		= 460;
		btn.poPosition.y		= 150;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_4=new CKbrdBtn(btn,COLOR_TXTBTNUP,0);
		m_pcKbrd_4->Create(this,g_hf17AcuBold,10);
		m_pcKbrd_4->SetText(_T("4"));
		m_pcKbrd_4->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_5;	
		btn.poPosition.x		= 280;
		btn.poPosition.y		= 195;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_5=new CKbrdBtn(btn,COLOR_TXTBTNUP,0);
		m_pcKbrd_5->Create(this,g_hf17AcuBold,10);
		m_pcKbrd_5->SetText(_T("5"));
		m_pcKbrd_5->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_6;	
		btn.poPosition.x		= 325;
		btn.poPosition.y		= 195;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_6=new CKbrdBtn(btn,COLOR_TXTBTNUP,0);
		m_pcKbrd_6->Create(this,g_hf17AcuBold,10);
		m_pcKbrd_6->SetText(_T("6"));
		m_pcKbrd_6->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_7;	
		btn.poPosition.x		= 370;
		btn.poPosition.y		= 195;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_7=new CKbrdBtn(btn,COLOR_TXTBTNUP,0);
		m_pcKbrd_7->Create(this,g_hf17AcuBold,10);
		m_pcKbrd_7->SetText(_T("7"));
		m_pcKbrd_7->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_8;	
		btn.poPosition.x		= 415;
		btn.poPosition.y		= 195;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_8=new CKbrdBtn(btn,COLOR_TXTBTNUP,0);
		m_pcKbrd_8->Create(this,g_hf17AcuBold,10);
		m_pcKbrd_8->SetText(_T("8"));
		m_pcKbrd_8->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_9;	
		btn.poPosition.x		= 460;
		btn.poPosition.y		= 195;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_9=new CKbrdBtn(btn,COLOR_TXTBTNUP,0);
		m_pcKbrd_9->Create(this,g_hf17AcuBold,10);
		m_pcKbrd_9->SetText(_T("9"));
		m_pcKbrd_9->ShowWindow(SW_SHOW);

		



		btn.wID					= IDC_BTN_SETUP_KBRD_a;	
		btn.poPosition.x		= 280;
		btn.poPosition.y		= 240;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_a=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_a->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_a->SetText(_T("A"));
		m_pcKbrd_a->ShowWindow(SW_SHOW);


		//CBtn* m_pcKbrd_b;
		btn.wID					= IDC_BTN_SETUP_KBRD_b;	
		btn.poPosition.x		= 325;
		btn.poPosition.y		= 240;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_b=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_b->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_b->SetText(_T("B"));
		m_pcKbrd_b->ShowWindow(SW_SHOW);



		//CBtn* m_pcKbrd_c;
		btn.wID					= IDC_BTN_SETUP_KBRD_c;	
		btn.poPosition.x		= 370;
		btn.poPosition.y		= 240;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_c=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_c->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_c->SetText(_T("C"));
		m_pcKbrd_c->ShowWindow(SW_SHOW);



		//CBtn* m_pcKbrd_d;
		btn.wID					= IDC_BTN_SETUP_KBRD_d;	
		btn.poPosition.x		= 415;
		btn.poPosition.y		= 240;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_d=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_d->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_d->SetText(_T("D"));
		m_pcKbrd_d->ShowWindow(SW_SHOW);




		//CBtn* m_pcKbrd_e;
		btn.wID					= IDC_BTN_SETUP_KBRD_e;	
		btn.poPosition.x		= 460;
		btn.poPosition.y		= 240;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_e=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_e->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_e->SetText(_T("E"));
		m_pcKbrd_e->ShowWindow(SW_SHOW);


	

		//CBtn* m_pcKbrd_f;
		btn.wID					= IDC_BTN_SETUP_KBRD_f;	
		btn.poPosition.x		= 280;
		btn.poPosition.y		= 285;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_f=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_f->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_f->SetText(_T("F"));
		m_pcKbrd_f->ShowWindow(SW_SHOW);



		btn.wID					= IDOK;	
		btn.poPosition.x		= 320;//200;
		btn.poPosition.y		= 410;//390;
		btn.pcBmpUp				= m_pc_Up;
		btn.pcBmpDown			= m_pc_Dw;
		btn.pcBmpFocus			= m_pc_Up;
		btn.pcBmpDisabled		= m_pc_Up;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		m_pcOk=new CPushBtn(btn,COLOR_TXTBTNUP);
		m_pcOk->Create(this,g_hf15AcuMed,0);
		//m_pcOk->SetText(_T("Ok"));
		m_pcOk->SetText(getModel()->GetLanguageString(IDS_BTN_OK));
		m_pcOk->ShowWindow(SW_SHOW);


		/*btn.wID					= IDCANCEL;	
		btn.poPosition.x		= 405;
		btn.poPosition.y		= 390;
		btn.pcBmpUp				= m_pc_Up;
		btn.pcBmpDown			= m_pc_Dw;
		btn.pcBmpFocus			= m_pc_Up;
		btn.pcBmpDisabled		= m_pc_Up;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		m_pcCancel=new CPushBtn(btn,COLOR_TXTBTNUP);
		m_pcCancel->Create(this,g_hf15AcuMed,0);
		m_pcCancel->SetText(getModel()->GetLanguageString(IDS_BTN_CANCEL));
		m_pcCancel->ShowWindow(SW_SHOW);*/
	

	
		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();//rkuNEWFIX

		return 1;
	}
	else
		return 0;
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewService::Show(bool bShow)
{
	if(bShow)
	{
		this->ShowWindow(SW_SHOW);

		Draw();

		if(m_pEditPwd)
			m_pEditPwd->SetFocus();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	else
	{
		this->ShowWindow(SW_HIDE);
	}


}

//************************************
// Method:    OnDestroy
// FullName:  CSubViewService::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CSubViewService::OnDestroy()
{
	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}


void CSubViewService::Key(int c)
{
	if(c == -1)
	{
		if(m_pEditPwd)
			m_pEditPwd->SendMessage(WM_CHAR, VK_BACK, 1);
	}
	else
	{
		if(m_pEditPwd)
			m_pEditPwd->SendMessage(WM_CHAR, c, 1);
		//m_pEditPwd->SendMessage(WM_CHAR, '*', 1);
	}
	ClearInfo();
	if(m_pEditPwd)
		m_pEditPwd->SetFocus();
	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);

}


void CSubViewService::OnBnClickedDel()
{
	Key(-1);
}
void CSubViewService::OnBnClicked_1()
{
	Key('1');
}
void CSubViewService::OnBnClicked_2()
{
	Key('2');
}
void CSubViewService::OnBnClicked_3()
{
	Key('3');
}
void CSubViewService::OnBnClicked_4()
{
	Key('4');
}
void CSubViewService::OnBnClicked_5()
{
	Key('5');
}
void CSubViewService::OnBnClicked_6()
{
	Key('6');
}
void CSubViewService::OnBnClicked_7()
{
	Key('7');
}
void CSubViewService::OnBnClicked_8()
{
	Key('8');
}
void CSubViewService::OnBnClicked_9()
{
	Key('9');
}
void CSubViewService::OnBnClicked_0()
{
	Key('0');
}


void CSubViewService::OnBnClicked_e()
{
	Key('E');
}
void CSubViewService::OnBnClicked_a()
{
	Key('A');
}
void CSubViewService::OnBnClicked_d()
{
	Key('D');
}
void CSubViewService::OnBnClicked_f()
{
	Key('F');
}
void CSubViewService::OnBnClicked_c()
{
	Key('C');
}

void CSubViewService::OnBnClicked_b()
{
	Key('B');
}


void CSubViewService::BnClickedOk()
{
	

	if(getModel()->getCONFIG()->IsServicePasswordDisabled())
	{
		if(GetParent())
			GetParent()->PostMessage(WM_PASSWORD_OK);
	}
	else
	{
		CStringW szTemp=_T("");
		if(m_pEditPwd)
			m_pEditPwd->GetWindowText(szTemp);

		if(szTemp==m_szPSSW)
		{
			if(GetParent())
				GetParent()->PostMessage(WM_PASSWORD_OK);
		}
		else
		{
			m_bInfoClean=false;
			m_szInfo=getModel()->GetLanguageString(IDS_TXT_INVPWD);
			if(m_pEditPwd)
				m_pEditPwd->SetWindowText(_T(""));
			Draw();
			if(m_pEditPwd)
				m_pEditPwd->SetFocus();
		}
	}
	
}
void CSubViewService::BnClickedCancel()
{
	if(GetParent())
		GetParent()->PostMessage(WM_PASSWORD_ERR);
}


void CSubViewService::ClearInfo()
{
	if(m_bInfoClean)
	{
		return;
	}
	else
	{
		m_bInfoClean=true;
		m_szInfo=_T("");
		Draw();
	}
}


void CSubViewService::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);


	int tc=SetTextColor(hdcMem,0x0000000);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf15AcuMed);
	//CBrush cbrWhite(RGB(255,255,255));
	CBrush cbrBack(RGB(181,178,181));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);

	//Rectangle(hdcMem, 0, 0, m_lX, m_lY);
	BitBlt(hdcMem, 0, 0, m_lX, m_lY, m_hDC, 0, 0, SRCCOPY);


	rc.left = 100;  
	rc.top = 100;  
	rc.right  = 700;  
	rc.bottom = 150;

	

	if(getModel()->getCONFIG()->IsServicePasswordDisabled())
	{
		//m_pcCancel->EnableWindow(FALSE);
		if(m_pEditPwd)
			m_pEditPwd->EnableWindow(FALSE);

		if(m_pcKbrd_Del)
			m_pcKbrd_Del->EnableWindow(FALSE);

		if(m_pcKbrd_1)
			m_pcKbrd_1->EnableWindow(FALSE);
		if(m_pcKbrd_2)
			m_pcKbrd_2->EnableWindow(FALSE);
		if(m_pcKbrd_3)
			m_pcKbrd_3->EnableWindow(FALSE);
		if(m_pcKbrd_4)
			m_pcKbrd_4->EnableWindow(FALSE);
		if(m_pcKbrd_5)
			m_pcKbrd_5->EnableWindow(FALSE);
		if(m_pcKbrd_6)
			m_pcKbrd_6->EnableWindow(FALSE);
		if(m_pcKbrd_7)
			m_pcKbrd_7->EnableWindow(FALSE);
		if(m_pcKbrd_8)
			m_pcKbrd_8->EnableWindow(FALSE);
		if(m_pcKbrd_9)
			m_pcKbrd_9->EnableWindow(FALSE);
		if(m_pcKbrd_0)
			m_pcKbrd_0->EnableWindow(FALSE);

		if(m_pcKbrd_a)
			m_pcKbrd_a->EnableWindow(FALSE);
		if(m_pcKbrd_b)
			m_pcKbrd_b->EnableWindow(FALSE);
		if(m_pcKbrd_c)
			m_pcKbrd_c->EnableWindow(FALSE);
		if(m_pcKbrd_d)
			m_pcKbrd_d->EnableWindow(FALSE);
		if(m_pcKbrd_e)
			m_pcKbrd_e->EnableWindow(FALSE);
		if(m_pcKbrd_f)
			m_pcKbrd_f->EnableWindow(FALSE);

		//m_szInfo=_T("Warning! Password is disabled! System can be damaged!");
		m_szInfo=getModel()->GetLanguageString(IDS_TXT_WARNNOPSWRD);
	}
	else
	{
		//m_pcCancel->EnableWindow(TRUE);
		if(m_pEditPwd)
			m_pEditPwd->EnableWindow(TRUE);

		if(m_pcKbrd_Del)
			m_pcKbrd_Del->EnableWindow(TRUE);

		if(m_pcKbrd_1)
			m_pcKbrd_1->EnableWindow(TRUE);
		if(m_pcKbrd_2)
			m_pcKbrd_2->EnableWindow(TRUE);
		if(m_pcKbrd_3)
			m_pcKbrd_3->EnableWindow(TRUE);
		if(m_pcKbrd_4)
			m_pcKbrd_4->EnableWindow(TRUE);
		if(m_pcKbrd_5)
			m_pcKbrd_5->EnableWindow(TRUE);
		if(m_pcKbrd_6)
			m_pcKbrd_6->EnableWindow(TRUE);
		if(m_pcKbrd_7)
			m_pcKbrd_7->EnableWindow(TRUE);
		if(m_pcKbrd_8)
			m_pcKbrd_8->EnableWindow(TRUE);
		if(m_pcKbrd_9)
			m_pcKbrd_9->EnableWindow(TRUE);
		if(m_pcKbrd_0)
			m_pcKbrd_0->EnableWindow(TRUE);

		if(m_pcKbrd_a)
			m_pcKbrd_a->EnableWindow(TRUE);
		if(m_pcKbrd_b)
			m_pcKbrd_b->EnableWindow(TRUE);
		if(m_pcKbrd_c)
			m_pcKbrd_c->EnableWindow(TRUE);
		if(m_pcKbrd_d)
			m_pcKbrd_d->EnableWindow(TRUE);
		if(m_pcKbrd_e)
			m_pcKbrd_e->EnableWindow(TRUE);
		if(m_pcKbrd_f)
			m_pcKbrd_f->EnableWindow(TRUE);
	}

	DrawText(hdcMem,m_szInfo,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	SetTextColor(hdcMem,RGB(200,0,0));
	rc.left = 100;  
	rc.top = 340;  
	rc.right  = 700;  
	rc.bottom = 400;
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_CONFIRMSERVICE),-1,&rc,DT_CENTER|DT_TOP|DT_WORDBREAK);
	//DrawText(hdcMem,_T("By stepping into service mode you confirm there is no patient being connected to the device!"),-1,&rc,DT_CENTER|DT_TOP|DT_WORDBREAK);

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}

// **************************************************************************
// 
// **************************************************************************
LRESULT CSubViewService::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			case IDOK:
				{
					BnClickedOk();
					return 1;
				}
				break;
			/*case IDCANCEL:
				{
					BnClickedCancel();
				}
				break;*/
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