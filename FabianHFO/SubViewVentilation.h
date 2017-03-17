#pragma once
#include "SelectSetupBtn.h"
#include "globDefs.h"
#include "MVModel.h"
#include "colour.h"
#include "KbrdBtn.h"

// CSubViewVentilation

class CSubViewVentilation : public CWnd
{
	DECLARE_DYNAMIC(CSubViewVentilation)

public:
	CSubViewVentilation();
	virtual ~CSubViewVentilation();

	bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void Show(bool bShow);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//void NotifyLanguageChanged();

protected:
	void Draw();
	//void DrawStatic(CDC* pDC);

	void SetOneButtonDepressed(int btnID);
	void SetOneButtonFocused(int btnID);
	void SetNextButtonFocused();
	void SetPrevButtonFocused();
	eBtnState GetCurrentBtnState();
	//void SetNextButtonValue();
	//void SetPrevButtonValue();
	void SetButtonClicked(int btnID);

	void OpenSettingsView();

	CMVModel *getModel();

private:
	CMVModel* m_pModel;

	CDialog*   m_pDlg;

	// Attributes
	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	/*HDC		m_hdcStatic;
	HBITMAP m_hbmpStatic;*/


	//CBtn* m_pcNextRight;

	CBmp* m_pcRegionTopLeCorner;
	CBmp* m_pcRegionTopRiCorner;
	CBmp* m_pcRegionBotLeCorner;
	CBmp* m_pcRegionBotRiCorner;

	CBmp* m_pcSetupVent_Up;
	CBmp* m_pcSetupVent_Dw;
	CBmp* m_pcSetupVent_Fc;
	CBmp* m_pcSetupVent_Dis;

	CBmp* m_pcDefaults_Up;
	CBmp* m_pcDefaults_Dw;
	CBmp* m_pcDefaults_Fc;
	CSelectSetupBtn* m_pcDefaults;

	CBmp* m_pcHospital_Up;
	CBmp* m_pcHospital_Dw;
	CBmp* m_pcHospital_Fc;
	CSelectSetupBtn* m_pcHospital;

	CSelectSetupBtn* m_pcLeakComp;
	CSelectSetupBtn* m_pcFuncITime;
	CSelectSetupBtn* m_pcManBreathTime;
	CSelectSetupBtn* m_pcTrigger;
	CSelectSetupBtn* m_pcTubeSet;
	CSelectSetupBtn* m_pcPressureUnit;
	CSelectSetupBtn* m_pcHFOManBreath;
	CSelectSetupBtn* m_pcEFlow;
	CSelectSetupBtn* m_pcAutoOxyCal;
	CSelectSetupBtn* m_pcPatientAlarmDelay;
	CSelectSetupBtn* m_pcPPSVasDeltaPEEPValue;
	CSelectSetupBtn* m_pcBreathToBreathVT;

	DWORD m_dwLastSetupTimer;

	int m_iCurPara;

	WORD m_kUp;
	WORD m_kDown;
	WORD m_kSpace;

	eIERelationMode m_eModeITime;
	eRefFlowSensor m_eRefFlowSensor;
	eTubeSet m_eTubeSet;
	ePressureUnit m_ePressureUnit;
	eAutoOxyCal m_eAutoOxyCal;
	WORD m_iEFlow;
	int m_iManBreathTime;
	UINT m_iPatientAlarmDelay;
	BOOL m_bPpsvAsDeltaPEEPValue;
	bool m_bUseTveBTB;
	eLeakCompensation m_eLeakComp;
	//BYTE m_byFlushTime;
	bool m_bHFOManBreathEnabled;
	//bool m_bVolumeTrigger;
	eTriggereType m_eTriggerType;

	CList<CSelectSetupBtn* , CSelectSetupBtn *> m_plBtn;

	//TEST
	//CBmp* m_pcTest;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedLeakComp();
	afx_msg void OnBnClickedModeIT();
	afx_msg void OnBnClickedManBreathTime();
	afx_msg void OnBnClickedPatAlarmDelay();
	afx_msg void OnBnClickedPPSVasDeltaPEEPValue();
	afx_msg void OnBnClickedBTBforVT();
	afx_msg void OnBnClickedVolumeTrigger();
	afx_msg void OnBnClickedTubeSet();
	afx_msg void OnBnClickedPressureUnit();
	afx_msg void OnBnClickedEFlow();
	afx_msg void OnBnClickedAutoOxyCal();
	afx_msg void OnBnClickedHFOManBreath();
	afx_msg void OnBnClickedDefaults();
	afx_msg void OnBnClickedHospital();
};