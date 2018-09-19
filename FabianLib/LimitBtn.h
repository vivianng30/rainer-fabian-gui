/**********************************************************************************************//**
 * \file	LimitBtn.h.
 *
 * Declares the limit button class
 **************************************************************************************************/

#pragma once
#include "Btn.h"
#include "TlsBlob.h"
#include "MVModel.h"


// CLimitBtn
extern HFONT g_hf7AcuBold;
extern HFONT g_hf9AcuBold;

extern HFONT g_hf31AcuBold;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf31AcuBoldNum;

class CWndDataSPO2;

class CLimitBtn : public CButton
{
	DECLARE_DYNAMIC(CLimitBtn)

public:
	explicit CLimitBtn(CWndDataSPO2 *parent, BTN &btn, int nOffset=0,bool bScrollOver=true);
	virtual ~CLimitBtn();
	virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	inline void GetButtonColors(BTNCOLORS* pbtncr){memcpy(pbtncr,&m_btncr,sizeof(BTNCOLORS));};
	inline void SetButtonColors(BTNCOLORS* pbtncr){
		memcpy(&m_btncr,pbtncr,sizeof(BTNCOLORS));};

		virtual void SetValueText(TCHAR* pszText,bool bTextOnly=false);
		virtual void SetValueText(CStringW& szText,bool bTextOnly=false);
		virtual void SetNameText(TCHAR* pszText);
		virtual void SetNameText(CStringW& szText);
		virtual void SetNameNoteText(TCHAR* pszText, bool bRefresh=false);
		virtual void SetNameNoteText(CStringW& szText, bool bRefresh=false);
		virtual void SetUnitText(TCHAR* pszText);
		virtual void SetUnitText(CStringW& szText);
		virtual void RefreshValueText(CStringW& szText,bool bTextOnly=false);
		virtual void RefreshBtn();
		virtual void RefreshBtnState();
		virtual void DrawDirectUp(bool bCheckValueChanged=false);

		void SetColors(COLORREF crTxtUp,COLORREF crTxtDown,COLORREF crSubTxtDown,COLORREF crTxtFocus, COLORREF crDisabel=0x00828282,bool bRefresh=false);

		virtual void SetParaValue(int v);
		virtual void SetValue(fVALUE v, bool bRedraw=false);
		virtual void GetValue(fVALUE* pv){memcpy(pv,&m_v,sizeof(fVALUE));};

		virtual bool GetButton(int* pnValue=NULL);
		virtual bool SetLimits(int nLower,int nUpper);

		inline void SetTextFormat(DWORD dwFormat){m_btn.dwFormat=dwFormat;};

		bool IsDepressed( void );
		bool Depress();

		bool IsOff();
		void SetOffState(bool state);

		void SetUDKeys(WORD kUP, WORD kDown, WORD kSpace);

		bool DisableValueText(bool bDisable=true);

		int GetBtnId();

		void SetBitmaps(CBmp*	pcBmpUp,
			CBmp*	pcBmpDown,
			CBmp*	pcBmpDisabled,
			CBmp*	pcBmpFocus,
            bool    bSignaled,
			bool bRedraw);

protected:
	void WriteCurrentValue();

	CMVModel *getModel();

	bool isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay=0);//used for (dwLastTickCount+DELAY<getTickCount64())
	// Attributes
protected:
	static CWndDataSPO2* WNDDATASPO2;
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

	

	CBmp* m_pcBmpFocusDown;

public:

	// Operations
public:
	virtual void Draw(int nState);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLimitBtn)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CLimitBtn)
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


