/**********************************************************************************************//**
 * \file	WndService.h.
 *
 * Declares the window service class
 **************************************************************************************************/

#pragma once
#include "globDefs.h"
#include "MVModel.h"
#include "PushBtn.h"

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000


//global font objects
extern HFONT g_hf13AcuBold;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf15AcuMed;
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf9AcuBold;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf33AcuBold;
extern HFONT g_hf27AcuBold;
extern HFONT g_hf17AcuBold;


//extern HFONT g_hf11AcuBold;
//extern HFONT g_hf13AcuBold;
extern HFONT g_hf14AcuMed;
extern HFONT g_hf14AcuBold;

//extern CLangAdmin* g_pGlobalLanguageStrings;

// CWndService

class CWndService : public CWnd
{
	DECLARE_DYNAMIC(CWndService)

public:
	CWndService();
	virtual ~CWndService();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual void Show(bool bShow);

	virtual void Init();

	//void BnClickedBack();

protected:
	virtual void Draw();

	CMVModel *getModel();

protected:
	CMVModel* m_pModel;

	// Attributes
	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	CPushBtn* m_pcMenuBack;

	CBmp* m_pcNumeric_Up;
	CBmp* m_pcNumeric_Dw;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
};


