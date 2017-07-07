#pragma once
#include "WndService.h"
//#include "BitmapSlider.h"
#include "KbrdBtn.h"

// CWndServiceLicensing

class CWndServiceLicensing : public CWndService
{
	DECLARE_DYNAMIC(CWndServiceLicensing)

public:
	CWndServiceLicensing();
	virtual ~CWndServiceLicensing();

void Init();

protected:
	void Draw();
	//void WriteLicenseFile();

private:
	CDialog*   m_pDlg;
	/*CBitmapSlider*	m_sliderEnable;

	CBmp* m_pcTest_Up;
	CBmp* m_pcTest_Dw;
	CBmp* m_pcTest_Dis;
	CKbrdBtn* m_pcTest;

	CBmp* m_pcNurse;*/

	CBmp* m_pcActivate_Up;
	CBmp* m_pcActivate_Dw;
	//CBmp* m_pcActivate_Dis;

	CKbrdBtn* m_pcActivateFOT;
	CKbrdBtn* m_pcActivateHFO;
	CKbrdBtn* m_pcActivateNMODE;
	CKbrdBtn* m_pcActivateVguaranty;
	CKbrdBtn* m_pcActivateVlimit;
	CKbrdBtn* m_pcActivateTrends;
	CKbrdBtn* m_pcActivateLungRec;
	CKbrdBtn* m_pcActivateTherapy;
	CKbrdBtn* m_pcActivatePRICO;
	CKbrdBtn* m_pcActivateNIVTRIGGER;

	CKbrdBtn* m_pcDelFOT;
	CKbrdBtn* m_pcDelHFO;
	//CKbrdBtn* m_pcDelCO2;
	CKbrdBtn* m_pcDelNMODE;
	CKbrdBtn* m_pcDelVguaranty;
	CKbrdBtn* m_pcDelVlimit;
	CKbrdBtn* m_pcDelTrends;
	CKbrdBtn* m_pcDelLungRec;
	CKbrdBtn* m_pcDelTherapy;
	CKbrdBtn* m_pcDelPRICO;
	CKbrdBtn* m_pcDelNIVTRIGGER;

	CStringW m_szLicenseFile;

	//CStringA m_szParsedMasterKey;
	CStringA m_szParsedHFOKey;
	//CStringA m_szParsedCO2Key;
	CStringA m_szParsedNMODEKey;
	CStringA m_szParsedVGUARANTYKey;
	CStringA m_szParsedVLIMITKey;
	CStringA m_szParsedTRENDSKey;
	CStringA m_szParsedLUNGRECKey;
	CStringA m_szParsedTHERAPYKey;
	CStringA m_szParsedPRICOKey;
	CStringA m_szParsedFOTKey;
	CStringA m_szParsedNIVTRIGGERKey;

	//bool m_bMasterModuleEnabled;
	bool m_bHFOModuleEnabled;
	//bool m_bCO2ModuleEnabled;
	bool m_bNMODEModuleEnabled;
	bool m_bVGUARANTModuleEnabled;
	bool m_bVLIMITModuleEnabled;
	bool m_bTRENDSModuleEnabled;
	bool m_bLUNGRECModuleEnabled;
	bool m_bTHERAPYModuleEnabled;
	bool m_bPRICOModuleEnabled;
	bool m_bFOTModuleEnabled;
	bool m_bNIVTRIGGERModuleEnabled;
	
	//CString m_szEncryptedFormattedMasterKey;
	CString m_szEncryptedFormattedHFOKey;
	//CString m_szEncryptedFormattedCO2Key;
	CString m_szEncryptedFormattedNMODEKey;
	CString m_szEncryptedFormattedVGUARANTYKey;
	CString m_szEncryptedFormattedVLIMITKey;
	CString m_szEncryptedFormattedTRENDSKey;
	CString m_szEncryptedFormattedLUNGRECKey;
	CString m_szEncryptedFormattedTHERAPYKey;
	CString m_szEncryptedFormattedPRICOKey;
	CString m_szEncryptedFormattedFOTKey;
	CString m_szEncryptedFormattedNIVTRIGGERKey;

	/*CStringA m_szEncryptedMasterKey;
	CStringA m_szEncryptedHFOKey;
	CStringA m_szEncryptedCO2Key;
	CStringA m_szEncryptedNMODEKey;
	CStringA m_szEncryptedVGUARANTYKey;
	CStringA m_szEncryptedVLIMITKey;*/

	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	/*afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);*/
	afx_msg void OnBnClickedActivateNIVTRIGGER();
	afx_msg void OnBnClickedActivateFOT();
	afx_msg void OnBnClickedActivateHFO();
	//afx_msg void OnBnClickedActivateCO2();
	afx_msg void OnBnClickedActivateNMODE();
	afx_msg void OnBnClickedActivateVGUARANTY();
	afx_msg void OnBnClickedActivateVLIMIT();
	afx_msg void OnBnClickedActivateLUNGREC();
	afx_msg void OnBnClickedActivateTRENDS();
	afx_msg void OnBnClickedActivateTHERAPY();
	afx_msg void OnBnClickedActivatePRICO();
	afx_msg void OnBnClickedDelNIVTRIGGER();
	afx_msg void OnBnClickedDelFOT();
	afx_msg void OnBnClickedDelHFO();
	//afx_msg void OnBnClickedDelCO2();
	afx_msg void OnBnClickedDelNMODE();
	afx_msg void OnBnClickedDelVGUARANTY();
	afx_msg void OnBnClickedDelVLIMIT();
	afx_msg void OnBnClickedDelLUNGREC();
	afx_msg void OnBnClickedDelTRENDS();
	afx_msg void OnBnClickedDelTHERAPY();
	afx_msg void OnBnClickedDelPRICO();
};
