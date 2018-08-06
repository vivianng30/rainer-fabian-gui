/**********************************************************************************************//**
 * \file	SubViewDisplay.h.
 *
 * Declares the sub view display class
 **************************************************************************************************/

#pragma once
#include "globDefs.h"
#include "MVModel.h"
#include "PushBtn.h"
#include "KbrdBtn.h"
#include "colour.h"
#include "BitmapSlider.h"
#include "UpDwnBtn.h"

// CSubViewDisplay

class CSubViewDisplay : public CWnd
{
	DECLARE_DYNAMIC(CSubViewDisplay)

public:
	CSubViewDisplay();
	virtual ~CSubViewDisplay();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void Show(bool bShow);


protected:
	void Initialize();
	void Draw();

	void Increase();
	void Decrease();
	void IncreaseMax();
	void DecreaseMax();
	//void Calibrate();
	void Screenlock();

	void OpenSettingsView();

	

	CMVModel *getModel();

private:
	CMVModel* m_pModel;

	// Attributes
	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;


	//CBmp* m_pcAutoscale;
	CBmp* m_pcNoautoscale;

	CBmp* m_pcScreenlocktime_Up;
	CBmp* m_pcScreenlocktime_Dw;
	CBmp* m_pcScreenlocktime_Fc;
	CBmp* m_pcScreenlocktime_Dis;

	CBmp* m_pcNight;
	CBmp* m_pcBrightness1;
	CBmp* m_pcBrightness2;
	CBmp* m_pcBrightness3;
	CBmp* m_pcBrightness4;
	CBmp* m_pcBrightness5;

	CBmp* m_pcNotFilledGraph;
	CBmp* m_pcFilledGraph;

	CBmp* m_pcIncrContrast_Up;
	CBmp* m_pcIncrContrast_Dw;
	CBmp* m_pcDecrContrast_Up;
	CBmp* m_pcDecrContrast_Dw;

	CBmp* m_pcIncrMaxContrast_Up;
	CBmp* m_pcIncrMaxContrast_Dw;
	CBmp* m_pcDecrMaxContrast_Up;
	CBmp* m_pcDecrMaxContrast_Dw;

	CBmp* m_pcRegionTopLeCorner;
	CBmp* m_pcRegionTopRiCorner;
	CBmp* m_pcRegionBotLeCorner;
	CBmp* m_pcRegionBotRiCorner;

	CBmp* m_pcScreenlock_Up;
	CBmp* m_pcScreenlock_Dw;
	//CBmp* m_pcScreenlock_Fc;
	/*CBmp* m_pcCal_Up;
	CBmp* m_pcCal_Dw;*/

	CPushBtn* m_pcScreenlocktime;

	CPushBtn* m_pcScreenlock;
	//CKbrdBtn* m_pcCal;

	CPushBtn* m_pcIncrease;
	CPushBtn* m_pcIncreaseMax;
	CPushBtn* m_pcDecrease;
	CPushBtn* m_pcDecreaseMax;

	//CDialog*   m_pDlg;

	CBitmapSlider*	m_sliderAutolock;
	CBitmapSlider*	m_sliderFilledGraph;
	CBitmapSlider*	m_sliderAutoscaleGraph;
	CBitmapSlider*	m_sliderFilledTrend;
	CBitmapSlider*	m_sliderAutoscaleTrend;

	DWORD m_iContrastParm;

	eAutoScreenlock m_eAutoSreenlock;
	bool m_bAutoScreenLock;

	bool m_bGraphFilled;
	bool m_bGraphAutoscale;

	bool m_bTrendFilled;
	bool m_bTrendAutoscale;


protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	//afx_msg void OnBnClickedCalibrate();
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
};


