/**********************************************************************************************//**
 * \file	WndServiceInterfaces.h.
 *
 * Declares the window service interfaces class
 **************************************************************************************************/

#pragma once
#include "WndService.h"
#include "KbrdBtn.h"


// CWndServiceInterfaces

class CWndServiceInterfaces : public CWndService
{
	DECLARE_DYNAMIC(CWndServiceInterfaces)

public:
	CWndServiceInterfaces();
	virtual ~CWndServiceInterfaces();

	//virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	////virtual BOOL PreTranslateMessage(MSG* pMsg);

	//void Show(bool bShow);

	void Init();

protected:
	void Draw();

private:
	CBmp* m_pcUp;
	CBmp* m_pcDw;
	CBmp* m_pcDis;

	CDialog*   m_pDlg;

	CStringW m_szNSPIinfo;
	CStringW m_szI2Cinfo;
	CStringW m_szPIFinfo;

	/*CKbrdBtn* m_pcEraseFRAM;*/
	CKbrdBtn* m_pcLogfile;
	CKbrdBtn* m_pcCloseApp;

	MAINBOARD_DATA m_dataMainboard;
	int m_iSERST0_Data;
	BYTE m_byI2CWatchdogState;

	bool m_bUSBavailable;
	bool m_bLOGavailable;
	bool m_bCopyLOGrunning;
	bool m_bCopyLOGfinnished;
	bool m_bCopyLOGsuccess;

	//CKbrdBtn* m_pcInstallUpdate;
	//CKbrdBtn* m_pcEnableHFO;

	//bool m_bHFOenabled;

	//BYTE m_bChargestate;
	//BYTE m_bRemainingAccu;
	//SHORT m_sUBATT_Data;
	//int m_iPARST0_Data;
	//int m_iOpMinBatt;

protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnBnClickedEraseFRAM();
	afx_msg void OnBnClickedExitApp();
	afx_msg void OnBnClickedLogfiles();
	//afx_msg void OnBnClickedEnableHFO();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


