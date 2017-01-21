// InterfaceCOMInbiolab.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "InterfaceCOMInbiolab.h"
#include "MVModel.h"

#include "InterfaceCOMInbiolab.h"
// CInterfaceCOMInbiolab

WORD fcsTable[256] = {
	0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
	0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7, 
	0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
	0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
	0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
	0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
	0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
	0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
	0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
	0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
	0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
	0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
	0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
	0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
	0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
	0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
	0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
	0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
	0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
	0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
	0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
	0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
	0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
	0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
	0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
	0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
	0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
	0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
	0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
	0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
	0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
	0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

CInterfaceCOMInbiolab::CInterfaceCOMInbiolab()
{
	fdwRecvBufLen = 0;
	feRecvState = COM_INBIOLAB_SOF;
	fwCalcChkSum = 0xffff;
	//fwCalcChkSum = 0;

	//fwReadChkSum=0;
	fwChkSumUpper = 0;
	fwChkSumLower = 0;

	m_cLastWaveVal=0;

	m_bTrigger=false;

	m_bCEflag=false;
	//fwReadChkSum=0;
	//m_byCmdBuffer[0]=0x0000;
	//m_byLength=0;
	//m_dwLastCalCheck=0;
	faRecvBuf[0]=0x0000;
	
	fiCntIdxOld=0;
	fiCntIdx=0;
	fiCntIdx_1=0;
	fiCntIdx_2=0;
	fiCntIdx_3=0;
	fiCntIdx_4=0;

	fiSample0=0;
	fiSample0_1=0;
	fiSample0_2=0;
	fiSample0_3=0;
	fiSample1=0;
	fiSample1_1=0;
	fiSample1_2=0;
	fiSample1_3=0;
	fiSample2=0;
	fiSample2_1=0;
	fiSample2_2=0;
	fiSample2_3=0;
	fiSample3=0;
	fiSample3_1=0;
	fiSample3_2=0;
	fiSample3_3=0;
	fiSample4=0;
	fiSample4_1=0;
	fiSample4_2=0;
	fiSample4_3=0;
	fiSample5=0;
	fiSample5_1=0;
	fiSample5_2=0;
	fiSample5_3=0;
	fiSample6=0;
	fiSample6_1=0;
	fiSample6_2=0;
	fiSample6_3=0;

	fiTrigger=0;

	fwFrameType=0;
	

	m_bProcessing=false;

	m_bDoSerialReceiveThread=false;
	m_pcwtSerialReceiveThread=NULL;
	m_hThreadSerialReceive=INVALID_HANDLE_VALUE;
}

CInterfaceCOMInbiolab::~CInterfaceCOMInbiolab()
{
}


// CInterfaceCOMInbiolab message handlers
int CInterfaceCOMInbiolab::Init(BYTE com)
{
	DWORD dwIndex=2;
	//DWORD dwIndex=9;

	if (!m_comPort.Open(dwIndex,115200))
	{
		theApp.getLog()->WriteLine(_T("#HFO:0009"));
		return false;
	}
	else
	{
		StartInbiolabThread();
		//StartSerialThread();
		return true;
	}

	return 0;
}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceCOMInbiolab::StartInbiolabThread( void )
{
	m_bDoSerialReceiveThread=true;

	//DEBUGMSG(TRUE, (TEXT("StartSerialThread1\r\n")));

	//DEBUGMSG(TRUE, (TEXT("StartSerialThread1\r\n")));

	if(m_pcwtSerialReceiveThread!=NULL)
	{
		delete m_pcwtSerialReceiveThread;
		m_pcwtSerialReceiveThread=NULL;

		if(m_hThreadSerialReceive!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSerialReceive);
			m_hThreadSerialReceive=INVALID_HANDLE_VALUE;
		}
	}
	//DEBUGMSG(TRUE, (TEXT("StartSerialThread2\r\n")));

	m_pcwtSerialReceiveThread=AfxBeginThread(CSerialReceiveThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadSerialReceive=m_pcwtSerialReceiveThread->m_hThread;
	m_pcwtSerialReceiveThread->m_bAutoDelete = FALSE; 
	m_pcwtSerialReceiveThread->ResumeThread();



}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCOMInbiolab::StopInbiolabThread( void )
{
	//if(m_bDoSerialSendThread)
	//{
	//	m_bDoSerialSendThread=false;
	//	//DEBUGMSG(TRUE, (TEXT("StopSerialThread 5\r\n")));
	//	if (WaitForSingleObject(m_pcwtSerialSendThread->m_hThread,2000) == WAIT_TIMEOUT)
	//	{
	//		DEBUGMSG(TRUE, (TEXT("TerminateThread 1 !!!!!!!!!!!!!!\r\n")));

	//		if(!TerminateThread(m_pcwtSerialSendThread,0))
	//		{

	//		}
	//	}
	//}

	//if(m_bDoSerialCheckThread)
	//{
	//	m_bDoSerialCheckThread=false;
	//	eventCheckPara.SetEvent();
	//	g_eventCOMCheckData.SetEvent();
	//	//DEBUGMSG(TRUE, (TEXT("StopSerialThread 1\r\n")));
	//	if (WaitForSingleObject(m_pcwtSerialCheckThread->m_hThread,1000) == WAIT_TIMEOUT)
	//	{
	//		DEBUGMSG(TRUE, (TEXT("TerminateThread 2 !!!!!!!!!!!!!!\r\n")));

	//		if(!TerminateThread(m_pcwtSerialCheckThread,0))
	//		{

	//		}

	//	}
	//}


	if(m_bDoSerialReceiveThread)
	{
		DWORD dwCommModemStatus;

		m_bDoSerialReceiveThread=false;

		GetCommMask(m_comPort.m_hCommPort,&dwCommModemStatus);
		BOOL fSuccess = SetCommMask (m_comPort.m_hCommPort,dwCommModemStatus);

		PurgeComm(m_comPort.m_hCommPort, PURGE_RXABORT | PURGE_RXCLEAR);

		if (WaitForSingleObject(m_pcwtSerialReceiveThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			DEBUGMSG(TRUE, (TEXT("TerminateThread 3 !!!!!!!!!!!!!!\r\n")));
			theApp.getLog()->WriteLine(_T("#THR:002a"));

			if(!TerminateThread(m_pcwtSerialReceiveThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:002b"));
			}

		}
		//DEBUGMSG(TRUE, (TEXT("StopSerialThread 4\r\n")));
	}

	//DEBUGMSG(TRUE, (TEXT("StopSerialThread exit\r\n")));
}

//#####################################ReceiveThread########################################
// **************************************************************************
// 
// **************************************************************************
static UINT CSerialReceiveThread( LPVOID pc )
{
	((CInterfaceCOMInbiolab*)pc)->ReceiveSerialData();
	return TRUE;
}

// **************************************************************************
// 
// **************************************************************************
DWORD CInterfaceCOMInbiolab::ReceiveSerialData(void) 
{
	CeSetThreadPriority(m_pcwtSerialReceiveThread->m_hThread,130);

	/*const int JUNK = 32;
	char temp[JUNK] = {0};*/

	DWORD dwCommModemStatus;

	if (m_comPort.m_hCommPort == INVALID_HANDLE_VALUE) 
	{
		// Handle the error. 
		return 0;
	}
	WORD i=0;
	//int size = 1;
	BYTE chRead;
	// Set the event mask. 
	SetCommMask (m_comPort.m_hCommPort, EV_RXCHAR | EV_CTS | EV_DSR | EV_BREAK | EV_ERR);

	while (m_comPort.m_hCommPort != INVALID_HANDLE_VALUE  && m_bDoSerialReceiveThread) 
	{
		// Wait for an event to occur for the port.
		WaitCommEvent (m_comPort.m_hCommPort, &dwCommModemStatus, 0);

		if(m_bDoSerialReceiveThread==true)
		{
			SetCommMask (m_comPort.m_hCommPort, EV_RXCHAR | EV_CTS | EV_DSR | EV_BREAK | EV_ERR);

			if ((dwCommModemStatus & EV_RXCHAR) && m_bDoSerialReceiveThread) 
			{
				fdwRecvBufLen = 1;

				while (fdwRecvBufLen>0 && m_bDoSerialReceiveThread) 
				{
					fdwRecvBufLen = m_comPort.Read(faRecvBuf,sizeof(faRecvBuf)-1);
					//fdwRecvBufLen = m_comPort.Read(faRecvBuf,1);

					if (fdwRecvBufLen > 0) 
					{
						for (i = 0; i < fdwRecvBufLen; i++)
						{
							chRead = faRecvBuf[i];
							//chRead = faRecvBuf[0];

							//DEBUGMSG(TRUE, (TEXT("%X "),chRead));

							switch (feRecvState)
							{
							case COM_INBIOLAB_SOF:

								fwCalcChkSum = 0xffff;
								fwChkSumUpper = 0;
								fwChkSumLower = 0;
								m_bCEflag=false;
								fiCntIdx=0;
								fiCntIdx_1=0;
								fiCntIdx_2=0;
								fiCntIdx_3=0;
								fiCntIdx_4=0;
								fiSample0=0;
								fiSample0_1=0;
								fiSample0_2=0;
								fiSample0_3=0;
								fiSample1=0;
								fiSample1_1=0;
								fiSample1_2=0;
								fiSample1_3=0;
								fiSample2=0;
								fiSample2_1=0;
								fiSample2_2=0;
								fiSample2_3=0;
								fiSample3=0;
								fiSample3_1=0;
								fiSample3_2=0;
								fiSample3_3=0;
								fiSample4=0;
								fiSample4_1=0;
								fiSample4_2=0;
								fiSample4_3=0;
								fiSample5=0;
								fiSample5_1=0;
								fiSample5_2=0;
								fiSample5_3=0;
								fiSample6=0;
								fiSample6_1=0;
								fiSample6_2=0;
								fiSample6_3=0;

								switch (chRead)
								{
								case INBIOLAB_MSG_SOF:
									feRecvState = COM_INBIOLAB_TYPE;
									break;

								default:
									// forget the rest
									feRecvState = COM_INBIOLAB_SOF;
									//return 0;
								} // switch (chRead)
								break;

							case COM_INBIOLAB_TYPE:
								
								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_TYPE;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);
									

									fwFrameType = chRead;
									//DEBUGMSG(TRUE, (TEXT("Frame %d "),fwFrameType));
									feRecvState = COM_INBIOLAB_CNT1;
									break;
								} // switch (chRead)
								
								break;

							case COM_INBIOLAB_CNT1:

								/*fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);
								m_iTestCnt++;*/

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_CNT1;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_CNT2;
									fiCntIdx_1=chRead;
									break;
								} // switch (chRead)
								
								break;

							case COM_INBIOLAB_CNT2:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_CNT2;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_CNT3;
									fiCntIdx_2=chRead;
									break;
								} // switch (chRead)
								
								break;

							case COM_INBIOLAB_CNT3:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_CNT3;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_CNT4;
									fiCntIdx_3=chRead;
									break;
								} // switch (chRead)

								break;

							case COM_INBIOLAB_CNT4:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_CNT4;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D5_0;//COM_INBIOLAB_D0_0;
									fiCntIdx_4=chRead;
									fiCntIdx=(fiCntIdx_4 << 24)+(fiCntIdx_3 << 16)+(fiCntIdx_2 << 8)+fiCntIdx_1;

									if(fiCntIdx-fiCntIdxOld>1)
									{
										DEBUGMSG(TRUE, (TEXT(".")));
									}
									fiCntIdxOld=fiCntIdx;

									break;
								} // switch (chRead)

								break;
							//sample 0
							case COM_INBIOLAB_D0_0:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D0_0;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D0_1;
									//fiSample0_1=chRead;
									break;
								} // switch (chRead)

								
								break;

							case COM_INBIOLAB_D0_1:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D0_1;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D0_2;
									//fiSample0_2=chRead;
									break;
								} // switch (chRead)
								
								break;

							case COM_INBIOLAB_D0_2:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D0_2;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D1_0;
									/*fiSample0_3=chRead;
									fiSample0=(fiSample0_3 << 16)+(fiSample0_2 << 8)+fiSample0_1;*/
									break;
								} // switch (chRead)

								break;
							//sample 1
							case COM_INBIOLAB_D1_0:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D1_0;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D1_1;
									//fiSample1_1=chRead;
									break;
								} // switch (chRead)
								
								break;

							case COM_INBIOLAB_D1_1:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D1_1;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D1_2;
									//fiSample1_2=chRead;
									break;
								} // switch (chRead)
								
								break;

							case COM_INBIOLAB_D1_2:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D1_2;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D2_0;
									/*fiSample1_3=chRead;
									fiSample1=(fiSample1_3 << 16)+(fiSample1_2 << 8)+fiSample1_1;*/
									break;
								} // switch (chRead)
								
								break;

							//sample 2
							case COM_INBIOLAB_D2_0:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D2_0;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D2_1;
									//fiSample2_1=chRead;
									break;
								} // switch (chRead)
								
								break;

							case COM_INBIOLAB_D2_1:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D2_1;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D2_2;
									//fiSample2_2=chRead;
									break;
								} // switch (chRead)

								break;

							case COM_INBIOLAB_D2_2:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D2_2;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D3_0;
									/*fiSample2_3=chRead;
									fiSample2=(fiSample2_3 << 16)+(fiSample2_2 << 8)+fiSample2_1;*/
									break;
								} // switch (chRead)

								break;

							//sample 3
							case COM_INBIOLAB_D3_0:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D3_0;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D3_1;
									//fiSample3_1=chRead;
									break;
								} // switch (chRead)
								
								break;

							case COM_INBIOLAB_D3_1:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D3_1;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D3_2;
									//fiSample3_2=chRead;
									break;
								} // switch (chRead)
								
								break;

							case COM_INBIOLAB_D3_2:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D3_2;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D4_0;
									/*fiSample3_3=chRead;
									fiSample3=(fiSample3_3 << 16)+(fiSample3_2 << 8)+fiSample3_1;*/
									break;
								} // switch (chRead)

								break;

							//sample 4
							case COM_INBIOLAB_D4_0:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D4_0;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D4_1;
									//fiSample4_1=chRead;
									break;
								} // switch (chRead)
								break;
								
							case COM_INBIOLAB_D4_1:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D4_1;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D4_2;
									//fiSample4_2=chRead;
									break;
								} // switch (chRead)
								
								break;

							case COM_INBIOLAB_D4_2:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D4_2;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D5_0;
									/*fiSample4_3=chRead;
									fiSample4=(fiSample4_3 << 16)+(fiSample4_2 << 8)+fiSample4_1;*/
									break;
								} // switch (chRead)

								break;

							//sample 5
							case COM_INBIOLAB_D5_0:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D5_0;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D5_1;
									fiSample5_1=chRead;
									//DEBUGMSG(TRUE, (TEXT("EMG5_1 %d "),fiSample5_1));
									break;
								} // switch (chRead)
								
								break;

							case COM_INBIOLAB_D5_1:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D5_1;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D5_2;
									fiSample5_2=chRead;
									//DEBUGMSG(TRUE, (TEXT("EMG5_2 %d "),fiSample5_2));
									break;
								} // switch (chRead)

								break;

							case COM_INBIOLAB_D5_2:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D5_2;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D6_0;
									fiSample5_3=chRead;
									fiSample5=(fiSample5_3 << 16)+(fiSample5_2 << 8)+fiSample5_1;
									break;
								} // switch (chRead)
								
								break;

							//sample 6
							case COM_INBIOLAB_D6_0:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D6_0;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D6_1;
									fiSample6_1=chRead;
									break;
								} // switch (chRead)
								
								break;

							case COM_INBIOLAB_D6_1:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D6_1;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_D6_2;
									fiSample6_2=chRead;
									break;
								} // switch (chRead)
								
								break;

							case COM_INBIOLAB_D6_2:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_D6_2;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									feRecvState = COM_INBIOLAB_CHKSUM1;
									fiSample6_3=chRead;
									fiSample6=(fiSample6_3 << 16)+(fiSample6_2 << 8)+fiSample6_1;
									break;
								} // switch (chRead)
								
								break;

							case COM_INBIOLAB_CHKSUM1:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_CHKSUM1;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									//fwChkSumLower = (WORD)chRead;
									feRecvState = COM_INBIOLAB_CHKSUM2;
									break;
								} // switch (chRead)
								
								break;

							case COM_INBIOLAB_CHKSUM2:

								switch (chRead)
								{
								case INBIOLAB_MSG_CE:
									m_bCEflag=true;
									feRecvState = COM_INBIOLAB_CHKSUM2;
									break;

								default:
									if(m_bCEflag)
									{
										m_bCEflag=false;

										//complement bit 5
										chRead^=(unsigned char)0x20;
									}
									fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

									//fwChkSumUpper = (WORD)chRead;
									//WORD testCh=((fwChkSumUpper << 8) + fwChkSumLower);

									//// wenn die Prüfsumme stimmt, weitermachen
									if (GOOD_FCS == fwCalcChkSum)
									{
										PerformMsg();
										//DEBUGMSG(TRUE, (TEXT(".")));
									}
									else
									{
										//error checksum
										//int blub = 1;
										DEBUGMSG(TRUE, (TEXT("_")));
									}

									feRecvState = COM_INBIOLAB_EOF;
									break;
								} // switch (chRead)
								

								//// TEST: fwCalcChkSum = (fwChkSumUpper << 8) + fwChkSumLower;
								/*if ((WORD)((fwChkSumUpper << 8) | fwChkSumLower) == fwChkSum)
									{
										PerformMsg();
									}
									else
									{
										sprintf(__msg, "OnEvent, GET_CHKSUM_2: checksum wrong (recv: %04X, calc: %04X)",
											((fwChkSumUpper << 8) + fwChkSumLower), fwChkSum);
										theApp.pLogMsg(__msg);
									}*/

								
								//faFcsBuf[0]=c;
								break;

							case COM_INBIOLAB_EOF:

								switch (chRead)
								{
								case INBIOLAB_MSG_EOF:
									// TEST: fwCalcChkSum = (fwChkSumUpper << 8) + fwChkSumLower;
									// wenn die Prüfsumme stimmt, weitermachen
									

									//if (((fwChkSumUpper << 8) + fwChkSumLower) == fwCalcChkSum)
									//	// if (true)
									//{
									//	PerformMsg();
									//}
									//else
									//{
									//	int blub = 1;
									//}
									feRecvState = COM_INBIOLAB_SOF;
									break;

								default:
									// forget the rest
									feRecvState = COM_INBIOLAB_SOF;
									//return 0;
								} // switch (faRecvBuf[i])
								break;

							default:
								feRecvState = COM_INBIOLAB_SOF;
								//return 0;
							} // switch (feRecvState)
							//Sleep(0);
						} // for i

						Sleep(0);
					}
					Sleep(0);
					
				}
				//int size = m_comPort.ReadCommand(temp,sizeof(temp)-1);

				//if (size > 0 && m_bDoSerialReceiveThread)
				//{
				//	/*if(ParseControllerCommand(CTlsBlob((BYTE*)temp,size))==true && m_bDoSerialReceiveThread)
				//	{
				//		g_eventCOM.SetEvent();
				//	}*/
				//}
			}
		}
	}

	DEBUGMSG(TRUE, (TEXT("END ReceiveSerialData\r\n")));
	theApp.getLog()->WriteLine(_T("#THR:002"));

	return 0;
}

int CInterfaceCOMInbiolab::Deinit()
{
	//DEBUGMSG(TRUE, (TEXT("CInterfaceCOMInbiolab::Deinit()\r\n")));
	////m_binitialized=false;
	//m_bProcessing=false;
	//
	////m_bStartup=false;
	//stopCOMcheckThread();
	//stopCOMThread();

	//
	//g_eventCOMIFData.SetEvent();

	//StopListener(-1);
	//Close();

	StopInbiolabThread();

	return 0;
}

bool CInterfaceCOMInbiolab::OpenCOM()
{
	bool bRes=false;





//ZUTTER
	//HANDLE hCOM;
	//hCOM= CreateFile(_T("COM2:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );

	//if( hCOM == INVALID_HANDLE_VALUE )
	//	return 0;

	//DCB dcb;
	//GetCommState(hCOM, &dcb);
	////dcb.fRtsControl = RTS_CONTROL_TOGGLE;
	//dcb.BaudRate=115200;
	//dcb.StopBits = 0;
	//dcb.Parity = 0;
	//dcb.ByteSize = 8;
	//SetCommState(hCOM, &dcb);

	//COMMPROP CommProp;
	//GetCommProperties(hCOM, &CommProp);

	//COMMTIMEOUTS to;
	//to.ReadIntervalTimeout = 1;
	//to.ReadTotalTimeoutConstant = 10000;
	//to.ReadTotalTimeoutMultiplier = 1;
	//to.WriteTotalTimeoutConstant = 5;
	//to.WriteTotalTimeoutMultiplier = 1;
	//SetCommTimeouts(hCOM, &to);

	//COMSTAT cs;

	//do
	//{
	//	BYTE r=0x31;
	//	DWORD n;
	//	DWORD err;  


	//	const int X=(3*512);
	//	BYTE bb[X];
	//	memset(&bb,0x44,X);
	//	while(1)
	//	{
	//		WriteFile(hCOM, &bb, X, &n, NULL);
	//		Sleep(400);
	//	}
	//}













	
	
	LONG    lLastError = Open(_T("COM9:"),0,0);
	

	if (lLastError != ERROR_SUCCESS)
	{
		if(ERROR_ALREADY_INITIALIZED==lLastError)
		{
			
		}
		//return ShowError(GetLastError(), _T("Unable to open COM-port"));
		return bRes;
	}

	// Setup the serial port (9600,8N1, which is the default setting)
	//lLastError = Setup(CSerial::EBaud230400,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
	lLastError = Setup(CSerial::EBaud115200,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
	if (lLastError != ERROR_SUCCESS)
	{
		DEBUGMSG(TRUE, (TEXT("OpenCOM Unable to set COM-port setting\r\n")));
		//return ShowError(GetLastError(), _T("Unable to set COM-port setting"));
		return bRes;
	}
	else
	{
		bRes=true;
	}

	DEBUGMSG(TRUE, (TEXT("OpenCOM success\r\n")));

	updateLastCOMCmd();

	return bRes;

}

int CInterfaceCOMInbiolab::ShowError (LONG lError, LPCTSTR lptszMessage)
{
	// Generate a message text
	TCHAR tszMessage[256];
	wsprintf(tszMessage,_T("#HFO test: %s (code %d)"), lptszMessage, lError);

	theApp.getLog()->WriteLine(tszMessage);
	// Display message-box and return with an error-code
	//::MessageBox(0,tszMessage,_T("Listener"), MB_ICONSTOP|MB_OK);
	return 1;
}
void CInterfaceCOMInbiolab::StartupSequenz()
{
	StartListener();

	startCOMcheckThread();
	//getModuleVersion();

	//m_cfgBoardMode=SPO2_MASIMO_HOSTCMD_BOARDMODE_NORMAL;
	//m_cfgBoardMode=25;
	//setBoardMode(SPO2_MASIMO_HOSTCMD_BOARDMODE_NORMAL);
	
}

void CInterfaceCOMInbiolab::InitializeSequenz()
{
	//todo init


	//startCOMcheckThread();
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceCOMInbiolab::checkModuleState()
{
	checkLastCOMCmd();
}

bool CInterfaceCOMInbiolab::PerformMsg()
{
	CString szError=_T("");
	bool bNewData=false;
	
	//updateLastCOMCmd();

	/*PBUFEMG _pBuf;
	_pBuf.iValEMG=fiSample5;*/

	//DEBUGMSG(TRUE, (TEXT("EMG5 %d "),fiSample5));
	
	if(fiSample6!=fiTrigger)
	{
		fiTrigger=fiSample6;

		if(fiTrigger==1)
		{
			m_bTrigger=true;
		}
		else
		{
			m_bTrigger=false;
		}

		bNewData=true;
		
		//DEBUGMSG(TRUE, (TEXT("EMG6 %d "),fiTrigger));
	}
	
	SHORT iTempfaWaveBuf=fiSample5;

	if(m_cLastWaveVal!=iTempfaWaveBuf)
	{
		doInterpolation(iTempfaWaveBuf);
	}

	/*EnterCriticalSection(&getModel()->getDATAHANDLER()->csEMGDataBuffer);
	getModel()->getDATAHANDLER()->m_rbufEMG.write(_pBuf);
	LeaveCriticalSection(&getModel()->getDATAHANDLER()->csEMGDataBuffer);*/

	if(bNewData)
		g_eventCOMIFData.SetEvent();
	//switch(fwDataSequence)
	//{
	//	//slave messages
	//	case 0:
	//		{
	//			//PBUFSPO2 _pBuf;
	//			if(bNewData)
	//				g_eventCOMIFData.SetEvent();

	//		}
	//		break;
	//	default:
	//		{
	//			//bResult = false;
	//			
	//			/*CStringW szTest=_T("");
	//			szTest.Format(_T("Unknown seuqnce %X\n"), fwDataSequence);
	//			TRACE(szTest);*/

	//		}
	//		break;
	//}

	

	

	
	return true;
}

void CInterfaceCOMInbiolab::doInterpolation(SHORT iTempfaWaveBuf)
{
	SHORT iDiff=((iTempfaWaveBuf-m_cLastWaveVal)/2);
	
	//SHORT iTemp=m_cLastWaveValW0;
	PBUFEMG _pBuf;
	_pBuf.iValEMG=m_cLastWaveVal;

	/*EnterCriticalSection(&getModel()->getDATAHANDLER()->csEMGDataBuffer);

	for(int i=0;i<2;i++)
	{
		getModel()->getDATAHANDLER()->m_rbufEMG.write(_pBuf);
		_pBuf.iValEMG+=iDiff;
	}
	LeaveCriticalSection(&getModel()->getDATAHANDLER()->csEMGDataBuffer);*/
	

	

	m_cLastWaveVal=iTempfaWaveBuf;
}


void CInterfaceCOMInbiolab::OnEvent(EEvent eEvent, EError eError)
{
	WORD i;
	BYTE chRead;
	// Handle data receive event
	if (eEvent & CSerial::EEventRecv)
	{
		do
		{
			// Read data from the COM-port
			Read(faRecvBuf, sizeof(faRecvBuf)-1,&fdwRecvBufLen);  //buffer auf 1 setzen!!!!!!!!!!!!!!!!!!!!!!!!

			for (i = 0; i < fdwRecvBufLen; i++)
			{
				chRead = faRecvBuf[i];

				//DEBUGMSG(TRUE, (TEXT("%X "),chRead));

				switch (feRecvState)
				{
				case COM_INBIOLAB_SOF:

					fwCalcChkSum = 0xffff;
					fwChkSumUpper = 0;
					fwChkSumLower = 0;
					m_bCEflag=false;
					fiCntIdx=0;
					fiCntIdx_1=0;
					fiCntIdx_2=0;
					fiCntIdx_3=0;
					fiCntIdx_4=0;
					fiSample0=0;
					fiSample0_1=0;
					fiSample0_2=0;
					fiSample0_3=0;
					fiSample1=0;
					fiSample1_1=0;
					fiSample1_2=0;
					fiSample1_3=0;
					fiSample2=0;
					fiSample2_1=0;
					fiSample2_2=0;
					fiSample2_3=0;
					fiSample3=0;
					fiSample3_1=0;
					fiSample3_2=0;
					fiSample3_3=0;
					fiSample4=0;
					fiSample4_1=0;
					fiSample4_2=0;
					fiSample4_3=0;
					fiSample5=0;
					fiSample5_1=0;
					fiSample5_2=0;
					fiSample5_3=0;
					fiSample6=0;
					fiSample6_1=0;
					fiSample6_2=0;
					fiSample6_3=0;

					switch (faRecvBuf[i])
					{
					case INBIOLAB_MSG_SOF:
						feRecvState = COM_INBIOLAB_TYPE;
						break;

					default:
						// forget the rest
						feRecvState = COM_INBIOLAB_SOF;
						return;
					} // switch (chRead)
					break;

				case COM_INBIOLAB_TYPE:
					
					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_TYPE;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);
						

						fwFrameType = chRead;
						//DEBUGMSG(TRUE, (TEXT("Frame %d "),fwFrameType));
						feRecvState = COM_INBIOLAB_CNT1;
						break;
					} // switch (chRead)
					
					break;

				case COM_INBIOLAB_CNT1:

					/*fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);
					m_iTestCnt++;*/

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_CNT1;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_CNT2;
						fiCntIdx_1=chRead;
						break;
					} // switch (chRead)
					
					break;

				case COM_INBIOLAB_CNT2:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_CNT2;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_CNT3;
						fiCntIdx_2=chRead;
						break;
					} // switch (chRead)
					
					break;

				case COM_INBIOLAB_CNT3:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_CNT3;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_CNT4;
						fiCntIdx_3=chRead;
						break;
					} // switch (chRead)

					break;

				case COM_INBIOLAB_CNT4:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_CNT4;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D5_0;//COM_INBIOLAB_D0_0;
						fiCntIdx_4=chRead;
						fiCntIdx=(fiCntIdx_4 << 24)+(fiCntIdx_3 << 16)+(fiCntIdx_2 << 8)+fiCntIdx_1;

						if(fiCntIdx-fiCntIdxOld>1)
						{
							DEBUGMSG(TRUE, (TEXT(".")));
						}
						fiCntIdxOld=fiCntIdx;

						break;
					} // switch (chRead)

					break;
				//sample 0
				case COM_INBIOLAB_D0_0:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D0_0;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D0_1;
						//fiSample0_1=chRead;
						break;
					} // switch (chRead)

					
					break;

				case COM_INBIOLAB_D0_1:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D0_1;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D0_2;
						//fiSample0_2=chRead;
						break;
					} // switch (chRead)
					
					break;

				case COM_INBIOLAB_D0_2:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D0_2;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D1_0;
						/*fiSample0_3=chRead;
						fiSample0=(fiSample0_3 << 16)+(fiSample0_2 << 8)+fiSample0_1;*/
						break;
					} // switch (chRead)

					break;
				//sample 1
				case COM_INBIOLAB_D1_0:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D1_0;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D1_1;
						//fiSample1_1=chRead;
						break;
					} // switch (chRead)
					
					break;

				case COM_INBIOLAB_D1_1:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D1_1;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D1_2;
						//fiSample1_2=chRead;
						break;
					} // switch (chRead)
					
					break;

				case COM_INBIOLAB_D1_2:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D1_2;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D2_0;
						/*fiSample1_3=chRead;
						fiSample1=(fiSample1_3 << 16)+(fiSample1_2 << 8)+fiSample1_1;*/
						break;
					} // switch (chRead)
					
					break;

				//sample 2
				case COM_INBIOLAB_D2_0:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D2_0;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D2_1;
						//fiSample2_1=chRead;
						break;
					} // switch (chRead)
					
					break;

				case COM_INBIOLAB_D2_1:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D2_1;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D2_2;
						//fiSample2_2=chRead;
						break;
					} // switch (chRead)

					break;

				case COM_INBIOLAB_D2_2:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D2_2;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D3_0;
						/*fiSample2_3=chRead;
						fiSample2=(fiSample2_3 << 16)+(fiSample2_2 << 8)+fiSample2_1;*/
						break;
					} // switch (chRead)

					break;

				//sample 3
				case COM_INBIOLAB_D3_0:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D3_0;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D3_1;
						//fiSample3_1=chRead;
						break;
					} // switch (chRead)
					
					break;

				case COM_INBIOLAB_D3_1:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D3_1;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D3_2;
						//fiSample3_2=chRead;
						break;
					} // switch (chRead)
					
					break;

				case COM_INBIOLAB_D3_2:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D3_2;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D4_0;
						/*fiSample3_3=chRead;
						fiSample3=(fiSample3_3 << 16)+(fiSample3_2 << 8)+fiSample3_1;*/
						break;
					} // switch (chRead)

					break;

				//sample 4
				case COM_INBIOLAB_D4_0:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D4_0;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D4_1;
						//fiSample4_1=chRead;
						break;
					} // switch (chRead)
					break;
					
				case COM_INBIOLAB_D4_1:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D4_1;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D4_2;
						//fiSample4_2=chRead;
						break;
					} // switch (chRead)
					
					break;

				case COM_INBIOLAB_D4_2:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D4_2;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D5_0;
						/*fiSample4_3=chRead;
						fiSample4=(fiSample4_3 << 16)+(fiSample4_2 << 8)+fiSample4_1;*/
						break;
					} // switch (chRead)

					break;

				//sample 5
				case COM_INBIOLAB_D5_0:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D5_0;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D5_1;
						fiSample5_1=chRead;
						//DEBUGMSG(TRUE, (TEXT("EMG5_1 %d "),fiSample5_1));
						break;
					} // switch (chRead)
					
					break;

				case COM_INBIOLAB_D5_1:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D5_1;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D5_2;
						fiSample5_2=chRead;
						//DEBUGMSG(TRUE, (TEXT("EMG5_2 %d "),fiSample5_2));
						break;
					} // switch (chRead)

					break;

				case COM_INBIOLAB_D5_2:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D5_2;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D6_0;
						fiSample5_3=chRead;
						fiSample5=(fiSample5_3 << 16)+(fiSample5_2 << 8)+fiSample5_1;
						break;
					} // switch (chRead)
					
					break;

				//sample 6
				case COM_INBIOLAB_D6_0:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D6_0;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D6_1;
						fiSample6_1=chRead;
						break;
					} // switch (chRead)
					
					break;

				case COM_INBIOLAB_D6_1:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D6_1;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_D6_2;
						fiSample6_2=chRead;
						break;
					} // switch (chRead)
					
					break;

				case COM_INBIOLAB_D6_2:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_D6_2;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						feRecvState = COM_INBIOLAB_CHKSUM1;
						fiSample6_3=chRead;
						fiSample6=(fiSample6_3 << 16)+(fiSample6_2 << 8)+fiSample6_1;
						break;
					} // switch (chRead)
					
					break;

				case COM_INBIOLAB_CHKSUM1:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_CHKSUM1;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						//fwChkSumLower = (WORD)chRead;
						feRecvState = COM_INBIOLAB_CHKSUM2;
						break;
					} // switch (chRead)
					
					break;

				case COM_INBIOLAB_CHKSUM2:

					switch (chRead)
					{
					case INBIOLAB_MSG_CE:
						m_bCEflag=true;
						feRecvState = COM_INBIOLAB_CHKSUM2;
						break;

					default:
						if(m_bCEflag)
						{
							m_bCEflag=false;

							//complement bit 5
							chRead^=(unsigned char)0x20;
						}
						fwCalcChkSum = AddByteToChkSum(fwCalcChkSum, chRead);

						//fwChkSumUpper = (WORD)chRead;
						//WORD testCh=((fwChkSumUpper << 8) + fwChkSumLower);

						//// wenn die Prüfsumme stimmt, weitermachen
						if (GOOD_FCS == fwCalcChkSum)
						{
							PerformMsg();
							//DEBUGMSG(TRUE, (TEXT(".")));
						}
						else
						{
							//error checksum
							//int blub = 1;
							DEBUGMSG(TRUE, (TEXT("_")));
						}

						feRecvState = COM_INBIOLAB_EOF;
						break;
					} // switch (chRead)
					

					//// TEST: fwCalcChkSum = (fwChkSumUpper << 8) + fwChkSumLower;
					/*if ((WORD)((fwChkSumUpper << 8) | fwChkSumLower) == fwChkSum)
						{
							PerformMsg();
						}
						else
						{
							sprintf(__msg, "OnEvent, GET_CHKSUM_2: checksum wrong (recv: %04X, calc: %04X)",
								((fwChkSumUpper << 8) + fwChkSumLower), fwChkSum);
							theApp.pLogMsg(__msg);
						}*/

					
					//faFcsBuf[0]=c;
					break;

				case COM_INBIOLAB_EOF:

					switch (faRecvBuf[i])
					{
					case INBIOLAB_MSG_EOF:
						// TEST: fwCalcChkSum = (fwChkSumUpper << 8) + fwChkSumLower;
						// wenn die Prüfsumme stimmt, weitermachen
						

						//if (((fwChkSumUpper << 8) + fwChkSumLower) == fwCalcChkSum)
						//	// if (true)
						//{
						//	PerformMsg();
						//}
						//else
						//{
						//	int blub = 1;
						//}
						feRecvState = COM_INBIOLAB_SOF;
						break;

					default:
						// forget the rest
						feRecvState = COM_INBIOLAB_SOF;
						return;
					} // switch (faRecvBuf[i])
					break;

				default:
					return;
				} // switch (feRecvState)
			} // for i


		}
		while (fdwRecvBufLen > 0);

	}

	return;
}

// Polynom für die CRC-16 Berechnung (nach CCITT)
//   X^16 + x^12 + x^5 + 1
// Dabei wird bei CRC16 immer implizit von x^16 ausgegangen,
// also: x^16+x^12+x^5+1 also 0b1.0001.0000.0010.0001 = 11021.
//#define   POLYNOMIAL   0x1021
//WORD crc_table[256];

WORD CInterfaceCOMInbiolab::AddByteToChkSum(WORD wActChkSum, unsigned char bByte)
{
	//WORD i, carry;
	//WORD lwSum;

	//lwSum = wActChkSum;
	//lwSum ^= (WORD)bByte << 8;
	//for (i = 0; i < 8; i++)
	//{
	//	if (carry = ((lwSum & 0x8000) != 0))
	//	{
	//		lwSum ^= cwGeneratorPolynom;
	//	}
	//	lwSum = (lwSum << 1) + carry;
	//} // for i

	wActChkSum = (wActChkSum >> 8) ^ fcsTable[(wActChkSum ^ bByte) & 0xff];

	return wActChkSum;
}
//void CInterfaceCOMInbiolab::InitCRC_Table(void)
//{
//	WORD b, v;
//	signed char i;
//
//	for (b = 0; b <= 255; ++b)
//	{
//		for ( v = b << 8, i = 8; --i >= 0;)
//		{
//			if ( (v & 0x8000) != 0x0000 )
//				v = ( v << 1 ) ^POLYNOMIAL;
//			else
//				v = v << 1;
//		} // for v
//		crc_table[b] = v;
//	} // for b
//}

// **************************************************************************
// 
// **************************************************************************
//DWORD CInterfaceCOMInbiolab::SendSPO2Command(BYTE byte0, BYTE byte1, BYTE byte2, BYTE byte3, BYTE byte4, BYTE byte5, BYTE byte6) 
//{
//	//if(getModel()->getDATAHANDLER()->IsCO2interfaceEnabled())
//	{
//		BYTE checksum=0;
//
//		LPSPO2MSG msg = new SPO2MSG;
//
//		BYTE byTmpBuffer[7];
//		byTmpBuffer[0]=byte0;
//		byTmpBuffer[1]=byte1;
//		byTmpBuffer[2]=byte2;
//		byTmpBuffer[3]=byte3;
//		byTmpBuffer[4]=byte4;
//		byTmpBuffer[5]=byte5;
//		byTmpBuffer[6]=byte6;
//
//		int iCount=0;
//		msg->byBuffer[iCount]=0x02;
//		iCount++;
//
//		//int sum1 = 0;
//		//int sum2 = 0;
//		for(int i=0;i<NUM_DATABYTES_MASIMO_SPO2;i++)
//		{
//			msg->byBuffer[iCount]=byTmpBuffer[i];
//			checksum += byTmpBuffer[i];
//			//sum1 += byTmpBuffer[i];
//			//sum2 += byTmpBuffer[i];
//			iCount++;
//		}
//
//		
//		//modulo 256 sum
//		//sum1 %= 256;
//		//sum2 &= 255;
//
//		/*BYTE ch1 = sum1;
//		BYTE ch2 = sum2;
//		BYTE ch3 = checksum;*/
//
//		
//
//		msg->byBuffer[iCount]=checksum;
//		iCount++;
//
//		msg->byBuffer[iCount]=0x03;
//		iCount++;
//
//		msg->bySize=iCount;
//		int iStop=0;
//
//		//int iCount=0;
//		//msg->byBuffer[iCount]=0x85;xxxxxx
//		//iCount++;
//
//		//msg->byBuffer[iCount]=Anzahlbytes;	//command byte + data bytes
//		//checksum ^= Anzahlbytes;
//		//iCount++;
//
//		//msg->byBuffer[iCount]=CO2_command;
//		//checksum ^= CO2_command;
//		//iCount++;
//
//		//int iBytesReadin=Anzahlbytes-1;
//
//		//for(int i=0;i<iBytesReadin;i++)
//		//{
//		//	if(byTmpBuffer[i]==0x80)
//		//	{
//		//		msg->byBuffer[iCount]=0x80;
//		//		checksum ^= 0x80;
//		//		iCount++;
//		//		msg->byBuffer[iCount]=0x00;
//		//		checksum ^= 0x00;
//		//	}
//		//	else if(byTmpBuffer[i]==0x85)
//		//	{
//		//		msg->byBuffer[iCount]=0x80;
//		//		checksum ^= 0x80;
//		//		iCount++;
//		//		msg->byBuffer[iCount]=0x05;
//		//		checksum ^= 0x05;
//		//	}
//		//	else
//		//	{
//		//		msg->byBuffer[iCount]=byTmpBuffer[i];
//		//		checksum ^= byTmpBuffer[i];
//		//	}
//		//	iCount++;
//		//}
//
//		//msg->byBuffer[iCount]=checksum;
//		//iCount++;
//
//
//		//msg->bySize=iCount;
//
//
//		send(msg);
//
//
//	}
//	return 0;
//}


bool CInterfaceCOMInbiolab::isStateOk()
{
	return m_bProcessing;
}

