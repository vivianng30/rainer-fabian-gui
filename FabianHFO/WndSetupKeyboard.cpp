// WndSetupKeyboard.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndSetupKeyboard.h"
//Timer
//#define CLOCKTIMER	0x05

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000

//global font objects
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf8AcuNorm;
extern HFONT g_hf8AcuBold;
//extern HFONT g_hf16Normal;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf13AcuBold;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf15AcuMed;
//extern HFONT g_hf24Normal;
//extern HFONT g_hf24Medium;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf19AcuMed;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf23AcuBold;
extern HFONT g_hf25AcuMed;
extern HFONT g_hf27AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf33AcuBold;
//extern HFONT g_hf42Bold;
extern HFONT g_hf43AcuBold;
extern HFONT g_hf53AcuBold;
extern HFONT g_hf70Bold;
//extern HFONT g_hf80Bold;
//extern HFONT g_hf90Bold;

//extern CLangAdmin* g_pGlobalLanguageStrings;


// CWndSetupPatient

IMPLEMENT_DYNAMIC(CWndSetupKeyboard, CWnd)

CWndSetupKeyboard::CWndSetupKeyboard()
{
	m_pModel=NULL;

	m_pEdit=NULL;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	m_pcKbrdAlphanum_Up=NULL;
	m_pcKbrdAlphanum_Dw=NULL;
	m_pcKbrdSpace_Up=NULL;
	m_pcKbrdSpace_Dw=NULL;
	m_pcKbrdDel_Up=NULL;
	m_pcKbrdDel_Dw=NULL;
	m_pcKbrdShift_Up=NULL;
	m_pcKbrdShift_Dw=NULL;

	m_pc_Left_Up=NULL;
	m_pc_Left_Dw=NULL;
	m_pc_Right_Up=NULL;
	m_pc_Right_Dw=NULL;
	m_pc_Up_Up=NULL;
	m_pc_Up_Dw=NULL;
	m_pc_Down_Up=NULL;
	m_pc_Down_dw=NULL;


	m_pcFocusLeft=NULL;
	m_pcFocusRight=NULL;
	m_pcFocusUp=NULL;
	m_pcFocusDown=NULL;

	m_iID=0;

	m_eKeyRange=KR_NONE;

	m_pcKbrd_Del=NULL;
	m_pcKbrd_Space=NULL;
	m_pcKbrd_Shift=NULL;

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
	m_pcKbrd_sz=NULL;

	m_pcKbrd_q=NULL;
	m_pcKbrd_w=NULL;
	m_pcKbrd_e=NULL;
	m_pcKbrd_r=NULL;
	m_pcKbrd_t=NULL;
	m_pcKbrd_z=NULL;
	m_pcKbrd_u=NULL;
	m_pcKbrd_i=NULL;
	m_pcKbrd_o=NULL;
	m_pcKbrd_p=NULL;
	m_pcKbrd_ue=NULL;
	//m_pcKbrd_plus=NULL;

	m_pcKbrd_a=NULL;
	m_pcKbrd_s=NULL;
	m_pcKbrd_d=NULL;
	m_pcKbrd_f=NULL;
	m_pcKbrd_g=NULL;
	m_pcKbrd_h=NULL;
	m_pcKbrd_j=NULL;
	m_pcKbrd_k=NULL;
	m_pcKbrd_l=NULL;
	m_pcKbrd_oe=NULL;
	m_pcKbrd_ae=NULL;
	//m_pcKbrd_sharp=NULL;

	m_pcKbrd_y=NULL;
	m_pcKbrd_x=NULL;
	m_pcKbrd_c=NULL;
	m_pcKbrd_v=NULL;
	m_pcKbrd_b=NULL;
	m_pcKbrd_n=NULL;
	m_pcKbrd_m=NULL;

}

CWndSetupKeyboard::~CWndSetupKeyboard()
{
	delete m_pcKbrdAlphanum_Up;
	m_pcKbrdAlphanum_Up=NULL;
	delete m_pcKbrdAlphanum_Dw;
	m_pcKbrdAlphanum_Dw=NULL;
	delete m_pcKbrdSpace_Up;
	m_pcKbrdSpace_Up=NULL;
	delete m_pcKbrdSpace_Dw;
	m_pcKbrdSpace_Dw=NULL;
	delete m_pcKbrdDel_Up;
	m_pcKbrdDel_Up=NULL;
	delete m_pcKbrdDel_Dw;
	m_pcKbrdDel_Dw=NULL;
	delete m_pcKbrdShift_Up;
	m_pcKbrdShift_Up=NULL;
	delete m_pcKbrdShift_Dw;
	m_pcKbrdShift_Dw=NULL;
	delete m_pc_Left_Up;
	m_pc_Left_Up=NULL;
	delete m_pc_Left_Dw;
	m_pc_Left_Dw=NULL;
	delete m_pc_Right_Up;
	m_pc_Right_Up=NULL;
	delete m_pc_Right_Dw;
	m_pc_Right_Dw=NULL;
	delete m_pc_Up_Up;
	m_pc_Up_Up=NULL;
	delete m_pc_Up_Dw;
	m_pc_Up_Dw=NULL;
	delete m_pc_Down_Up;
	m_pc_Down_Up=NULL;
	delete m_pc_Down_dw;
	m_pc_Down_dw=NULL;
	delete m_pcFocusLeft;
	m_pcFocusLeft=NULL;
	delete m_pcFocusRight;
	m_pcFocusRight=NULL;
	delete m_pcFocusUp;
	m_pcFocusUp=NULL;
	delete m_pcFocusDown;
	m_pcFocusDown=NULL;
	delete m_pcKbrd_Del;
	m_pcKbrd_Del=NULL;
	delete m_pcKbrd_Space;
	m_pcKbrd_Space=NULL;
	delete m_pcKbrd_Shift;
	m_pcKbrd_Shift=NULL;
	
	
	

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
	delete m_pcKbrd_sz;
	m_pcKbrd_sz=NULL;
	
	
	delete m_pcKbrd_q;
	m_pcKbrd_q=NULL;
	delete m_pcKbrd_w;
	m_pcKbrd_w=NULL;
	delete m_pcKbrd_e;
	m_pcKbrd_e=NULL;
	delete m_pcKbrd_r;
	m_pcKbrd_r=NULL;
	delete m_pcKbrd_t;
	m_pcKbrd_t=NULL;
	delete m_pcKbrd_z;
	m_pcKbrd_z=NULL;
	delete m_pcKbrd_u;
	m_pcKbrd_u=NULL;
	delete m_pcKbrd_i;
	m_pcKbrd_i=NULL;
	delete m_pcKbrd_o;
	m_pcKbrd_o=NULL;
	delete m_pcKbrd_p;
	m_pcKbrd_p=NULL;
	delete m_pcKbrd_ue;
	m_pcKbrd_ue=NULL;
	//delete m_pcKbrd_plus;
	//m_pcKbrd_plus=NULL;
	
		

	delete m_pcKbrd_a;
	m_pcKbrd_a=NULL;
	delete m_pcKbrd_s;
	m_pcKbrd_s=NULL;
	delete m_pcKbrd_d;
	m_pcKbrd_d=NULL;
	delete m_pcKbrd_f;
	m_pcKbrd_f=NULL;
	delete m_pcKbrd_g;
	m_pcKbrd_g=NULL;
	delete m_pcKbrd_h;
	m_pcKbrd_h=NULL;
	delete m_pcKbrd_j;
	m_pcKbrd_j=NULL;
	delete m_pcKbrd_k;
	m_pcKbrd_k=NULL;
	delete m_pcKbrd_l;
	m_pcKbrd_l=NULL;
	delete m_pcKbrd_oe;
	m_pcKbrd_oe=NULL;
	delete m_pcKbrd_ae;
	m_pcKbrd_ae=NULL;
	//delete m_pcKbrd_sharp;
	//m_pcKbrd_sharp=NULL;
	

	delete m_pcKbrd_y;
	m_pcKbrd_y=NULL;
	delete m_pcKbrd_x;
	m_pcKbrd_x=NULL;
	delete m_pcKbrd_c;
	m_pcKbrd_c=NULL;
	delete m_pcKbrd_v;
	m_pcKbrd_v=NULL;
	delete m_pcKbrd_b;
	m_pcKbrd_b=NULL;
	delete m_pcKbrd_n;
	m_pcKbrd_n=NULL;
	delete m_pcKbrd_m;
	m_pcKbrd_m=NULL;
	/*delete m_pcKbrd_semicol;
	delete m_pcKbrd_point;
	delete m_pcKbrd_dash;*/

	
	/*m_pcKbrd_semicol=NULL;
	m_pcKbrd_point=NULL;
	m_pcKbrd_dash=NULL;*/
}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CWndSetupKeyboard::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


BEGIN_MESSAGE_MAP(CWndSetupKeyboard, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_DEL, &CWndSetupKeyboard::OnBnClickedDel)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_SPACE, &CWndSetupKeyboard::OnBnClickedSpace)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_SHIFT, &CWndSetupKeyboard::OnBnClickedShift)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_1, &CWndSetupKeyboard::OnBnClicked_1)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_2, &CWndSetupKeyboard::OnBnClicked_2)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_3, &CWndSetupKeyboard::OnBnClicked_3)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_4, &CWndSetupKeyboard::OnBnClicked_4)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_5, &CWndSetupKeyboard::OnBnClicked_5)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_6, &CWndSetupKeyboard::OnBnClicked_6)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_7, &CWndSetupKeyboard::OnBnClicked_7)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_8, &CWndSetupKeyboard::OnBnClicked_8)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_9, &CWndSetupKeyboard::OnBnClicked_9)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_0, &CWndSetupKeyboard::OnBnClicked_0)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_sz, &CWndSetupKeyboard::OnBnClicked_sz)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_q, &CWndSetupKeyboard::OnBnClicked_q)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_w, &CWndSetupKeyboard::OnBnClicked_w)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_e, &CWndSetupKeyboard::OnBnClicked_e)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_r, &CWndSetupKeyboard::OnBnClicked_r)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_t, &CWndSetupKeyboard::OnBnClicked_t)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_z, &CWndSetupKeyboard::OnBnClicked_z)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_u, &CWndSetupKeyboard::OnBnClicked_u)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_i, &CWndSetupKeyboard::OnBnClicked_i)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_o, &CWndSetupKeyboard::OnBnClicked_o)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_p, &CWndSetupKeyboard::OnBnClicked_p)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_ue, &CWndSetupKeyboard::OnBnClicked_ue)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_a, &CWndSetupKeyboard::OnBnClicked_a)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_s, &CWndSetupKeyboard::OnBnClicked_s)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_d, &CWndSetupKeyboard::OnBnClicked_d)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_f, &CWndSetupKeyboard::OnBnClicked_f)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_g, &CWndSetupKeyboard::OnBnClicked_g)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_h, &CWndSetupKeyboard::OnBnClicked_h)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_j, &CWndSetupKeyboard::OnBnClicked_j)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_k, &CWndSetupKeyboard::OnBnClicked_k)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_l, &CWndSetupKeyboard::OnBnClicked_l)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_oe, &CWndSetupKeyboard::OnBnClicked_oe)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_ae, &CWndSetupKeyboard::OnBnClicked_ae)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_y, &CWndSetupKeyboard::OnBnClicked_y)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_x, &CWndSetupKeyboard::OnBnClicked_x)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_c, &CWndSetupKeyboard::OnBnClicked_c)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_v, &CWndSetupKeyboard::OnBnClicked_v)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_b, &CWndSetupKeyboard::OnBnClicked_b)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_n, &CWndSetupKeyboard::OnBnClicked_n)
	ON_BN_CLICKED(IDC_BTN_SETUP_KBRD_m, &CWndSetupKeyboard::OnBnClicked_m)
	END_MESSAGE_MAP()





// CWndSetupPatient-Meldungshandler


// **************************************************************************
// 
// **************************************************************************
BOOL CWndSetupKeyboard::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{


	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD/*|WS_VISIBLE*/,rc,pParentWnd,nID))
	{
		CClientDC dc(this);

		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		//CBrush cbrBack(RGB(200,200,200));
		CBrush cbrBack(RGB(181,178,181));
		//CBrush cbrBack(RGB(100,100,100));
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		m_pcKbrdAlphanum_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_ALPHANUM_UP);
		m_pcKbrdAlphanum_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_ALPHANUM_DW);
		m_pcKbrdSpace_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_SPACE_UP);
		m_pcKbrdSpace_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_SPACE_DW);
		m_pcKbrdDel_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_DEL_UP);
		m_pcKbrdDel_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_DEL_DW);
		m_pcKbrdShift_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_SHIFT_UP);
		m_pcKbrdShift_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_SHIFT_DW);

		m_pc_Left_Up	= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_LEFT_UP);
		m_pc_Left_Dw	= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_LEFT_DW);
		m_pc_Right_Up	= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_RIGHT_UP);
		m_pc_Right_Dw	= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_RIGHT_DW);
		m_pc_Up_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_UP_UP);
		m_pc_Up_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_UP_DW);
		m_pc_Down_Up	= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_DOWN_UP);
		m_pc_Down_dw	= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_KBRD_DOWN_DW);


		BTN btn;

		btn.wID					= IDC_BTN_SETUP_FOCUS_LEFT;	
		btn.poPosition.x		= 540+90;
		btn.poPosition.y		= 135;
		btn.pcBmpUp				= m_pc_Left_Up;
		btn.pcBmpDown			= m_pc_Left_Dw;
		btn.pcBmpFocus			= m_pc_Left_Up;
		btn.pcBmpDisabled		= m_pc_Left_Up;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		m_pcFocusLeft=new CPushBtn(btn,COLOR_TXTBTNUP);
		m_pcFocusLeft->Create(this,g_hf15AcuMed,0);
		m_pcFocusLeft->SetText(_T(""));
		m_pcFocusLeft->ShowWindow(SW_SHOW);


		btn.wID					= IDC_BTN_SETUP_FOCUS_RIGHT;	
		btn.poPosition.x		= 630+90;
		btn.poPosition.y		= 135;
		btn.pcBmpUp				= m_pc_Right_Up;
		btn.pcBmpDown			= m_pc_Right_Dw;
		btn.pcBmpFocus			= m_pc_Right_Up;
		btn.pcBmpDisabled		= m_pc_Right_Up;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		m_pcFocusRight=new CPushBtn(btn,COLOR_TXTBTNUP);
		m_pcFocusRight->Create(this,g_hf15AcuMed,0);
		m_pcFocusRight->SetText(_T(""));
		m_pcFocusRight->ShowWindow(SW_SHOW);


		btn.wID					= IDC_BTN_SETUP_FOCUS_UP;	
		btn.poPosition.x		= 585+90;
		btn.poPosition.y		= 90;
		btn.pcBmpUp				= m_pc_Up_Up;
		btn.pcBmpDown			= m_pc_Up_Dw;
		btn.pcBmpFocus			= m_pc_Up_Up;
		btn.pcBmpDisabled		= m_pc_Up_Up;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		m_pcFocusUp=new CPushBtn(btn,COLOR_TXTBTNUP);
		m_pcFocusUp->Create(this,g_hf15AcuMed,0);
		m_pcFocusUp->SetText(_T(""));
		m_pcFocusUp->ShowWindow(SW_SHOW);


		btn.wID					= IDC_BTN_SETUP_FOCUS_DOWN;	
		btn.poPosition.x		= 585+90;
		btn.poPosition.y		= 135;
		btn.pcBmpUp				= m_pc_Down_Up;
		btn.pcBmpDown			= m_pc_Down_dw;
		btn.pcBmpFocus			= m_pc_Down_Up;
		btn.pcBmpDisabled		= m_pc_Down_Up;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		m_pcFocusDown=new CPushBtn(btn,COLOR_TXTBTNUP);
		m_pcFocusDown->Create(this,g_hf15AcuMed,0);
		m_pcFocusDown->SetText(_T(""));
		m_pcFocusDown->ShowWindow(SW_SHOW);



		//IDB_BTN_KBRD_ALPHANUM_UP

		

		//CBtn* m_pcKbrd_Del;
		btn.wID					= IDC_BTN_SETUP_KBRD_DEL;	
		btn.poPosition.x		= 450+60;//575;
		btn.poPosition.y		= 135;
		btn.pcBmpUp				= m_pcKbrdDel_Up;
		btn.pcBmpDown			= m_pcKbrdDel_Dw;
		btn.pcBmpFocus			= m_pcKbrdDel_Up;
		btn.pcBmpDisabled		= m_pcKbrdDel_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_Del=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_Del->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_Del->SetText(_T(""));
		m_pcKbrd_Del->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_Space;
		btn.wID					= IDC_BTN_SETUP_KBRD_SPACE;	
		btn.poPosition.x		= 360+60;//485;
		btn.poPosition.y		= 135;
		btn.pcBmpUp				= m_pcKbrdSpace_Up;
		btn.pcBmpDown			= m_pcKbrdSpace_Dw;
		btn.pcBmpFocus			= m_pcKbrdSpace_Up;
		btn.pcBmpDisabled		= m_pcKbrdSpace_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_Space=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_Space->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_Space->SetText(_T(""));
		m_pcKbrd_Space->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_Shift;
		btn.wID					= IDC_BTN_SETUP_KBRD_SHIFT;	
		btn.poPosition.x		= 60;
		btn.poPosition.y		= 90;
		btn.pcBmpUp				= m_pcKbrdShift_Up;
		btn.pcBmpDown			= m_pcKbrdShift_Dw;
		btn.pcBmpFocus			= m_pcKbrdShift_Up;
		btn.pcBmpDisabled		= m_pcKbrdShift_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_Shift=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_Shift->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_Shift->SetText(_T(""),_T(""));
		m_pcKbrd_Shift->ShowWindow(SW_SHOW);


		//***********************************************************
		btn.wID					= IDC_BTN_SETUP_KBRD_1;	
		btn.poPosition.x		= 45+60;
		btn.poPosition.y		= 0;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_1=new CKbrdBtn(btn,COLOR_TXTBTNUP,true);
		m_pcKbrd_1->Create(this,g_hf13AcuBold,10);
		m_pcKbrd_1->SetText(_T("!"),_T("1"));
		m_pcKbrd_1->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_2;	
		btn.poPosition.x		= 90+60;
		btn.poPosition.y		= 0;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_2=new CKbrdBtn(btn,COLOR_TXTBTNUP,true);
		m_pcKbrd_2->Create(this,g_hf13AcuBold,10);
		m_pcKbrd_2->SetText(_T("?"),_T("2"));
		m_pcKbrd_2->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_3;	
		btn.poPosition.x		= 135+60;
		btn.poPosition.y		= 0;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_3=new CKbrdBtn(btn,COLOR_TXTBTNUP,true);
		m_pcKbrd_3->Create(this,g_hf13AcuBold,10);
		m_pcKbrd_3->SetText(_T("."),_T("3"));
		m_pcKbrd_3->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_4;	
		btn.poPosition.x		= 180+60;
		btn.poPosition.y		= 0;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_4=new CKbrdBtn(btn,COLOR_TXTBTNUP,true);
		m_pcKbrd_4->Create(this,g_hf13AcuBold,10);
		m_pcKbrd_4->SetText(_T(","),_T("4"));
		m_pcKbrd_4->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_5;	
		btn.poPosition.x		= 225+60;
		btn.poPosition.y		= 0;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_5=new CKbrdBtn(btn,COLOR_TXTBTNUP,true);
		m_pcKbrd_5->Create(this,g_hf13AcuBold,10);
		m_pcKbrd_5->SetText(_T(":"),_T("5"));
		m_pcKbrd_5->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_6;	
		btn.poPosition.x		= 270+60;
		btn.poPosition.y		= 0;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_6=new CKbrdBtn(btn,COLOR_TXTBTNUP,true);
		m_pcKbrd_6->Create(this,g_hf13AcuBold,10);
		m_pcKbrd_6->SetText(_T("+"),_T("6"));
		m_pcKbrd_6->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_7;	
		btn.poPosition.x		= 315+60;
		btn.poPosition.y		= 0;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_7=new CKbrdBtn(btn,COLOR_TXTBTNUP,true);
		m_pcKbrd_7->Create(this,g_hf13AcuBold,10);
		m_pcKbrd_7->SetText(_T("-"),_T("7"));
		m_pcKbrd_7->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_8;	
		btn.poPosition.x		= 360+60;
		btn.poPosition.y		= 0;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_8=new CKbrdBtn(btn,COLOR_TXTBTNUP,true);
		m_pcKbrd_8->Create(this,g_hf13AcuBold,10);
		m_pcKbrd_8->SetText(_T("="),_T("8"));
		m_pcKbrd_8->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_9;	
		btn.poPosition.x		= 405+60;
		btn.poPosition.y		= 0;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_9=new CKbrdBtn(btn,COLOR_TXTBTNUP,true);
		m_pcKbrd_9->Create(this,g_hf13AcuBold,10);
		m_pcKbrd_9->SetText(_T("("),_T("9"));
		m_pcKbrd_9->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_KBRD_0;	
		btn.poPosition.x		= 450+60;
		btn.poPosition.y		= 0;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_0=new CKbrdBtn(btn,COLOR_TXTBTNUP,true);
		m_pcKbrd_0->Create(this,g_hf13AcuBold,10);
		m_pcKbrd_0->SetText(_T(")"),_T("0"));
		m_pcKbrd_0->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_ß;
		btn.wID					= IDC_BTN_SETUP_KBRD_sz;	
		btn.poPosition.x		= 495+60;
		btn.poPosition.y		= 0;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_LEFT;
		m_pcKbrd_sz=new CKbrdBtn(btn,COLOR_TXTBTNUP,true);
		m_pcKbrd_sz->Create(this,g_hf13AcuBold,10);
		m_pcKbrd_sz->SetText(_T("%"),_T("ß"));
		m_pcKbrd_sz->ShowWindow(SW_SHOW);

		/*******************************************************/

		//CBtn* m_pcKbrd_q;
		btn.wID					= IDC_BTN_SETUP_KBRD_q;	
		btn.poPosition.x		= 45+60;
		btn.poPosition.y		= 45;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_q=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_q->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_q->SetText(_T("q"));
		m_pcKbrd_q->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_w;
		btn.wID					= IDC_BTN_SETUP_KBRD_w;	
		btn.poPosition.x		= 90+60;
		btn.poPosition.y		= 45;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_w=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_w->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_w->SetText(_T("w"));
		m_pcKbrd_w->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_e;
		btn.wID					= IDC_BTN_SETUP_KBRD_e;	
		btn.poPosition.x		= 135+60;
		btn.poPosition.y		= 45;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_e=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_e->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_e->SetText(_T("e"));
		m_pcKbrd_e->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_r;
		btn.wID					= IDC_BTN_SETUP_KBRD_r;	
		btn.poPosition.x		= 180+60;
		btn.poPosition.y		= 45;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_r=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_r->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_r->SetText(_T("r"));
		m_pcKbrd_r->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_t;
		btn.wID					= IDC_BTN_SETUP_KBRD_t;	
		btn.poPosition.x		= 225+60;
		btn.poPosition.y		= 45;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_t=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_t->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_t->SetText(_T("t"));
		m_pcKbrd_t->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_y;
		btn.wID					= IDC_BTN_SETUP_KBRD_y;	
		btn.poPosition.x		= 270+60;
		btn.poPosition.y		= 45;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_y=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_y->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_y->SetText(_T("y"));
		m_pcKbrd_y->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_u;
		btn.wID					= IDC_BTN_SETUP_KBRD_u;	
		btn.poPosition.x		= 315+60;
		btn.poPosition.y		= 45;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_u=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_u->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_u->SetText(_T("u"));
		m_pcKbrd_u->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_i;
		btn.wID					= IDC_BTN_SETUP_KBRD_i;	
		btn.poPosition.x		= 360+60;
		btn.poPosition.y		= 45;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_i=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_i->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_i->SetText(_T("i"));
		m_pcKbrd_i->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_o;
		btn.wID					= IDC_BTN_SETUP_KBRD_o;	
		btn.poPosition.x		= 405+60;
		btn.poPosition.y		= 45;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_o=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_o->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_o->SetText(_T("o"));
		m_pcKbrd_o->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_p;
		btn.wID					= IDC_BTN_SETUP_KBRD_p;	
		btn.poPosition.x		= 450+60;
		btn.poPosition.y		= 45;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_p=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_p->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_p->SetText(_T("p"));
		m_pcKbrd_p->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_ü;
		btn.wID					= IDC_BTN_SETUP_KBRD_ue;	
		btn.poPosition.x		= 495+60;
		btn.poPosition.y		= 45;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_ue=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_ue->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_ue->SetText(_T("ü"));
		m_pcKbrd_ue->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_plus;
		/*btn.wID					= IDC_BTN_SETUP_KBRD_plus;	
		btn.poPosition.x		= 545;
		btn.poPosition.y		= 345;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_plus=new CBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_plus->Create(this,g_hf15AcuMed,0);
		m_pcKbrd_plus->SetText(_T("+"));
		m_pcKbrd_plus->ShowWindow(SW_SHOW);*/

		/***********************************************************/
		//CBtn* m_pcKbrd_a;
		btn.wID					= IDC_BTN_SETUP_KBRD_a;	
		btn.poPosition.x		= 45+60;
		btn.poPosition.y		= 90;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_a=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_a->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_a->SetText(_T("a"));
		m_pcKbrd_a->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_s;
		btn.wID					= IDC_BTN_SETUP_KBRD_s;	
		btn.poPosition.x		= 90+60;
		btn.poPosition.y		= 90;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_s=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_s->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_s->SetText(_T("s"));
		m_pcKbrd_s->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_d;
		btn.wID					= IDC_BTN_SETUP_KBRD_d;	
		btn.poPosition.x		= 135+60;
		btn.poPosition.y		= 90;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_d=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_d->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_d->SetText(_T("d"));
		m_pcKbrd_d->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_f;
		btn.wID					= IDC_BTN_SETUP_KBRD_f;	
		btn.poPosition.x		= 180+60;
		btn.poPosition.y		= 90;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_f=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_f->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_f->SetText(_T("f"));
		m_pcKbrd_f->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_g;
		btn.wID					= IDC_BTN_SETUP_KBRD_g;	
		btn.poPosition.x		= 225+60;
		btn.poPosition.y		= 90;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_g=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_g->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_g->SetText(_T("g"));
		m_pcKbrd_g->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_h;
		btn.wID					= IDC_BTN_SETUP_KBRD_h;	
		btn.poPosition.x		= 270+60;
		btn.poPosition.y		= 90;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_h=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_h->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_h->SetText(_T("h"));
		m_pcKbrd_h->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_j;
		btn.wID					= IDC_BTN_SETUP_KBRD_j;	
		btn.poPosition.x		= 315+60;
		btn.poPosition.y		= 90;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_j=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_j->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_j->SetText(_T("j"));
		m_pcKbrd_j->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_k;
		btn.wID					= IDC_BTN_SETUP_KBRD_k;	
		btn.poPosition.x		= 360+60;
		btn.poPosition.y		= 90;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_k=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_k->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_k->SetText(_T("k"));
		m_pcKbrd_k->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_l;
		btn.wID					= IDC_BTN_SETUP_KBRD_l;	
		btn.poPosition.x		= 405+60;
		btn.poPosition.y		= 90;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_l=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_l->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_l->SetText(_T("l"));
		m_pcKbrd_l->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_ö;
		btn.wID					= IDC_BTN_SETUP_KBRD_oe;	
		btn.poPosition.x		= 450+60;
		btn.poPosition.y		= 90;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_oe=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_oe->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_oe->SetText(_T("ö"));
		m_pcKbrd_oe->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_ä;
		btn.wID					= IDC_BTN_SETUP_KBRD_ae;	
		btn.poPosition.x		= 495+60;
		btn.poPosition.y		= 90;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_ae=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_ae->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_ae->SetText(_T("ä"));
		m_pcKbrd_ae->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_sharp;
		/*btn.wID					= IDC_BTN_SETUP_KBRD_sharp;	
		btn.poPosition.x		= 545;
		btn.poPosition.y		= 390;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_sharp=new CBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_sharp->Create(this,g_hf15AcuMed,0);
		m_pcKbrd_sharp->SetText(_T("#"));
		m_pcKbrd_sharp->ShowWindow(SW_SHOW);*/

		/***********************************************************/
		//CBtn* m_pcKbrd_z;
		btn.wID					= IDC_BTN_SETUP_KBRD_z;	
		btn.poPosition.x		= 45+60;
		btn.poPosition.y		= 135;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_z=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_z->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_z->SetText(_T("z"));
		m_pcKbrd_z->ShowWindow(SW_SHOW);


		//CBtn* m_pcKbrd_x;
		btn.wID					= IDC_BTN_SETUP_KBRD_x;	
		btn.poPosition.x		= 90+60;
		btn.poPosition.y		= 135;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_x=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_x->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_x->SetText(_T("x"));
		m_pcKbrd_x->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_c;
		btn.wID					= IDC_BTN_SETUP_KBRD_c;	
		btn.poPosition.x		= 135+60;
		btn.poPosition.y		= 135;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_c=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_c->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_c->SetText(_T("c"));
		m_pcKbrd_c->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_v;
		btn.wID					= IDC_BTN_SETUP_KBRD_v;	
		btn.poPosition.x		= 180+60;
		btn.poPosition.y		= 135;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_v=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_v->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_v->SetText(_T("v"));
		m_pcKbrd_v->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_b;
		btn.wID					= IDC_BTN_SETUP_KBRD_b;	
		btn.poPosition.x		= 225+60;
		btn.poPosition.y		= 135;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_b=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_b->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_b->SetText(_T("b"));
		m_pcKbrd_b->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_n;
		btn.wID					= IDC_BTN_SETUP_KBRD_n;	
		btn.poPosition.x		= 270+60;
		btn.poPosition.y		= 135;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_n=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_n->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_n->SetText(_T("n"));
		m_pcKbrd_n->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_m;
		btn.wID					= IDC_BTN_SETUP_KBRD_m;	
		btn.poPosition.x		= 315+60;
		btn.poPosition.y		= 135;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_m=new CKbrdBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_m->Create(this,g_hf17AcuBold,0);
		m_pcKbrd_m->SetText(_T("m"));
		m_pcKbrd_m->ShowWindow(SW_SHOW);

		//CBtn* m_pcKbrd_semicol;
		/*btn.wID					= IDC_BTN_SETUP_KBRD_semicol;	
		btn.poPosition.x		= 365;
		btn.poPosition.y		= 435;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_semicol=new CBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_semicol->Create(this,g_hf15AcuMed,0);
		m_pcKbrd_semicol->SetText(_T(";"));
		m_pcKbrd_semicol->ShowWindow(SW_SHOW);*/

		//CBtn* m_pcKbrd_point;
		/*btn.wID					= IDC_BTN_SETUP_KBRD_point;	
		btn.poPosition.x		= 410;
		btn.poPosition.y		= 435;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_point=new CBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_point->Create(this,g_hf15AcuMed,0);
		m_pcKbrd_point->SetText(_T(":"));
		m_pcKbrd_point->ShowWindow(SW_SHOW);*/

		//CBtn* m_pcKbrd_dash;
		/*btn.wID					= IDC_BTN_SETUP_KBRD_dash;	
		btn.poPosition.x		= 455;
		btn.poPosition.y		= 435;
		btn.pcBmpUp				= m_pcKbrdAlphanum_Up;
		btn.pcBmpDown			= m_pcKbrdAlphanum_Dw;
		btn.pcBmpFocus			= m_pcKbrdAlphanum_Up;
		btn.pcBmpDisabled		= m_pcKbrdAlphanum_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;
		m_pcKbrd_dash=new CBtn(btn,COLOR_TXTBTNUP);
		m_pcKbrd_dash->Create(this,g_hf15AcuMed,0);
		m_pcKbrd_dash->SetText(_T("_"));
		m_pcKbrd_dash->ShowWindow(SW_SHOW);*/


		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();//rkuNEWFIX

		return 1;
	}
	else
		return 0;
}





void CWndSetupKeyboard::Show(bool bShow)
{
	if(bShow)
	{
		this->ShowWindow(SW_SHOW);

		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);

		//Draw();
	}
	else
	{
		this->ShowWindow(SW_HIDE);
		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
	}


}
void CWndSetupKeyboard::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	// CWnd::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

//************************************
// Method:    OnDestroy
// FullName:  CWndSetupKeyboard::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndSetupKeyboard::OnDestroy()
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

//void CWndSetupKeyboard::OnTimer(UINT_PTR nIDEvent)
//{
//
//
//	//if(nIDEvent==CLOCKTIMER)
//	//{
//	//	/*struct tm *oTime;
//	//	time_t t;
//	//	time(&t);
//	//	oTime = gmtime(&t);*/
//	//	m_Analog.m_Country = "GMT";
//	//	// the name of the Time-Zone
//	//	//m_Analog.SetTime(oTime);
//	//	m_Analog.UpdateTime();
//
//	//}
//
//
//	CWnd::OnTimer(nIDEvent);
//}




void CWndSetupKeyboard::SetEditWnd(CWnd* rEdit, eKeyRange range, int iID)
{
	m_pEdit=rEdit;
	m_eKeyRange=range;
	m_iID=iID;
}

//void CWndSetupKeyboard::Draw()
//{
//	RECT rcCl;
//	RECT rc;
//	GetClientRect(&rcCl);
//
//	CClientDC dc(this);
//
//
//
//	//int bc=SetBkColor(m_hDC,BACKGND);
//	//int bc=SetBkColor(m_hDC,RGB(181,178,181));
//
//	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
//	//FillRect(m_hDC,&rcCl,CBrush(BACKGND));
//
//	rc.left = 50;  
//	rc.top = 50;  
//	rc.right  = 592;  
//	rc.bottom = 400;
//
//	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf21AcuBold);
//
//
//	int tc=SetTextColor(m_hDC,0x0000000);
//
//
//	CStringW cs = _T("Patientendaten");
//
//
//	DrawText(m_hDC,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
//
//
//
//
//	//dc.BitBlt(0,0,rcCl.right,rcCl.bottom,CDC::FromHandle(m_hDC),0,0,SRCCOPY);
//	//BitBlt(m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);
//	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
//
//
//	SetTextColor(m_hDC,tc);
//	//SetBkColor(m_hDC,bc);
//	SetBkMode(m_hDC,nBkMode);
//
//
//
//	SelectObject(m_hDC,hPrevFont);
//
//}

//=============================================================================
void CWndSetupKeyboard::Key(int c)
{
	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
	if(m_pEdit)
	{
		if(c == -1)
		{
			m_pEdit->SendMessage(WM_CHAR, VK_BACK, 1);
		}
		else
		{
			switch(m_eKeyRange)
			{
			case KR_ALPHANUM:
				{
					if(isalpha(c))
						m_pEdit->SendMessage(WM_CHAR, c, 1);
				}
				break;
			case KR_ALPHA:
				{
					if(isalpha(c))
						m_pEdit->SendMessage(WM_CHAR, c, 1);
				}
				break;
			case KR_NUMBERS:
				{
					 if(isdigit(c))
						 m_pEdit->SendMessage(WM_CHAR, c, 1);
				}
				break;
			case KR_DAYS:
				{
					CStringW szVal=_T("");
					if(isdigit(c))
					{
						m_pEdit->SendMessage(WM_CHAR, c, 1);
						m_pEdit->GetWindowText(szVal);
						int iVal=_wtoi(szVal);

						
						if(iVal<0 || iVal>31)
						{
							if(GetParent())
								GetParent()->PostMessage(WM_INVALID_DAY);
							m_pEdit->SetWindowText(_T(""));
						}
					}
					else
					{
						if(GetParent())
							GetParent()->PostMessage(WM_INVALID_DAY);
						//m_pEdit->SetWindowText(_T(""));
					}
				}
				break;
			case KR_MONTH:
				{
					//int iTest=m_pEdit->GetDlgCtrlID();
					CStringW szVal=_T("");
					if(isdigit(c))
					{
						m_pEdit->SendMessage(WM_CHAR, c, 1);
						m_pEdit->GetWindowText(szVal);
						int iVal=_wtoi(szVal);
						if(iVal<0 || iVal>12)
						{
							if(GetParent())
								GetParent()->PostMessage(WM_INVALID_MONTH);
							m_pEdit->SetWindowText(_T(""));
						}
					}
					else
					{
						if(GetParent())
							GetParent()->PostMessage(WM_INVALID_MONTH);
						//m_pEdit->SetWindowText(_T(""));
					}
					
					
				}
				break;
			case KR_YEAR:
				{
					CStringW szVal=_T("");
					SYSTEMTIME st;
					GetLocalTime(&st);
					COleDateTime dtBirthday(st);

					if(isdigit(c))
					{
						m_pEdit->SendMessage(WM_CHAR, c, 1);
						m_pEdit->GetWindowText(szVal);

						int iVal=_wtoi(szVal);
						if(iVal>dtBirthday.GetYear())
						{
							if(GetParent())
								GetParent()->PostMessage(WM_INVALID_YEAR);
							szVal.Format(_T("%04d"),dtBirthday.GetYear());
							m_pEdit->SetWindowText(szVal);
						}
					}
					else
					{
						if(GetParent())
							GetParent()->PostMessage(WM_INVALID_YEAR);
						
						/*szVal.Format(_T("%04d"),dtBirthday.GetYear());
						m_pEdit->SetWindowText(szVal);*/
					}
				}
				break;
			default:
				{
					m_pEdit->SendMessage(WM_CHAR, c, 1);
				}
				break;
			}
			
		}
		m_pEdit->SetFocus();
		//m_pEdit->SetCapture();

		if(GetParent())
			GetParent()->PostMessage(WM_PATIENTDATA);
	}
	
}


void CWndSetupKeyboard::OnBnClickedDel()
{
	Key(-1);
}

void CWndSetupKeyboard::OnBnClickedSpace()
{
	Key(' ');
}
void CWndSetupKeyboard::OnBnClickedShift()
{
	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);

	RedrawButtons();
}


void CWndSetupKeyboard::OnBnClicked_1()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('!');
	}
	else
	{
		Key('1');
	}
}
void CWndSetupKeyboard::OnBnClicked_2()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('?');
	}
	else
	{
		Key('2');
	}
}
void CWndSetupKeyboard::OnBnClicked_3()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('.');
	}
	else
	{
		Key('3');
	}
}
void CWndSetupKeyboard::OnBnClicked_4()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key(',');
	}
	else
	{
		Key('4');
	}
}
void CWndSetupKeyboard::OnBnClicked_5()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key(':');
	}
	else
	{
		Key('5');
	}
}
void CWndSetupKeyboard::OnBnClicked_6()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('+');
	}
	else
	{
		Key('6');
	}
}
void CWndSetupKeyboard::OnBnClicked_7()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('-');
	}
	else
	{
		Key('7');
	}
}
void CWndSetupKeyboard::OnBnClicked_8()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('=');
	}
	else
	{
		Key('8');
	}
}
void CWndSetupKeyboard::OnBnClicked_9()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('(');
	}
	else
	{
		Key('9');
	}
}
void CWndSetupKeyboard::OnBnClicked_0()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key(')');
	}
	else
	{
		Key('0');
	}
}
void CWndSetupKeyboard::OnBnClicked_sz()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('%');
	}
	else
	{
		//Key('ß');
		Key(0xDF);
	}
}
void CWndSetupKeyboard::OnBnClicked_q()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('Q');
	}
	else
	{
		Key('q');
	}
}
void CWndSetupKeyboard::OnBnClicked_w()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('W');
	}
	else
	{
		Key('w');
	}
}
void CWndSetupKeyboard::OnBnClicked_e()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('E');
	}
	else
	{
		Key('e');
	}
}
void CWndSetupKeyboard::OnBnClicked_r()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('R');
	}
	else
	{
		Key('r');
	}
}
void CWndSetupKeyboard::OnBnClicked_t()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('T');
	}
	else
	{
		Key('t');
	}
}
void CWndSetupKeyboard::OnBnClicked_z()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('Z');
	}
	else
	{
		Key('z');
	}
}
void CWndSetupKeyboard::OnBnClicked_u()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('U');
	}
	else
	{
		Key('u');
	}
}
void CWndSetupKeyboard::OnBnClicked_i()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('I');
	}
	else
	{
		Key('i');
	}
}
void CWndSetupKeyboard::OnBnClicked_o()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('O');
	}
	else
	{
		Key('o');
	}
}
void CWndSetupKeyboard::OnBnClicked_p()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('P');
	}
	else
	{
		Key('p');
	}
}
void CWndSetupKeyboard::OnBnClicked_ue()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		//Key('Ü');
		Key(0xDC);
	}
	else
	{
		//Key('ü');
		Key(0xFC);
	}
}
void CWndSetupKeyboard::OnBnClicked_a()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('A');
	}
	else
	{
		Key('a');
	}
}
void CWndSetupKeyboard::OnBnClicked_s()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('S');
	}
	else
	{
		Key('s');
	}
}
void CWndSetupKeyboard::OnBnClicked_d()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('D');
	}
	else
	{
		Key('d');
	}
}
void CWndSetupKeyboard::OnBnClicked_f()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('F');
	}
	else
	{
		Key('f');
	}
}
void CWndSetupKeyboard::OnBnClicked_g()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('G');
	}
	else
	{
		Key('g');
	}
}
void CWndSetupKeyboard::OnBnClicked_h()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('H');
	}
	else
	{
		Key('h');
	}
}
void CWndSetupKeyboard::OnBnClicked_j()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('J');
	}
	else
	{
		Key('j');
	}
}
void CWndSetupKeyboard::OnBnClicked_k()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('K');
	}
	else
	{
		Key('k');
	}
}
void CWndSetupKeyboard::OnBnClicked_l()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('L');
	}
	else
	{
		Key('l');
	}
}
void CWndSetupKeyboard::OnBnClicked_oe()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		//Key('Ö');
		Key(0xD6);
	}
	else
	{
		//Key('ö');
		Key(0xF6);
	}
}
void CWndSetupKeyboard::OnBnClicked_ae()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		//Key('Ä');
		Key(0xC4);
	}
	else
	{
		//Key('ä');
		Key(0xE4);
	}
}
void CWndSetupKeyboard::OnBnClicked_y()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('Y');
	}
	else
	{
		Key('y');
	}
}
void CWndSetupKeyboard::OnBnClicked_x()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('X');
	}
	else
	{
		Key('x');
	}
}
void CWndSetupKeyboard::OnBnClicked_c()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('C');
	}
	else
	{
		Key('c');
	}
}
void CWndSetupKeyboard::OnBnClicked_v()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('V');
	}
	else
	{
		Key('v');
	}
}
void CWndSetupKeyboard::OnBnClicked_b()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('B');
	}
	else
	{
		Key('b');
	}
}
void CWndSetupKeyboard::OnBnClicked_n()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('N');
	}
	else
	{
		Key('n');
	}
}
void CWndSetupKeyboard::OnBnClicked_m()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		Key('M');
	}
	else
	{
		Key('m');
	}
}

void CWndSetupKeyboard::RedrawButtons()
{
	if(m_pcKbrd_Shift->IsDepressed())
	{
		m_pcKbrd_q->RefreshText(_T("Q"));
		m_pcKbrd_w->RefreshText(_T("W"));
		m_pcKbrd_e->RefreshText(_T("E"));
		m_pcKbrd_r->RefreshText(_T("R"));
		m_pcKbrd_t->RefreshText(_T("T"));
		m_pcKbrd_y->RefreshText(_T("Y"));
		m_pcKbrd_u->RefreshText(_T("U"));
		m_pcKbrd_i->RefreshText(_T("I"));
		m_pcKbrd_o->RefreshText(_T("O"));
		m_pcKbrd_p->RefreshText(_T("P"));
		m_pcKbrd_ue->RefreshText(_T("Ü"));
		m_pcKbrd_a->RefreshText(_T("A"));
		m_pcKbrd_s->RefreshText(_T("S"));
		m_pcKbrd_d->RefreshText(_T("D"));
		m_pcKbrd_f->RefreshText(_T("F"));
		m_pcKbrd_g->RefreshText(_T("G"));
		m_pcKbrd_h->RefreshText(_T("H"));
		m_pcKbrd_j->RefreshText(_T("J"));
		m_pcKbrd_k->RefreshText(_T("K"));
		m_pcKbrd_l->RefreshText(_T("L"));
		m_pcKbrd_oe->RefreshText(_T("Ö"));
		m_pcKbrd_ae->RefreshText(_T("Ä"));
		m_pcKbrd_z->RefreshText(_T("Z"));
		m_pcKbrd_x->RefreshText(_T("X"));
		m_pcKbrd_c->RefreshText(_T("C"));
		m_pcKbrd_v->RefreshText(_T("V"));
		m_pcKbrd_b->RefreshText(_T("B"));
		m_pcKbrd_n->RefreshText(_T("N"));
		m_pcKbrd_m->RefreshText(_T("M"));
	}
	else
	{
		m_pcKbrd_q->RefreshText(_T("q"));
		m_pcKbrd_w->RefreshText(_T("w"));
		m_pcKbrd_e->RefreshText(_T("e"));
		m_pcKbrd_r->RefreshText(_T("r"));
		m_pcKbrd_t->RefreshText(_T("t"));
		m_pcKbrd_y->RefreshText(_T("y"));
		m_pcKbrd_u->RefreshText(_T("u"));
		m_pcKbrd_i->RefreshText(_T("i"));
		m_pcKbrd_o->RefreshText(_T("o"));
		m_pcKbrd_p->RefreshText(_T("p"));
		m_pcKbrd_ue->RefreshText(_T("ü"));
		m_pcKbrd_a->RefreshText(_T("a"));
		m_pcKbrd_s->RefreshText(_T("s"));
		m_pcKbrd_d->RefreshText(_T("d"));
		m_pcKbrd_f->RefreshText(_T("f"));
		m_pcKbrd_g->RefreshText(_T("g"));
		m_pcKbrd_h->RefreshText(_T("h"));
		m_pcKbrd_j->RefreshText(_T("j"));
		m_pcKbrd_k->RefreshText(_T("k"));
		m_pcKbrd_l->RefreshText(_T("l"));
		m_pcKbrd_oe->RefreshText(_T("ö"));
		m_pcKbrd_ae->RefreshText(_T("ä"));
		m_pcKbrd_z->RefreshText(_T("z"));
		m_pcKbrd_x->RefreshText(_T("x"));
		m_pcKbrd_c->RefreshText(_T("c"));
		m_pcKbrd_v->RefreshText(_T("v"));
		m_pcKbrd_b->RefreshText(_T("b"));
		m_pcKbrd_n->RefreshText(_T("n"));
		m_pcKbrd_m->RefreshText(_T("m"));
	}

	if(m_pEdit)
		m_pEdit->SetFocus();
}

void CWndSetupKeyboard::BnClickedFocusLeft()
{
	if(GetParent())
		GetParent()->PostMessage(WM_PATIENT_FOCUS_LEFT,m_iID);
}
void CWndSetupKeyboard::BnClickedFocusRight()
{
	if(GetParent())
		GetParent()->PostMessage(WM_PATIENT_FOCUS_RIGHT,m_iID);
}
void CWndSetupKeyboard::BnClickedFocusUp()
{
	if(GetParent())
		GetParent()->PostMessage(WM_PATIENT_FOCUS_UP,m_iID);
}
void CWndSetupKeyboard::BnClickedFocusDown()
{
	if(GetParent())
		GetParent()->PostMessage(WM_PATIENT_FOCUS_DOWN,m_iID);
}

// **************************************************************************
// 
// **************************************************************************
LRESULT CWndSetupKeyboard::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{

			switch(wParam)
			{
			case IDC_BTN_SETUP_FOCUS_LEFT:
				{
					BnClickedFocusLeft();
					return 1;
				}
				break;
			case IDC_BTN_SETUP_FOCUS_RIGHT:
				{
					BnClickedFocusRight();
					return 1;
				}
				break;
			case IDC_BTN_SETUP_FOCUS_UP:
				{
					BnClickedFocusUp();
					return 1;
				}
				break;
			case IDC_BTN_SETUP_FOCUS_DOWN:
				{
					BnClickedFocusDown();
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