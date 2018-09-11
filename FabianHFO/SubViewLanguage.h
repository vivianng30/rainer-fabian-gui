/**********************************************************************************************//**
 * \file	SubViewLanguage.h.
 *
 * Declares the sub view language class
 **************************************************************************************************/

#pragma once
#include "SelectSetupBtn.h"
#include "globDefs.h"
#include "MVModel.h"
#include "colour.h"
#include "WndWaitHourglass.h"

// CSubViewLanguage

class CSubViewLanguage : public CWnd
{
	DECLARE_DYNAMIC(CSubViewLanguage)

public:
	CSubViewLanguage();
	virtual ~CSubViewLanguage();

	bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void NotifyLanguageChanged();

	void Show(bool bShow);

protected:
	void Draw();

	void SetSelectedPos(CStringW csLang);
	void SetFocusedPos(int btnID);
	void SetOneButtonDepressed(int btnID);
	void SetOneButtonFocused(int btnID);
	void SetNextButtonFocused();
	void SetPrevButtonFocused();
	eBtnState GetCurrentBtnState();
	eBtnState GetBtnState(int btnID);

	void SetLanguage(int btnID);

	void ReorganizeBtns(bool forward);

	CMVModel *getModel();

	bool CreateWndHourglass();
	void ShowWndHourglass(bool bShow);
	bool DestroyWndHourglass();

	void StartLoadLanguageThread(void);
	void StopLoadLanguageThread(void);

	static friend UINT CLoadLanguageThread(LPVOID pc);
	CWinThread*	m_pcwtLoadLanguageThread;
	DWORD LoadLanguage(void);
	bool m_bDoLoadLanguageThread;
	CEvent eventLoadLanguage;
	HANDLE m_hThreadLoadLanguage;

private:
	CMVModel* m_pModel;

	CWndWaitHourglass *m_pWndHourglass;

	DWORD m_dwLastSetupTimer;

	bool m_bExit;
	bool m_bInitialized;
	CStringW m_szLangToLoad;

	// Attributes
	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	CBmp* m_pcUp_Up;
	CBmp* m_pcUp_Dw;
	CBmp* m_pcDw_Up;
	CBmp* m_pcDw_Dw;

	CBtn* m_pcNextUp;
	CBtn* m_pcNextDwn;

	CBmp* m_pcFocused;
	CBmp* m_pcNotSel;
	CBmp* m_pcSelected;


	CSelectSetupBtn* m_pbtnLanguage1;
	CSelectSetupBtn* m_pbtnLanguage2;
	CSelectSetupBtn* m_pbtnLanguage3;
	CSelectSetupBtn* m_pbtnLanguage4;
	CSelectSetupBtn* m_pbtnLanguage5;
	CSelectSetupBtn* m_pbtnLanguage6;
	CSelectSetupBtn* m_pbtnLanguage7;


	CStringList* m_pcsLangNames;
	int m_iNumLanguages;

	bool m_bSDCARDfont;

	CList<CSelectSetupBtn* , CSelectSetupBtn *> m_plLangBtn;
	CList<CSelectSetupBtn* , CSelectSetupBtn *> m_plUsedLangBtn;

	//int m_iCurPara;
	int m_iCurBlockPos;	//last Pos of block in LanguageList
	int m_iCurPosInBlock;	//current pos inside of block == Id of button (1-7)

	int m_iCurFocusedPos;
	int m_iCurSelectedPos;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedLang1();
	afx_msg void OnBnClickedLang2();
	afx_msg void OnBnClickedLang3();
	afx_msg void OnBnClickedLang4();
	afx_msg void OnBnClickedLang5();
	afx_msg void OnBnClickedLang6();
	afx_msg void OnBnClickedLang7();
	afx_msg void OnBnClickedNextUp();
	afx_msg void OnBnClickedNextDw();
};