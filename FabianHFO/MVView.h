#pragma once
#include "MVModel.h"
#include "globDefs.h"
#include "colour.h"

extern HFONT g_hf3AcuNorm;
extern HFONT g_hf4AcuNorm;
extern HFONT g_hf5AcuNorm;
extern HFONT g_hf6AcuNorm;
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf7AcuBold90degree;
extern HFONT g_hf8AcuNorm;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf9AcuBold;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf13AcuBold;
extern HFONT g_hf14AcuMed;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf15AcuMed;
extern HFONT g_hf14AcuNormNum;

//extern HFONT g_hf24Medium;

extern HFONT g_hf17AcuBold;
extern HFONT g_hf19AcuMed;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf23AcuBold;
extern HFONT g_hf25AcuMed;
extern HFONT g_hf27AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf33AcuBold;
extern HFONT g_hf34BoldNum;
extern HFONT g_hf43AcuBold;
extern HFONT g_hf53AcuBold;
extern HFONT g_hf70Bold;
//extern HFONT g_hf80Bold;
//extern HFONT g_hf90Bold;



/////////////////////////////////////////////////////////////////////////////
//typedef struct LINEDIAGRAMATTRIB
//{
//	int			nX;
//	int			nY;
//	int			nCx;
//	int			nCy;
//	int			nPage;
//}LINEDIAGRAMATTRIB;

//class CModelObserver;
// CMVView

class CMVView : public CWnd, public CModelObserver
{

public:
	CMVView(int ViewID = 0);
	virtual ~CMVView();

	virtual BOOL Create(CWnd* pParentWnd, const RECT &rc, UINT nID, CCreateContext* pContext = NULL);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//virtual BOOL PreTranslateMessage(MSG* pMsg);

	//update function observer
	virtual void NotifyDataChanged();

	virtual bool CreateView()=0;
	virtual bool Initialize()=0;
	virtual void Open();
	virtual void Close();

	virtual void Show();
	virtual void Show(bool bRedraw=false);
	virtual void Hide();

	virtual void SetViewFocus();
	virtual bool SetNextFocus();
	virtual bool SetPrevFocus();

	int GetViewID(){return m_iViewID;};

	//virtual void Trigger(int btnID=0,int iGroupId=0)=0;
	//virtual void SetFocusTo(int iID){};
	//CWnd *GetParentView();

	CMVModel *getModel();

	

protected:
	//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:

	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;
	
	//CBrush m_brsBkGrnd;

	int m_iViewID;

	WORD m_kUp;
	WORD m_kDown;
	WORD m_kSpace;

	
private:
	CMVModel* m_pModel;

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg void OnDestroy();
	afx_msg void OnDestroy();
};


