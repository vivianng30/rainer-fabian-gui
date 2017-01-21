#pragma once
#include "globDefs.h"
#include "MVModel.h"
#include "PushBtn.h"
#include "colour.h"
#include "BitmapSlider.h"
#include "SettingBtn.h"

//global font objects
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf8AcuNorm;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf13AcuBold;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf15AcuMed;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf19AcuMed;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf23AcuBold;
extern HFONT g_hf25AcuMed;
extern HFONT g_hf27AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf33AcuBold;
extern HFONT g_hf43AcuBold;
extern HFONT g_hf53AcuBold;
extern HFONT g_hf70Bold;

// CSubViewSettings

class CWndSubSettings : public CWnd
{
	DECLARE_DYNAMIC(CWndSubSettings)

public:
	CWndSubSettings(UINT viewFlag);
	virtual ~CWndSubSettings();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void Show(bool bShow);


protected:
	virtual void Initialize()=0;
	virtual void Draw()=0;

	void SetOneButtonDepressed(int btnID);
	void SetOneButtonFocused(int btnID);
	void SetNextButtonFocused();
	void SetPrevButtonFocused();
	eBtnState GetCurrentBtnState();
	eBtnState GetBtnState(int btnID);
	
	CMVModel *getModel();

protected:
	CMVModel* m_pModel;

	UINT m_viewFlag;
	CStringW m_szBack;

	// Attributes
	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	
	//CDialog*   m_pDlg;

	CBmp* m_pcRegionTopLeCorner;
	CBmp* m_pcRegionTopRiCorner;
	CBmp* m_pcRegionBotLeCorner;
	CBmp* m_pcRegionBotRiCorner;

	CBmp* m_pcFocused;
	CBmp* m_pcNotSel;
	CBmp* m_pcSelected;

	/*CBmp* m_pcBack_Up;
	CBmp* m_pcBack_Dw;
	CBmp* m_pcBack_Fc;*/

	CBmp* m_pcWarning;

	//CPushBtn* m_pcBack;


	CSettingBtn* m_pbtn1;
	CSettingBtn* m_pbtn2;
	CSettingBtn* m_pbtn3;
	CSettingBtn* m_pbtn4;
	CSettingBtn* m_pbtn5;
	CSettingBtn* m_pbtn6;
	CSettingBtn* m_pbtn7;
	CSettingBtn* m_pbtn8;
	//CSelectSetupBtn* m_pbtn9;

	CList<CSettingBtn* , CSettingBtn *> m_plBtn;
	//CList<CSelectSetupBtn* , CSelectSetupBtn *> m_plUsedBtn;

	DWORD m_dwLastSetupTimer;

	int m_iNumValues;
	int m_iCurBlockPos;	//last Pos of block in LanguageList
	int m_iCurPosInBlock;	//current pos inside of block == Id of button (1-7)

	CStringW m_szSetting;

	

protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	/*afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClicked1();
	afx_msg void OnBnClicked2();
	afx_msg void OnBnClicked3();
	afx_msg void OnBnClicked4();
	afx_msg void OnBnClicked5();
	afx_msg void OnBnClicked6();
	afx_msg void OnBnClicked7();*/
};

