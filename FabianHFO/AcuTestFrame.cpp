#include "StdAfx.h"
#include "AcuTestFrame.h"
#include "MVModel.h"
#include "MVViewHandler.h"

CAcuTestFrame* CAcuTestFrame::theAcuTestFrame=0;

CAcuTestFrame::CAcuTestFrame(void)
{
	//m_pModel=CMVModel::GetInstance();
	//m_szSerialData=_T("");
	//m_blSerialData=_T("");
	//m_pModel=CMVModel::GetInstance();

	/*m_pcwtTestTimerThread=NULL;
	m_bDoTestTimerFunctionsThread=false;
	m_hThreadTestTimerFunctions=INVALID_HANDLE_VALUE;*/
	m_iWaitForComand=0;
}

CAcuTestFrame::~CAcuTestFrame(void)
{
	/*if(m_pcwtTestTimerThread!=NULL)
	{
		delete m_pcwtTestTimerThread;
		m_pcwtTestTimerThread=NULL;

		if(m_hThreadTestTimerFunctions!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadTestTimerFunctions);
			m_hThreadTestTimerFunctions=INVALID_HANDLE_VALUE;
		}

	}*/
}

// **************************************************************************
// 
// **************************************************************************
void CAcuTestFrame::DestroyInstance()
{
	delete theAcuTestFrame;
	theAcuTestFrame = NULL;
}



// **************************************************************************
// 
// **************************************************************************
CAcuTestFrame* CAcuTestFrame::GetInstance()
{
	if(theAcuTestFrame == 0)
	{
		theAcuTestFrame = new CAcuTestFrame;
	}
	return theAcuTestFrame;
}
CMVModel *CAcuTestFrame::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

void CAcuTestFrame::startTest()
{
	testParaBtn();
}

void CAcuTestFrame::testParaBtn()
{
	DWORD WAIT=500;
	DWORD dwWait=0;
	DWORD dwDiff = 0;
	DWORD dwStart = GetTickCount();
	DWORD dwEnd=0;

#ifdef TEST_FRAMEWORK
	CMVViewHandler *pViewHandler=CMVViewHandler::GetInstance();

	CList<LPPARAVALUE, LPPARAVALUE>  pParaValue; 
	LPPARAVALUE para_IFLOW = new PARAVALUE;
	para_IFLOW->iParaBtnID=IDC_BTN_PARA_IFLOW;
	if(getModel()->GetDataHandler()->activeModeIsIPPV())
	{
		para_IFLOW->iLowerLimit=getModel()->GetDataHandler()->PARADATA()->GetIFlowMinPara_IPPV();
		para_IFLOW->iUpperLimit=getModel()->GetDataHandler()->PARADATA()->GetIFlowMaxPara_IPPV();
	}
	else
	{
		para_IFLOW->iLowerLimit=getModel()->GetDataHandler()->PARADATA()->GetIFlowMinPara_TRIGGER();
		para_IFLOW->iUpperLimit=getModel()->GetDataHandler()->PARADATA()->GetIFlowMaxPara_TRIGGER();
	}
	para_IFLOW->iStep=100;
	pParaValue.AddTail(para_IFLOW);

	POSITION pos = pParaValue.GetHeadPosition();
	while(pos!=NULL)
	{
		LPPARAVALUE pTemp = pParaValue.GetAt(pos);

		//todo
		int iValue=pTemp->iLowerLimit;
		int iHigh=pTemp->iUpperLimit;
		for(iValue;iValue<=iHigh;)
		{
			if(pViewHandler->m_vParaBtn)
			{
				m_iWaitForComand=pTemp->iParaBtnID;
				HWND hWnd2=pViewHandler->m_vParaBtn->GetSafeHwnd();
				CParaBtn* pWnd = (CParaBtn*)CWnd::FromHandle(GetDlgItem(hWnd2,pTemp->iParaBtnID));
				pWnd->SetParaValue(iValue);
				pWnd->WriteCurrentValue();


				int iStop=0;
				dwEnd=GetTickCount();
		
				if(dwEnd>=dwStart)
					dwDiff=dwEnd-dwStart;
				else
					dwDiff=MAXDWORD-dwStart+dwEnd;
		
				if(dwDiff<WAIT)
				{
					dwWait=WAIT-dwDiff;
				}
				else
				{
					dwWait=1;
				}
		
				DWORD dw = ::WaitForSingleObject(eventDoTestTimerFunctions, dwWait);
		
				switch(dw)
				{
				case WAIT_OBJECT_0:
					{
						checkSerialData(pTemp->iParaBtnID,iValue);
					}
					break;
				default:
					{
						int iStop=0;
						//checkSerialData(pTemp->iParaBtnID,iValue);
					}
					break;
				}

				Sleep(100);
		
				dwStart = GetTickCount();
			}

			iValue+=pTemp->iStep;
		}

		pParaValue.RemoveAt(pos);
		delete (void *)pTemp;

		pos = pParaValue.GetHeadPosition();
	}
	
	
	if(pParaValue.GetCount()>0)
	{
		POSITION pos = pParaValue.GetHeadPosition();
		if(pos==NULL)
		{

		}
		else
		{
			LPPARAVALUE pTemp = pParaValue.GetAt(pos);
			pParaValue.RemoveAt(pos);
			delete (void *)pTemp;
		}
	}
#endif
}

bool CAcuTestFrame::checkSerialData(int iBtnID, int iVal)
{
	bool bResult = false;
	unsigned char* psz = m_blSerialData.GetBuffer();

	switch(m_iWaitForComand)
	{
	case IDC_BTN_PARA_IFLOW:
		{
			if(psz[0] == 'u')
			{
				CStringW sToken = m_blSerialData.GetString();
				sToken = sToken.Right(sToken.GetLength()-1);
				sToken = CTlsString::RemoveLeadingZeroes(sToken);

				int iTemp = _wtoi(sToken);
				if(iVal==iTemp)
				{
					m_iWaitForComand=0;

					//eventDoTestTimerFunctions.SetEvent();
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("ERROR TEST\r\n")));
				}
				
			}
		}
		break;
	default:
		{

		}
		break;
	}




	return bResult;
}

void CAcuTestFrame::serialData(CStringW szData)
{
	m_blSerialData=szData;

	unsigned char* psz = m_blSerialData.GetBuffer();

	switch(m_iWaitForComand)
	{
	case IDC_BTN_PARA_IFLOW:
		{
			if(psz[0] == 'u')
			{
				/*CStringW sToken = m_blSerialData.GetString();
				sToken = sToken.Right(sToken.GetLength()-1);
				sToken = CTlsString::RemoveLeadingZeroes(sToken);

				int iTemp = _wtoi(sToken);
				int iStop=0;*/

				eventDoTestTimerFunctions.SetEvent();
			}
		}
		break;
	default:
		{

		}
		break;
	}

}

// **************************************************************************
// 
// **************************************************************************
//void CAcuTestFrame::StartTestTimerThread(void)
//{
//	m_bDoTestTimerFunctionsThread=true;
//
//	if(m_pcwtTestTimerThread!=NULL)
//	{
//		delete m_pcwtTestTimerThread;
//		m_pcwtTestTimerThread=NULL;
//
//		if(m_hThreadTestTimerFunctions!=INVALID_HANDLE_VALUE)
//		{
//			CloseHandle(m_hThreadTestTimerFunctions);
//			m_hThreadTestTimerFunctions=INVALID_HANDLE_VALUE;
//		}
//	}
//
//	m_pcwtTestTimerThread=AfxBeginThread(CTestTimerThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
//	m_hThreadTestTimerFunctions=m_pcwtTestTimerThread->m_hThread;
//	m_pcwtTestTimerThread->m_bAutoDelete = FALSE; 
//	m_pcwtTestTimerThread->ResumeThread();
//}
//void CAcuTestFrame::StopTestTimerThread( void )
//{
//	if(m_bDoTestTimerFunctionsThread)
//	{
//		m_bDoTestTimerFunctionsThread=false;
//		eventDoTestTimerFunctions.SetEvent();
//
//		if (WaitForSingleObject(m_pcwtTestTimerThread->m_hThread,3000) == WAIT_TIMEOUT)
//		{
//			if(!TerminateThread(m_pcwtTestTimerThread,0))
//			{
//			}
//		}
//	}
//	
//}
// **************************************************************************
// 
// **************************************************************************
//static UINT CTestTimerThread( LPVOID pc )
//{
//	((CAcuTestFrame*)pc)->DoTestTimerFunctions();
//	return true;
//}

// **************************************************************************
// 
// **************************************************************************
//DWORD CAcuTestFrame::DoTestTimerFunctions(void) 
//{
//	DWORD WAIT=500;
//	DWORD dwWait=0;
//	DWORD dwDiff = 0;
//	DWORD dwStart = GetTickCount();
//	DWORD dwEnd=0;
//	
//	do
//	{
//
//		dwEnd=GetTickCount();
//
//		if(dwEnd>=dwStart)
//			dwDiff=dwEnd-dwStart;
//		else
//			dwDiff=MAXDWORD-dwStart+dwEnd;
//
//		if(dwDiff<WAIT)
//		{
//			dwWait=WAIT-dwDiff;
//			//Sleep(dwWait-dwDiff);
//		}
//		else
//		{
//			dwWait=1;
//		}
//
//		testParaBtn(IDC_BTN_PARA_IFLOW,iLowerLimit, iUpperLimit, iStep);
//
//		DWORD dw = ::WaitForSingleObject(eventDoTestTimerFunctions, dwWait);
//
//		if(m_bDoTestTimerFunctionsThread==false)
//			continue;
//
//		switch(dw)
//		{
//		case WAIT_OBJECT_0:
//			{
//				
//			}
//			break;
//		default:
//			{
//				check
//			}
//			break;
//		}
//
//		dwStart = GetTickCount();
//
//	}while(m_bDoTestTimerFunctionsThread);
//
//	m_bDoTestTimerFunctionsThread=false;
//
//	return 0;
//}

