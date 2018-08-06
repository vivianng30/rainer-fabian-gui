/**********************************************************************************************//**
 * \file	NumericFieldC20C.h.
 *
 * Declares the numeric field c20c class
 **************************************************************************************************/

#pragma once
#include "NumericField.h"

// CNumericFieldC20C

class CNumericFieldC20C : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldC20C)

public:
	CNumericFieldC20C(eNumericSize size);
	virtual ~CNumericFieldC20C();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldC20C)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


