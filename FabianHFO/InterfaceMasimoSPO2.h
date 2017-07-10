#pragma once
#include "InterfaceSPO2.h"

typedef enum
{
	SPO2_MASIMO_SOM,
	SPO2_MASIMO_SEQ,
	SPO2_MASIMO_D1,
	SPO2_MASIMO_D2,
	SPO2_MASIMO_W0,
	SPO2_MASIMO_W1,
	SPO2_MASIMO_W2,
	SPO2_MASIMO_W3,
	SPO2_MASIMO_W4,
	SPO2_MASIMO_W5,
	SPO2_MASIMO_W6,
	SPO2_MASIMO_W7,
	SPO2_MASIMO_CHKSUM,
	SPO2_MASIMO_EOM,
} ReceiverStatesMasimoSPO2;


class CInterfaceMasimoSPO2: public CInterfaceSPO2
{
public:
	CInterfaceMasimoSPO2(void);
	~CInterfaceMasimoSPO2(void);

	int Init(BYTE com);
	int Deinit(bool bReinit);

	bool OpenCOM();

	//void StartupSequenz();
	void InitializeSequenz();

	void checkModuleState();

	void set_FastSATon(bool bFastSATon);
	void set_SensitivityMode(eSPO2sensitivitymode mode);
	void set_SPO2averagingTime(eSPO2averagingtime avtime);
	void set_SPO2alarmDelay(eSPO2alarmdelay delay);

	WORD get_ProductID();
	WORD get_VersionMCU();
	WORD get_VersionDSP();
	SHORT get_EXCEPTION2();

	bool PerformMsg();

	void OnEvent (EEvent eEvent, EError eError);

	//void getModuleConfig(BYTE byte0);
	//void setModuleConfig(BYTE byte0, BYTE byte1, BYTE byte2, BYTE byte3, BYTE byte4, BYTE byte5, BYTE byte6);

	void setBoardMode(BYTE byte);
	void setDigitalWaveformConfiguration_WC0(WORD waveSource, BYTE waveScaling, BYTE waveDataSize);
	void setDigitalWaveformConfiguration_WC1(WORD waveSource, BYTE waveScaling, BYTE waveDataSize);
	void sendDigitalWaveformConfigurations();
	void setSpO2AveragingTime(BYTE byte);
	void setAlgorithmMode(bool bit0, bool bit1, bool bit2);
	void setWaveformMode(BYTE byte);
	bool isStateOk();
	void doInterpolationW0(SHORT iTempfaWaveBuf);

	void checkSENSORFAILURE();
	void checkEXCEPTIONS2();
	

protected:
	DWORD SendSPO2Command(	BYTE byte0=0, 
		BYTE byte1=0, 
		BYTE byte2=0, 
		BYTE byte3=0, 
		BYTE byte4=0, 
		BYTE byte5=0, 
		BYTE byte6=0);

	int ShowError (LONG lError, LPCTSTR lptszMessage);

protected:
	BYTE	faRecvBuf[256];
	BYTE	faDataBuf[2];
	char	faWaveBuf[8];

	SHORT m_cLastWaveValW0;
	SHORT m_cLastWaveValW1;

	DWORD			fdwRecvBufLen;
	ReceiverStatesMasimoSPO2	feRecvState;
	BYTE			fwCalcChkSum;
	BYTE			fwDataSequence;
	BYTE			fwPrevSequence;//rkuNEWFIX
	BYTE			fwReadChkSum;
	BYTE m_byCmdBuffer[1001];
	
	BYTE m_cfgBoardMode;						
	BYTE m_cfgDigitalWaveformConfiguration_WC0;// used for RawIR
	BYTE m_cfgDigitalWaveformConfiguration_WC1;//used for Signal IQ
	BYTE m_cfgSpO2AveragingTime;				
	BYTE m_cfgAlgorithmMode;					
	BYTE m_cfgWaveformMode;		

	bool m_bCheck_BoardMode;
	bool m_bCheck_DigitalWaveformConfigurationWC0;
	bool m_bCheck_SpO2AveragingTime;
	bool m_bCheck_AlgorithmMode;
	bool m_bCheck_tWaveformMode;

	SHORT m_iEXCEPTIONS2;
	SHORT m_iSENSORFAILURE;

	WORD m_iProductID;
	WORD m_iVersionDSP;
	WORD m_iVersionMCU;

	bool m_bProcessing;

	bool m_bFastSATon;
	eSPO2sensitivitymode m_eSPO2SensitivityMode;
	eSPO2alarmdelay m_eSPO2alarmdelay;
	eSPO2averagingtime m_eSPO2averagingTime;
};
