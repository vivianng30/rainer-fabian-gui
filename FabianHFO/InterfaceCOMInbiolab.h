#pragma once
#include "InterfaceCOMport.h"

typedef enum
{
	COM_INBIOLAB_SOF,
	COM_INBIOLAB_TYPE,
	COM_INBIOLAB_CNT1,
	COM_INBIOLAB_CNT2,
	COM_INBIOLAB_CNT3,
	COM_INBIOLAB_CNT4,
	COM_INBIOLAB_D0_0,
	COM_INBIOLAB_D0_1,
	COM_INBIOLAB_D0_2,
	COM_INBIOLAB_D1_0,
	COM_INBIOLAB_D1_1,
	COM_INBIOLAB_D1_2,
	COM_INBIOLAB_D2_0,
	COM_INBIOLAB_D2_1,
	COM_INBIOLAB_D2_2,
	COM_INBIOLAB_D3_0,
	COM_INBIOLAB_D3_1,
	COM_INBIOLAB_D3_2,
	COM_INBIOLAB_D4_0,
	COM_INBIOLAB_D4_1,
	COM_INBIOLAB_D4_2,
	COM_INBIOLAB_D5_0,
	COM_INBIOLAB_D5_1,
	COM_INBIOLAB_D5_2,
	COM_INBIOLAB_D6_0,
	COM_INBIOLAB_D6_1,
	COM_INBIOLAB_D6_2,
	COM_INBIOLAB_CHKSUM1,
	COM_INBIOLAB_CHKSUM2,
	COM_INBIOLAB_EOF,
} ReceiverStatesInbiolab;

class CInterfaceCOMInbiolab: public CInterfaceCOMport
{
public:
	CInterfaceCOMInbiolab(void);
	~CInterfaceCOMInbiolab(void);

	int Init(BYTE com);
	int Deinit();

	bool OpenCOM();

	void StartupSequenz();
	void InitializeSequenz();

	void checkModuleState();

	bool PerformMsg();
	bool isStateOk();

	void OnEvent (EEvent eEvent, EError eError);

	
	void doInterpolation(SHORT iTempfaWaveBuf);

protected:

	WORD AddByteToChkSum(WORD wActChkSum, unsigned char bByte);

	int ShowError (LONG lError, LPCTSTR lptszMessage);

protected:


	CTlsComPort m_comPort;

	friend UINT CSerialReceiveThread(LPVOID pc);
	CWinThread*	m_pcwtSerialReceiveThread;
	DWORD ReceiveSerialData(void);
	bool m_bDoSerialReceiveThread;
	HANDLE m_hThreadSerialReceive;

	void StartInbiolabThread(void);
	void StopInbiolabThread( void );


	//BYTE	faRecvBuf[128];
	BYTE	faRecvBuf[64];
	//BYTE	faRecvBuf[32];
	//BYTE	faRecvBuf[4096];
	//BYTE	faRecvBuf[1024];
	UINT	fiCntIdxOld;
	
	UINT	fiCntIdx;
	UINT	fiCntIdx_1;
	UINT	fiCntIdx_2;
	UINT	fiCntIdx_3;
	UINT	fiCntIdx_4;
	int		fiSample0;
	int		fiSample0_1;
	int		fiSample0_2;
	int		fiSample0_3;
	int		fiSample1;
	int		fiSample1_1;
	int		fiSample1_2;
	int		fiSample1_3;
	int		fiSample2;
	int		fiSample2_1;
	int		fiSample2_2;
	int		fiSample2_3;
	int		fiSample3;
	int		fiSample3_1;
	int		fiSample3_2;
	int		fiSample3_3;
	int		fiSample4;
	int		fiSample4_1;
	int		fiSample4_2;
	int		fiSample4_3;
	int		fiSample5;
	int		fiSample5_1;
	int		fiSample5_2;
	int		fiSample5_3;
	int		fiSample6;
	int		fiSample6_1;
	int		fiSample6_2;
	int		fiSample6_3;
	
	int		fiTrigger;
	/*char	faSample0Buf[3];
	char	faSample1Buf[3];
	char	faSample2Buf[3];
	char	faSample3Buf[3];
	char	faSample4Buf[3];
	char	faSample5Buf[3];
	char	faSample6Buf[3];
	char	faFcsBuf[2];*/
	
	bool m_bProcessing;
	bool m_bCEflag;
	

	DWORD			fdwRecvBufLen;
	ReceiverStatesInbiolab	feRecvState;
	WORD			fwCalcChkSum;

	WORD			fwChkSumUpper;		// WORD entspricht unsigned short (= 16 bit)
	WORD			fwChkSumLower;
	
	BYTE			fwFrameType;

	
	//UINT m_iTestCnt;
	//BYTE m_byCmdBuffer[1001];
	
};
