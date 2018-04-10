/**********************************************************************************************//**
 * \file	WndDataFOT.h.
 *
 * Declares the window data fot class
 **************************************************************************************************/

#pragma once

#include "MVModel.h"
#include "Bmp.h"
#include "colour.h"
#include "PushBtn.h"
#include "UpDwnBtn.h"

#include "ParaBtn_FOTSTEPS.h"
#include "ParaBtn_FOTPMEANSTART.h"
#include "ParaBtn_FOTAMPLITUDE.h"
#include "ParaBtn_FOTFREQ.h"
#include "ParaBtn_FOTPMEANEND.h"
#include "ParaBtn_FOTPEEPEND.h"
#include "ParaBtn_FOTPEEPSTART.h"
#include "MVView.h"

//global font objects
extern HFONT g_hf14AcuMed;
extern HFONT g_hf43AcuBold;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf8AcuBold;

// CWndDataFOT

class CWndDataFOT : public CWnd
{
	DECLARE_DYNAMIC(CWndDataFOT)

public:
	CWndDataFOT(CMVView *parentView);
	virtual ~CWndDataFOT();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void Init();
	void Show(bool bShow);

	void SetViewFocus(int iBtn);
	bool SetNextFocus();
	bool SetPrevFocus();

	//void setCollectdataFOTtime(BYTE iCountFOTimer);
	void setFOTtime(BYTE iCountFOTimer);
	void setFOTrunning(bool state);
	void updateFOTPmeanPara();
	void updateFOTPEEPPara();

protected:
	CMVModel *getModel();

	void Draw(bool bStatic);
	void DrawStatic();
	void drawHourglass();
	void drawMbarStep();
	void drawPIPmax();
	void drawFOTmenubar();

	void SetSTARTSTOPUnpressed();
	void SetSTARTSTOPDepressed();
	//void SetSTARTSTOPPreset();
	void showFOTMessage();

	void startHourglass();
	void stopHourglass();
	
	BYTE getCurrentFOT_STEPSPara();

private:
	CMVView *m_parentView;
	CMVModel* m_pModel;
	CDialog*   m_pDlg;

	HDC		m_hdcStatic;
	HBITMAP m_hbmpStatic;

	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	CParaBtn_FOTSTEPS* m_pcPara_FOT_Steps;
	CParaBtn_FOTPMEANSTART* m_pcPara_FOT_PmeanLow;
	CParaBtn_FOTPMEANEND* m_pcPara_FOT_PmeanHigh;
	CParaBtn_FOTPEEPSTART* m_pcPara_FOT_PEEPLow;
	CParaBtn_FOTPEEPEND* m_pcPara_FOT_PEEPHigh;

	//CParaBtn_FOTAMPLITUDE* m_pcPara_FOTamplitude;
	//CParaBtn_FOTFREQ* m_pcPara_FOTfreq;
	
	CUpDwnBtn* m_pcBtnStartStopSeq;
	CBmp* m_pcStartStopSeq_Dw;
	CBmp* m_pcStartStopSeq_Up;
	CBmp* m_pcStartStopSeq_Dis;

	CPushBtn* m_pcBtnRunSeq;
	CBmp* m_pcRunSeq_Dw;
	CBmp* m_pcRunSeq_Up;
	CBmp* m_pcRunSeq_Dis;

	CPushBtn* m_pcBtnRepeatSeq;
	CBmp* m_pcRepeatSeq_Dw;
	CBmp* m_pcRepeatSeq_Up;
	CBmp* m_pcRepeatSeq_Dis;

	CPushBtn* m_pcBtnDecreaseSeq;
	CBmp* m_pcDecreaseSeq_Dw;
	CBmp* m_pcDecreaseSeq_Up;
	CBmp* m_pcDecreaseSeqx_Dis;

	/*CUpDwnBtn* m_pcMenuStartStop;
	CBmp* m_pcMenuStartStop_Dw;
	CBmp* m_pcMenuStartStop_Up;
	CBmp* m_pcMenuStartStop_Dis;

	CPushBtn* m_pcContinueFOTseq;
	CBmp* m_pcContinueFOTseq_Dw;
	CBmp* m_pcContinueFOTseq_Up;*/

	CBmp* m_pcPara_UpT;
	CBmp* m_pcPara_FcT;
	CBmp* m_pcPara_DwT;
	CBmp* m_pcPara_UpB;
	CBmp* m_pcPara_FcB;
	CBmp* m_pcPara_DwB;

	CBmp* m_pcWait;
	CBmp* m_pcWait1;
	CBmp* m_pcWait2;
	CBmp* m_pcWait3;
	CBmp* m_pcWait4;
	CBmp* m_pcWait5;
	CBmp* m_pcWait6;
	CBmp* m_pcWait7;
	CBmp* m_pcWait8;

	int m_iWaitCount;
	bool m_bHourglassRun;

	SequenceStatesFOT feFOTstate;

	BYTE m_iCountFOTimer;

	//bool m_bPMEANEND_SetKey;
	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStartStop();
	afx_msg void OnBnClickedDecrease();
	afx_msg void OnBnClickedRepeat();
	afx_msg void OnBnClickedContinue();
};


