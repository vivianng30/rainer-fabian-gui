/**********************************************************************************************//**
 * \file	NumericFieldIFLOW.h.
 *
 * Declares the numeric field iflow class
 **************************************************************************************************/

#pragma once
#include "NumericField.h"

// CNumericFieldIFLOW

class CNumericFieldIFLOW : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldIFLOW)

public:
	CNumericFieldIFLOW(eNumericSize size);
	virtual ~CNumericFieldIFLOW();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	//{{AFX_MSG(CNumericFieldIFLOW)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
