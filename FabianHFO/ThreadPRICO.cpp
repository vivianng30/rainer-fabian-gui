// ThreadPRICO.cpp : implementation file
//

// CThreadPRICO
#include "stdafx.h"
#include "Fabianlib.h"
#include "ThreadPRICO.h"
#include "MVModel.h"

#include <cmath>

extern CEvent g_eventPRICO;

//CircularBuffer<SHORT> CThreadPRICO::m_piCircBufSpO2(MAXSIZE_SPO2PRICO_BUFFER); 

// CThreadPRICO
CThreadPRICO* CThreadPRICO::theThreadPRICO=0;

CThreadPRICO::CThreadPRICO()
{
	InitializeCriticalSection(&csPRICOthread);
	InitializeCriticalSection(&csPRICOstate);
	//InitializeCriticalSection(&csPRICOactoxy);
	InitializeCriticalSection(&csPRICOcalcFiO2);
	InitializeCriticalSection(&csPRICOactspo2);
	InitializeCriticalSection(&csPRICOSpO2limit);
	InitializeCriticalSection(&csPRICOcntdown);

	m_pModel = NULL;
	//m_pAlarmlist=NULL;
	m_iPrevAvgSpO2=0;
	m_iAvgSpO2=0;
	m_iPrevSpO2val=0;
	m_iCurSpO2val=0;
	m_iCalcFiO2=0;

	//m_bPRICOdisabledByAlarm=false;
	m_bDoPRICOThread=false;
	m_pcwtPRICOThread=NULL;
	m_hThreadPRICO=INVALID_HANDLE_VALUE;
	
	m_bPRICOrunning=false;

	//m_iMeasuredOxy=-1;
	//m_iSettingOxy=-1;
	m_iHighSpO2PRICOlimit=0;
	m_iLowSpO2PRICOlimit=0;
	//m_iMeasuredSpO2=-1;
	m_bTimeOut=false;

	//m_piSpO2=  new SHORT[30]; 						// last 30s of SpO2 data, assumed pSpO2[0] is current, pSpO2[29] 29sec ago.
	//m_iCntSpo2val=0;

	//resetAlarmStates();

	m_iCntDelay=0;
}

//************************************
// Method:    ~CThreadPRICO
// FullName:  CThreadPRICO::~CThreadPRICO
// Access:    protected 
// Returns:   
// Qualifier:
//
// 2015/06/19: checked for correct closing of thread
//************************************
CThreadPRICO::~CThreadPRICO()
{
	stopPRICOThread();

	if(m_pcwtPRICOThread!=NULL)
	{
		delete m_pcwtPRICOThread;
		m_pcwtPRICOThread=NULL;
		if(m_hThreadPRICO!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadPRICO);
			m_hThreadPRICO=INVALID_HANDLE_VALUE;
		}
	}
	//EnterCriticalSection(&csPRICOactspo2);		// Get rid of SpO2 pointer array
	//m_piCircBufSpO2.~CircularBuffer();
	//LeaveCriticalSection(&csPRICOactspo2);

	//delete[] m_piSpO2;	// Get rid of SpO2 working pointer array
	//m_piSpO2 	= 0;

	DeleteCriticalSection(&csPRICOcntdown);
	DeleteCriticalSection(&csPRICOactspo2);
	DeleteCriticalSection(&csPRICOSpO2limit);
	//DeleteCriticalSection(&csPRICOactoxy);
	DeleteCriticalSection(&csPRICOcalcFiO2);
	DeleteCriticalSection(&csPRICOstate);
	DeleteCriticalSection(&csPRICOthread);
}
//=============================================================================
/**
 * @brief Get the instance of PRICO thread (singleton).
 *
 * @return the instance of PRICO thread
 *
 **/
//=============================================================================
CThreadPRICO* CThreadPRICO::getInstance()
{
	if(theThreadPRICO == 0)
	{
		theThreadPRICO = new CThreadPRICO;
	}
	return theThreadPRICO;
}

//=============================================================================
/**
 * @brief Destroys the instance of PRICO thread (singleton).
 **/
//=============================================================================
void CThreadPRICO::destroyInstance()
{
	if(theThreadPRICO != NULL)
	{
		delete theThreadPRICO;
		theThreadPRICO = NULL;
	}
}
//=============================================================================
/**
 * @brief Get the instance of the model (singleton).
 *
 * @return the instance of the model
 *
 **/
//=============================================================================
CMVModel *CThreadPRICO::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


// CThreadPRICO message handlers
// **************************************************************************
// 
// **************************************************************************
void CThreadPRICO::startPRICOThread( void )
{
	startThread();

	if(m_pcwtPRICOThread!=NULL)
	{
		delete m_pcwtPRICOThread;
		m_pcwtPRICOThread=NULL;

		if(m_hThreadPRICO!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadPRICO);
			m_hThreadPRICO=INVALID_HANDLE_VALUE;
		}

	}

	m_pcwtPRICOThread=AfxBeginThread(PRICOThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadPRICO=m_pcwtPRICOThread->m_hThread;
	m_pcwtPRICOThread->m_bAutoDelete = FALSE; 
	m_pcwtPRICOThread->ResumeThread();
}

//************************************
// Method:    stopPRICOThread
// FullName:  CThreadPRICO::stopPRICOThread
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
void CThreadPRICO::stopPRICOThread( void )
{
	if(doThread())
	{
		stopThread();
		g_eventPRICO.SetEvent();
		//WaitForSingleObject(m_pcwtPRICOThread->m_hThread,1000);

		if (WaitForSingleObject(m_pcwtPRICOThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			if(!TerminateThread(m_pcwtPRICOThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:PRICO"));
				/*int err = GetLastError();
				CStringW temp;
				temp.Format(L"TerminateThread error ConnectionThread [%d]",err);*/
			}
		}
	}
	
}

bool CThreadPRICO::doThread()
{
	bool bRes=false;
	EnterCriticalSection(&csPRICOthread);
	bRes=m_bDoPRICOThread;
	LeaveCriticalSection(&csPRICOthread);
	return bRes;
}
void CThreadPRICO::startThread()
{
	EnterCriticalSection(&csPRICOthread);
	m_bDoPRICOThread=true;
	LeaveCriticalSection(&csPRICOthread);
}
void CThreadPRICO::stopThread()
{
	EnterCriticalSection(&csPRICOthread);
	m_bDoPRICOThread=false;
	LeaveCriticalSection(&csPRICOthread);
}
// **************************************************************************
// 
// **************************************************************************
static UINT PRICOThread( LPVOID pc )
{
	((CThreadPRICO*)pc)->PRICOData();
	return TRUE;
}
// **************************************************************************
// 
// **************************************************************************
DWORD CThreadPRICO::PRICOData(void) 
{
	CeSetThreadPriority(m_pcwtPRICOThread->m_hThread,256);

	DEBUGMSG(TRUE, (TEXT("PRICOData thread\r\n")));

	CStringW sData = _T("");
	bool bDisplayLowFiO2minMsg=true;

	resetDelay();


	//m_bPRICOdisabledByAlarm=false;
	startPRICOalgorithm();

	do
	{
		if (::WaitForSingleObject(g_eventPRICO, INFINITE) == WAIT_OBJECT_0)
		{
			if(doThread() && false==getModel()->isO2FlushActive())
			{
				int iAvgSpO2 = 0;
				bool bRunPrico=false;

				SHORT iLastFiO2=CTlsFloat::Round(((double)getModel()->getDATAHANDLER()->getAppliedFiO2para())/10, 0);
				if(getModel()->getDATAHANDLER()->GetOxyCalRunning()==false)
				{
					SHORT iMeasuredOxi=CTlsFloat::Round(((double)getModel()->getDATAHANDLER()->getMessureDataO2())/10, 0);
					if(iMeasuredOxi<iLastFiO2-5 || iMeasuredOxi>iLastFiO2+5)
					{
						if(getModel()->getALARMHANDLER()->CanSetAlarm_PRICO_FiO2outOfRange())//PRICO2
						{
							if(AfxGetApp()) 
								AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_PRICO_FiO2outOfRange); //"FiO2 out of range" alarm, red one

							DEBUGMSG(TRUE, (TEXT("FiO2outOfRange\r\n")));

							CString szO2=_T("");
							szO2.Format(_T("#PRICOALARM FiO2outOfRange: LastApplied %d, Measured %d"), iLastFiO2,iMeasuredOxi);
							theApp.getLog()->WriteLine(szO2);
						}
						continue;
					}
				}
				
				SHORT iStepFiO2 = (SHORT)CTlsFloat::Round((double)calcFiO2()/10,0);
				SHORT iCalcFiO2=iLastFiO2+iStepFiO2; //do not use unsigned!!!! it was unsigned and if the step is too negative the result can be increasing the fio2

				//DEBUGMSG(TRUE, (TEXT("calculated %d\r\n"),iCalcFiO2));

				if (iCalcFiO2<21)
					iCalcFiO2=21;
				//else if (iCalcFiO2>100) //pro, new
				//	iCalcFiO2=100;	//pro, new

				bool bUpdateFiO2=true; //no fio2 out of range alarm, I changed that bool to this
				bool bLowerAlarmReset=false; //reset the "Lower minimum FiO2" alarm
				bool bHigherAlarmReset=false; //reset the "FiO2 at max target" alarm

				SHORT iFIO2low=getModel()->getDATAHANDLER()->getPRICO_FIO2lowRange();
				SHORT iFIO2high=getModel()->getDATAHANDLER()->getPRICO_FIO2highRange();

				BYTE iSpO2high= getHighSpO2PRICOlimit();
				BYTE iSpO2low= getLowSpO2PRICOlimit();
				BYTE iSpO2RangeCentre=(BYTE)CTlsFloat::Round((double)(iSpO2low+iSpO2high)/2,0);
				BYTE iCurSpo2val=(BYTE)CTlsFloat::Round((double)m_iCurSpO2val/10,0);


				if(	(iLastFiO2>iFIO2low && iLastFiO2<iFIO2high) || (iCurSpo2val>=iFIO2low && iCurSpo2val<=iFIO2high))
				{
					bLowerAlarmReset=true;
					bHigherAlarmReset=true;
				}



				if(iStepFiO2!=0)
				{
					Sleep(0);					

					if( iCalcFiO2 >= iFIO2low && iCalcFiO2 <= iFIO2high) //will update the value to the calculated one if it is in the range
					{
						bUpdateFiO2=true;
						//DEBUGMSG(TRUE, (TEXT("normal update %d\r\n"),iCalcFiO2));
						bLowerAlarmReset=true;
						bHigherAlarmReset=true;
					}
					else if( iLastFiO2 == iFIO2low && iCalcFiO2 < iFIO2low ) //gives and alarm if we already using the lowest possible value but the algorithm wants to step below
					{
						bUpdateFiO2=false;

						if(getModel()->getALARMHANDLER()->CanSetAlarm_PRICO_FiO2min())//PRICO2
						{
							if (iFIO2low>21)
							{
								if(AfxGetApp()) 
									AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_PRICO_FiO2min); //"Lower minimum FiO2" alarm, red one
								DEBUGMSG(TRUE, (TEXT("Lower minimum FiO2\r\n")));

								CString szO2=_T("");
								szO2.Format(_T("#PRICOALARM FiO2:%d, "), iCalcFiO2);
								theApp.getLog()->WriteLine(szO2);
							}
						}
						bHigherAlarmReset=true;
						bLowerAlarmReset=false;
					}
					else if ( iLastFiO2 == iFIO2high && iCalcFiO2 > iFIO2high ) //gives an alarm if we already using the highest possible value but the algorithm wants to step above
					{
						bUpdateFiO2=false;

						if(getModel()->getALARMHANDLER()->CanSetAlarm_PRICO_FiO2max())//PRICO2
						{
							if(AfxGetApp()) 
								AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_PRICO_FiO2max); //"FiO2 at max target" alarm, red one
							DEBUGMSG(TRUE, (TEXT("Maximum FiO2 reached\r\n")));

							CString szO2=_T("");
							szO2.Format(_T("#PRICOALARM FiO2:%d, "), iCalcFiO2);
							theApp.getLog()->WriteLine(szO2);
						}
						bLowerAlarmReset=true;
						bHigherAlarmReset=false;
					}
					else if( iCalcFiO2 < iFIO2low) //sets it to the lowest possible value if the algorithm wants to step below from the range
					{
						bUpdateFiO2=true;
						iCalcFiO2=iFIO2low;
						DEBUGMSG(TRUE, (TEXT("low set\r\n")));
						bHigherAlarmReset=true;
						bLowerAlarmReset=true;
					}
					else if( iCalcFiO2 > iFIO2high) //sets it to the highest possible value if the algorithm wants to step above from the range
					{
						bUpdateFiO2=true;
						iCalcFiO2=iFIO2high;
						DEBUGMSG(TRUE, (TEXT("high set\r\n")));
						bLowerAlarmReset=true;
						bHigherAlarmReset=true;
					}
					bDisplayLowFiO2minMsg=true;
				}
				else//PRICO2
				{
					Sleep(0);

					BYTE iFiO2RangeCentre=(BYTE)CTlsFloat::Round((double)(iFIO2low+iFIO2high)/2,0);					

					if( iLastFiO2 < iFIO2low) //safety, sets it to the lowest possible value if somehow the FiO2 would be below
					{
						bUpdateFiO2=true;
						iCalcFiO2=iFIO2low;
						DEBUGMSG(TRUE, (TEXT("low set\r\n")));
					}
					else if( iLastFiO2 > iFIO2high) //safety, sets it to the highest possible value if somehow the FiO2 would be higher
					{
						bUpdateFiO2=true;
						iCalcFiO2=iFIO2high;
						DEBUGMSG(TRUE, (TEXT("high set\r\n")));
					}

					if ( iCurSpo2val >= iSpO2RangeCentre && iLastFiO2>21)
					{
						if ( iLastFiO2==iFIO2low ) //if we already using the lowest value and it could be decreased gives a recommendation message
						{
							bUpdateFiO2=false;
							if(bDisplayLowFiO2minMsg)//PRICO2
							{		
								bDisplayLowFiO2minMsg=false;
								if(AfxGetApp()) 
									AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_PRICO_LOWFIO2); //"Lower minimum FiO2" message, not yellow, not red

								CString szO2=_T("");
								szO2.Format(_T("#PRICORECOM FiO2:%d, "), iCalcFiO2);
								theApp.getLog()->WriteLine(szO2);
								DEBUGMSG(TRUE, (TEXT("Lower minimum FiO2 recom\r\n")));
							}
						} 
						else //if we do not use the lowest possible value then it decreases it with 1%
						{
							bDisplayLowFiO2minMsg=true;
							bUpdateFiO2=true;
							iStepFiO2=-1;
							iCalcFiO2=iLastFiO2+iStepFiO2;
							DEBUGMSG(TRUE, (TEXT("new step FiO2 is -1\r\n")));
						}
					} 
					else if ( iLastFiO2 < iFiO2RangeCentre && iCurSpo2val < iSpO2RangeCentre && iLastFiO2<100)
					{
						if (iLastFiO2>=iFIO2high) //cannot happen but lets handle it
						{
							bUpdateFiO2=false;
							DEBUGMSG(TRUE, (TEXT("cannot happen\r\n")));
						} 
						else //tries to increase SpO2 by increasing O2 with 1% till it is not higher than the rangecentre
						{
							bUpdateFiO2=true;
							iStepFiO2= 1;	
							iCalcFiO2=iLastFiO2+iStepFiO2;
							DEBUGMSG(TRUE, (TEXT("new step FiO2 is +1\r\n")));
						}
						bDisplayLowFiO2minMsg=true;
					}		
					else
					{
						bUpdateFiO2=false;
						//DEBUGMSG(TRUE, (TEXT("FiO2 not changed\r\n")));
						bDisplayLowFiO2minMsg=true;
					}
					bHigherAlarmReset=true;
					bLowerAlarmReset=true;
				}

				if(bUpdateFiO2)
				{
					resetDelay();						
					//send to PIC
					getModel()->Send_PARA_OXY_RATIO(iCalcFiO2,true,true);//PRICO04
					setCalculatedOxyValue(iCalcFiO2);
				}

				if(bLowerAlarmReset)
				{
					if(getModel()->getALARMHANDLER()->ALARM_PRICO_FiO2min->getAlarmState()==AS_ACTIVE)
					{
						getModel()->getALARMHANDLER()->setStateOfAlarm(AL_PRICO_FiO2min, AS_SIGNALED);
					}
				}

				if(bHigherAlarmReset)
				{
					if(getModel()->getALARMHANDLER()->ALARM_PRICO_FiO2max->getAlarmState()==AS_ACTIVE)
					{
						getModel()->getALARMHANDLER()->setStateOfAlarm(AL_PRICO_FiO2max, AS_SIGNALED);
					}
				}
			}
			Sleep(0);
		}
		Sleep(0);

	}while(doThread());

	stopPRICOalgorithm();

	resetCircBufSpO2();

	theApp.getLog()->WriteLine(_T("#THR:PRICO end"));
	DEBUGMSG(TRUE, (TEXT("CThreadPRICO::PRICOData stop\r\n")));

	return 0;
}

void CThreadPRICO::resetCircBufSpO2()
{
	EnterCriticalSection(&csPRICOactspo2);
	//m_piCircBufSpO2.reset();
	m_iPrevAvgSpO2=0;
	m_iAvgSpO2=0;
	m_iPrevSpO2val=0;
	m_iCurSpO2val=0;
	LeaveCriticalSection(&csPRICOactspo2);
}
//void CThreadPRICO::setAlarmDisabled()
//{
//	m_bPRICOdisabledByAlarm=true;
//	DEBUGMSG(TRUE, (TEXT("m_bPRICOdisabledByAlarm=true\r\n")));
//}
//void CThreadPRICO::resetAlarmDisabled()
//{
//	m_bPRICOdisabledByAlarm=false;
//}
//bool CThreadPRICO::isAlarmDisabled()
//{
//	return m_bPRICOdisabledByAlarm;
//}


//BYTE CThreadPRICO::getSpO2buf(SHORT* piSpO2)
//{
//	BYTE iWindow=0;
//	EnterCriticalSection(&csPRICOactspo2);
//	memcpy(&piSpO2,m_piSpO2,sizeof(m_piSpO2));
//	iWindow=m_iCntSpo2val;
//	m_iCntSpo2val=0;
//	LeaveCriticalSection(&csPRICOactspo2);
//	return iWindow;
//	
//}
SHORT CThreadPRICO::calcFiO2(/*int iAvgSpO2*/)
{
	DWORD dwStart=GetTickCount();
	
	// Future Targets, used for the prediction part of the algorithm
      int iCurHiTarget= getHighSpO2PRICOlimit()*10;
      int iCurLoTarget=getLowSpO2PRICOlimit()*10;
      int iFutHiTarget = iCurHiTarget-10; //It needs a factor 10 like above, if its true that the spo2 is given as a 210 to 1000 range
      int iFutLoTarget = iCurLoTarget+10;



      // Calculate Rate assuming they are one sec apart...
      //int iRateSpO2 = piSpO2[0]-piSpO2[1];      //Tom: I use the average SpO2 to calculate the speed, so iAvgSpO2
      //int iRateSpO2 = m_piSpO2[0]-m_piSpO2[1];
      //int iRateSpO2 = m_iCurSpO2val-m_iPrevSpO2val;  //As stated above, should be based on m_iAvgSpO2
      int iRateSpO2 = m_iAvgSpO2- m_iPrevAvgSpO2;  //these have to be added //PRICO04

      // Calculate Predicted Value
      //int iPredSpO2 = iAvgSpO2 + (MAXSIZE_SPO2PRICO_BUFFER*iRateSpO2);
      //int iPredSpO2 = m_iAvgSpO2 + 30*iRateSpO2; //It should predict the SpO2 30 seconds in the future
	  int iPredSpO2 = CTlsFloat::Round((double)(m_iAvgSpO2 + 30*iRateSpO2)/10,0)*10; //pro, change 4
	  int iCurSpO2val=CTlsFloat::Round((double)m_iCurSpO2val/10,0)*10; //pro, change 4
	  //pro, added 2
	  if(iPredSpO2 >= 1000)
		  iPredSpO2 = 1000;
	  else if(iPredSpO2 <= 0)
		  iPredSpO2 = 0;

      // Condition for Positive and Negative Step Value FiO2
      bool bCondHi = false;
      bool bCondLo = false;
      // Tom: Changed the high low conditions the elseif will make the controller quicker but less stable
      //if((iPredSpO2 > iFutHiTarget) && (m_iCurSpO2val > iCurHiTarget-10)){bCondHi = true;}  // SpO2 above the high target, FiO2 needs to go down
      //if((iPredSpO2 < iFutLoTarget) && (m_iCurSpO2val < iCurLoTarget+10)){bCondLo = true;}  // SpO2 below the low target, FiO2 needs to go up
	  if((iPredSpO2 > iFutHiTarget) && (iCurSpO2val > iCurHiTarget)){bCondHi = true;}  // SpO2 above the high target, FiO2 needs to go down //pro, change 4
      if((iPredSpO2 < iFutLoTarget) && (iCurSpO2val < iCurLoTarget)){bCondLo = true;}  // SpO2 below the low target, FiO2 needs to go up //pro, change 4

      // Reduce Target range to the relevant Single Target
      int iCurTarget = 0;
      int iFutTarget = 0; 

      // Targets for bCondHi &  bCondLo
      if(bCondHi==true)
      {
            iCurTarget = iCurHiTarget;
            iFutTarget = iFutHiTarget;
      }
      else if(bCondLo==true)
      {
            iCurTarget = iCurLoTarget;
            iFutTarget = iFutLoTarget;
      }

      // Current and Predicted Errors
      //int iErrorSat = m_iCurSpO2val - iCurTarget; //pro, change 4
	  int iErrorSat = iCurSpO2val - iCurTarget;
      int iPredErrorSat = iPredSpO2 - iFutTarget;

      // Check return of these values.
     // double dFiO2 = (double)getMeasuredOxyValue();
	  double dFiO2=getModel()->getDATAHANDLER()->getAppliedFiO2para();
	  double dInitGain = (dFiO2/100) + 0.8;
      double dErrorSat = (double)iErrorSat;
      double dCtrlError = dInitGain * (-dErrorSat);
      double dPredErrorSat = (double)iPredErrorSat;                    //Tom: moved these up with the rest
      double dCtrlPredError = dInitGain * (-dPredErrorSat);

      // Below Target - @TOM GOOS: Can you double check this section??
      // --------------------------------------------------------------
      bool bBelowTarget = false;
      double dMinVal = -1;
      // if(dCtrlError < 0)
      // instead use
      if(bCondLo == true)
      {
            bBelowTarget = true;
            dMinVal = 1;
      }

      // Ctrl based on Pred Error
      if(std::abs(dPredErrorSat) < 2)
      {
            dCtrlPredError = dMinVal;
      }           // Limis stepsize if Prediction is only just of target

      // Calculate Initial Step
      double dInitStep = dCtrlError;
      if(bBelowTarget)
      {
            if(iRateSpO2 <= 0)
            {
                  dInitStep = dCtrlPredError;
            }                 // If there is no rate of change or it is moving towards the target, the stepsize is calculated based on the prediction
            else if(dCtrlError == 0)
            {
                  dInitStep = 1;
            }
      }
      else
      {
            if(iRateSpO2 >= 0)
            {
                  dInitStep = dCtrlPredError;
            }                 // If there is no rate of change or it is moving towards the target, the stepsize is calculated based on the prediction
            else if(dCtrlError == 0)
            {
                  dInitStep = -1;
            }
      }

      // Rounding of Stepsize
      double dStep;
      //if(std::isnan(dInitStep))
      if(_isnan(dInitStep))
      {
            dStep = 0;
      }
      else
      {
            //dInitStep = std::round(dInitStep);
            dInitStep = CTlsFloat::Round(dInitStep, 0);
            if(std::abs(dInitStep) > 10)
            {
                  //dStep = (std::round(dInitStep/5))*5;
                  dStep = (CTlsFloat::Round(dInitStep/5, 0))*5;
            }
            else
            {
                  dStep = dInitStep;
            }
      }
      //int iStep = (int)std::round(dStep);
      int iStep = (int)CTlsFloat::Round(dStep, 0);

      // Final Step size
      int iFiO2Step = 0;

      if((bCondHi==true || bCondLo==true)){iFiO2Step = iStep;}         
      if(bCondHi==bCondLo){iFiO2Step = 0;}                       //Tom: This now only works if the actual and predicted are on the same side of the target

      //CStringW sData = _T("");
      //sData.Format(L"PRICO: %d:%d->%d",m_iAvgSpO2,iPredSpO2,iFiO2Step);
      ////theApp.getLog()->WriteLine(sData);
      //CMVEventInfotext event2(CMVEventInfotext::EV_TIMETEXT,  sData, 1000);
      //getModel()->triggerEvent(&event2);

      /*delete[] piSpO2;      
      piSpO2      = 0;*/

	  //DWORD dwEnd=GetTickCount();
	  //DEBUGMSG(TRUE, (TEXT("TIME calcFiO2 %d\r\n"),dwEnd-dwStart));
      // result
      //pro, added 2
      if (iFiO2Step>=100)
		  iFiO2Step=100;
	  else if (iFiO2Step<=-100)
		  iFiO2Step=-100;

      return iFiO2Step;
	
}



void CThreadPRICO::startPRICOalgorithm()
{
	DEBUGMSG(TRUE, (TEXT("CThreadPRICO::startPRICO\r\n")));
	EnterCriticalSection(&csPRICOstate);
	m_bPRICOrunning=true;
	LeaveCriticalSection(&csPRICOstate);
}

void CThreadPRICO::stopPRICOalgorithm()
{
	DEBUGMSG(TRUE, (TEXT("CThreadPRICO::stopPRICO\r\n")));
	EnterCriticalSection(&csPRICOstate);
	m_bPRICOrunning=false;
	LeaveCriticalSection(&csPRICOstate);
}

bool CThreadPRICO::isPRICOalgorithmRunning()
{
	EnterCriticalSection(&csPRICOstate);
	bool bRunning=m_bPRICOrunning;
	LeaveCriticalSection(&csPRICOstate);
	return bRunning;
}



//void CThreadPRICO::setMeasuredOxyValue(SHORT value)
//{
//	EnterCriticalSection(&csPRICOactoxy);
//	m_iMeasuredOxy=value;
//	LeaveCriticalSection(&csPRICOactoxy);
//
//	//DEBUGMSG(TRUE, (TEXT("PRICO::setMeasuredOxyValue %d\r\n"),value));
//}
//
//SHORT CThreadPRICO::getMeasuredOxyValue()
//{
//	EnterCriticalSection(&csPRICOactoxy);
//	SHORT value=m_iMeasuredOxy;
//	LeaveCriticalSection(&csPRICOactoxy);
//	return value;
//}
void CThreadPRICO::setStartupOxyValue(SHORT value)
{
	setCalculatedOxyValue(value);
}
void CThreadPRICO::setCalculatedOxyValue(SHORT value)
{
	EnterCriticalSection(&csPRICOcalcFiO2);
	m_iCalcFiO2=value;
	LeaveCriticalSection(&csPRICOcalcFiO2);

	CString szO2=_T("");
	szO2.Format(_T("#PRICO FiO2:%d, "), value);
	theApp.getLog()->WriteLine(szO2);
}

SHORT CThreadPRICO::getCalculatedOxyValue()
{
	EnterCriticalSection(&csPRICOcalcFiO2);
	SHORT value=m_iCalcFiO2;
	LeaveCriticalSection(&csPRICOcalcFiO2);
	return value;
}

void CThreadPRICO::setMeasuredSpO2Value(SHORT iValue)
{
	if(getModel()->isO2FlushActive())
		return;

	EnterCriticalSection(&csPRICOactspo2);
	m_iPrevSpO2val=m_iCurSpO2val;
	m_iCurSpO2val=iValue;

	m_iPrevAvgSpO2=m_iAvgSpO2;//PRICO04
		
	if(m_iAvgSpO2==0)
	{
		m_iAvgSpO2=m_iCurSpO2val;
	}
	else
	{
		m_iAvgSpO2=(PRICO_ALPHA*m_iCurSpO2val)+((1-PRICO_ALPHA)*m_iAvgSpO2);
	}
	LeaveCriticalSection(&csPRICOactspo2);

	decreaseDelay();

	if(doThread() && getDelay()==0)
		g_eventPRICO.SetEvent();
	//else if(m_bDoPRICOThread)//check FiO2 against range //rku O2
	//{
	//	checkFiO2range();
	//}
}

void CThreadPRICO::decreaseDelay()
{
	EnterCriticalSection(&csPRICOcntdown);
	if(m_iCntDelay>0)
		m_iCntDelay--;

	LeaveCriticalSection(&csPRICOcntdown);
}
void CThreadPRICO::resetDelay()
{
	EnterCriticalSection(&csPRICOcntdown);
	m_iCntDelay=30;
	LeaveCriticalSection(&csPRICOcntdown);
}
BYTE CThreadPRICO::getDelay()
{
	BYTE res=0;
	EnterCriticalSection(&csPRICOcntdown);
	res=m_iCntDelay;
	LeaveCriticalSection(&csPRICOcntdown);
	return res;
}



//void CThreadPRICO::checkFiO2range()
//{
//	BYTE iFIO2low=getModel()->getDATAHANDLER()->getPRICO_FIO2lowRange();
//	BYTE iFIO2high=getModel()->getDATAHANDLER()->getPRICO_FIO2highRange();
//	int iFiO2=CTlsFloat::Round(((double)getModel()->getDATAHANDLER()->getMessureDataO2calindependent())/10, 0);//rku O2
//	if(		iFiO2<iFIO2low 
//		||	iFiO2>iFIO2high)
//	{
//		//set alarm
//		if(getModel()->getALARMHANDLER()->CanSetAlarm_PRICO_FiO2())//PRICO2
//		{
//			if(AfxGetApp())
//				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_PRICO_FiO2);
//		}
//	}
//	else if(getModel()->getALARMHANDLER()->ALARM_PRICO_FiO2limit->getAlarmState()==AS_ACTIVE)
//	{
//		getModel()->getALARMHANDLER()->setStateOfAlarm(AL_PRICO_FiO2limit, AS_SIGNALED);
//	}
//}









//SHORT CThreadPRICO::getMeasuredSpO2Value(BYTE iCnt)
//{
//	SHORT value=0;
//	EnterCriticalSection(&csPRICOactspo2);
//	if(iCnt<=m_iCntSpo2val)
//		value=m_piSpO2[m_iCntSpo2val];
//	LeaveCriticalSection(&csPRICOactspo2);
//	return value;
//}
//BYTE CThreadPRICO::getCountMeasuredSpO2Buf()
//{
//	BYTE iCnt=0;
//	EnterCriticalSection(&csPRICOactspo2);
//	iCnt=m_iCntSpo2val;
//	LeaveCriticalSection(&csPRICOactspo2);
//	return iCnt;
//}
void CThreadPRICO::setHighSpO2PRICOlimit(SHORT value)
{
	EnterCriticalSection(&csPRICOSpO2limit);
	m_iHighSpO2PRICOlimit=value;
	LeaveCriticalSection(&csPRICOSpO2limit);
}

SHORT CThreadPRICO::getHighSpO2PRICOlimit()
{
	EnterCriticalSection(&csPRICOSpO2limit);
	SHORT value=m_iHighSpO2PRICOlimit;
	LeaveCriticalSection(&csPRICOSpO2limit);
	return value;
}

void CThreadPRICO::setLowSpO2PRICOlimit(SHORT value)
{
	EnterCriticalSection(&csPRICOSpO2limit);
	m_iLowSpO2PRICOlimit=value;
	LeaveCriticalSection(&csPRICOSpO2limit);
}

SHORT CThreadPRICO::getLowSpO2PRICOlimit()
{
	EnterCriticalSection(&csPRICOSpO2limit);
	SHORT value=m_iLowSpO2PRICOlimit;
	LeaveCriticalSection(&csPRICOSpO2limit);
	return value;
}



/**********************************************************************************************/
//code from TOM
//int fnUpdate(int iCurHiTarget, int iCurLoTarget,int iCurFiO2,int* piSpO2,int iWindow)
//{
//	// Future Targets, used for the prediction part of the algoritm
//	int iFutHiTarget = iCurHiTarget-1;
//	int iFutLoTarget = iCurLoTarget+1;
//
//	//Calculate Average SpO2 over window
//	double dAvgSpO2 = 0;
//	for(int i=0;i<iWindow;i++){dAvgSpO2 += (double)piSpO2[i];}  // dAvgSpO2[0] = a * piSpO2[0] + (1-a) * dAvgSpO2[1] a = ( 2 / ( 1 +iWindow ))
//	dAvgSpO2 = dAvgSpO2/iWindow;
//	int iAvgSpO2 = (int)std::round(dAvgSpO2);
//
//	// Calculate Rate assuming they are one sec apart...
//	int iRateSpO2 = piSpO2[0]-piSpO2[1];      //Tom: I use the average SpO2 to calculate the speed, so iAvgSpO2
//
//	// Calculate Predicted Value
//	int iPredSpO2 = iAvgSpO2 + (iWindow*iRateSpO2);
//
//	// Condition for Positive and Negative Step Value FiO2
//	bool bCondHi = false;
//	bool bCondLo = false;
//	// Tom: Changed the high low conditions the elseif will make the controller quicker but less stable
//	if((iPredSpO2 > iFutHiTarget) && (piSpO2[0] > iCurHiTarget-1)){bCondHi = true;}  // SpO2 above the high target, FiO2 needs to go down
//	// else if ((piSpO2[0] < iCurHiTarget) && (piSpO2[0] > iCurLoTarget) && iPredSpO2 > iFutHiTarget)) {bCondHi = true;}  //SpO2 within, but prediction above, FiO2 needs to go down
//	if((iPredSpO2 < iFutLoTarget) && (piSpO2[0] < iCurLoTarget+1)){bCondLo = true;}  // SpO2 below the low target, FiO2 needs to go up
//	// else if ((piSpO2[0] < iCurHiTarget) && (piSpO2[0] > iCurLoTarget) && (iPredSpO2 < iFutLoTarget)) {bCondLo = true;} //SpO2 within, but prediction below, FiO2 needs to go up
//
//	// Reduce Target range to the relevant Single Target
//	int iCurTarget = 0;
//	int iFutTarget = 0; 
//
//	// Targets for bCondHi &  bCondLo
//	if(bCondHi==true)
//	{
//		iCurTarget = iCurHiTarget;
//		iFutTarget = iFutHiTarget;
//	}
//	else if(bCondLo==true)
//	{
//		iCurTarget = iCurLoTarget;
//		iFutTarget = iFutLoTarget;
//	}
//
//	// Current and Predicted Errors
//	int iErrorSat = piSpO2[0] - iCurTarget;
//	int iPredErrorSat = iPredSpO2 - iFutTarget;
//
//	// Check return of these values.
//	double dFiO2 = (double)iFiO2;
//	double dInitGain = (dFiO2/100) + 0.8;
//	double dErrorSat = (double)iErrorSat;
//	double dCtrlError = dInitGain * (-ErrorSat);
//	double dPredError = (double)iPredErrorSat;				//Tom: moved these up with the rest
//	double dCtrlPredError = dInitGain * (-dPredErrorSat);
//
//	// Below Target - @TOM GOOS: Can you double check this section??
//	// --------------------------------------------------------------
//	bool bBelowTarget = false;
//	double dMinVal = -1;
//	// if(dCtrlError < 0)
//	// instead use
//	if bCondLo = true
//	{
//		bBelowTarget = true;
//		dMinVal = 1;
//	}
//
//	// Ctrl based on Pred Error
//	if(std::abs(dPredError) < 2){dCtrlPredError = dMinVal;}		// Limis stepsize if Prediction is only just of target
//
//	// Calculate Initial Step
//	double dInitStep = = dCtrlError;
//	if(bBelowTarget)
//	{
//		if(iRateSpO2 <= 0){dInitStep = dCtrlPredError;}			// If there is no rate of change or it is moving towards the target, the stepsize is calculated based on the prediction
//		else if(dCtrlError == 0){dInitStep = 1;}
//	}
//	else
//	{
//		if(iRateSpO2 >= 0){dInitStep = CtrlPredError;}			// If there is no rate of change or it is moving towards the target, the stepsize is calculated based on the prediction
//		else if(dCtrError == 0){dInitStep = -1;}
//	}
//
//	// Rounding of Stepsize
//	double dStep;
//	if(std::isnan(dInitStep)){dStep = 0;}
//	else
//	{
//		dInitStep = std::round(dInitStep);
//		if(std::abs(dInitStep) > 10){dStep = (std::round(dInitStep/5))*5;}
//		else{dStep = dInitVal;}
//	}
//	int iStep = (int)std::round(dStep);
//
//	// Final Step size
//	int iFiO2Step = 0;
//	if((std::abs(piSpO2[0] - iAvgSpO2) <= 10))				// check if the current SpO2 is not to far form the average, if they are it indicates a rappid change and controll should wait
//		// We coud make this a time out condition
//	{
//		if((bCondHi==true || bCondLo==true)){iFiO2Step = iStep;}		
//		if(bCondHi==bCondLo){iFiO2Step = 0;}				//Tom: This now only works if the actual and predicted are on the same side of the target
//	}
//
//	// result
//	return iFiO2Step;	
//}





// ========================================================================== 
//   main.cpp                                                    				
//  (c) 2015, C.E.E. Zonneveld                                                

//   Description:                                                              
//   Translation of Tom's Algorithm 							
// ==========================================================================
//#include <cmath>
//
//int fnUpdate(int iCurHiTarget, int iCurLoTarget,int iCurFiO2,int* piSpO2,int iWindow)
//{
//	// Future Targets, used for the prediction part of the algorithm
//	int iFutHiTarget = iCurHiTarget-1;
//	int iFutLoTarget = iCurLoTarget+1;
//
//	//Calculate Average SpO2 over window
//	double dAvgSpO2 = 0;
//	for(int i=0;i<iWindow;i++){dAvgSpO2 += (double)piSpO2[i];}
//	dAvgSpO2 = dAvgSpO2/iWindow;
//	int iAvgSpO2 = (int)std::round(dAvgSpO2);
//	int iAvgSpO2 = (int)std::round(dAvgSpO2); // te fixen 1 - 31, 0-30
//
//	//dAvgSpO2[0] = a  piSpO2[0] + (1-a)  dAvgSpO2[1]
//	//( 2 / ( 1 +iWindow )) Avg Exponential decrease average
//
//	// Calculate Rate assuming they are one sec apart...
//	int iRateSpO2 = piSpO2[0]-piSpO2[1];      //Tom: I use the average SpO2 to calculate the speed, so iAvgSpO2
//
//	// Calculate Predicted Value
//	int iPredSpO2 = iAvgSpO2 + (iWindow*iRateSpO2);
//
//	// Condition for Positive and Negative Step Value FiO2
//	bool bCondHi = false;
//	bool bCondLo = false;
//	// Tom: Changed the high low conditions the elseif will make the controller quicker but less stable
//	if((iPredSpO2 > iFutHiTarget) && (piSpO2[0] > iCurHiTarget-1)){bCondHi = true;}  // SpO2 above the high target, FiO2 needs to go down
//	if((iPredSpO2 < iFutLoTarget) && (piSpO2[0] < iCurLoTarget+1)){bCondLo = true;}  // SpO2 below the low target, FiO2 needs to go up
//
//	// Reduce Target range to the relevant Single Target
//	int iCurTarget = 0;
//	int iFutTarget = 0; 
//
//	// Targets for bCondHi &  bCondLo
//	if(bCondHi==true)
//	{
//		iCurTarget = iCurHiTarget;
//		iFutTarget = iFutHiTarget;
//	}
//	else if(bCondLo==true)
//	{
//		iCurTarget = iCurLoTarget;
//		iFutTarget = iFutLoTarget;
//	}
//
//	// Current and Predicted Errors
//	int iErrorSat = piSpO2[0] - iCurTarget;
//	int iPredErrorSat = iPredSpO2 - iFutTarget;
//
//	// Check return of these values.
//	double dFiO2 = (double)iFiO2;
//	double dInitGain = (dFiO2/100) + 0.8;
//	double dErrorSat = (double)iErrorSat;
//	double dCtrlError = dInitGain * (-ErrorSat);
//	double dPredError = (double)iPredErrorSat;				//Tom: moved these up with the rest
//	double dCtrlPredError = dInitGain * (-dPredErrorSat);
//
//	// Below Target - @TOM GOOS: Can you double check this section??
//	// --------------------------------------------------------------
//	bool bBelowTarget = false;
//	double dMinVal = -1;
//	// if(dCtrlError < 0)
//	// instead use
//	if(bCondLo == true)
//	{
//		bBelowTarget = true;
//		dMinVal = 1;
//	}
//
//	// Ctrl based on Pred Error
//	if(std::abs(dPredError) < 2){dCtrlPredError = dMinVal;}		// Limis stepsize if Prediction is only just of target
//
//	// Calculate Initial Step
//	double dInitStep = = dCtrlError;
//	if(bBelowTarget)
//	{
//		if(iRateSpO2 <= 0){dInitStep = dCtrlPredError;}			// If there is no rate of change or it is moving towards the target, the stepsize is calculated based on the prediction
//		else if(dCtrlError == 0){dInitStep = 1;}
//	}
//	else
//	{
//		if(iRateSpO2 >= 0){dInitStep = CtrlPredError;}			// If there is no rate of change or it is moving towards the target, the stepsize is calculated based on the prediction
//		else if(dCtrError == 0){dInitStep = -1;}
//	}
//
//	// Rounding of Stepsize
//	double dStep;
//	if(std::isnan(dInitStep)){dStep = 0;}
//	else
//	{
//		dInitStep = std::round(dInitStep);
//		if(std::abs(dInitStep) > 10){dStep = (std::round(dInitStep/5))*5;}
//		else{dStep = dInitVal;}
//	}
//	int iStep = (int)std::round(dStep);
//
//	// Final Step size
//	int iFiO2Step = 0;
//
//	if((bCondHi==true || bCondLo==true)){iFiO2Step = iStep;}		
//	if(bCondHi==bCondLo){iFiO2Step = 0;}				//Tom: This now only works if the actual and predicted are on the same side of the target
//
//
//	// result
//	return iFiO2Step;	
//}
//
//
//void main()
//{
//	// External Dummy parameters (from GUI)
//	int iHiSpO2 = 95;										// hyperoxia (high SpO2 alarm limit)
//	int iLoSpO2 = 85;										// hypoxia (low SpO2 alarm limit)
//	int iFiO2 = 21;											// current FiO2
//	const int iWindow = 30;									// 30s Window
//	int* piSpO2;											// pointer array for SpO2 data
//	piSpO2 new int[iWindow]; 								// last 30s of SpO2 data, assumed pSpO2[0] is current, pSpO2[29] 29sec ago.
//	for(int i=0;i<iWindow;i++)	{piSpO2[i] = 90;}			// fill pointer array with dummy data
//
//	// Timed out if there has been an FiO2 adjustment in the last 30s (other conditions to follow??). if((std::abs(piSpO2[0] - iAvgSpO2) <= 10))	
//	bool TimeOut = true; 
//
//	//	if not timed out, adjust external FiO2
//	if(!TimeOut){iFiO2 += fnUpdate(iHiSpO2,iLoSpO2,iFiO2,piSpO2,iWindow);}
//
//	// administrative duties
//	delete[] piSpO2;	piSpO2 	= 0;							// Get rid of SpO2 pointer array
//}



//	if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
//	{
//		if(getModel()->getPRICOThread()->isPRICOrunning())
//		{
//			EnterCriticalSection(&csAlarmList);
//			getModel()->getPRICOThread()->setAlarmStates(m_pAlarmlist);
//			LeaveCriticalSection(&csAlarmList);
//		}
//	}
