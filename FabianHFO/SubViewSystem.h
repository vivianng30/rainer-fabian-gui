#pragma once
#include "globDefs.h"
#include "MVModel.h"
//#include "PushBtn.h"
//#include "KbrdBtn.h"
#include "colour.h"

// CSubViewSystem

class CSubViewSystem : public CWnd
{
	DECLARE_DYNAMIC(CSubViewSystem)

public:
	CSubViewSystem();
	virtual ~CSubViewSystem();
	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	void Show(bool bShow);


protected:
	void Initialize();
	void Draw();

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

	MAINBOARD_DATA m_dataMainboard;
	CStringW m_szVersion;
	CStringW m_szUniqueID;
	CStringW m_szLangVersion;
	DWORD m_iOpMinDev;
	DWORD m_iOpMinHFO;
	DWORD m_iOpMinBatt;
	BYTE m_iMainVersion;
	//WORD m_iSystemErrors;

	//CDialog*   m_pDlg;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};


