/**********************************************************************************************//**
 * \file	NumericFieldCOMPLIANCE.h.
 *
 * Declares the numeric field compliance class
 **************************************************************************************************/

#pragma once
#include "NumericField.h"

// CNumericFieldCOMPLIANCE

class CNumericFieldCOMPLIANCE : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldCOMPLIANCE)

public:
	CNumericFieldCOMPLIANCE(eNumericSize size);
	virtual ~CNumericFieldCOMPLIANCE();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldCOMPLIANCE)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

