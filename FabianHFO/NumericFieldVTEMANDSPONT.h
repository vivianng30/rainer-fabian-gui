/**********************************************************************************************//**
 * \file	NumericFieldVTEMANDSPONT.h.
 *
 * Declares the numeric field vtemandspont class
 **************************************************************************************************/

#pragma once
#include "NumericField.h"

// CNumericFieldVTEMANDSPONT

class CNumericFieldVTEMANDSPONT : public CNumericField
{
	DECLARE_DYNAMIC(CNumericFieldVTEMANDSPONT)

public:
	CNumericFieldVTEMANDSPONT(eNumericSize size);
	virtual ~CNumericFieldVTEMANDSPONT();

	bool drawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);
private:

	//bool drawFrames(CDC* pDC);
	bool drawStaticText(CDC* pDC);
	bool drawLimits(CDC* pDC);

protected:
	CString m_szNameNoteResp;
	CString m_szNameNotePat;

protected:
	//{{AFX_MSG(CNumericFieldVTEMANDSPONT)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


