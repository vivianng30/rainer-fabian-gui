/**********************************************************************************************//**
 * \class	CChildView
 *
 * \brief	Base class for child views.
 *
 * \author	Rainer
 * \date	18.10.2017
 * 			
 **************************************************************************************************/

#pragma once


class CChildView : public CWnd
{
// Construction
public:


	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:


	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:


	virtual ~CChildView();

private:
	CBrush m_brsBkGrnd; ///< The brs block grnd

	// Generated message map functions
protected:

	afx_msg void OnPaint();

	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

