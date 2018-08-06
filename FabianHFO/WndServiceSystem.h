/**********************************************************************************************//**
 * \file	WndServiceSystem.h.
 *
 * Declares the window service system class
 **************************************************************************************************/

#pragma once
#include "WndService.h"
//#include "KbrdBtn.h"
#include "PushBtn.h"
//#include "BitmapSlider.h"

// CWndServiceSystem

class CWndServiceSystem : public CWndService
{
	DECLARE_DYNAMIC(CWndServiceSystem)

public:
	CWndServiceSystem();
	virtual ~CWndServiceSystem();

	//virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	////virtual BOOL PreTranslateMessage(MSG* pMsg);

	//void Show(bool bShow);

	void Init();

protected:
	void Draw();

private:
	//CMVModel* getModel();

	//// Attributes
	//HDC			m_hDC;
	//HBITMAP		m_hBmp;
	//HBITMAP		m_hBmpPrev;
	//LONG		m_lX;
	//LONG		m_lY;
	//LONG		m_lXo;
	//LONG		m_lYo;

	CDialog*   m_pDlg;


	//CBmp* m_pcAlarm_Up;
	//CBmp* m_pcAlarm_Dw;
	//CKbrdBtn* m_pcAlarm;
	//CKbrdBtn* m_pcResetOpTime;
	//CKbrdBtn* m_pcResetAccuOpTime;
	//CKbrdBtn* m_pcResetHFOOpTime;

	//CKbrdBtn* m_pcInstaller;

	CStringW m_szKernel;
	//int m_iSRev;
	int m_iSERST0_Data;
	MAINBOARD_DATA m_dataMainboard;
	CStringW m_szUniqueID;
	int m_iMBDST0_Data;

	//BYTE m_iHFOSpringType;
	BYTE m_iMainVersion;
	BYTE m_iSystemHWrevision;
	eDevType m_eDevType;
	eFlowSensorType m_eFlowSensorType;
	eAVRevision m_eAVRevision;
	eHFModuleRevision m_eHFRevision;

	//bool m_bAlarmOn;

	//CBitmapSlider*	m_sliderPassword;
	//bool m_bServicePswrdDisabled;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	/*afx_msg void OnBnClickedAlarm();
	afx_msg void OnBnClickedInstaller();
	afx_msg void OnBnClickedResetOpTime();
	afx_msg void OnBnClickedResetOpTimeBattery();
	afx_msg void OnBnClickedResetOpTimeHFO();
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);*/
};


