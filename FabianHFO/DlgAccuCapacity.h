#pragma once


// CDlgAccuCapacity dialog

class CDlgAccuCapacity : public CDialog
{
	DECLARE_DYNAMIC(CDlgAccuCapacity)

public:
	explicit CDlgAccuCapacity(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAccuCapacity();

// Dialog Data
	enum { IDD = IDD_DLGACCUTEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
