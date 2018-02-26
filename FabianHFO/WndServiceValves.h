/**********************************************************************************************//**
 * \file	WndServiceValves.h.
 *
 * Declares the window service valves class
 **************************************************************************************************/

#pragma once
#include "WndService.h"
#include "KbrdBtn.h"
#include "UpDwnBtn.h"
#include "UDBtn.h"

// CWndServiceValves

class CWndServiceValves : public CWndService
{
	DECLARE_DYNAMIC(CWndServiceValves)

public:
	CWndServiceValves();
	virtual ~CWndServiceValves();

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void Init();

protected:
	void Draw();

	/*void ClearFgBit(int iBit);
	void SetFgBit(int iBit);*/

private:
	/*CBmp* m_pcValve_Up;
	CBmp* m_pcValve_Dw;

	CBmp* m_pcValveOff_Up;
	CBmp* m_pcValveOff_Dw;

	CBmp* m_pcFg_Up;
	CBmp* m_pcFg_Dw;

	CBmp* m_pcOpen_Up;
	CBmp* m_pcOpen_Dw;
	CKbrdBtn* m_pcExpValveOpen;
	CKbrdBtn* m_pcExpValveClose;
	CKbrdBtn* m_pcOpValveOpen;
	CKbrdBtn* m_pcOpValveClose;

	CUDBtn* m_pcExpValve;
	CUDBtn* m_pcOpenValve;

	CKbrdBtn* m_pcValveOff;

	CUpDwnBtn* m_pcValve0;
	CUpDwnBtn* m_pcValve1;
	CUpDwnBtn* m_pcValve2;
	CUpDwnBtn* m_pcValve3;
	CUpDwnBtn* m_pcValve8;
	CUpDwnBtn* m_pcValve9;
	CUpDwnBtn* m_pcValve10;
	CUpDwnBtn* m_pcValve11;
	CUpDwnBtn* m_pcValve12;
	CUpDwnBtn* m_pcValve13;
	CUpDwnBtn* m_pcValve14;
	CUpDwnBtn* m_pcValve15;
	CUpDwnBtn* m_pcValve16;
	CUpDwnBtn* m_pcValve17;
	CUpDwnBtn* m_pcValve18;
	CUpDwnBtn* m_pcValve19;
	CUpDwnBtn* m_pcValve20;
	CUpDwnBtn* m_pcValve21;
	CUpDwnBtn* m_pcValve22;
	CUpDwnBtn* m_pcValve23;*/

	CPushBtn* m_pcVentilBurn;
	CBmp* m_pcMenu_UP;
	CBmp* m_pcMenu_DW;
	bool m_bVentilBurnRunning;
	
	int m_iFlow; //Achtung, tatsächlicher Meß-Wert ist um Faktor 2 größer
	int m_iPaw;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedVentilBurn();
	/*afx_msg void OnBnClickedValv0();
	afx_msg void OnBnClickedValv1();
	afx_msg void OnBnClickedValv2();
	afx_msg void OnBnClickedValv3();
	afx_msg void OnBnClickedValv8();
	afx_msg void OnBnClickedValv9();
	afx_msg void OnBnClickedValv10();
	afx_msg void OnBnClickedValv11();
	afx_msg void OnBnClickedValv12();
	afx_msg void OnBnClickedValv13();
	afx_msg void OnBnClickedValv14();
	afx_msg void OnBnClickedValv15();
	afx_msg void OnBnClickedValv16();
	afx_msg void OnBnClickedValv17();
	afx_msg void OnBnClickedValv18();
	afx_msg void OnBnClickedValv19();
	afx_msg void OnBnClickedValv20();
	afx_msg void OnBnClickedValv21();
	afx_msg void OnBnClickedValv22();
	afx_msg void OnBnClickedValv23();
	afx_msg void OnBnClickedValvOff();
	afx_msg void OnBnClickedExpValveOpen();
	afx_msg void OnBnClickedExpValveClose();
	afx_msg void OnBnClickedOpValveOpen();
	afx_msg void OnBnClickedOpValveClose();*/
};


