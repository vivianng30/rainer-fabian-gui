/**********************************************************************************************//**
 * \file	NumericFieldRSBI.h.
 *
 * Declares the numeric field rsbi class
 **************************************************************************************************/

#pragma once
#include "NumericField.h"

// CNumericFieldRSBI

class CNumericFieldRSBI : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldRSBI)

public:
	CNumericFieldRSBI(eNumericSize size);
	virtual ~CNumericFieldRSBI();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldRSBI)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


