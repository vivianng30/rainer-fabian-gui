/**********************************************************************************************//**
 * \file	NumericFieldVTE.h.
 *
 * Declares the numeric field vte class
 **************************************************************************************************/

#pragma once
#include "NumericField.h"

// CNumericFieldVTE

class CNumericFieldVTE : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldVTE)

public:
	CNumericFieldVTE(eNumericSize size);
	virtual ~CNumericFieldVTE();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldVTE)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



