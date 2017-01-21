#pragma once
#include "Btn.h"
#include "TlsBlob.h"
#include "MVModel.h"

//class CViewAlarmLimit;

typedef struct tagfALVALUE
{
	int iAbsoluteUpperLimit;
	int iAbsoluteLowerLimit;
	int iCurrentLimit;
}fALVALUE;

// CParaBtn
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
extern HFONT g_hf13AcuBoldNum;
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

class CALimitBtn : public CButton
{
	DECLARE_DYNAMIC(CALimitBtn)

public:
	explicit CALimitBtn(const BTN &btn);
	virtual ~CALimitBtn();
	virtual BOOL Create(CWnd* pParentWnd,DWORD dwStyle, fALVALUE &v);

	inline void GetButtonColors(BTNCOLORS* pbtncr){memcpy(pbtncr,&m_btncr,sizeof(BTNCOLORS));};
	inline void SetButtonColors(BTNCOLORS* pbtncr){
		memcpy(&m_btncr,pbtncr,sizeof(BTNCOLORS));};

	/*virtual void SetNameText(TCHAR* pszText);
	virtual void SetNameText(CStringW& szText);
	virtual void SetUnitText(TCHAR* pszText);
	virtual void SetUnitText(CStringW& szText);*/
	virtual void SetCurLimit(int val, bool bRedraw=false);
	//virtual void GetLimits(fALVALUE* pv){memcpy(pv,&m_v,sizeof(fALVALUE));};
	virtual void DrawDirectUp();
	virtual void DrawDirectDown();
	virtual void Redraw();
	//virtual void SetMessureData(int iData);
	
	void SetColors(COLORREF crTxtUp,COLORREF crTxtDown,COLORREF crSubTxtDown,COLORREF crTxtFocus, COLORREF crDisabel=0x00828282);

	virtual bool SetAbsoluteLimits(int nLower,int nUpper);
	
	//void ShowAlarm(bool state, bool bRedraw);
	void setAlarm(eStateOfAlarm state=AS_NONE, bool bRedraw=true);
	eStateOfAlarm GetAlarm();
	//void SetAuto(bool state);
	void SetAlarmLimitState(eAlarmLimitState state);
	void SetLimitRange(int iLimit);

	void SetUDKeys(WORD kUP, WORD kDown, WORD kSpace);

	int GetBtnId();
	bool IsDepressed( void );

	DWORD GetTimeDiff(DWORD dwStart);


protected:
	//bool DrawData(/*bool bFrames, bool bText, bool bLimits*/);
	void WriteCurrentValue();
	CMVModel *getModel();

	bool isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay=0);//used for (dwLastTickCount+DELAY<getTickCount64())

		// Attributes
private:
	CMVModel* m_pModel;


	CBmp* m_pcClock_Up;
	CBmp* m_pcClock_Dw;
	CBmp* m_pcClock_Fc;
	CBmp* m_pcClockInactive_Up;
	CBmp* m_pcClockInactive_Dw;
	CBmp* m_pcClockInactive_Fc;

	CBmp* m_pcLimitHigh_Up;
	CBmp* m_pcLimitHigh_Dw;
	CBmp* m_pcLimitHigh_Fc;
	CBmp* m_pcLimitLow_Up;
	CBmp* m_pcLimitLow_Dw;
	CBmp* m_pcLimitLow_Fc;

	HBITMAP m_hBm;
	HBITMAP m_hBmPrev;
	HDC m_hDC;
	BTNCOLORS m_btncr;
	fALVALUE m_v;
	BTN m_btn;
	RECT m_rcClient;

	eAlarmLimitState m_bAlarmLimitState;
	eStateOfAlarm m_eAlarm;
	eAlarmLimitState m_ePrevstate;
	
	DWORD m_dwPushDelta;
	
	WORD m_kUp;
	WORD m_kDown;
	WORD m_kSpace;

	int m_iLimitRange;
	int m_iPrevValue;

	bool m_bHasFocus;
	bool m_bDepressed;
	bool m_bLMouseButtonDown;
	bool m_bLButtonDown;
	bool m_bEndOfRange;

	bool m_bLowOff;
	bool m_bHighOff;


public:

	// Operations
public:
	virtual void Draw(int nState);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALimitBtn)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


	// Generated message map functions
protected:
	//{{AFX_MSG(CALimitBtn)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


