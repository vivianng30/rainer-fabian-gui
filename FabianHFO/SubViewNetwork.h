/**********************************************************************************************//**
 * \file	SubViewNetwork.h.
 *
 * Declares the sub view network class
 **************************************************************************************************/

#pragma once
#include "globDefs.h"
#include "MVModel.h"
#include "DTUpDwnBtn.h"
#include "SelectSetupBtn.h"
#include "colour.h"

// CSubViewNetwork

class CSubViewNetwork : public CWnd
{
	DECLARE_DYNAMIC(CSubViewNetwork)

public:
	CSubViewNetwork();
	virtual ~CSubViewNetwork();
	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	/*void SetOneButtonDepressed(int btnID);
	void SetAllButtonUnpressed();
	void SetOneButtonFocused(int btnID);
	void SetNextButtonFocused();
	void SetPrevButtonFocused();
	eBtnState GetCurrentBtnState();
	eBtnState GetBtnState(int btnID);*/

	void Show(bool bShow);

protected:
	void Initialize();
	void Draw();

	CMVModel *getModel();

	void WaveChanged();
	CString GetWaveString(SHORT iWave);

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


	CBmp* m_pcRegionTopLeCorner;
	CBmp* m_pcRegionTopRiCorner;
	CBmp* m_pcRegionBotLeCorner;
	CBmp* m_pcRegionBotRiCorner;

	CBmp* m_pcSmallUp_Up;
	CBmp* m_pcSmallUp_Dw;
	CBmp* m_pcSmallDw_Up;
	CBmp* m_pcSmallDw_Dw;

	CBmp* m_pcOkGreen_Up;
	CBmp* m_pcOkGreen_Dw;
	CBmp* m_pcOkRed_Up;
	CBmp* m_pcOkRed_Dw;

	/*CBmp* m_pcWave_Up;
	CBmp* m_pcWave_Dw;
	CBmp* m_pcWave_Fc;*/

	/*CList<CSelectSetupBtn* , CSelectSetupBtn *> m_plStaticBtn;

	CDTUpDwnBtn* m_pcWave1NextUp;
	CDTUpDwnBtn* m_pcWave1NextDwn;

	CDTUpDwnBtn* m_pcWave2NextUp;
	CDTUpDwnBtn* m_pcWave2NextDwn;

	CSelectSetupBtn* m_pbtnWave1;
	CSelectSetupBtn* m_pbtnWave2;*/

	/*CBtn* m_pbtnOk;*/

	//CDialog*   m_pDlg;

	/*DWORD m_dwLastSetupTimer;
	bool m_bWavesChanged;
	SHORT m_iCurrentField;*/

	UINT m_uiPort;
	BYTE m_iPDMSProtocol;

	/*BYTE m_iWave1;
	BYTE m_iWave2;*/

	//bool m_bETCO2;

	//bool m_bRS232;
	CString m_szIP;
	CString m_szGateway;
	CString m_szSubnet;
	bool m_bDHCP;

	bool m_bExit;

	/*int m_iCounter;
	bool m_bBigSteps;
	eTimeChanger m_eTimeChanger;*/

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	/*afx_msg void OnBnClickedWave1();
	afx_msg void OnBnClickedWave2();
	afx_msg void OnBnClickedSetWave();*/
	
};

