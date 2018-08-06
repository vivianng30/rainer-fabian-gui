/**********************************************************************************************//**
 * \file	WndSetupPatient.h.
 *
 * Declares the window setup patient class
 **************************************************************************************************/

#pragma once
#include "Btn.h"
#include "PushBtn.h"
#include "KbrdBtn.h"
#include "UpDwnBtn.h"
#include "globDefs.h"
#include "MVModel.h"
#include "WndSetupKeyboard.h"
#include "colour.h"
//#include "WndWaitHourglass.h"

class CWndSetupPatient : public CWnd
{
	DECLARE_DYNAMIC(CWndSetupPatient)

public:
	CWndSetupPatient();
	virtual ~CWndSetupPatient();

	bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void Show(bool bShow);

protected:
	void Initialize();

	void Draw();

	bool CreateWndKeyboard();
	bool DestroyWndKeyboard();
	void ShowWndKeyboard(bool bShow);

	//bool CreateWndHourglass();
	//void ShowWndHourglass(bool bShow);
	//bool DestroyWndHourglass();


	void PatientDataChanged();

	void BnClickedReset();
	void BnClickedChange();
	void BnClickedApply();

	//void StartDeleteThread(void);
	//void StopDeleteThread(void);

	


protected:
	CWndSetupKeyboard *m_pcWndKeyboard;
	CMVModel *getModel();

private:
	CMVModel* m_pModel;

	//CWnd* m_pParentWnd;

	//CWnd* m_pEdit;

	//CWndWaitHourglass *m_pWndHourglass;

	// Attributes
	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	//bool m_bExit;

	CBmp* m_pc_Up;
	CBmp* m_pc_Dw;
	CBmp* m_pc_Red;
	CBmp* m_pc_Dis;

	CBmp* m_pcToday_Up;
	CBmp* m_pcToday_Dw;
	CKbrdBtn* m_pcToday;

	CBmp* m_pcPatientData;

	

	CEdit* m_pEditName;
	CEdit* m_pEditFirstname;
	CEdit* m_pEditBirthDay;
	CEdit* m_pEditBirthMonth;
	CEdit* m_pEditBirthYear;
	CEdit* m_pEditPersID;
	CEdit* m_pEditWeight;
	CEdit* m_pEditRemark;

	//CDateTimeCtrl* m_pCtrl;


	CPushBtn* m_pcReset;
	CPushBtn* m_pcChange;
	CPushBtn* m_pcAply;

	CDialog*   m_pDlg;

	

	bool m_bPatientDataChanged;

	bool m_bChangeMode;


protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSetFocusEditName();
	afx_msg void OnSetFocusEditFirtsName();
	afx_msg void OnSetFocusEditPersID();
	afx_msg void OnSetFocusEditBirthDay();
	afx_msg void OnSetFocusEditBirthMonth();
	afx_msg void OnSetFocusEditBirthYear();
	afx_msg void OnSetFocusEditWeight();
	afx_msg void OnSetFocusEditRemark();
	afx_msg void OnBnClickedToday();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnBnClickedSetDateTime();
};



