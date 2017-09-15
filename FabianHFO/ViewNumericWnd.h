#pragma once
#include "MVView.h"
//#include "WndNumAv.h"


#include "WndNumConfig.h"

#include "PushBtn.h"

/////////////////////////////////////////////////////////////////////////////
typedef struct tagNUMWNDATTRIB
{
	int			nX;
	int			nY;
	int			nCx;
	int			nCy;
}NUMWNDATTRIB;


class CViewNumericWnd :public CMVView
{
	
public:
	CViewNumericWnd(int ViewID = 0);
	virtual ~CViewNumericWnd(void);

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual bool CreateView();
	virtual bool Initialize();
	virtual void Open();
	virtual void Close();

	virtual void Show();
	virtual void Hide();

	
	//Observer
	virtual void NotifyViewStateChanged();
	virtual void NotifyVentModeChanged();
	virtual void NotifyParaBtnEvent(CMVEvent* pEvent);
	//virtual void NotifyLanguageChanged();
	virtual void NotifyDataChanged();
	virtual void NotifyExspirationDataChanged();
	virtual void NotifyAlarmLimitChanged();
	virtual void NotifyCalculateAlarmLimit();
	virtual void NotifyEvent(CMVEvent* pEvent);

	
protected:
	
	bool createWndNumConfig(UINT ID, NUMERICINI* pbufNumeric);
	void deleteWndNumConfig();


	void ShowWndNumIPPV();
	void ShowWndNumSIPPV();
	void ShowWndNumSIMV();
	void ShowWndNumSIMVPSV();
	void ShowWndNumPSV();
	void ShowWndNumCPAP();
	void ShowWndNumDUOPAP();
	void ShowWndNumNCPAP();
	void ShowWndNumHFO();
	void ShowWndNumTHERAPY();
	void ShowWndNumFLOWOFFCONV();
	void ShowWndNumFLOWOFFCPAP();
	void ShowWndNumFLOWOFFHFO();

	void redrawNumerics(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);;

	void DrawGraphCursor(int iDiagrmm);
	void DeleteGraphCursor();

	//void ShowNextNumericWnd();

	void SetNumericBlock(bool bChangeCurNumBlock);
	eNumBlock SetNumericBlock_IPPV();
	eNumBlock SetNumericBlock_SIMV();
	eNumBlock SetNumericBlock_SIMVPSV();
	eNumBlock SetNumericBlock_SIPPV();
	eNumBlock SetNumericBlock_PSV();
	eNumBlock SetNumericBlock_CPAP();
	eNumBlock SetNumericBlock_HFO();
	eNumBlock SetNumericBlock_NCPAP();
	eNumBlock SetNumericBlock_DUOPAP();
	eNumBlock SetNumericBlock_THERAPIE();

	void ShowNumWnd(eNumBlock eBlock,bool bChangeCurNumBlock);
	void HideNumWnd();

	void RedrawAlarmLimits();
	void CalculateAlarmLimit();

	void BnMenuNUMERIC();
	bool SetNextNumericBlock();

protected:
	CRITICAL_SECTION	csNumWnd;
	
	CPushBtn* m_pcMenuNumeric;

	CBmp* m_pcNumeric_Up;
	CBmp* m_pcNumeric_Dw;

	CWndNumConfig* m_pcWndNumConfig;


	NUMWNDATTRIB m_nwa;

	eNumBlock m_eNumBlock;

	int m_iCurrWnd;

	BYTE m_iCountNumericBlock;
	BYTE m_iCurNumericBlock;
	bool m_bExit;
	bool m_bBackState;
	
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
};
