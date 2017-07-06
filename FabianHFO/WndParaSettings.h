#pragma once
#include "MVModel.h"
#include "UpDwnBtn.h"
#include "PresetMenuBtn.h"
//#include "DwnBtn.h"

#include "ParaBtn_FlushTime.h"
#include "ParaBtn_ManBreath.h"
#include "ParaBtn_PCurve.h"
#include "ParaBtn_AbortPSV.h"
#include "ParaBtn_FREQ_REC.h"
#include "ParaBtn_EFLOW.h"
#include "ParaBtn_LeakCompensation.h"

// CWndParaSettings

class CWndParaSettings : public CWnd
{
	DECLARE_DYNAMIC(CWndParaSettings)

public:
	CWndParaSettings();
	virtual ~CWndParaSettings();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void Show(bool bShow);

	void SetViewFocus(int iBtn);
	bool SetNextFocus();
	bool SetPrevFocus();
protected:
	CMVModel *getModel();

	void Draw();

	/*void OnBnClickedShowSIMVPSV();
	void OnBnClickedHideSIMVPSV();*/
	/*void OnBnClickedVG();
	void OnBnClickedVL();
	void OnBnClickedFreshgasInt();
	void OnBnClickedFreshgasExt();*/

	//FRESHGAS
	//void SetAllButtonUnpressed_Freshgas();
	void showFreshgasMessage(int btnID);

	void SetOneButtonDepressed_Freshgas(int btnID);
	void SetOneButtonPreset_Freshgas(int btnID);
	/*void SetButtonState_Freshgas(int btnID, CPresetMenuBtn::eBtnState state);
	void ResetButtonPresetstate_Freshgas();*/

	//VOLUME
	//void SetAllButtonUnpressed_Volume();
	void SetOneButtonDepressed_Volume(int btnID);
	void SetOneButtonPreset_Volume(int btnID);
	/*void SetButtonState_Volume(int btnID, CPresetMenuBtn::eBtnState state);
	void ResetButtonPresetstate_Volume();*/

	void setBiasFlow();
	void setVG_VL();

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

	CBmp* m_pcSelSetting_Up;
	CBmp* m_pcSelSetting_Dw;
	CBmp* m_pcSelSetting_Fc;
	//CBmp* m_pcSelSetting_Dis;
	/*CDwnBtn* m_pcSIMVPSV_ON;
	CDwnBtn* m_pcSIMVPSV_OFF;
	CDwnBtn* m_pcFreshgasInt;
	CDwnBtn* m_pcFreshgasExt;
	CDwnBtn* m_pcVolumeGuarantee;
	CDwnBtn* m_pcVolumeLimit;*/

	CPresetMenuBtn* m_pcFreshgasInt;
	CPresetMenuBtn* m_pcFreshgasExt;
	CPresetMenuBtn* m_pcVolumeGuarantee;
	CPresetMenuBtn* m_pcVolumeLimit;

	CBmp* m_pcParaMenu_Up;
	CBmp* m_pcParaMenu_Dw;
	CUpDwnBtn* m_pcParaMenu;

	CBmp* m_pcPara_UpT;
	CBmp* m_pcPara_FcT;
	CBmp* m_pcPara_DwT;
	CBmp* m_pcPara_UpB;
	CBmp* m_pcPara_FcB;
	CBmp* m_pcPara_DwB;
	CBmp* m_pcPara_DisB;

	/*CUpDwnBtn* m_pcPara_OnOff;
	CBmp* m_pcPara_OnOff_Up;
	CBmp* m_pcPara_OnOff_Dw;*/

	CUpDwnBtn* m_pcPara_Confirm;
	CBmp* m_pcPara_Confirm_yes;
	CBmp* m_pcPara_Confirm_no;

	CBmp* m_pcExclamation;

	CParaBtn_FREQ_REC* m_pcPara_FREQ_REC;
	CParaBtn_PCurve* m_pcPara_PCurve;
	CParaBtn_AbortPSV* m_pcPara_AbortPSV;
	CParaBtn_ManBreath* m_pcPara_ManBreath;
	CParaBtn_FlushTime* m_pcPara_FlushTime;
	CParaBtn_EFLOW* m_pcPara_EFLOW;
	CParaBtn_LeakCompensation* m_pcPara_LeakCompensation;

	eParaSetWarning m_enumMessage;

	bool m_bBiasFlowConfirmed;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	/*afx_msg void OnBnClickedShowSIMVPSV();
	afx_msg void OnBnClickedHideSIMVPSV();
	afx_msg void OnBnClickedVG();
	afx_msg void OnBnClickedVL();
	afx_msg void OnBnClickedFreshgasInt();
	afx_msg void OnBnClickedFreshgasExt();*/
};


