#include "Bmp.h"
#pragma once
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

// CNumericField

class CNumericField : public CWnd
{
	DECLARE_DYNAMIC(CNumericField)

public:
	CNumericField(eNumericSize size);
	virtual ~CNumericField();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//virtual void deinit();

	//void setFieldData(LPNUMERICBLOCK pnbData);
	//void setFieldType(eNumericType eType, eNumericSize eSize);
	//void setLimits(bool	bShowHiAlarmLimit, eAlarmLimitState	enumStateHiLimit,int iHiLimitValue,bool	bShowLoAlarmLimit,eAlarmLimitState enumStateLoLimit,int iLoLimitValue, bool bRedraw=false);
	//void setLimitsValue(eAlarmLimitState enumStateHiLimit,int iHiLimitValue,eAlarmLimitState enumStateLoLimit,int iLoLimitValue, bool bRedraw=false);

	virtual bool drawData(bool bData, bool bFrames, bool bStatic, bool bLimits, bool bFlowmeter)=0;
	//void setNumericValue(INT iVal, bool bRedraw);

	void setCursorField(bool state);
	bool isCursorField();
protected:
	CMVModel *getModel();

	virtual void drawLowLimit(CDC* pDC);
	virtual void drawHighLimit(CDC* pDC);
	virtual bool drawFrames(CDC* pDC);
	virtual bool drawStaticText(CDC* pDC)=0;
	virtual bool drawLimits(CDC* pDC)=0;

protected:
	
	//LPNUMERICBLOCK m_pnbFieldData;

	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;

	HDC		m_hdcStatic;
	HBITMAP m_hbmpStatic;

	CMVModel* m_pModel;

	CRITICAL_SECTION csDraw;

	CBmp* m_pcLimitHigh_Up;
	CBmp* m_pcLimitLow_Up;

	CBmp* m_pcTauSize1;
	//CBmp* m_pcTauSize2;

	CBmp* m_pcNumPara1er;
	CBmp* m_pcNumPara2er;
	CBmp* m_pcNumPara3er;

	CBmp* m_pcWarningYellow;

	bool m_bCursorField;
	eNumericSize m_eSize;
	//eNumericType eType;
	//eNumericSize eSize;
	//TCHAR	numName[15];
	//TCHAR	numNameNote[10];
	//TCHAR	numUnit[15];//
	//INT		iNumValue;//
	//bool	bHiAlarmLimit;
	//eAlarmLimitState enumStateHiAlarmLimit;
	//INT		iHiAlarmLimit;
	//bool	bLoAlarmLimit;
	//eAlarmLimitState enumStateLoAlarmLimit;
	//INT		iLoAlarmLimit;

	CString m_szName;
	CString m_szNameNote;
	CString m_szUnit;

	//{{AFX_MSG(CWndNumeric)
	afx_msg void OnDestroy();
	/*afx_msg void OnPaint();*/
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


