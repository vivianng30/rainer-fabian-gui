#pragma once
//#include "../MVViewHandler.h"
#include "Btn.h"
#include "TlsBlob.h"
//#include "../MVModel.h"
#include "MVModel.h"


//typedef struct tagfVALUE
//{
//	int iValue;
//	int iUpperLimit;
//	int iLowerLimit;
//	TCHAR tText[16];
//}fVALUE;

// CParaBtn
//global font objects
//extern HFONT g_hf13AcuBold;
//extern HFONT g_hf14AcuBold;
//extern HFONT g_hf10AcuBold;
//extern HFONT g_hf11AcuBold;
//extern HFONT g_hf31AcuBold;
//extern HFONT g_hf27AcuBold;
//extern HFONT g_hf23AcuBold;
extern HFONT g_hf17AcuBold;

extern HFONT g_hf7AcuBold;
extern HFONT g_hf9AcuBold;

extern HFONT g_hf31AcuBold;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf31AcuBoldNum;

class CParaBtn : public CButton
{
	DECLARE_DYNAMIC(CParaBtn)



public:
	explicit CParaBtn(BTN &btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CParaBtn();
	virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	inline void GetButtonColors(BTNCOLORS* pbtncr){memcpy(pbtncr,&m_btncr,sizeof(BTNCOLORS));};
	inline void SetButtonColors(BTNCOLORS* pbtncr){
		memcpy(&m_btncr,pbtncr,sizeof(BTNCOLORS));};

	virtual void SetValueText(TCHAR* pszText,bool bTextOnly=false);
	virtual void SetValueText(CStringW& szText,bool bTextOnly=false);
	virtual void SetNameText(TCHAR* pszText, bool bRefresh=false);
	virtual void SetNameText(CStringW& szText, bool bRefresh=false);
	virtual void SetNameNoteText(TCHAR* pszText, bool bRefresh=false);
	virtual void SetNameNoteText(CStringW& szText, bool bRefresh=false);
	virtual void SetUnitText(TCHAR* pszText);
	virtual void SetUnitText(CStringW& szText);
	virtual void RefreshValueText(CStringW& szText,bool bTextOnly=false);
	virtual void RefreshBtn();
	virtual void RefreshBtnState();
	//void SSTextOut(CDC* pDC, CStringW data , CRect* drawRect, int justification);
	//void DrawDirectDown(void);
	virtual void DrawDirectUp(bool bCheckValueChanged=false);
	//virtual void DrawDirectFocused(void);

	void SetColors(COLORREF crTxtUp,COLORREF crTxtDown,COLORREF crSubTxtDown,COLORREF crTxtFocus, COLORREF crDisabel=0x00828282,bool bRefresh=false);

	virtual void SetParaValue(int v);
	virtual void SetValue(fVALUE v, bool bRedraw=false);
	virtual void GetValue(fVALUE* pv){memcpy(pv,&m_v,sizeof(fVALUE));};

	virtual bool GetButton(int* pnValue=NULL);
	virtual bool SetLimits(int nLower,int nUpper);

	inline void SetTextFormat(DWORD dwFormat){m_btn.dwFormat=dwFormat;};
	//inline void SetDepress(void){m_bButtonIsActive=true;};

	bool IsDepressed( void );
	bool Depress();

	bool IsOff();
	void SetOffState(bool state);
	//void SetAuto(bool bAuto,bool bRedraw);


	void SetUDKeys(WORD kUP, WORD kDown, WORD kSpace);

	bool DisableValueText(bool bDisable=true);

	//void DrawDirect(void);

	//void SetGroupID(int ID);
	//int GetGroupID();
	int GetBtnId();

	void ClearArrows();
	void SetArrowDown(bool state=true);
	void SetArrowUp(bool state=true);

	void SetAlarmArrowDown(bool state=true);
	void SetAlarmArrowUp(bool state=true);

	void SetWarning(bool state=true);

	void SetKeyValueAccepted(bool state);


	void SetSignaled(bool bSignaled);
	void SetBitmaps(CBmp*	pcBmpUp,
		CBmp*	pcBmpDown,
		CBmp*	pcBmpDisabled,
		CBmp*	pcBmpFocus,
		bool bSignaled,
		bool bRedraw);

protected:

	bool isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay=0);//used for (dwLastTickCount+DELAY<getTickCount64())

	void WriteCurrentValue();

	DWORD GetTimeDiff(DWORD dwStart);
	CMVModel *getModel();


	// Attributes
protected:
	CMVModel* m_pModel;


	BTNCOLORS m_btncr;
	BTN m_btn;
	HDC m_hDC;
	HBITMAP m_hBm;
	HBITMAP m_hBmPrev;
	RECT m_rcClient;

	DWORD m_dwPushDelta;

	TCHAR m_pszValueText[64];
	TCHAR m_pszNameText[64];
	TCHAR m_pszNameNoteText[64];
	TCHAR m_pszUnitText[64];
	bool m_bTextOnly;
	fVALUE m_v;
	int m_nOffset;
	bool m_bDisableText;
	bool m_bScrollOver;
	WORD m_kUp;
	WORD m_kDown;
	WORD m_kSpace;

	int m_iPrevValue;
	bool m_bLastParaState;

	bool m_bTurnedOff;
	bool m_bHasFocus;
	bool m_bDepressed;
	bool m_bLMouseButtonDown;
	bool m_bLButtonDown;
	bool m_bEndOfRange;
	bool m_bOff;
	bool m_bLimited;
	bool m_bDrawArrowDown;
	bool m_bDrawArrowUp;
	bool m_bKeyValueAccepted;
	bool m_bWaitConfirm;
	bool m_bDrawKey;
	bool m_bDrawWarning;
	//bool m_bAutoState;
	bool m_bSignaled;
	bool m_bDrawAlarmArrowDown;
	bool m_bDrawAlarmArrowUp;

	BOOL m_bKeyBeep;

	//int m_iCountUDP;
	DWORD m_dwSpeedDelta;
	

	//CBmp* m_pcArrow_Up;
	//CBmp* m_pcArrow_Dw;
	CBmp* m_pcArrowUp_GREY;
	CBmp* m_pcArrowUp_RED;
	CBmp* m_pcArrowDw_RED;
	CBmp* m_pcArrowUp_UP;
	CBmp* m_pcArrowUp_FC;
	CBmp* m_pcArrowUp_DW;
	CBmp* m_pcArrowDw_UP;
	CBmp* m_pcArrowDw_FC;
	CBmp* m_pcArrowDw_DW;
	CBmp* m_pcKey;
	CBmp* m_pcWarning_Dis;
	CBmp* m_pcWarning_Up;
	CBmp* m_pcWarning_Fc;
	CBmp* m_pcWarning_Dw;
	CBmp* m_pcWarning_Red;
	CBmp* m_pcBmpFocusDown;

	//CBmp* m_pcAlarmArrowUp_RED;
	//CBmp* m_pcAlarmArrowDw_RED;
	CBmp* m_pcAlarmArrowUp_UP;
	CBmp* m_pcAlarmArrowUp_FC;
	CBmp* m_pcAlarmArrowUp_DW;
	CBmp* m_pcAlarmArrowDw_UP;
	CBmp* m_pcAlarmArrowDw_FC;
	CBmp* m_pcAlarmArrowDw_DW;


	//CMVViewHandler* m_pViewHandler;
public:

	// Operations
public:
	virtual void Draw(int nState);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParaBtn)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


	// Generated message map functions
protected:
	//{{AFX_MSG(CParaBtn)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	/*afx_msg void OnSetFocus(CWnd* pOldWnd);*/
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


