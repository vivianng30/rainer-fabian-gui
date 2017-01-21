#pragma once
#include "globDefs.h"
#include "colour.h"
#include "MVModel.h"
#include "MenuBtn.h"
//#include "SelectSetupBtn.h"

// CSubViewMenuSelect

enum eMenuView {
	MV_MENU			= 0,
	MV_CALIBRATION	= 1,
	MV_DISPLAY		= 2,
	MV_VENTILATION	= 3,
	MV_PATIENTDATA	= 4,
	MV_LANGUAGE		= 5,
	MV_DATETIME		= 6,
	MV_TOOLS		= 7,
	MV_SYSTEM		= 8,
	MV_SERVICEMODE	= 9,
	//MV_HELP			= 11,
	MV_VIDEO		= 10,
	MV_NETWORK		= 11,
	MV_O2			= 12,
	MV_CO2			= 13,
	MV_SPO2			= 14

};

class CSubViewMenuSelect : public CWnd
{
	DECLARE_DYNAMIC(CSubViewMenuSelect)

	

public:
	CSubViewMenuSelect();
	virtual ~CSubViewMenuSelect();

	bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void SetOneButtonDepressed(int btnID);
	void SetAllButtonUnpressed();
	void SetOneButtonFocused(int btnID);
	void SetNextButtonFocused();
	void SetPrevButtonFocused();
	/*eBtnState GetCurrentBtnState();
	eBtnState GetBtnState(int btnID);*/

	void Show(bool bShow, eMenuView view);


protected:
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



	CMenuBtn* m_pcMenuCalibration;
	CMenuBtn* m_pcMenuDisplay;
	CMenuBtn* m_pcMenuVentilation;
	CMenuBtn* m_pcMenuPatient;
	CMenuBtn* m_pcMenuLanguage;
	CMenuBtn* m_pcMenuDate;
	CMenuBtn* m_pcMenuInfo;
	CMenuBtn* m_pcMenuService;
	CMenuBtn* m_pcMenuTools;
	//CMenuBtn* m_pcMenuHelp;
	//CMenuBtn* m_pcMenuCO2;
	//CMenuBtn* m_pcMenuVideo;
	CMenuBtn* m_pcMenuNetwork;
	//CMenuBtn* m_pcMenuSpO2;

	CBmp* m_pcMenuCalibration_UP;
	CBmp* m_pcMenuCalibration_DW;
	CBmp* m_pcMenuCalibration_FC;
	
	CBmp* m_pcMenuDisplay_UP;
	CBmp* m_pcMenuDisplay_DW;
	CBmp* m_pcMenuDisplay_FC;
	
	CBmp* m_pcMenuVentilation_UP;
	CBmp* m_pcMenuVentilation_DW;
	CBmp* m_pcMenuVentilation_FC;
	
	CBmp* m_pcMenuPatient_UP;
	CBmp* m_pcMenuPatient_DW;
	CBmp* m_pcMenuPatient_FC;
	
	CBmp* m_pcMenuLanguage_UP;
	CBmp* m_pcMenuLanguage_DW;
	CBmp* m_pcMenuLanguage_FC;
	
	CBmp* m_pcMenuDate_UP;
	CBmp* m_pcMenuDate_DW;
	CBmp* m_pcMenuDate_FC;
	
	CBmp* m_pcMenuInfo_UP;
	CBmp* m_pcMenuInfo_DW;
	CBmp* m_pcMenuInfo_FC;
	
	CBmp* m_pcMenuTools_UP;
	CBmp* m_pcMenuTools_DW;
	CBmp* m_pcMenuTools_FC;
	CBmp* m_pcMenuTools_DIS;
	
	CBmp* m_pcMenuService_UP;
	CBmp* m_pcMenuService_DW;
	CBmp* m_pcMenuService_FC;

	CBmp* m_pcMenuHelp_UP;
	CBmp* m_pcMenuHelp_DW;
	CBmp* m_pcMenuHelp_FC;
	CBmp* m_pcMenuHelp_DIS;

	/*CBmp* m_pcMenuCO2_UP;
	CBmp* m_pcMenuCO2_DW;
	CBmp* m_pcMenuCO2_FC;
	CBmp* m_pcMenuCO2_DIS;

	CBmp* m_pcMenuSPO2_UP;
	CBmp* m_pcMenuSPO2_DW;
	CBmp* m_pcMenuSPO2_FC;
	CBmp* m_pcMenuSPO2_DIS;*/

	/*CBmp* m_pcMenuVideo_UP;
	CBmp* m_pcMenuVideo_DW;
	CBmp* m_pcMenuVideo_FC;
	CBmp* m_pcMenuVideo_DIS;*/

	CBmp* m_pcMenuNetwork_UP;
	CBmp* m_pcMenuNetwork_DW;
	CBmp* m_pcMenuNetwork_FC;
	CBmp* m_pcMenuNetwork_DIS;

	CList<CMenuBtn* , CMenuBtn *> m_plMenuBtn;

	DWORD m_dwLastSetupTimer;

	int m_iCurrentField;

	bool m_bCO2FeatureAvailable;
	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCalibration();
	afx_msg void OnBnClickedDisplay();
	afx_msg void OnBnClickedVentilation();
	afx_msg void OnBnClickedPatientdata();
	afx_msg void OnBnClickedLanguage();
	afx_msg void OnBnClickedDate();
	afx_msg void OnBnClickedInfo();
	afx_msg void OnBnClickedService();
	afx_msg void OnBnClickedTools();
	//afx_msg void OnBnClickedHelp();
	//afx_msg void OnBnClickedCO2();
	//afx_msg void OnBnClickedSPO2();
	afx_msg void OnBnClickedVideo();
	afx_msg void OnBnClickedNetwork();
};


