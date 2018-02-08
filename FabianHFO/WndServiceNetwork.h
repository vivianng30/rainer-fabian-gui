#pragma once
#include "WndService.h"
#include "BitmapSlider.h"
#include "KbrdBtn.h"

// CWndServiceNetwork

class CWndServiceNetwork : public CWndService
{
	DECLARE_DYNAMIC(CWndServiceNetwork)

public:
	CWndServiceNetwork();
	virtual ~CWndServiceNetwork();

	void Init();

protected:
	void Draw();

private:
	/*CBitmapSlider*	m_sliderEnable;

	CBmp* m_pcTest_Up;
	CBmp* m_pcTest_Dw;
	CBmp* m_pcTest_Dis;
	CKbrdBtn* m_pcTest;

	CBmp* m_pcNurse;*/

	CBitmapSlider*	m_sliderTerminalWave;
	//bool m_bTerminalWave;

	bool m_bAcuLink;
	bool m_bAcuLinkLicense;

	UINT m_uiPort;
	BYTE m_iPDMSProtocol;

	CString m_szIP;
	CString m_szGateway;
	CString m_szSubnet;
	bool m_bDHCP;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	/*afx_msg void OnBnClickedTestNursecall();*/
};
