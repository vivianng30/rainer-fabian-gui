/**********************************************************************************************//**
 * \file	ViewStartup.h.
 *
 * Declares the view startup class
 **************************************************************************************************/

#pragma once
#include "MVView.h"
#include "Bmp.h"
//#include "CalBtn.h"
//#include "WndAlarmLog.h"
//#include "NumBtn.h"


class CViewStartup :public CMVView
{
public:
	CViewStartup(int ViewID = 0);
	virtual ~CViewStartup(void);

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual bool CreateView();
	virtual bool Initialize();
	virtual void Open();
	virtual void Close();

	virtual void Show();
	virtual void Hide();

	/*virtual void SetViewFocus(){SetFocus();}
	virtual bool SetNextFocus(){return false;}
	virtual bool SetPrevFocus(){return false;}*/

	//Observer
	//virtual void NotifyEvent(CMVEvent* pEvent);


protected:

	void Draw();
	
protected:

	CBmp* m_pcLogo;
	//CBmp* m_pcBaby;
	//CBmp* m_pcWarning;

	bool m_bWatchdog;

	CStringW m_szSWVers;

	bool m_bStartupCheck;
	int m_iCountStartup;

	bool m_bERROR;
	bool m_bSerialChecksumError;
	bool m_bCom_ERROR;
	int m_iChecksum;
	SHORT m_byAlarmState;
	bool m_bSPI_ERROR;
	bool m_bSPIChecksumError;
	bool m_bRelais_ERROR;
	bool m_bVoltage_ERROR;
	bool m_bI2C_ERROR;
	bool m_bPIF_ERROR;
	bool m_bDIO_ERROR;

	DWORD m_dwFactorydefaults;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};





