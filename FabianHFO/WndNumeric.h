#pragma once

#include "Bmp.h"
//#include "Btn.h"
#include "KbrdBtn.h"
#include "globdefs.h"
#include "colour.h"
#include "MVModel.h"

//global font objects
extern HFONT g_hf3AcuNorm;
extern HFONT g_hf4AcuNorm;
extern HFONT g_hf5AcuNorm;
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf8AcuNorm;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf9AcuBold;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf13AcuBold;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf15AcuMed;
extern HFONT g_hf11AcuBoldNum;
//extern HFONT g_hf24Medium;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf19AcuMed;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf23AcuBold;
extern HFONT g_hf25AcuMed;
extern HFONT g_hf27AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf33AcuBold;
extern HFONT g_hf33AcuBoldNum;
//extern HFONT g_hf42Bold;
extern HFONT g_hf43AcuBold;
extern HFONT g_hf53AcuBold;
extern HFONT g_hf70Bold;
extern HFONT g_hf70BoldNum;
extern HFONT g_hf34BoldNum;

typedef struct tagWNDNUM_LBL
{
	//CStringW		szHeader;
	bool		bVal1;
	CStringW		szName1;
	CStringW		szNameNote1;
	CStringW		szUnit1;
	bool		bVal2;
	CStringW		szName2;
	CStringW		szNameNote2;
	CStringW		szUnit2;
	bool		bVal3;
	CStringW		szName3;
	CStringW		szNameNote3;
	CStringW		szUnit3;
	bool		bVal4;
	CStringW		szName4;
	CStringW		szNameNote4;
	CStringW		szUnit4;
	bool		bVal5;
	CStringW		szName5;
	CStringW		szNameNote5;
	CStringW		szUnit5;
	bool		bVal6;
	CStringW		szName6;
	CStringW		szNameNote6;
	CStringW		szUnit6;
	bool		bVal7;
	CStringW		szName7;
	CStringW		szNameNote7;
	CStringW		szUnit7;
	bool		bVal8;
	CStringW		szName8;
	CStringW		szNameNote8;
	CStringW		szUnit8;
	bool		bVal9;
	CStringW		szName9;
	CStringW		szNameNote9;
	CStringW		szUnit9;
	/*bool		bVal10;
	CStringW		szName10;
	CStringW		szNameNote10;
	CStringW		szUnit10;*/
}WNDNUM_LABELS;

typedef struct tagWNDNUM_ALARMLIM
{
	int		iUpperLimit1;
	eAlarmLimitState	enumUpperLimit1;
	int		iLowerLimit1;
	eAlarmLimitState	enumLowerLimit1;
	int		iUpperLimit2;
	eAlarmLimitState	enumUpperLimit2;
	int		iLowerLimit2;
	eAlarmLimitState	enumLowerLimit2;
	int		iUpperLimit3;
	eAlarmLimitState	enumUpperLimit3;
	int		iLowerLimit3;
	eAlarmLimitState	enumLowerLimit3;
	int		iUpperLimit4;
	eAlarmLimitState	enumUpperLimit4;
	int		iLowerLimit4;
	eAlarmLimitState	enumLowerLimit4;
	int		iUpperLimit5;
	eAlarmLimitState	enumUpperLimit5;
	int		iLowerLimit5;
	eAlarmLimitState	enumLowerLimit5;
	int		iUpperLimit6;
	eAlarmLimitState	enumUpperLimit6;
	int		iLowerLimit6;
	eAlarmLimitState	enumLowerLimit6;
	int		iUpperLimit7;
	eAlarmLimitState	enumUpperLimit7;
	int		iLowerLimit7;
	eAlarmLimitState	enumLowerLimit7;
	int		iUpperLimit8;
	eAlarmLimitState	enumUpperLimit8;
	int		iLowerLimit8;
	eAlarmLimitState	enumLowerLimit8;
	int		iUpperLimit9;
	eAlarmLimitState	enumUpperLimit9;
	int		iLowerLimit9;
	eAlarmLimitState	enumLowerLimit9;
	/*int		iUpperLimit10;
	eAlarmLimitState	enumUpperLimit10;
	int		iLowerLimit10;
	eAlarmLimitState	enumLowerLimit10;*/
}WNDNUM_ALARMLIMITS;

class CWndNumeric : public CWnd
{
	DECLARE_DYNAMIC(CWndNumeric)

public:
	CWndNumeric();
	virtual ~CWndNumeric();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//void SetLables(WNDNUM_LABELS* pwnl);
	//void SetValues(WNDNUM_VALUES* pwnv, bool bRedraw=false);
	//void refreshMeasurements();
	void refreshLimits();
	void refreshPmax();
	//void SetPmax(int iPmax, bool bRedraw=false);
	//void SetLimits(WNDNUM_ALARMLIMITS* pwna, bool bRedraw=false);
	//void GetLimits(WNDNUM_ALARMLIMITS* pwna);
	//void GetValues(WNDNUM_VALUES* pwnv);
	virtual void SetGraphCursor(bool bDraw, bool bMoveXscale, bool bMoveYscale);
	virtual bool DrawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)=0;
	
	//bool isGraphCursor();

	void Show(bool bShow);
	//int double2int(double d);

	//void SetTimerRunning(bool state);
	//void SetFlowData(int valueInsp, int valueExsp);

	CMVModel *getModel();
protected:
	bool isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay=0);//used for (dwLastTickCount+DELAY<getTickCount64())

protected:
	// Attributes

	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;

	HDC		m_hdcStatic;
	HBITMAP m_hbmpStatic;


	CBmp* m_pcNumPara1er;
	CBmp* m_pcNumPara2er;
	CBmp* m_pcNumPara3er;
	//CBmp* m_pcNumHeader;

	CBmp* m_pcNumInfoTop;
	CBmp* m_pcNumInfoBot;

	CBmp* m_pcNumericTop;
	CBmp* m_pcNumericBot;

	CBmp* m_pcFlowmeterTherapy;
	CBmp* m_pcFlowmeterLeft;
	CBmp* m_pcFlowmeterRight;


	CBmp* m_pcLimitHigh_Up;
	CBmp* m_pcLimitLow_Up;

	CBmp* m_pcGraphCursor_left_up;
	CBmp* m_pcGraphCursor_left_dw;
	CBmp* m_pcGraphCursor_right_up;
	CBmp* m_pcGraphCursor_right_dw;
	CBmp* m_pcGraphCursor_up_up;
	CBmp* m_pcGraphCursor_up_dw;
	CBmp* m_pcGraphCursor_down_up;
	CBmp* m_pcGraphCursor_down_dw;
	CBmp* m_pcGraphCursor_pic;

	CBmp* m_pcXmoveDown_up;
	CBmp* m_pcXmoveDown_dw;
	CBmp* m_pcXmoveUp_up;
	CBmp* m_pcXmoveUp_dw;
	CBmp* m_pcYmoveLeft_up;
	CBmp* m_pcYmoveLeft_dw;
	CBmp* m_pcYmoveRight_up;
	CBmp* m_pcYmoveRight_dw;
	CBmp* m_pcMoveXscale_pic;
	CBmp* m_pcMoveYscale_pic;

	CBmp* m_pcWarningYellow;

	/*CBmp* m_pcMediJet;
	CBmp* m_pcInfantFlow;
	CBmp* m_pcInfantFlowLP;*/

	CKbrdBtn* m_pbtnCursorUp;
	CKbrdBtn* m_pbtnCursorDown;
	CKbrdBtn* m_pbtnCursorLeft;
	CKbrdBtn* m_pbtnCursorRight;

	CKbrdBtn* m_pbtnXmoveUp;
	CKbrdBtn* m_pbtnXmoveDown;
	CKbrdBtn* m_pbtnYmoveLeft;
	CKbrdBtn* m_pbtnYmoveRight;

	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	bool m_bGraphCursor;
	bool m_bMoveXscale;
	bool m_bMoveYscale;

	virtual bool DrawFrames(CDC* pDC)=0;
	virtual bool DrawStaticText(CDC* pDC)=0;
	virtual bool DrawLimits(CDC* pDC)=0;
	
	/*CBmp* m_pcWait;
	CBmp* m_pcWait1;
	CBmp* m_pcWait2;
	CBmp* m_pcWait3;
	CBmp* m_pcWait4;
	CBmp* m_pcWait5;
	CBmp* m_pcWait6;
	CBmp* m_pcWait7;
	CBmp* m_pcWait8;*/

	CBmp* m_pcFlowKegelInsp;
	CBmp* m_pcFlowKegelExpp;
	CBmp* m_pcFlowKegel_0;
	CBmp* m_pcFlowKegel_1;
	CBmp* m_pcFlowKegel_2;
	CBmp* m_pcFlowKegel_3;
	CBmp* m_pcFlowKegel_4;
	CBmp* m_pcFlowKegel_5;
	CBmp* m_pcFlowKegel_6;
	CBmp* m_pcFlowKegel_7;
	CBmp* m_pcFlowKegel_8;
	CBmp* m_pcFlowKegel_9;

	int m_iFlowKegelInsp;
	int m_iFlowKegelExsp;
	int m_valueFlowInsp;
	int m_valueFlowExsp;

	//int m_iWaitCount;
	//bool m_bCalRunning;

	//CBmp* m_pcOxyWarning;


	CMVModel* m_pModel;

	CRITICAL_SECTION csDraw;

	//{{AFX_MSG(CWndNumeric)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedCursorUp();
	afx_msg void OnBnClickedCursorDwn();
	afx_msg void OnBnClickedCursorLeft();
	afx_msg void OnBnClickedCursorRight();
	afx_msg void OnBnClickedMoveXUp();
	afx_msg void OnBnClickedMoveXDown();
	afx_msg void OnBnClickedMoveYLeft();
	afx_msg void OnBnClickedMoveYRight();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


