// ChildView.h : interface of the CChildView class
//


#pragma once

/**********************************************************************************************//**
 * CChildView window.
 *
 * \author	Rainer Kühner
 * \date	11.07.2017
 **************************************************************************************************/

class CChildView : public CWnd
{
// Construction
public:

	/**********************************************************************************************//**
	 * Default constructor.
	 *
	 * \author	Rainer Kühner
	 * \date	11.07.2017
	 **************************************************************************************************/

	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:

	/**********************************************************************************************//**
	 * Pre create window.
	 *
	 * \author	Rainer Kühner
	 * \date	11.07.2017
	 *
	 * \param [in,out]	cs	The create struct.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:

	/**********************************************************************************************//**
	 * Destructor.
	 *
	 * \author	Rainer Kühner
	 * \date	11.07.2017
	 **************************************************************************************************/

	virtual ~CChildView();

private:
	CBrush m_brsBkGrnd; ///< The brs block grnd

	// Generated message map functions
protected:

	/**********************************************************************************************//**
	 * Paints this window.
	 *
	 * \author	Rainer Kühner
	 * \date	11.07.2017
	 **************************************************************************************************/

	afx_msg void OnPaint();

	/**********************************************************************************************//**
	 * Executes the erase bkgnd action.
	 *
	 * \author	Rainer Kühner
	 * \date	11.07.2017
	 *
	 * \param [in,out]	pDC	If non-null, the device-context.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

