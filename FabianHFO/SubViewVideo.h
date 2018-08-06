#pragma once
#include "globDefs.h"
#include "MVModel.h"
//#include "PushBtn.h"
#include "KbrdBtn.h"
#include "colour.h"

// CSubViewVideo

class CSubViewVideo : public CWnd
{
	DECLARE_DYNAMIC(CSubViewVideo)

public:
	CSubViewVideo();
	virtual ~CSubViewVideo();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	void Show(bool bShow);

	void StopVideo();
protected:
	void Initialize();
	void Draw();

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

	//CWndWaitHourglass *m_pWndHourglass;

	CBmp* m_pcRegionTopLeCorner;
	CBmp* m_pcRegionTopRiCorner;
	CBmp* m_pcRegionBotLeCorner;
	CBmp* m_pcRegionBotRiCorner;

	CDialog*   m_pDlg;

	CBmp* m_pcStop_Up;
	CBmp* m_pcStop_Dw;
	CBmp* m_pcStart_Up;
	CBmp* m_pcStart_Dw;

	CKbrdBtn* m_pcStartStop;

	CBmp* m_pcNoVideo;

	bool m_bExit;

	CString m_sizeX;
	CString m_sizeY;
	CString m_posX;
	CString m_posY;

	bool m_bVideoRunning;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedStartStopVideo();
};


