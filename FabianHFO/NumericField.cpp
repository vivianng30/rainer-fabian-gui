// NumericField.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "NumericField.h"
#include "WuLine.h"

// CNumericField

IMPLEMENT_DYNAMIC(CNumericField, CWnd)

CNumericField::CNumericField(eNumericSize size)
{
	InitializeCriticalSection(&csDraw);

	m_eSize=size;

	m_pModel=NULL;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;

	m_hdcStatic=NULL;
	m_hbmpStatic=NULL;

	m_pcNumPara1er=NULL;
	m_pcNumPara2er=NULL;
	m_pcNumPara3er=NULL;

	m_pcLimitHigh_Up=NULL;
	m_pcLimitLow_Up=NULL;

	m_pcTauSize1=NULL;
	//m_pcTauSize2=NULL;

	m_pcWarningYellow=NULL;

	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;

	m_bCursorField=false;
	//m_pnbFieldData=NULL;

	//eType=eType;
	//bHiAlarmLimit=bShowHiAlarmLimit;
	//bLoAlarmLimit=bShowLoAlarmLimit;
	//eSize=NUMERICSIZE_0;
	//numName[0]=0x0000;
	//numNameNote[0]=0x0000;
	//numUnit[0]=0x0000;
	//iNumValue=0;//
	//bHiAlarmLimit=false;
	//enumStateHiAlarmLimit=AL_OFF;
	//enumStateLoAlarmLimit=AL_OFF;
	//iLoAlarmLimit=0;
}

CNumericField::~CNumericField()
{
	DeleteCriticalSection(&csDraw);
}


BEGIN_MESSAGE_MAP(CNumericField, CWnd)
	//{{AFX_MSG_MAP(CNumericField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CNumericField message handlers

// **************************************************************************
// 
// **************************************************************************
CMVModel *CNumericField::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CNumericField::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD|WS_VISIBLE,rc,pParentWnd,nID))
	{
		CClientDC dc(this);

		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		CBrush cbrBack(BACKGND);
		//CBrush cbrBack(RGB(140,140,255));
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);


		m_hdcStatic = CreateCompatibleDC(m_hDC);
		m_hbmpStatic = CreateCompatibleBitmap(m_hDC, m_lX, m_lY);
		SelectObject(m_hdcStatic, m_hbmpStatic);

		HPEN hpenprevStatic=(HPEN)SelectObject(m_hdcStatic, (HPEN)GetStockObject(NULL_PEN));	
		HBRUSH hbrprevStatic=(HBRUSH)SelectObject(m_hdcStatic,cbrBack); 
		Rectangle(m_hdcStatic, 0, 0, m_lX, m_lY);

		
		if(m_eSize==NUMERICSIZE_1)
			m_pcNumPara1er		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUMPARA_1);
		else if(m_eSize==NUMERICSIZE_2)
			m_pcNumPara2er		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUMPARA_2);
		else 
			m_pcNumPara3er		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUMPARA_3);
		
		m_pcLimitHigh_Up	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_LIMIT_HIGH);
		m_pcLimitLow_Up		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_LIMIT_LOW);

		m_pcTauSize1	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUM_TAU1);
		//m_pcTauSize2	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUM_TAU2);

		m_pcWarningYellow = new CBmp(theApp.m_hInstance,m_hDC,IDB_PARA_WARN_SUB);

		SelectObject(m_hdcStatic, hpenprevStatic);	
		SelectObject(m_hdcStatic,hbrprevStatic);
		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();//rkuNEWFIX

		return 1;
	}
	else
		return 0;
}

// **************************************************************************
// 
// **************************************************************************
void CNumericField::setCursorField(bool state)
{
	m_bCursorField=state;
}
bool CNumericField::isCursorField()
{
	return m_bCursorField;
}

// **************************************************************************
// 
// **************************************************************************
//void CNumericField::setFieldData(LPNUMERICBLOCK pnbData)
//{
//	m_nbFieldData.eType=pnbData->eType;
//	m_nbFieldData.eSize=pnbData->eSize;
//	_tcscpy_s(m_nbFieldData.numName,_countof(m_nbFieldData.numName),pnbData->numName);
//	_tcscpy_s(m_nbFieldData.numNameNote,_countof(m_nbFieldData.numNameNote),pnbData->numNameNote);
//	_tcscpy_s(m_nbFieldData.numUnit,_countof(m_nbFieldData.numUnit),pnbData->numUnit);
//	m_nbFieldData.iNumValue=pnbData->iNumValue;//
//	m_nbFieldData.bHiAlarmLimit=pnbData->bHiAlarmLimit;
//	m_nbFieldData.enumStateHiAlarmLimit=pnbData->enumStateHiAlarmLimit;
//	m_nbFieldData.iHiAlarmLimit=pnbData->iHiAlarmLimit;
//	m_nbFieldData.bLoAlarmLimit=pnbData->bLoAlarmLimit;
//	m_nbFieldData.enumStateHiAlarmLimit=pnbData->enumStateHiAlarmLimit;
//	m_nbFieldData.iLoAlarmLimit=pnbData->iLoAlarmLimit;
//
//
//}
//void CNumericField::setFieldType(eNumericType eType, eNumericSize eSize)
//{
//	m_nbFieldData.eType=eType;
//	m_nbFieldData.eSize=eSize;
//
//	//getModel()->getDATAHANDLER()->fillNumericField(m_nbFieldData.eType, &m_nbFieldData.eType);
//	//
//}

// **************************************************************************
// 
// **************************************************************************
//void CNumericField::setLimits(bool	bShowHiAlarmLimit, eAlarmLimitState	enumStateHiLimit,int iHiLimitValue,bool	bShowLoAlarmLimit,eAlarmLimitState enumStateLoLimit,int iLoLimitValue, bool bRedraw)
//{
//	m_nbFieldData.bHiAlarmLimit=bShowHiAlarmLimit;
//	m_nbFieldData.enumStateHiAlarmLimit=enumStateHiLimit;
//	m_nbFieldData.iHiAlarmLimit=iHiLimitValue;
//	m_nbFieldData.bLoAlarmLimit=bShowLoAlarmLimit;
//	m_nbFieldData.enumStateLoAlarmLimit=enumStateLoLimit;
//	m_nbFieldData.iLoAlarmLimit=iLoLimitValue;
//
//	if(bRedraw)
//	{
//		drawData(true,false, false, true,true);
//	}
//}

// **************************************************************************
// 
// **************************************************************************
//void CNumericField::setLimitsValue(eAlarmLimitState enumStateHiLimit,int iHiLimitValue,eAlarmLimitState enumStateLoLimit,int iLoLimitValue, bool bRedraw)
//{
//	m_pnbFieldData->enumStateHiAlarmLimit=enumStateHiLimit;
//	m_pnbFieldData->iHiAlarmLimit=iHiLimitValue;
//	m_pnbFieldData->enumStateLoAlarmLimit=enumStateLoLimit;
//	m_pnbFieldData->iLoAlarmLimit=iLoLimitValue;
//
//	if(bRedraw)
//	{
//		drawData(true,false, false, true,true);
//	}
//}
// **************************************************************************
// 
// **************************************************************************
//void CNumericField::setNumericValue(INT iVal, bool bRedraw)
//{
//	m_pnbFieldData->iNumValue=iVal;
//
//	if(bRedraw)
//	{
//		drawData(true,false, false, false,true);
//	}
//}

// **************************************************************************
// 
// **************************************************************************
BOOL CNumericField::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		if(pMsg->wParam==VK_SPACE)
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETFOCUS_PIMARYVIEW);
			return 1;
		}
		break;
	default:
		{

		}
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

//************************************
// Method:    OnDestroy
// FullName:  CNumericField::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CNumericField::OnDestroy() 
{
	if(m_pcWarningYellow)
		delete m_pcWarningYellow;
	m_pcWarningYellow=NULL;

	if(m_pcNumPara1er)
		delete m_pcNumPara1er;
	m_pcNumPara1er=NULL;

	if(m_pcNumPara2er)
		delete m_pcNumPara2er;
	m_pcNumPara2er=NULL;

	if(m_pcNumPara3er)
		delete m_pcNumPara3er;
	m_pcNumPara3er=NULL;

	if(m_pcLimitHigh_Up)
		delete m_pcLimitHigh_Up;
	m_pcLimitHigh_Up=NULL;

	if(m_pcLimitLow_Up)
		delete m_pcLimitLow_Up;
	m_pcLimitLow_Up=NULL;

	if(m_pcTauSize1)
		delete m_pcTauSize1;
	m_pcTauSize1=NULL;

	/*if(m_pcTauSize2)
		delete m_pcTauSize2;
	m_pcTauSize2=NULL;*/

	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);

	if(m_hdcStatic)
		DeleteDC(m_hdcStatic);

	if(m_hbmpStatic)
		DeleteObject(m_hbmpStatic);

}
// **************************************************************************
// 
// **************************************************************************
//void CNumericField::deinit()
//{
//	DestroyWindow();
//}
// **************************************************************************
// 
// **************************************************************************
bool CNumericField::drawFrames(CDC* pDC)
{
	HDC hdc = *pDC;
	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));	
	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

	/*if(m_pcNumPara3er)
		m_pcNumPara3er->Draw(hdc,0,5);*/

	//RECT rc;

	switch(m_eSize)
	{
	case NUMERICSIZE_1:
		{
			if(m_pcNumPara1er)
				m_pcNumPara1er->Draw(hdc,0,5);
		}
		break;
	case NUMERICSIZE_2:
		{
			if(m_pcNumPara2er)
				m_pcNumPara2er->Draw(hdc,0,5);
		}
		break;
	case NUMERICSIZE_3:
		{
			if(m_pcNumPara3er)
				m_pcNumPara3er->Draw(hdc,0,5);
		}
		break;
	}
	

	SelectObject(hdc, hPrevBrush);	
	SelectObject(hdc, hPrevPen);

	return true;
}

void CNumericField::drawHighLimit(CDC* pDC)
{
	HDC hdc = *pDC;
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	CPen penLine;
	penLine.CreatePen(PS_SOLID,2,NUMERICLIMIT_COL);
	HPEN hpenprev=(HPEN)SelectObject(hdc,(HPEN)penLine);

	if(m_eSize==NUMERICSIZE_1)
	{
		if(m_pcLimitHigh_Up)
			m_pcLimitHigh_Up->Draw(hdc,120,11);
		/*DrawWuLine(pDC,123, 23,127, 23, NUMERICLIMIT_COL);
		DrawWuLine(pDC,127, 23,133, 14, NUMERICLIMIT_COL);
		DrawWuLine(pDC,133, 14, 145, 14, NUMERICLIMIT_COL);

		DrawWuLine(pDC,139, 15,133, 23, NUMERICLIMIT_COL);
		DrawWuLine(pDC,133, 23, 145, 23, NUMERICLIMIT_COL);
		DrawWuLine(pDC,145, 23, 139, 15, NUMERICLIMIT_COL);*/
	}
	//else //if(m_eSize==NUMERICSIZE_2)
	//{
	//	/*if(m_pcLimitHigh_Up)
	//		m_pcLimitHigh_Up->Draw(hdc,120,60);*/

	//	/*if(m_pcLimitLow_Up)
	//		m_pcLimitLow_Up->Draw(hdc,120,95);*/

	//	
	//}

	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hpenprev);

	penLine.DeleteObject();
}

void CNumericField::drawLowLimit(CDC* pDC)
{
	HDC hdc = *pDC;
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	CPen penLine;
	penLine.CreatePen(PS_SOLID,2,NUMERICLIMIT_COL);
	HPEN hpenprev=(HPEN)SelectObject(hdc,(HPEN)penLine);

	if(m_eSize==NUMERICSIZE_1)
	{
		if(m_pcLimitLow_Up)
			m_pcLimitLow_Up->Draw(hdc,120,44);
		/*DrawWuLine(pDC,123,56,134,56, NUMERICLIMIT_COL);
		DrawWuLine(pDC,134,56,140,47, NUMERICLIMIT_COL);
		DrawWuLine(pDC,140,47,145,47, NUMERICLIMIT_COL);

		DrawWuLine(pDC,129, 55,134,47, NUMERICLIMIT_COL);
		DrawWuLine(pDC,134, 47,123,47, NUMERICLIMIT_COL);
		DrawWuLine(pDC,123, 47,129,55, NUMERICLIMIT_COL);*/

	}
	//else //if(m_eSize==NUMERICSIZE_2)
	//{
	//}

	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hpenprev);

	penLine.DeleteObject();
}
