#pragma once
#include "InterfaceAcuLink.h"
#include "AcuLink_DLL.h"

// CInterfaceAcuLink_SERIAL

class CInterfaceAcuLink_SERIAL : public CInterfaceAcuLink
{

public:
	CInterfaceAcuLink_SERIAL();
	virtual ~CInterfaceAcuLink_SERIAL();

	bool init();
	bool deinit();
	//HINSTANCE isAcuLink();
	//int sendWaveData();
	int sendMeasurementData();
	int setMeasurementData(int idx, int iValue);
	void setParaData(UINT index, int val);
	void setAlarmData(UINT index, int val);
	void setGraphDataV3(SHORT iValVolume ,SHORT iValPressure ,SHORT iValFlow, SHORT iValCO2);
	void setGraphDataV4(SHORT iValVolume ,SHORT iValPressure ,SHORT iValFlow, SHORT iValCO2, SHORT iValSPO2);

protected:
	int getParaData(int idx);
	int getAlarmData(int idx);
	int getMeasurementData(int idx);

	void initCRC_Table(void);
	WORD calculateCRC(unsigned char *msg, WORD  cnt);

	int buildValue(char *sResult, int iVal, int iNk, int iLinkType);
	int setPoint( char *s, char *news, int nk );

	void sendTestWaveData();
	
protected:
	CTlsComPort m_comPort;

};