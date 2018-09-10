// WndNumConfig.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndNumConfig.h"

#include "NumericFieldC20C.h"
#include "NumericFieldCOMPLIANCE.h"
#include "NumericFieldDCO2.h"
#include "NumericFieldDEMANDFLOW.h"
#include "NumericFieldEFLOW.h"
#include "NumericFieldETCO2.h"
#include "NumericFieldFREQ.h"
#include "NumericFieldFREQETCO2.h"
#include "NumericFieldHFOAMP.h"
#include "NumericFieldHFOFREQ.h"
#include "NumericFieldHFOVTE.h"
#include "NumericFieldIFLOW.h"
#include "NumericFieldLEAK.h"
#include "NumericFieldMVEXP.h"
#include "NumericFieldMECHANIC.h"
#include "NumericFieldOXY.h"
#include "NumericFieldPEEP.h"
#include "NumericFieldPMEAN.h"
#include "NumericFieldPPEAK.h"
#include "NumericFieldPSVTIME.h"
#include "NumericFieldRESISTANCE.h"
#include "NumericFieldRSBI.h"
#include "NumericFieldSHAREMV.h"
#include "NumericFieldSPO2.h"
#include "NumericFieldSPO2PI.h"
#include "NumericFieldSPO2PULSERATE.h"
#include "NumericFieldTAU.h"
#include "NumericFieldTRIGGER.h"
#include "NumericFieldVTE.h"
#include "NumericFieldVTEMAND.h"
#include "NumericFieldVTEMANDSPONT.h"
#include "NumericFieldVTESPONT.h"
#include "NumericFieldVTI.h"
#include "NumericFieldFICO2.h"
#include "NumericFieldVTEKG.h"
#include "NumericFieldINFOPATAL.h"
#include "NumericFieldINFOTUBE.h"
#include "NumericFieldSPACE.h"
#include "NumericFieldINFOFLOWOFF.h"

/**********************************************************************************************//**
 * CWndNumConfig
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndNumConfig, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndNumConfig class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pbufNumeric	If non-null, the pbuf numeric.
 **************************************************************************************************/

CWndNumConfig::CWndNumConfig(NUMERICINI* pbufNumeric):
CWndNumeric()
{
	m_pbufNumeric=pbufNumeric;
	 m_bExit=false;
	
	m_iSizeComplete=0;
	m_iCurrentNumPositionYaxes=0;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndNumConfig class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndNumConfig::~CWndNumConfig()
{
}


BEGIN_MESSAGE_MAP(CWndNumConfig, CWnd)
	//{{AFX_MSG_MAP(CWndNumConfig)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CURSORBTN_UP, &CWndNumeric::OnBnClickedCursorUp)
	ON_BN_CLICKED(IDC_CURSORBTN_DOWN, &CWndNumeric::OnBnClickedCursorDwn)
	ON_BN_CLICKED(IDC_CURSORBTN_LEFT, &CWndNumeric::OnBnClickedCursorLeft)
	ON_BN_CLICKED(IDC_CURSORBTN_RIGHT, &CWndNumeric::OnBnClickedCursorRight)
	ON_BN_CLICKED(IDC_CURSORBTN_XUP, &CWndNumeric::OnBnClickedMoveXUp)
	ON_BN_CLICKED(IDC_CURSORBTN_XDOWN, &CWndNumeric::OnBnClickedMoveXDown)
	ON_BN_CLICKED(IDC_CURSORBTN_YLEFT, &CWndNumeric::OnBnClickedMoveYLeft)
	ON_BN_CLICKED(IDC_CURSORBTN_YRIGHT, &CWndNumeric::OnBnClickedMoveYRight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndNumConfig::OnPaint() 
{
	CPaintDC dc(this);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

/**********************************************************************************************//**
 * CWndNumConfig message handlers
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndNumConfig::init() 
{
	if(m_bExit)
		return;

	if(m_pbufNumeric->SHOW==TRUE)
	{
		if(m_pbufNumeric->SIZE1!=NUMERICSIZE_0)
		{
			createNumericFields(1);
		}

		if(m_pbufNumeric->SIZE2!=NUMERICSIZE_0)
		{
			createNumericFields(2);
		}

		if(m_pbufNumeric->SIZE3!=NUMERICSIZE_0)
		{
			createNumericFields(3);
		}

		if(m_pbufNumeric->SIZE4!=NUMERICSIZE_0)
		{
			createNumericFields(4);
		}

		if(m_pbufNumeric->SIZE5!=NUMERICSIZE_0)
		{
			createNumericFields(5);
		}

		if(m_pbufNumeric->SIZE6!=NUMERICSIZE_0)
		{
			createNumericFields(6);
		}

		if(m_pbufNumeric->SIZE7!=NUMERICSIZE_0)
		{
			createNumericFields(7);
		}

		if(m_pbufNumeric->SIZE8!=NUMERICSIZE_0)
		{
			createNumericFields(8);
		}

	}
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndNumConfig::OnDestroy() 
{
	m_bExit=true;

	EnterCriticalSection(&csDraw);
	CNumericField* pNumField=NULL;
	int iSizeOfArray = m_pNumFieldArray.GetSize();

	if(0 < iSizeOfArray)
	{
		for(int iIndex = iSizeOfArray; iIndex>0; iIndex--)	//durchläuft Array 
		{		
			pNumField = m_pNumFieldArray.ElementAt( iIndex-1 );
			m_pNumFieldArray.RemoveAt( iIndex-1 );
			if(pNumField)
			{
				pNumField->DestroyWindow();
				delete pNumField;
			}
		}
	}
	pNumField = NULL;
	LeaveCriticalSection(&csDraw);
	
	CWndNumeric::OnDestroy();
}

/**********************************************************************************************//**
 * Creates numeric fields
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	block	The block.
 **************************************************************************************************/

void CWndNumConfig::createNumericFields(BYTE block)
{
	if(m_bExit)
		return;

	RECT rcLd;
	rcLd.left=0;
	rcLd.right=190;
	rcLd.top=m_iCurrentNumPositionYaxes;	
	
	//NUMERICINI* pbufNumeric=

	switch(block)
	{
	case 1:
		{
			createNumericField1();
		}
		break;
	case 2:
		{
			createNumericField2();
		}
		break;
	case 3:
		{
			createNumericField3();
		}
		break;
	case 4:
		{
			createNumericField4();
		}
		break;
	case 5:
		{
			createNumericField5();
		}
		break;
	case 6:
		{
			createNumericField6();
		}
		break;
	case 7:
		{
			createNumericField7();
		}
		break;
	case 8:
		{
			createNumericField8();
		}
		break;
	}
}

/**********************************************************************************************//**
 * Gets bottom number position yaxes
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	numSize	Number of sizes.
 *
 * \return	The bottom number position yaxes.
 **************************************************************************************************/

SHORT CWndNumConfig::getBottomNumPositionYaxes(eNumericSize numSize)
{
	SHORT iBottom=0;
	switch(numSize)
	{
	case NUMERICSIZE_2:
		{
			iBottom=m_iCurrentNumPositionYaxes+119+5;
		}
		break;
	case NUMERICSIZE_3:
		{
			iBottom=m_iCurrentNumPositionYaxes+181+5;
		}
		break;
	case NUMERICSIZE_1:
	default:
		{
			iBottom=m_iCurrentNumPositionYaxes+57+5;
		}
		break;
	}
	return iBottom;
}

/**********************************************************************************************//**
 * Creates new numeric field
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	numType	Number of types.
 * \param	numSize	Number of sizes.
 *
 * \return	Null if it fails, else the new new numeric field.
 **************************************************************************************************/

CNumericField* CWndNumConfig::createNewNumericField(eNumericType numType, eNumericSize numSize)
{
	CNumericField *pNumField=NULL;
	EnterCriticalSection(&csDraw);
	switch(numType)
	{
	case NUMT_PPEAK:
		{
			pNumField= new CNumericFieldPPEAK(numSize);
		}
		break;
	case NUMT_PMEAN:
		{
			pNumField= new CNumericFieldPMEAN(numSize);
		}
		break;
	case NUMT_PEEP:
		{
			pNumField= new CNumericFieldPEEP(numSize);
		}
		break;
	case NUMT_PSVTIME:
		{
			pNumField= new CNumericFieldPSVTIME(numSize);
		}
		break;
	case NUMT_COMPLIANCE:
		{
			pNumField= new CNumericFieldCOMPLIANCE(numSize);
		}
		break;
	case NUMT_C20C:
		{
			pNumField= new CNumericFieldC20C(numSize);
		}
		break;
	case NUMT_RESISTANCE:
		{
			pNumField= new CNumericFieldRESISTANCE(numSize);
		}
		break;
	case NUMT_MVEXP:
		{
			pNumField= new CNumericFieldMVEXP(numSize);
		}
		break;
	case NUMT_SHAREMV:
		{
			pNumField= new CNumericFieldSHAREMV(numSize);
		}
		break;
	case NUMT_VTE:
		{
			pNumField= new CNumericFieldVTE(numSize);
		}
		break;
	case NUMT_VTEMAND:
		{
			pNumField= new CNumericFieldVTEMAND(numSize);
		}
		break;
	case NUMT_VTESPONT:
		{
			pNumField= new CNumericFieldVTESPONT(numSize);
		}
		break;
	case NUMT_LEAK:
		{
			pNumField= new CNumericFieldLEAK(numSize);
		}
		break;
	case NUMT_FREQ:
		{
			pNumField= new CNumericFieldFREQ(numSize);
		}
		break;
	case NUMT_TRIGGER:
		{
			pNumField= new CNumericFieldTRIGGER(numSize);
		}
		break;
	case NUMT_VTI:
		{
			pNumField= new CNumericFieldVTI(numSize);
		}
		break;
	case NUMT_HFOAMP:
		{
			pNumField= new CNumericFieldHFOAMP(numSize);
		}
		break;
	case NUMT_HFOVTE:
		{
			pNumField= new CNumericFieldHFOVTE(numSize);
		}
		break;
	case NUMT_DCO2:
		{
			pNumField= new CNumericFieldDCO2(numSize);
		}
		break;
	case NUMT_HFOFREQ:
		{
			pNumField= new CNumericFieldHFOFREQ(numSize);
		}
		break;
	case NUMT_OXY:
		{
			pNumField= new CNumericFieldOXY(numSize);
		}
		break;
	case NUMT_ETCO2:
		{
			pNumField= new CNumericFieldETCO2(numSize);
		}
		break;
	case NUMT_DEMANDFLOW:
		{
			pNumField= new CNumericFieldDEMANDFLOW(numSize);
		}
		break;
	case NUMT_IFLOW:
		{
			pNumField= new CNumericFieldIFLOW(numSize);
		}
		break;
	case NUMT_EFLOW:
		{
			pNumField= new CNumericFieldEFLOW(numSize);
		}
		break;
	case NUMT_TAU:
		{
			pNumField= new CNumericFieldTAU(numSize);
		}
		break;
	case NUMT_FREQETCO2:
		{
			pNumField= new CNumericFieldFREQETCO2(numSize);
		}
		break;
	case NUMT_SPO2:
		{
			pNumField= new CNumericFieldSPO2(numSize);
		}
		break;
	case NUMT_SPO2_PI:
		{
			pNumField= new CNumericFieldSPO2PI(numSize);
		}
		break;
	case NUMT_SPO2_PR:
		{
			pNumField= new CNumericFieldSPO2PULSERATE(numSize);
		}
		break;
	case NUMT_RSBI:
		{
			pNumField= new CNumericFieldRSBI(numSize);
		}
		break;
	case NUMT_MECHANIC:
		{
			pNumField= new CNumericFieldMECHANIC(numSize);
		}
		break;
	case NUMT_VTE2:
		{
			pNumField= new CNumericFieldVTEMANDSPONT(numSize);
		}
		break;
	case NUMT_FICO2:
		{
			pNumField= new CNumericFieldFICO2(numSize);
		}
		break;
	case NUMT_VTEKG:
		{
			pNumField= new CNumericFieldVTEKG(numSize);
		}
		break;
	case NUMT_INFO_PATAL:
		{
			pNumField= new CNumericFieldINFOPATAL(numSize);
		}
		break;
	case NUMT_INFO_TUBE:
		{
			pNumField= new CNumericFieldINFOTUBE(numSize);
		}
		break;
	case NUMT_SPACE:
		{
			pNumField= new CNumericFieldSPACE(numSize);
		}
		break;
	case NUMT_INFO_FLOWOFF:
		{
			pNumField= new CNumericFieldINFOFLOWOFF(numSize);
		}
		break;
	}
	LeaveCriticalSection(&csDraw);

	return pNumField;
}

/**********************************************************************************************//**
 * Creates numeric field 1
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndNumConfig::createNumericField1()
{
	bool bCursorField=false;
	CNumericField* pNumField=NULL;
	eNumericType numType=m_pbufNumeric->VAL1;
	eNumericSize numSize=m_pbufNumeric->SIZE1;
	UINT numBlockID=IDC_VIEW_NUM_FIELD1;
	RECT rcLd = {0, 0, 0, 0};
	rcLd.left=0;
	rcLd.right=190;
	rcLd.top=m_iCurrentNumPositionYaxes;	

	if(numSize!=NUMERICSIZE_0)
	{
		m_iSizeComplete+=(BYTE)numSize;

		rcLd.bottom=getBottomNumPositionYaxes(numSize);
	}

	if(rcLd.top>=191 && rcLd.top<434)
	{
		bCursorField=true;
	}
	else if(rcLd.bottom>=191 && rcLd.bottom<434)
	{
		bCursorField=true;
	}

	pNumField=createNewNumericField(numType, numSize);
	EnterCriticalSection(&csDraw);
	if(pNumField!=NULL)
	{
		if(pNumField->Create(this,rcLd,numBlockID))
		{
			m_pNumFieldArray.Add(pNumField);
		}
		pNumField->setCursorField(bCursorField);
	}
	LeaveCriticalSection(&csDraw);
	m_iCurrentNumPositionYaxes=(SHORT)rcLd.bottom;
}

/**********************************************************************************************//**
 * Creates numeric field 2
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndNumConfig::createNumericField2()
{
	bool bCursorField=false;
	CNumericField* pNumField=NULL;
	eNumericType numType=m_pbufNumeric->VAL2;
	eNumericSize numSize=m_pbufNumeric->SIZE2;
	UINT numBlockID=IDC_VIEW_NUM_FIELD2;
	RECT rcLd = {0, 0, 0, 0};
	rcLd.left=0;
	rcLd.right=190;
	rcLd.top=m_iCurrentNumPositionYaxes;	

	if(numSize!=NUMERICSIZE_0)
	{
		m_iSizeComplete+=(BYTE)numSize;

		rcLd.bottom=getBottomNumPositionYaxes(numSize);
	}

	if(rcLd.top>=191 && rcLd.top<434)
	{
		bCursorField=true;
	}
	else if(rcLd.bottom>=191 && rcLd.bottom<434)
	{
		bCursorField=true;
	}

	pNumField=createNewNumericField(numType, numSize);
	EnterCriticalSection(&csDraw);
	if(pNumField!=NULL)
	{
		if(pNumField->Create(this,rcLd,numBlockID))
		{
			m_pNumFieldArray.Add(pNumField);
		}
		pNumField->setCursorField(bCursorField);
	}
	LeaveCriticalSection(&csDraw);
	m_iCurrentNumPositionYaxes=(SHORT)rcLd.bottom;
}

/**********************************************************************************************//**
 * Creates numeric field 3
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndNumConfig::createNumericField3()
{
	bool bCursorField=false;
	CNumericField* pNumField=NULL;
	eNumericType numType=m_pbufNumeric->VAL3;
	eNumericSize numSize=m_pbufNumeric->SIZE3;
	UINT numBlockID=IDC_VIEW_NUM_FIELD3;
	RECT rcLd = {0, 0, 0, 0};
	rcLd.left=0;
	rcLd.right=190;
	rcLd.top=m_iCurrentNumPositionYaxes;	

	if(numSize!=NUMERICSIZE_0)
	{
		m_iSizeComplete+=(BYTE)numSize;

		rcLd.bottom=getBottomNumPositionYaxes(numSize);
	}

	if(rcLd.top>=191 && rcLd.top<434)
	{
		bCursorField=true;
	}
	else if(rcLd.bottom>=191 && rcLd.bottom<434)
	{
		bCursorField=true;
	}

	pNumField=createNewNumericField(numType, numSize);
	EnterCriticalSection(&csDraw);
	if(pNumField!=NULL)
	{
		if(pNumField->Create(this,rcLd,numBlockID))
		{
			m_pNumFieldArray.Add(pNumField);
		}
		pNumField->setCursorField(bCursorField);
	}
	LeaveCriticalSection(&csDraw);
	m_iCurrentNumPositionYaxes=(SHORT)rcLd.bottom;
}

/**********************************************************************************************//**
 * Creates numeric field 4
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndNumConfig::createNumericField4()
{
	bool bCursorField=false;
	CNumericField* pNumField=NULL;
	eNumericType numType=m_pbufNumeric->VAL4;
	eNumericSize numSize=m_pbufNumeric->SIZE4;
	UINT numBlockID=IDC_VIEW_NUM_FIELD4;
	RECT rcLd = {0, 0, 0, 0};
	rcLd.left=0;
	rcLd.right=190;
	rcLd.top=m_iCurrentNumPositionYaxes;	

	if(numSize!=NUMERICSIZE_0)
	{
		m_iSizeComplete+=(BYTE)numSize;

		rcLd.bottom=getBottomNumPositionYaxes(numSize);
	}

	if(rcLd.top>=191 && rcLd.top<434)
	{
		bCursorField=true;
	}
	else if(rcLd.bottom>=191 && rcLd.bottom<434)
	{
		bCursorField=true;
	}

	pNumField=createNewNumericField(numType, numSize);
	EnterCriticalSection(&csDraw);
	if(pNumField!=NULL)
	{
		if(pNumField->Create(this,rcLd,numBlockID))
		{
			m_pNumFieldArray.Add(pNumField);
		}
		pNumField->setCursorField(bCursorField);
	}
	LeaveCriticalSection(&csDraw);
	m_iCurrentNumPositionYaxes=(SHORT)rcLd.bottom;
}

/**********************************************************************************************//**
 * Creates numeric field 5
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndNumConfig::createNumericField5()
{
	bool bCursorField=false;
	CNumericField* pNumField=NULL;
	eNumericType numType=m_pbufNumeric->VAL5;
	eNumericSize numSize=m_pbufNumeric->SIZE5;
	UINT numBlockID=IDC_VIEW_NUM_FIELD5;
	RECT rcLd = {0, 0, 0, 0};
	rcLd.left=0;
	rcLd.right=190;
	rcLd.top=m_iCurrentNumPositionYaxes;	

	if(numSize!=NUMERICSIZE_0)
	{
		m_iSizeComplete+=(BYTE)numSize;

		rcLd.bottom=getBottomNumPositionYaxes(numSize);
	}

	if(rcLd.top>=191 && rcLd.top<434)
	{
		bCursorField=true;
	}
	else if(rcLd.bottom>=191 && rcLd.bottom<434)
	{
		bCursorField=true;
	}

	pNumField=createNewNumericField(numType, numSize);
	EnterCriticalSection(&csDraw);
	if(pNumField!=NULL)
	{
		if(pNumField->Create(this,rcLd,numBlockID))
		{
			m_pNumFieldArray.Add(pNumField);
		}
		pNumField->setCursorField(bCursorField);
	}
	LeaveCriticalSection(&csDraw);
	m_iCurrentNumPositionYaxes=(SHORT)rcLd.bottom;
}

/**********************************************************************************************//**
 * Creates numeric field 6
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndNumConfig::createNumericField6()
{
	bool bCursorField=false;
	CNumericField* pNumField=NULL;
	eNumericType numType=m_pbufNumeric->VAL6;
	eNumericSize numSize=m_pbufNumeric->SIZE6;
	UINT numBlockID=IDC_VIEW_NUM_FIELD6;
	RECT rcLd = {0, 0, 0, 0};
	rcLd.left=0;
	rcLd.right=190;
	rcLd.top=m_iCurrentNumPositionYaxes;	

	if(numSize!=NUMERICSIZE_0)
	{
		m_iSizeComplete+=(BYTE)numSize;

		rcLd.bottom=getBottomNumPositionYaxes(numSize);
	}

	if(rcLd.top>=191 && rcLd.top<434)
	{
		bCursorField=true;
	}
	else if(rcLd.bottom>=191 && rcLd.bottom<434)
	{
		bCursorField=true;
	}

	pNumField=createNewNumericField(numType, numSize);
	EnterCriticalSection(&csDraw);
	if(pNumField!=NULL)
	{
		if(pNumField->Create(this,rcLd,numBlockID))
		{
			m_pNumFieldArray.Add(pNumField);
		}
		pNumField->setCursorField(bCursorField);
	}
	LeaveCriticalSection(&csDraw);
	m_iCurrentNumPositionYaxes=(SHORT)rcLd.bottom;
}

/**********************************************************************************************//**
 * Creates numeric field 7
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndNumConfig::createNumericField7()
{
	bool bCursorField=false;
	CNumericField* pNumField=NULL;
	eNumericType numType=m_pbufNumeric->VAL7;
	eNumericSize numSize=m_pbufNumeric->SIZE7;
	UINT numBlockID=IDC_VIEW_NUM_FIELD7;
	RECT rcLd = {0, 0, 0, 0};
	rcLd.left=0;
	rcLd.right=190;
	rcLd.top=m_iCurrentNumPositionYaxes;	

	if(numSize!=NUMERICSIZE_0)
	{
		m_iSizeComplete+=(BYTE)numSize;

		rcLd.bottom=getBottomNumPositionYaxes(numSize);
	}

	if(rcLd.top>=191 && rcLd.top<434)
	{
		bCursorField=true;
	}
	else if(rcLd.bottom>=191 && rcLd.bottom<434)
	{
		bCursorField=true;
	}

	pNumField=createNewNumericField(numType, numSize);
	EnterCriticalSection(&csDraw);
	if(pNumField!=NULL)
	{
		if(pNumField->Create(this,rcLd,numBlockID))
		{
			m_pNumFieldArray.Add(pNumField);
		}
		pNumField->setCursorField(bCursorField);
	}
	LeaveCriticalSection(&csDraw);
	m_iCurrentNumPositionYaxes=(SHORT)rcLd.bottom;
}

/**********************************************************************************************//**
 * Creates numeric field 8
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndNumConfig::createNumericField8()
{
	bool bCursorField=false;
	CNumericField* pNumField=NULL;
	eNumericType numType=m_pbufNumeric->VAL8;
	eNumericSize numSize=m_pbufNumeric->SIZE8;
	UINT numBlockID=IDC_VIEW_NUM_FIELD8;
    RECT rcLd = {0, 0, 0, 0};
	rcLd.left=0;
	rcLd.right=190;
	rcLd.top=m_iCurrentNumPositionYaxes;	

	if(numSize!=NUMERICSIZE_0)
	{
		m_iSizeComplete+=(BYTE)numSize;

		rcLd.bottom=getBottomNumPositionYaxes(numSize);
	}

	if(rcLd.top>=191 && rcLd.top<434)
	{
		bCursorField=true;
	}
	else if(rcLd.bottom>=191 && rcLd.bottom<434)
	{
		bCursorField=true;
	}

	pNumField=createNewNumericField(numType, numSize);
	EnterCriticalSection(&csDraw);
	if(pNumField!=NULL)
	{
		if(pNumField->Create(this,rcLd,numBlockID))
		{
			m_pNumFieldArray.Add(pNumField);
		}
		pNumField->setCursorField(bCursorField);
	}
	LeaveCriticalSection(&csDraw);
	m_iCurrentNumPositionYaxes=(SHORT)rcLd.bottom;
}

/**********************************************************************************************//**
 * Sets graph cursor
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	bDraw	   	True to draw.
 * \param	bMoveXscale	True to move xscale.
 * \param	bMoveYscale	True to move yscale.
 **************************************************************************************************/

void CWndNumConfig::SetGraphCursor(bool bDraw, bool bMoveXscale, bool bMoveYscale)
{
	bool bRedraw=false;
	if(m_bGraphCursor!=bDraw)
		bRedraw=true;
	else if(m_bMoveXscale!=bMoveXscale)
		bRedraw=true;
	else if(m_bMoveYscale!=bMoveYscale)
		bRedraw=true;

	m_bGraphCursor=bDraw;
	m_bMoveXscale=bMoveXscale;
	m_bMoveYscale=bMoveYscale;

	if(bRedraw)
	{
		drawCursor();
		DrawData(true,true, true, true,true);
		
	}
}

/**********************************************************************************************//**
 * Draw data
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	bData	  	True to data.
 * \param	bFrames   	True to frames.
 * \param	bText	  	True to text.
 * \param	bLimits   	True to limits.
 * \param	bFlowmeter	True to flowmeter.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CWndNumConfig::DrawData(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
{
	if(m_bExit)
		return false;

	EnterCriticalSection(&csDraw);
	bool bReturn = true;

	CNumericField* pNumField=NULL;

	
	if(0==m_pNumFieldArray.IsEmpty())
	{
		for (int iIndex=0;iIndex < m_pNumFieldArray.GetCount();iIndex++)
		{		
			pNumField = m_pNumFieldArray.ElementAt(iIndex);

			if(m_bGraphCursor && pNumField->isCursorField())
			{
				pNumField->ShowWindow(SW_HIDE);
			}
			else
			{
				pNumField->ShowWindow(SW_SHOW);
				pNumField->drawData(bData, bFrames, bText, bLimits, bFlowmeter);
			}
		}
	}

	pNumField = NULL;

	LeaveCriticalSection(&csDraw);

	return bReturn;
}

/**********************************************************************************************//**
 * Draw frames
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CWndNumConfig::DrawFrames(CDC* pDC)
{
	return true;
}

/**********************************************************************************************//**
 * Determines if we can draw cursor
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CWndNumConfig::drawCursor()
{
	if(m_bExit)
		return false;

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	HBRUSH hPrevBrush = (HBRUSH)SelectObject(m_hdcStatic, (HBRUSH)GetStockObject(WHITE_BRUSH));	
	HPEN hPrevPen = (HPEN)SelectObject(m_hdcStatic, (HPEN)GetStockObject(WHITE_PEN));

	RECT rc;

	if(false==m_bGraphCursor)
	{
		if(m_pbtnCursorUp)
			m_pbtnCursorUp->ShowWindow(SW_HIDE);
		if(m_pbtnCursorDown)
			m_pbtnCursorDown->ShowWindow(SW_HIDE);
		if(m_pbtnCursorLeft)
			m_pbtnCursorLeft->ShowWindow(SW_HIDE);
		if(m_pbtnCursorRight)
			m_pbtnCursorRight->ShowWindow(SW_HIDE);
		if(m_pbtnXmoveUp)
			m_pbtnXmoveUp->ShowWindow(SW_HIDE);
		if(m_pbtnXmoveDown)
			m_pbtnXmoveDown->ShowWindow(SW_HIDE);
		if(m_pbtnYmoveLeft)
			m_pbtnYmoveLeft->ShowWindow(SW_HIDE);
		if(m_pbtnYmoveRight)
			m_pbtnYmoveRight->ShowWindow(SW_HIDE);

		rc.left = 0;  
		rc.top = 191;  
		rc.right  = 190;  
		rc.bottom = 446;

		CBrush br(BACKGND);
		FillRect(m_hdcStatic,&rc,br);
	
	}
	else
	{
		if(m_pcNumInfoTop)
			m_pcNumInfoTop->Draw(m_hdcStatic,0,191);
		if(m_pcNumInfoBot)
			m_pcNumInfoBot->Draw(m_hdcStatic,0,419);

		rc.left = 0;  
		rc.top = 208;  
		rc.right  = 190;  
		rc.bottom = 419;

		CBrush br(RGB(176,176,177));
		FillRect(m_hdcStatic,&rc,br);

		if(!m_bMoveYscale && !m_bMoveXscale)
		{
			if(m_pbtnCursorUp)
				m_pbtnCursorUp->MoveWindow(70,217,51,68);
			if(m_pbtnCursorDown)
				m_pbtnCursorDown->MoveWindow(70,338,51,68);
			if(m_pbtnCursorLeft)
				m_pbtnCursorLeft->MoveWindow(2,286,68,51);
			if(m_pbtnCursorRight)
				m_pbtnCursorRight->MoveWindow(121,286,68,51);

			if(m_pcGraphCursor_pic)
				m_pcGraphCursor_pic->Draw(m_hdcStatic,74,289);
		}
		else
		{
			if(m_pbtnCursorUp)
				m_pbtnCursorUp->MoveWindow(70,195,51,68);
			if(m_pbtnCursorDown)
				m_pbtnCursorDown->MoveWindow(70,316,51,68);
			if(m_pbtnCursorLeft)
				m_pbtnCursorLeft->MoveWindow(2,264,68,51);
			if(m_pbtnCursorRight)
				m_pbtnCursorRight->MoveWindow(121,264,68,51);

			if(m_pcGraphCursor_pic)
				m_pcGraphCursor_pic->Draw(m_hdcStatic,74,267);
		}
		

		if(m_pbtnCursorUp)
			m_pbtnCursorUp->ShowWindow(SW_SHOW);
		if(m_pbtnCursorDown)
			m_pbtnCursorDown->ShowWindow(SW_SHOW);
		if(m_pbtnCursorLeft)
			m_pbtnCursorLeft->ShowWindow(SW_SHOW);
		if(m_pbtnCursorRight)
			m_pbtnCursorRight->ShowWindow(SW_SHOW);
		

		if(m_bMoveXscale)
		{
			SelectObject(m_hdcStatic, (HPEN)GetStockObject(BLACK_PEN));
			MoveToEx(m_hdcStatic, 5, 388, NULL);
			LineTo(m_hdcStatic, 195, 388);

			if(m_pbtnYmoveLeft)
				m_pbtnYmoveLeft->ShowWindow(SW_HIDE);
			if(m_pbtnYmoveRight)
				m_pbtnYmoveRight->ShowWindow(SW_HIDE);
			if(m_pbtnXmoveUp)
				m_pbtnXmoveUp->ShowWindow(SW_SHOW);
			if(m_pbtnXmoveDown)
				m_pbtnXmoveDown->ShowWindow(SW_SHOW);
			if(m_pcMoveXscale_pic)
				m_pcMoveXscale_pic->Draw(m_hdcStatic,80,400);
		}
		else if(m_bMoveYscale)
		{
			SelectObject(m_hdcStatic, (HPEN)GetStockObject(BLACK_PEN));
			MoveToEx(m_hdcStatic, 5, 388, NULL);
			LineTo(m_hdcStatic, 195, 388);

			if(m_pbtnXmoveUp)
				m_pbtnXmoveUp->ShowWindow(SW_HIDE);
			if(m_pbtnXmoveDown)
				m_pbtnXmoveDown->ShowWindow(SW_HIDE);
			if(m_pbtnYmoveLeft)
				m_pbtnYmoveLeft->ShowWindow(SW_SHOW);
			if(m_pbtnYmoveRight)
				m_pbtnYmoveRight->ShowWindow(SW_SHOW);
			if(m_pcMoveYscale_pic)
				m_pcMoveYscale_pic->Draw(m_hdcStatic,80,400);
		}
		else
		{
			if(m_pbtnXmoveUp)
				m_pbtnXmoveUp->ShowWindow(SW_HIDE);
			if(m_pbtnXmoveDown)
				m_pbtnXmoveDown->ShowWindow(SW_HIDE);
			if(m_pbtnYmoveLeft)
				m_pbtnYmoveLeft->ShowWindow(SW_HIDE);
			if(m_pbtnYmoveRight)
				m_pbtnYmoveRight->ShowWindow(SW_HIDE);
		}
	}
	
	BitBlt(hdcMem, 0, 0, m_lX, m_lY,m_hdcStatic , 0, 0, SRCCOPY);
	BitBlt(m_hDC, 0, 0, m_lX, m_lY,hdcMem , 0, 0, SRCCOPY);
	BitBlt(dc.m_hDC, 0, 0, m_lX, m_lY, hdcMem, 0, 0, SRCCOPY);

	SelectObject(m_hdcStatic, hPrevBrush);	
	SelectObject(m_hdcStatic, hPrevPen);

	if(hBmpMem)
	{
		SelectObject(hdcMem,hBmpMemPrev);
		DeleteObject(hBmpMem);
	}
	if(hdcMem)
		DeleteDC(hdcMem);
	return true;
}

/**********************************************************************************************//**
 * Draw static text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CWndNumConfig::DrawStaticText(CDC* pDC)
{
	return true;
}

/**********************************************************************************************//**
 * Draw limits
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CWndNumConfig::DrawLimits(CDC* pDC)
{
	return true;
}


